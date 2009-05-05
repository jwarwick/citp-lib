#include "PeerInformationSocket.h"

#include <QTimer>
#include <QtDebug>

#include "citp-lib.h"

#include "CITPDefines.h"
#include "PacketCreator.h"
#include "Peer.h"
#include "Visualizer.h"

#ifdef Q_OS_WIN
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#endif

#define TRANSMIT_INTERVAL_MS  5000

PeerInformationSocket::PeerInformationSocket(QObject *parent)
  : QUdpSocket(parent),
    m_name(tr("Unknown name")),
    m_state(tr("Unknown state")),
    m_packetBuffer(NULL),
    m_packetBufferLen(0)
{
  
  m_timer = new QTimer(this);
  Q_CHECK_PTR(m_timer);
  connect(m_timer, SIGNAL(timeout()),
	  this, SLOT(transmitPLoc()));
  m_timer->setInterval(TRANSMIT_INTERVAL_MS);
}

PeerInformationSocket::~PeerInformationSocket()
{
  if (m_timer)
    {
      m_timer->stop();
    }
}

bool PeerInformationSocket::init(const QString &name, const QString &state)
{
  if (m_timer)
    {
      m_timer->stop();
    }

  m_name = name;
  m_state = state;

  // XXX - close socket if already opened?

  // create multicast socket, bind to port
  if (!bind(CITP_PINF_MULTICAST_PORT, ShareAddress | ReuseAddressHint))
    {
      qDebug() << "Multicast bind failed";
      return false;
    }
  
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(CITP_PINF_MULTICAST_IP);
  mreq.imr_interface.s_addr = INADDR_ANY;
  int r = ::setsockopt(socketDescriptor(), IPPROTO_IP, IP_ADD_MEMBERSHIP,
		       (const char *)&mreq, sizeof(struct ip_mreq));
  if (0 != r)
    {
      qDebug() << "setsockopt failed, r:" << r;
      return false;
    }


  delete m_packetBuffer;

  m_packetBuffer = PacketCreator::createPLocPacket(PacketCreator::LightingConsole, name, state, m_packetBufferLen);
  if (!m_packetBuffer)
    {
      m_packetBufferLen = 0;
      return false;
    }

  transmitPLoc();

  // XXX - don't connect this more than once..
  connect(this, SIGNAL(readyRead()),
  	  this, SLOT(handleReadReady()));

  
  if (m_timer)
    {
      m_timer->start();
    }
  

  return true;
}

void PeerInformationSocket::transmitPLoc()
{
  if (m_packetBuffer && m_packetBufferLen > 0)
    {
      QHostAddress addr(CITP_PINF_MULTICAST_IP);
      qint64 ret = writeDatagram((const char*)m_packetBuffer, m_packetBufferLen, 
				 addr, CITP_PINF_MULTICAST_PORT);
      if (-1 == ret)
	{
	  qDebug() << "Failed to send multicast packet:" << error();
	}
    }
}

void PeerInformationSocket::handleReadReady()
{
  while (hasPendingDatagrams()) 
    {
      QByteArray datagram;
      datagram.resize(pendingDatagramSize());
      QHostAddress sender;
      quint16 senderPort;

      if (-1 != readDatagram(datagram.data(), datagram.size(),
			     &sender, &senderPort))
	{
	  processPacket(sender, datagram);
	}
    }
}

void PeerInformationSocket::processPacket(const QHostAddress &address, const QByteArray &packetArray)
{
  const char *data = packetArray.constData();
  CITP_PINF_PLoc *packet = (CITP_PINF_PLoc*)data;

  // CITP header
  if (packet->CITPPINFHeader.CITPHeader.Cookie != COOKIE_CITP)
    {
      return;
    }

  if (packet->CITPPINFHeader.CITPHeader.VersionMajor != 0x01)
    {
      qDebug() << "Invalid VersionMajor value:" << packet->CITPPINFHeader.CITPHeader.VersionMajor;
      return;
    }

  if (packet->CITPPINFHeader.CITPHeader.VersionMinor != 0x00)
    {
      qDebug() << "Invalid VersionMinor value:" << packet->CITPPINFHeader.CITPHeader.VersionMinor;
      return;
    }
  //packet->CITPPINFHeader.CITPHeader.Reserved[0] = 0x00;
  //packet->CITPPINFHeader.CITPHeader.Reserved[1] = 0x00; 
  //packet->CITPPINFHeader.CITPHeader.MessageSize = bufferLen;
  //packet->CITPPINFHeader.CITPHeader.MessagePartCount = 0x01;
  //packet->CITPPINFHeader.CITPHeader.MessagePart = 0x01; // XXX - doc says 0-based?
  
  if (packet->CITPPINFHeader.CITPHeader.ContentType != COOKIE_PINF)
    {
      return;
    }

  // PINF header
  if (packet->CITPPINFHeader.ContentType != COOKIE_PINF_PLOC)
    {
      return;
    }

  // PLoc data
  quint16 listeningPort = packet->ListeningTCPPort;
  if (0 == listeningPort)
    {
      // XXX - uncomment if we don't want non-listeners
      //return;
    }
  
  // type
  int offset = sizeof(struct CITP_PINF_PLoc);
  QString typeString(packetArray.constData()+offset);
  //memcpy(buffer + offset, typeString.toAscii().constData(), typeString.size());
  
  // name
  offset += typeString.size() + 1;
  QString nameString(packetArray.constData()+offset);
  //memcpy(buffer + offset, name.toAscii().constData(), name.size());

  // state
  offset += nameString.size() + 1;
  QString stateString(packetArray.constData()+offset);
  //memcpy(buffer + offset, state.toAscii().constData(), state.size());

  // XXX - these strings are going out of scope because they are implicitly shared
  // from the original buffer

  addPeer(address, listeningPort, typeString, nameString, stateString);
}

void PeerInformationSocket::addPeer(const QHostAddress &host, quint16 listeningPort, 
				    const QString &typeString, 
				    const QString &name, const QString &state)
{
  // check if we already have this peer
  foreach (const Peer *peer, m_peerList)
    {
      if (peer)
	{
	  if (peer->peerHost() == host.toString() && peer->peerListeningPort() == listeningPort)
	    {
	      return;
	    }
	}
    }

  //  qDebug() << "Adding new peer to list:" << host.toString() << listeningPort << typeString << name << state;

  // add the newly discovered peer
  Peer *newPeer;
  if ("Visualizer" == typeString)
    {
      newPeer = new Visualizer(host, listeningPort, name, state);
    }
  else
    {
      newPeer = new Peer(host, listeningPort, name, state);
    }
  Q_CHECK_PTR(newPeer);
  m_peerList.append(newPeer);

  qDebug() << "added peer, peer list size:" << m_peerList.size();

  emit peersUpdated();
}

bool PeerInformationSocket::listPeers(QList<Peer*> &peerList)
{
  peerList = m_peerList;
  return true;
}
    
