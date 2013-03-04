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

  /* Enforce a fixed size and remove close button */
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  /* Do the final show once everything is set up */
  show();
}

/* Destructor function */
Application::~Application()
{
}

void Application::closeEvent(QCloseEvent* event)
{
  close();
  event->ignore();
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
  Category* bloodclaw_scion = new Category("Blooclaw Scion", 30, 30, 30, 30, 30,
                                           30, 30, 30, 30, 30, 30, 30, 30, 30,
                                           300, 30, 5, 5, 2, 300, 1500, 1500,
                                           1500, 1500, 1500, 1500, 1500, 1500,
                                           1500, 1500, 1500, 1500, 1500, 15000,
                                           286, 150, 150, 75);

  Person* main_character = new Person("Malgidus", human, bloodclaw_scion, "CYA", "POB");
  Person* arcadius = new Person("Arcadius", human, bloodclaw_scion, "CYB", "POS");
  Person* ladybear = new Person("RT Guy", human, bloodclaw_scion, "CYA", "POB");
  Person* aurumba = new Person("Jesze", human, bloodclaw_scion, "CYA", "POB");
  Person* berfriegg = new Person("Atkst", human, bloodclaw_scion, "CYA", "POB");
  Person* cloud_dude = new Person("Cloud Dude", human, bloodclaw_scion, "CYA", "POB");
  Person* cloud_dude2 = new Person("Cloud Dude", human, bloodclaw_scion, "CYA", "POB");
  Person* cloud_dude3 = new Person("Cloud Dude", human, bloodclaw_scion, "CYA", "POB");
  Person* ember = new Person("Ember", human, bloodclaw_scion, "CYA", "POB");
  Person* raven = new Person("Raven", human, bloodclaw_scion, "CYA", "POB");
  main_character->setLevel(14);
  arcadius->setLevel(121);
  ladybear->setLevel(22);
  aurumba->setLevel(1);
  berfriegg->setLevel(3);
  cloud_dude->setLevel(4);
  cloud_dude2->setLevel(14);
  cloud_dude3->setLevel(1);
  ember->setLevel(14);
  raven->setLevel(17);

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

  /* Level test
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

  // qDebug() << "Party sizes: " << enemy_clan->getPartySize() << " " << friend_clan->getPartySize();
  // qDebug() << "Party max sizes: " << enemy_clan->getMaxSize() << " " << friend_clan->getMaxSize();
  */

  /* Action Test
  Action* test = new Action("1,RAISE,THERMAL AGGRESSION,2.7,1,THERMAL,1,POLAR,1,0.76;");
  qDebug() << "Action ID:  " << test->getId();
  qDebug() << "Ailment:    " << test->getAilment();
  qDebug() << "Max & Min:  " << test->getMaximum() << " " << test->getMinimum();
  qDebug() << "Variance:   " << test->getVariance();
  qDebug() << "Base Chage: " << test->getBaseChange();
  qDebug() << "RAISE:      " << test->getActionFlag(Action::RAISE);
  qDebug() << "LOWER:      " << test->getActionFlag(Action::LOWER);
  qDebug() << "GIVE:       " << test->getActionFlag(Action::GIVE);
  qDebug() << "OFFENSIVE:  " << test->getActionFlag(Action::OFFENSIVE);
  qDebug() << "DEFENSIVE:  " << test->getActionFlag(Action::DEFENSIVE);
  qDebug() << "PHYSICAL:   " << test->getActionFlag(Action::PHYSICAL);
  qDebug() << "THERMAL:    " << test->getActionFlag(Action::THERMAL);
  qDebug() << "POLAR:      " << test->getActionFlag(Action::POLAR);
  qDebug() << "PRIMAL:     " << test->getActionFlag(Action::PRIMAL);
  qDebug() << "CHARGED:    " << test->getActionFlag(Action::CHARGED);
  qDebug() << "CYBERNETIL: " << test->getActionFlag(Action::CYBERNETIC);
  qDebug() << "NIHIL:      " << test->getActionFlag(Action::NIHIL);
  qDebug() << "VITALITY:   " << test->getActionFlag(Action::VITALITY);
  qDebug() << "QUANTM_DRV: " << test->getActionFlag(Action::QUANTUM_DRIVE);
  qDebug() << "MOMENTUML:  " << test->getActionFlag(Action::MOMENTUM);
  qDebug() << "LIMBERTUDE: " << test->getActionFlag(Action::LIMBERTUDE);
  qDebug() << "UNBRABILTY: " << test->getActionFlag(Action::UNBEARABILITY);
  qDebug() << "I ELMN ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_ELMN_ATK);
  qDebug() << "I PHYS ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_PHYS_ATK);
  qDebug() << "I THER ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_THER_ATK);
  qDebug() << "I POLA ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_POLA_ATK);
  qDebug() << "I PRIM ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_PRIM_ATK);
  qDebug() << "I CHAR ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_CHAR_ATK);
  qDebug() << "I CYBE ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_CYBE_ATK);
  qDebug() << "I NIHI ATK: " << test->getIgnoreAtkFlag(Action::IGNORE_NIHI_ATK);
  qDebug() << "I ATK:      " << test->getIgnoreAtkFlag(Action::IGNORE_ATK);
  qDebug() << "I ELMN DEF: " << test->getIgnoreDefFlag(Action::IGNORE_ELMN_DEF);
  qDebug() << "I PHYS DEF: " << test->getIgnoreDefFlag(Action::IGNORE_PHYS_DEF);
  qDebug() << "I THER DEF: " << test->getIgnoreDefFlag(Action::IGNORE_THER_DEF);
  qDebug() << "I POLA DEF: " << test->getIgnoreDefFlag(Action::IGNORE_POLA_DEF);
  qDebug() << "I PRIM DEF: " << test->getIgnoreDefFlag(Action::IGNORE_PRIM_DEF);
  qDebug() << "I CHAR DEF: " << test->getIgnoreDefFlag(Action::IGNORE_CHAR_DEF);
  qDebug() << "I CYBE DEF: " << test->getIgnoreDefFlag(Action::IGNORE_CYBE_DEF);
  qDebug() << "I NIHI DEF: " << test->getIgnoreDefFlag(Action::IGNORE_NIHI_DEF);
  qDebug() << "I  DEF:     " << test->getIgnoreDefFlag(Action::IGNORE_DEF);
  */
  test_battle = new Battle(friend_clan, enemy_clan); //TODO PARENTLESS =(

}

/*
 * Temporary map setup
 * DELETE when done
 */
void Application::setupMap()
{
  test_map = new Map();
}
