#include "Peer.h"
#include "CITPDefines.h"
#include "PacketCreator.h"
#include "Fixture.h"
#include "FixtureModel.h"

#include <QTcpSocket>

Peer::Peer(const QHostAddress &host, quint16 listeningPort,
       const QString &peerName, const QString &peerState, QObject *parent)
  : QObject(parent),
    m_peerType(UNKNOWN_PEER),
    m_peerState(peerState),
    m_peerName(peerName),
    m_host(host),
    m_listeningPort(listeningPort)
{
  m_fixtureModel = new FixtureModel(this);
  Q_CHECK_PTR(m_fixtureModel);

  m_tcpSocket = new QTcpSocket(this);
  Q_CHECK_PTR(m_tcpSocket);

  connect(m_tcpSocket, SIGNAL(connected()),
	  this, SIGNAL(connectedToPeer()));
  connect(m_tcpSocket, SIGNAL(disconnected()),
	  this, SIGNAL(disconnectedFromPeer()));
  connect(m_tcpSocket, SIGNAL(readyRead()),
	  this, SLOT(handleReadyRead()));

}

Peer::~Peer()
{
  if (m_tcpSocket)
    {
      m_tcpSocket->close();
    }
}

PeerType Peer::peerType() const
{
  return m_peerType;
}

QString Peer::peerName() const
{
  return m_peerName;
}

QString Peer::peerState() const
{
  return m_peerState;
}

QString Peer::peerHost() const
{
  return m_host.toString();
}

quint16 Peer::peerListeningPort() const
{
  return m_listeningPort;
}

bool Peer::connectToPeer()
{
  if (0 == m_listeningPort)
    {
      qDebug() << "Peer::connect() - Peer is not listening for connections";
      return false;
    }

  if (!m_tcpSocket)
    {
      return false;
    }
  
  m_tcpSocket->connectToHost(m_host, m_listeningPort);

  // XXX - use waitForConnected() ?

  return true;
}

bool Peer::sendPacket(const unsigned char *buffer, int bufferLen)
{
  if (!m_tcpSocket)
    {
      return false;
    }
  
  if (QAbstractSocket::ConnectedState != m_tcpSocket->state())
    {
      qDebug() << "Peer::sendPacket() - Socket not connected";
      return false;
    }

  if (bufferLen != m_tcpSocket->write((const char*)buffer, bufferLen))
    {
      qDebug() << "Peer::sendPacket() write failed:" << m_tcpSocket->error();
      return false;
    }

  return true;
}

void Peer::handleReadyRead()
{
  if (!m_tcpSocket)
    {
      return;
    }

  while(m_tcpSocket->bytesAvailable())
    {

      QByteArray peekArray = m_tcpSocket->peek(12);
      if (12 != peekArray.size())
	{
	  return;
	}

      if (peekArray[0] != 'C' &&
	  peekArray[1] != 'I' &&
	  peekArray[2] != 'T' &&
	  peekArray[3] != 'P')
	{
	  // remove the first byte and try again
	  QByteArray b = m_tcpSocket->read(1);
	  qDebug() << "Discarding byte:" << b[0];
	  continue;
	}

      quint32 messageSize = 0;
      messageSize = peekArray[11];
      messageSize <<= 8;
      messageSize |= peekArray[10];
      messageSize <<= 8;
      messageSize |= peekArray[9];
      messageSize <<= 8;
      messageSize |= peekArray[8];

      qDebug() << "Found message size:" << messageSize;
      
      if (messageSize > m_tcpSocket->bytesAvailable())
	{
	  qDebug() << "Not enough bytes available, only have:" << m_tcpSocket->bytesAvailable();
	  return;
	}

      QByteArray byteArray = m_tcpSocket->read(messageSize);
      qDebug() << "Read Data:" << byteArray[0] << byteArray[1] << byteArray[2] << byteArray[3]; 

      qDebug() << "byteArray size:" << byteArray.size();

      parsePacket(byteArray);

    }
}

void Peer::parsePacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_Header *citpHeader = (struct CITP_Header*)data;

  // CITP header
  if (citpHeader->Cookie != COOKIE_CITP)
    {
      qDebug() << "parsePacket: invalid Cookie" << citpHeader->Cookie;
      return;
    }

  if (citpHeader->VersionMajor != 0x01)
    {
      qDebug() << "parsePacket: invalid VersionMajor:" << citpHeader->VersionMajor;
      return;
    }

  // XXX - this coming back as 0x01 during patch operations?
  /*
  if (citpHeader->VersionMinor != 0x00)
    {
      qDebug() << "parsePacket: invalid VersionMinor:" << citpHeader->VersionMinor;
      return;
    }
  */
  //packet->bufferLen;
  //packet->MessagePartCount = 0x01;
  //packet->MessagePart = 0x01; // XXX - doc says 0-based?
  
  switch (citpHeader->ContentType)
    {
    case COOKIE_PINF:
      qDebug() << "parsePacket: got COOKIE_PINF packet on TCP Socket...";
      return;
      break;

    case COOKIE_FPTC:
      parseFPTCPacket(byteArray);
      break;

    case COOKIE_FSEL:
      parseFSELPacket(byteArray);
      break;

    default:
      qDebug() << "parsePacket: unknown ContentType:" << citpHeader->ContentType;
      return;
    }
}

void Peer::parseFPTCPacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_FPTC_Header *fptcHeader = (struct CITP_FPTC_Header*)data;

  switch (fptcHeader->ContentType)
    {
    case COOKIE_FPTC_PTCH:
      parsePTCHPacket(byteArray);
      break;

    case COOKIE_FPTC_UPTC:
      parseUPTCPacket(byteArray);
      break;

    case COOKIE_FPTC_SPTC:
      parseSPTCPacket(byteArray);
      break;

    default:
      qDebug() << "parseFPTCPacket: unknown ContentType:" << fptcHeader->ContentType;
    }
}

void Peer::parseUPTCPacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_FPTC_UPtc *uptcPacket = (struct CITP_FPTC_UPtc*)data;

  quint16 fixtureCount = uptcPacket->FixtureCount;
  if (0 == fixtureCount)
    {
      emit unpatchAllFixtures();
      return;
    }

  QList<quint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FPTC_UPtc);
  for (int i=0; i<fixtureCount; ++i)
    {
      quint16 *fixId = (quint16*)(data + offset);
      fixtureIdentifiers.append(*fixId);
      offset += 2;
    }

  qDebug() << "parseUPTCPacket: Unpatch Fixtures:";
  foreach (quint16 id, fixtureIdentifiers)
    {
      qDebug() << "\tunpatch id:" << id;
    }

  emit unpatchFixtures(fixtureIdentifiers);
}

void Peer::parseSPTCPacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_FPTC_SPtc *sptcPacket = (struct CITP_FPTC_SPtc*)data;

  quint16 fixtureCount = sptcPacket->FixtureCount;
  if (0 == fixtureCount)
    {
      emit sendPatchAllFixtures();
      return;
    }

  QList<quint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FPTC_SPtc);
  for (int i=0; i<fixtureCount; ++i)
    {
      quint16 *fixId = (quint16*)(data + offset);
      fixtureIdentifiers.append(*fixId);
      offset += 2;
    }

  qDebug() << "parseSPTCPacket: Unpatch Fixtures:";
  foreach (quint16 id, fixtureIdentifiers)
    {
      qDebug() << "\tpatch id:" << id;
    }

  emit sendPatchFixtures(fixtureIdentifiers);
}

void Peer::parsePTCHPacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_FPTC_Ptch *ptchPacket = (struct CITP_FPTC_Ptch*)data;

  quint16 fixId = ptchPacket->FixtureIdentifier;
  quint8 universe = ptchPacket->Universe;
  quint16 channel = ptchPacket->Channel;
  quint16 channelCount = ptchPacket->ChannelCount;
  
  int offset = sizeof(struct CITP_FPTC_Ptch);
  QString makeString(data+offset);  

  offset += makeString.size() + 1;
  QString nameString(data+offset);

  Fixture *fix = new Fixture(fixId, universe, channel, channelCount, makeString, nameString, this);
  Q_CHECK_PTR(fix);

  qDebug() << "parseFPTCPacket Id:" << fixId << "universe:" << universe << "channel:" << 
    channel << "channelcount:" << channelCount << makeString << nameString;

  m_fixtureList.append(fix);
  m_fixtureModel->addFixture(fix);

  emit updatedFixtureList();
}

void Peer::parseFSELPacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_FSEL_Header *fselHeader = (struct CITP_FSEL_Header*)data;

  switch (fselHeader->ContentType)
    {
    case COOKIE_FSEL_SELE:
      parseSELEPacket(byteArray);
      break;

    case COOKIE_FSEL_DESE:
      parseDESEPacket(byteArray);
      break;

    default:
      qDebug() << "parseFSELPacket: unknown ContentType:" << fselHeader->ContentType;
    }
}

void Peer::parseSELEPacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_FSEL_Sele *selePacket = (struct CITP_FSEL_Sele*)data;

  quint16 fixtureCount = selePacket->FixtureCount;
  bool complete = selePacket->Complete;

  QList<quint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FSEL_Sele);
  for (int i=0; i<fixtureCount; ++i)
    {
      quint16 *fixId = (quint16*)(data + offset);
      fixtureIdentifiers.append(*fixId);
      offset += 2;
    }

  qDebug() << "parseSELEPacket: Select Fixtures:";
  foreach (quint16 id, fixtureIdentifiers)
    {
      qDebug() << "\tselect id:" << id;
    }

  emit selectFixtures(complete, fixtureIdentifiers);
}

void Peer::parseDESEPacket(const QByteArray &byteArray)
{
  const char *data = byteArray.constData();
  struct CITP_FSEL_DeSe *selePacket = (struct CITP_FSEL_DeSe*)data;

  quint16 fixtureCount = selePacket->FixtureCount;
  if (0 == fixtureCount)
    {
      emit deselectAllFixtures();
    }

  QList<quint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FSEL_DeSe);
  for (int i=0; i<fixtureCount; ++i)
    {
      quint16 *fixId = (quint16*)(data + offset);
      fixtureIdentifiers.append(*fixId);
      offset += 2;
    }

  qDebug() << "parseDESEPacket: Deselect Fixtures:";
  foreach (quint16 id, fixtureIdentifiers)
    {
      qDebug() << "\tdeselect id:" << id;
    }

  emit deselectFixtures(fixtureIdentifiers);
}
