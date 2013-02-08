#include <QtGui/QApplication>
#include <QDebug>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow mw;
  QObject::connect(&app, SIGNAL(aboutToQuit()), &mw, SLOT(closing()));

  return app.exec();
}
