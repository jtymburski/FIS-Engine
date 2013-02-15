/******************************************************************************
* Class Name: Application
* Date Created: Jan 29, 2013
* Inheritance: QWidget
* Description: The Main Application that handles all the displays for the 
*              entire game. This includes the primary switching mechanism to 
*              switch between widgets as needed according to the selection on 
*              the screen.
******************************************************************************/
#include "Application.h"

#include <QDebug>

/* Constructor function */
Application::Application(QWidget* parent)
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

  /* Set the widget location (center of the screen) */
  QDesktopWidget desktopWidget;
  QRect desktopRect(desktopWidget
						.availableGeometry(desktopWidget.primaryScreen()));
  setGeometry((desktopRect.width() - 1216) / 2, 
              (desktopRect.height() - 704) / 2, 1216, 704);

  show();
}

/* Destructor function */
Application::~Application()
{
}

void Application::close()
{
  emit closing();
}

void Application::switchWidget(int index)
{
  widget_stack->setCurrentIndex(index);

  if(index == 2)
    title_screen->playBackground();
}

/* 
 * Temporary battle setup
 * DELETE when done
 */
void Application::setupBattle()
{
  Race* human = new Race("Humans");
  Category* bloodclaw_scion = new Category("Blooclaw Scion", 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 300, 30, 5, 5, 2, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 15000, 286, 150, 150, 75);

  Person* main_character = new Person("Main Character", human, bloodclaw_scion, "CYA", "POB");
  Person* arcadius = new Person("Arcadius", human, bloodclaw_scion, "CYB", "POS");
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

  for (int i = 1; i < 125; i++)
  {
    arcadius->setLevel(i);
    qDebug() << "Level" << arcadius->getLevel();
    qDebug() << "Vitality: " << arcadius->getVitality();
    qDebug() << "Quantum Drive: " << arcadius->getQuantumDrive();
    qDebug() << "Phys Atk: " << arcadius->getPhysicalAggression();
    qDebug() << "Phys Def: " << arcadius->getPhysicalFortitude();
    qDebug() << "Ther Atk: " << arcadius->getThermalAggression();
    qDebug() << "Ther Def: " << arcadius->getThermalFortitude();
    qDebug() << "Cybe Atk: " << arcadius->getCyberneticAggression();
    qDebug() << "Cybe Def: " << arcadius->getCyberneticFortitude();
    qDebug() << "Pola Atk: " << arcadius->getPolarAggression();
    qDebug() << "Pola Def: " << arcadius->getPolarFortitude() << endl;
  }

  qDebug() << "Party sizes: " << enemy_clan->getPartySize() << " " << friend_clan->getPartySize();
  qDebug() << "Party max sizes: " << enemy_clan->getMaxSize() << " " << friend_clan->getMaxSize();
  test_battle = new Battle(friend_clan, enemy_clan);
}

/*
 * Temporary map setup
 * DELETE when done
 */
void Application::setupMap()
{
  test_map = new Map();
}
