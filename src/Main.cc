#include <QtGui/QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow mw;
  QObject::connect(&mw, SIGNAL(closing()), &app, SLOT(quit()));

  return app.exec();
}
