#include <QtGui/QApplication>
#include "Application.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Application* game_app = new Application();

    return app.exec();
}
