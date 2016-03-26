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
  PARTY,
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
  Frame frame_bubbies;
  Frame frame_equipment;
  Frame frame_footsteps;
  Frame frame_items;
  Frame frame_key_items;
  Frame frame_location;
  Frame frame_money;

  /* Current menu layer */
  MenuLayer layer;

  /* Main Section (Centre) Window */
  Window main_section;

  /* Assigned Renderer */
  SDL_Renderer* renderer;

  /* Vector of title elements */
  std::vector<TitleElement> title_elements;

  /* Selected TitleElement index */
  int32_t title_element_index;

  /* Title Section (Left) Window */
  Window title_section;

  /* ------- Constants ------- */

  /* Size Constants */
  static const uint8_t kTITLE_ALPHA;
  static const float kTITLE_HEIGHT;
  static const float kTITLE_WIDTH;
  static const float kTITLE_X_OFFSET;
  static const float kTITLE_Y_OFFSET;
  static const float kTITLE_ELEMENT_GAP;
  static const float kTITLE_CORNER_LENGTH;
  static const float kTITLE_SLIDE_RATE;
  static const float kMAIN_SLIDE_RATE;

  /* Colors */
  static const SDL_Color kCOLOR_TITLE_BG;
  static const SDL_Color kCOLOR_TITLE_BORDER;
  static const SDL_Color kCOLOR_TITLE_HOVER;
  static const SDL_Color kCOLOR_TEXT;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Constructs icon frames */
  void buildIconFrames();

  /* Construct the inventory display screen */
  void buildInventoryScreen();

  /* Construct the main section backdrop */
  void buildMainBackdrop();

  /* Construct a detail Person screen */
  void buildPersonDetailScreen();

  /* Construct the Sleuth overview screen */
  void buildSleuthScreen();

  /* Construct a vector of TitleElements for the Title Section */
  void buildTitleElements();

  /* Construct the TitleSection (Main Selection) of the Menu */
  void buildTitleSection();

  /* Clear the title section data */
  void clearTitleSection();

  /* Clear the Main Backdrop */
  void clearMainBackdrop();

  /* Render the title section */
  void renderTitleSection();

  void renderMainSection();

  void renderMainBackdrop();

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

  /*=============================================================================
   * PRIVATE STATIC FUNCTIONS
   *============================================================================*/
private:
};

#endif // MENU_H
