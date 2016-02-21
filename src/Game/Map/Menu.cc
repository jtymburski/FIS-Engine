/*******************************************************************************
* Class Name: Menu [Implementation]
* Date Created: February 20, 2016
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/
#include "Game/Map/Menu.h"

/*=============================================================================
 * CONSTANTS - Public (for use in Battle)
 *============================================================================*/

/*=============================================================================
 * CONSTANTS - Private
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: none
 */
Menu::Menu()
  : event_handler{nullptr}
  , config{nullptr}
  , renderer{nullptr}
  , title_elements{}
{
  buildTitleElements();
}

// Menu::Menu(EventHandler* event_handler, Options* config, SDL_Renderer* renderer)
//   : Menu()
//   , event_handler{event_handler}
//   , config{config}
//   , renderer{renderer}
// {

// }

Menu::~Menu()
{
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Menu::buildTitleElements()
{
  title_elements.clear();

  /* Party TitleElement */
  title_elements.push_back(TitleElement("PARTY", true, MenuType::PARTY));

  /* Inventory TitleElement */
  title_elements.push_back(
      TitleElement("INVENTORY", true, MenuType::INVENTORY));

  /* Exit TitleElement */
  title_elements.push_back(TitleElement("EXIT", true, MenuType::EXIT));
}

void Menu::buildTitleSection()
{
 // title_section.background = new Sprite(
  /* Sprite */

  //title_section.point.x =

}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void Menu::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;
}

void Menu::setConfig(Options* config)
{
  this->config = config;
}

void Menu::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
