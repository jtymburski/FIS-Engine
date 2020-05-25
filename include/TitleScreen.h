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
#include "Game/Save.h"
#include "AnalogOption.h"
#include "DigitalOption.h"
#include "Music.h"
#include "Options.h"
#include "SoundHandler.h"
#include "Sprite.h"
#include "Text.h"
#include "Window.h"

/* BState Flags - Flags which describe only battle-related flags */
ENUM_FLAGS(TitleState)
enum class TitleState
{
  GO_TO_GAME = 1 << 0, /* Can the person use item in battle? */
  CLEAR_NAME = 1 << 1,
  GAME_LOADING = 1 << 2,
  EXIT_GAME = 1 << 3, /* Can the person use 'Defend'? */
  LOAD_FROM_SAVE = 1 << 4,
  DELETE_SAVE = 1 << 5,
  SELECTING_PRIMARY = 1 << 6,
  SELECTING_SECONDARY = 1 << 7
};

/* TitleBackground Elements */
class TitleBackground
{
public:
  TitleBackground();

public:
  /* The background frame to render */
  Sprite background;

  /* Title in corner */
  Text title;

public:
  /* Construct the Sprites */
  void buildSprites(Options* config, SDL_Renderer* renderer);

  /* Render the TitleBackground */
  bool render(SDL_Renderer* renderer, Options* config);
};

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

  /* Loading element indexes */
  int32_t load_element_index;
  MenuSaveState load_state;
  std::vector<Save> save_data;

  /* Pointer to the SoundHandler */
  SoundHandler* sound_handler;

#ifndef TITLE_SKIP
  /* The TitleBackground, if it is defined */
  TitleBackground title_background;
#endif

  /* Vector of TitleElements */
  std::vector<TitleElement> title_elements;
  std::vector<TitleElement> option_title_elements;

  /* Boxes */
  Box title_element_box;
  Box option_element_box;
  Box option_menu_box;
  Box player_selection_box;
  Box save_scroll_box;

  /* Current title mennu index */
  int32_t title_menu_index;
  int32_t option_menu_index;
  int32_t option_element_index;
  int32_t player_menu_index;

  /* ----------------- CONSTANTS -------------------- */
  static const SDL_Color kCOLOR_BACKGROUND;
  static const SDL_Color kCOLOR_BORDER;
  static const SDL_Color kCOLOR_SELECT;
  static const SDL_Color kCOLOR_TEXT;
  static const SDL_Color kCOLOR_TEXT_INVALID;
  static const SDL_Color kCOLOR_TITLE_HOVER;

  static const float kOPTIONS_GAP;
  static const float kOPTIONS_ELEMENT_HEIGHT;
  static const float kOPTIONS_HEIGHT;
  static const float kOPTIONS_WIDTH;

  static const float kSAVE_GAP;
  static const float kSAVE_ELEMENT_WIDTH;
  static const float kSAVE_ELEMENT_HEIGHT;
  static const float kSAVE_POPUP_GAP;
  static const float kSAVE_POPUP_WIDTH;
  static const float kSAVE_POPUP_HEIGHT;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Constructs the menu options */
  void buildTitleElements();
  void buildOptionTitleElements();

  /* Build the standard hover rectancular given coordinate information */
  SDL_Rect getRect(Coordinate current, int32_t height, int32_t width);

  /* Return the reference to the currently selected key */
  Key& getSelectedKey(KeyHandler& key_handler);

  /* Checks if the Player name is valid */
  bool isPlayerNameValid(KeyHandler& key_handler);

  /* KeyDown Events */
  void keyDownAction(SDL_Renderer* renderer, KeyHandler& key_handler);
  void keyDownCancel(KeyHandler& key_handler);
  void keyDownDown(KeyHandler& key_handler);
  void keyDownLeft(KeyHandler& key_handler);
  void keyDownRight(KeyHandler& key_handler);
  void keyDownUp(KeyHandler& key_handler);

  /* Render the Title Options */
  void renderOptions(SDL_Renderer* renderer, KeyHandler& key_handler);
  void renderOptionsMain(SDL_Renderer* renderer, KeyHandler& key_handler);
  void renderOptionsControls(SDL_Renderer* renderer, KeyHandler& key_handler);
  void renderOptionElementTitle(SDL_Renderer* renderer, TitleElement& title);
  void renderOptionElementTitles(SDL_Renderer* renderer, int32_t gap);

  /* Render the Player Section Box */
  void renderPlayerSelection(SDL_Renderer* renderer, KeyHandler& key_handler);

  /* Render Load Selection */
  void renderLoadSelection(SDL_Renderer* renderer);

  /* Render Title Elements */
  void renderTitleElements(SDL_Renderer* renderer);

  /* Updates the state of  the KeyHandler (INPUT vs. TEXT_ENTRY) */
  bool updateKeyHandler(KeyHandler& key_handler);

  /*======================== PUBLIC FUNCTIONS ================================*/
public:
  /* Constructs the Save Frames */
  void buildSave(SDL_Renderer* renderer);

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

  /* Return the selected player name */
  std::string getPlayerNameSelect();

  /* Return the selected player sex */
  Sex getPlayerSexSelect();

  /* Return the select save to LOAD or DELETE */
  int32_t getSaveIndex();

  /* Key down event, called with the KeyHandler */
  void keyDownEvent(SDL_Renderer* renderer, KeyHandler& key_handler);

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

  /* */
  void setSaveData(std::vector<Save> saves);

  /* Assigns the running sound handler */
  bool setSoundHandler(SoundHandler* sound_hanler);

  /* Updates the title screen. Necessary for visual updates */
  bool update(int32_t cycle_time, KeyHandler& key_handler);
};

#endif // TITLESCREEN_H
