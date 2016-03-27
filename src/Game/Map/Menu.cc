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

const uint8_t Menu::kTITLE_ALPHA{255};
const float Menu::kTITLE_HEIGHT{0.77};
const float Menu::kTITLE_WIDTH{0.17};
const float Menu::kTITLE_X_OFFSET{0.02};
const float Menu::kTITLE_Y_OFFSET{0.05};
const float Menu::kTITLE_ELEMENT_GAP{0.80};
const float Menu::kTITLE_CORNER_LENGTH{0.02};
const float Menu::kTITLE_SLIDE_RATE{0.60};
const float Menu::kMAIN_SLIDE_RATE{2.05};

const SDL_Color Menu::kCOLOR_TITLE_BG{0, 0, 0, 255};
const SDL_Color Menu::kCOLOR_TITLE_BORDER{255, 255, 255, 255};
const SDL_Color Menu::kCOLOR_TITLE_HOVER{255, 255, 255, 65};
const SDL_Color Menu::kCOLOR_TEXT{255, 255, 255, 255};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: none
 */
Menu::Menu()
    : curr_map{nullptr},
      curr_player{nullptr},
      event_handler{nullptr},
      config{nullptr},
      flags{static_cast<MenuState>(0)},
      layer{MenuLayer::INVALID},
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

void Menu::buildIconFrames()
{
  if(config && renderer)
  {
    frame_bubbies = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/bubbies.png", renderer);
    frame_equipment = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/equipment.png", renderer);
    frame_footsteps = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/footsteps.png", renderer);
    frame_items = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/items.png", renderer);
    frame_key_items = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/key_items.png", renderer);
    frame_location = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/location.png", renderer);
    frame_money = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/money.png", renderer);
    frame_equipment = Frame(
        config->getBasePath() + "sprites/Overlay/Menu/equipment.png", renderer);
  }
}

void Menu::buildInventoryScreen()
{
}

void Menu::buildMainBackdrop()
{
  if(config && renderer)
  {
    clearMainBackdrop();

    auto width = config->getScreenWidth();
    auto height = config->getScreenHeight();

    auto frame_w = main_section.backdrop->getWidth();
    auto frame_h = main_section.backdrop->getHeight();

    frame_w = (float)frame_w * width / (float)Options::kDEF_SCREEN_WIDTH;
    frame_h = (float)frame_h * height / (float)Options::kDEF_SCREEN_HEIGHT;

    main_section.location.point.x = -frame_w;
    main_section.location.point.y =
        title_section.location.point.y + (kTITLE_CORNER_LENGTH * width);
    main_section.location.width = frame_w;
    main_section.location.height = frame_h;

    main_section.point.x = 0;
    main_section.point.y = main_section.location.point.y;

    std::vector<Frame*> box_frames;

    for(uint32_t i = 0; i < 255; i++)
    {
      SDL_Texture* texture =
          SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_TARGET, 220, 30);
      SDL_SetRenderTarget(renderer, texture);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawColor(renderer, i, i, i, i);

      SDL_Rect element_rect;
      element_rect.x = 1;
      element_rect.y = 1;
      element_rect.w = 220;
      element_rect.h = 30;

      SDL_RenderFillRect(renderer, &element_rect);
      box_frames.push_back(new Frame());
      box_frames.back()->setTexture(texture);
      SDL_SetRenderTarget(renderer, nullptr);

      // test_box = Box({600, 250}, 300, 250, box_frames);
      // test_box2 = Box({280, 250}, 300, 250, box_frames);
      // test_box.setFlag(ScrollBoxState::SELECTABLE);
      // test_box2.scroll_width *= 2;
      // test_box2.scroll_inset_x *= 2;
      // test_box2.scroll_inset_y *= 2;
      // test_box2.setFlag(ScrollBoxState::SELECTED);
      // test_box.color_element_border_selected = {255, 255, 255, 255};
    }
  }
}

void Menu::buildPersonDetailScreen()
{
}

void Menu::buildSleuthScreen()
{
}

void Menu::buildTitleElements()
{
  title_elements.clear();

  /* Party TitleElement */
  title_elements.push_back(TitleElement("Sleuth", true, MenuType::PARTY));

  /* Inventory TitleElement */
  title_elements.push_back(
      TitleElement("Inventory", true, MenuType::INVENTORY));

  /* Options TitleElement */
  title_elements.push_back(TitleElement("Options", true, MenuType::OPTIONS));

  /* Save TitleElement */
  title_elements.push_back(TitleElement("Save", true, MenuType::SAVE));

  /* Quit (Return to title) TitleElement */
  title_elements.push_back(TitleElement("Quit", true, MenuType::QUIT));
}

void Menu::buildTitleSection()
{
  clearTitleSection();

  auto height = config->getScreenHeight();
  auto width = config->getScreenWidth();

  auto title_height = (int32_t)std::round((float)height * kTITLE_HEIGHT);
  // auto title_width = (int32_t)std::round((float)width * kTITLE_WIDTH);

  auto y = (height - title_height) / 2 - (height * kTITLE_Y_OFFSET);

  if(!title_section.backdrop)
  {
    title_section.backdrop = new Frame(
        config->getBasePath() + "sprites/Overlay/menu_title_section.png",
        renderer);

    auto title_width = (uint32_t)std::round(width * kTITLE_WIDTH);
    auto title_height = (uint32_t)std::round(height * kTITLE_HEIGHT);
    auto corner_width = (uint32_t)std::round(width * kTITLE_CORNER_LENGTH);

    /* Starting and ending Coordinates for the Title Section */
    title_section.location.point.x = -(title_width + corner_width);
    title_section.location.point.y = y;
    title_section.location.height = title_height;
    title_section.location.width = title_width + corner_width;
    title_section.point.x = 0;
    title_section.point.y = y;

    title_section.alpha = kTITLE_ALPHA;
    title_section.status = WindowStatus::OFF;
  }

  // Create frames/textures for the title sections
  // if(curr_map && curr_player)
  // {
  //   std::cout << "Location frame size: " << frame_location.getHeight()
  //             << std::endl;
  //   auto step_count = curr_player->getSteps();
  //   auto credits = curr_player->getCredits();
  //   auto location = curr_map->getName();

  //   uint32_t location_x = (uint32_t)std::round(0.03 * width);
  //   uint32_t location_y = (uint32_t)std::round(point.y + (0.70 * height));

  //   std::cout << "Location of location frame: " << location_x << " , "
  //             << location_y << std::endl;

  //   SDL_Texture* texture = SDL_CreateTexture(
  //       renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
  //       frame_location.getWidth(), frame_location.getHeight());
  //   SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  //   SDL_SetRenderTarget(renderer, texture);
  //   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  //   SDL_RenderClear(renderer);

  //   frame_location.render(renderer, location_x, location_y);
  // }
}

void Menu::clearTitleSection()
{
  // if(title_section.backdrop)
  //  delete title_section.backdrop;

  // title_section.backdrop = nullptr;

  title_section.status = WindowStatus::OFF;
}

void Menu::renderTitleSection()
{
  auto font_main_title = config->getFontTTF(FontName::MENU_MAIN_TITLE);
  auto font_title_element = config->getFontTTF(FontName::MENU_TITLE_ELEMENT);
  auto height = config->getScreenHeight();
  auto width = config->getScreenWidth();
  auto location = title_section.location;
  auto point = title_section.location.point;

  auto corner_inset = (int32_t)std::round(width * kTITLE_CORNER_LENGTH);
  auto title_width = location.width + corner_inset;

  /* Render the frame outline and backdrop */
  Coordinate tl = {point.x, point.y};
  Coordinate tr = {point.x + location.width - corner_inset, point.y};
  Coordinate bl = {point.x, point.y + location.height};
  Coordinate br = {point.x + location.width - corner_inset,
                   point.y + location.height};

  Coordinate tc = {tr.x + corner_inset, tr.y + corner_inset};
  Coordinate bc = {br.x + corner_inset, br.y};

  Coordinate tr_aa_top = {tr.x + 1, tr.y};
  Coordinate tc_aa_top = {tc.x, tc.y - 1};

  Coordinate tr_aa_bot = {tr.x, tr.y + 1};
  Coordinate tc_aa_bot = {tc.x - 1, tc.y};

  auto top_bar = Helpers::bresenhamPoints(tl, tr);
  auto bot_bar = Helpers::bresenhamPoints(bl, br);
  auto top_corner = Helpers::bresenhamPoints(tr, tc);
  auto bot_corner = Helpers::bresenhamPoints(br, bc);
  auto right_line = Helpers::bresenhamPoints(tc, bc);
  auto corner_aa_top = Helpers::bresenhamPoints(tr_aa_top, tc_aa_top);
  auto corner_aa_bot = Helpers::bresenhamPoints(tr_aa_bot, tc_aa_bot);

  Frame::setRenderDrawColor(renderer, kCOLOR_TITLE_BG);
  Frame::renderFillLineToLine(top_bar, bot_bar, renderer, true);
  Frame::renderFillLineToLine(top_corner, bot_corner, renderer, true);

  Frame::setRenderDrawColor(renderer, kCOLOR_TITLE_BORDER);
  Frame::drawLine(top_bar, renderer);
  Frame::drawLine(bot_bar, renderer);
  Frame::drawLine(top_corner, renderer);
  Frame::drawLine(bot_corner, renderer);
  Frame::drawLine(right_line, renderer);

  Frame::setRenderDrawColor(renderer, {255, 255, 255, 110});
  Frame::drawLine(corner_aa_top, renderer);

  Frame::setRenderDrawColor(renderer, {255, 255, 255, 65});
  Frame::drawLine(corner_aa_bot, renderer);

  /* Text Color */
  Frame::setRenderDrawColor(renderer, kCOLOR_TEXT);

  auto x_offset = (int32_t)std::round(width * kTITLE_X_OFFSET);
  auto y_offset = (int32_t)std::round(height * kTITLE_Y_OFFSET);

  Text t_main_title(font_main_title);
  t_main_title.setText(renderer, "MENU", kCOLOR_TEXT);

  int32_t y_gap = (int32_t)std::round((float)config->getScreenHeight() * 0.06);
  int32_t element_offset =
      point.y + y_offset + t_main_title.getHeight() + y_gap;
  int32_t running_offset = element_offset;
  int32_t title_element_height = 0;

  Text t_title_element(font_title_element);
  t_title_element.setText(renderer, "INVALID", kCOLOR_TEXT);
  title_element_height = t_title_element.getHeight();

  if(title_element_index != -1 &&
     title_element_index < (int)title_elements.size())
  {
    auto rect_y = element_offset + (title_element_index)*y_gap - (y_gap / 4);
    auto rect_h = title_element_height + (y_gap / 2);

    Coordinate top_left{point.x, rect_y};
    Coordinate top_right{point.x + title_width, rect_y};
    Coordinate bot_left{point.x, rect_y + rect_h};
    Coordinate bot_right{point.x + title_width, rect_y + rect_h};
    auto top_line = Helpers::bresenhamPoints(top_left, top_right);
    auto bot_line = Helpers::bresenhamPoints(bot_left, bot_right);

    SDL_Rect rect;
    rect.x = point.x + title_width * 0.08;
    rect.y = rect_y;
    rect.h = rect_h;
    rect.w = title_width * 0.74;

    auto brightness = Helpers::updateHoverBrightness(
        title_elements.at(title_element_index).hover_time, 0.0014, 0.4, 0.8);

    auto hover_color = kCOLOR_TITLE_HOVER;
    SDL_SetRenderDrawColor(renderer, hover_color.r, hover_color.g,
                           hover_color.b, hover_color.a * brightness);
    SDL_RenderFillRect(renderer, &rect);
  }

  for(auto& title_element : title_elements)
  {
    t_title_element.setText(renderer, title_element.name, kCOLOR_TEXT);
    t_title_element.render(renderer, point.x + x_offset, running_offset);
    running_offset += y_gap;
  }

  t_main_title.render(renderer, point.x + x_offset, point.y + y_offset);
}

void Menu::renderMainSection()
{
  renderMainBackdrop();

  // test_box.render(renderer);
  // test_box2.render(renderer);
}

void Menu::renderMainBackdrop()
{
  // if(main_section.backdrop)
  // {
  //   main_section.backdrop->render(
  //       renderer, main_section.location.point.x,
  //       main_section.location.point.y,
  //       main_section.location.width, main_section.location.height);
  // }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void Menu::clear()
{
  clearTitleSection();
  clearMainBackdrop();
  setFlag(MenuState::SHOWING, false);
}

void Menu::clearMainBackdrop()
{
  if(main_section.backdrop)
    delete main_section.backdrop;

  main_section.backdrop = nullptr;

  main_section.status = WindowStatus::OFF;
}

void Menu::hide()
{
  title_section.status = WindowStatus::HIDING;
  title_element_index = -1;
  layer = MenuLayer::INVALID;
}

bool Menu::keyDownEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_DOWN)
  {
    if(layer == MenuLayer::TITLE)
    {
      if(title_element_index + 1 < (int32_t)title_elements.size())
        title_element_index++;
      else
        title_element_index = 0;
    }
  }
  else if(event.keysym.sym == SDLK_UP)
  {
    if(layer == MenuLayer::TITLE)
    {
      if(title_element_index > 0)
        title_element_index--;
      else
        title_element_index = title_elements.size() - 1;
    }
  }
  else if(event.keysym.sym == SDLK_SPACE)
  {
    if(title_section.status == WindowStatus::ON &&
       main_section.status == WindowStatus::OFF)
    {
      if(title_element_index < (int32_t)title_elements.size() &&
         title_elements.at(title_element_index).name == "Quit")
      {
        // TODO: [02-27-16]
        std::cout << "[Future] -- Return To Title" << std::endl;
        hide();
      }
      else
      {
        std::cout << "Name: " << title_elements.at(title_element_index).name
                  << std::endl;
        layer = MenuLayer::MAIN;
        main_section.status = WindowStatus::SHOWING;
      }
    }
  }
  else if(event.keysym.sym == SDLK_BACKSPACE)
  {
    if(main_section.status == WindowStatus::ON)
    {
      layer = MenuLayer::TITLE;
      main_section.status = WindowStatus::HIDING;
    }
    else if(title_section.status == WindowStatus::ON &&
            (main_section.status == WindowStatus::OFF ||
             main_section.status == WindowStatus::ON))
    {
      hide();
    }
  }

  return false;
}

void Menu::show()
{
  clear();

  setFlag(MenuState::SHOWING, true);

  buildTitleElements();
  buildTitleSection();
  buildIconFrames();
  // buildMainBackdrop();

  title_section.status = WindowStatus::SHOWING;

  title_element_index = 0;
  layer = MenuLayer::TITLE;
}

void Menu::render()
{
  if(renderer && config)
  {
    if(main_section.status != WindowStatus::OFF)
      renderMainSection();

    if(title_section.status != WindowStatus::OFF)
      renderTitleSection();
  }
}

bool Menu::update(int32_t cycle_time)
{
  auto& title_point = title_section.location.point;
  auto& title_location = title_section.location;
  auto& main_point = main_section.location.point;
  auto& main_location = main_section.location;

  if(title_section.status == WindowStatus::SHOWING)
  {
    /* Update the Coordinate of the TitleScreen */
    title_point = Helpers::updateCoordinate(
        cycle_time, title_point, title_section.point, kTITLE_SLIDE_RATE);

    if(title_point == title_section.point)
      title_section.status = WindowStatus::ON;
  }
  else if(title_section.status == WindowStatus::HIDING)
  {
    title_point = Helpers::updateCoordinate(
        cycle_time, title_point,
        Coordinate(-title_location.width, title_point.y), kTITLE_SLIDE_RATE);

    if(config)
    {
      if(title_point.x == -title_location.width)
      {
        title_section.status = WindowStatus::OFF;
        clear();
      }
    }
  }

  if(main_section.status == WindowStatus::SHOWING)
  {
    main_point = Helpers::updateCoordinate(
        cycle_time, main_point, main_section.point, kMAIN_SLIDE_RATE);

    if(main_point == main_section.point)
      main_section.status = WindowStatus::ON;
  }
  else if(main_section.status == WindowStatus::HIDING)
  {
    main_point = Helpers::updateCoordinate(
        cycle_time, main_point, Coordinate(-main_location.width, main_point.y),
        kMAIN_SLIDE_RATE);

    if(main_point.x == -main_location.width)
    {
      main_section.status = WindowStatus::OFF;
    }
  }

  if(title_element_index < (int32_t)title_elements.size() &&
     title_element_index > -1)
  {
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

void Menu::setMap(Map* new_map)
{
  this->curr_map = new_map;
}

void Menu::setPlayer(Player* new_player)
{
  this->curr_player = new_player;
}

void Menu::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
