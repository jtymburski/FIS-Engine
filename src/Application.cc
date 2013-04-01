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

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

Application::Application(QWidget* parent)
{
  setParent(parent);
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

Application::~Application()
{
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

  test_battle = new Battle(friends, foes, this);
}

/*
 * Temporary map setup
 * DELETE when done
 */
void Application::setupMap()
{
  test_map = new Map();
}
