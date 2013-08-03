/******************************************************************************
* Class Name: Application
* Date Created: Jan 29, 2013
* Inheritance: QStackedWidget
* Description: The Main Application that handles all the displays for the 
*              entire game. This includes the primary switching mechanism to 
*              switch between widgets as needed according to the selection on 
*              the screen.
******************************************************************************/
#include "Application.h"

/* Constant Implementation - see header file for descriptions */
const short Application::kRESOLUTION_X = 1216;
const short Application::kRESOLUTION_Y = 704;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

Application::Application(QWidget* parent)
{
  setParent(parent);

  /* Set some of the options */
#ifdef unix
  vsync_enabled = false;
#else
  vsync_enabled = true;
#endif

  // TODO: make this dependent on focus of the game. If it loses focus, 
  // return the normal cursor (Qt::ArrowCursor);
  //setCursor(Qt::BlankCursor);
  //setCursor(Qt::ArrowCursor);

  title_screen = new TitleScreen(kRESOLUTION_X, kRESOLUTION_Y);
  //title_screen->show();

  setupBattle();
  //test_battle->show();
  
  /* Sets up the map format and map with vsync and double buffering forced on */
  /* TODO: add checking if OpenGL is not enabled */
  QGLFormat gl_format(QGL::SampleBuffers);
  gl_format.setDoubleBuffer(true);
  if(vsync_enabled)
    gl_format.setSwapInterval(1);
  else
    gl_format.setSwapInterval(0);
  test_map = new Map(gl_format, kRESOLUTION_X, kRESOLUTION_Y);
  //test_map->loadMap("maps/test_03");
  //test_map->getViewport()->show();

  /* Add widgets to the stack */
  addWidget(test_battle);
  addWidget(test_map); // TODO: should be viewport
  addWidget(title_screen);
  setCurrentIndex(2); // TODO (0=battle, 1=map, 2=titlescreen)

  /* Widget information for handling the game */
  title_screen->setFocus();
  setMaximumWidth(kRESOLUTION_X);
  setMaximumHeight(kRESOLUTION_Y);
  setMinimumWidth(kRESOLUTION_X);
  setMinimumHeight(kRESOLUTION_Y);

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
  setGeometry((desktopRect.width() - kRESOLUTION_X) / 2, 
              (desktopRect.height() - kRESOLUTION_Y) / 2, 
              kRESOLUTION_X, kRESOLUTION_Y);

  /* Do the final show once everything is set up */
  //showFullScreen();
}

Application::~Application()
{
  delete test_map;
  test_map = 0;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void Application::closeEvent(QCloseEvent* event)
{
  close();
  event->ignore();
}

/*============================================================================
 * SLOTS
 *===========================================================================*/

void Application::close()
{
  emit closing();
}

void Application::switchWidget(int index)
{
  if(!test_map->isLoaded() && index == 1)
    test_map->loadMap("maps/test_04");
  //if(test_map->isLoaded() && index != 1)
  //  test_map->unloadMap();
  
  /* Map timer stop */
  if(index != 1)
    test_map->tickStop();

  /* Sets the current displayed index */
  setCurrentIndex(index);

  if(index == 1)
    test_map->tickStart();
  else if(index == 2)
    title_screen->playBackground();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Temporary battle setup
 * DELETE when done
 */
void Application::setupBattle()
{
  /* Category Setups */
  AttributeSet bloodclaw_attr = AttributeSet();
  AttributeSet scion_attr = AttributeSet();
  AttributeSet cloud_attr = AttributeSet();

  bloodclaw_attr.setAll(1000, 125, 200, 150, 10, 8, 10, 8, 10, 8, 10, 8, 10,
                        8, 10, 8, 5, 5, 5);
  bloodclaw_attr.setAll(10000, 1250, 2000, 1500, 1000, 800, 1000, 800, 1000,
                        800, 1000, 800, 1000, 800, 1000, 800, 500, 500, 500,
                        true);

  scion_attr.setAll(900, 125, 200, 150, 10, 8, 35, 8, 10, 8, 10, 8, 10,
                    8, 10, 8, 5, 5, 5);
  scion_attr.setAll(10000, 1250, 2000, 1500, 1000, 800, 1000, 800, 1000,
                    800, 1000, 800, 1000, 800, 1000, 800, 500, 500, 500,
                    true);

  cloud_attr.setAll(500, 120, 180, 135, 9, 7, 9, 7, 9, 7, 9, 7, 9, 7, 5, 5,
                     5, 5, 5, false);
  cloud_attr.setAll(10000, 1250, 2000, 1500, 1000, 800, 1000, 800, 1000,
                     800, 1000, 800, 1000, 800, 1000, 800, 500, 500, 500,
                     true);

  Category* bloodclaw  = new Category("Bloodclaw", bloodclaw_attr);
  Category* scion      = new Category("Scion", scion_attr);
  Category* cloud_dudes = new Category("Cloud Attr", cloud_attr);

  /* Race Setups */
  AttributeSet bear_attr  = AttributeSet();
  AttributeSet human_attr = AttributeSet();
  AttributeSet base_enemy = AttributeSet();

  bear_attr.setAll(100, 12, 20, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
  bear_attr.setAll(300, 120, 150, 100, 80, 100, 80, 100, 80, 100, 80, 100, 80,
                   100, 80, 100, 50, 50, 50, true);

  human_attr.setAll(1000, 125, 200, 150, 10, 8, 10, 8, 10, 8, 10, 8, 10,
                    8, 10, 8, 5, 5, 5);
  human_attr.setAll(10000, 1250, 2000, 1500, 1000, 800, 1000, 800, 1000,
                     800, 1000, 800, 1000, 800, 1000, 800, 500, 500, 500, true);

  base_enemy.setAll(100, 12, 20, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
  base_enemy.setAll(300, 120, 150, 100, 80, 100, 80, 100, 80, 100, 80, 100, 80,
                   100, 80, 100, 50, 50, 50, true);

  Race* bears   = new Race("Bears", bear_attr);
  Race* humans  = new Race("Humans", human_attr);
  Race* base_enemies = new Race("Fiend", base_enemy);

  /* Sprite Setup */
  Sprite* main_ally      = new Sprite(":/A_player");
  Sprite* arcadius_ally  = new Sprite(":/A_arcadius");
  Sprite* gyrokin_ally   = new Sprite(":/A_gyrokin");
  Sprite* ulterius_ally  = new Sprite(":/A_ulterius");
  Sprite* aurumba_ally   = new Sprite(":/A_auruba");

  Sprite* cloud_enemy = new Sprite(":/E_cloud_thing");
  //Sprite* arcadius_enemy = new Sprite(":/E_arcadius");
  //Sprite* ulterius_enemy = new Sprite(":/E_ulterius");
  //Sprite* abbotmalus_enemy = new Sprite(":/E_abbotmalus");
  //Sprite* arcadius_enemy_modulated = new Sprite(":/E_arcadius_modulated");
  //Sprite* aurora_agent_enemy = new Sprite(":/E_aurora_agent");
  //Sprite* ballman_enemy = new Sprite(":/E_ballman");

  /* Item Setup */
  Item* anti_matter = new Item("Anti Matter", 1000, 0, 0);
  anti_matter->setBriefDescription("Not Matter");
  anti_matter->setDescription("This is a scary Item!");
  anti_matter->setItemFlag(Item::KEYITEM, true);

  Item* synthetic_food = new Item("Synthetic Food", 15, 0, 1.15);
  synthetic_food->setDescription("Biological Composite Material");
  synthetic_food->setItemFlag(Item::HEALITEM, true);
  synthetic_food->setItemFlag(Item::STACKABLE, true);

  Item* infinite_nourishment = new Item("Infinite Nourishment", 25, 0, 5.56);
  infinite_nourishment->setDescription("NEVER ENDING HEALS");
  infinite_nourishment->setItemFlag(Item::HEALITEM, true);
  infinite_nourishment->setItemFlag(Item::INDEFINITE, true);
  infinite_nourishment->setItemFlag(Item::STACKABLE, true);

  /* Equipment Setup */

  /* ---- */

  /* Person Setup */

  /* Test Party */
  Person* malgidus = new Person("Malgidus", bears, bloodclaw,  "THA", "PRB");
  malgidus->setFirstPerson(main_ally);

  Person* artemis  = new Person("Artemis", humans, scion,      "NIA", "PRC");
  artemis->setFirstPerson(arcadius_ally);

  Person* yolo     = new Person("Yolo", humans, bloodclaw,     "CHC", "PRB");
  yolo->setFirstPerson(gyrokin_ally);

  Person* helga    = new Person("Helga", humans, scion,        "POC", "NIC");
  helga->setFirstPerson(aurumba_ally);

  Person* gertrude = new Person("Gertrude", humans, bloodclaw, "PHA", "CYC");
  gertrude->setFirstPerson(ulterius_ally);

  /* Test Foes */
  Person* cloud_foe
          = new Person("Cloud Dude 1", base_enemies, cloud_dudes, "PRB", "POC");
  cloud_foe->setThirdPerson(cloud_enemy);

  Person* cloud_foe2
          = new Person("Cloud Dude 1", base_enemies, cloud_dudes, "PRB", "POC");
  cloud_foe2->setThirdPerson(cloud_enemy);

  Person* cloud_foe3
          = new Person("Cloud Dude 1", base_enemies, cloud_dudes, "PRB", "POC");
  cloud_foe3->setThirdPerson(cloud_enemy);

  Person* cloud_foe4
          = new Person("Cloud Dude 1", base_enemies, cloud_dudes, "PRB", "POC");
  cloud_foe4->setThirdPerson(cloud_enemy);

  Person* cloud_foe5
          = new Person("Cloud Dude 1", base_enemies, cloud_dudes, "PRB", "POC");
  cloud_foe5->setThirdPerson(cloud_enemy);

  /* Party Setup */

  /* Main Party Setup */
  Party* friends = new Party(malgidus, 0, EnumDb::SLEUTH);
  friends->addMember(artemis);
  friends->addMember(yolo);
  friends->addMember(helga);
  friends->addMember(gertrude);

  /* if (friends->removeMember("Artemis"))
      qDebug() << "Removing Artemis";
  if (friends->removeMember("Yolo"))
      qDebug() << "Removing Yolo";
  if (friends->removeMember("Helga"))
      qDebug() << "Removing Helga";
  if (friends->removeMember("Gertrude"))
      qDebug() << "Removing Gertrude";
  if (friends->removeMember("Malgidus"))
      qDebug() << "Removing Malgidus"; */

  /* Foes Setup */
  Party* foes = new Party(cloud_foe, 0, EnumDb::REGULAR_FOE);
  foes->addMember(cloud_foe2);
  foes->addMember(cloud_foe3);
  foes->addMember(cloud_foe4);
  foes->addMember(cloud_foe5);





  qDebug() << "=========================================";
  anti_matter->printAll();

  qDebug() << "=========================================";

  /* Item Setup */
  // Item* potion = new Item("Potion", 15, 0, 1.15);
  //potion->setDescription("This item will do some magical healing for you.");
  //potion->setUsingMessage("WHY ARE YOU USING ME");
  /* Level Up Test
  for (int i = 1; i < 128; i++)
  {
    main->setLevel(i);
    qDebug() << "LEVEL: " << main->getLevel();
    for (int j = 0; j < main->getStats()->getSize(); j++)
        qDebug() << main->getStats()->getStat(j) << " MAX: " << main->baseStats()->getMax(j);
    qDebug() << endl << endl;

  } */

  /* Ailment Testing
  Ailment* new_ailment1 = new Ailment(main, POISON, 2, 0.147, this);

  qDebug() << "Turns left: " << new_ailment1->getTurnsLeft();
  new_ailment1->update();
  qDebug() << "Turns left: " << new_ailment1->getTurnsLeft();
  new_ailment1->update();
  qDebug() << "Turns left: " << new_ailment1->getTurnsLeft();
  new_ailment1->update();
  qDebug() << "Turns left: " << new_ailment1->getTurnsLeft();
  new_ailment1->update();
  qDebug() << "Turns left: " << new_ailment1->getTurnsLeft();
  new_ailment1->update();

  * End Ailment Testing */


  test_battle = new Battle(friends, foes, this);
}
