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

  QObject::connect(title_screen, SIGNAL(closing()), 
                   this,         SLOT(close()));
  QObject::connect(title_screen, SIGNAL(openingBattle(int)), 
                   this,         SLOT(switchWidget(int)));
  QObject::connect(title_screen, SIGNAL(openingMap(int)), 
                   this,         SLOT(switchWidget(int)));
  QObject::connect(test_map, SIGNAL(closingMap(int)), 
                   this,     SLOT(switchWidget(int)));
  QObject::connect(test_battle, SIGNAL(closingBattle(int)), 
                   this,        SLOT(switchWidget(int)));

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

void MainWindow::switchWidget(int index)
{
  widget_stack->setCurrentIndex(index);

  if(index == 2)
    title_screen->playBackground();
}

/* 
 * Temporary battle setup
 * DELETE when done
 */
void MainWindow::setupBattle()
{
  Race* human = new Race("Humans");
  Category* bloodclaw_scion = new Category("Blooclaw Scion");

  Person* main_character = new Person("Main Character", human, bloodclaw_scion, "CYA", "POB");
  Person* arcadius = new Person("Arcadius", human, bloodclaw_scion, "CYA", "POB");
  Person* ladybear = new Person("Arcadius", human, bloodclaw_scion, "CYA", "POB");
  Person* aurumba = new Person("Arcadius", human, bloodclaw_scion, "CYA", "POB");
  Person* berfriegg = new Person("Arcadius", human, bloodclaw_scion, "CYA", "POB");
  Person* cloud_dude = new Person("Arcadius", human, bloodclaw_scion, "CYA", "POB");
  Person* cloud_dude2 = new Person("Arcadius", human, bloodclaw_scion, "CYA", "POB");
  Person* cloud_dude3 = new Person("Arcadius", human, bloodclaw_scion, "CYA", "POB");
  Person* ember = new Person("Ember", human, bloodclaw_scion, "CYA", "POB");
  Person* raven = new Person("raven", human, bloodclaw_scion, "CYA", "POB");

  Sprite* main_char_fp = new Sprite(":/A_player");
  Sprite* arcadius_fp = new Sprite(":/A_arcadius");
  Sprite* ladybear_fp = new Sprite(":/A_turbo_fish");
  Sprite* aurumba_fp = new Sprite(":/A_aurumba");
  Sprite* berfriegg_fp = new Sprite(":/A_berfriegg");
  Sprite* cloud_dude_tp = new Sprite(":/E_cloud_thing");
  Sprite* ember_tp = new Sprite(":/E_ember");
  Sprite* raven_tp = new Sprite(":/E_draganaraven");
  main_character->setFirstPerson(main_char_fp);
  arcadius->setFirstPerson(arcadius_fp);
  ladybear->setFirstPerson(ladybear_fp);
  aurumba->setFirstPerson(aurumba_fp);
  berfriegg->setFirstPerson(berfriegg_fp);
  cloud_dude->setThirdPerson(cloud_dude_tp);
  cloud_dude2->setThirdPerson(cloud_dude_tp);
  cloud_dude3->setThirdPerson(cloud_dude_tp);
  ember->setThirdPerson(ember_tp);
  raven->setThirdPerson(raven_tp);

  Party* friend_clan = new Party(main_character);
  friend_clan->addMember(arcadius);
  friend_clan->addMember(ladybear);
  friend_clan->addMember(aurumba);
  friend_clan->addMember(berfriegg);
  Party* enemy_clan = new Party(cloud_dude);
  enemy_clan->addMember(ember);
  enemy_clan->addMember(raven);
  enemy_clan->addMember(cloud_dude2);
  enemy_clan->addMember(cloud_dude3);

  
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
