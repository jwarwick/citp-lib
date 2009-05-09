#include "SampleConsole.h"

#include "citp-lib.h"
#include "Visualizer.h"
#include "FixtureModel.h"

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

  connect(m_visualizer, SIGNAL(unpatchAllFixtures()),
	  this, SLOT(handleUnpatchAllFixtures()));
  connect(m_visualizer, SIGNAL(unpatchFixtures(const QList<quint16> &)),
	  this, SLOT(handleUnpatchFixtures(const QList<quint16> &)));
  connect(m_visualizer, SIGNAL(updatedFixtureList()),
	  this, SLOT(handleUpdatedFixtureList()));

  ui.tableView->setModel(m_visualizer->m_fixtureModel);
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

void SampleConsole::on_sendPatchButton_clicked()
{
  if (m_visualizer)
    {
      QList<quint16> fixList;
      if (!m_visualizer->sendPatchRequest(fixList))
	{
	  qDebug() << "send patch request failed";
	  return;
	}
    }
}

void SampleConsole::on_patchButton_clicked()
{
}

void SampleConsole::on_unpatchButton_clicked()
{
  if (m_visualizer)
    {
      QList<quint16> fixList;
      if (!m_visualizer->sendUnpatchMessage(fixList))
	{
	  qDebug() << "send unpatch message failed";
	  return;
	}
    }
}

void SampleConsole::on_selectPushButton_clicked()
{
  if (m_visualizer)
    {
      int id = ui.idSpinBox->value();
      bool complete = Qt::Checked == ui.completeCheckBox->checkState();
      QList<quint16> fixIds;
      fixIds.append(id);
      if (!m_visualizer->sendSelectMessage(complete, fixIds))
	{
	  qDebug() << "send select message failed";
	  return;
	}
    }
}

void SampleConsole::on_deselectPushButton_clicked()
{
  if (m_visualizer)
    {
      int id = ui.idSpinBox->value();
      QList<quint16> fixIds;
      fixIds.append(id);
      if (!m_visualizer->sendDeselectMessage(fixIds))
	{
	  qDebug() << "send deselect message failed";
	  return;
	}
    }
}

void SampleConsole::handleUnpatchAllFixtures()
{
  ui.textEdit->appendPlainText(tr("Got Unpatch All Fixtures signal"));
}

void SampleConsole::handleUnpatchFixures(const QList<quint16> &fixtureIdentifiers)
{
  ui.textEdit->appendPlainText(tr("Got Unpatch Fixture List signal"));
}

void SampleConsole::handleUpdatedFixtureList()
{
  ui.textEdit->appendPlainText(tr("Got Updated Fixture List signal"));
}

void SampleConsole::handleSelectFixtures(bool complete, const QList<quint16> &fixtureIdentifiers)
{
  ui.textEdit->appendPlainText(tr("Got Select Fixtures signal"));
}

void SampleConsole::handleDeselectAllFixtures()
{
  ui.textEdit->appendPlainText(tr("Got Deselect All Fixtures signal"));
}

void SampleConsole::handleDeselectFixtures(const QList<quint16> &fixtureIdentifiers)
{
  ui.textEdit->appendPlainText(tr("Got Deselect Fixtures signal"));
}
