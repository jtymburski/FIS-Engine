/*******************************************************************************
* Class Name: Menu [Declaration]
* Date Created: February 20, 2016
* Inheritance: None
* Description:
*
*
* TODO
* ----
*
* See .cc file for NOTEs
*****************************************************************************/
#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>

#include "EnumFlags.h"
#include "Helpers.h"
#include "Game/EventHandler.h"
#include "Game/Map/Map.h"
#include "Game/Player/Player.h"
#include "Options.h"
#include "Sprite.h"
#include "Box.h"

/* Enumerated MenuType */
enum class MenuType
{
  SLEUTH,
  INVENTORY,
  OPTIONS,
  SAVE,
  LOAD,
  QUIT,
  INVALID,
};

/* Enumerated MenuLayer */
enum class MenuLayer
{
  TITLE,
  MAIN,
  INVALID
};

/* Option Box Construction for Analog Values */
struct AnalogOption
{
  /* Constructs an AnalogOption */
  AnalogOption();

  /* Constructs an AnalogOption with a string name */
  AnalogOption(std::string name);

  /* Pointer to the value for the analog option (0 - 100) */
  uint32_t* val;

  /* Pointer to a default value */
  uint32_t default_val;

  /* The number of possible options */
  uint32_t num_options;

  /* Location for the analog option */
  Box location;

  /* String name of the option */
  std::string name;

  /* Decrease the value */
  void decrease();

  /* Reset the analog option to a default value */
  void reset();

  /* Increase the value */
  void increase();
};

/* Option Box Construction for On/Off Values */
struct DigitalOption
{
  /* Constructs a DigitalOption */
  DigitalOption();

  /* Constructs a DigitalOption */
  DigitalOption(Options* config, Coordinate point, int32_t width,
                int32_t height, int32_t flag_index, std::string name);

  /* Configuraiton pointer */
  Options* config;

  /* Value */
  int32_t flag_index;

  /* Location for the DigitalOption */
  Box location;

  /* String name of the option */
  std::string name;

  /* Resets a DigitalOption */
  void reset();

  /* Sets a DigitalOption */
  void set();

  /* Toggles the DigitalOption */
  void toggle();

  /* Unsets the DigitalOption */
  void unset();

  /* Returns whether the Option is Set */
  bool isSet();
};

struct TitleElement
{
  /* Default TitleElement Constructor */
  TitleElement()
      : name{""},
        enabled{false},
        hovered{false},
        hover_time{0},
        menu_type{MenuType::INVALID} {};

  /* General TitleElement Constructor */
  TitleElement(std::string name, bool enabled, MenuType menu_type)
      : TitleElement()
  {
    this->name = name;
    this->enabled = enabled;
    this->menu_type = menu_type;
  };

  /* Name of the TitleElement */
  std::string name;

  /* Is the TitleElement selectable? */
  bool enabled;

  /* Whether this TitleElement is being hovered over */
  bool hovered;

  /* Time since this TitleElement is hovered on */
  int32_t hover_time;

  /* Enumerated type */
  MenuType menu_type;
};

struct Window
{
  Window()
      : alpha{0},
        point{Coordinate(0, 0)},
        status{WindowStatus::OFF},
        backdrop{nullptr} {};

  /* Current alpha for rendering the window */
  uint8_t alpha;

  /* Resting point for the Window */
  Coordinate point;

  /* The location of the Window */
  Box location;

  /* Enumerated status of the Window */
  WindowStatus status;

  /* Window background */
  Frame* backdrop;
};

ENUM_FLAGS(MenuState)
enum class MenuState
{
  SHOWING = 1 << 2, /* Is the menu currently rendering */
};

class Menu
{
public:
  /* BattleMenu Default Initialization Function */
  Menu();

  /* Annihilates a BattleMenu object */
  ~Menu();

private:
  /* Current Map the Menu is Running On */
  Map* curr_map;

  /* The current player for the menu */
  Player* curr_player;

  /* Assigned EventHandler */
  EventHandler* event_handler;

  /* Assigned Configuration */
  Options* config;

  /* Enumerated flags for the Menu */
  MenuState flags;

  /* Frames to be built */
  Frame* frame_bubbies;
  Frame* frame_checkbox;
  Frame* frame_equipment;
  Frame* frame_footsteps;
  Frame* frame_items;
  Frame* frame_key_items;
  Frame* frame_location;
  Frame* frame_money;

  /* Current menu layer */
  MenuLayer layer;

  /* Main Section (Centre) Window */
  Window main_section;

  /* Options for the Option Menu */
  AnalogOption option_audio_level;
  AnalogOption option_music_level;
  DigitalOption option_auto_run;
  DigitalOption option_mute;

  /* Assigned Renderer */
  SDL_Renderer* renderer;

  /* Vector of title elements */
  std::vector<TitleElement> title_elements;

  /* Selected TitleElement index */
  int32_t title_element_index;
  int32_t option_element_index;

  /* Title Section (Left) Window */
  Window title_section;

  /* Options Elements */
  AnalogOption audio_level;
  AnalogOption music_level;

  /* ------- Constants ------- */

  /* Title Section */
  static const uint8_t kTITLE_ALPHA;
  static const float kTITLE_HEIGHT;
  static const float kTITLE_WIDTH;
  static const float kTITLE_X_OFFSET;
  static const float kTITLE_ELEMENT_X_OFFSET;
  static const float kTITLE_Y_OFFSET;
  static const float kTITLE_ELEMENT_GAP;
  static const float kTITLE_CORNER_LENGTH;
  static const float kTITLE_SLIDE_RATE;
  static const float kTITLE_LOCATION_Y_OFFSET;
  static const float kTITLE_ICONS_Y_GAP;
  static const float kTITLE_ICON_TEXT_X;
  static const float kTITLE_ICON_TEXT_Y;
  static const float kTITLE_HOVER_OFFSET_X;
  static const float kTITLE_HOVER_WIDTH;
  static const float kTITLE_HOVER_RATE;
  static const float kTITLE_HOVER_MIN;
  static const float kTITLE_HOVER_MAX;

  /* Main Section General */
  static const uint8_t kMAIN_ALPHA;
  static const float kMAIN_SLIDE_RATE;
  static const float kMAIN_CORNER_LENGTH;
  static const float kINV_WIDTH;
  static const float kOPTIONS_WIDTH;
  static const float kQUIT_WIDTH;
  static const float kSAVE_WIDTH;
  static const float kSLEUTH_WIDTH;

  /* Inventory Section */
  static const float kINV_X_GAP;
  static const float kINV_Y_GAP;
  static const float kINV_MASS_TEXT_Y;
  static const float kINV_MASS_VALUE_Y;
  static const float kINV_THUMB_GAP;
  static const float kINV_ITEM_NAME_X;
  static const float kINV_ITEM_NAME_Y;
  static const float kINV_ITEM_MASS_Y;
  static const float kINV_ITEM_DESC_Y;

  /* Options Section */
  static const uint32_t kNUM_OPTIONS;
  static const float kOPTIONS_X;
  static const float kOPTIONS_Y;
  static const float kOPTIONS_Y_BAR_GAP;
  static const float kOPTIONS_Y_GAP;
  static const float kOPTIONS_DIGITAL_TEXT_GAP;
  static const float kOPTIONS_BOX_SIZE;

  /* Colors */
  static const SDL_Color kCOLOR_TITLE_BG;
  static const SDL_Color kCOLOR_TITLE_BORDER;
  static const SDL_Color kCOLOR_TITLE_HOVER;
  static const SDL_Color kCOLOR_MAIN_BORDER;
  static const SDL_Color kCOLOR_TEXT;
  static const SDL_Color kCOLOR_OPTION_FILL;
  static const SDL_Color kCOLOR_OPTION_FILL_SELECTED;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Constructs icon frames */
  void buildIconFrames();

  /* Construct the inventory display screen */
  void buildInventoryScreen();

  /* Construct the main section backdrop */
  void buildMainSection(MenuType menu_type);

  /* Constructs the Options Data */
  void buildOptions();

  /* Construct a detail Person screen */
  void buildPersonDetailScreen();

  /* Construct the Sleuth overview screen */
  void buildSleuthScreen();

  /* Construct a vector of TitleElements for the Title Section */
  void buildTitleElements();

  /* Construct the TitleSection (Main Selection) of the Menu */
  void buildTitleSection();

  /* Clear out the Icon Frames */
  void clearIconFrames();

  /* Change the selected option */
  void decrementOptionIndex();
  void incrementOptionIndex();
  void unselectOptionIndex();
  void selectOptionIndex();

  /* Render the title section */
  void renderTitleSection();

  /* Render the main section */
  void renderMainSection();
  void renderInventory();

  /* Render options functions */
  UCoordinate renderOptionAnalog(AnalogOption& option, UCoordinate point);
  UCoordinate renderOptionDigital(DigitalOption& option, UCoordinate point);
  void renderOptions();

  void renderSleuth();
  void renderSave();
  void renderQuit();

  /* Returns the MenuType currently rendering */
  MenuType getMainMenuType();

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Clear the Menu State */
  void clear();

  /* Hide the Menu */
  void hide();

  /* Key down event */
  bool keyDownEvent(SDL_KeyboardEvent event);

  /* Show the Menu */
  void show();

  /* Render the Menu in its current state */
  void render();

  /* Update the menu with the current cycle time */
  bool update(int32_t cycle_time);

  /* Returns an evaluated MenuState flag */
  bool getFlag(const MenuState& test_flag);

  /* Assign Configuration */
  void setConfig(Options* config);

  /* Assign EventHandler */
  void setEventHandler(EventHandler* event_handler);

  /* Assign a given MenuState flag a given value */
  void setFlag(MenuState set_flags, const bool& set_value = true);

  /* Assign a map the menu is running on */
  void setMap(Map* new_map);

  /* Assign a new player to the menu */
  void setPlayer(Player* new_player);

  /* Assign Renderer */
  void setRenderer(SDL_Renderer* renderer);
};

#endif // MENU_H
