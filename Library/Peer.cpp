#include "Peer.h"
#include "PacketCreator.h"

#include <QTcpSocket>

Peer::Peer(const QHostAddress &host, quint16 listeningPort,
       const QString &peerName, const QString &peerState, QObject *parent)
  : QObject(parent),
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

bool Peer::sendUniverseName(quint8 universeIndex, const QString &universeName)
{
  if (!m_tcpSocket)
    {
      return false;
    }
  
  if (QAbstractSocket::ConnectedState != m_tcpSocket->state())
    {
      qDebug() << "Peer::sendUniverseName() - Socket not connected";
      return false;
    }

  int bufferLen;
  unsigned char *buffer = PacketCreator::createUNamPacket(universeIndex, universeName, bufferLen);
  if (!buffer)
    {
      qDebug() << "createUNamPacket() failed";
      return false;
    }

  if (bufferLen != m_tcpSocket->write((const char*)buffer, bufferLen))
    {
      qDebug() << "sendUniverseName() write failed:" << m_tcpSocket->error();
      return false;
    }

  delete buffer;

  return true;
}

