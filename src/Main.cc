#include <QtGui/QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setApplicationName("Univursa Game");

  MainWindow mw;
  QObject::connect(&mw, SIGNAL(closing()), &app, SLOT(quit()));

  return app.exec();
}
