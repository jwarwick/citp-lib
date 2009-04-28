#include "SampleConsole.h"

#include "citp-lib.h"
#include "Peer.h"

#include <QtDebug>

SampleConsole::SampleConsole(QWidget *parent)
  : QMainWindow(parent)
{
  ui.setupUi(this);

  m_citp = new CITPLib(this);
  Q_CHECK_PTR(m_citp);
  connect(m_citp, SIGNAL(peersUpdated()),
	  this, SLOT(handlePeersUpdated()));

  if (!m_citp->createPeerInformationSocket("Sample Console", "Testing..."))
    {
      qDebug() << "CreatePeerInformationSocket failed";
    }
}

SampleConsole::~SampleConsole()
{
}

void SampleConsole::on_actionExit_triggered()
{
  close();
}

void SampleConsole::handlePeersUpdated()
{
  qDebug() << "handlePeersUpdated()";

  QList<Peer*> peerList;
  if (m_citp)
    {
      if (!m_citp->listPeers(peerList))
	{
	  qDebug() << "listPeers() failed";
	  return;
	}
    }

  qDebug() << "Peer list length:" << peerList.size();

  ui.textEdit->appendPlainText("\n\nPeer List Updated:");
  foreach (const Peer *peer, peerList)
    {
      if (!peer)
	{
	  continue;
	}

      QString desc = tr("\t%1, %2 (%3:%4)\n").arg(peer->peerName()).arg(peer->peerState())
	.arg(peer->peerHost()).arg(peer->peerListeningPort());
      ui.textEdit->appendPlainText(desc);

    }

}

void SampleConsole::on_connectButton_clicked()
{
  QList<Peer*> peers;
  m_citp->listPeers(peers);
  if (2 != peers.size())
    {
      qDebug() << "not two peers";
      return;
    }

  m_visualizer = peers.at(1);
  if (!m_visualizer->connectToPeer())
    {
      qDebug() << "ConnectToPeer failed";
      return;
    }
}

void SampleConsole::on_sendNameButton_clicked()
{
  if (m_visualizer)
    {
      if (!m_visualizer->sendUniverseName(0, "universe 0"))
	{
	  qDebug() << "send universe name failed";
	  return;
	}
    }
}

