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
const short Application::kTICK_DELAY = 10;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

Application::Application(QWidget* parent)
{
  setParent(parent);

  // TODO: make this dependent on focus of the game. If it loses focus, 
  // return the normal cursor (Qt::ArrowCursor);
  //setCursor(Qt::BlankCursor);
  //setCursor(Qt::ArrowCursor);

  /* General class variables */
  close_command = false;
  
  /* Options information */
  short screen_height = system_options.getScreenHeight();
  short screen_width = system_options.getScreenWidth();

  /* Set up the title screen */
  title_screen = new TitleScreen(system_options);
  //title_screen->show();

  /* Set up the game */
  game_handler = new Game(system_options);

  /* Add widgets to the stack */
  addWidget(title_screen);
  addWidget(game_handler);
  switchWidget(TITLESCREEN);

  /* Widget information for handling the game */
  setMaximumWidth(screen_width);
  setMaximumHeight(screen_height);
  setMinimumWidth(screen_width);
  setMinimumHeight(screen_height);

  /* Object function call connections */
  QObject::connect(title_screen, SIGNAL(exit()),
                   this,         SLOT(exit()));
  QObject::connect(title_screen, SIGNAL(openBattle()), 
                   this,         SLOT(openBattle()));
  QObject::connect(title_screen, SIGNAL(openMap()), 
                   this,         SLOT(openMap()));
  QObject::connect(game_handler, SIGNAL(closeGame()),
                   this,         SLOT(closeGame()));

  /* Set the widget location (center of the screen) */
  QDesktopWidget desktopWidget;
  QRect desktopRect(desktopWidget
                      .availableGeometry(desktopWidget.primaryScreen()));
  setGeometry((desktopRect.width() - screen_width) / 2, 
              (desktopRect.height() - screen_height) / 2, 
              screen_width, screen_height);

  /* Set up the tick and start the update sequence */
  QObject::connect(&tick, SIGNAL(timeout()), this, SLOT(updateApp()));
  update_time.start();
  tick.setSingleShot(true);
  tick.start(kTICK_DELAY);
  //updateApp();

  /* Do the final show once everything is set up */
  //showFullScreen();
}

Application::~Application()
{
  /* Clear out the widgets in the stack */
  while(count() > 0)
    removeWidget(widget(0));

  /* Delete the game */
  if(game_handler != 0)
  {
    delete game_handler;
    game_handler = 0;
  }
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

void Application::switchWidget(int index)
{
  /* Sets the current displayed index */
  setCurrentIndex(index);
  if(index != GAME)
    currentWidget()->setFocus();

  /* Restart the background music if title screen is activated */
  if(index == TITLESCREEN && title_screen != 0)
    title_screen->playBackground();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

void Application::closeEvent(QCloseEvent* event)
{
  exit();
  event->ignore();
}

/*============================================================================
 * SLOTS
 *===========================================================================*/

void Application::closeGame()
{
  switchWidget(TITLESCREEN);
}

void Application::exit()
{
  close_command = true;
  emit closing();
}

/* TEMP */
void Application::openBattle()
{
  if(game_handler != 0)
  {
    game_handler->switchGameMode(Game::BATTLE);
    switchWidget(GAME);
  }
}

/* TEMP */
void Application::openMap()
{
  if(game_handler != 0)
  {
    game_handler->switchGameMode(Game::MAP);
    switchWidget(GAME);
  }
}

void Application::updateApp()
{
  /* Implement the update, where necessary */
  int cycle_time = update_time.restart();
  if(currentIndex() == TITLESCREEN && title_screen != 0)
    title_screen->update(); // TODO: Change to custom function??
  else if(currentIndex() == GAME && game_handler != 0)
    game_handler->updateGame(cycle_time);

  /* Restart the timer */
  if(!close_command)
    tick.start(kTICK_DELAY);
}
