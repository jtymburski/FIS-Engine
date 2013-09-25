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

  /* The application tick, for executing updates */
  QTimer tick;

  /* The displayed title screen for the game */
  TitleScreen* title_screen;
  
  /*------------------- Constants -----------------------*/
  const static short kTICK_DELAY; /* The tick time, in ms */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Switches the widget inside the stack (plus appropriate calls needed) */
  void switchWidget(int index);

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  void closeEvent(QCloseEvent* event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* The closing signal, to shutdown the app */
  void closing();

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  /* The function called for closing. Clean-up put here */
  void close();

  /* Called when the game is closed to return the application back to title */
  void closeGame();

  /* Temp opening calls */
  void openBattle(); // TEMP
  void openMap(); // TEMP
  
  /* Updates the application, called on the tick */
  void updateApp();
};

#endif // APPLICATION_H
