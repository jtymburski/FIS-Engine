#include <QtGui/QApplication>

//#include "Application.h"
#include "Game/Map/Map.h"
#include "Game/Battle/Battle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Application* game_app = new Application();
    // Map m;

    Battle b;

    return app.exec();
}
