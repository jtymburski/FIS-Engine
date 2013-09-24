/******************************************************************************
* Class Name: TitleScreen
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: Is the widget for the main display of the game. Will mostly 
*              just be a menu but this allows for the safe passage between
*              classes as interactions with Application occur.
******************************************************************************/
#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QKeyEvent>
#include <QLabel>
#include <QList>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QWidget>

#include "Options.h"
#include "Sound.h"

class TitleScreen : public QWidget
{
  Q_OBJECT

public:
  /* Default constructor */
  TitleScreen(QWidget* parent = 0);

  /* Constructor function */
  TitleScreen(Options running_config, QWidget* parent = 0);

  /* Destructor function */
  ~TitleScreen();

  /* Enumerator: Application options to be selected */
  enum MenuItems{TESTMAP    = 0,
                 TESTBATTLE = 1,
                 OPTIONS    = 2,
                 EXIT       = 3};
private:
  /* Background audio during title screen */
  Sound background_sound;

  /* The selection cursor - needed?? */
  //Frame cursor;

  /* Indication for the title screen menu's position */
  int cursor_index;

  /* Menu click audio, during selection changes */
  Sound menu_click_sound;

  /* The options at the first menu level */
  QList<QLabel*> option_labels;

  /* The configuration for display of the game */
  Options system_options;

  /* ------------------ Constants ------------------ */
  const static QString kMENU_ITEMS[]; /* The stored menu items */
  const static short kNUM_MENU_ITEMS;   /* Number of menu items in screen */
  
/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Decrements the selected menu item */
  void decrementSelected();

  /* Highlight the selected index, with a surrounding border */
  bool highlight(int index);

  /* Increments the selected menu item */
  void incrementSelected();

  /* Sets the selected menu item - fails if out of range */
  bool setSelected(int index);
 
  /* Sets up the class, with the appropriate widget information */
  void setupClass();

  /* Set up the menu display text, for painting */
  void setupMenu();
  
  /* Un-Highlight the selected index by removing the border */
  bool unhighlight(int index);

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* The paint event for the widget, to display everything */
  void paintEvent(QPaintEvent*);

  /* The key press event, activated when this widget is in the forefront */
  void keyPressEvent(QKeyEvent*);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void close();
  void openBattle(); // TEMP
  void openMap(); // TEMP

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Play the background sound file, once the menu status is returned */
  void playBackground();

  /* Sets the running configuration, from the options class */
  void setConfiguration(Options running_config);
};

#endif // TITLESCREEN_H
