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

/* Constant Implementation - see header file for descriptions */
const short Application::kRESOLUTION_X = 1216;
const short Application::kRESOLUTION_Y = 704;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

Application::Application(QWidget* parent)
{
  setParent(parent);

  // TODO: make this dependent on focus of the game. If it loses focus, 
  // return the normal cursor (Qt::ArrowCursor);
  setCursor(Qt::BlankCursor);
  //setCursor(Qt::ArrowCursor);

  title_screen = new TitleScreen(kRESOLUTION_X, kRESOLUTION_Y);
  //title_screen->show();

  setupBattle();
  //test_battle->show();
  
  test_map = new Map(kRESOLUTION_X, kRESOLUTION_Y);
  //test_map->loadMap("maps/test_03");
  //test_map->getViewport()->show();
  
  widget_stack = new QStackedWidget();
  widget_stack->addWidget(test_battle);
  widget_stack->addWidget(test_map->getViewport());
  widget_stack->addWidget(title_screen);

  widget_stack->setCurrentIndex(2); // TODO (0=battle, 1=map, 2=titlescreen)

  /* Widget information for handling the game */
  setCentralWidget(widget_stack);
  setFocus(Qt::OtherFocusReason);
  setFocusProxy(title_screen);
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
  show();
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
    test_map->loadMap("maps/test_03");

  widget_stack->setCurrentIndex(index);

  if(index == 2)
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
  AttributeSet bear_attr = AttributeSet();

  bloodclaw_attr.setAll(1000, 125, 200, 150, 10, 8, 10, 8, 10, 8, 10, 8, 10,
                        8, 10, 8, 5, 5, 5);
  bloodclaw_attr.setAll(10000, 1250, 2000, 1500, 1000, 800, 1000, 800, 1000,
                        800, 1000, 800, 1000, 800, 1000, 800, 500, 500, 500,
                        true);
  bear_attr.setAll(100, 12, 20, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
  bear_attr.setAll(300, 120, 150, 100, 80, 100, 80, 100, 80, 100, 80, 100, 80,
                   100, 80, 100, 50, 50, 50, true);

  Category* battle_class   = new Category("Bloodclaw", bloodclaw_attr);
  Race* the_bears          = new Race("Bears", bear_attr);

  Person* main = new Person("Malgidus", the_bears, battle_class, "THA", "PRB");
  Person* secd = new Person("CloudGuy", the_bears, battle_class, "THA", "PRC");

  Sprite* one = new Sprite(":/A_arcadius");
  Sprite* two = new Sprite(":/E_ulterius");

  main->setFirstPerson(one);
  secd->setThirdPerson(two);

  Party* friends = new Party(main, 5);
  friends->setPartyFlag(Party::MAIN, true);

  Party* foes = new Party(secd, 5);
  friends->setPartyFlag(Party::FOE, true);

  /* Level Up Test
  for (int i = 1; i < 128; i++)
  {
    main->setLevel(i);
    qDebug() << "LEVEL: " << main->getLevel();
    for (int j = 0; j < main->getStats()->getSize(); j++)
        qDebug() << main->getStats()->getStat(j) << " MAX: " << main->baseStats()->getMax(j);
    qDebug() << endl << endl;

  } */

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

  test_battle = new Battle(friends, foes, this);
}
