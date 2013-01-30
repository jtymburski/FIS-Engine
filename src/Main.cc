#include <QtGui/QApplication>
#include <QDebug>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow mw;

  return app.exec();
}
