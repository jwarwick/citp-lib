#include "Peer.h"

Peer::Peer(const QHostAddress &host, quint16 listeningPort, const QString &type, 
       const QString &name, const QString &state, QObject *parent)
  : QObject(parent),
    m_host(host),
    m_listeningPort(listeningPort),
    m_type(type),
    m_name(name),
    m_state(state)
{
}

Peer::~Peer()
{
}

QString Peer::peerName() const
{
  return m_name;
}
