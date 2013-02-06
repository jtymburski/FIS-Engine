/******************************************************************************
* Class Name: Application
* Date Created: Nov 4 2012
* Inheritance: QWidget
* Description: The Application class
******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtGui/QWidget>

#include "EnumDatabase.h"
#include "Game/Game.h"
#include "Options.h"
//#include "SavedGame.h"
#include "TitleScreen.h"

class Application : public QWidget
{
public:
  /* Constructor function */
  Application(QWidget* parent=0);

  /* Destructor function */
  ~Application();

private:
  /* Keys used in the application (Key is qt enum) */
  int enter, esc, space, up, down, left, right, shift, F1;

  /* The current application mode */
  ApplicationMode current_application_mode;

  /* The applications title screen */
  TitleScreen* application_titlescreen;

  /* The applications options */
  Options application_options;

  /* The applications saved games */
//  SavedGame application_savedgame;

  /* The game */
  Game application_game;

public:
  /* Switches the current_application_mode */
  void swapModes();
};

#endif // APPLICATION_H
