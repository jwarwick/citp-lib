#include <QApplication>

#include "SampleConsole.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  SampleConsole sampleConsole;
  sampleConsole.show();
  return app.exec();
}
