#include <QtGui/QApplication>
#include <QDebug>

//#include "Application.h"
#include "Game/Map/Map.h"
#include "Game/Battle/Battle.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  //Application* game_app = new Application();
  // Map m;

  Person* main_character = new Person(5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2,
                                      500, 100, 7, 7, 7, 12, 180,
                                      "Malgidus", "cyberneticA", "nihilB",
                                      "Sleuthmaster");
  Person* cloud_dude = new Person(5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2,
                                  300, 80, 6, 6, 6, 11, 160,
                                  "Cloud Guy 1", "primalB", "polarC",
                                  "Enemy");
  Sprite* main_char_fp = new Sprite(":/A_ladybear");
  Sprite* cloud_dude_tp = new Sprite(":E_cloud_thing");
  main_character->setFirstPerson(main_char_fp);
  cloud_dude->setThirdPerson(cloud_dude_tp);

  Party* friend_clan = new Party(main_character);
  Party* enemy_clan = new Party(cloud_dude);

  new Battle(friend_clan, enemy_clan);


    return app.exec();
}
