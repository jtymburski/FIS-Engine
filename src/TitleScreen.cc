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
 * TITLE_BACKGROUND
 *============================================================================*/

/* Construct the TitleBackground Class */
TitleBackground::TitleBackground()
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
    }

    /* Render the title */
    success &= title.render(renderer, 50, 50);
  }

  return success;
}

void TitleBackground::buildSprites(Options* config, SDL_Renderer* renderer)
{
  if(config && renderer)
  {
    background.insertFirst(
        config->getBasePath() + "assets/images/backgrounds/title.png", renderer);

    title.setFont(config->getFontTTF(FontName::TITLE_MAIN_HEADER));
    title.setText(renderer, "Real Title 2: End of the Seven Seas", {255, 255, 255, 255});
  }
}

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const SDL_Color TitleScreen::kCOLOR_BACKGROUND{0, 0, 0, 200};
const SDL_Color TitleScreen::kCOLOR_BORDER{255, 255, 255, 255};
const SDL_Color TitleScreen::kCOLOR_SELECT{125, 125, 125, 75};
const SDL_Color TitleScreen::kCOLOR_TEXT{255, 255, 255, 255};
const SDL_Color TitleScreen::kCOLOR_TEXT_INVALID{200, 100, 100, 255};
const SDL_Color TitleScreen::kCOLOR_TITLE_HOVER{255, 255, 255, 65};

const float TitleScreen::kOPTIONS_GAP{0.011};
const float TitleScreen::kOPTIONS_ELEMENT_HEIGHT{0.05};
const float TitleScreen::kOPTIONS_HEIGHT{0.6};
const float TitleScreen::kOPTIONS_WIDTH{0.3};

const float TitleScreen::kSAVE_GAP{0.014};
const float TitleScreen::kSAVE_ELEMENT_WIDTH{0.60};
const float TitleScreen::kSAVE_ELEMENT_HEIGHT{0.21};
const float TitleScreen::kSAVE_POPUP_WIDTH{0.12};
const float TitleScreen::kSAVE_POPUP_GAP{0.058};
const float TitleScreen::kSAVE_POPUP_HEIGHT{0.21};

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
      sound_handler{nullptr},
      title_elements{},
      option_title_elements{},
      title_element_box{Box()},
      option_element_box{Box()},
      option_menu_box{Box()},
      player_selection_box{Box()},
      save_scroll_box{Box()},
      title_menu_index{-1},
      option_menu_index{-1},
      option_element_index{-1},
      player_menu_index{-1}
{
  buildTitleElements();
  buildOptionTitleElements();
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

void TitleScreen::buildOptionTitleElements()
{
  option_title_elements.clear();

  auto elm = TitleElement(Helpers::menuTypeToStr(MenuType::TITLE_OPTIONS_MAIN),
                          true, MenuType::TITLE_OPTIONS_MAIN);
  option_title_elements.push_back(elm);

  elm = TitleElement(Helpers::menuTypeToStr(MenuType::TITLE_OPTIONS_CONTROLS),
                     true, MenuType::TITLE_OPTIONS_CONTROLS);
  option_title_elements.push_back(elm);
}

/* Checks if the entered player name is valid for game selection */
bool TitleScreen::isPlayerNameValid(KeyHandler& key_handler)
{
  auto player_name = key_handler.getTextEntry();
  auto trim_name = Helpers::trim(key_handler.getTextEntry());

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
  bool play_next_sound = false;
  bool play_prev_sound = false;

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
      else if(title_menu_type == MenuType::TITLE_LOAD_GAME)
      {
        menu_type = title_menu_type;
        buildSave(renderer);
      }
      else if(title_menu_type == MenuType::TITLE_OPTIONS)
      {
        menu_type = MenuType::TITLE_OPTIONS;
        option_menu_index = 0;
        option_element_index = 0;
      }
      else if(title_menu_type == MenuType::TITLE_QUIT)
        setFlag(TitleState::EXIT_GAME);

      play_next_sound = true;
    }
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

      menu_layer = MenuLayer::TITLE;
      player_menu_index = -1;
    }
    else if(menu_type == MenuType::TITLE_LOAD_GAME)
    {
      menu_layer = MenuLayer::POPUP;
      load_element_index = 0;
    }
    else if(menu_type == MenuType::TITLE_OPTIONS)
    {
      if(option_menu_index == 1)
      {
        auto& key = getSelectedKey(key_handler);

        if(key.state == GameKeyState::READY)
        {
          key.state = GameKeyState::ASSIGNING;
          setFlag(TitleState::SELECTING_PRIMARY, true);
          play_next_sound = true;
        }
      }
    }
  }
  else if(menu_layer == MenuLayer::POPUP)
  {
    if(load_element_index == 0)
    {
      menu_layer = MenuLayer::MAIN;
      load_element_index = -1;
      play_prev_sound = true;
    }
    else if(load_element_index == 1)
    {
      /* Load the Game from Save */
      setFlag(TitleState::LOAD_FROM_SAVE);
      load_element_index = -1;
      menu_layer = MenuLayer::TITLE;
      play_next_sound = true;
    }
    else if(load_element_index == 2)
    {
      /* Delete that Save */
      setFlag(TitleState::DELETE_SAVE);
      load_element_index = -1;
      menu_layer = MenuLayer::MAIN;
      play_next_sound = true;
    }
  }

  if(sound_handler)
  {
    if(play_next_sound)
    {
      sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
    }
    else if(play_prev_sound)
    {
      sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_PREV,
                                    SoundChannels::MENUS);
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
    option_menu_index = -1;
  }
  else if(menu_layer == MenuLayer::POPUP)
  {
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_PREV,
                                  SoundChannels::MENUS);

    menu_layer = MenuLayer::MAIN;
    load_element_index = -1;
  }
}

/* Processing for the Down key */
void TitleScreen::keyDownDown(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING
  bool play_sound = false;

  if(menu_layer == MenuLayer::TITLE)
  {
    if(title_menu_index + 1 < (int32_t)title_elements.size())
    {
      title_menu_index++;
      play_sound = true;
    }
    else
    {
      title_menu_index = 0;
      play_sound = true;
    }
  }
  else if(menu_layer == MenuLayer::MAIN)
  {
    if(menu_type == MenuType::TITLE_PLAYER_SELECT)
    {
      if(player_menu_index + 1 < 3)
      {
        player_menu_index++;
        play_sound = true;
      }
    }
    else if(menu_type == MenuType::TITLE_LOAD_GAME)
    {
      if(save_scroll_box.nextIndex())
        play_sound = true;
    }
    else if(menu_type == MenuType::TITLE_OPTIONS)
    {
      if(option_element_index + 1 < (int32_t)key_handler.getGameKeys().size())
      {
        option_element_index++;
        play_sound = true;
      }
    }
  }
  else if(menu_layer == MenuLayer::POPUP)
  {
    if(load_element_index + 1 < 3)
    {
      load_element_index++;
      play_sound = true;
    }
  }

  if(sound_handler && play_sound)
  {
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_CHG,
                                  SoundChannels::MENUS);
  }
}

/* Processing for the Left key */
void TitleScreen::keyDownLeft(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING

  bool play_sound = false;

  if(menu_layer == MenuLayer::MAIN)
  {
    if(player_menu_index == 1)
    {
      player_sex_select = Sex::FEMALE;
      play_sound = true;
    }

    if(option_menu_index == 1)
    {
      option_menu_index--;
      play_sound = true;
    }
  }

  if(sound_handler && play_sound)
  {
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_CHG,
                                  SoundChannels::MENUS, true);
  }
}

/* Processing for the Right key */
void TitleScreen::keyDownRight(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING

  bool play_sound = false;

  if(menu_layer == MenuLayer::MAIN)
  {
    if(player_menu_index == 1)
    {
      player_sex_select = Sex::MALE;
      play_sound = true;
    }

    if(option_menu_index == 0)
    {
      option_menu_index++;
      play_sound = true;
    }
  }

  if(sound_handler && play_sound)
  {
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_CHG,
                                  SoundChannels::MENUS, true);
  }
}

/* Processing for the Up key */
void TitleScreen::keyDownUp(KeyHandler& key_handler)
{
  (void)key_handler; // WARNING

  bool play_sound = false;

  if(menu_layer == MenuLayer::TITLE)
  {
    if(title_menu_index > 0)
    {
      title_menu_index--;
      play_sound = true;
    }
    else
    {
      title_menu_index = title_elements.size() - 1;
      play_sound = true;
    }
  }
  else if(menu_layer == MenuLayer::MAIN)
  {
    if(menu_type == MenuType::TITLE_PLAYER_SELECT)
    {
      /* Decrement the menu index on the Player box */
      if(player_menu_index > 0)
      {
        player_menu_index--;
        play_sound = true;
      }
    }
    else if(menu_type == MenuType::TITLE_LOAD_GAME)
    {
      if(save_scroll_box.prevIndex())
        play_sound = true;
    }
    else if(menu_type == MenuType::TITLE_OPTIONS)
    {
      if(option_element_index > 0)
      {
        option_element_index--;
        play_sound = true;
      }
    }
  }
  else if(menu_layer == MenuLayer::POPUP)
  {
    if(load_element_index > 0)
    {
      load_element_index--;
      play_sound = true;
    }
  }

  if(sound_handler && play_sound)
  {
    sound_handler->addPlayToQueue(Sound::kID_SOUND_MENU_CHG,
                                  SoundChannels::MENUS);
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

Key& TitleScreen::getSelectedKey(KeyHandler& key_handler)
{
  if(option_element_index > -1 &&
     option_element_index < (int32_t)key_handler.getGameKeys().size() &&
     option_menu_index == 1)
  {
    return key_handler.getKey(
        key_handler.getGameKeys().at(option_element_index));
  }

  return key_handler.getKey(GameKey::ACTION);
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
      key_handler.setMode(KeyMode::NAME_ENTRY);
    else
      key_handler.setMode(KeyMode::INPUT);
  }
  else
    key_handler.setMode(KeyMode::INPUT);

  return false;
}

void TitleScreen::renderOptions(SDL_Renderer* renderer, KeyHandler& key_handler)
{
  if(config)
  {
    auto centre_x = config->getScreenWidth() / 2;
    auto centre_y = config->getScreenHeight() / 2;

    auto width = (int32_t)std::round(config->getScaledWidth() * kOPTIONS_WIDTH);
    auto height =
        (int32_t)std::round(config->getScaledHeight() * kOPTIONS_HEIGHT);

    option_menu_box.height = height;
    option_menu_box.width = width;
    option_menu_box.point.x = centre_x - option_menu_box.width / 2;
    option_menu_box.point.y = centre_y - option_menu_box.height / 2;
    option_menu_box.color_border = kCOLOR_BORDER;
    option_menu_box.color_bg = kCOLOR_BACKGROUND;
    option_menu_box.render(renderer);

    auto gap = (int32_t)std::round(width * kOPTIONS_GAP);

    option_menu_box.render(renderer);

    renderOptionElementTitles(renderer, gap);

    if(option_menu_index == 0)
      renderOptionsMain(renderer, key_handler);
    else if(option_menu_index == 1)
      renderOptionsControls(renderer, key_handler);
  }
}

void TitleScreen::renderOptionsMain(SDL_Renderer* renderer,
                                    KeyHandler& key_handler)
{
  if(config)
  {
  }
}

void TitleScreen::renderOptionElementTitles(SDL_Renderer* renderer, int32_t gap)
{
  if(config && renderer && option_menu_index > -1 &&
     option_menu_index < (int32_t)option_title_elements.size() &&
     option_title_elements.size() > 0)
  {
    /*Render the required number of boxes for the element titles */
    auto point = Coordinate{option_menu_box.point.x, option_menu_box.point.y};
    auto box_length = (int32_t)std::ceil(option_menu_box.width * 1.0 /
                                         option_title_elements.size());
    auto box_height = (int32_t)std::round(config->getScaledHeight() *
                                          kOPTIONS_ELEMENT_HEIGHT);

    int32_t index{0};
    int32_t max_remain_length = option_menu_box.width + 2;

    for(auto& title : option_title_elements)
    {
      auto actual_length = std::min(max_remain_length, box_length);
      title.title_box = Box(point, actual_length, box_height);
      max_remain_length -= actual_length - 1;

      title.title_box.color_bg = {0, 0, 0, 255};
      title.title_box.color_bg_selected = {0, 0, 0, 0};
      title.title_box.color_border = {255, 255, 255, 255};
      title.title_box.color_border_selected = {255, 255, 255, 255};

      if(index == option_menu_index)
        title.title_box.color_bg = {46, 46, 46, 255};
      else
        title.title_box.setFlag(BoxState::SELECTED, false);

      point.x += box_length - 1;
      index++;
    }

    for(uint32_t i = 0; i < option_title_elements.size(); i++)
      renderOptionElementTitle(renderer, option_title_elements.at(i));
  }
}

void TitleScreen::renderOptionElementTitle(SDL_Renderer* renderer,
                                           TitleElement& element)
{
  element.title_box.render(renderer);

  Text t(config->getFontTTF(FontName::M_SMALL_TITLE_ELM));

  t.setText(renderer, element.name, kCOLOR_TEXT);

  auto b = element.title_box;

  t.render(renderer, b.point.x + b.width / 2 - t.getWidth() / 2,
           b.point.y + b.height / 2 - t.getHeight() / 2);
}

void TitleScreen::renderOptionsControls(SDL_Renderer* renderer,
                                        KeyHandler& key_handler)
{
  if(config && renderer)
  {
    /* Grab each game key and display the current state of control select */
    auto keys = key_handler.getGameKeys();

    auto gap = (int32_t)std::round(config->getScaledWidth() * kOPTIONS_GAP);
    auto point =
        Coordinate{option_menu_box.point.x, option_menu_box.point.y + 2 * gap};

    if(option_title_elements.size())
      point.y += option_title_elements.at(0).title_box.height;

    auto font = config->getFontTTF(FontName::M_HEADER);
    int32_t index{0};

    Text t_key_name_game_h(font);
    Text t_key_name_prim_h(font);
    Text t_key_name_secd_h(font);

    t_key_name_game_h.setText(renderer, "Action", kCOLOR_TEXT);
    t_key_name_prim_h.setText(renderer, "Primary", kCOLOR_TEXT);
    t_key_name_secd_h.setText(renderer, "Secondary", kCOLOR_TEXT);

    t_key_name_game_h.render(renderer, point.x + gap, point.y);
    t_key_name_prim_h.render(
        renderer, point.x + option_menu_box.width / 3 + gap, point.y);
    t_key_name_secd_h.render(
        renderer, point.x + 2 * option_menu_box.width / 3 + gap, point.y);

    point.y += t_key_name_game_h.getHeight() + 3 * gap / 2;

    for(auto& key : keys)
    {
      auto key_name_game = Helpers::gameKeyToStr(key);
      auto key_name_prim = key_handler.getKeyNamePrim(key, nullptr);
      auto key_name_secd = key_handler.getKeyNameSecd(key, nullptr);

      bool assigning = key_handler.getKey(key).state == GameKeyState::ASSIGNING;

      Text t_key_name_game(font);
      Text t_key_name_prim(font);
      Text t_key_name_secd(font);

      SDL_Color t_color_h = kCOLOR_TEXT;
      SDL_Color t_color_prim = kCOLOR_TEXT;
      SDL_Color t_color_secd = kCOLOR_TEXT;

      /* Change the display of the game Key whether it is being assigned */
      if(key_handler.getKey(key).state == GameKeyState::READY)
      {
        t_color_prim = kCOLOR_TEXT;
        t_color_secd = kCOLOR_TEXT;
      }
      else if(assigning)
      {
        if(getFlag(TitleState::SELECTING_PRIMARY))
          t_color_prim = kCOLOR_TEXT_INVALID;
        if(getFlag(TitleState::SELECTING_SECONDARY))
          t_color_secd = kCOLOR_TEXT_INVALID;
      }

      t_key_name_game.setText(renderer, key_name_game, t_color_h);
      t_key_name_prim.setText(renderer, key_name_prim, t_color_prim);
      t_key_name_secd.setText(renderer, key_name_secd, t_color_secd);

      t_key_name_game.render(renderer, point.x + gap, point.y);
      t_key_name_prim.render(
          renderer, point.x + option_menu_box.width / 3 + gap, point.y);
      t_key_name_secd.render(
          renderer, point.x + 2 * option_menu_box.width / 3 + gap, point.y);

      if(option_element_index == index)
      {
        Coordinate box_point{point.x + gap / 2, point.y - gap / 2};

        Box option_key_box{Box(box_point, option_menu_box.width - gap,
                               t_key_name_secd.getHeight() + gap)};

        if(assigning)
        {
          option_key_box.color_bg = {100, 100, 100, 100};
          option_key_box.color_border = {255, 255, 255, 255};
        }
        else
        {
          option_key_box.color_bg = {75, 75, 75, 75};
          option_key_box.color_border = {100, 100, 100, 255};
        }

        option_key_box.render(renderer);
      }

      index++;
      point.y += t_key_name_game.getHeight() + gap;
    }
  }
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
    save_scroll_box.height = save_height * 3 + 4 * gap;
    save_scroll_box.width = save_width + 2 * gap;
    save_scroll_box.element_gap = gap;
    save_scroll_box.element_inset_y = gap;
    save_scroll_box.element_inset_x = gap;
    save_scroll_box.scroll_inset_y = gap * 1.3;

    save_scroll_box.render(renderer);

    if(menu_layer == MenuLayer::POPUP)
    {
      Box save_popup_box;
      save_popup_box.color_bg = {0, 0, 0, 255};
      save_popup_box.color_bg_selected = {0, 0, 0, 0};
      save_popup_box.color_border = {125, 125, 125, 125};
      save_popup_box.color_border_selected = {255, 255, 255, 255};

      auto p_width = std::round(config->getScaledWidth() * kSAVE_POPUP_WIDTH);
      auto p_height =
          std::round(config->getScaledHeight() * kSAVE_POPUP_HEIGHT);
      auto popup_gap = std::round(config->getScaledHeight() * kSAVE_POPUP_GAP);

      save_popup_box.height = p_height;
      save_popup_box.width = p_width;
      save_popup_box.point.x = save_scroll_box.point.x +
                               save_scroll_box.width / 2 -
                               save_popup_box.width / 2;
      save_popup_box.point.y = save_scroll_box.point.y +
                               save_scroll_box.height / 2 -
                               save_popup_box.height / 2;

      save_popup_box.render(renderer);

      Text t_cancel(config->getFontTTF(FontName::M_HEADER));
      Text t_load(config->getFontTTF(FontName::M_HEADER));
      Text t_delete(config->getFontTTF(FontName::M_HEADER));

      t_cancel.setText(renderer, "Cancel", kCOLOR_TEXT);
      t_load.setText(renderer, "Load", kCOLOR_TEXT);
      t_delete.setText(renderer, "Delete", kCOLOR_TEXT);

      SDL_Rect rect;
      rect.w = std::max(t_cancel.getWidth(), t_load.getWidth());
      rect.w = std::max(rect.w, t_delete.getWidth()) * 1.45;
      rect.h = t_delete.getHeight() * 1.45;

      current.x = save_popup_box.point.x;
      current.y = save_popup_box.point.y + popup_gap / 2;

      t_cancel.render(renderer,
                      current.x + p_width / 2 - t_cancel.getWidth() / 2,
                      current.y);

      if(load_element_index == 0)
      {
        rect.x = current.x + p_width / 2 - rect.w / 2;
        rect.y = current.y + t_cancel.getHeight() / 2 - rect.h / 2;

        Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
      }

      current.y += popup_gap;
      t_load.render(renderer, current.x + p_width / 2 - t_load.getWidth() / 2,
                    current.y);

      if(load_element_index == 1)
      {
        rect.x = current.x + p_width / 2 - rect.w / 2;
        rect.y = current.y + t_load.getHeight() / 2 - rect.h / 2;

        Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
      }

      current.y += popup_gap;
      t_delete.render(renderer,
                      current.x + p_width / 2 - t_delete.getWidth() / 2,
                      current.y);

      if(load_element_index == 2)
      {
        rect.x = current.x + p_width / 2 - rect.w / 2;
        rect.y = current.y + t_delete.getHeight() / 2 - rect.h / 2;

        Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
      }
    }
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void TitleScreen::buildTitleBackground(SDL_Renderer* renderer)
{
  title_background.buildSprites(config, renderer);
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

int32_t TitleScreen::getSaveIndex()
{
  return save_scroll_box.getElementIndex();
}

/* The KeyDown event handler -- sends keys to specific functions */
void TitleScreen::keyDownEvent(SDL_Renderer* renderer, KeyHandler& key_handler)
{
  if(config && sound_handler && !getFlag(TitleState::CLEAR_NAME))
  {
    if(!getFlag(TitleState::SELECTING_PRIMARY) &&
       !getFlag(TitleState::SELECTING_SECONDARY))
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
    else
    {
      auto& key = getSelectedKey(key_handler);
      auto event = key_handler.getLastEvent();

      std::cout << "Last key event: " << SDL_GetKeyName(event.keysym.sym)
                << std::endl;

      if(getFlag(TitleState::SELECTING_PRIMARY))
      {
        auto success =
            key_handler.setKeyPrimary(key.game_key, event.keysym.sym);

        if(success)
        {
          setFlag(TitleState::SELECTING_PRIMARY, false);
          setFlag(TitleState::SELECTING_SECONDARY, true);
        }
      }
      else if(getFlag(TitleState::SELECTING_SECONDARY))
      {
        auto success =
            key_handler.setKeySecondary(key.game_key, event.keysym.sym);

        if(success)
        {
          setFlag(TitleState::SELECTING_SECONDARY, false);
          key.state = GameKeyState::READY;
        }
      }
    }
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
  /* Update the title background */
  title_background.render(renderer, config);

  if(menu_layer == MenuLayer::TITLE)
  {
    renderTitleElements(renderer);
  }
  else if(menu_layer == MenuLayer::MAIN || menu_layer == MenuLayer::POPUP)
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
    else if(menu_type == MenuType::TITLE_OPTIONS)
    {
      renderOptions(renderer, key_handler);
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
