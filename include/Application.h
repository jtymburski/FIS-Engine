/*******************************************************************************
 * Class Name: Application
 * Date Created: November 17, 2013
 * Inheritance: none
 * Description: The Main Application that handles all the displays for the 
 *              entire game. This includes the primary switching mechanism to 
 *              switch between views as needed according to the selection on 
 *              the screen.
 ******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//#include <vector>
//#include <QDebug>
//#include <QCloseEvent>
//#include <QDesktopWidget>
//#include <QLayout>
//#include <QStackedWidget>

#include "Game/Sprite.h"

//#include "Game/Game.h"
//#include "Options.h"
//#include "SavedGame.h"
//#include "TitleScreen.h"

class Application// : public QStackedWidget
{
//  Q_OBJECT

public:
  /* Constructor function */
  Application();//QWidget* parent = 0);

  /* Destructor function */
  ~Application();

  /* Enumerator: Application options to be selected */
  //enum MenuState{OFF,MAIN,CONTINUE,INOPTIONS,INEXIT,SECRET}; // OLD
  enum AppItems{TITLESCREEN = 0,
                GAME        = 1,
                OPTIONS     = 2,
                EXIT        = 3};

private:
  /* A status if the application has been commanded to close */
  //bool close_command;
  
  /* The running game */
  //Game* game_handler;

  /* Status if the subsystems have been successfully initialized */
  bool initialized;
  
  /* Internal variable to if the run loop should be stopped */
  bool quit;
  
  /* The renderer for handling all interactions with the window */
  SDL_Renderer* renderer;
  
  /* All options available for the system */
  //Options system_options;

  /* The application tick, for executing updates */
  //QTimer tick;

  /* The displayed title screen for the game */
  //TitleScreen* title_screen;
  
  /* The update time, for each cycle */
  //QTime update_time;
  
  /* The window to be displayed when the application is started */
  SDL_Window* window;
  
  /*------------------- Constants -----------------------*/
  //const static short kTICK_DELAY; /* The tick time, in ms */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Goes through all available events that are currently on the stack */
  void handleEvents();
  
  /* Switches the widget inside the stack (plus appropriate calls needed) */
//  void switchWidget(int index);


  
/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:
//  void closeEvent(QCloseEvent* event);

/*=============================================================================
 * SIGNALS
 *============================================================================*/
//signals:
  /* The closing signal, to shutdown the app */
//  void closing();

/*=============================================================================
 * SLOTS
 *============================================================================*/
//public slots:
  /* Called when the game is closed to return the application back to title */
//  void closeGame();

  /* The function called for closing. Clean-up put here */
//  void exit();

  /* Temp opening calls */
//  void openBattle(); // TEMP
//  void openMap(); // TEMP
  
  /* Updates the application, called on the tick */
//  void updateApp();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Provides initialization of the SDL engine. Required for running. */
  bool initialize();
  
  /* Returns the status if the application libraries have been initialized */
  bool isInitialized();
  
  /* Runs the application */
  bool run();
  
  /* Uninitializes all set functions in the application. Used to wind down
   * and no rendering will take place after this. */
  void uninitialize();
};

#endif // APPLICATION_H
