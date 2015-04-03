/*******************************************************************************
 * Class Name: TitleScreen
 * Date Created: Dec 02 2012
 * Inheritance: none
 * Description: Is the view for the main control display of the game. Will 
 *              mostly just be a menu but this allows for the safe passage 
 *              between classes as interactions with Application occur.
 ******************************************************************************/
#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <string>
#include <vector>

#include "Music.h"
#include "Options.h"
#include "Sound.h"
#include "Sprite.h"
#include "Text.h"

#define TITLE_SKIP

class TitleScreen
{
public:
  /* Constructor function */
  TitleScreen(Options* running_config = NULL);

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
  Sprite background; // TODO: Fix - base
  float rotate1;
  Sprite background2; // TODO: Remove - temporary - planet
  float rotate2;
  Sprite background3; // TODO: Remove - temporary - planet shroud
  float rotate3;
  Sprite background4; // TODO: Remove - temporary - booms 1
  Sprite background5; // TODO: Remove - temporary - booms 2
  Sprite background6; // TODO: Remove - temporary - moon
  float rotate6;
  Sprite background7; // TODO: Remove - temporary - waldo
  uint16_t delay; // TODO: Remove or fix - for displaying poofs
  uint16_t delay2; // TODO: Remove or fix - for displaying poofs
  
  /* Background music during title screen */
  Music background_music;
  
  /* The base path, for all rendering */
  std::string base_path;
  
  /* Indication for the title screen menu's position */
  uint8_t cursor_index;

  /* The font that is being used throughout the program */
  TTF_Font* font;
  
  /* The navigation direction keys indicator */
  bool nav_down;
  uint16_t nav_time;
  bool nav_up;
  
  /* Menu click audio, during selection changes */
  Sound menu_click_sound;

  /* The render index of the top left of the title screen options */
  int render_index;
 
  /* Title in corner */
  Frame title;

  /* The options at the first menu level */
  std::vector<Text*> selected_options;
  std::vector<Text*> unselected_options;

  /* The configuration for display of the game */
  Options* system_options;

  /* ------------------ Constants ------------------ */
  const static uint8_t kFONT_SIZE; /* The size of the font for rendering */
  const static std::string kMENU_ITEMS[]; /* The stored menu items */
  const static uint16_t kNAV_TIME_LIMIT; /* The key click time */
  const static uint8_t kNUM_MENU_ITEMS; /* Number of menu items in screen */
  const static uint16_t kTEXT_GAP; /* The gap between the rows of text */
  const static uint16_t kTEXT_MARGIN; /* The margin between the text and edge
                                       * of the screen */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Decrements the selected option */
  void decrementSelected();
  
  /* Increments the selected option */
  void incrementSelected();
  
  /* Sets the selected item. This gets polled by another class */
  void setAction();
  
  /* Set up the menu display text, for painting */
  bool setMenu(SDL_Renderer* renderer);

  /* Unsets all the menu related data, such as font and labels */
  void unsetMenu();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Enables or disables the view. This includes any initialization for before
   * or after it was visible */
  void enableView(bool enable);
  
  /* Returns the active action */
  MenuItems getAction();
  
  /* The key up and down events to be handled by the class */
  void keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Renders the title screen */
  bool render(SDL_Renderer* renderer);

  /* Sets the background frames to be rendered */
  bool setBackground(std::string path, SDL_Renderer* renderer);
  
  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);
  
  /* Sets the music in the title screen - TODO: Encapsulate in file load? */
  void setMusic();
  
  /* Updates the title screen. Necessary for visual updates */
  bool update(int32_t cycle_time);
};

#endif // TITLESCREEN_H