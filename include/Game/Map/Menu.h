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
#include "Options.h"
#include "Sprite.h"

enum class MenuType
{
  PARTY,
  INVENTORY,
  EXIT,
  INVALID,
};

enum class MenuLayer
{
  TITLE,
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
  /* Current alpha for rendering the window */
  uint8_t alpha;

  /* Resting point for the Window */
  Coordinate point;

  /* The location of the Window */
  Box location;

  /* Enumerated status of the Window */
  WindowStatus status;

  /* Window background */
  Frame background;
};

ENUM_FLAGS(MenuState)
enum class MenuState
{
  ENABLED = 1 << 1, /* Is selecting the menu permissible */
  SHOWING = 1 << 2 /* Is the menu currently rendering */
};

class Menu
{
public:
  /* BattleMenu Default Initialization Function */
  Menu();

  /* Annihilates a BattleMenu object */
  ~Menu();

private:
  /* Assigned EventHandler */
  EventHandler* event_handler;

  /* Assigned Configuration */
  Options* config;

  /* Enumerated flags for the Menu */
  MenuState flags;

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
  static const uint8_t kTITLE_ALPHA;
  static const float kTITLE_HEIGHT;
  static const float kTITLE_WIDTH;
  static const float kTITLE_X_OFFSET;
  static const float kTITLE_Y_OFFSET;
  static const float kTITLE_ELEMENT_GAP;
  static const float kTITLE_CORNER_LENGTH;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
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

  /* Render the title section */
  void renderTitleSection();

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
  void setFlag(MenuState set_flags, const bool &set_value = true);

  /* Assign Renderer */
  void setRenderer(SDL_Renderer* renderer);

  /*=============================================================================
   * PRIVATE STATIC FUNCTIONS
   *============================================================================*/
private:
};

#endif // MENU_H
