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
  QList<uint> stats1;

  for (int i = 0; i < 19; i++)
    stats1.append(5);

  AttributeSet base_set(stats1);

  Race* base_race = new Race("Fiends");
  base_race->setAttrSet(stats1);
  base_race->setMaxSet(stats1);

  Category* base_category = new Category("Battle Class");
  base_category->setAttrSet(stats1);
  base_category->setMaxSet(stats1);

  Person* main_character
          = new Person("Malgidus", base_race, base_category, "PHA", "CYB");
  Person* secd_character
          = new Person("Arcadius", base_race, base_category, "PHA", "CYA");

  Party* friends = new Party(main_character);
  Party* foes = new Party(secd_character);


  /* Bubby Signature Testing */
  BubbyFlavour* spark_flavour = new BubbyFlavour(0, "Spark");
  spark_flavour->setAttr(&base_set);

  Bubby* first_bubby = new Bubby(spark_flavour);
  first_bubby->setLevel(5);
  first_bubby->setTier(3);

  Bubby* second_bubby = new Bubby(spark_flavour);
  second_bubby->setTier(1);

  Bubby* third_bubby = new Bubby(spark_flavour);

  std::vector<std::pair<ushort, ushort> > list;
  list.push_back(std::make_pair(0, 0));
  list.push_back(std::make_pair(1, 1));
  list.push_back(std::make_pair(2, 2));
  list.push_back(std::make_pair(3, 3));
  list.push_back(std::make_pair(4, 4));
  Signature* equip_signature = new Signature(6, 6, list);

  qDebug() << " ======================= ";
  qDebug() << "Tier: " << first_bubby->getTier();
  qDebug() << equip_signature->attach(0, 1, second_bubby);
  qDebug() << equip_signature->attach(0, 3, first_bubby);

  qDebug() << equip_signature->attach(0, 0, third_bubby);
  QList<BubbyFlavour*> flavours = equip_signature->getUniqueFlavours();
  for (int i = 0; i < flavours.size(); i++)
    qDebug() << flavours.at(i)->getName();

  equip_signature->unattach(0, 3);
  equip_signature->unattach(0, 1);

  equip_signature->attach(0, 3, second_bubby);
  equip_signature->unattach(0, 3);

  equip_signature->attach(0, 3, first_bubby);
  equip_signature->unattach(2, 3);

  qDebug() << " ==================== ";
  equip_signature->printInfo();

  test_battle = new Battle(friends, foes, this);
}
