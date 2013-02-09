/******************************************************************************
* Class Name: MainWindow
* Date Created: Jan 29, 2013
* Inheritance: QWidget
* Description: The MainWindow that handles all the displays for the entire
*              game. This includes the primary switching mechanism to switch
*              between widgets as needed according to the selection on the
*              screen.
******************************************************************************/
#include "MainWindow.h"

/* Constructor function */
MainWindow::MainWindow(QWidget* parent)
{
  title_screen = new TitleScreen(1216, 704);
  setupBattle();
  setupMap();

  widget_stack = new QStackedWidget();
  widget_stack->addWidget(test_battle);
  widget_stack->addWidget(test_map);
  widget_stack->addWidget(title_screen);

  widget_stack->setCurrentIndex(2); // TODO (0=battle, 1=map, 2=titlescreen)

  setCentralWidget(widget_stack);
  setFocus(Qt::OtherFocusReason);
  setFocusProxy(title_screen);

  QObject::connect(title_screen, SIGNAL(closing()), this, SLOT(close()));

  show();
}

/* Destructor function */
MainWindow::~MainWindow()
{
}

void MainWindow::close()
{
  emit closing();
}

/* 
 * Temporary battle setup
 * DELETE when done
 */
void MainWindow::setupBattle()
{
  Person* main_character = new Person(5, 2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 500,
                                      100, 7, 7, 7, 12, 180,  "Malgidus");
  Person* arcadius = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 9500,
                                      100, 7, 7, 7, 12, 180,  "Arcadius");
  Person* ladybear = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 500,
                                      100, 7, 7, 7, 12, 180,  "Ladybear");
  Person* aurumba = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 175,
                                      100, 7, 7, 7, 12, 180,  "Aurumba");
  Person* berfriegg = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 2000,
                                      100, 7, 7, 7, 12, 180,  "Berfriegg");
  Person* cloud_dude = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 300, 80,
                                  6, 6, 6, 11, 160,"Cloud Guy 1");
  Person* cloud_dude2 = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 300, 80,
                                  6, 6, 6, 11, 160,"Cloud Guy 2");
  Person* cloud_dude3 = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 300, 80,
                                  6, 6, 6, 11, 160,"Cloud Guy 3");
  Person* cloud_dude4 = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 300, 80,
                                  6, 6, 6, 11, 160,"Cloud Guy 4");
  Person* turbo_fish = new Person(5,2,5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 5, 2, 300, 80,
                                  6, 6, 6, 11, 160,"Turbofish 1");

  Sprite* main_char_fp = new Sprite(":/A_player");
  Sprite* arcadius_fp = new Sprite(":/A_arcadius");
  Sprite* ladybear_fp = new Sprite(":/A_turbo_fish");
  Sprite* aurumba_fp = new Sprite(":/A_aurumba");
  Sprite* berfriegg_fp = new Sprite(":/A_berfriegg");
  Sprite* cloud_dude_tp = new Sprite(":/E_cloud_thing");
  Sprite* turbo_fish_tp = new Sprite(":/E_turbo_fish");
  main_character->setFirstPerson(main_char_fp);
  arcadius->setFirstPerson(arcadius_fp);
  ladybear->setFirstPerson(ladybear_fp);
  aurumba->setFirstPerson(aurumba_fp);
  berfriegg->setFirstPerson(berfriegg_fp);
  cloud_dude->setThirdPerson(cloud_dude_tp);
  cloud_dude2->setThirdPerson(cloud_dude_tp);
  cloud_dude3->setThirdPerson(cloud_dude_tp);
  cloud_dude4->setThirdPerson(cloud_dude_tp);
  turbo_fish->setThirdPerson(turbo_fish_tp);

  Party* friend_clan = new Party(main_character);
  friend_clan->addMember(arcadius);
  friend_clan->addMember(ladybear);
  friend_clan->addMember(aurumba);
  friend_clan->addMember(berfriegg);
  Party* enemy_clan = new Party(cloud_dude);
  enemy_clan->addMember(cloud_dude2);
  enemy_clan->addMember(cloud_dude3);
  enemy_clan->addMember(cloud_dude4);
  enemy_clan->addMember(turbo_fish);
  
  test_battle = new Battle(friend_clan, enemy_clan);
}

/*
 * Temporary map setup
 * DELETE when done
 */
void MainWindow::setupMap()
{
  test_map = new Map();
}
