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
const uint8_t Menu::kTITLE_ALPHA{235};
const float Menu::kTITLE_HEIGHT{0.75};
const float Menu::kTITLE_WIDTH{0.14};
const float Menu::kTITLE_X_OFFSET{0.30};
const float Menu::kTITLE_Y_OFFSET{0.50};
const float Menu::kTITLE_ELEMENT_GAP{0.80};
const float Menu::kTITLE_CORNER_LENGTH{0.03};

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
      title_elements{},
      title_element_index{-1}
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
  auto height = config->getScreenHeight();
  auto width = config->getScreenWidth();

  auto title_height = (int32_t)std::round((float)height * kTITLE_HEIGHT);
  auto title_width = (int32_t)std::round((float)width * kTITLE_WIDTH);

  auto y = (height - title_height) / 2;

  /* Starting and ending Coordinates for the Title Section */
  title_section.location.point.x = -title_width;
  title_section.location.point.y = y;
  title_section.location.height = title_height;
  title_section.location.width = title_width;
  title_section.point.x = 0;
  title_section.point.y = title_section.location.point.y;

  title_section.alpha = kTITLE_ALPHA;
  title_section.status = WindowStatus::OFF;
}

void Menu::clearTitleSection()
{
  title_section.status = WindowStatus::HIDING;
}

void Menu::renderTitleSection()
{
  /* Text Color */
  SDL_Color color{235, 235, 235, kTITLE_ALPHA};

  auto height = config->getScreenHeight();
  auto width = config->getScreenWidth();
  auto font_maintitle = config->getFontTTF(FontName::MENU_MAINTITLE);
  auto font_title = config->getFontTTF(FontName::MENU_TITLE);
  auto title_height = title_section.location.height;
  auto title_width = title_section.location.width;
  auto corner_length = (int32_t)std::round((float)width * kTITLE_CORNER_LENGTH);
  auto x = title_section.location.point.x;
  auto y = title_section.location.point.y;

  SDL_Rect rect;
  rect.h = title_height;
  rect.w = title_width - corner_length;
  rect.x = x;
  rect.y = y;

  SDL_SetRenderDrawColor(renderer, 5, 25, 45, 2 * kTITLE_ALPHA / 3);
  SDL_RenderFillRect(renderer, &rect);

  auto x2 = x + title_width - corner_length;
  auto y2 = (height - title_height) / 2;
  auto y3 = y + corner_length;
  auto x3 = x + title_width;
  auto y4 = y + title_height - corner_length;
  auto y5 = y + title_height;

  /* Create the Background Frame */
  auto top_line = Helpers::bresenhamPoints({0, y2}, {x2, y2});
  auto bottom_line = Helpers::bresenhamPoints({0, y5}, {x2, y5});
  auto top_corner_line = Helpers::bresenhamPoints({x2, y2}, {x3, y3});
  auto bot_corner_line = Helpers::bresenhamPoints({x2, y5}, {x3, y4});
  auto right_line = Helpers::bresenhamPoints({x3, y3}, {x3, y4});

  /* Render fill between the two corner lines */
  Frame::renderFillLineToLine(bot_corner_line, top_corner_line, renderer, true);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, kTITLE_ALPHA);
  Frame::drawLine(top_line, renderer);
  Frame::drawLine(bottom_line, renderer);
  Frame::drawLine(top_corner_line, renderer);
  Frame::drawLine(bot_corner_line, renderer);
  Frame::drawLine(right_line, renderer);

  auto x_offset = (int32_t)std::round((float)config->getScreenWidth() * 0.01);
  auto y_offset = (int32_t)std::round((float)config->getScreenHeight() * 0.03);

  Text t_main_title(font_maintitle);
  t_main_title.setText(renderer, "MENU", color);

  auto y_gap = (int32_t)std::round((float)config->getScreenHeight() * 0.11);
  auto element_offset = y + 2 * y_offset + t_main_title.getHeight() + y_gap;
  auto running_offset = element_offset;
  auto title_element_height = 0;

  Text t_title_element(font_title);
  t_title_element.setText(renderer, "INVALID", color);
  title_element_height = t_title_element.getHeight();

  /* Render the selected title element hover */
  if(title_element_index != -1 &&
     title_element_index < (int)title_elements.size())
  {

    auto rect_y = element_offset + (title_element_index)*y_gap - (y_gap / 4);
    auto rect_h = title_element_height + (y_gap / 2);

    Coordinate top_left{x, rect_y};
    Coordinate top_right{x + title_width, rect_y};
    Coordinate bot_left{x, rect_y + rect_h};
    Coordinate bot_right{x + title_width, rect_y + rect_h};

    auto top_line = Helpers::bresenhamPoints(top_left, top_right);
    auto bot_line = Helpers::bresenhamPoints(bot_left, bot_right);

    SDL_Rect rect;
    rect.x = x;
    rect.y = rect_y;
    rect.h = rect_h;
    rect.w = title_width;

    auto brightness = Helpers::updateHoverBrightness(
        title_elements.at(title_element_index).hover_time, 0.0008, 0.05, 0.9);

    SDL_SetRenderDrawColor(renderer, 5, 25, 45, kTITLE_ALPHA * brightness);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, kTITLE_ALPHA * brightness);
    Frame::drawLine(top_line, renderer);
    Frame::drawLine(bot_line, renderer);
  }

  for(auto& title_element : title_elements)
  {
    t_title_element.setText(renderer, title_element.name, color);
    t_title_element.render(renderer, x + x_offset, running_offset);
    running_offset += y_gap;
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
  title_element_index = -1;
}

void Menu::show()
{
  clear();

  setFlag(MenuState::SHOWING, true);

  buildTitleElements();
  buildTitleSection();

  title_section.status = WindowStatus::SHOWING;
  title_element_index = 0;
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

  for(auto& title_element : title_elements)
  {
    if(title_element_index < title_elements.size() && title_element_index > -1)
      title_elements.at(title_element_index).hover_time += cycle_time;
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
