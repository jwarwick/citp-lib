#include "Peer.h"
#include "CITPDefines.h"
#include "PacketCreator.h"
#include "Fixture.h"

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
  m_tcpSocket = new QTcpSocket(this);
  Q_CHECK_PTR(m_tcpSocket);

  connect(m_tcpSocket, SIGNAL(connected()),
	  this, SIGNAL(connectedToPeer()));
  connect(m_tcpSocket, SIGNAL(disconnected()),
	  this, SIGNAL(disconnectedFromPeer()));

  qDebug() << "Peer::Peer(), name=" << m_peerName;
  qDebug() << "Peer::Peer(), state=" << m_peerState;
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
      QByteArray byteArray = m_tcpSocket->readAll();
      qDebug() << "Read Data:" << byteArray[0] << byteArray[1] << byteArray[2] << byteArray[3]; 

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

  if (citpHeader->VersionMinor != 0x00)
    {
      qDebug() << "parsePacket: invalid VersionMinor:" << citpHeader->VersionMinor;
      return;
    }

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

  qDebug() << "parseFPTCPacket:" << fixId << universe << channel << channelCount << makeString << nameString;

  m_fixtureList.append(fix);

  emit updatedFixtureList();
}
