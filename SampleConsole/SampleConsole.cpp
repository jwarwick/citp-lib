#include "SampleConsole.h"

#include "citp-lib.h"

#include <QtDebug>

SampleConsole::SampleConsole(QWidget *parent)
  : QMainWindow(parent)
{
  ui.setupUi(this);

  m_citp = new CITPLib(this);
  Q_CHECK_PTR(m_citp);

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
