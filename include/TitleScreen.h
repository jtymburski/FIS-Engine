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

#include <string>
#include <vector>
//#include <QKeyEvent>
//#include <QLabel>
//#include <QList>
//#include <QPainter>
//#include <QPixmap>
//#include <QString>
//#include <QWidget>

#include "Frame.h"
#include "Options.h"
//#include "Sound.h"
#include "Text.h"

class TitleScreen// : public QWidget
{
  //Q_OBJECT

public:
  /* Default constructor */
  TitleScreen();

  /* Constructor function */
  TitleScreen(Options* running_config);

  /* Destructor function */
  ~TitleScreen();

  /* Enumerator: Application options to be selected */
  enum MenuItems{GAME    = 0,
                 OPTIONS = 1,
                 EXIT    = 2,
                 NONE    = 3};

private:
  /* The menu item that has been initiated. Gets polled by a parent class for
   * usage */
  MenuItems action;
  
  /* The background frame to render */
  Frame background;
  
  /* Background audio during title screen */
  //Sound background_sound;

  /* Indication for the title screen menu's position */
  uint8_t cursor_index;

  /* The font that is being used throughout the program */
  TTF_Font* font;
  
  /* Menu click audio, during selection changes */
  //Sound menu_click_sound;

  /* The render index of the top left of the title screen options */
  int render_index;
  
  /* The options at the first menu level */
  std::vector<Text*> selected_options;
  std::vector<Text*> unselected_options;

  /* The configuration for display of the game */
  Options* system_options;

  /* ------------------ Constants ------------------ */
  const static uint8_t kFONT_SIZE; /* The size of the font for rendering */
  const static std::string kMENU_ITEMS[]; /* The stored menu items */
  const static short kNUM_MENU_ITEMS;   /* Number of menu items in screen */
  const static uint16_t kTEXT_GAP; /* The gap between the rows of text */
  const static uint16_t kTEXT_MARGIN; /* The margin between the text and edge
                                       * of the screen */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Decrements the selected menu item */
  //void decrementSelected();

  /* Highlight the selected index, with a surrounding border */
  //bool highlight(int index);

  /* Increments the selected menu item */
  //void incrementSelected();

  /* Sets the selected menu item - fails if out of range */
  //bool setSelected(int index);
 
  /* Sets up the class, with the appropriate widget information */
  //void setupClass();

  /* Sets the selected item. This gets polled by another class */
  void setAction();
  
  /* Set up the menu display text, for painting */
  bool setMenu(SDL_Renderer* renderer);
  
  /* Un-Highlight the selected index by removing the border */
  //bool unhighlight(int index);

  /* Unsets all the menu related data, such as font and labels */
  void unsetMenu();
  
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
//protected:
  /* The paint event for the widget, to display everything */
  //void paintEvent(QPaintEvent*);

  /* The key press event, activated when this widget is in the forefront */
  //void keyPressEvent(QKeyEvent*);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
//signals:
//  void exit();
//  void openBattle(); // TEMP
//  void openMap(); // TEMP
//  void openOptions();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the active action */
  MenuItems getAction();
  
  /* Check if there's an active action */
  bool isActionOnQueue();
  
  /* The key up and down events to be handled by the class */
  void keyDownEvent(SDL_Keysym symbol);
  void keyUpEvent(SDL_Keysym symbol);
  
  /* Play the background sound file, once the menu status is returned */
  //void playBackground();

  /* Renders the title screen */
  bool render(SDL_Renderer* renderer);
  
  /* Sets the background frames to be rendered */
  bool setBackground(std::string path, SDL_Renderer* renderer);
  
  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);
};

#endif // TITLESCREEN_H
