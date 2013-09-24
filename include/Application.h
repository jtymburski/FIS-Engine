/******************************************************************************
* Class Name: Application
* Date Created: Jan 29, 2013
* Inheritance: QStackedWidget
* Description: The Main Application that handles all the displays for the 
*              entire game. This includes the primary switching mechanism to 
*              switch between widgets as needed according to the selection on 
*              the screen.
******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <QDebug>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QLayout>
#include <QStackedWidget>

#include "Game/Game.h"
#include "Options.h"
//#include "SavedGame.h"
#include "TitleScreen.h"

/* DELETE: Temporary header access for testing */
#include "Game/Map/Map.h"
#include "Game/Battle/Battle.h"
#include "Game/Player/Action.h"
#include "MathHelper.h"
#include "GrammarHelper.h"

class Application : public QStackedWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  Application(QWidget* parent = 0);

  /* Destructor function */
  ~Application();

  /* Enumerator: Application options to be selected */
  //enum MenuState{OFF,MAIN,CONTINUE,INOPTIONS,INEXIT,SECRET}; // OLD
  enum AppItems{TITLESCREEN = 0,
                GAME        = 1,
                OPTIONS     = 2,
                EXIT        = 3};

private:
  /* The running game */
  Game* game_handler;

  /* All options available for the system */
  Options system_options;

  /* The displayed title screen for the game */
  TitleScreen* title_screen;
  
  /* DELETE two pointers below. Currently here for testing */
  Battle* test_battle;
  Map* test_map;

  /*------------------- Constants -----------------------*/
  //const static short kRESOLUTION_X; /* Sample */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  void closeEvent(QCloseEvent* event);

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  void close();
  void openBattle(); // TEMP
  void openMap(); // TEMP
  void switchWidget(int index);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void closing();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* DELETE two functions below. Currently here for testing */
  void setupBattle();
};

#endif // MAINWINDOW_H
