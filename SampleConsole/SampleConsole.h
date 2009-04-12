#ifndef _SAMPLECONSOLE_H_
#define _SAMPLECONSOLE_H_

#include <QMainWindow>

#include "ui_SampleConsole.h"

class CITPLib;

class SampleConsole : public QMainWindow
{
     Q_OBJECT
public:
  SampleConsole(QWidget *parent = 0);
  virtual ~SampleConsole();				    

private slots:
  void on_actionExit_triggered();

private:
  Ui::SampleConsole ui;

  CITPLib *m_citp;

};

#endif // _SAMPLECONSOLE_H_
