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
  EXIT_GAME = 1 << 1   /* Can the person use 'Defend'? */
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
  bool render(SDL_Renderer* renderer);

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

  /* Enumerated flags for the TitleScreen */
  TitleState flags;

  /* Current layer to the menu */
  MenuLayer menu_layer;

  /* Current main MenuType enumerated value */
  MenuType menu_type;

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

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Constructs the menu options */
  void buildTitleElements();

  /* KeyDown Events */
  void keyDownAction();
  void keyDownCancel();
  void keyDownDown();
  void keyDownLeft();
  void keyDownRight();
  void keyDownUp();

  /* Render Title Elements */
  void renderTitleElements(SDL_Renderer* renderer);

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
  bool render(SDL_Renderer* renderer);

  /* Assigns the running configuration for the Class */
  bool setConfig(Options* config);

  /* Assign flag */
  void setFlag(TitleState set_flags, const bool& set_value = true);

  /* Assigns the running sound handler */
  bool setSoundHandler(SoundHandler* sound_hanler);

  /* Updates the title screen. Necessary for visual updates */
  bool update(int32_t cycle_time);
};

#endif // TITLESCREEN_H
