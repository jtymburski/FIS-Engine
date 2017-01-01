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

bool TitleBackground::render(SDL_Renderer* renderer, Options* config)
{
  bool success = false;

  if(renderer)
  {
    success = true;

    if(config)
    {

      auto x_offset = config->getScreenWidth() - Options::kDEF_SCREEN_WIDTH;
      auto y_offset = config->getScreenHeight() - Options::kDEF_SCREEN_HEIGHT;

      /* Render the background */
      success &= background.render(renderer, 0, 0, config->getScreenWidth(),
                                   config->getScreenHeight());
      success &= background6.render(renderer, -220 + x_offset, -48 + y_offset);

      success &= background2.render(renderer, 153 + x_offset, 314 + y_offset);

      /* Waldo show - TODO: Only certain hours? */
      success &= background7.render(renderer, 153 + x_offset, 314 + y_offset);

      /* The flash and bangs */
      success &= background4.render(renderer, 153 + x_offset, 314 + y_offset);
      success &= background5.render(renderer, 153 + x_offset, 314 + y_offset);

      /* Render atmosphere */
      success &= background3.render(renderer, 153 + x_offset, 314 + y_offset);
    }

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

const SDL_Color TitleScreen::kCOLOR_BACKGROUND{0, 0, 0, 200};
const SDL_Color TitleScreen::kCOLOR_BORDER{255, 255, 255, 255};
const SDL_Color TitleScreen::kCOLOR_SELECT{125, 125, 125, 75};
const SDL_Color TitleScreen::kCOLOR_TEXT{255, 255, 255, 255};
const SDL_Color TitleScreen::kCOLOR_TEXT_INVALID{200, 100, 100, 255};

const float TitleScreen::kSAVE_GAP{0.016};
const float TitleScreen::kSAVE_ELEMENT_WIDTH{0.48};
const float TitleScreen::kSAVE_ELEMENT_HEIGHT{0.15};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

TitleScreen::TitleScreen(Options* config)
    : config{config},
      flags{static_cast<TitleState>(0)},
      menu_layer{MenuLayer::TITLE},
      menu_type{MenuType::INVALID},
      player_name_select{""},
      player_sex_select{Sex::FEMALE},
      load_element_index{-1},
      load_state{MenuSaveState::NONE},
      save_scroll_box{Box()},
      sound_handler{nullptr},
      title_elements{},
      title_menu_index{-1},
      player_menu_index{-1}
{
  buildTitleElements();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void TitleScreen::buildSave(SDL_Renderer* renderer)
{
  auto gap = (int32_t)std::round(config->getScaledWidth() * kSAVE_GAP);
  auto save_width = config->getScaledWidth() * kSAVE_ELEMENT_WIDTH;
  auto save_height = config->getScaledHeight() * kSAVE_ELEMENT_HEIGHT;

  /* Refresh elements for the Save scroll box */
  std::vector<Frame*> save_frames;
  save_scroll_box.clearElements();

  for(auto& save : save_data)
  {
    save.location.color_bg = {0, 0, 0, 255};
    save.location.color_bg_selected = {0, 0, 0, 255};
    save.location.color_border = {46, 46, 46, 255};
    save.location.color_border_selected = {255, 255, 255, 255};
    save.location.width = save_width;
    save.location.height = save_height;

    auto new_texture = save.createRenderFrame(renderer);

    if(new_texture)
    {
      save_frames.push_back(new Frame());
      save_frames.back()->setTexture(new_texture);
    }
    else
    {
      std::cerr << "[Error] Creating render frame for Save file." << std::endl;
    }
  }

  save_scroll_box.setElements(save_frames);
}

/* Create the main menu title elements */
void TitleScreen::buildTitleElements()
{
  /* Clear the title elements to begin */
  title_elements.clear();

  /* Create tNew Game element */
  auto elm = TitleElement(Helpers::menuTypeToStr(MenuType::TITLE_PLAYER_SELECT),
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

/* Checks if the entered player name is valid for game selection */
bool TitleScreen::isPlayerNameValid(KeyHandler& key_handler)
{
  auto player_name = key_handler.getTextEntry();
  auto trim_name = key_handler.getTextEntry();
  Helpers::trim(trim_name);

  bool valid{true};

  valid &= player_name != "";
  valid &= (player_name == trim_name);
  valid &= (player_name != "ARCADIUS");
  valid &= (player_name != "MALGIDUS");
  valid &= (player_name != "FROSTY");
  valid &= (player_name != "ATKST");
  valid &= (player_name != "CHUBBY");
  valid &= (player_name != "KEVIN");
  valid &= (player_name != "DAVE");

  return valid;
}

/* Processing for the Action key */
void TitleScreen::keyDownAction(SDL_Renderer* renderer, KeyHandler& key_handler)
{
  if(menu_layer == MenuLayer::TITLE)
  {
    if(title_menu_index != -1 &&
       (uint32_t)title_menu_index < title_elements.size())
    {
      auto title_menu_type = title_elements.at(title_menu_index).menu_type;

      menu_layer = MenuLayer::MAIN;

      /* If new game was selected, set the KeyHandler to text entry mode */
      if(title_menu_type == MenuType::TITLE_NEW_GAME)
      {
        menu_type = MenuType::TITLE_PLAYER_SELECT;
        player_menu_index = 0;

        /* Set the clear name to true to purge first key press (this one) */
        setFlag(TitleState::CLEAR_NAME);
      }
      else
      {
        menu_type = title_menu_type;

        if(menu_type == MenuType::TITLE_LOAD_GAME)
        {
          buildSave(renderer);
        }
      }

      /* Play sound */
      sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS, true);
    }

    /* If the current menu type is now TITLE_QUIT, set flag to quit the game */
    if(menu_type == MenuType::TITLE_QUIT)
      setFlag(TitleState::EXIT_GAME);
  }
  else if(menu_layer == MenuLayer::MAIN)
  {
    /* Start the Game if at player selection and the player has a name */
    if(menu_type == MenuType::TITLE_PLAYER_SELECT &&
       isPlayerNameValid(key_handler) && player_menu_index == 2)
    {
      player_name_select = Helpers::titleCase(key_handler.getTextEntry());
      key_handler.clearTextEntry();
      setFlag(TitleState::GO_TO_GAME);
    }
  }
}

/* Processing for the Cancel key */
void TitleScreen::keyDownCancel(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING

  if(menu_layer == MenuLayer::TITLE)
  {
    /* Play sound */
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_PREV,
                                  SoundChannels::MENUS);
  }
  else if(menu_layer == MenuLayer::MAIN)
  {
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_PREV,
                                  SoundChannels::MENUS);

    menu_type = MenuType::INVALID;
    menu_layer = MenuLayer::TITLE;
    player_menu_index = -1;
  }
}

/* Processing for the Down key */
void TitleScreen::keyDownDown(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING

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
  else if(menu_layer == MenuLayer::MAIN)
  {
    if(player_menu_index + 1 < 3)
    {
      player_menu_index++;
    }
  }
}

/* Processing for the Left key */
void TitleScreen::keyDownLeft(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING

  if(menu_layer == MenuLayer::MAIN)
  {
    if(player_menu_index == 1)
      player_sex_select = Sex::FEMALE;
  }
}

/* Processing for the Right key */
void TitleScreen::keyDownRight(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING

  if(menu_layer == MenuLayer::MAIN)
  {
    if(player_menu_index == 1)
      player_sex_select = Sex::MALE;
  }
}

/* Processing for the Up key */
void TitleScreen::keyDownUp(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING
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
  else if(menu_layer == MenuLayer::MAIN)
  {
    /* Decrement the menu index on the Player box */
    if(player_menu_index > 0)
      player_menu_index--;
  }
}

/* Render the TitleElements */
void TitleScreen::renderTitleElements(SDL_Renderer* renderer)
{
  if(config)
  {
    auto font = config->getFontTTF(FontName::M_TITLE_ELM);
    auto centre_x = config->getScreenWidth() / 2;
    auto centre_y = 2 * config->getScreenHeight() / 3;

    current = Coordinate{centre_x, centre_y};

    /* Title Element Box Rendering */
    title_element_box.width = 1 * config->getScaledWidth() / 6;
    title_element_box.height = 1 * config->getScaledHeight() / 4;
    title_element_box.point.x = centre_x - title_element_box.width / 2;
    title_element_box.point.y = centre_y - title_element_box.height / 5;
    title_element_box.color_border = kCOLOR_BORDER;
    title_element_box.color_bg = kCOLOR_BACKGROUND;
    title_element_box.render(renderer);

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
          Frame::renderRectSelect(
              getRect(current, t_element.getHeight(), t_element.getWidth()),
              renderer, kCOLOR_SELECT);
        }

        current.y += (int32_t)std::round(t_element.getHeight() * 1.425);

        /* Increment the index */
        i++;
      }
    }
  }
}

SDL_Rect TitleScreen::getRect(Coordinate current, int32_t height, int32_t width)
{
  SDL_Rect rect;
  rect.w = width + 10;
  rect.h = height + 10;
  rect.x = current.x - rect.w / 2;
  rect.y = current.y - rect.h / 2;

  return rect;
}

bool TitleScreen::updateKeyHandler(KeyHandler& key_handler)
{
  if(menu_layer == MenuLayer::MAIN)
  {
    if(player_menu_index == 0)
    {
      key_handler.setMode(KeyMode::NAME_ENTRY);
    }
    else
      key_handler.setMode(KeyMode::INPUT);
  }
  else
    key_handler.setMode(KeyMode::INPUT);

  return false;
}

void TitleScreen::renderPlayerSelection(SDL_Renderer* renderer,
                                        KeyHandler& key_handler)
{
  /* Grab the current player name entered */
  auto player_name = Helpers::titleCase(key_handler.getTextEntry());
  auto name_valid = isPlayerNameValid(key_handler);

  if(config)
  {
    auto font = config->getFontTTF(FontName::M_TITLE_ELM);
    auto centre_x = config->getScreenWidth() / 2;
    auto centre_y = config->getScreenHeight() / 2;

    player_selection_box.width = config->getScaledWidth() / 3;
    player_selection_box.height = config->getScaledHeight() / 4;
    player_selection_box.point.x = centre_x - player_selection_box.width / 2;
    player_selection_box.point.y = centre_y - player_selection_box.height / 2;
    player_selection_box.color_border = kCOLOR_BORDER;
    player_selection_box.color_bg = kCOLOR_BACKGROUND;
    player_selection_box.render(renderer);

    current = {player_selection_box.point.x + 10,
               player_selection_box.point.y + 40};

    Coordinate start = current;

    if(font)
    {
      Text t_player_title(font);
      Text t_player_name(font);
      Text t_female(font);
      Text t_male(font);
      Text t_begin(font);

      SDL_Color name_color = kCOLOR_TEXT;

      if(!name_valid)
        name_color = kCOLOR_TEXT_INVALID;

      t_player_title.setText(renderer, "Player Name: ", name_color);
      t_player_name.setText(renderer, player_name, name_color);
      t_player_title.render(renderer, current.x, current.y);
      current.x += t_player_title.getWidth();

      t_player_name.render(renderer, current.x, current.y);
      current.x = player_selection_box.point.x + 10;
      current.y += player_selection_box.height / 4;

      if(player_menu_index == 0)
      {
        auto width = t_player_title.getWidth() + t_player_name.getWidth();
        auto rect = getRect(
            {start.x + width / 2, start.y + t_player_title.getHeight() / 2},
            t_player_title.getHeight(), width);

        Frame::renderRectSelect(rect, renderer, kCOLOR_SELECT);
      }

      t_female.setText(renderer, "Female", kCOLOR_TEXT);
      current.x = centre_x - player_selection_box.width / 3;
      t_female.render(renderer, current.x, current.y);

      if(player_sex_select == Sex::FEMALE)
      {
        auto rect = getRect({current.x + t_female.getWidth() / 2,
                             current.y + t_female.getHeight() / 2},
                            t_female.getHeight(), t_female.getWidth());

        SDL_Color color = kCOLOR_SELECT;

        if(player_menu_index == 1)
          color = {125, 125, 125, 125};

        Frame::renderRectSelect(rect, renderer, color);
      }

      t_male.setText(renderer, "Male", kCOLOR_TEXT);
      current.x = centre_x + player_selection_box.width / 3 - t_male.getWidth();
      t_male.render(renderer, current.x, current.y);

      if(player_sex_select == Sex::MALE)
      {
        auto rect = getRect({current.x + t_male.getWidth() / 2,
                             current.y + t_male.getHeight() / 2},
                            t_male.getHeight(), t_male.getWidth());

        SDL_Color color = kCOLOR_SELECT;

        if(player_menu_index == 1)
          color = {125, 125, 125, 125};

        Frame::renderRectSelect(rect, renderer, color);
      }

      t_begin.setText(renderer, "Begin", kCOLOR_TEXT);
      current.y += player_selection_box.height / 4;
      current.x = centre_x - t_begin.getWidth() / 2;
      t_begin.render(renderer, current.x, current.y);

      if(player_menu_index == 2)
      {
        auto rect = getRect({current.x + t_begin.getWidth() / 2,
                             current.y + t_begin.getHeight() / 2},
                            t_begin.getHeight(), t_begin.getWidth());

        Frame::renderRectSelect(rect, renderer, kCOLOR_SELECT);
      }
    }
  }
}

void TitleScreen::renderLoadSelection(SDL_Renderer* renderer)
{
  if(renderer && config)
  {
    auto gap = (int32_t)std::round(config->getScaledWidth() * kSAVE_GAP);
    auto save_width = config->getScaledWidth() * kSAVE_ELEMENT_WIDTH;
    auto save_height = config->getScaledHeight() * kSAVE_ELEMENT_HEIGHT;

    Coordinate current =
        Coordinate{config->getScreenWidth() / 2 - save_width / 2,
                   config->getScreenHeight() / 2 - save_height / 2};

    save_scroll_box.setFlag(BoxState::SCROLL_BOX);
    save_scroll_box.setFlag(BoxState::SELECTABLE);

    save_scroll_box.color_border = {255, 255, 255, 255};
    save_scroll_box.color_border_selected = {0, 0, 0, 0};

    save_scroll_box.color_element_border_selected = {255, 255, 255, 255};
    save_scroll_box.color_element_border = {125, 125, 125, 255};
    save_scroll_box.color_bg = {0, 0, 0, 255};
    save_scroll_box.color_bg_selected = {0, 0, 0, 0};

    save_scroll_box.point.x = current.x;
    save_scroll_box.point.y = current.y;
    save_scroll_box.height = save_height * 3 + 3 * gap;
    save_scroll_box.width = save_width + 2 * gap;
    save_scroll_box.element_gap = gap;

    save_scroll_box.render(renderer);

    // if(layer == MenuLayer::POPUP)
    // {
    //   Box save_popup_box;
    //   setupDefaultBox(save_popup_box);
    //   save_popup_box.color_border = {255, 255, 255, 255};

    //   auto p_width = std::round(config->getScaledWidth() *
    //   kSAVE_POPUP_WIDTH);
    //   auto p_height = std::round(config->getScaledHeight() *
    //   kSAVE_POPUP_HEIGHT);
    //   auto popup_gap = std::round(config->getScaledHeight() *
    //   kSAVE_POPUP_GAP);

    //   save_popup_box.point.x = main.point.x + main.width / 2 - p_width / 2;
    //   save_popup_box.point.y = main.point.y + main.height / 2 - p_height / 2;
    //   save_popup_box.width = p_width;
    //   save_popup_box.height = p_height;

    //   save_popup_box.render(renderer);

    //   Text t_cancel(getFont(FontName::M_HEADER));
    //   Text t_save(getFont(FontName::M_HEADER));
    //   Text t_delete(getFont(FontName::M_HEADER));

    //   t_cancel.setText(renderer, "Cancel", kCOLOR_TEXT);
    //   t_save.setText(renderer, "Save", kCOLOR_TEXT);
    //   t_delete.setText(renderer, "Delete", kCOLOR_TEXT);

    //   SDL_Rect rect;
    //   rect.w = std::max(t_cancel.getWidth(), t_save.getWidth());
    //   rect.w = std::max(rect.w, t_delete.getWidth()) * 1.45;
    //   rect.h = t_delete.getHeight() * 1.45;

    //   current.x = save_popup_box.point.x;
    //   current.y = save_popup_box.point.y + popup_gap / 2;

    //   t_cancel.render(renderer, current.x + p_width / 2 - t_cancel.getWidth()
    //   / 2,
    //                   current.y);

    //   if(save_element_index == 1)
    //   {
    //     rect.x = current.x + p_width / 2 - rect.w / 2;
    //     rect.y = current.y + t_cancel.getHeight() / 2 - rect.h / 2;

    //     Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
    //   }

    //   current.y += popup_gap;
    //   t_save.render(renderer, current.x + p_width / 2 - t_save.getWidth() /
    //   2,
    //                 current.y);

    //   if(save_element_index == 2)
    //   {
    //     rect.x = current.x + p_width / 2 - rect.w / 2;
    //     rect.y = current.y + t_save.getHeight() / 2 - rect.h / 2;

    //     Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
    //   }

    //   current.y += popup_gap;
    //   t_delete.render(renderer, current.x + p_width / 2 - t_delete.getWidth()
    //   / 2,
    //                   current.y);

    //   if(save_element_index == 3)
    //   {
    //     rect.x = current.x + p_width / 2 - rect.w / 2;
    //     rect.y = current.y + t_delete.getHeight() / 2 - rect.h / 2;

    //     Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
    //   }
    // }
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void TitleScreen::buildTitleBackground(SDL_Renderer* renderer)
{
#ifndef TITLE_SKIP
  title_background.buildSprites(config, renderer);
#else
  (void)renderer;
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

/* Return player Name selection */
std::string TitleScreen::getPlayerNameSelect()
{
  return player_name_select;
}

/* Return player Sex selection */
Sex TitleScreen::getPlayerSexSelect()
{
  return player_sex_select;
}

/* The KeyDown event handler -- sends keys to specific functions */
void TitleScreen::keyDownEvent(SDL_Renderer* renderer, KeyHandler& key_handler)
{
  if(config && sound_handler && !getFlag(TitleState::CLEAR_NAME))
  {
    /* Main code items */
    if(key_handler.isDepressed(GameKey::ACTION))
      keyDownAction(renderer, key_handler);
    if(key_handler.isDepressed(GameKey::CANCEL))
      keyDownCancel(key_handler);
    if(key_handler.isDepressed(GameKey::MOVE_DOWN))
      keyDownDown(key_handler);
    if(key_handler.isDepressed(GameKey::MOVE_LEFT))
      keyDownLeft(key_handler);
    if(key_handler.isDepressed(GameKey::MOVE_RIGHT))
      keyDownRight(key_handler);
    if(key_handler.isDepressed(GameKey::MOVE_UP))
      keyDownUp(key_handler);
  }
}

/* Key down event for test keys - isolated from key handler system */
#ifdef UDEBUG
void TitleScreen::keyTestDownEvent(SDL_KeyboardEvent event)
{
  (void)event;
}
#endif

/* Renders the title screen */
bool TitleScreen::render(SDL_Renderer* renderer, KeyHandler& key_handler)
{
/* Update the title background if it's enabled */
#ifndef TITLE_SKIP
  title_background.render(renderer, config);
#endif

  if(menu_layer == MenuLayer::TITLE)
  {
    renderTitleElements(renderer);
  }
  else if(menu_layer == MenuLayer::MAIN)
  {
    if(menu_type == MenuType::TITLE_PLAYER_SELECT)
    {
      if(!getFlag(TitleState::CLEAR_NAME))
        renderPlayerSelection(renderer, key_handler);
      else
        renderTitleElements(renderer);
    }
    else if(menu_type == MenuType::TITLE_LOAD_GAME)
    {
      renderLoadSelection(renderer);
    }
  }

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

void TitleScreen::setSaveData(std::vector<Save> saves)
{
  this->save_data = saves;
}

/* Sets the sound handler used. If unset, no sounds will play */
bool TitleScreen::setSoundHandler(SoundHandler* sound_handler)
{
  this->sound_handler = sound_handler;

  return (this->sound_handler != nullptr);
}

/* Updates the title screen. Necessary for visual updates */
bool TitleScreen::update(int32_t cycle_time, KeyHandler& key_handler)
{
/* Update the tile screen if it's enabled */
#ifndef TITLE_SKIP
  title_background.update(cycle_time);
#else
  (void)cycle_time; // TODO
#endif

  if(getFlag(TitleState::CLEAR_NAME) &&
     !key_handler.isDepressed(GameKey::ACTION))
  {
    key_handler.clearTextEntry();
    setFlag(TitleState::CLEAR_NAME, false);
  }

  /* Update Key Handler state - INPUT vs. TEXT_ENTRY state */
  updateKeyHandler(key_handler);

  return true;
}
