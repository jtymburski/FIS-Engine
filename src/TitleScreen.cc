/*******************************************************************************
 * Class Name: TitleScreen
 * Date Created: Dec 02 2012
 * Inheritance: none
 * Description: Is the view for the main control display of the game. Will
 *              mostly just be a menu but this allows for the safe passage
 *              between classes as interactions with Application occur.
 *
 * TODO:
 ******************************************************************************/
#include "TitleScreen.h"

/*=============================================================================
 * TITLE_BACKGROUND - If defined
 *============================================================================*/

#ifndef TITLE_SKIP
/* Construct the TitleBackground Class */
TitleBackground::TitleBackground()
    : delay{0},
      delay2{0},
      elapsed_time{0},
      render_disabled{false},
      rotate1{0},
      rotate2{0},
      rotate3{0},
      rotate6{0}
{
}

bool TitleBackground::render(SDL_Renderer* renderer)
{
  bool success = false;

  if(renderer)
  {
    success = true;

    /* Render the background */
    success &= background.render(renderer, 0, 0);
    success &= background6.render(renderer, -220, -48);
    success &= background2.render(renderer, 153, 314);

    /* Waldo show - TODO: Only certain hours? */
    success &= background7.render(renderer, 153, 314);

    /* The flash and bangs */
    success &= background4.render(renderer, 153, 314);
    success &= background5.render(renderer, 153, 314);

    /* Render atmosphere */
    success &= background3.render(renderer, 153, 314);

    if(!render_disabled)
      success &= title.render(renderer, 50, 50);
  }

  return success;
}

void TitleBackground::buildSprites(Options* config, SDL_Renderer* renderer)
{
  if(config && renderer)
  {
    auto path = config->getBasePath();

    background.insertFirst(path + "sprites/Title/title-backdrop.png", renderer);
    background2.insertFirst(path + "sprites/Title/title-dynaton.png", renderer);
    background4.insertFirst(path + "sprites/Title/title-dynatonbooms1.png",
                            renderer);
    background4.setOpacity(0);
    background5.insertFirst(path + "sprites/Title/title-dynatonbooms2.png",
                            renderer);
    background5.setOpacity(0);
    background3.insertFirst(path + "sprites/Title/title-dynatonatmosphere.png",
                            renderer);
    background3.setOpacity(196);
    background3.setColorBalance(255, 170, 170);
    background6.insertFirst(path + "sprites/Title/title-moon.png", renderer);
    background7.insertFirst(path + "sprites/Title/title-waldo.png", renderer);
    title.setTexture(path + "sprites/Title/title.png", renderer);
  }
}

/* Updates the title screen. Necessary for visual updates */
bool TitleBackground::update(int32_t cycle_time)
{
  /* Increment the nav time */
  elapsed_time += cycle_time;

  /* Rotation testing */
  // rotate1 += 0.03125;//0.0625;//0.001;
  rotate2 += 0.03125;
  rotate3 += 0.05;
  rotate6 -= 0.075;

  // background.setRotation(rotate1);
  background2.setRotation(rotate2);
  background3.setRotation(rotate3);
  background4.setRotation(rotate2);
  background5.setRotation(rotate2);
  background6.setRotation(rotate6);
  background7.setRotation(rotate2);

  /* Poof Number 1 */
  if(delay < 5000)
  {
    delay += cycle_time;
  }
  else if(delay < 10000)
  {
    delay = 10000;
    background4.setOpacity(255);
  }

  if(delay == 10000)
  {
    background4.setBrightness(background4.getBrightness() - 0.03125);

    if(background4.getBrightness() == 0.0)
    {
      uint8_t opacity = background4.getOpacity();

      if(opacity >= 2)
        background4.setOpacity(opacity - 2);
      else
      {
        background4.setBrightness(1.0);
        background4.setOpacity(0);
        delay = 0;
      }
    }
  }

  /* Poof Number 2 */
  if(delay2 < 7500)
  {
    delay2 += cycle_time;
  }
  else if(delay2 < 10000)
  {
    delay2 = 10000;
    background5.setOpacity(255);
  }

  if(delay2 == 10000)
  {
    background5.setBrightness(background5.getBrightness() - 0.03125);

    if(background5.getBrightness() == 0.0)
    {
      uint8_t opacity = background5.getOpacity();

      if(opacity >= 2)
        background5.setOpacity(opacity - 2);
      else
      {
        background5.setBrightness(1.0);
        background5.setOpacity(0);
        delay2 = 0;
      }
    }
  }

  /* Brightness (flicker) concept */
  // if(Helpers::randInt(10) < 2)
  //  background.setBrightness(background.getBrightness() - 0.01);
  // if(background.getBrightness() < 0.6)
  //  background.setBrightness(1.0);

  return false;
}
#endif

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

TitleScreen::TitleScreen(Options* config)
    : config{config},
      flags{static_cast<TitleState>(0)},
      menu_layer{MenuLayer::TITLE},
      menu_type{MenuType::INVALID},
      sound_handler{nullptr},
      title_elements{},
      title_menu_index{-1}
{
  buildTitleElements();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Create the main menu title elements */
void TitleScreen::buildTitleElements()
{
  /* Clear the title elements to begin */
  title_elements.clear();

  /* Create tNew Game element */
  auto elm = TitleElement(Helpers::menuTypeToStr(MenuType::TITLE_NEW_GAME),
                          true, MenuType::TITLE_NEW_GAME);
  title_elements.push_back(elm);

  /* Create the Load Title element */
  elm = TitleElement(Helpers::menuTypeToStr(MenuType::TITLE_LOAD_GAME), true,
                     MenuType::TITLE_LOAD_GAME);
  title_elements.push_back(elm);

  /* Creates the Options */
  elm = TitleElement(Helpers::menuTypeToStr(MenuType::TITLE_OPTIONS), true,
                     MenuType::TITLE_OPTIONS);
  title_elements.push_back(elm);

  /* Create the Quit title element */
  elm = TitleElement(Helpers::menuTypeToStr(MenuType::TITLE_QUIT), true,
                     MenuType::TITLE_QUIT);
  title_elements.push_back(elm);

  /* Assign the current index and menu type after building title elements */
  title_menu_index = 0;
  menu_type = MenuType::TITLE_NEW_GAME;
}

/* Processing for the Action key */
void TitleScreen::keyDownAction()
{
  if(menu_layer == MenuLayer::TITLE)
  {
    if(title_menu_index != -1 &&
       (uint32_t)title_menu_index < title_elements.size())
    {
      /* Play sound */
      sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS, true);
    }

    menu_type = title_elements.at(title_menu_index).menu_type;

    if(menu_type == MenuType::TITLE_NEW_GAME)
      setFlag(TitleState::GO_TO_GAME);
    else if(menu_type == MenuType::TITLE_QUIT)
      setFlag(TitleState::EXIT_GAME);
  }
}

/* Processing for the Cancel key */
void TitleScreen::keyDownCancel()
{
  if(menu_layer == MenuLayer::TITLE)
  {
    /* Play sound */
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_PREV,
                                  SoundChannels::MENUS);
  }
}

/* Processing for the Down key */
void TitleScreen::keyDownDown()
{
  if(menu_layer == MenuLayer::TITLE)
  {
    if(title_menu_index + 1 < (int32_t)title_elements.size())
    {
      sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_CHG,
                                    SoundChannels::MENUS);

      title_menu_index++;
    }
    else
      title_menu_index = 0;
  }
}

/* Processing for the Left key */
void TitleScreen::keyDownLeft()
{
}

/* Processing for the Right key */
void TitleScreen::keyDownRight()
{
}

/* Processing for the Up key */
void TitleScreen::keyDownUp()
{
  if(menu_layer == MenuLayer::TITLE)
  {
    if(title_menu_index > 0)
    {
      sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_CHG,
                                    SoundChannels::MENUS);

      title_menu_index--;
    }
    else
      title_menu_index = title_elements.size() - 1;
  }
}

/* Render the TitleElements */
void TitleScreen::renderTitleElements(SDL_Renderer* renderer)
{
  // TODO
  SDL_Color kCOLOR_TEXT = {255, 255, 255, 255};

  if(config)
  {
    auto font = config->getFontTTF(FontName::M_TITLE_ELM);
    auto centre_x = config->getScreenWidth() / 2;
    auto centre_y = 2 * config->getScreenHeight() / 3;

    // TODO
    Coordinate current{centre_x, centre_y};

    /* Title Element Box Rendering */
    title_element_box.width = 1 * config->getScreenWidth() / 7;
    title_element_box.height = 1 * config->getScreenHeight() / 4;
    title_element_box.point.x = centre_x - title_element_box.width / 2;
    title_element_box.point.y = centre_y - title_element_box.height / 5;
    title_element_box.color_border = {255, 255, 255, 255};
    title_element_box.color_bg = {0, 0, 0, 200};
    title_element_box.render(renderer);

    /* */
    current.y -= 5;

    if(font)
    {
      int32_t i = 0;

      for(auto& element : title_elements)
      {
        Text t_element(font);
        t_element.setText(renderer, element.name, kCOLOR_TEXT);
        t_element.render(renderer, current.x - t_element.getWidth() / 2,
                         current.y - t_element.getHeight() / 2);

        // TODO
        if(i != -1 && i == title_menu_index)
        {
          SDL_Rect rect;
          rect.w = t_element.getWidth() + 10;
          rect.h = t_element.getHeight() + 10;
          rect.x = current.x - rect.w / 2;
          rect.y = current.y - rect.h / 2;
          Frame::setRenderDrawColor(renderer, {150, 150, 150, 125});
          SDL_RenderFillRect(renderer, &rect);
        }

        current.y += (int32_t)std::round(t_element.getHeight() * 1.425);

        /* Increment the index */
        i++;
      }
    }
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void TitleScreen::buildTitleBackground(SDL_Renderer* renderer)
{
#ifndef TITLE_SKIP
  title_background.buildSprites(config, renderer);
#endif
}

/* Enables or disables the view. This includes any initialization for before
 * or after it was visible */
void TitleScreen::enableView(bool enable)
{
  /* Enables all relevant control for the view - not used */
  if(enable)
  {
    firstUpdate();
  }
}

/* First update call each time the view changes - must be called */
void TitleScreen::firstUpdate()
{
  /* Music trigger, if relevant. Otherwise, stop tunes */
  if(sound_handler)
  {
    Sound* title_music = sound_handler->getAudioMusic(Sound::kID_MUSIC_TITLE);

    if(title_music)
    {
      sound_handler->addPlayToQueue(Sound::kID_MUSIC_TITLE,
                                    SoundChannels::MUSIC1);
    }
    else
      sound_handler->addStopToQueue(SoundChannels::MUSIC1);
  }
}

/* Current main MenuType enumerated value */
MenuType TitleScreen::getActiveTitleMenu()
{
  return this->menu_type;
}

bool TitleScreen::getFlag(const TitleState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/* The KeyDown event handler -- sends keys to specific functions */
void TitleScreen::keyDownEvent(KeyHandler& key_handler)
{
  if(config && sound_handler)
  {
    /* Main code items */
    if(key_handler.isDepressed(GameKey::ACTION))
      keyDownAction();
    if(key_handler.isDepressed(GameKey::CANCEL))
      keyDownCancel();
    if(key_handler.isDepressed(GameKey::MOVE_DOWN))
      keyDownDown();
    if(key_handler.isDepressed(GameKey::MOVE_LEFT))
      keyDownLeft();
    if(key_handler.isDepressed(GameKey::MOVE_RIGHT))
      keyDownRight();
    if(key_handler.isDepressed(GameKey::MOVE_UP))
      keyDownUp();
  }
}

/* Key down event for test keys - isolated from key handler system */
#ifdef UDEBUG
void TitleScreen::keyTestDownEvent(SDL_KeyboardEvent event)
{
#ifndef TITLE_SKIP
  /* Hide Text & logo from title */
  if(event.keysym.sym == SDLK_F1)
    title_background.render_disabled = !title_background.render_disabled;
#else
  /* Process warning if TitleSkip is not assigned */
  (void)event;
#endif
}
#endif

/* Renders the title screen */
bool TitleScreen::render(SDL_Renderer* renderer)
{
/* Update the title background if it's enabled */
#ifndef TITLE_SKIP
  title_background.render(renderer);
#endif

  renderTitleElements(renderer);

  return true;
}

/* Sets the running configuration, from the options class */
bool TitleScreen::setConfig(Options* config)
{
  this->config = config;

  return (this->config != nullptr);
}

void TitleScreen::setFlag(TitleState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

/* Sets the sound handler used. If unset, no sounds will play */
bool TitleScreen::setSoundHandler(SoundHandler* sound_handler)
{
  this->sound_handler = sound_handler;

  return (this->sound_handler != nullptr);
}

/* Updates the title screen. Necessary for visual updates */
bool TitleScreen::update(int32_t cycle_time)
{
/* Update the tile screen if it's enabled */
#ifndef TITLE_SKIP
  title_background.update(cycle_time);
#else
  (void)cycle_time; // TODO
#endif
  return true;
  // return menu_type != MenuType::INVALID;
}
