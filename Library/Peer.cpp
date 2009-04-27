#include "Peer.h"


Peer::Peer(const QString &host, quint16 listeningPort, const QString &peerType, 
       const QString &peerName, const QString &peerState, QObject *parent)
  : QObject(parent),
    m_host(host),
    m_listeningPort(listeningPort),
    m_peerType(peerType),
    m_peerName(peerName),
    m_peerState(peerState)
{
  /*
  QString typeTmp(peerType.constData(), peerType.length());
  m_peerType = typeTmp;

  QString nameTmp(peerName.constData(), peerName.length());
  m_peerName = nameTmp;

  QString stateTmp(peerState.constData(), peerState.length());
  m_peerState = stateTmp;
  */

  qDebug() << "Peer::Peer(), name=" << m_peerName;
  qDebug() << "Peer::Peer(), type=" << m_peerType;
  qDebug() << "Peer::Peer(), state=" << m_peerState;
}

/*
Peer::Peer(QObject *parent)
  : QObject(parent),
    m_peerName("default constructor")
{
}
*/

Peer::~Peer()
{
}

/*
QString Peer::peerName() const
{
  return m_peerName;
}
*/
