#include "SampleConsole.h"

#include "citp-lib.h"
#include "Visualizer.h"

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

  m_visualizer = (Visualizer*)(peers.at(1));
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

void SampleConsole::on_sendDataButton_clicked()
{
  static quint8 count = 128;

  if (m_visualizer)
    {
      bool blind = false;
      quint8 universeIndex = 0;
      quint16 firstChannelIndex = 0;
      quint16 channelCount = 24;
      quint8 *channelLevels = new quint8[channelCount];
      memset(channelLevels, count, channelCount);

      if (!m_visualizer->sendChannelData(blind, universeIndex, firstChannelIndex, 
					 channelCount, channelLevels))
	{
	  qDebug() << "send channel data failed";
	}

      delete[] channelLevels;
      count++;
    }
}
