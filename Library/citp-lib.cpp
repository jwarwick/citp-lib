#include "citp-lib.h"

#include <QtDebug>

#include "CITPDefines.h"
#include "PeerInformationSocket.h"
#include "Peer.h"

CITPLib::CITPLib(QObject *parent)
  : QObject(parent),
    m_peerSocket(NULL)
{
  
  m_peerSocket = new PeerInformationSocket(this);
  Q_CHECK_PTR(m_peerSocket);
  connect(m_peerSocket, SIGNAL(peersUpdated()),
	  this, SIGNAL(peersUpdated()));
}

CITPLib::~CITPLib()
{
}

bool CITPLib::createPeerInformationSocket(const QString &name, const QString &state)
{

  if (!m_peerSocket)
    {
      return false;
    }

  qDebug() << "about to call peer socket init";

  return m_peerSocket->init(name, state);
}

bool CITPLib::listPeers(QList<Peer*> &peerList)
{
  if (!m_peerSocket)
    {
      return false;
    }

  return m_peerSocket->listPeers(peerList);
}
