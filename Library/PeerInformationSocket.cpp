#include "PeerInformationSocket.h"

#include <QTimer>
#include <QtDebug>

#include "CITPDefines.h"
#include "PacketCreator.h"

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

  connect(this, SIGNAL(readReady()),
	  this, SLOT(handleReadReady()));

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
      return false;
    }

  transmitPLoc();
  if (m_timer)
    {
      m_timer->start();
    }

  return true;
}

void PeerInformationSocket::transmitPLoc()
{
}

void PeerInformationSocket::handleReadReady()
{
}
