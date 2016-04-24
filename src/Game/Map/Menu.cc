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
 * ANALOGOPTION
 *============================================================================*/

AnalogOption::AnalogOption()
    : val{nullptr}, default_val{0}, num_options{1}, location{Box()}
{
}

AnalogOption::AnalogOption(std::string name) : AnalogOption()
{
  this->name = name;
}

/* Decrease the value */
void AnalogOption::decrease()
{
  if(val && (*val) > 0)
    *val -= 100 / num_options;
}

/* Reset the analog option to a default value */
void AnalogOption::reset()
{
  if(default_val && val)
    *val = default_val;
}

/* Increase the value */
void AnalogOption::increase()
{
  if(val && (*val) < 100)
    *val += 100 / num_options;
}

/*=============================================================================
 * DIGITALOPTION
 *============================================================================*/

/* Constructs a DigitalOption */
DigitalOption::DigitalOption() : config{nullptr}, flag_index{35}
{
}

/* Constructs a DigitalOption */
DigitalOption::DigitalOption(Options* config, Coordinate point, int32_t width,
                             int32_t height, int32_t flag_index,
                             std::string name)
    : DigitalOption()
{
  this->config = config;
  this->flag_index = flag_index;
  this->name = name;

  if(config)
    location = Box{point, width, height};
}

/* Sets the DigitalOption to the Default Value */
void DigitalOption::reset()
{
  if(config)
  {
    config->setFlag(
        static_cast<OptionState>(1 << flag_index),
        config->getDefaultFlag(static_cast<OptionState>(1 << flag_index)));
  }
}

/* Sets the DigitalOption Flag */
void DigitalOption::set()
{
  if(config)
    config->setFlag(static_cast<OptionState>(1 << flag_index), true);
}

/* Toggles the DigitalOption Flag */
void DigitalOption::toggle()
{
  if(config)
  {
    config->setFlag(
        static_cast<OptionState>(1 << flag_index),
        !config->getFlag(static_cast<OptionState>(1 << flag_index)));
  }
}

/* Unsets the DigitalOption Flag */
void DigitalOption::unset()
{
  if(config)
    config->setFlag(static_cast<OptionState>(1 << flag_index), false);
}

bool DigitalOption::isSet()
{
  if(config)
  {
    return config->getFlag(static_cast<OptionState>(1 << flag_index));
  }

  return false;
}

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/* Title Section */
const uint8_t Menu::kTITLE_ALPHA{255};
const float Menu::kTITLE_HEIGHT{0.77};
const float Menu::kTITLE_WIDTH{0.17};
const float Menu::kTITLE_X_OFFSET{0.02};
const float Menu::kTITLE_ELEMENT_X_OFFSET{0.01};
const float Menu::kTITLE_Y_OFFSET{0.05};
const float Menu::kTITLE_ELEMENT_GAP{0.80};
const float Menu::kTITLE_CORNER_LENGTH{0.02};
const float Menu::kTITLE_SLIDE_RATE{0.60};
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
const float Menu::kMAIN_SLIDE_RATE{2.05};
const float Menu::kMAIN_CORNER_LENGTH{0.025};
const float Menu::kINV_WIDTH{0.53};
const float Menu::kOPTIONS_WIDTH{0.40};
const float Menu::kQUIT_WIDTH{0.33};
const float Menu::kSAVE_WIDTH{0.55};
const float Menu::kSLEUTH_WIDTH{0.65};

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
const uint32_t Menu::kNUM_OPTIONS{4};
const float Menu::kOPTIONS_X{0.025};
const float Menu::kOPTIONS_Y{0.05};
const float Menu::kOPTIONS_Y_BAR_GAP{0.04};
const float Menu::kOPTIONS_Y_GAP{0.075};
const float Menu::kOPTIONS_DIGITAL_TEXT_GAP{0.03};
const float Menu::kOPTIONS_BOX_SIZE{0.017};

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
      frame_bubbies{nullptr},
      frame_checkbox{nullptr},
      frame_equipment{nullptr},
      frame_footsteps{nullptr},
      frame_items{nullptr},
      frame_key_items{nullptr},
      frame_location{nullptr},
      frame_money{nullptr},
      layer{MenuLayer::INVALID},
      player_inventory{nullptr},
      renderer{nullptr},
      title_elements{},
      sleuth_element_index{-1},
      title_element_index{-1},
      option_element_index{-1},
      inventory_title_index{InventoryIndex::NONE},
      inventory_element_index{-1}
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
  }
}

void Menu::buildSleuthScreen()
{
  sleuth_faces.clear();
  sleuth_stat_frames.clear();

  if(config && curr_player && curr_player->getSleuth())
  {
    if(curr_player && curr_player->getSleuth())
    {
      /* Construct the face graphic sprites */
      auto party = curr_player->getSleuth();
      auto members = party->getMembers();

      for(const auto& member : party->getMembers())
      {
        if(member->getFaceSpritePath() != "")
        {
          // TODO: Create face sprite from path. Render face sprite.
        }
      }
    }
  }
}

void Menu::buildSleuthStats()
{
}

/* Constructs the Inventory Screen */
void Menu::buildIconTitles(int32_t number)
{
  titles.clear();

  if(config)
  {
    int32_t title_box_size{main_section.location.height / 6};

    /* Construct the Title Icon Boxes */
    Box icon_box{Coordinate{0, 0}, title_box_size, title_box_size};
    icon_box.color_bg = kCOLOR_ICON_UNSELECTED_FILL;
    icon_box.color_bg_selected = kCOLOR_INVENTORY_ICON_FILL;
    icon_box.color_border = kCOLOR_BORDER_UNSELECTED;
    icon_box.color_border_selected = kCOLOR_TITLE_BORDER;

    for(int32_t i = 0; i < number; i++)
      titles.push_back(icon_box);
  }
}

void Menu::buildInventoryElements()
{
  if(config)
  {
    // auto width = config->getScreenWidth();
    // auto height = config->getScreenHeight();

    inventory_details.clear();
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

/* Construct the Item Frames -- Scroll Box and Details */
void Menu::buildInventoryItems()
{
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
    auto font_item = config->getFontTTF(FontName::MENU_HEADER);
    auto item_inset = (uint32_t)std::round(width * kINV_ITEM_ELEMENT_INSET);

    Text t_item_name{font_item};
    Text t_item_count{font_item};

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

SDL_Texture* Menu::buildItemDetailFrame(Item* build_item, uint32_t width,
                                        uint32_t height)
{
  (void)build_item;
  (void)width;
  (void)height;
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
        std::ceil(config->getScreenWidth() * kMAIN_CORNER_LENGTH));
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

  frame_money = nullptr;
  frame_location = nullptr;
  frame_footsteps = nullptr;
  frame_bubbies = nullptr;
  frame_checkbox = nullptr;
  frame_equipment = nullptr;
  frame_items = nullptr;
  frame_key_items = nullptr;
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
    option_audio_level.location.color_border_selected = {255, 255, 255, 255};
    option_audio_level.location.color_border = {255, 255, 255, 65};
  }
}

/* Constructs the Main Section as Required */
void Menu::buildMainSection(MenuType menu_type)
{
  if(config && renderer)
  {
    auto width = config->getScreenWidth();
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

      /* Construct the face graphics and other sprites */
      buildSleuthScreen();

      sleuth_element_index = 0;
      selectSleuthIndex();
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
  }
}

/* Constructs TitleElements on the Title Menu Section */
void Menu::buildTitleElements()
{
  title_elements.clear();

  /* Party TitleElement */
  title_elements.push_back(TitleElement("Sleuth", true, MenuType::SLEUTH));

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

/* Constructs the TitleSection */
void Menu::buildTitleSection()
{
  auto height = config->getScreenHeight();
  auto width = config->getScreenWidth();

  auto title_height = (uint32_t)std::round(height * kTITLE_HEIGHT);
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
  selectInventoryIndex();
}

// TODO: abtract out BETA [04-9-16]
void Menu::decrementOptionIndex()
{
  unselectOptionIndex();
  option_element_index--;
  selectOptionIndex();
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
  selectInventoryIndex();
}

void Menu::incrementOptionIndex()
{
  unselectOptionIndex();
  option_element_index++;
  selectOptionIndex();
}

void Menu::incrementSleuthIndex()
{
  unselectSleuthIndex();
  sleuth_element_index++;
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
    option_auto_run.location.setFlag(BoxState::SELECTED);
  else if(option_element_index == 3)
    option_mute.location.setFlag(BoxState::SELECTED);
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
    option_auto_run.location.setFlag(BoxState::SELECTED, false);
  else if(option_element_index == 3)
    option_mute.location.setFlag(BoxState::SELECTED, false);
}

void Menu::unselectSleuthIndex()
{
  if(sleuth_element_index < (int32_t)titles.size() && sleuth_element_index > -1)
    titles.at(sleuth_element_index).setFlag(BoxState::SELECTED, false);
}

void Menu::selectSleuthIndex()
{
  if(sleuth_element_index < (int32_t)titles.size() && sleuth_element_index >= 0)
    titles.at(sleuth_element_index).setFlag(BoxState::SELECTED, true);
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
  auto font_main_title = config->getFontTTF(FontName::MENU_MAIN_TITLE);
  auto font_title_element = config->getFontTTF(FontName::MENU_TITLE_ELEMENT);
  auto font_header = config->getFontTTF(FontName::MENU_HEADER);

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

  /* Anti-Aliased Top Line */
  Frame::setRenderDrawColor(renderer, {255, 255, 255, 110});
  Frame::drawLine(corner_aa_top, renderer);

  /* Anti-Aliased Bot Line */
  Frame::setRenderDrawColor(renderer, {255, 255, 255, 65});
  Frame::drawLine(corner_aa_bot, renderer);

  auto x_offset = (int32_t)std::round(width * kTITLE_X_OFFSET);
  auto y_offset = (int32_t)std::round(height * kTITLE_Y_OFFSET);

  Text map_name(font_header);
  Text footsteps(font_header);
  Text credits(font_header);
  Text t_main_title(font_main_title);
  Text t_title_element(font_title_element);

  t_main_title.setText(renderer, "MENU", kCOLOR_TEXT);

  int32_t y_gap = (int32_t)std::round(config->getScreenHeight() * 0.06);
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

    Coordinate top_left{point.x, rect_y};
    Coordinate top_right{point.x + title_width, rect_y};
    Coordinate bot_left{point.x, rect_y + rect_h};
    Coordinate bot_right{point.x + title_width, rect_y + rect_h};
    auto top_line = Helpers::bresenhamPoints(top_left, top_right);
    auto bot_line = Helpers::bresenhamPoints(bot_left, bot_right);

    SDL_Rect rect;
    rect.x = point.x + title_width * kTITLE_HOVER_OFFSET_X;
    rect.y = rect_y;
    rect.h = rect_h;
    rect.w = title_width * kTITLE_HOVER_WIDTH;

    auto brightness = Helpers::updateHoverBrightness(
        title_elements.at(title_element_index).hover_time, kTITLE_HOVER_RATE,
        kTITLE_HOVER_MIN, kTITLE_HOVER_MAX);

    auto hover_color = kCOLOR_TITLE_HOVER;
    SDL_SetRenderDrawColor(renderer, hover_color.r, hover_color.g,
                           hover_color.b, hover_color.a * brightness);
    SDL_RenderFillRect(renderer, &rect);
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
    auto bot_y_offset = (int32_t)std::round(height * kTITLE_LOCATION_Y_OFFSET);
    auto icons_y_gap = (int32_t)std::round(height * kTITLE_ICONS_Y_GAP);
    auto tx = (int32_t)std::round(width * kTITLE_ICON_TEXT_X);
    auto ty = (int32_t)std::round(height * kTITLE_ICON_TEXT_Y);
    auto curr_y = bot_y_offset + icons_y_gap;

    if(frame_location)
    {
      map_name.setText(renderer, curr_map->getName(), kCOLOR_TEXT);
      frame_location->render(renderer, point.x + x_offset, curr_y);
      map_name.render(renderer, point.x + x_offset + tx, curr_y + ty);
    }
    if(frame_footsteps)
    {
      curr_y += icons_y_gap;
      footsteps.setText(renderer, Helpers::formatUInt(curr_player->getSteps()),
                        kCOLOR_TEXT);
      frame_footsteps->render(renderer, point.x + x_offset, curr_y);
      footsteps.render(renderer, point.x + x_offset + tx, curr_y + ty);
    }
    if(frame_money)
    {
      curr_y += icons_y_gap;
      credits.setText(renderer, Helpers::formatUInt(curr_player->getCredits()),
                      kCOLOR_TEXT);
      frame_money->render(renderer, point.x + x_offset, curr_y);
      credits.render(renderer, point.x + x_offset + tx, curr_y + ty);
    }
  }

  t_main_title.render(renderer, point.x + x_offset, point.y + y_offset);
}

/* Render the Main Section */
void Menu::renderMainSection()
{
  if((uint32_t)title_element_index < title_elements.size() && config &&
     renderer)
  {
    auto menu_type = getMainMenuType();
    auto location = main_section.location;
    auto point = main_section.location.point;
    auto corner_inset = calcMainCornerInset();

    /* Render the frame outline and backdrop */
    Coordinate tl = {point.x, point.y};
    Coordinate tr = {point.x + location.width - corner_inset, point.y};
    Coordinate bl = {point.x, point.y + location.height};
    Coordinate br = {point.x + location.width - corner_inset,
                     point.y + location.height};

    Coordinate blc = {br.x + 1, br.y - 1};
    Coordinate tlc = {tr.x + 1, tl.y};
    Coordinate trc = {tr.x + corner_inset, tr.y};
    Coordinate brc = {br.x + corner_inset, br.y - corner_inset};

    Coordinate atl = {blc.x - 1, blc.y};
    Coordinate abl = {blc.x, blc.y + 1};
    Coordinate atr = {brc.x - 1, brc.y};
    Coordinate abr = {brc.x, brc.y + 1};

    auto top_bar = Helpers::bresenhamPoints(tl, tr);
    auto bot_bar = Helpers::bresenhamPoints(bl, br);
    auto top_corner = Helpers::bresenhamPoints(tlc, trc);
    auto bot_corner = Helpers::bresenhamPoints(blc, brc);
    auto right_line = Helpers::bresenhamPoints(trc, brc);
    auto corner_aa_top = Helpers::bresenhamPoints(atl, atr);
    auto corner_aa_bot = Helpers::bresenhamPoints(abl, abr);

    Frame::setRenderDrawColor(renderer, {0, 0, 0, main_section.alpha});
    Frame::renderFillLineToLine(top_bar, bot_bar, renderer, true);
    Frame::renderFillLineToLine(top_corner, bot_corner, renderer, true);

    Frame::setRenderDrawColor(renderer, kCOLOR_MAIN_BORDER);
    Frame::drawLine(top_bar, renderer);
    Frame::drawLine(bot_bar, renderer);
    Frame::drawLine(top_corner, renderer);
    Frame::drawLine(bot_corner, renderer);
    Frame::drawLine(right_line, renderer);

    /* Anti-Aliased Top Line */
    Frame::setRenderDrawColor(renderer, {255, 255, 255, 45});
    Frame::drawLine(corner_aa_top, renderer);

    /* Anti-Aliased Bot Line */
    Frame::setRenderDrawColor(renderer, {255, 255, 255, 80});
    Frame::drawLine(corner_aa_bot, renderer);

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
  auto title_font = config->getFontTTF(FontName::MENU_TITLE_ELEMENT);
  auto font_header = config->getFontTTF(FontName::MENU_HEADER);
  auto font_subheader = config->getFontTTF(FontName::MENU_SUBHEADER);
  auto inv = player_inventory;
  auto width = config->getScreenWidth();
  auto start = main_section.location.point;
  auto gap = (int32_t)std::round(width * kINV_GAP);
  auto curr_y = start.y + gap;

  /* Render the Title Boxes */
  for(uint32_t i = 0; i < titles.size(); i++)
  {
    auto& icon = titles.at(i);

    icon.point.x = start.x + gap;
    icon.point.y = curr_y;
    icon.render(renderer);
    curr_y += gap + icon.height;

    if(i == 0)
    {
      frame_items->render(
          renderer, icon.point.x + icon.width / 2 - frame_items->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_items->getHeight() / 2);
    }
    else if(i == 1)
    {
      frame_equipment->render(
          renderer,
          icon.point.x + icon.width / 2 - frame_equipment->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_equipment->getHeight() / 2);
    }
    else if(i == 2)
    {
      frame_bubbies->render(
          renderer,
          icon.point.x + icon.width / 2 - frame_bubbies->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_bubbies->getHeight() / 2);
    }
    else if(i == 3)
    {
      frame_key_items->render(
          renderer,
          icon.point.x + icon.width / 2 - frame_key_items->getWidth() / 2,
          icon.point.y + icon.height / 2 - frame_key_items->getHeight() / 2);
    }
  }

  auto icon_w = main_section.location.height / 6;
  curr_y += gap;

  /* Render the Mass of the Inventory */
  Text mass_title(font_subheader);
  Text mass_value(font_header);
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

  mass_title.render(
      renderer, start.x + gap + icon_w / 2 - mass_title.getWidth() / 2, curr_y);
  mass_value.render(renderer,
                    start.x + gap + icon_w / 2 - mass_value.getWidth() / 2,
                    curr_y + icon_w / 3);

  // TODO: Render the selection triangle [04-13-16]

  /* Render the Top Boxes and the Bottom Boxes */
  auto top_section_height = (main_section.location.height - 3 * gap) / 2;

  inventory_top_box.point = {start.x + 2 * gap + icon_w, start.y + gap};
  inventory_top_box.width = main_section.location.width - icon_w - 3 * gap;
  inventory_top_box.height = top_section_height / 5;
  inventory_top_box.color_bg = kCOLOR_TITLE_BG;
  inventory_top_box.color_bg_selected = kCOLOR_TITLE_BG;
  inventory_top_box.color_border = kCOLOR_BORDER_UNSELECTED;
  inventory_top_box.color_border_selected = kCOLOR_TITLE_BORDER;
  inventory_top_box.render(renderer);

  /* Render the scroll inventory box */
  inventory_scroll_box.point = {inventory_top_box.point.x,
                                inventory_top_box.point.y +
                                    inventory_top_box.height - 1};
  inventory_scroll_box.width = inventory_top_box.width;
  inventory_scroll_box.height = top_section_height - inventory_top_box.height;
  inventory_scroll_box.color_bg = kCOLOR_TITLE_BG;
  inventory_scroll_box.color_bg_selected = kCOLOR_TITLE_BG;
  inventory_scroll_box.color_border = kCOLOR_BORDER_UNSELECTED;
  inventory_scroll_box.color_border_selected = kCOLOR_TITLE_BORDER;
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
  auto bot_height = main_section.location.height - top_section_height - 3 * gap;
  auto corner_inset = calcMainCornerInset();

  Coordinate tl = {inventory_top_box.point.x,
                   inventory_top_box.point.y + inventory_top_box.height +
                       inventory_scroll_box.height + gap};
  Coordinate tr = {tl.x + inventory_top_box.width - corner_inset, tl.y};
  Coordinate bl = {tl.x, tl.y + bot_height};
  Coordinate br = {tl.x + inventory_top_box.width - corner_inset,
                   tl.y + bot_height};

  Coordinate blc = {br.x + 1, br.y - 1};
  Coordinate tlc = {tr.x + 1, tl.y};
  Coordinate trc = {tr.x + corner_inset, tr.y};
  Coordinate brc = {br.x + corner_inset, br.y - corner_inset};

  auto left_line = Helpers::bresenhamPoints(tl, bl);
  auto top_bar = Helpers::bresenhamPoints(tl, tr);
  auto bot_bar = Helpers::bresenhamPoints(bl, br);
  auto top_corner = Helpers::bresenhamPoints(tlc, trc);
  auto bot_corner = Helpers::bresenhamPoints(blc, brc);
  auto right_line = Helpers::bresenhamPoints(trc, brc);

  Frame::setRenderDrawColor(renderer, {0, 0, 0, 255});
  Frame::renderFillLineToLine(top_bar, bot_bar, renderer, true);
  Frame::renderFillLineToLine(top_corner, bot_corner, renderer, true);

  Frame::setRenderDrawColor(renderer, kCOLOR_BORDER_UNSELECTED);
  Frame::drawLine(left_line, renderer);
  Frame::drawLine(top_bar, renderer);
  Frame::drawLine(bot_bar, renderer);
  Frame::drawLine(top_corner, renderer);
  Frame::drawLine(bot_corner, renderer);
  Frame::drawLine(right_line, renderer);

  /* Render the Item Title Text */
  Text title_text(title_font);
  Text number_items(title_font);
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

    title_text.setText(renderer, "Equipment", kCOLOR_TEXT);
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
    renderItem(tl, icon_w, gap, bot_height);
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
      auto font_options = config->getFontTTF(FontName::MENU_OPTIONS);
      auto font_title = config->getFontTTF(FontName::MENU_ITEM_HEADER);
      auto font_standard = config->getFontTTF(FontName::MENU_STANDARD);
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

      Text item_name(font_options);
      Text item_mass_title(font_title);
      Text item_mass(font_options);
      Text item_value_title(font_title);
      Text item_value(font_options);
      Text item_description(font_standard);

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

      auto curr_y = inventory_icon_box.point.y + item_name.getHeight() + gap;
      item_mass_title.render(renderer, curr_x, curr_y);

      item_value_title.render(renderer, mass_x, curr_y);
      curr_y += item_mass_title.getHeight() + gap / 4;
      item_mass.render(renderer, curr_x, curr_y);
      item_value.render(renderer, mass_x, curr_y);

      curr_y = inventory_icon_box.point.y + inventory_icon_box.height + gap;
      auto desc_split = Text::splitLine(font_standard, item->getDescription(),
                                        inventory_top_box.width - 2 * gap);

      /* Render the description lines */
      for(auto& line : desc_split)
      {
        item_description.setText(renderer, line, kCOLOR_TEXT);
        item_description.render(renderer, start.x + gap, curr_y);
        curr_y += item_description.getHeight() * 1.1;
      }

      Text item_flags(font_standard);
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
  auto width = config->getScreenWidth();
  auto height = config->getScreenHeight();

  auto start_x =
      main_section.location.point.x + (uint32_t)std::round(kOPTIONS_X * width);
  auto start_y =
      main_section.location.point.y + (uint32_t)std::round(kOPTIONS_Y * height);

  auto curr_x = start_x;
  auto curr_y = start_y;

  auto y_gap = (uint32_t)std::round(kOPTIONS_Y_GAP * height);
  // auto digital_gap = (uint32_t)std::round(kOPTIONS_DIGITAL_GAP * width);

  /* Render the audio level */
  auto end = renderOptionAnalog(option_audio_level, {curr_x, curr_y});

  /* Render the music level */
  curr_y = end.y + y_gap; // todo
  end = renderOptionAnalog(option_music_level, {curr_x, curr_y});

  /* Render the auto run flag */
  curr_y = end.y + y_gap; // todo
  end = renderOptionDigital(option_auto_run, {curr_x, curr_y});

  /* Render the mute flag */
  curr_y = end.y + y_gap;
  // curr_x = end.x + digital_gap; // todo
  renderOptionDigital(option_mute, {curr_x, curr_y});
}

/* Render a given option at a given point */
UCoordinate Menu::renderOptionAnalog(AnalogOption& option, UCoordinate point)
{
  UCoordinate end{0, 0};

  if(renderer && config && option.val)
  {
    auto width = config->getScreenWidth();
    auto height = config->getScreenHeight();
    auto bar_gap = (uint32_t)std::round(kOPTIONS_Y_BAR_GAP * height);
    auto box_size = (int32_t)std::round(kOPTIONS_BOX_SIZE * width);
    auto start_x = (int32_t)point.x;

    /* Render the Option Name Text */
    auto option_font = config->getFontTTF(FontName::MENU_HEADER);
    SDL_Color fill_color = kCOLOR_OPTION_FILL;
    SDL_Color border_color = {255, 255, 255, 65};

    if(option.location.getFlag(BoxState::SELECTED))
    {
      border_color = kCOLOR_MAIN_BORDER;
      fill_color = kCOLOR_OPTION_FILL_SELECTED;
    }

    /* Create and render the option name text */
    Text t(option_font);
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
    auto width = config->getScreenWidth();
    auto text_gap = (uint32_t)std::round(kOPTIONS_DIGITAL_TEXT_GAP * width);
    auto box_size = (int32_t)std::round(kOPTIONS_BOX_SIZE * width);
    auto start_x = (int32_t)point.x;
    auto option_font = config->getFontTTF(FontName::MENU_HEADER);
    SDL_Color box_border = {255, 255, 255, 65};

    if(option.location.getFlag(BoxState::SELECTED))
      box_border = kCOLOR_MAIN_BORDER;

    /* Render the Box */
    Box digital_box{{start_x, (int32_t)point.y}, box_size, box_size};
    digital_box.color_border = box_border;

    /* Render the Check if flag is set */
    if(option.isSet())
      digital_box.color_bg = kCOLOR_OPTION_FILL; // TODO

    digital_box.render(renderer);

    // frame_checkbox->render(renderer, point.x, point.y, box_size, box_size);

    /* Render the Digital Option Name */
    Text t(option_font);
    t.setText(renderer, option.name, kCOLOR_TEXT);
    t.render(renderer, point.x + text_gap, point.y);

    end.y = point.y + t.getHeight();
  }

  return end;
}

/* Renders the Sleuth Screen */
void Menu::renderSleuth()
{
  // auto title_font = config->getFontTTF(FontName::MENU_TITLE_ELEMENT);
  /// auto font_header = config->getFontTTF(FontName::MENU_HEADER);
  //// auto font_subheader = config->getFontTTF(FontName::MENU_SUBHEADER);

  auto width = config->getScreenWidth();
  auto start = main_section.location.point;
  auto gap = (int32_t)std::round(width * kINV_GAP);

  auto curr_y = start.y + gap;

  /* Render the Sleuth Members Face Boxes */
  for(uint32_t i = 0; i < titles.size(); i++)
  {
    auto& icon = titles.at(i);

    icon.point.x = start.x + gap;
    icon.point.y = curr_y;
    icon.render(renderer);
    curr_y += gap + icon.height;

    // TODO: Render the face graphics
  }

  /* */

  // Render the sleuth top box with name

  // Render the sleuth sprite box

  // Render the sleuth top stats box

  // Render the sleuth bot stats box

  // Render the sleuth info box

  // Render the sleuth equip boxes

  // Render the sleuth equip frames
}

/* Renders the Save Screen */
void Menu::renderSave()
{
}

/* Renders the Quit Screen */
void Menu::renderQuit()
{
}

Person* Menu::getCurrentPerson()
{
  if(curr_player && curr_player->getSleuth())
  {
    auto members = curr_player->getSleuth()->getMembers();

    if(sleuth_element_index > 0 &&
       sleuth_element_index < (int32_t)members.size())
      return members.at(sleuth_element_index);
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

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Clear the menu, revert everything off */
void Menu::clear()
{
  main_section.status = WindowStatus::OFF;
  title_section.status = WindowStatus::OFF;

  clearIconFrames();
  setFlag(MenuState::SHOWING, false);
}

/* Hides the menu */
void Menu::hide()
{
  title_section.status = WindowStatus::HIDING;
  title_element_index = -1;
  layer = MenuLayer::INVALID;
}

void Menu::keyDownUp()
{
  if(layer == MenuLayer::TITLE)
  {
    if(title_element_index > 0)
      title_element_index--;
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
    else if(getMainMenuType() == MenuType::SLEUTH)
    {
      if((uint32_t)sleuth_element_index > 1)
      {
        decrementSleuthIndex();
      }
    }
  }
  else if(layer == MenuLayer::MAIN_INDENT)
  {
    if(getMainMenuType() == MenuType::INVENTORY)
    {
      if(inventory_scroll_box.prevIndex())
        inventory_element_index--;
    }
  }
}

void Menu::keyDownDown()
{
  if(layer == MenuLayer::TITLE)
  {
    if(title_element_index + 1 < (int32_t)title_elements.size())
      title_element_index++;
    else
      title_element_index = 0;
  }
  else if(layer == MenuLayer::MAIN)
  {
    if(getMainMenuType() == MenuType::OPTIONS)
    {
      if((uint32_t)option_element_index + 1 < kNUM_OPTIONS)
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
    else if(getMainMenuType() == MenuType::SLEUTH)
    {
      if(canIncrementSleuth())
      {
        incrementSleuthIndex();
        buildSleuthStats();
      }
    }
  }
  else if(layer == MenuLayer::MAIN_INDENT)
  {
    if(getMainMenuType() == MenuType::INVENTORY)
    {
      if(inventory_scroll_box.nextIndex())
        inventory_element_index++;
    }
  }
}

void Menu::keyDownLeft()
{
  if(main_section.status == WindowStatus::ON)
  {
    if(getMainMenuType() == MenuType::OPTIONS)
    {
      if(option_element_index == 0)
        option_audio_level.decrease();
      else if(option_element_index == 1)
        option_music_level.decrease();
    }
  }
}

void Menu::keyDownRight()
{
  if(main_section.status == WindowStatus::ON)
  {
    if(getMainMenuType() == MenuType::OPTIONS)
    {
      if(option_element_index == 0)
        option_audio_level.increase();
      else if(option_element_index == 1)
        option_music_level.increase();
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
    }
    /* Key down event on the options. --> If digital option, select flag */
    else if(getMainMenuType() == MenuType::OPTIONS)
    {
      if(option_element_index == 2)
        option_auto_run.toggle();
      else if(option_element_index == 3)
        option_mute.toggle();
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
        inventory_element_index = 0;
        layer = MenuLayer::MAIN_INDENT;
        inventory_top_box.setFlag(BoxState::SELECTED);
        inventory_scroll_box.setFlag(BoxState::SELECTED);
      }
    }
  }
}

void Menu::keyDownCancel()
{
  if(layer == MenuLayer::MAIN_INDENT)
  {
    if(getMainMenuType() == MenuType::INVENTORY)
    {
      inventory_element_index = -1;
      layer = MenuLayer::MAIN;
      inventory_top_box.setFlag(BoxState::SELECTED, false);
      inventory_scroll_box.setFlag(BoxState::SELECTED, false);
    }
  }
  else if(main_section.status == WindowStatus::ON)
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

/* Process key down event */
bool Menu::keyDownEvent()
{
  if(event_handler)
  {
    auto& key_handler = event_handler->getKeyHandler();
    key_handler.update(0);

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
  }

  return false;
}

/* Show the menu status */
void Menu::show()
{
  clear();

  setFlag(MenuState::SHOWING, true);

  buildTitleElements();
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

  return false;
}

/* Returns the value of a MenuState flag */
bool Menu::getFlag(const MenuState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
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
