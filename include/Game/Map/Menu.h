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

#include "Helpers.h"
#include "Game/EventHandler.h"
#include "Options.h"
#include "Sprite.h"

enum MenuType
{
  PARTY,
  INVENTORY,
  EXIT,
  INVALID,
};

struct TitleElement
{
  TitleElement() : name{""}, enabled{false}, menu_type{MenuType::INVALID} {};

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

  /* Enumerated type */
  MenuType menu_type;
};

struct Window
{
  /* Resting point for the Window */
  Coordinate point;

  /* The location of the Window */
  Box location;

  /* Enumerated status of the Window */
  WindowStatus status;

  /* Window background */
  Sprite* background;
};

ENUM_FLAGS(MenuState)
enum class MenuState
{

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

  /* Main Section (Centre) Window */
  Window main_section;

  /* Assigned Renderer */
  SDL_Renderer* renderer;

  /* Vector of title elements */
  std::vector<TitleElement> title_elements;

  /* Title Section (Left) Window */
  Window title_section;

  /* ------- Constants ------- */

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Construct a vector of TitleElements for the Title Section */
  void buildTitleElements();

  void buildTitleSection();

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  void setEventHandler(EventHandler* event_handler);
  void setConfig(Options* config);
  void setRenderer(SDL_Renderer* renderer);

  /*=============================================================================
   * PRIVATE STATIC FUNCTIONS
   *============================================================================*/
private:
};

#endif // MENU_H
