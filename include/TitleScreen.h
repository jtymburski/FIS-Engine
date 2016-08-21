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

#include "Game/KeyHandler.h"
#include "Music.h"
#include "Options.h"
#include "SoundHandler.h"
#include "Sprite.h"
#include "Text.h"
#include "Window.h"

// SKIP THE TITLE BACKGROUND LOADING / RENDERING
#define TITLE_SKIP

/* BState Flags - Flags which describe only battle-related flags */
ENUM_FLAGS(TitleState)
enum class TitleState
{
  GO_TO_GAME = 1 << 0, /* Can the person use item in battle? */
  GAME_LOADING = 1 << 1,
  EXIT_GAME = 1 << 2   /* Can the person use 'Defend'? */
};

/* TitleBackground Elements */
#ifndef TITLE_SKIP
class TitleBackground
{
public:
  TitleBackground();

public:
  /* TODO The background frame to render */
  Sprite background;  // TODO: Fix - base
  Sprite background2; // TODO: Remove - temporary - planet
  Sprite background3; // TODO: Remove - temporary - planet shroud
  Sprite background4; // TODO: Remove - temporary - booms 1
  Sprite background5; // TODO: Remove - temporary - booms 2
  Sprite background6; // TODO: Remove - temporary - moon
  Sprite background7; // TODO: Remove - temporary - waldo

  /* Delay values */
  uint16_t delay;  // TODO: Remove or fix - for displaying poofs
  uint16_t delay2; // TODO: Remove or fix - for displaying poofs

  /* Elapsed time of the TitleBackground */
  uint32_t elapsed_time;

  /* Title render is disabled */
  bool render_disabled;

  /* Rotation values */
  float rotate1;
  float rotate2;
  float rotate3;
  float rotate6;

  /* Title in corner */
  Frame title;

public:
  /* Construct the Sprites */
  void buildSprites(Options* config, SDL_Renderer* renderer);

  /* Render the TitleBackground */
  bool render(SDL_Renderer* renderer, KeyHandler& key_handler);

  /* Update the TitleBackground */
  bool update(int32_t cycle_time);
};
#endif

/* TitleScreen Class */
class TitleScreen
{
public:
  TitleScreen(Options* running_config = nullptr);

private:
  /* Running configuration assigned to the TitleScreen */
  Options* config;

  /* Current working coordinate for TitleScreen rendering */
  Coordinate current;

  /* Enumerated flags for the TitleScreen */
  TitleState flags;

  /* Current layer to the menu */
  MenuLayer menu_layer;

  /* Current main MenuType enumerated value */
  MenuType menu_type;

  /* Player Information Selection */
  std::string player_name_select;
  Sex player_sex_select;

  /* Pointer to the SoundHandler */
  SoundHandler* sound_handler;

#ifndef TITLE_SKIP
  /* The TitleBackground, if it is defined */
  TitleBackground title_background;
#endif

  /* Vector of TitleElements */
  std::vector<TitleElement> title_elements;

  /* TitleElement Box */
  Box title_element_box;
  Box player_selection_box;

  /* Current title mennu index */
  int32_t title_menu_index;
  int32_t player_menu_index;

  /* ----------------- CONSTANTS -------------------- */
  static const SDL_Color kCOLOR_BACKGROUND;
  static const SDL_Color kCOLOR_BORDER;
  static const SDL_Color kCOLOR_TEXT;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Constructs the menu options */
  void buildTitleElements();

  /* Build the standard hover rectancular given coordinate information */
  SDL_Rect getRect(Coordinate current, int32_t height, int32_t width);

  /* Checks if the Player name is valid */
  bool isPlayerNameValid(KeyHandler& key_handler);

  /* KeyDown Events */
  void keyDownAction(KeyHandler& key_handler);
  void keyDownCancel(KeyHandler& key_handler);
  void keyDownDown(KeyHandler& key_handler);
  void keyDownLeft(KeyHandler& key_handler);
  void keyDownRight(KeyHandler& key_handler);
  void keyDownUp(KeyHandler& key_handler);

  /* Render the Player Section Box */
  void renderPlayerSelection(SDL_Renderer* renderer, KeyHandler& key_handler);

  /* Render Title Elements */
  void renderTitleElements(SDL_Renderer* renderer);

  /* Updates the state of the KeyHandler (INPUT vs. TEXT_ENTRY) */
  bool updateKeyHandler(KeyHandler& key_handler);

  /*======================== PUBLIC FUNCTIONS ================================*/
public:
  /* Constructs the TitleBackground */
  void buildTitleBackground(SDL_Renderer* renderer);

  /* Enables or disables the view. This includes any initialization for before
   * or after it was visible */
  void enableView(bool enable);

  /* First update call each time the view changes - must be called */
  void firstUpdate();

  /* Returns the enumerated active title menu - polled by application */
  MenuType getActiveTitleMenu();

  /* Returns the value of a given ActorState flag */
  bool getFlag(const TitleState& test_flag);

  /* Key down event, called with the KeyHandler */
  void keyDownEvent(KeyHandler& key_handler);

/* If in debug mode, activate test keys */
#ifdef UDEBUG
  void keyTestDownEvent(SDL_KeyboardEvent event);
#endif

  /* Renders the title screen */
  bool render(SDL_Renderer* renderer, KeyHandler& key_handler);

  /* Assigns the running configuration for the Class */
  bool setConfig(Options* config);

  /* Assign flag */
  void setFlag(TitleState set_flags, const bool& set_value = true);

  /* Assigns the running sound handler */
  bool setSoundHandler(SoundHandler* sound_hanler);

  /* Updates the title screen. Necessary for visual updates */
  bool update(int32_t cycle_time, KeyHandler& key_handler);
};

#endif // TITLESCREEN_H
