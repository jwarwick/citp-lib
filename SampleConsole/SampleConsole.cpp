#include "SampleConsole.h"

#include "citp-lib.h"

#include <QtDebug>

SampleConsole::SampleConsole(QWidget *parent)
  : QMainWindow(parent)
{
  ui.setupUi(this);

  m_citp = new CITPLib(this);
  Q_CHECK_PTR(m_citp);
  connect(m_citp, SIGNAL(peersUpdated()),
	  this, SLOT(handlePeersUpdated()));

  qDebug() << "SampleConsole: about to create peer info socket";
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

  QList<struct PeerDescription*> peerList;
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
  foreach (struct PeerDescription *peer, peerList)
    {
      if (!peer)
	{
	  continue;
	}

      QString desc = tr("\t%1 %2 %3 %4:%5\n").arg(peer->m_name).arg(peer->m_state).arg(peer->m_type)
	.arg(peer->m_ip.toString()).arg(peer->m_port);
      ui.textEdit->appendPlainText(desc);

    }

}

void SampleConsole::on_connectButton_clicked()
{
}

