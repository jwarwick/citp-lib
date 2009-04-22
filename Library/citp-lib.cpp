#include "citp-lib.h"

#include "CITPDefines.h"
#include "PeerInformationSocket.h"

CITPLib::CITPLib(QObject *parent)
  : QObject(parent)
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

  return m_peerSocket->init(name, state);
}

bool CITPLib::listPeers(QList<PeerDescription*> &peerList)
{
  if (!m_peerSocket)
    {
      return false;
    }

  qDebug() << "citplib::listPeers()";
  return m_peerSocket->listPeers(peerList);
}
