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
const float Menu::kTITLE_WIDTH{0.14};
const float Menu::kTITLE_X_OFFSET{0.50};
const float Menu::kTITLE_Y_OFFSET{0.05};
const float Menu::kTITLE_ELEMENT_GAP{0.80};
const float Menu::kTITLE_CORNER_LENGTH{0.019};
const float Menu::kTITLE_SLIDE_RATE{0.70};
const float Menu::kMAIN_SLIDE_RATE{2.05};

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

void Menu::buildInventoryScreen()
{
}

void Menu::buildMainBackdrop()
{
  if(config && renderer)
  {
    clearMainBackdrop();

    main_section.backdrop = new Frame(
        config->getBasePath() + "sprites/Overlay/menu_main.png", renderer);

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

      test_box = Box({600, 250}, 300, 250, box_frames);
      test_box2 = Box({280, 250}, 300, 250, box_frames);
      test_box.setFlag(ScrollBoxState::SELECTABLE);
      test_box2.scroll_width *= 2;
      test_box2.scroll_inset_x *= 2;
      test_box2.scroll_inset_y *= 2;

      test_box2.setFlag(ScrollBoxState::SELECTED);
      test_box.color_element_border_selected = {255, 255, 255, 255};
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

  /* Load TitleElement */
  title_elements.push_back(TitleElement("Load", true, MenuType::LOAD));

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

    auto frame_w = title_section.backdrop->getWidth();
    auto frame_h = title_section.backdrop->getHeight();

    frame_w = (float)frame_w * width / (float)Options::kDEF_SCREEN_WIDTH;
    frame_h = (float)frame_h * height / (float)Options::kDEF_SCREEN_HEIGHT;

    /* Starting and ending Coordinates for the Title Section */
    title_section.location.point.x = -frame_w;
    title_section.location.point.y = y;
    title_section.location.height = frame_h;
    title_section.location.width = frame_w;
    title_section.point.x = 0;
    title_section.point.y = title_section.location.point.y;

    title_section.alpha = kTITLE_ALPHA;
    title_section.status = WindowStatus::OFF;
  }
}

void Menu::clearTitleSection()
{
  if(title_section.backdrop)
    delete title_section.backdrop;

  title_section.backdrop = nullptr;

  title_section.status = WindowStatus::OFF;
}

void Menu::renderTitleSection()
{
  // if(title_section.backdrop)
  // {
  //   title_section.backdrop->render(renderer, title_section.location.point.x,
  //                                  title_section.location.point.y,
  //                                  title_section.location.width,
  //                                  title_section.location.height);
  // }

  // /* Text Color */
  // SDL_Color color{235, 235, 235, kTITLE_ALPHA};

  // // auto width = config->getScreenWidth();
  // auto font_maintitle = config->getFontTTF(FontName::MENU_MAINTITLE);
  // auto font_title = config->getFontTTF(FontName::MENU_TITLE);
  // auto title_width = title_section.location.width;
  // auto x = title_section.location.point.x;
  // auto y = title_section.location.point.y;

  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, kTITLE_ALPHA);

  // auto x_offset = (int32_t)std::round((float)config->getScreenWidth() * 0.01);
  // auto y_offset = (int32_t)std::round((float)config->getScreenHeight() * 0.02);

  // Text t_main_title(font_maintitle);
  // t_main_title.setText(renderer, "MENU", color);

  // auto y_gap = (int32_t)std::round((float)config->getScreenHeight() * 0.07);
  // auto element_offset = y + y_offset + t_main_title.getHeight() + y_gap;
  // auto running_offset = element_offset;
  // auto title_element_height = 0;

  // Text t_title_element(font_title);
  // t_title_element.setText(renderer, "INVALID", color);
  // title_element_height = t_title_element.getHeight();

  //  Render the selected title element hover
  // if(title_element_index != -1 &&
  //    title_element_index < (int)title_elements.size())
  // {
  //   auto rect_y = element_offset + (title_element_index)*y_gap - (y_gap / 4);
  //   auto rect_h = title_element_height + (y_gap / 2);

  //   Coordinate top_left{x, rect_y};
  //   Coordinate top_right{x + title_width, rect_y};
  //   Coordinate bot_left{x, rect_y + rect_h};
  //   Coordinate bot_right{x + title_width, rect_y + rect_h};

  //   auto top_line = Helpers::bresenhamPoints(top_left, top_right);
  //   auto bot_line = Helpers::bresenhamPoints(bot_left, bot_right);

  //   SDL_Rect rect;
  //   rect.x = x - 3;
  //   rect.y = rect_y;
  //   rect.h = rect_h;
  //   rect.w = title_width;

  //   auto brightness = Helpers::updateHoverBrightness(
  //       title_elements.at(title_element_index).hover_time, 0.0010, 0.05, 0.8);

  //   SDL_SetRenderDrawColor(renderer, 5, 25, 45, kTITLE_ALPHA * brightness);
  //   SDL_RenderFillRect(renderer, &rect);
  //   SDL_SetRenderDrawColor(renderer, 255, 255, 255, kTITLE_ALPHA * brightness);
  //   Frame::drawLine(top_line, renderer);
  //   Frame::drawLine(bot_line, renderer);
  // }

  // for(auto& title_element : title_elements)
  // {
  //   t_title_element.setText(renderer, title_element.name, color);
  //   t_title_element.render(renderer, x + x_offset, running_offset);
  //   running_offset += y_gap;
  // }

  // t_main_title.render(renderer, x + x_offset, y + y_offset);
}

void Menu::renderMainSection()
{
  renderMainBackdrop();

  test_box.render(renderer);
  test_box2.render(renderer);
}

void Menu::renderMainBackdrop()
{
  // if(main_section.backdrop)
  // {
  //   main_section.backdrop->render(
  //       renderer, main_section.location.point.x, main_section.location.point.y,
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

    //TODO: Testing
    test_box.nextIndex();
    test_box2.nextIndex();
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

    //TODO: Testing
    test_box.prevIndex();
    test_box2.prevIndex();
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
  buildMainBackdrop();

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
  if(title_section.status == WindowStatus::SHOWING)
  {
    /* Update the Coordinate of the TitleScreen */
    title_section.location.point =
        Helpers::updateCoordinate(cycle_time, title_section.location.point,
                                  title_section.point, kTITLE_SLIDE_RATE);

    if(title_section.location.point == title_section.point)
      title_section.status = WindowStatus::ON;
  }
  else if(title_section.status == WindowStatus::HIDING)
  {
    title_section.location.point =
        Helpers::updateCoordinate(cycle_time, title_section.location.point,
                                  Coordinate(-title_section.location.width,
                                             title_section.location.point.y),
                                  kTITLE_SLIDE_RATE);

    if(title_section.location.point.x == -title_section.location.width)
    {
      title_section.status = WindowStatus::OFF;
      clear();
    }
  }

  if(main_section.status == WindowStatus::SHOWING)
  {
    main_section.location.point =
        Helpers::updateCoordinate(cycle_time, main_section.location.point,
                                  main_section.point, kMAIN_SLIDE_RATE);

    if(main_section.location.point == main_section.point)
      main_section.status = WindowStatus::ON;
  }
  else if(main_section.status == WindowStatus::HIDING)
  {
    main_section.location.point = Helpers::updateCoordinate(
        cycle_time, main_section.location.point,
        Coordinate(-main_section.location.width, main_section.location.point.y),
        kMAIN_SLIDE_RATE);

    if(main_section.location.point.x == -main_section.location.width)
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

void Menu::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
