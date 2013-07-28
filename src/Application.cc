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
  vsync_enabled = false;
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
  AttributeSet bloodclaw_attr = AttributeSet();
  AttributeSet scion_attr = AttributeSet();

  AttributeSet bear_attr = AttributeSet();
  AttributeSet human_attr = AttributeSet();

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

  bear_attr.setAll(100, 12, 20, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
  bear_attr.setAll(300, 120, 150, 100, 80, 100, 80, 100, 80, 100, 80, 100, 80,
                   100, 80, 100, 50, 50, 50, true);

  human_attr.setAll(1000, 125, 200, 150, 10, 8, 10, 8, 10, 8, 10, 8, 10,
                    8, 10, 8, 5, 5, 5);
  human_attr.setAll(10000, 1250, 2000, 1500, 1000, 800, 1000, 800, 1000,
                     800, 1000, 800, 1000, 800, 1000, 800, 500, 500, 500, true);

  Category* battle_class   = new Category("Bloodclaw", bloodclaw_attr);
  //Category* scion_class    = new Category("Scion", scion_attr);
  Race* the_bears          = new Race("Bears", bear_attr);
  Race* the_humans         = new Race("Humans", human_attr);


  Person* main = new Person("Malgidus", the_bears, battle_class, "THA", "PRB");
  Person* friend2 = new Person("Artemis", the_humans, battle_class, "NIA", "PRC");
  //Person* friend3 = new Person("Yolo", the_humans, scion_class, "CHC", "PRB");
  //Person* friend4 = new Person("Helga", the_bears, battle_class, "POC", "NIC");

  Person* secd = new Person("CloudGuy", the_bears, battle_class, "THA", "PRC");

  Sprite* main_sprite = new Sprite(":/A_arcadius");
  Sprite* secd_sprite = new Sprite(":/E_ulterius");
  Sprite* friend2_sprite = new Sprite(":/A_arcadius");

  main->setFirstPerson(main_sprite);
  secd->setThirdPerson(secd_sprite);
  friend2->setFirstPerson(friend2_sprite);

  Party* friends = new Party(main, 5);
  friends->setPartyFlag(Party::MAIN, true);
  friends->addMember(friend2);
  //friends->addMember(friend3);
  //friends->addMember(friend4);

  Party* foes = new Party(secd, 5);
  friends->setPartyFlag(Party::FOE, true);

  /* Inventory Testing */
  //Inventory* first_inventory = new Inventory("Basic Inventory");

  //Item* potion = new Item("Potion", 15, 0, 1.15);
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

  qDebug() << "Entering Battle.";
  test_battle = new Battle(friends, foes, this);
}
