#include "PeerInformationSocket.h"

#include <QTimer>

#include "PacketCreator.h"

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

  // close socket if already opened

  // create multicast socket, bind to port

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
