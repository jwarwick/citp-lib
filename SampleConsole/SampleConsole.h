#ifndef _SAMPLECONSOLE_H_
#define _SAMPLECONSOLE_H_

#include <QMainWindow>

#include "ui_SampleConsole.h"

class CITPLib;
class Peer;

class SampleConsole : public QMainWindow
{
     Q_OBJECT
public:
  SampleConsole(QWidget *parent = 0);
  virtual ~SampleConsole();				    

private slots:
  void on_actionExit_triggered();
  void on_connectButton_clicked();
  void on_sendNameButton_clicked();

  void handlePeersUpdated();

private:
  Ui::SampleConsole ui;

  CITPLib *m_citp;
  Peer *m_visualizer;

};

#endif // _SAMPLECONSOLE_H_
