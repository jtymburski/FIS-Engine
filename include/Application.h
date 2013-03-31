/******************************************************************************
* Class Name: Application
* Date Created: Jan 29, 2013
* Inheritance: QMainWindow
* Description: The Main Application that handles all the displays for the 
*              entire game. This includes the primary switching mechanism to 
*              switch between widgets as needed according to the selection on 
*              the screen.
******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QLayout>
#include <QStackedWidget>
#include <QtGui/QMainWindow>

#include "TitleScreen.h"

/* DELETE: Temporary header access for testing */
#include "Game/Map/Map.h"
#include "Game/Battle/Battle.h"
#include "Game/Player/Action.h"

class Application : public QMainWindow
{
  Q_OBJECT

public:
  /* Constructor function */
  Application(QWidget* parent = 0);

  /* Destructor function */
  ~Application();

private:
  QStackedWidget* widget_stack;
  TitleScreen* title_screen;
  
  /* DELETE two pointers below. Currently here for testing */
  Battle* test_battle;
  Map* test_map;

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
  void setupMap();
};

#endif // MAINWINDOW_H
