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

#include <iostream>
#include <SDL2/SDL.h>

#include "Game/Player/Action.h"
#include "Game/Game.h"
#include "Helpers.h"
#include "Options.h"
//#include "SavedGame.h"
#include "Sound.h"
#include "SoundHandler.h"
#include "Sprite.h"
#include "TestBattle.h"
#include "Text.h"
#include "TitleScreen.h"

#ifdef _WIN32
  #define _WIN32_OPENGL
#endif

class Application
{
public:
  /* Constructor function */
  Application(std::string base_path = "");

  /* Destructor function */
  ~Application();

  /* Enumerator: Application options to be selected */
  //enum MenuState{OFF,MAIN,CONTINUE,INOPTIONS,INEXIT,SECRET}; // OLD
  enum AppMode
  {
    TITLESCREEN = 0,
    GAME        = 1,
    TESTBATTLE  = 2,
    OPTIONS     = 3,
    PAUSED      = 4,
    EXIT        = 5
  };

private:
  /* The base path, for accessing resources */
  std::string base_path;

  /* The running game */
  Game game_handler;

  /* Status if the subsystems have been successfully initialized */
  bool initialized;

  /* The current application that is running, under the head application
   * management */
  AppMode mode;
  AppMode temp_mode;

  /* The renderer for handling all interactions with the window */
  SDL_Renderer* renderer;

  /* Sound Handler */
  SoundHandler sound_handler;

  /* All options available for the system */
  Options* system_options;

  /* The test battle interface and handler */
  TestBattle test_battle;

  /* The displayed title screen for the game */
  TitleScreen title_screen;

  /* Update handling variables */
  uint8_t update_rate;
  uint8_t update_sync;

  /* The window to be displayed when the application is started */
  SDL_Window* window;

  /*------------------- Constants -----------------------*/
  const static uint8_t kUPDATE_CHANGE_LIMIT; /* The # of different frame times
                                              * allowed */
  const static uint8_t kUPDATE_RATE; /* The minimum ms per update sequence */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Change the mode that the application is running */
  bool changeMode(AppMode mode);

  /* Goes through all available events that are currently on the stack */
  void handleEvents();

  /* Log an Error */
  void logError(std::ostream &os, const std::string &msg);
  void logSDLError(std::ostream &os, const std::string &msg);

  /* Renders the current view and all relevant visual data */
  void render(uint32_t cycle_time);

  bool revertMode();

  /* Update the cycle time and return the update time sequence */
  int updateCycleTime(int cycle_time);

  /* Handles actions in views, depending on what's active */
  bool updateViews(int cycle_time);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Provides initialization of the SDL engine. Required for running. */
  bool initialize();

  /* Returns the status if the application libraries have been initialized */
  bool isInitialized();

  /* Runs the application */
  bool run(std::string test_path = "", int map_lvl = 0);

  /* Uninitializes all set functions in the application. Used to wind down
   * and no rendering will take place after this. */
  void uninitialize();
};

#endif // APPLICATION_H
