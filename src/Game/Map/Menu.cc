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
 * CONSTANTS
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
    : event_handler{nullptr},
      config{nullptr},
      flags{static_cast<MenuState>(0)},
      renderer{nullptr},
      title_elements{}
{
}

Menu::~Menu()
{
  clear();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Menu::buildTitleElements()
{
  title_elements.clear();

  /* Party TitleElement */
  title_elements.push_back(TitleElement("Sleuth", true, MenuType::PARTY));

  /* Inventory TitleElement */
  title_elements.push_back(
      TitleElement("Inventory", true, MenuType::INVENTORY));

  /* Exit TitleElement */
  title_elements.push_back(TitleElement("Exit", true, MenuType::EXIT));
}

void Menu::buildTitleSection()
{
  // TODO Construct the background sprite
  title_section.alpha = 255;

  // TODO Base x point on width of background
  // TODO Base y on centred between screen height
  title_section.location.point.x = -200;
  title_section.location.point.y = 0;

  /* Ending Position for the Window */
  title_section.point.x = 0;
  title_section.point.y = 0;

  title_section.status = WindowStatus::OFF;
  title_section.background = nullptr;
}

void Menu::clearTitleSection()
{
  title_section.status = WindowStatus::HIDING;

  if(title_section.background)
    delete title_section.background;

  title_section.background = nullptr;
}

void Menu::renderTitleSection()
{
  /* Text Color */
  SDL_Color color{255, 255, 255, title_section.alpha};

  auto font_maintitle = config->getFontTTF(FontName::MENU_MAINTITLE);
  auto font_title = config->getFontTTF(FontName::MENU_TITLE);

  // TODO: Render the background. (location.width, height)

  auto x = title_section.location.point.x;
  auto y = title_section.location.point.y;

  std::cout << "Rendering title section at: " << x << ", " << y << std::endl;

  auto x_offset = (int32_t)std::round((float)config->getScreenWidth() * 0.01);
  auto y_offset = (int32_t)std::round((float)config->getScreenHeight() * 0.03);

  Text t_main_title(font_maintitle);
  t_main_title.setText(renderer, "MENU", color);


  auto y_gap = (int32_t)std::round((float)config->getScreenHeight() * 0.11);
  auto element_offset = y + 2 * y_offset + t_main_title.getHeight() + y_gap;

  Text t_title_element(font_title);

  for(auto& title_element : title_elements)
  {
    t_title_element.setText(renderer, title_element.name, color);
    t_title_element.render(renderer, x + x_offset, element_offset);
    element_offset += y_gap;
  }

  t_main_title.render(renderer, x + x_offset, y + y_offset);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void Menu::clear()
{
  clearTitleSection();
  setFlag(MenuState::SHOWING, false);
}

void Menu::hide()
{
  title_section.status = WindowStatus::HIDING;

  std::cout << "Hiding the menu" << std::endl;
}

void Menu::show()
{
  clear();

  setFlag(MenuState::SHOWING, true);

  buildTitleElements();
  buildTitleSection();

  title_section.status = WindowStatus::SHOWING;

  std::cout << "Showing the menu" << std::endl;
}

void Menu::render()
{
  if(renderer && config)
    if(title_section.status != WindowStatus::OFF)
      renderTitleSection();
}

bool Menu::update(int32_t cycle_time)
{

  if(title_section.status == WindowStatus::SHOWING)
  {
    /* Update the Coordinate of the TitleScreen */
    title_section.location.point = Helpers::updateCoordinate(
        cycle_time, title_section.location.point, title_section.point, 0.35);

    if(title_section.location.point == title_section.point)
      title_section.status = WindowStatus::ON;
  }
  else if(title_section.status == WindowStatus::HIDING)
  {
    title_section.location.point = Helpers::updateCoordinate(
        cycle_time, title_section.location.point,
        Coordinate(-400, title_section.location.point.y), 0.35);

    if(title_section.location.point.x == -400)
    {
      title_section.status = WindowStatus::OFF;
      clear();
    }
  }

  return false;
}

bool Menu::getFlag(const MenuState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

void Menu::setConfig(Options* config)
{
  this->config = config;
}

void Menu::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;
}

void Menu::setFlag(MenuState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

void Menu::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
