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

/* Bars */
const float Menu::kBAR_VITA_WIDTH{0.2};
const float Menu::kBAR_QTDR_WIDTH{0.15};

/* Title Section */
const uint8_t Menu::kTITLE_ALPHA{255};
const float Menu::kTITLE_HEIGHT{0.77};
const float Menu::kTITLE_WIDTH{0.17};
const float Menu::kTITLE_X_OFFSET{0.02};
const float Menu::kTITLE_ELEMENT_X_OFFSET{0.01};
const float Menu::kTITLE_Y_OFFSET{0.05};
const float Menu::kTITLE_ELEMENT_GAP{0.80};
const float Menu::kTITLE_CORNER_LENGTH{0.02};
const float Menu::kTITLE_SLIDE_RATE{1.1};
const float Menu::kTITLE_LOCATION_Y_OFFSET{0.63};
const float Menu::kTITLE_ICONS_Y_GAP{0.05};
const float Menu::kTITLE_ICON_TEXT_X{0.025};
const float Menu::kTITLE_ICON_TEXT_Y{0.003};
const float Menu::kTITLE_HOVER_OFFSET_X{0.08};
const float Menu::kTITLE_HOVER_WIDTH{0.74};
const float Menu::kTITLE_HOVER_RATE{0.0014};
const float Menu::kTITLE_HOVER_MIN{0.4};
const float Menu::kTITLE_HOVER_MAX{0.8};

/* Main Section General */
const uint8_t Menu::kMAIN_ALPHA{191};
const float Menu::kMAIN_SLIDE_RATE{1.8};
const float Menu::kMAIN_CORNER_LENGTH{0.025};
const float Menu::kINV_WIDTH{0.53};
const float Menu::kOPTIONS_WIDTH{0.40};
const float Menu::kQUIT_WIDTH{0.33};
const float Menu::kSAVE_WIDTH{0.55};
const float Menu::kSLEUTH_WIDTH{0.60};

/* Save Section */
const float Menu::kSAVE_GAP{0.016};
const float Menu::kSAVE_ELEMENT_WIDTH{0.90};
const float Menu::kSAVE_ELEMENT_HEIGHT{0.29};

/* Sleuth Section */
const float Menu::kSLEUTH_GAP{0.009};
const float Menu::kSLEUTH_SPRITE_WIDTH{0.1505};
const float Menu::kSLEUTH_ATTRIBUTE_HEIGHT{0.062};
const float Menu::kSLEUTH_ELEMENT_HEIGHT{0.092};
const float Menu::kSLEUTH_EQUIP_ICON_SIZE{0.0406};
const float Menu::kSLEUTH_ATTRIBUTE_INSET{0.015};

/* Skill Section */
const float Menu::kSKILL_ELEMENT_WIDTH{0.778};
const float Menu::kSKILL_ELEMENT_HEIGHT{0.06};
const float Menu::kSKILL_ELEMENT_INSET{0.02};

/* Inventory Section */
const float Menu::kINV_GAP{0.01};
const float Menu::kINV_MASS_TEXT_Y{0.85};
const float Menu::kINV_MASS_VALUE_Y{0.90};
const float Menu::kINV_THUMB_GAP{0.02};
const float Menu::kINV_ITEM_NAME_X{0.1};
const float Menu::kINV_ITEM_NAME_Y{0.1};
const float Menu::kINV_ITEM_ELEMENT_WIDTH{0.738};
const float Menu::kINV_ITEM_ELEMENT_HEIGHT{0.0645};
const float Menu::kINV_ITEM_ELEMENT_INSET{0.02};
const float Menu::kINV_ITEM_MASS_Y{0.25};
const float Menu::kINV_ITEM_DESC_Y{0};

/* Options Section */
const uint32_t Menu::kNUM_OPTIONS{5};
const float Menu::kOPTIONS_X{0.025};
const float Menu::kOPTIONS_Y{0.05};
const float Menu::kOPTIONS_Y_BAR_GAP{0.04};
const float Menu::kOPTIONS_Y_GAP{0.045};
const float Menu::kOPTIONS_DIGITAL_TEXT_GAP{0.03};
const float Menu::kOPTIONS_BOX_SIZE{0.017};

/* Popup Sections */
const float Menu::kSAVE_POPUP_HEIGHT{0.21};
const float Menu::kSAVE_POPUP_WIDTH{0.1};
const float Menu::kSAVE_POPUP_GAP{0.055};

const SDL_Color Menu::kCOLOR_TITLE_BG{0, 0, 0, 255};
const SDL_Color Menu::kCOLOR_TITLE_BORDER{255, 255, 255, 255};
const SDL_Color Menu::kCOLOR_TITLE_HOVER{255, 255, 255, 65};
const SDL_Color Menu::kCOLOR_MAIN_BORDER{255, 255, 255, 255};
const SDL_Color Menu::kCOLOR_TEXT{255, 255, 255, 255};
const SDL_Color Menu::kCOLOR_OPTION_FILL{70, 70, 70, 128};
const SDL_Color Menu::kCOLOR_OPTION_FILL_SELECTED{125, 125, 125, 255};
const SDL_Color Menu::kCOLOR_INVENTORY_ICON_FILL{40, 40, 40, 255};
const SDL_Color Menu::kCOLOR_BORDER_UNSELECTED{46, 46, 46, 255};
const SDL_Color Menu::kCOLOR_ICON_UNSELECTED_FILL{25, 25, 25, 128};
const SDL_Color Menu::kCOLOR_ELEMENTAL_CURVE{20, 190, 20, 255};

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
      current{Coordinate()},
      flags{static_cast<MenuState>(0)},
      frame_bubbies{nullptr},
      frame_checkbox{nullptr},
      frame_equipment{nullptr},
      frame_footsteps{nullptr},
      frame_items{nullptr},
      frame_key_items{nullptr},
      frame_location{nullptr},
      frame_money{nullptr},
      frame_exp_full{nullptr},
      frame_exp_middle{nullptr},
      frame_exp_empty{nullptr},
      layer{MenuLayer::INVALID},
      player_inventory{nullptr},
      renderer{nullptr},
      save_state{MenuSaveState::NONE},
      title_elements{},
      inventory_element_index{-1},
      option_element_index{-1},
      person_element_index{-1},
      save_element_index{-1},
      skills_element_index{-1},
      sleuth_element_index{-1},
      title_element_index{-1},
      inventory_title_index{InventoryIndex::NONE},
      quit_index{QuitIndex::NONE}
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
  clearIconFrames();

  if(config && renderer)
  {
    frame_bubbies = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/bubbies.png", renderer);
    frame_checkbox = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/check.png", renderer);
    frame_equipment = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/equipment.png", renderer);
    frame_footsteps = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/footsteps.png", renderer);
    frame_items = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/items.png", renderer);
    frame_key_items = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/key_items.png", renderer);
    frame_location = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/location.png", renderer);
    frame_money = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/credits.png", renderer);
    frame_equipment = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/equipment.png", renderer);
    frame_exp_full = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/exp_full.png", renderer);
    frame_exp_middle =
        new Frame(config->getBasePath() + "sprites/Overlay/Menu/exp_middle.png",
                  renderer);
    frame_exp_empty = new Frame(
        config->getBasePath() + "sprites/Overlay/Menu/exp_empty.png", renderer);
  }
}

bool Menu::buildSleuthScreen()
{
  bool success = false;

  sleuth_faces.clear();
  sleuth_stat_frames.clear();

  if(config && renderer && curr_player && curr_player->getSleuth())
  {
    if(curr_player && curr_player->getSleuth())
    {
      success = true;

      /* Clear the current party actors if set */
      clearActors();

      /* Construct the face graphic sprites */
      auto party = curr_player->getSleuth();
      auto members = party->getMembers();

      for(const auto& member : party->getMembers())
      {
        success &= (member != nullptr);

        if(success && member)
          actors.push_back(new BattleActor(member));

        success &= (actors.back() != nullptr);
      }
    }
  }

  return success;
}

/* Calculate the height required for the Attribute stat frames */
int32_t Menu::calcSleuthAttributeHeight()
{
  return (int32_t)std::round(main_section.location.height *
                             kSLEUTH_ATTRIBUTE_HEIGHT);
}

int32_t Menu::calcSleuthElementHeight()
{
  return (int32_t)std::round(main_section.location.height *
                             kSLEUTH_ELEMENT_HEIGHT);
}

/* Calculuate the tile size required for the persons face graphics */
int32_t Menu::calcSleuthTileSize()
{
  if(config)
  {
    int32_t sleuth_gap =
        (int32_t)std::round(config->getScaledWidth() * kSLEUTH_GAP);

    auto size = (main_section.location.height - 6 * sleuth_gap) / 5;

    if(size % 2 != 0 && size > 1)
      --size;

    return size;
  }

  return 0;
}

/* Constructs the Inventory Screen */
void Menu::buildIconTitles(int32_t number)
{
  titles.clear();

  if(config)
  {
    int32_t title_box_size = 0;

    if(getMainMenuType() == MenuType::SLEUTH)
      title_box_size = calcSleuthTileSize();
    else if(getMainMenuType() == MenuType::INVENTORY)
      title_box_size = main_section.location.height / 6;

    /* Construct the Title Icon Boxes */
    Box icon_box{Coordinate{0, 0}, title_box_size, title_box_size};
    setupDefaultBox(icon_box);
    icon_box.color_bg = kCOLOR_ICON_UNSELECTED_FILL;
    icon_box.color_bg_selected = kCOLOR_INVENTORY_ICON_FILL;

    for(int32_t i = 0; i < number; i++)
      titles.push_back(icon_box);
  }
}

void Menu::buildInventoryElements()
{
  if(config)
  {
    inventory_scroll_box.clearElements();
    inventory_scroll_box.setFlag(BoxState::SCROLL_BOX);
    inventory_scroll_box.setFlag(BoxState::SELECTABLE);

    if(inventory_title_index == InventoryIndex::ITEMS)
      buildInventoryItems();
    else if(inventory_title_index == InventoryIndex::EQUIPMENT)
      buildInventoryEquips();
    else if(inventory_title_index == InventoryIndex::BUBBIES)
      buildInventoryBubbies();
    else if(inventory_title_index == InventoryIndex::KEY_ITEMS)
      buildInventoryKeyItems();

    if(inventory_scroll_box.getNumElements())
      inventory_element_index = 0;
  }
}

/* Construct the Bubby Frames -- Scroll Box and Details */
void Menu::buildInventoryBubbies()
{
}

/* Construct the Equipment Frames -- Scroll Box and Details */
void Menu::buildInventoryEquips()
{
}

void Menu::buildSkillFrames()
{
  /* Clear the skill element frames before creating new ones */
  skills_scroll_box.clearElements();
  skills_scroll_box.setFlag(BoxState::SCROLL_BOX);
  skills_scroll_box.setFlag(BoxState::SELECTABLE);

  if(getCurrentPerson() && getCurrentPerson()->getCurrSkills())
  {
    auto curr_skills = getCurrentPerson()->getCurrSkills();

    if(curr_skills)
    {
      auto elements = curr_skills->getElements(getCurrentPerson()->getLevel());
      std::vector<Frame*> skill_frames;

      for(auto& element : elements)
      {
        if(element.skill)
        {
          auto width = calcSkillTitleWidth();
          auto height = calcSkillTitleHeight();

          auto texture = buildSkillListFrame(element.skill, width, height);

          if(texture)
          {
            skill_frames.push_back(new Frame());
            skill_frames.back()->setTexture(texture);
          }
        }
      }

      /* Assign the elements to the skill scroll box */
      skills_scroll_box.setElements(skill_frames);

      if(skill_frames.size() > 0)
        skills_element_index = 0;
    }
  }
}

void Menu::buildSignature()
{
}

/* Construct the Item Frames -- Scroll Box and Details */
void Menu::buildInventoryItems()
{
  /* Clear the inventory item frames before creating new ones */
  inventory_scroll_box.clearElements();

  if(player_inventory)
  {
    auto item_pairs = player_inventory->getItems();
    std::vector<Frame*> item_elements;

    auto width = calcItemTitleWidth();
    auto height = calcItemTitleHeight();

    for(auto& item_pair : item_pairs)
    {
      auto texture =
          buildItemListFrame(item_pair.first, item_pair.second, width, height);

      /* Create frame with the texture if it was created */
      if(texture)
      {
        item_elements.push_back(new Frame());
        item_elements.back()->setTexture(texture);
      }
    }

    /* Assign the Item Scroll Box with the frames, passing ownership */
    inventory_scroll_box.setElements(item_elements);
  }
}

SDL_Texture* Menu::buildItemListFrame(Item* build_item, int32_t count,
                                      uint32_t width, uint32_t height)
{
  if(config && renderer && build_item && count > 0)
  {
    auto item_inset = (uint32_t)std::round(width * kINV_ITEM_ELEMENT_INSET);

    Text t_item_name{getFont(FontName::M_HEADER)};
    Text t_item_count{getFont(FontName::M_HEADER)};

    t_item_name.setText(renderer, build_item->getName(), kCOLOR_TEXT);
    t_item_count.setText(renderer, std::to_string(count), kCOLOR_TEXT);

    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    auto text_y = (height - t_item_name.getHeight()) / 2;
    auto count_x = width - t_item_count.getWidth() - item_inset;

    t_item_name.render(renderer, item_inset, text_y);
    t_item_count.render(renderer, count_x, text_y);

    SDL_SetRenderTarget(renderer, nullptr);

    return texture;
  }

  return nullptr;
}

SDL_Texture* Menu::buildSkillListFrame(Skill* build_skill, uint32_t width,
                                       uint32_t height)
{
  if(config && renderer && build_skill)
  {
    auto skill_inset = (uint32_t)std::round(width * kSKILL_ELEMENT_INSET);

    Text t_skill_name{getFont(FontName::M_HEADER)};

    t_skill_name.setText(renderer, build_skill->getName(), kCOLOR_TEXT);

    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    auto text_y = (height - t_skill_name.getHeight()) / 2;
    t_skill_name.render(renderer, skill_inset, text_y);

    SDL_SetRenderTarget(renderer, nullptr);

    return texture;
  }

  return nullptr;
}

/* Construct the Key Item Frames -- Scroll Box and Details */
void Menu::buildInventoryKeyItems()
{
}

int32_t Menu::calcMainCornerInset()
{
  if(config)
  {
    return static_cast<int32_t>(
        std::ceil(config->getScaledWidth() * kMAIN_CORNER_LENGTH));
  }

  return 0;
}

std::string Menu::calcItemDetailsString(Item* item)
{
  std::string str = "";

  if(item)
  {
    str = Helpers::tierToDisplayStr(item->getItemTier());

    if(item->getFlag(ItemFlags::CONSUMED))
      str = str + " | Consumed";
    else if(item->getFlag(ItemFlags::MATERIAL))
      str = str + " | Material";
    else if(item->getFlag(ItemFlags::STAT_ALTERING))
      str = str + " | Attribute Altering";
    else if(item->getFlag(ItemFlags::SKILL_LEARNING))
      str = str + " | Skill Learning";
  }

  return str;
}

int32_t Menu::calcItemTitleWidth()
{
  return (int32_t)std::round(main_section.location.width *
                             kINV_ITEM_ELEMENT_WIDTH);
}

int32_t Menu::calcItemTitleHeight()
{
  return (int32_t)std::round(main_section.location.height *
                             kINV_ITEM_ELEMENT_HEIGHT);
}

int32_t Menu::calcSkillTitleWidth()
{
  return (int32_t)std::round(main_section.location.width *
                             kSKILL_ELEMENT_WIDTH);
}

int32_t Menu::calcSkillTitleHeight()
{
  return (int32_t)std::round(main_section.location.height *
                             kSKILL_ELEMENT_HEIGHT);
}

bool Menu::canIncrementSleuth()
{
  if(curr_player && curr_player->getSleuth())
  {
    auto members = curr_player->getSleuth()->getMembers();

    if(sleuth_element_index + 1 < (int32_t)members.size())
      return true;
  }

  return false;
}

/* Clears the menu actors */
void Menu::clearActors()
{
  for(auto& actor : actors)
  {
    delete actor;
    actor = nullptr;
  }

  actors.clear();
}

/* Delets the Icon Frames */
void Menu::clearIconFrames()
{
  if(frame_money)
    delete frame_money;
  if(frame_location)
    delete frame_location;
  if(frame_footsteps)
    delete frame_footsteps;
  if(frame_bubbies)
    delete frame_bubbies;
  if(frame_checkbox)
    delete frame_checkbox;
  if(frame_equipment)
    delete frame_equipment;
  if(frame_items)
    delete frame_items;
  if(frame_key_items)
    delete frame_key_items;
  if(frame_exp_full)
    delete frame_exp_full;
  if(frame_exp_middle)
    delete frame_exp_middle;
  if(frame_exp_empty)
    delete frame_exp_empty;

  frame_money = nullptr;
  frame_location = nullptr;
  frame_footsteps = nullptr;
  frame_bubbies = nullptr;
  frame_checkbox = nullptr;
  frame_equipment = nullptr;
  frame_items = nullptr;
  frame_key_items = nullptr;
  frame_exp_full = nullptr;
  frame_exp_middle = nullptr;
  frame_exp_empty = nullptr;
}

void Menu::buildSave()
{
  auto main = main_section.location;
  auto save_width = (int32_t)std::round(main.width * kSAVE_ELEMENT_WIDTH);
  auto save_height = (int32_t)std::round(main.height * kSAVE_ELEMENT_HEIGHT);

  /* Refresh elements for the Save scroll box */
  std::vector<Frame*> save_frames;
  save_scroll_box.clearElements();

  for(auto& save : save_data)
  {
    setupDefaultBox(save.location);
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

/* Constructs the Options */
void Menu::buildOptions()
{
  if(config)
  {
    auto analog_box = Box({0, 0}, 0, 0);

    /* Auto Run Flag - Digital */
    option_auto_run = DigitalOption(config, {0, 0}, 20, 20, 3, "AUTO RUN");
    option_auto_run.location.color_border_selected = {255, 255, 255, 255};
    option_auto_run.location.color_border = {255, 255, 255, 65};

    /* Mute Flag - Digital */
    option_mute = DigitalOption(config, {0, 0}, 20, 20, 6, "MUTE");
    option_mute.location.color_border_selected = {255, 255, 255, 255};
    option_mute.location.color_border = {255, 255, 255, 65};

    /* Fast Battle - Digital */
    option_fast_battle =
        DigitalOption(config, {0, 0}, 20, 20, 7, "FAST BATTLE");
    option_fast_battle.location.color_border_selected = {255, 255, 255, 255};
    option_fast_battle.location.color_border = {255, 255, 255, 65};

    /* Audio Level - Analog */
    option_audio_level = AnalogOption("AUDIO LEVEL");
    option_audio_level.location = analog_box;
    option_audio_level.val = &config->audio_level;
    option_audio_level.default_val = Options::kDEF_AUDIO_LEVEL;
    option_audio_level.num_options = 20;
    option_audio_level.location.color_border_selected = {255, 255, 255, 255};
    option_audio_level.location.color_border = {255, 255, 255, 65};

    /* Music Level - Analog */
    option_music_level = AnalogOption("MUSIC LEVEL");
    option_music_level.location = analog_box;
    option_music_level.val = &config->music_level;
    option_music_level.default_val = Options::kDEF_MUSIC_LEVEL;
    option_music_level.num_options = 20;
    option_music_level.location.color_border_selected = {255, 255, 255, 255};
    option_music_level.location.color_border = {255, 255, 255, 65};

    /* Scaling - Options */
    option_scaling_ui_level = AnalogOption("UI SCALING");
    option_scaling_ui_level.location = analog_box;
    option_scaling_ui_level.val = &config->scaling_ui;
    option_scaling_ui_level.default_val = Options::kDEF_SCALING_UI;
    option_scaling_ui_level.num_options = 20;
    option_scaling_ui_level.location.color_border_selected = {255, 255, 255,
                                                              255};
    option_scaling_ui_level.location.color_border = {255, 255, 255, 65};
  }
}

/* Constructs the Main Section as Required */
void Menu::buildMainSection(MenuType menu_type)
{
  if(config && renderer)
  {
    auto width = config->getScaledWidth();
    int32_t main_width{0};
    auto title_point = title_section.location.point;
    auto title_corner = (int32_t)std::round(width * kTITLE_CORNER_LENGTH);
    Coordinate start_point = {title_point.x + title_section.location.width,
                              title_point.y + title_corner};
    auto corner_width = (uint32_t)std::round(main_width * kMAIN_CORNER_LENGTH);

    if(menu_type == MenuType::INVENTORY)
      main_width = (int32_t)std::round(width * kINV_WIDTH);
    else if(menu_type == MenuType::OPTIONS)
      main_width = (int32_t)std::round(width * kOPTIONS_WIDTH);
    else if(menu_type == MenuType::SLEUTH)
      main_width = (int32_t)std::round(width * kSLEUTH_WIDTH);
    else if(menu_type == MenuType::SAVE)
      main_width = (int32_t)std::round(width * kSAVE_WIDTH);
    else if(menu_type == MenuType::QUIT)
      main_width = (int32_t)std::round(width * kQUIT_WIDTH);

    main_section.location.point.x = -(main_width + corner_width);
    main_section.location.point.y = start_point.y;
    main_section.location.height = title_section.location.height - title_corner;
    main_section.location.width = main_width + corner_width;
    main_section.point = start_point;
    main_section.alpha = kMAIN_ALPHA;
    main_section.status = WindowStatus::OFF;

    if(menu_type == MenuType::SLEUTH)
    {
      /* Construct the gray tile boxes */
      buildIconTitles(5);

      sleuth_element_index = 0;
      person_element_index = 0;

      selectSleuthIndex();

      /* Construct the face graphics and other sprites/frames required */
      buildSleuthScreen();
      buildSkillFrames();
    }
    else if(menu_type == MenuType::SIGNATURE)
    {
      // buildSignature();
    }
    else if(menu_type == MenuType::INVENTORY)
    {
      buildIconTitles(4);
      inventory_title_index = InventoryIndex::ITEMS;
      buildInventoryElements();
      selectInventoryIndex();
    }
    else if(menu_type == MenuType::OPTIONS)
    {
      buildOptions();
      option_element_index = 0;
      selectOptionIndex();
    }
    else if(menu_type == MenuType::SAVE)
    {
      buildSave();
    }
    else if(menu_type == MenuType::QUIT)
    {
      quit_index = QuitIndex::NO;
    }
  }
}

/* Constructs TitleElements on the Title Menu Section */
void Menu::buildTitleElements()
{
  /* Clear the title elements to begin */
  title_elements.clear();

  /* Party TitleElement */
  title_elements.push_back(TitleElement(
      Helpers::menuTypeToStr(MenuType::SLEUTH), true, MenuType::SLEUTH));

  /* Party TitleElement */
  // title_elements.push_back(TitleElement(
  //     "Signature", true, MenuType::SIGNATURE));

  /* Inventory TitleElement */
  title_elements.push_back(
      TitleElement("Inventory", true, MenuType::INVENTORY));

  /* Options TitleElement */
  title_elements.push_back(TitleElement(
      Helpers::menuTypeToStr(MenuType::OPTIONS), true, MenuType::OPTIONS));

  /* Save TitleElement */
  title_elements.push_back(TitleElement(Helpers::menuTypeToStr(MenuType::SAVE),
                                        true, MenuType::SAVE));

  /* Quit (Return to title) TitleElement */
  title_elements.push_back(TitleElement(Helpers::menuTypeToStr(MenuType::QUIT),
                                        true, MenuType::QUIT));
}

void Menu::buildPersonTitleElements()
{
  person_title_elements.clear();

  /* Person overview of statistics etc. */
  person_title_elements.push_back(
      TitleElement("Overview", true, MenuType::SLEUTH_OVERVIEW));

  // TODO [05-21-16]: Future equipment title selection
  // /* Person equipment screen */
  // person_title_elements.push_back(
  //     TitleElement("Equipment", true, MenuType::SLEUTH_EQUIPMENT));

  /* Person skill list screen */
  person_title_elements.push_back(
      TitleElement("Skills", true, MenuType::SLEUTH_SKILLS));

  /* Person details screen (extra information) */
  // person_title_elements.push_back(
  //     TitleElement("Details", true, MenuType::SLEUTH_DETAILS));
}

void Menu::buildQuit()
{
}

/* Constructs the TitleSection */
void Menu::buildTitleSection()
{
  auto height = config->getScreenHeight();
  auto screen_height = config->getScaledHeight();
  auto width = config->getScaledWidth();

  auto title_height = (uint32_t)std::round(screen_height * kTITLE_HEIGHT);
  auto y = (height - title_height) / 2 - (height * kTITLE_Y_OFFSET);
  auto title_width = (uint32_t)std::round(width * kTITLE_WIDTH);
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

void Menu::decrementInventoryIndex()
{
  unselectInventoryIndex();
  auto new_index = static_cast<uint32_t>(inventory_title_index) - 1;
  inventory_title_index = static_cast<InventoryIndex>(new_index);
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);
  selectInventoryIndex();
}

// TODO [04-9-16]: abtract out BETA
void Menu::decrementOptionIndex()
{
  unselectOptionIndex();
  option_element_index--;
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);
  selectOptionIndex();
}

void Menu::decrementQuitIndex()
{
  auto new_index = static_cast<uint32_t>(quit_index) - 1;
  quit_index = static_cast<QuitIndex>(new_index);
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);
}

void Menu::decrementSleuthIndex()
{
  unselectSleuthIndex();
  sleuth_element_index--;

  selectSleuthIndex();
}

void Menu::incrementInventoryIndex()
{
  unselectInventoryIndex();
  auto new_index = static_cast<uint32_t>(inventory_title_index) + 1;
  inventory_title_index = static_cast<InventoryIndex>(new_index);
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);
  selectInventoryIndex();
}

void Menu::incrementOptionIndex()
{
  unselectOptionIndex();
  option_element_index++;
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);
  selectOptionIndex();
}

void Menu::incrementQuitIndex()
{
  auto new_index = static_cast<uint32_t>(quit_index) + 1;
  quit_index = static_cast<QuitIndex>(new_index);
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);
}

void Menu::incrementSleuthIndex()
{
  unselectSleuthIndex();
  sleuth_element_index++;
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);
  selectSleuthIndex();
}

void Menu::selectInventoryIndex()
{
  if(inventory_title_index != InventoryIndex::NONE &&
     inventory_title_index <= InventoryIndex::KEY_ITEMS)
  {
    titles.at(static_cast<uint32_t>(inventory_title_index) - 1)
        .setFlag(BoxState::SELECTED);
  }
}

/* Select the current option index */
void Menu::selectOptionIndex()
{
  if(option_element_index == 0)
    option_audio_level.location.setFlag(BoxState::SELECTED);
  else if(option_element_index == 1)
    option_music_level.location.setFlag(BoxState::SELECTED);
  else if(option_element_index == 2)
    option_scaling_ui_level.location.setFlag(BoxState::SELECTED);
  else if(option_element_index == 3)
    option_auto_run.location.setFlag(BoxState::SELECTED);
  else if(option_element_index == 4)
    option_mute.location.setFlag(BoxState::SELECTED);
  else if(option_element_index == 5)
    option_fast_battle.location.setFlag(BoxState::SELECTED);
}

void Menu::unselectInventoryIndex()
{
  if(inventory_title_index != InventoryIndex::NONE &&
     inventory_title_index <= InventoryIndex::KEY_ITEMS)
  {
    titles.at(static_cast<uint32_t>(inventory_title_index) - 1)
        .setFlag(BoxState::SELECTED, false);
  }
}

/* Unselect the current option index */
void Menu::unselectOptionIndex()
{
  if(option_element_index == 0)
    option_audio_level.location.setFlag(BoxState::SELECTED, false);
  else if(option_element_index == 1)
    option_music_level.location.setFlag(BoxState::SELECTED, false);
  else if(option_element_index == 2)
    option_scaling_ui_level.location.setFlag(BoxState::SELECTED, false);
  else if(option_element_index == 3)
    option_auto_run.location.setFlag(BoxState::SELECTED, false);
  else if(option_element_index == 4)
    option_mute.location.setFlag(BoxState::SELECTED, false);
  else if(option_element_index == 5)
    option_fast_battle.location.setFlag(BoxState::SELECTED, false);
}

void Menu::unselectSleuthIndex()
{
  if(sleuth_element_index < (int32_t)titles.size() && sleuth_element_index > -1)
    titles.at(sleuth_element_index).setFlag(BoxState::SELECTED, false);
}

void Menu::updateScalingFactor()
{
  if(config)
  {
    buildTitleSection();

    title_section.location.point = title_section.point;
    title_section.status = WindowStatus::ON;

    buildMainSection(MenuType::OPTIONS);

    main_section.location.point = main_section.point;
    main_section.status = WindowStatus::ON;

    unselectOptionIndex();
    option_element_index = 2;
    selectOptionIndex();
  }
}

void Menu::selectSleuthIndex()
{
  if(sleuth_element_index < (int32_t)titles.size() && sleuth_element_index >= 0)
    titles.at(sleuth_element_index).setFlag(BoxState::SELECTED, true);
}

void Menu::clearAttributeFrames()
{
  for(auto& attribute_frame : sleuth_attr_frames)
  {
    if(attribute_frame)
      delete attribute_frame;

    attribute_frame = nullptr;
  }

  sleuth_attr_frames.clear();
}

void Menu::clearElementFrames()
{
  for(auto& element_frame : sleuth_stat_frames)
  {
    if(element_frame)
      delete element_frame;

    element_frame = nullptr;
  }

  sleuth_stat_frames.clear();
}

void Menu::renderAttributes(Coordinate start, int32_t gap)
{
  clearElementFrames();
  auto width = s_attributes_box.width;
  auto attr_height = calcSleuthAttributeHeight();
  auto elmt_height = calcSleuthElementHeight();

  SDL_Texture* prim_texture = nullptr;
  SDL_Texture* secd_texture = nullptr;
  SDL_Texture* unbr_texture = nullptr;
  SDL_Texture* limb_texture = nullptr;
  SDL_Texture* will_texture = nullptr;

  prim_texture = buildElementFrame(ElementType::PRIMARY, width, elmt_height);
  secd_texture = buildElementFrame(ElementType::SECONDARY, width, elmt_height);
  unbr_texture = buildAttributeFrame(Attribute::UNBR, width, attr_height);
  limb_texture = buildAttributeFrame(Attribute::LIMB, width, attr_height);
  will_texture = buildAttributeFrame(Attribute::WILL, width, attr_height);

  if(prim_texture)
  {
    sleuth_stat_frames.push_back(new Frame());
    sleuth_stat_frames.back()->setTexture(prim_texture);
  }

  if(secd_texture)
  {
    sleuth_stat_frames.push_back(new Frame());
    sleuth_stat_frames.back()->setTexture(secd_texture);
  }

  if(unbr_texture)
  {
    sleuth_stat_frames.push_back(new Frame());
    sleuth_stat_frames.back()->setTexture(unbr_texture);
  }

  if(limb_texture)
  {
    sleuth_stat_frames.push_back(new Frame());
    sleuth_stat_frames.back()->setTexture(limb_texture);
  }

  if(will_texture)
  {
    sleuth_stat_frames.push_back(new Frame());
    sleuth_stat_frames.back()->setTexture(will_texture);
  }

  current = Coordinate{start.x + gap, start.y + gap};

  for(auto& stat_frame : sleuth_stat_frames)
  {
    if(stat_frame)
    {
      stat_frame->render(renderer, current.x, current.y);
      current.y += stat_frame->getHeight() + (int32_t)std::round(0.54 * gap);
    }
  }
}

SDL_Texture* Menu::buildAttributeFrame(Attribute attr, uint32_t width,
                                       uint32_t height)
{
  auto actor = getCurrentActor();

  if(battle_display_data && renderer && config && actor)
  {
    auto frame_attribute = battle_display_data->getFrameAttribute(attr);
    auto inset = (uint32_t)std::round(width * kSLEUTH_ATTRIBUTE_INSET);

    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    if(frame_attribute)
    {
      frame_attribute->render(renderer, inset,
                              height / 2 - frame_attribute->getHeight() / 2);

      Text t_name{getFont(FontName::M_VALUE)};
      Text t_value{getFont(FontName::M_VALUE)};
      Text t_max{getFont(FontName::M_VALUE)};

      t_max.setText(renderer, "Unbearability: 9999", kCOLOR_TEXT);

      if(attr == Attribute::UNBR)
        t_name.setText(renderer, "Unbearability: ", kCOLOR_TEXT);
      else if(attr == Attribute::LIMB)
        t_name.setText(renderer, "Limbertude: ", kCOLOR_TEXT);
      else if(attr == Attribute::WILL)
        t_name.setText(renderer, "Will", kCOLOR_TEXT);

      std::string value_str =
          std::to_string(actor->getStatsRendered().getValue(attr));

      t_value.setText(renderer, value_str, kCOLOR_TEXT);

      auto text_x = 2 * inset + frame_attribute->getWidth();
      t_name.render(renderer, text_x, height - t_name.getHeight());

      auto value_x = text_x + t_max.getWidth() - t_value.getWidth();
      t_value.render(renderer, value_x, height - t_value.getHeight());
    }

    SDL_SetRenderTarget(renderer, nullptr);
    return texture;
  }

  return nullptr;
}

SDL_Texture* Menu::buildElementFrame(ElementType element_type, uint32_t width,
                                     uint32_t height)
{
  auto actor = getCurrentActor();
  auto person = getCurrentPerson();

  if(battle_display_data && renderer && config && actor && person)
  {
    Element curr_element = person->getSecondary();

    if(element_type == ElementType::PRIMARY)
      curr_element = person->getPrimary();

    auto frame_element = battle_display_data->getFrameElement(curr_element);
    auto stats = actor->getStatsRendered();

    auto attributes = Helpers::elementTypeToStats(element_type);

    // TODO: Element special inset?
    auto inset = (uint32_t)std::round(width * kSLEUTH_ATTRIBUTE_INSET);

    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    if(frame_element)
    {
      frame_element->render(renderer, inset,
                            height / 2 - frame_element->getHeight() / 2);

      Text t_name{getFont(FontName::M_ITEM_HEADER)};
      Text t_atk_title{getFont(FontName::M_VALUE)};
      Text t_def_title{getFont(FontName::M_VALUE)};
      Text t_value_atk{getFont(FontName::M_VALUE)};
      Text t_value_def{getFont(FontName::M_VALUE)};
      Text t_max{getFont(FontName::M_VALUE)};

      t_max.setText(renderer, "Unbearability: 9999", kCOLOR_TEXT);

      t_atk_title.setText(renderer, "Aggression: ", kCOLOR_TEXT);
      t_def_title.setText(renderer, "Fortitude: ", kCOLOR_TEXT);

      std::string value_atk_str =
          std::to_string(stats.getValue(attributes.second));
      std::string value_def_str =
          std::to_string(stats.getValue(attributes.second));

      t_value_atk.setText(renderer, value_atk_str, kCOLOR_TEXT);
      t_value_def.setText(renderer, value_def_str, kCOLOR_TEXT);

      std::string element_str = Helpers::elementToDisplayString(curr_element);
      std::transform(element_str.begin(), element_str.end(),
                     element_str.begin(), ::toupper);

      t_name.setText(renderer, element_str, kCOLOR_TEXT);
      t_value_atk.setText(renderer, value_atk_str, kCOLOR_TEXT);
      t_value_def.setText(renderer, value_def_str, kCOLOR_TEXT);

      auto text_x = 2 * inset + frame_element->getWidth();
      auto text_y = t_name.getHeight();

      t_name.render(renderer, text_x, 1);
      t_atk_title.render(renderer, text_x, text_y);

      auto value_x = text_x + t_max.getWidth() - t_value_atk.getWidth();

      t_value_atk.render(renderer, value_x, text_y);

      auto def_y = text_y + t_value_atk.getHeight();

      t_def_title.render(renderer, text_x, def_y);
      t_value_def.render(renderer, value_x, def_y);
    }

    SDL_SetRenderTarget(renderer, nullptr);
    return texture;
  }

  return nullptr;
}

void Menu::renderBubbies()
{
}

void Menu::renderEquipment()
{
}

void Menu::renderKeyItems()
{
}

/* Renders the TitleSection */
void Menu::renderTitleSection()
{
  auto screen_height = config->getScreenHeight();
  auto height = config->getScaledHeight();
  auto width = config->getScaledWidth();
  auto location = title_section.location;
  auto point = title_section.location.point;

  auto corner_inset = (int32_t)std::round(width * kTITLE_CORNER_LENGTH);
  auto title_width = location.width + corner_inset;

  /* Render the frame outline and backdrop */
  Coordinate tl = {point.x, point.y};
  Coordinate tr = {point.x + location.width - corner_inset, point.y};
  Coordinate bl = {point.x, point.y + location.height};
  Coordinate tc = {tr.x + corner_inset, tr.y + corner_inset};
  Coordinate bc = {tc.x, bl.y};

  short main_x[5] = {tl.x, tr.x, tc.x, bc.x, bl.x};
  short main_y[5] = {tl.y, tr.y, tc.y, bc.y, bl.y};

  filledPolygonRGBA(renderer, main_x, main_y, 5, kCOLOR_TITLE_BG.r,
                    kCOLOR_TITLE_BG.g, kCOLOR_TITLE_BG.b, kCOLOR_TITLE_BG.a);

  polygonRGBA(renderer, main_x, main_y, 5, kCOLOR_TITLE_BORDER.r,
              kCOLOR_TITLE_BORDER.g, kCOLOR_TITLE_BORDER.b,
              kCOLOR_TITLE_BORDER.a);

  auto x_offset = (int32_t)std::round(width * kTITLE_X_OFFSET);
  auto y_offset = (int32_t)std::round(height * kTITLE_Y_OFFSET);

  Text map_name{getFont(FontName::M_HEADER)};
  Text footsteps{getFont(FontName::M_HEADER)};
  Text credits{getFont(FontName::M_HEADER)};
  Text t_main_title{getFont(FontName::M_MAIN_TITLE)};
  Text t_title_element{getFont(FontName::M_TITLE_ELM)};

  t_main_title.setText(renderer, "MENU", kCOLOR_TEXT);

  int32_t y_gap = (int32_t)std::round(config->getScaledHeight() * 0.06);
  int32_t element_offset =
      point.y + y_offset + t_main_title.getHeight() + y_gap;
  int32_t running_offset = element_offset;
  int32_t title_element_height = 0;

  t_title_element.setText(renderer, "INVALID", kCOLOR_TEXT);
  title_element_height = t_title_element.getHeight();

  if(title_element_index != -1 &&
     title_element_index < (int)title_elements.size())
  {
    auto rect_y = element_offset + (title_element_index)*y_gap - (y_gap / 4);
    auto rect_h = title_element_height + (y_gap / 2);

    Coordinate tl = {point.x + title_width * kTITLE_HOVER_OFFSET_X, rect_y};
    Coordinate tr = {tl.x + title_width * kTITLE_HOVER_WIDTH, rect_y};
    Coordinate bl = {tl.x, tl.y + rect_h};
    Coordinate br = {tr.x, bl.y};

    short select_x[4] = {tl.x, tr.x, br.x, bl.x};
    short select_y[4] = {tl.y, tr.y, br.y, bl.y};

    auto brightness = Helpers::updateHoverBrightness(
        title_elements.at(title_element_index).hover_time, kTITLE_HOVER_RATE,
        kTITLE_HOVER_MIN, kTITLE_HOVER_MAX);

    filledPolygonRGBA(renderer, select_x, select_y, 4, kCOLOR_TITLE_HOVER.r,
                      kCOLOR_TITLE_HOVER.g, kCOLOR_TITLE_HOVER.b,
                      kCOLOR_TITLE_HOVER.a * brightness);
  }

  /* Text Color */
  Frame::setRenderDrawColor(renderer, kCOLOR_TEXT);

  auto text_offset_x = (uint32_t)std::round(kTITLE_ELEMENT_X_OFFSET * width);
  auto element_text_x = point.x + x_offset + text_offset_x;

  for(auto& title_element : title_elements)
  {
    t_title_element.setText(renderer, title_element.name, kCOLOR_TEXT);
    t_title_element.render(renderer, element_text_x, running_offset);
    running_offset += y_gap;
  }

  if(curr_map && curr_player)
  {
    // auto bot_y_offset = (int32_t)std::round(screen_height *
    // kTITLE_LOCATION_Y_OFFSET);
    auto icons_y_gap = (int32_t)std::round(height * kTITLE_ICONS_Y_GAP);
    auto tx = (int32_t)std::round(width * kTITLE_ICON_TEXT_X);
    auto ty = (int32_t)std::round(height * kTITLE_ICON_TEXT_Y);
    current.y = title_section.location.height + point.y;

    if(frame_money)
    {
      current.y -= icons_y_gap;
      credits.setText(renderer, Helpers::formatUInt(curr_player->getCredits()),
                      kCOLOR_TEXT);
      frame_money->render(renderer, point.x + x_offset, current.y);
      credits.render(renderer, point.x + x_offset + tx, current.y);
    }

    if(frame_footsteps)
    {
      current.y -= icons_y_gap;
      footsteps.setText(renderer, Helpers::formatUInt(curr_player->getSteps()),
                        kCOLOR_TEXT);
      frame_footsteps->render(renderer, point.x + x_offset, current.y);
      footsteps.render(renderer, point.x + x_offset + tx, current.y);
    }

    if(frame_location)
    {
      current.y -= icons_y_gap;

      map_name.setText(renderer, curr_map->getName(), kCOLOR_TEXT);
      frame_location->render(renderer, point.x + x_offset, current.y);
      map_name.render(renderer, point.x + x_offset + tx, current.y);
    }
  }

  t_main_title.render(renderer, point.x + x_offset, point.y + y_offset);
}

void Menu::renderTitleTriangle(Box& icon)
{
  /* Determine the length/size of the triangle */
  auto tan47 = std::tan(47 * 3.141592635 / 180.0);
  auto l = (int32_t)std::round(icon.height / 15.0 * tan47);

  /* Calculate the three points of the triangle */
  Coordinate end{icon.point.x + icon.width, icon.point.y + icon.height};
  Coordinate p1{end.x, end.y - icon.height / 2 - icon.height / 15};
  Coordinate p2{end.x + l - 1, end.y - icon.height / 2};
  Coordinate p3{end.x, end.y - icon.height / 2 + icon.height / 15};

  /* Render the triangle */
  if(p1.x > 0 && p2.x > 0 && p3.x > 0)
    Frame::renderTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, renderer);
}

/* Render the Main Section */
void Menu::renderMainSection()
{
  if((uint32_t)title_element_index < title_elements.size() && config &&
     renderer)
  {
    auto menu_type = getMainMenuType();

    setupDefaultBox(main_section.location);
    main_section.location.corner_inset = calcMainCornerInset();
    main_section.location.box_type = BoxType::CORNER_CUT_BOX;
    main_section.location.color_border = kCOLOR_MAIN_BORDER;
    main_section.location.color_bg = {0, 0, 0, main_section.alpha};
    main_section.location.render(renderer);

    if(menu_type == MenuType::INVENTORY)
      renderInventory();
    else if(menu_type == MenuType::OPTIONS)
      renderOptions();
    else if(menu_type == MenuType::SLEUTH)
      renderSleuth();
    else if(menu_type == MenuType::SAVE)
      renderSave();
    else if(menu_type == MenuType::QUIT)
      renderQuit();
  }
}

/* Renders the Inventory Screen */
void Menu::renderInventory()
{
  auto inv = player_inventory;
  auto width = config->getScaledWidth();
  auto start = main_section.location.point;
  auto gap = (int32_t)std::round(width * kINV_GAP);
  current.y = start.y + gap;

  Frame::setRenderDrawColor(renderer, kCOLOR_MAIN_BORDER);

  /* Render the Title Boxes */
  for(uint32_t i = 0; i < titles.size(); i++)
  {
    auto& icon = titles.at(i);

    icon.point.x = start.x + gap;
    icon.point.y = current.y;
    icon.render(renderer);
    current.y += gap + icon.height;

    if(i == 0)
    {
      frame_items->render(
          renderer, icon.point.x + icon.width / 2 - frame_items->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_items->getHeight() / 2);

      if(inventory_title_index == InventoryIndex::ITEMS)
        renderTitleTriangle(icon);
    }
    else if(i == 1)
    {
      frame_equipment->render(
          renderer,
          icon.point.x + icon.width / 2 - frame_equipment->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_equipment->getHeight() / 2);

      if(inventory_title_index == InventoryIndex::EQUIPMENT)
        renderTitleTriangle(icon);
    }
    else if(i == 2)
    {
      frame_bubbies->render(
          renderer,
          icon.point.x + icon.width / 2 - frame_bubbies->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_bubbies->getHeight() / 2);

      if(inventory_title_index == InventoryIndex::BUBBIES)
        renderTitleTriangle(icon);
    }
    else if(i == 3)
    {
      frame_key_items->render(
          renderer,
          icon.point.x + icon.width / 2 - frame_key_items->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_key_items->getHeight() / 2);

      if(inventory_title_index == InventoryIndex::KEY_ITEMS)
        renderTitleTriangle(icon);
    }
  }

  auto icon_w = main_section.location.height / 6;
  current.y += gap;

  /* Render the Mass of the Inventory */
  Text mass_title{getFont(FontName::M_SUBHEADER)};
  Text mass_value{getFont(FontName::M_HEADER)};
  std::string mass_string;

  mass_title.setText(renderer, "TOTAL MASS", kCOLOR_TEXT);

  if(inv->getMassLimit() < 1000)
  {
    mass_string = std::to_string((uint32_t)std::round(inv->getMass())) + " / " +
                  std::to_string((uint32_t)std::round(inv->getMassLimit())) +
                  " kg";
  }
  else
  {
    mass_string =
        std::to_string((uint32_t)std::round(inv->getMass() / 1000)) + " / " +
        std::to_string((uint32_t)std::round(inv->getMassLimit() / 1000)) +
        " Mg";
  }

  mass_value.setText(renderer, mass_string, kCOLOR_TEXT);

  mass_title.render(renderer,
                    start.x + gap + icon_w / 2 - mass_title.getWidth() / 2,
                    current.y);
  mass_value.render(renderer,
                    start.x + gap + icon_w / 2 - mass_value.getWidth() / 2,
                    current.y + icon_w / 3);

  /* Render the Top Boxes and the Bottom Boxes */
  auto top_section_height = (main_section.location.height - 3 * gap) / 2;

  inventory_top_box.point = {start.x + 2 * gap + icon_w, start.y + gap};
  inventory_top_box.width = main_section.location.width - icon_w - 3 * gap;
  inventory_top_box.height = top_section_height / 5;
  setupDefaultBox(inventory_top_box);
  inventory_top_box.render(renderer);

  /* Render the scroll inventory box cut */
  inventory_scroll_box.point = {inventory_top_box.point.x,
                                inventory_top_box.point.y +
                                    inventory_top_box.height - 1};
  inventory_scroll_box.width = inventory_top_box.width;
  inventory_scroll_box.height = top_section_height - inventory_top_box.height;
  setupDefaultBox(inventory_scroll_box);
  inventory_scroll_box.color_element_selected = kCOLOR_INVENTORY_ICON_FILL;
  inventory_scroll_box.color_scroll = kCOLOR_OPTION_FILL_SELECTED;
  inventory_scroll_box.color_scroll_selected = kCOLOR_OPTION_FILL_SELECTED;
  inventory_scroll_box.element_inset_x = gap;
  inventory_scroll_box.element_inset_y = gap;
  inventory_scroll_box.scroll_inset_x = gap;
  inventory_scroll_box.scroll_inset_y = gap + calcItemTitleHeight() / 2;
  inventory_scroll_box.scroll_width = 10;
  inventory_scroll_box.render(renderer);

  /* Render the bottom icon detail box */
  inventory_bottom_box.width = inventory_top_box.width;
  inventory_bottom_box.height =
      main_section.location.height - top_section_height - 3 * gap;
  inventory_bottom_box.point = {inventory_top_box.point.x,
                                inventory_top_box.point.y +
                                    inventory_top_box.height +
                                    inventory_scroll_box.height + gap};
  setupDefaultBox(inventory_bottom_box);
  inventory_bottom_box.corner_inset = calcMainCornerInset();
  inventory_bottom_box.box_type = BoxType::CORNER_CUT_BOX;
  inventory_bottom_box.render(renderer);

  /* Render the Item Title Text */
  Text title_text{getFont(FontName::M_TITLE_ELM)};
  Text number_items{getFont(FontName::M_TITLE_ELM)};
  std::string number_str = "";

  // TODO Color if at limit?
  if(inventory_title_index == InventoryIndex::ITEMS)
  {
    number_str = std::to_string(inv->getItemTotalCount()) + " / " +
                 std::to_string(inv->getItemLimit());

    title_text.setText(renderer, "Items", kCOLOR_TEXT);
  }
  else if(inventory_title_index == InventoryIndex::EQUIPMENT)
  {
    number_str = std::to_string(inv->getEquipTotalCount()) + " / " +
                 std::to_string(inv->getEquipmentLimit());

    title_text.setText(renderer, "Crafting", kCOLOR_TEXT);
  }
  else if(inventory_title_index == InventoryIndex::BUBBIES)
  {
    number_str = std::to_string(inv->getBubbyTotalCount()) + " / " +
                 std::to_string(inv->getBubbyLimit());

    title_text.setText(renderer, "Bubbies", kCOLOR_TEXT);
  }
  else if(inventory_title_index == InventoryIndex::KEY_ITEMS)
  {
    title_text.setText(renderer, "Key Items", kCOLOR_TEXT);
  }

  /* Assemble and Render the Item Limits */
  auto title_text_y = inventory_top_box.point.y + inventory_top_box.height / 2 -
                      title_text.getHeight() / 2;
  number_items.setText(renderer, number_str, kCOLOR_TEXT);

  title_text.render(renderer, inventory_top_box.point.x + gap, title_text_y);

  number_items.render(renderer,
                      inventory_top_box.point.x + inventory_top_box.width -
                          number_items.getWidth() - 3 * gap,
                      title_text_y);

  if(inventory_title_index == InventoryIndex::ITEMS &&
     inventory_element_index != -1)
  {
    renderItem(inventory_bottom_box.point, icon_w, gap,
               inventory_bottom_box.height);
  }
}

/* Render the extra information for an item */
void Menu::renderItem(Coordinate start, int32_t icon_w, int32_t gap,
                      int32_t bot_height)
{
  if(inventory_element_index < (int32_t)player_inventory->getItems().size())
  {
    auto item_pair = player_inventory->getItems().at(inventory_element_index);

    if(item_pair.first)
    {
      auto item = item_pair.first;

      auto corner_inset = calcMainCornerInset();

      /* Render the Inventory Icon Box */
      inventory_icon_box.point = {start.x + gap, start.y + gap};
      inventory_icon_box.width = icon_w;
      inventory_icon_box.height = icon_w;
      inventory_icon_box.color_bg = kCOLOR_ICON_UNSELECTED_FILL;
      inventory_icon_box.color_border = kCOLOR_BORDER_UNSELECTED;
      inventory_icon_box.render(renderer);

      /* Render the Item's Icon in the Icon Box */
      auto thumb = item->getThumb();

      if(thumb)
      {
        thumb->render(renderer,
                      inventory_icon_box.point.x +
                          inventory_icon_box.width / 2 - thumb->getWidth(),
                      inventory_icon_box.point.y +
                          inventory_icon_box.height / 2 - thumb->getHeight(),
                      thumb->getWidth() * 2, thumb->getHeight() * 2);
      }

      /* Render the Item Name, Mass, Description */
      auto curr_x = inventory_icon_box.point.x + inventory_icon_box.width + gap;

      Text item_name{getFont(FontName::M_OPTIONS)};
      Text item_mass_title{getFont(FontName::M_ITEM_HEADER)};
      Text item_mass{getFont(FontName::M_OPTIONS)};
      Text item_value_title{getFont(FontName::M_ITEM_HEADER)};
      Text item_value{getFont(FontName::M_OPTIONS)};
      Text item_description{getFont(FontName::M_STANDARD)};

      item_name.setText(renderer, item->getName(), kCOLOR_TEXT);
      item_mass_title.setText(renderer, "MASS", kCOLOR_TEXT);
      item_mass.setText(renderer, std::to_string(item->getMass()) + " g",
                        kCOLOR_TEXT);

      item_value_title.setText(renderer, "VALUE", kCOLOR_TEXT);
      item_value.setText(renderer, std::to_string(item->getValue()),
                         kCOLOR_TEXT);

      item_name.render(renderer, curr_x, inventory_icon_box.point.y);
      auto mass_x = std::max(item_mass.getWidth(), item_mass_title.getWidth());
      mass_x += curr_x + 2 * gap;

      current.y = inventory_icon_box.point.y + item_name.getHeight() + gap;
      item_mass_title.render(renderer, curr_x, current.y);

      item_value_title.render(renderer, mass_x, current.y);
      current.y += item_mass_title.getHeight() + gap / 4;
      item_mass.render(renderer, curr_x, current.y);
      item_value.render(renderer, mass_x, current.y);

      current.y = inventory_icon_box.point.y + inventory_icon_box.height + gap;
      auto desc_split =
          Text::splitLine(getFont(FontName::M_STANDARD), item->getDescription(),
                          inventory_top_box.width - 2 * gap);

      /* Render the description lines */
      for(auto& line : desc_split)
      {
        item_description.setText(renderer, line, kCOLOR_TEXT);
        item_description.render(renderer, start.x + gap, current.y);
        current.y += item_description.getHeight() * 1.1;
      }

      Text item_flags{getFont(FontName::M_STANDARD)};
      item_flags.setText(renderer, calcItemDetailsString(item), kCOLOR_TEXT);

      auto width = inventory_top_box.width - corner_inset;

      item_flags.render(renderer,
                        start.x + width / 2 - item_flags.getWidth() / 2,
                        start.y + bot_height - item_flags.getHeight() - gap);
    }
  }
}

/* Renders the Options Screen */
void Menu::renderOptions()
{
  auto width = config->getScaledWidth();
  auto height = config->getScaledHeight();

  UCoordinate ucurrent{main_section.location.point.x +
                           (uint32_t)std::round(kOPTIONS_X * width),
                       main_section.location.point.y +
                           (uint32_t)std::round(kOPTIONS_Y * height)};

  auto y_gap = (uint32_t)std::round(kOPTIONS_Y_GAP * height);

  /* Render the audio level */
  auto end = renderOptionAnalog(option_audio_level, ucurrent);

  /* Render the music level */
  ucurrent.y = end.y + y_gap;
  end = renderOptionAnalog(option_music_level, ucurrent);

  /* Render the UI scaling */
  ucurrent.y = end.y + y_gap;
  end = renderOptionAnalog(option_scaling_ui_level, ucurrent);

  /* Render the auto run flag */
  ucurrent.y = end.y + y_gap;
  end = renderOptionDigital(option_auto_run, ucurrent);

  /* Render the mute flag */
  ucurrent.y = end.y + y_gap;
  end = renderOptionDigital(option_mute, ucurrent);

  /* Render the fast battel flag */
  ucurrent.y = end.y + y_gap;
  renderOptionDigital(option_fast_battle, ucurrent);
}

/* Render a given option at a given point */
UCoordinate Menu::renderOptionAnalog(AnalogOption& option, UCoordinate point)
{
  UCoordinate end{0, 0};

  if(renderer && config && option.val)
  {
    auto width = config->getScaledWidth();
    auto height = config->getScaledHeight();
    auto bar_gap = (uint32_t)std::round(kOPTIONS_Y_BAR_GAP * height);
    auto box_size = (int32_t)std::round(kOPTIONS_BOX_SIZE * width);
    auto start_x = (int32_t)point.x;

    /* Render the Option Name Text */
    SDL_Color fill_color = kCOLOR_OPTION_FILL;
    SDL_Color border_color = {255, 255, 255, 65};

    if(option.location.getFlag(BoxState::SELECTED))
    {
      border_color = kCOLOR_MAIN_BORDER;
      fill_color = kCOLOR_OPTION_FILL_SELECTED;
    }

    /* Create and render the option name text */
    Text t{getFont(FontName::M_HEADER)};
    t.setText(renderer, option.name, kCOLOR_TEXT);
    t.render(renderer, point.x, point.y);

    /* Update the running coordinate, past the text and add the bar gap */
    point.y += t.getHeight() + bar_gap;

    /* Render the Selected Boxes Sized (box_size x box_size) */
    auto num_filled = (int32_t)(*(option.val) / 100.0 * option.num_options);
    auto num_unfilled = (int32_t)(option.num_options - num_filled);

    for(int32_t i = 0; i < num_filled; i++)
    {
      Box filled_box{Coordinate{(int32_t)point.x, (int32_t)point.y}, box_size,
                     box_size};
      filled_box.color_bg = fill_color;
      filled_box.color_border = {255, 255, 255, 65};
      filled_box.render(renderer);
      point.x += box_size;
    }

    /* Render the Unselected Boxes */
    for(int32_t i = 0; i < num_unfilled; i++)
    {
      Box filled_box{Coordinate{(int32_t)point.x, (int32_t)point.y}, box_size,
                     box_size};
      filled_box.color_border = {255, 255, 255, 65};
      filled_box.render(renderer);
      point.x += box_size;
    }

    /* Render the Border Around the Boxes W = Point.X = Original Point.x */
    Box border_box{Coordinate{start_x, (int32_t)point.y},
                   (int32_t)(point.x - start_x), box_size};
    border_box.color_border = border_color;
    border_box.render(renderer);

    end.x = point.x;
    end.y = point.y + box_size;
  }

  return end;
}

/* Render a given option at a given point */
UCoordinate Menu::renderOptionDigital(DigitalOption& option, UCoordinate point)
{
  UCoordinate end{0, 0};

  if(renderer && config)
  {
    auto width = config->getScaledWidth();
    auto text_gap = (uint32_t)std::round(kOPTIONS_DIGITAL_TEXT_GAP * width);
    auto box_size = (int32_t)std::round(kOPTIONS_BOX_SIZE * width);
    auto start_x = (int32_t)point.x;
    SDL_Color box_border = {255, 255, 255, 65};

    if(option.location.getFlag(BoxState::SELECTED))
      box_border = kCOLOR_MAIN_BORDER;

    /* Render the Box */
    Box digital_box{{start_x, (int32_t)point.y}, box_size, box_size};
    digital_box.color_border = box_border;

    /* Render the Check if flag is set */
    if(option.isSet())
    {
      frame_checkbox->render(
          renderer,
          point.x + digital_box.width / 2 - frame_checkbox->getWidth() / 2,
          point.y + digital_box.height / 2 - frame_checkbox->getHeight() / 2);
    }

    digital_box.render(renderer);

    /* Render the Digital Option Name */
    Text t{getFont(FontName::M_HEADER)};
    t.setText(renderer, option.name, kCOLOR_TEXT);
    t.render(renderer, point.x + text_gap, point.y);

    end.y = point.y + t.getHeight();
  }

  return end;
}

void Menu::renderPersonElementTitles(int32_t gap)
{
  if(config && renderer && person_element_index > -1 &&
     person_element_index < (int32_t)person_title_elements.size() &&
     person_title_elements.size() > 0)
  {

    /* Render the required number of boxes for the element titles */
    auto point = Coordinate{current.x, current.y + gap};
    auto box_length = (int32_t)std::ceil(s_top_box.width * 1.0 /
                                         person_title_elements.size());
    auto box_height = s_top_box.height;

    int32_t max_remain_length = s_top_box.width + 2;

    int32_t index{0};

    for(auto& title : person_title_elements)
    {
      auto actual_length = std::min(max_remain_length, box_length);
      title.title_box = Box(point, actual_length, box_height);
      max_remain_length -= actual_length - 1;
      setupDefaultBox(title.title_box);

      if(index == person_element_index)
      {
        // title.title_box.setFlag(BoxState::SELECTED, true);
        title.title_box.color_bg = kCOLOR_BORDER_UNSELECTED;
      }
      else
      {
        title.title_box.setFlag(BoxState::SELECTED, false);
      }

      point.x += box_length - 1;
      index++;
    }

    for(uint32_t i = 0; i < person_title_elements.size(); i++)
      if((int32_t)i != person_element_index)
        renderPersonElementTitle(person_title_elements.at(i));

    for(uint32_t i = 0; i < person_title_elements.size(); i++)
      if((int32_t)i == person_element_index)
        renderPersonElementTitle(person_title_elements.at(i));
  }
}

void Menu::renderPersonElementTitle(TitleElement& element)
{
  element.title_box.render(renderer);

  Text t{getFont(FontName::M_SMALL_TITLE_ELM)};
  t.setText(renderer, element.name, kCOLOR_TEXT);

  /* Render the text centered in the title box */
  auto b = element.title_box;
  t.render(renderer, b.point.x + b.width / 2 - t.getWidth() / 2,
           b.point.y + b.height / 2 - t.getHeight() / 2);
}

/* Renders the Sleuth Screen */
void Menu::renderSleuth()
{
  /* Obtained values */
  auto width = config->getScaledWidth();
  auto start = main_section.location.point;

  /* Calculated values */
  auto gap = (int32_t)std::round(width * kSLEUTH_GAP);
  current.y = start.y + gap;

  /* Render the Sleuth Members Face Boxes */
  for(uint32_t i = 0; i < titles.size(); i++)
  {
    auto& icon = titles.at(i);

    icon.point.x = start.x + gap;
    icon.point.y = current.y;
    icon.render(renderer);
    current.y += gap + icon.height;

    // TODO [05-21-16]: Properly implement face graphics post-alpha
    // -- editor face graphics saving
    // -- maintain face graphics inside of person
    if(i == 0)
    {
      Frame* face = new Frame(
          config->getBasePath() + "sprites/Map/Faces/player.png", renderer);

      if(face)
      {
        face->render(renderer, icon.point.x + icon.width / 2 - face->getWidth(),
                     icon.point.y + icon.height / 2 - face->getHeight(),
                     face->getWidth() * 2, face->getHeight() * 2);
        delete face;
      }

      face = nullptr;
      renderTitleTriangle(icon);
    }
  }

  s_top_box.point = {start.x + 2 * gap + calcSleuthTileSize(), start.y};
  s_top_box.width =
      main_section.location.width - calcSleuthTileSize() - 3 * gap;
  s_top_box.height = (main_section.location.height - 3 * gap) / 10;

  current = s_top_box.point;
  renderPersonElementTitles(gap);

  if(getSleuthMenuType() == MenuType::SLEUTH_OVERVIEW)
    renderSleuthOverview();
  else if(getSleuthMenuType() == MenuType::SLEUTH_EQUIPMENT)
    renderSleuthEquipment();
  else if(getSleuthMenuType() == MenuType::SLEUTH_SKILLS)
    renderSleuthSkills();
  // else if(getSleuthMenuType() == MenuType::SLEUTH_DETAILS)
  //   renderSleuthDetails();
}

void Menu::renderSleuthOverview()
{
  /* Sanity */
  if(sleuth_element_index == -1)
    sleuth_element_index = 0;

  auto start = main_section.location.point;
  auto width = config->getScaledWidth();
  auto gap = (int32_t)std::round(width * kSLEUTH_GAP);
  auto person = getCurrentPerson();
  auto actor = getCurrentActor();
  auto equip_icon_size = (int32_t)std::round(width * kSLEUTH_EQUIP_ICON_SIZE);
  auto main = main_section.location;

  assert(person);
  assert(actor);
  assert(config);
  assert(renderer);
  assert(frame_exp_empty);
  assert(frame_exp_full);
  assert(frame_exp_middle);

  /* Render the sleuth top box with name */
  current.y = start.y + gap;
  auto icon_w = calcSleuthTileSize();

  Text t_name{getFont(FontName::M_TITLE_ELM)};

  s_top_box.point.y += s_top_box.height + gap - 1;
  setupDefaultBox(s_top_box);
  s_top_box.render(renderer);

  /* Render the primary and secondary elements of the person at the bottom
   * of the sprite box */
  auto f_prim = battle_display_data->getFrameElement(person->getPrimary());
  auto f_secd = battle_display_data->getFrameElement(person->getSecondary());

  Coordinate elements{s_top_box.point.x + s_top_box.width - gap,
                      s_top_box.point.y};

  if(f_prim && f_secd)
  {
    f_secd->render(renderer, elements.x - f_secd->getWidth(),
                   elements.y + s_top_box.height / 2 - f_secd->getHeight() / 2);
    elements.x -= f_secd->getWidth() + gap / 2;

    f_prim->render(renderer, elements.x - f_prim->getWidth(),
                   elements.y + s_top_box.height / 2 - f_prim->getHeight() / 2);
  }

  if(person)
  {
    /* Render the player's name in the title box */
    t_name.setText(renderer, person->getName(), kCOLOR_TEXT);

    auto name_text_y = s_top_box.point.y + gap;
    current.x = s_top_box.point.x + gap;

    t_name.render(renderer, current.x, name_text_y);
  }

  /* Render the top stats box */
  s_top_stats_box.point = {start.x + 2 * gap + icon_w,
                           current.y +
                               (main_section.location.height - 3 * gap) / 10 -
                               2 + s_top_box.height};
  s_top_stats_box.width = s_top_box.width;
  s_top_stats_box.height =
      (main_section.location.height - s_top_box.height - gap * 4) / 4;
  setupDefaultBox(s_top_stats_box);
  s_top_stats_box.render(renderer);

  /* Render the details box */
  current.x = s_top_box.point.x;
  current.y = s_top_stats_box.point.y + s_top_stats_box.height + gap;
  renderSleuthDetailsRank();

  auto rank_box_width =
      (main_section.location.width - calcSleuthTileSize() - 4 * gap) / 2;

  /* Render the attributes box */
  s_attributes_box.point = {s_top_stats_box.point.x + rank_box_width + gap,
                            s_top_stats_box.point.y + s_top_stats_box.height +
                                gap};
  s_attributes_box.width = rank_box_width;
  s_attributes_box.height =
      main.height - s_top_stats_box.height - 2 * s_top_box.height - 3 * gap;
  setupDefaultBox(s_attributes_box);
  s_attributes_box.corner_inset = calcMainCornerInset();
  s_attributes_box.box_type = BoxType::CORNER_CUT_BOX;
  s_attributes_box.render(renderer);

  /* Render the sleuth top stats box */
  start = {s_top_stats_box.point.x, s_top_stats_box.point.y};
  auto stats = actor->getStatsRendered();
  current = Coordinate{start.x + gap, start.y + gap};

  /* Render the base frame exp */
  frame_exp_full->render(renderer, current.x, current.y);

  /* Render the exp gained frame as proportion of the amount gained */
  auto height =
      frame_exp_empty->getHeight() -
      (person->findExpPercent() / 100.0 * frame_exp_empty->getHeight());

  SDL_Rect exp_rect;
  exp_rect.x = 0;
  exp_rect.y = 0;
  exp_rect.h = height;
  exp_rect.w = frame_exp_full->getWidth();

  frame_exp_empty->render(renderer, current.x, current.y, 0, height, &exp_rect);

  /* Render the middle exp frame */
  frame_exp_middle->render(renderer,
                           current.x + frame_exp_empty->getWidth() / 2 -
                               frame_exp_middle->getWidth() / 2,
                           current.y + frame_exp_empty->getHeight() / 2 -
                               frame_exp_middle->getHeight() / 2);

  /* Render the level text centred to the frame middle exp */
  Text t_level_title{getFont(FontName::M_ITEM_HEADER)};
  Text t_level{getFont(FontName::M_TITLE_ELM)};

  t_level_title.setText(renderer, "LEVEL", kCOLOR_TEXT);
  t_level.setText(renderer, std::to_string(person->getLevel()), kCOLOR_TEXT);

  auto x = current.x;
  auto y = current.y;
  current.x += frame_exp_empty->getWidth() / 2 - t_level_title.getWidth() / 2;
  current.y += frame_exp_empty->getHeight() * 0.24;
  t_level_title.render(renderer, current.x, current.y);

  current.x = x + frame_exp_empty->getWidth() / 2 - t_level.getWidth() / 2;
  current.y += 3 * gap / 2;
  t_level.render(renderer, current.x, current.y);

  Text t_exp{getFont(FontName::M_OPTIONS)};
  Text t_vita{getFont(FontName::M_OPTIONS)};
  Text t_qtdr{getFont(FontName::M_OPTIONS)};

  std::string exp_str = "";

  if(person->getExpAt(person->getLevel() + 1) == 0)
  {
    exp_str = std::to_string(person->getTotalExp()) + " Experience";
  }
  else
  {
    exp_str = std::to_string(person->getTotalExp()) + " / " +
              std::to_string(person->getExpAt(person->getLevel() + 1)) +
              " Experience";
  }

  std::string vita_str =
      std::to_string(stats.getValue(Attribute::VITA)) + " / " +
      std::to_string(stats.getValue(Attribute::MVIT)) + " Vitality";

  std::string qtdr_str =
      std::to_string(stats.getValue(Attribute::QTDR)) + " / " +
      std::to_string(stats.getValue(Attribute::MQTD)) + " Quantum Drive";

  t_exp.setText(renderer, exp_str, kCOLOR_TEXT);
  t_vita.setText(renderer, vita_str, kCOLOR_TEXT);
  t_qtdr.setText(renderer, qtdr_str, kCOLOR_TEXT);

  current.x = x + frame_exp_empty->getWidth();
  current.y = y + frame_exp_empty->getHeight() / 10;
  t_exp.render(renderer, current.x, current.y);
  current.y = y + frame_exp_empty->getHeight() / 2;

  auto health_pc = actor->getPCVita() / 100.0;
  auto qtdr_pc = actor->getPCQtdr() / 100.0;
  health_pc = Helpers::setInRange(health_pc, 0.0, 1.0);
  qtdr_pc = Helpers::setInRange(qtdr_pc, 0.0, 1.0);

  /* Render vitality bar */
  auto vita_height = t_vita.getHeight() * 0.75;
  s_vita_bar.point = current;
  s_vita_bar.width = (int32_t)std::round(kBAR_VITA_WIDTH * width);
  s_vita_bar.height = vita_height;
  setupDefaultBox(s_vita_bar);
  s_vita_bar.color_bg = kCOLOR_BORDER_UNSELECTED;
  s_vita_bar.color_border = {230, 230, 230, 255};
  s_vita_bar.bar_amount = health_pc;
  s_vita_bar.color_bar = {(uint8_t)(150 * (1 - health_pc) * 2), 150, 0, 255};
  s_vita_bar.box_type = BoxType::BAR;
  s_vita_bar.bar_degrees = 59.5;
  s_vita_bar.render(renderer);

  current.x += s_vita_bar.width + gap;
  t_vita.render(renderer, current.x, current.y);

  /* Render quantum drive bar */
  auto qtdr_height = t_qtdr.getHeight() * 0.75;
  current.x = current.x - s_vita_bar.width - 3 * gap / 2;
  current.y = y + frame_exp_empty->getHeight() - qtdr_height;

  s_qtdr_bar.point = current;
  s_qtdr_bar.width = (int32_t)std::round(kBAR_QTDR_WIDTH * width);
  s_qtdr_bar.height = qtdr_height;
  setupDefaultBox(s_vita_bar);
  s_qtdr_bar.color_bg = kCOLOR_BORDER_UNSELECTED;
  s_qtdr_bar.color_border = {230, 230, 230, 255};
  s_qtdr_bar.color_bar = {58, 170, 198, 255};
  s_qtdr_bar.bar_amount = qtdr_pc;
  s_qtdr_bar.bar_degrees = 59.5;
  s_qtdr_bar.box_type = BoxType::BAR;
  s_qtdr_bar.render(renderer);

  current.x += s_qtdr_bar.width + gap;
  t_qtdr.render(renderer, current.x, current.y);

  /* Render the sleuth attributes */
  renderAttributes(s_attributes_box.point, gap);
}

void Menu::renderSleuthEquipment()
{
  /* Render the sleuth equipment */
}

/* Render the skills screen for the current person in sleuth selection */
void Menu::renderSleuthSkills()
{
  auto width = config->getScaledWidth();
  auto gap = (int32_t)std::round(width * kSLEUTH_GAP);

  /* Useable height between the top box and the details box */
  auto useable_height =
      main_section.location.height - 3 * gap - s_top_box.height;

  /* Icon Width */
  int32_t icon_w = main_section.location.height / 6;

  /* Render the top box */
  skills_top_box.point.x = s_top_box.point.x;
  skills_top_box.point.y = current.y + s_top_box.height + gap - 1;
  skills_top_box.height = std::round(3.0 * useable_height / 5);
  skills_top_box.width = s_top_box.width;
  setupDefaultBox(skills_top_box);
  skills_top_box.render(renderer);

  /* Render the Skills Title Box */
  skills_name_box.point = skills_top_box.point;
  skills_name_box.height = skills_top_box.height / 5;
  skills_name_box.width = s_top_box.width;
  setupDefaultBox(skills_name_box);
  skills_name_box.render(renderer);

  /* Render the skills title name */
  Text title_text{getFont(FontName::M_TITLE_ELM)};
  title_text.setText(renderer, "Skills", kCOLOR_TEXT);
  title_text.render(renderer, skills_name_box.point.x + gap,
                    skills_name_box.point.y + skills_name_box.height / 2 -
                        title_text.getHeight() / 2);

  /* Render the skills scroll box inside of the top box */
  skills_scroll_box.point = {skills_top_box.point.x,
                             skills_name_box.point.y + skills_name_box.height -
                                 1};
  skills_scroll_box.width = skills_top_box.width;
  skills_scroll_box.height = skills_top_box.height - skills_name_box.height + 1;
  setupDefaultBox(skills_scroll_box);
  skills_scroll_box.color_element_selected = kCOLOR_INVENTORY_ICON_FILL;
  skills_scroll_box.color_scroll = kCOLOR_OPTION_FILL_SELECTED;
  skills_scroll_box.color_scroll_selected = kCOLOR_OPTION_FILL_SELECTED;
  skills_scroll_box.element_inset_x = gap;
  skills_scroll_box.element_inset_y = gap;
  skills_scroll_box.scroll_inset_x = gap;
  skills_scroll_box.scroll_inset_y = gap + calcSkillTitleHeight() / 2;
  skills_scroll_box.scroll_width = 10;
  skills_scroll_box.render(renderer);

  /* Render the bottom skill frames box - detail section */
  skills_bot_box.point.x = s_top_box.point.x;
  skills_bot_box.point.y = skills_top_box.point.y + skills_top_box.height + gap;
  skills_bot_box.height = useable_height - skills_top_box.height;
  skills_bot_box.width = s_top_box.width;
  setupDefaultBox(skills_bot_box);
  skills_bot_box.corner_inset = calcMainCornerInset();
  skills_bot_box.box_type = BoxType::CORNER_CUT_BOX;
  skills_bot_box.render(renderer);

  if(skills_element_index != -1)
    renderSleuthSkillDetail(skills_bot_box.point, icon_w, gap);
}

void Menu::renderSleuthSkillDetail(Coordinate start, int32_t icon_w,
                                   int32_t gap)
{
  auto curr_skill = getCurrentSkill();

  if(curr_skill)
  {
    skills_icon_box.point = {start.x + gap, start.y + gap};
    skills_icon_box.width = icon_w;
    skills_icon_box.height = icon_w;
    skills_icon_box.color_bg = kCOLOR_ICON_UNSELECTED_FILL;
    skills_icon_box.color_border = kCOLOR_BORDER_UNSELECTED;
    skills_icon_box.render(renderer);

    current = skills_icon_box.point;

    /* Render the skill icon */
    if(curr_skill->getThumbnail())
    {
      auto t = curr_skill->getThumbnail();
      t->render(renderer, skills_icon_box.point.x + skills_icon_box.width / 2 -
                              t->getWidth() / 2,
                skills_icon_box.point.y + skills_icon_box.height / 2 -
                    t->getHeight() / 2);
    }

    current.x += icon_w + gap;

    /* Render the Skills Text */
    Text skill_name{getFont(FontName::M_OPTIONS)};
    Text skill_qd{getFont(FontName::M_ITEM_HEADER)};
    Text skill_acc{getFont(FontName::M_ITEM_HEADER)};
    Text skill_cd{getFont(FontName::M_ITEM_HEADER)};
    Text skill_description{getFont(FontName::M_STANDARD)};

    skill_name.setText(renderer, curr_skill->getName(), kCOLOR_TEXT);
    skill_qd.setText(renderer, std::to_string(curr_skill->getCost()),
                     kCOLOR_TEXT);
    skill_acc.setText(
        renderer, std::to_string((int32_t)std::round(curr_skill->getChance())),
        kCOLOR_TEXT);
    skill_cd.setText(renderer, std::to_string(curr_skill->getCooldown()),
                     kCOLOR_TEXT);

    auto desc_split = Text::splitLine(getFont(FontName::M_STANDARD),
                                      curr_skill->getDescription(),
                                      skills_bot_box.width - 8 * gap - icon_w);

    skill_name.render(renderer, current.x, current.y);
    current.y += skill_name.getHeight() + gap;

    for(auto& line : desc_split)
    {
      skill_description.setText(renderer, line, kCOLOR_TEXT);
      skill_description.render(renderer, current.x, current.y);
      current.y += skill_description.getHeight() * 1.1;
    }

    /* Render the skill cost, accuracy, cooldown */
    current.x = skills_bot_box.point.x + skills_bot_box.width - gap;
    current.y = skills_bot_box.point.y + gap;

    auto f_qd = battle_display_data->getFrameQD();

    if(f_qd)
    {
      f_qd->render(renderer, current.x - f_qd->getWidth(), current.y);
      current.x -= f_qd->getWidth() + gap / 2 + skill_qd.getWidth();
      skill_qd.render(renderer, current.x, current.y);
      current.y += skill_qd.getHeight() + gap;
    }

    auto f_acc = battle_display_data->getFramePercent();
    current.x = skills_bot_box.point.x + skills_bot_box.width - gap;

    if(f_acc)
    {
      f_acc->render(renderer, current.x - f_acc->getWidth(), current.y);
      current.x -= f_acc->getWidth() + gap / 2 + skill_acc.getWidth();
      skill_acc.render(renderer, current.x, current.y);
      current.y += skill_acc.getHeight() + gap;
    }

    auto f_time = battle_display_data->getFrameTime();
    current.x = skills_bot_box.point.x + skills_bot_box.width - gap;

    if(f_time)
    {
      f_time->render(renderer, current.x - f_time->getWidth(), current.y);
      current.x -= f_time->getWidth() + gap / 2 + skill_cd.getWidth();
      skill_cd.render(renderer, current.x, current.y);
    }

    /* Render the action scope frame */
    current.x = skills_bot_box.point.x + gap;

    auto f_scope = battle_display_data->getFrameScope(curr_skill->getScope());

    if(f_scope)
    {
      f_scope->render(renderer, current.x, skills_bot_box.point.y +
                                               skills_bot_box.height -
                                               f_scope->getHeight() - gap);

      current.x += f_scope->getWidth() + gap;
    }

    /* Render the primary element frame */
    auto f_pri = battle_display_data->getFrameElement(curr_skill->getPrimary());

    if(f_pri)
    {
      f_pri->render(renderer, current.x, skills_bot_box.point.y +
                                             skills_bot_box.height -
                                             f_pri->getHeight() - gap);

      current.x += f_pri->getWidth() + gap;
    }

    /* Render the secondary element frame */
    auto f_sec =
        battle_display_data->getFrameElement(curr_skill->getSecondary());

    if(f_sec)
    {
      f_sec->render(renderer, current.x, skills_bot_box.point.y +
                                             skills_bot_box.height -
                                             f_pri->getHeight() - gap);

      current.x += f_sec->getWidth() + gap;
    }
  }
}

void Menu::renderSleuthDetails()
{
  auto width = config->getScaledWidth();
  auto gap = (int32_t)std::round(width * kSLEUTH_GAP);

  /* Useable height */
  current.y = main_section.location.point.y + s_top_box.height + gap - 1;
  current.x = s_top_box.point.x;

  /* Render the top box with text "Details" */
  s_top_box.point.y = current.y;
  setupDefaultBox(s_top_box);
  s_top_box.render(renderer);

  current.y += s_top_box.height + gap;

  Text t_name{getFont(FontName::M_TITLE_ELM)};
  t_name.setText(renderer, "Details", kCOLOR_TEXT);
  t_name.render(renderer, s_top_box.point.x + gap, s_top_box.point.y +
                                                       s_top_box.height / 2 -
                                                       t_name.getHeight() / 2);

  /* Render the sections of the Sleuth Details screen */
  // renderSleuthDetailsExp();
  // renderSleuthDetailsStats();
}

/* Render a detailed breakdown of the experience for the person */
void Menu::renderSleuthDetailsStats()
{
  auto gap = (int32_t)std::round(config->getScaledWidth() * kSLEUTH_GAP);
  auto person = getCurrentPerson();

  if(person)
  {
    current.x = s_rank_box.point.x + s_rank_box.width + gap;
    current.y = s_rank_box.point.y;

    s_details_box.point = current;
    s_details_box.width = main_section.location.width - calcSleuthTileSize() -
                          s_rank_box.width - 4 * gap;
    s_details_box.height = s_rank_box.height;
    //    main_section.location.height - titles.at(0).height - 3 * gap;
    setupDefaultBox(s_details_box);
    // s_details_box.corner_inset = calcMainCornerInset();
    // s_details_box.box_type = BoxType::CORNER_CUT_BOX;
    s_details_box.render(renderer);
  }
}

/* Render the extra attributes for the person (MMNT, LIMB, UNBR) */
void Menu::renderSleuthDetailsExp()
{
  auto width = config->getScaledWidth();
  auto gap = (int32_t)std::round(width * kSLEUTH_GAP);
  auto person = getCurrentPerson();

  if(person)
  {
    current.x = s_rank_box.point.x;
    current.y += s_rank_box.height + gap;

    s_exp_box.point = current;
    s_exp_box.height = main_section.location.height -
                       (s_rank_box.point.y + s_rank_box.height -
                        main_section.location.point.y) -
                       gap * 2;
    s_exp_box.width = s_rank_box.width;
    setupDefaultBox(s_exp_box);
    s_exp_box.render(renderer);

    Text t_exp_title{getFont(FontName::M_OPTIONS)};
    Text t_exp_total{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_exp_curr{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_exp_next{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_exp_progress{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_exp_val_total{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_exp_val_curr{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_exp_val_next{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_exp_val_progress{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_mod_title{getFont(FontName::M_OPTIONS)};
    Text t_mod_exp{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_mod_dmg{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_mod_val_exp{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_mod_val_dmg{getFont(FontName::M_ITEM_HEADER_14)};
    Text t_max_exp{getFont(FontName::M_ITEM_HEADER_14)};

    auto total_exp = person->getTotalExp();
    auto exp_at_curr = Person::getExpAt(person->getLevel());
    auto exp_at_next = Person::getExpAt(person->getLevel() + 1);
    auto total_str = std::to_string(person->getTotalExp());
    auto curr_exp = std::to_string(total_exp - exp_at_curr);
    auto mod_exp = std::to_string((int32_t)person->getExpMod() * 100) + "\%";
    auto mod_dmg = std::to_string((int32_t)person->getDmgMod() * 100) + "\%";
    auto max_exp_str = "   " + Helpers::formatUInt(Person::getMaxExp(), " ");
    auto progress = std::to_string(person->findExpPercent()) + "\%";
    std::string next_exp = "MAX LEVEL";

    if(exp_at_next != 0)
      next_exp = std::to_string(exp_at_next);

    t_exp_title.setText(renderer, "Experience", kCOLOR_TEXT);
    t_exp_total.setText(renderer, "TOTAL ", kCOLOR_TEXT);
    t_exp_curr.setText(renderer, "THIS LEVEL", kCOLOR_TEXT);
    t_exp_next.setText(renderer, "NEXT LEVEL", kCOLOR_TEXT);
    t_exp_progress.setText(renderer, "PROGRESS", kCOLOR_TEXT);
    t_exp_val_total.setText(renderer, total_str, kCOLOR_TEXT);
    t_exp_val_curr.setText(renderer, curr_exp, kCOLOR_TEXT);
    t_exp_val_next.setText(renderer, next_exp, kCOLOR_TEXT);
    t_exp_val_progress.setText(renderer, progress, kCOLOR_TEXT);
    t_mod_title.setText(renderer, "Modifications", kCOLOR_TEXT);
    t_mod_exp.setText(renderer, "EXPERIENCE", kCOLOR_TEXT);
    t_mod_dmg.setText(renderer, "DAMAGE", kCOLOR_TEXT);
    t_mod_val_exp.setText(renderer, mod_exp, kCOLOR_TEXT);
    t_mod_val_dmg.setText(renderer, mod_dmg, kCOLOR_TEXT);
    t_max_exp.setText(renderer, max_exp_str, kCOLOR_TEXT);

    /* Setup for second column text */
    auto c2 = current;
    c2.x += t_mod_title.getWidth() + t_max_exp.getWidth() + 2 * gap;

    current = {current.x + gap, current.y + gap};

    t_exp_title.render(renderer, current.x, current.y);
    current.y += t_exp_title.getHeight() + 3 * gap / 2;

    t_exp_total.render(renderer, current.x, current.y);
    t_exp_val_total.render(renderer, c2.x - t_exp_val_total.getWidth(),
                           current.y);
    current.y += t_exp_total.getHeight() + 2 * gap / 3;

    t_exp_curr.render(renderer, current.x, current.y);
    t_exp_val_curr.render(renderer, c2.x - t_exp_val_curr.getWidth(),
                          current.y);
    current.y += t_exp_curr.getHeight() + 2 * gap / 3;

    t_exp_next.render(renderer, current.x, current.y);
    t_exp_val_next.render(renderer, c2.x - t_exp_val_next.getWidth(),
                          current.y);
    current.y += t_exp_curr.getHeight() + 2 * gap / 3;

    t_exp_progress.render(renderer, current.x, current.y);
    t_exp_val_progress.render(renderer, c2.x - t_exp_val_progress.getWidth(),
                              current.y);
    current.y += t_exp_next.getHeight() + 3 * gap / 2;

    t_mod_title.render(renderer, current.x, current.y);
    current.y += t_mod_title.getHeight() + 3 * gap / 2;

    t_mod_exp.render(renderer, current.x, current.y);
    t_mod_val_exp.render(renderer, c2.x - t_mod_val_exp.getWidth(), current.y);
    current.y += t_mod_exp.getHeight() + 2 * gap / 3;

    t_mod_dmg.render(renderer, current.x, current.y);
    t_mod_val_dmg.render(renderer, c2.x - t_mod_val_dmg.getWidth(), current.y);
    current.y += t_mod_dmg.getHeight() + 2 * gap / 3;
  }
}

void Menu::renderSleuthDetailsRank()
{
  auto gap = (int32_t)std::round(config->getScaledWidth() * kSLEUTH_GAP);
  auto person = getCurrentPerson();

  if(person)
  {
    /* Render the text for the bottom statistics box */
    auto rank_str = Helpers::rankToDisplayStr(person->getRank());
    auto prim_element = Helpers::elementToDisplayString(person->getPrimary());
    auto secd_element = Helpers::elementToDisplayString(person->getSecondary());
    auto prim_curve_str =
        Helpers::curveToString(person->getPrimaryCurve(), false);
    auto secd_curve_str =
        Helpers::curveToString(person->getSecondaryCurve(), false);

    /* Render the sleuth bottom stats box */
    s_rank_box.point = current;
    s_rank_box.width =
        (main_section.location.width - calcSleuthTileSize() - 4 * gap) / 2;
    s_rank_box.height = main_section.location.height / 4;
    setupDefaultBox(s_rank_box);
    s_rank_box.render(renderer);

    Text t_rank{getFont(FontName::M_OPTIONS)};
    Text t_race{getFont(FontName::M_OPTIONS)};
    Text t_race_title{getFont(FontName::M_ITEM_HEADER)};
    Text t_class{getFont(FontName::M_OPTIONS)};
    Text t_class_title{getFont(FontName::M_ITEM_HEADER)};
    Text t_prim_curve{getFont(FontName::M_CURVE)};
    Text t_prim_title{getFont(FontName::M_ITEM_HEADER)};
    Text t_prim{getFont(FontName::M_OPTIONS)};
    Text t_secd_curve{getFont(FontName::M_CURVE)};
    Text t_secd_title{getFont(FontName::M_ITEM_HEADER)};
    Text t_secd{getFont(FontName::M_OPTIONS)};

    t_rank.setText(renderer, rank_str, kCOLOR_TEXT);
    t_race.setText(renderer, person->getRace()->getName(), kCOLOR_TEXT);
    t_race_title.setText(renderer, "RACE", kCOLOR_TEXT);
    t_class.setText(renderer, person->getClass()->getName(), kCOLOR_TEXT);
    t_class_title.setText(renderer, "CLASS", kCOLOR_TEXT);
    t_prim_curve.setText(renderer, prim_curve_str, kCOLOR_ELEMENTAL_CURVE);
    t_prim_title.setText(renderer, "PRIMARY ELEMENT", kCOLOR_TEXT);
    t_prim.setText(renderer, prim_element, kCOLOR_TEXT);
    t_secd_curve.setText(renderer, secd_curve_str, kCOLOR_ELEMENTAL_CURVE);
    t_secd_title.setText(renderer, "SECONDARY ELEMENT", kCOLOR_TEXT);
    t_secd.setText(renderer, secd_element, kCOLOR_TEXT);

    auto start =
        Coordinate{s_rank_box.point.x + 3 * gap / 2, s_rank_box.point.y + gap};
    auto current = start;

    /* Render column one text */
    t_rank.render(renderer, current.x, current.y);
    current.y += t_rank.getHeight() + gap * 2 / 3;

    t_race_title.render(renderer, current.x, current.y);
    current.y += t_race_title.getHeight() + gap / 6;

    t_race.render(renderer, current.x, current.y);
    current.y += t_race.getHeight() + gap * 2 / 3;

    t_class_title.render(renderer, current.x, current.y);
    current.y += t_class_title.getHeight() + gap / 6;

    t_class.render(renderer, current.x, current.y);

    /* Render column two text */
    current.x = start.x + s_rank_box.width / 2 - gap * 3 / 2;
    current.y = start.y + t_rank.getHeight() + gap / 2;

    t_prim_title.render(renderer, current.x, current.y);
    current.y += t_prim_title.getHeight() + gap / 6;

    t_prim.render(renderer, current.x, current.y);
    auto curve_x = current.x + t_prim.getWidth() + gap / 3;
    auto curve_y = current.y + t_prim.getHeight() - t_prim_curve.getHeight();

    t_prim_curve.render(renderer, curve_x, curve_y);
    current.y += t_prim.getHeight() + gap * 2 / 3;

    t_secd_title.render(renderer, current.x, current.y);
    current.y += t_secd_title.getHeight() + gap / 6;

    t_secd.render(renderer, current.x, current.y);
    curve_x = current.x + t_secd.getWidth() + gap / 3;
    curve_y = current.y + t_secd.getHeight() - t_secd_curve.getHeight();

    t_secd_curve.render(renderer, curve_x, curve_y);
  }
}

/* Renders the Save Screen */
void Menu::renderSave()
{
  auto main = main_section.location;
  auto gap = (int32_t)std::round(main.width * kSAVE_GAP);
  auto save_width = (int32_t)std::round(main.width * kSAVE_ELEMENT_WIDTH);
  auto save_height = (int32_t)std::round(main.height * kSAVE_ELEMENT_HEIGHT);

  save_scroll_box.setFlag(BoxState::SCROLL_BOX);
  save_scroll_box.setFlag(BoxState::SELECTABLE);

  save_scroll_box.color_border = {0, 0, 0, 0};
  save_scroll_box.color_border_selected = {0, 0, 0, 0};

  save_scroll_box.color_element_border_selected = {255, 255, 255, 255};
  save_scroll_box.color_element_border = {125, 125, 125, 255};
  save_scroll_box.color_bg = {0, 0, 0, 0};
  save_scroll_box.color_bg_selected = {0, 0, 0, 0};

  save_scroll_box.point.x = main.point.x + gap;
  save_scroll_box.point.y = main.point.y + gap;
  save_scroll_box.height = main.height - 3 * gap;
  save_scroll_box.width = main.width - 2 * gap;
  save_scroll_box.element_gap = gap;

  save_scroll_box.render(renderer);

  if(layer == MenuLayer::POPUP)
  {
    Box save_popup_box;
    setupDefaultBox(save_popup_box);
    save_popup_box.color_border = {255, 255, 255, 255};

    auto p_width = std::round(config->getScaledWidth() * kSAVE_POPUP_WIDTH);
    auto p_height = std::round(config->getScaledHeight() * kSAVE_POPUP_HEIGHT);
    auto popup_gap = std::round(config->getScaledHeight() * kSAVE_POPUP_GAP);

    save_popup_box.point.x = main.point.x + main.width / 2 - p_width / 2;
    save_popup_box.point.y = main.point.y + main.height / 2 - p_height / 2;
    save_popup_box.width = p_width;
    save_popup_box.height = p_height;

    save_popup_box.render(renderer);

    Text t_cancel(getFont(FontName::M_HEADER));
    Text t_save(getFont(FontName::M_HEADER));
    Text t_delete(getFont(FontName::M_HEADER));

    t_cancel.setText(renderer, "Cancel", kCOLOR_TEXT);
    t_save.setText(renderer, "Save", kCOLOR_TEXT);
    t_delete.setText(renderer, "Delete", kCOLOR_TEXT);

    SDL_Rect rect;
    rect.w = std::max(t_cancel.getWidth(), t_save.getWidth());
    rect.w = std::max(rect.w, t_delete.getWidth()) * 1.45;
    rect.h = t_delete.getHeight() * 1.45;

    current.x = save_popup_box.point.x;
    current.y = save_popup_box.point.y + popup_gap / 2;

    t_cancel.render(renderer, current.x + p_width / 2 - t_cancel.getWidth() / 2,
                    current.y);

    if(save_element_index == 1)
    {
      rect.x = current.x + p_width / 2 - rect.w / 2;
      rect.y = current.y + t_cancel.getHeight() / 2 - rect.h / 2;

      Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
    }

    current.y += popup_gap;
    t_save.render(renderer, current.x + p_width / 2 - t_save.getWidth() / 2,
                  current.y);

    if(save_element_index == 2)
    {
      rect.x = current.x + p_width / 2 - rect.w / 2;
      rect.y = current.y + t_save.getHeight() / 2 - rect.h / 2;

      Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
    }

    current.y += popup_gap;
    t_delete.render(renderer, current.x + p_width / 2 - t_delete.getWidth() / 2,
                    current.y);

    if(save_element_index == 3)
    {
      rect.x = current.x + p_width / 2 - rect.w / 2;
      rect.y = current.y + t_delete.getHeight() / 2 - rect.h / 2;

      Frame::renderRectSelect(rect, renderer, kCOLOR_TITLE_HOVER);
    }
  }
}

/* Renders the Quit Screen */
void Menu::renderQuit()
{
  auto gap = (int32_t)std::round(config->getScaledWidth() * kSLEUTH_GAP);

  /* Render the are you sure text */
  Text t_question{getFont(FontName::M_TITLE_ELM)};
  Text t_yes{getFont(FontName::M_TITLE_ELM)};
  Text t_no{getFont(FontName::M_TITLE_ELM)};

  t_question.setText(renderer, "Are you sure?", kCOLOR_TEXT);
  t_yes.setText(renderer, "Yes", kCOLOR_TEXT);
  t_no.setText(renderer, "No", kCOLOR_TEXT);

  current.x = main_section.location.point.x;
  auto start = current;
  current.y = main_section.location.point.y + main_section.location.height / 3;

  t_question.render(renderer, current.x + main_section.location.width / 2 -
                                  t_question.getWidth() / 2,
                    current.y);

  current.x =
      start.x + main_section.location.width / 2 - 3 * gap - t_yes.getWidth();
  current.y += main_section.location.height / 3;

  Box yes_box;
  yes_box.width = t_yes.getWidth() * 1.4;
  yes_box.height = t_yes.getWidth() * 1.2;
  yes_box.point = {current.x, current.y - yes_box.height / 2};
  yes_box.color_bg = SDL_Color{0, 0, 0, 0};
  yes_box.color_bg_selected = kCOLOR_OPTION_FILL_SELECTED;
  yes_box.color_border = SDL_Color{0, 0, 0, 0};
  yes_box.color_border_selected = SDL_Color{0, 0, 0, 0};

  if(quit_index == QuitIndex::YES)
    yes_box.setFlag(BoxState::SELECTED, true);

  yes_box.render(renderer);

  t_yes.render(renderer,
               yes_box.point.x + yes_box.width / 2 - t_yes.getWidth() / 2,
               yes_box.point.y + yes_box.height / 2 - t_yes.getHeight() / 2);

  current.x = start.x + main_section.location.width / 2 + 3 * gap;

  Box no_box;
  no_box.width = t_yes.getWidth() * 1.4;
  no_box.height = t_yes.getWidth() * 1.2;
  no_box.point = {current.x, current.y - no_box.height / 2};
  no_box.color_bg = SDL_Color{0, 0, 0, 0};
  no_box.color_bg_selected = kCOLOR_OPTION_FILL_SELECTED;
  no_box.color_border = SDL_Color{0, 0, 0, 0};
  no_box.color_border_selected = SDL_Color{0, 0, 0, 0};

  if(quit_index == QuitIndex::NO)
    no_box.setFlag(BoxState::SELECTED, true);

  no_box.render(renderer);

  t_no.render(renderer, no_box.point.x + no_box.width / 2 - t_no.getWidth() / 2,
              no_box.point.y + no_box.height / 2 - t_no.getHeight() / 2);
}

void Menu::setupDefaultBox(Box& setup_box)
{
  setup_box.color_bg = kCOLOR_TITLE_BG;
  setup_box.color_bg_selected = kCOLOR_TITLE_BG;
  setup_box.color_border = kCOLOR_BORDER_UNSELECTED;
  setup_box.color_border_selected = kCOLOR_TITLE_BORDER;
}

BattleActor* Menu::getCurrentActor()
{
  if(sleuth_element_index < (int32_t)actors.size() && sleuth_element_index >= 0)
    return actors.at(sleuth_element_index);

  return nullptr;
}

Skill* Menu::getCurrentSkill()
{
  auto curr_person = getCurrentPerson();

  if(curr_person)
  {
    auto curr_skills = curr_person->getCurrSkills();

    if(curr_skills && skills_element_index > -1 &&
       skills_element_index < (int32_t)curr_skills->getSize())
    {
      return curr_skills->getElement(skills_element_index).skill;
    }
  }

  return nullptr;
}

Person* Menu::getCurrentPerson()
{
  if(curr_player && curr_player->getSleuth())
  {
    auto members = curr_player->getSleuth()->getMembers();

    if(sleuth_element_index >= 0 &&
       sleuth_element_index < (int32_t)members.size())
      return members.at(sleuth_element_index);
  }

  return nullptr;
}

TTF_Font* Menu::getFont(FontName font_name)
{
  if(config && event_handler)
  {
    auto font = config->getFontTTF(font_name);

    if(font)
      return font;
    else
      event_handler->logError("Null font requested");
  }

  return nullptr;
}

/* Returns the enumerated MenuType of the Menu Seciton currently */
MenuType Menu::getMainMenuType()
{
  if(title_element_index != -1 &&
     (uint32_t)title_element_index < title_elements.size())
    return title_elements.at(title_element_index).menu_type;

  return MenuType::INVALID;
}

MenuType Menu::getSleuthMenuType()
{
  if(person_element_index == 0)
    return MenuType::SLEUTH_OVERVIEW;
  // TODO [05-22-16]: Add equipment screen when equipment is ready
  // else if(person_element_index == 1)
  //   return MenuType::SLEUTH_EQUIPMENT;
  else if(person_element_index == 1)
    return MenuType::SLEUTH_SKILLS;
  // else if(person_element_index == 2)
  //   return MenuType::SLEUTH_DETAILS;

  return MenuType::INVALID;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Clear the menu, revert everything off */
void Menu::clear()
{
  main_section.status = WindowStatus::OFF;
  title_section.status = WindowStatus::OFF;

  clearActors();
  clearAttributeFrames();
  clearElementFrames();
  clearIconFrames();
  setFlag(MenuState::SHOWING, false);
}

/* Hides the menu */
void Menu::hide()
{
  event_handler->triggerSound(Sound::kID_SOUND_MENU_PREV, SoundChannels::MENUS);
  title_section.status = WindowStatus::HIDING;
  title_element_index = -1;
  layer = MenuLayer::INVALID;
}

void Menu::keyDownUp()
{
  if(layer == MenuLayer::TITLE)
  {
    if(title_element_index > 0)
    {
      event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                  SoundChannels::MENUS);

      title_element_index--;
    }
    else
      title_element_index = title_elements.size() - 1;
  }
  else if(layer == MenuLayer::MAIN)
  {
    if(getMainMenuType() == MenuType::OPTIONS)
    {
      if(option_element_index > 0)
        decrementOptionIndex();
    }
    else if(getMainMenuType() == MenuType::INVENTORY)
    {
      if(static_cast<uint32_t>(inventory_title_index) > 1)
      {
        decrementInventoryIndex();
        buildInventoryElements();
      }
    }
    else if(getMainMenuType() == MenuType::SAVE)
    {
      if(save_scroll_box.prevIndex())
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
      }
    }
    else if(getMainMenuType() == MenuType::SLEUTH)
    {
      if((uint32_t)sleuth_element_index > 1)
      {
        decrementSleuthIndex();
        buildSkillFrames();
      }
    }
  }
  else if(layer == MenuLayer::MAIN_INDENT)
  {
    if(getMainMenuType() == MenuType::SLEUTH)
    {
      if(person_element_index == 1)
      {
        if(skills_scroll_box.prevIndex())
        {
          event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                      SoundChannels::MENUS);
          skills_element_index--;
        }
      }
    }
    if(getMainMenuType() == MenuType::INVENTORY)
    {
      if(inventory_scroll_box.prevIndex())
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        inventory_element_index--;
      }
    }
  }
  else if(layer == MenuLayer::POPUP)
  {
    if(getMainMenuType() == MenuType::SAVE)
    {
      if(save_element_index > 1)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        save_element_index--;
      }
    }
  }
}

void Menu::keyDownDown()
{
  if(layer == MenuLayer::TITLE)
  {
    if(title_element_index + 1 < (int32_t)title_elements.size())
    {
      event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                  SoundChannels::MENUS);

      title_element_index++;
    }
    else
      title_element_index = 0;
  }
  else if(layer == MenuLayer::MAIN)
  {
    if(getMainMenuType() == MenuType::OPTIONS)
    {
      if((uint32_t)option_element_index + 1 <= kNUM_OPTIONS)
        incrementOptionIndex();
    }
    else if(getMainMenuType() == MenuType::INVENTORY)
    {
      if(inventory_title_index != InventoryIndex::KEY_ITEMS)
      {
        incrementInventoryIndex();
        buildInventoryElements();
      }
    }
    else if(getMainMenuType() == MenuType::SAVE)
    {
      if(save_scroll_box.nextIndex())
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
      }
    }
    else if(getMainMenuType() == MenuType::SLEUTH)
    {
      if(canIncrementSleuth())
      {
        incrementSleuthIndex();
        buildSkillFrames();
      }
    }
  }
  else if(layer == MenuLayer::MAIN_INDENT)
  {
    if(getMainMenuType() == MenuType::SLEUTH)
    {
      if(person_element_index == 1)
      {
        if(skills_scroll_box.nextIndex())
        {
          event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                      SoundChannels::MENUS);
          skills_element_index++;
        }
      }
    }
    if(getMainMenuType() == MenuType::INVENTORY)
    {
      if(inventory_scroll_box.nextIndex())
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        inventory_element_index++;
      }
    }
  }
  else if(layer == MenuLayer::POPUP)
  {
    if(getMainMenuType() == MenuType::SAVE)
    {
      if(save_element_index < 3)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        save_element_index++;
      }
    }
  }
}

void Menu::keyDownLeft()
{
  if(layer == MenuLayer::MAIN)
  {
    if(getMainMenuType() == MenuType::SLEUTH)
    {
      if(person_element_index > 0)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        person_element_index--;
      }
    }
    else if(getMainMenuType() == MenuType::OPTIONS)
    {
      if(option_element_index == 0)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        option_audio_level.decrease();
      }
      else if(option_element_index == 1)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        option_music_level.decrease();
      }
      else if(option_element_index == 2)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        option_scaling_ui_level.decrease();
        updateScalingFactor();
      }
    }
    else if(getMainMenuType() == MenuType::QUIT)
    {
      if(quit_index != QuitIndex::YES)
        incrementQuitIndex();
    }
  }
}

void Menu::keyDownRight()
{
  if(layer == MenuLayer::MAIN)
  {
    if(getMainMenuType() == MenuType::SLEUTH)
    {
      if((uint32_t)person_element_index + 1 < person_title_elements.size())
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        person_element_index++;
      }
    }
    else if(getMainMenuType() == MenuType::OPTIONS)
    {
      if(option_element_index == 0)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        option_audio_level.increase();
      }
      else if(option_element_index == 1)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        option_music_level.increase();
      }
      else if(option_element_index == 2)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT,
                                    SoundChannels::MENUS);
        option_scaling_ui_level.increase();
        updateScalingFactor();
      }
    }
    else if(getMainMenuType() == MenuType::QUIT)
    {
      if(quit_index != QuitIndex::NO)
        decrementQuitIndex();
    }
  }
}

void Menu::keyDownAction()
{
  if(title_section.status == WindowStatus::ON &&
     main_section.status == WindowStatus::OFF)
  {
    if(title_element_index < (int32_t)title_elements.size())
    {
      event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                  SoundChannels::MENUS);
      layer = MenuLayer::MAIN;

      /* Construct the main section with the appropriate parameters */
      buildMainSection(getMainMenuType());
      main_section.status = WindowStatus::SHOWING;
    }
  }
  else if(layer == MenuLayer::MAIN)
  {
    if(getMainMenuType() == MenuType::SLEUTH)
    {
      if(person_element_index == 1)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                    SoundChannels::MENUS);
        layer = MenuLayer::MAIN_INDENT;
        skills_top_box.setFlag(BoxState::SELECTED);
        skills_name_box.setFlag(BoxState::SELECTED);
        skills_scroll_box.setFlag(BoxState::SELECTED);
      }
    }
    /* Key down event on the options. --> If digital option, select flag */
    else if(getMainMenuType() == MenuType::OPTIONS)
    {
      if(option_element_index == 3)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                    SoundChannels::MENUS);
        option_auto_run.toggle();
      }
      else if(option_element_index == 4)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                    SoundChannels::MENUS);
        option_mute.toggle();
      }
      else if(option_element_index == 5)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                    SoundChannels::MENUS);

        option_fast_battle.toggle();
      }
    }
    else if(getMainMenuType() == MenuType::INVENTORY && player_inventory)
    {
      bool success = false;

      /* Set to items if there is at least one item */
      if(inventory_title_index == InventoryIndex::ITEMS)
      {
        if(player_inventory->getItemTotalCount() > 0)
          success = true;
      }
      else if(inventory_title_index == InventoryIndex::EQUIPMENT)
      {
        if(player_inventory->getBubbyTotalCount() > 0)
          success = true;
      }
      else if(inventory_title_index == InventoryIndex::BUBBIES)
      {
        if(player_inventory->getEquipTotalCount() > 0)
          success = true;
      }
      else if(inventory_title_index == InventoryIndex::KEY_ITEMS)
      {
        if(player_inventory->getKeyItems().size() > 0)
          success = true;
      }

      /* Indent the Menu */
      if(success)
      {
        event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                    SoundChannels::MENUS);
        layer = MenuLayer::MAIN_INDENT;
        inventory_top_box.setFlag(BoxState::SELECTED);
        inventory_scroll_box.setFlag(BoxState::SELECTED);
      }
    }
    else if(getMainMenuType() == MenuType::SAVE)
    {
      layer = MenuLayer::POPUP;
      save_element_index = 1;
    }
    else if(getMainMenuType() == MenuType::QUIT)
    {
      if(quit_index == QuitIndex::NO)
      {
        quit_index = QuitIndex::NONE;
        layer = MenuLayer::TITLE;
        main_section.status = WindowStatus::HIDING;
      }
      else if(quit_index == QuitIndex::YES)
      {
        setFlag(MenuState::QUITTING, true);
      }
    }
  }
  else if(layer == MenuLayer::POPUP)
  {
    if(getMainMenuType() == MenuType::SAVE)
    {
      layer = MenuLayer::MAIN;
      save_state = MenuSaveState::NONE;

      if(save_element_index == 2)
      {
        std::cout << "Setting to write" << std::endl;
        save_state = MenuSaveState::WRITE;
      }
      else if(save_element_index == 3)
        save_state = MenuSaveState::CLEAR;

      save_element_index = -1;
    }
  }
}

void Menu::keyDownCancel()
{
  if(layer == MenuLayer::MAIN_INDENT)
  {
    if(getMainMenuType() == MenuType::SLEUTH)
    {
      skills_scroll_box.setFlag(BoxState::SELECTED, false);
      skills_top_box.setFlag(BoxState::SELECTED, false);
      skills_name_box.setFlag(BoxState::SELECTED, false);
      layer = MenuLayer::MAIN;
      event_handler->triggerSound(Sound::kID_SOUND_MENU_PREV,
                                  SoundChannels::MENUS);
    }
    else if(getMainMenuType() == MenuType::INVENTORY)
    {
      layer = MenuLayer::MAIN;
      inventory_top_box.setFlag(BoxState::SELECTED, false);
      inventory_scroll_box.setFlag(BoxState::SELECTED, false);
      event_handler->triggerSound(Sound::kID_SOUND_MENU_PREV,
                                  SoundChannels::MENUS);
    }
  }
  else if(layer == MenuLayer::POPUP)
  {
    if(getMainMenuType() == MenuType::SAVE)
    {
      layer = MenuLayer::MAIN;
      save_element_index = -1;
    }
  }
  else if(main_section.status == WindowStatus::ON)
  {
    layer = MenuLayer::TITLE;
    main_section.status = WindowStatus::HIDING;
    event_handler->triggerSound(Sound::kID_SOUND_MENU_PREV,
                                SoundChannels::MENUS);
  }
  else if(title_section.status == WindowStatus::ON &&
          (main_section.status == WindowStatus::OFF ||
           main_section.status == WindowStatus::ON))
  {
    hide();
    event_handler->triggerSound(Sound::kID_SOUND_MENU_PREV,
                                SoundChannels::MENUS);
  }
}

bool Menu::isMainSliding()
{
  return main_section.status == WindowStatus::SHOWING ||
         main_section.status == WindowStatus::HIDING;
}

/* Process key down event */
bool Menu::keyDownEvent(KeyHandler& key_handler)
{
  key_handler.setMode(KeyMode::TEXT_ENTRY);

  if(key_handler.isDepressed(GameKey::MOVE_UP))
  {
    keyDownUp();
    key_handler.setHeld(GameKey::MOVE_UP);
  }
  else if(key_handler.isDepressed(GameKey::MOVE_DOWN))
  {
    keyDownDown();
    key_handler.setHeld(GameKey::MOVE_DOWN);
  }
  else if(key_handler.isDepressed(GameKey::MOVE_LEFT))
  {
    keyDownLeft();
    key_handler.setHeld(GameKey::MOVE_LEFT);
  }
  else if(key_handler.isDepressed(GameKey::MOVE_RIGHT))
  {
    keyDownRight();
    key_handler.setHeld(GameKey::MOVE_RIGHT);
  }
  else if(key_handler.isDepressed(GameKey::ACTION))
  {
    keyDownAction();
    key_handler.setHeld(GameKey::ACTION);
  }
  else if(key_handler.isDepressed(GameKey::CANCEL))
  {
    keyDownCancel();
    key_handler.setHeld(GameKey::CANCEL);
  }

  key_handler.setMode(KeyMode::INPUT);

  return false;
}

/* Show the menu status */
void Menu::show()
{
  if(event_handler)
  {
    event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG,
                                SoundChannels::MENUS);
  }

  clear();

  setFlag(MenuState::SHOWING, true);

  buildTitleElements();
  buildPersonTitleElements();
  buildIconFrames();
  buildTitleSection();

  title_section.status = WindowStatus::SHOWING;

  title_element_index = 0;
  layer = MenuLayer::TITLE;
}

/* Render function to call other render functions */
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

/* Update the Menu Screen */
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

  if(getFlag(MenuState::QUITTING))
  {
    setFlag(MenuState::QUITTING, false);
    setFlag(MenuState::SHOWING, false);

    return true;
  }

  return false;
}

/* Returns the value of a MenuState flag */
bool Menu::getFlag(const MenuState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

MenuLayer Menu::getMenuLayer()
{
  return layer;
}

int32_t Menu::getSaveIndex()
{
  if(getMainMenuType() == MenuType::SAVE)
    return save_scroll_box.getElementIndex();

  return -1;
}

MenuSaveState Menu::getMenuSaveState()
{
  return save_state;
}

/* Assign the BattleDisplayData */
void Menu::setBattleDisplayData(BattleDisplayData* battle_display_data)
{
  this->battle_display_data = battle_display_data;
}

/* Assign configuraiton */
void Menu::setConfig(Options* config)
{
  this->config = config;
}

/* Assign the EventHandler */
void Menu::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;
}

/* Assign a MenuState flag a value */
void Menu::setFlag(MenuState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

/* Assign a new Inventory */
void Menu::setInventory(Inventory* player_inventory)
{
  this->player_inventory = player_inventory;
}

/* Assigns the currently active Map */
void Menu::setMap(Map* new_map)
{
  this->curr_map = new_map;
}

/* Sets the Player Pointer */
void Menu::setPlayer(Player* new_player)
{
  this->curr_player = new_player;
}

/* Assigns the Renderer */
void Menu::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;
}

/* Assigns the save data to the menu */
void Menu::setSaveData(std::vector<Save> saves)
{
  this->save_data = saves;
}

void Menu::setMenuSaveState(MenuSaveState save_state)
{
  this->save_state = save_state;
}

void Menu::updateSaveTitles()
{
  auto old_index = save_scroll_box.getElementIndex();

  buildSave();
  renderSave();

  save_scroll_box.setIndex(old_index);
}