/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: February 23rd, 2014
* Date Redesigned: September 14th, 2015
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
#include "Game/Battle/BattleMenu.h"

/*=============================================================================
 * CONSTANTS - Public (for use in Battle)
 *============================================================================*/

/*=============================================================================
 * CONSTANTS - Private
 *============================================================================*/
const uint16_t BattleMenu::kBIGBAR_CHOOSE{100};
const uint16_t BattleMenu::kALLY_HEIGHT{70};

const float BattleMenu::kBIGBAR_L{0.2};
const float BattleMenu::kBIGBAR_M1{0.1};
const float BattleMenu::kBIGBAR_M2{0.3};
const uint16_t BattleMenu::kBIGBAR_OFFSET{88};
const uint16_t BattleMenu::kBIGBAR_R_OFFSET{25};

const uint8_t BattleMenu::kMENU_SEPARATOR_B{8};
const uint8_t BattleMenu::kMENU_SEPARATOR_T{12};

const uint8_t BattleMenu::kSCROLL_R{2};

const uint8_t BattleMenu::kSKILL_BORDER{10};
const uint8_t BattleMenu::kSKILL_BORDER_WIDTH{1};
const uint8_t BattleMenu::kSKILL_DESC_GAP{10};
const uint8_t BattleMenu::kSKILL_DESC_LINES{4};
const uint8_t BattleMenu::kSKILL_DESC_SEP{4};
const uint8_t BattleMenu::kSKILL_FRAME_S{32};
const uint8_t BattleMenu::kSKILL_FRAME_L{64};
const uint8_t BattleMenu::kSKILL_QD_GAP{15};
const uint8_t BattleMenu::kSKILL_SEP{5};
const uint8_t BattleMenu::kSKILL_SUCCESS{20};
const uint8_t BattleMenu::kSKILL_TIME_GAP{18};

const uint8_t BattleMenu::kTYPE_MARGIN{7};
const uint8_t BattleMenu::kTYPE_MAX{5};
const uint8_t BattleMenu::kTYPE_SELECT{3};

const uint16_t BattleMenu::kINFO_W{180};

const std::vector<int32_t> BattleMenu::kINDEX_ORDER{-5, -4, -3, -1, -2,
                                                    2,  1,  3,  4,  5};
/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: BattleMenu general constructor. Constructs a new BattleMenu.
 *
 * Inputs: none
 */
BattleMenu::BattleMenu()
    : actor{nullptr},
      battle_display_data{nullptr},
      config{nullptr},
      frame_qd{Frame()},
      flags{static_cast<BattleMenuState>(0)},
      menu_layer{BattleMenuLayer::ZEROTH_LAYER},
      renderer{nullptr},
      selected_action_scope{ActionScope::NO_SCOPE},
      selected_action_type{ActionType::NONE},
      status_window{WindowStatus::HIDING},
      element_index{-1}
{
}

/*=============================================================================
 * PRIVATE FUNCTIONS - Operation
 *============================================================================*/

std::vector<BattleActor*> BattleMenu::getSelectableTargets()
{
  std::vector<BattleActor*> selectable_targets;

  if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  {
    if(selected_action_type == ActionType::SKILL)
    {
      if((uint32_t)element_index < valid_battle_skills.size())
        if(valid_battle_skills.at(element_index))
          selectable_targets = valid_battle_skills.at(element_index)->targets;
    }
    else if(selected_action_type == ActionType::ITEM)
    {
      if((uint32_t)element_index < valid_battle_items.size())
        if(valid_battle_items.at(element_index))
          selectable_targets = valid_battle_items.at(element_index)->targets;
    }
  }

  return selectable_targets;
}

/* kINDEX_ORDER {-5, -4, -3, -1, -2, 2, 1, 3, 4, 5} */
BattleActor* BattleMenu::getMostLeft()
{
  assert(actor);

  BattleActor* most_left = nullptr;
  auto targets = getSelectableTargets();

  /* If the actor is an Enemy, find the most left negative index which is among
   * a valid target in the targets vector, otherwise for allies */
  for(auto ordered : kINDEX_ORDER)
  {
    for(auto target : targets)
    {
      assert(target);
      if(actor->getIndex() < 0)
      {
        if(target->getIndex() < 0 && target->getIndex() == ordered)
          most_left = target;
      }
      if(actor->getIndex() > 0)
      {
        if(target->getIndex() < 0 && target->getIndex() == ordered)
          most_left = target;
      }
    }
  }

  return most_left;
}

BattleActor* BattleMenu::getMostRight()
{
  assert(actor);

  BattleActor* most_right = nullptr;

  std::vector<int32_t> reversed = kINDEX_ORDER;
  std::reverse(begin(reversed), end(reversed));
  auto targets = getSelectableTargets();

  for(auto ordered : reversed)
  {
    for(auto target : targets)
    {
      assert(target);
      if(actor->getIndex() < 0)
      {
        if(target->getIndex() < 0 && target->getIndex() == ordered)
          most_right = target;
      }
      if(actor->getIndex() > 0)
      {
        if(target->getIndex() < 0 && target->getIndex() == ordered)
          most_right = target;
      }
    }
  }

  return most_right;
}

/*=============================================================================
 * PRIVATE FUNCTIONS - Display
 *============================================================================*/

void BattleMenu::clearSkillFrames()
{
  for(auto& skill_frame : frames_skill_info)
  {
    if(skill_frame)
      delete skill_frame;
    skill_frame = nullptr;
  }

  for(auto& name_frame : frames_skill_name)
  {
    if(name_frame)
      delete name_frame;
    name_frame = nullptr;
  }

  frames_skill_info.clear();
  frames_skill_name.clear();
}

SDL_Texture* BattleMenu::createSkillFrame(BattleSkill* battle_skill,
                                          uint32_t width, uint32_t height)
{
  /* Grab the skill pointer */
  auto skill = battle_skill->skill;

  /* Fonts */
  auto font_header = config->getFontTTF(FontName::BATTLE_HEADER);
  auto font_subheader = config->getFontTTF(FontName::BATTLE_SUBHEADER);

  /* Create rendering texture */
  SDL_Texture* texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  /* Render the skill box */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect rect_top;
  rect_top.x = kSKILL_BORDER;
  rect_top.y = kSKILL_BORDER;
  rect_top.w = kSKILL_FRAME_L;
  rect_top.h = kSKILL_FRAME_L;
  if(skill->getThumbnail() != nullptr)
    skill->getThumbnail()->render(renderer, rect_top.x, rect_top.y);
  Frame::renderRect(rect_top, kSKILL_BORDER_WIDTH, renderer, true);

  /* Render the action scope */
  Frame frame_scope = battle_display_data->getFrameScope(skill->getScope());
  Frame* scope_frame = &frame_scope;

  SDL_Rect rect_bot;
  rect_bot.x = kSKILL_BORDER;
  rect_bot.h = kSKILL_FRAME_S;
  rect_bot.y = height - kSKILL_BORDER - rect_bot.h;
  rect_bot.w = kSKILL_FRAME_L;
  if(scope_frame != nullptr)
    scope_frame->render(renderer, rect_bot.x, rect_bot.y);
  // Frame::renderRect(rect_bot, kSKILL_BORDER_WIDTH, renderer, true);

  /* Render the primary element */
  Frame frame_primary =
      battle_display_data->getFrameElement(skill->getPrimary());
  Frame* primary_frame = &frame_primary;

  rect_bot.x += rect_bot.w + kSKILL_BORDER;
  rect_bot.w = kSKILL_FRAME_S;
  if(primary_frame != nullptr)
    primary_frame->render(renderer, rect_bot.x, rect_bot.y);
  // Frame::renderRect(rect_bot, kSKILL_BORDER_WIDTH, renderer, true);

  /* Render the secondary element */
  Frame frame_secondary =
      battle_display_data->getFrameElement(skill->getSecondary());
  Frame* secondary_frame = &frame_secondary;
  rect_bot.x += rect_bot.w + kSKILL_BORDER;
  if(secondary_frame != nullptr)
  {
    secondary_frame->render(renderer, rect_bot.x, rect_bot.y);
    // Frame::renderRect(rect_bot, kSKILL_BORDER_WIDTH, renderer, true);
  }

  /* Render the cost */
  SDL_Color color = {255, 255, 255, 255};
  uint16_t qd_x = width - kSKILL_BORDER - frame_qd.getWidth();
  uint16_t qd_y = kSKILL_QD_GAP;
  frame_qd.render(renderer, qd_x, qd_y);

  Text t1(font_header);
  Text t2(font_subheader);

  t1.setText(renderer, std::to_string(skill->getCost()), color);
  qd_x -= t1.getWidth() + kSKILL_SEP;
  t1.render(renderer, qd_x, qd_y - 1);

  /* Render the name */
  t1.setText(renderer, skill->getName(), color);
  uint16_t text_x = rect_top.x + rect_top.w + kSKILL_BORDER;
  uint16_t text_y = qd_y - 1;
  t1.render(renderer, text_x, text_y);

  /* Render the cooldown */
  auto frame_time = battle_display_data->getFrameTime();
  auto frame_percent = battle_display_data->getFramePercent();

  if(frame_time && frame_percent)
  {
    uint16_t time_x = width - kSKILL_BORDER - frame_time->getWidth();
    uint16_t time_y = height - kSKILL_TIME_GAP - frame_time->getHeight();

    frame_time->render(renderer, time_x, time_y);

    t1.setText(renderer, std::to_string(skill->getCooldown()), color);
    time_x -= t1.getWidth() + kSKILL_SEP;
    t1.render(renderer, time_x, time_y - 1);

    /* Render the chance of success */
    time_x -= frame_percent->getWidth() + kSKILL_SUCCESS;
    frame_percent->render(renderer, time_x, time_y);

    t1.setText(renderer, std::to_string((int)skill->getChance()), color);
    t1.render(renderer, time_x - t1.getWidth() - kSKILL_SEP, time_y - 1);
  }

  /* Render the description */
  uint16_t line_width = width - text_x;
  std::vector<std::string> desc_set =
      Text::splitLine(font_subheader, skill->getDescription(), line_width);

  text_y += t1.getHeight() + kSKILL_DESC_GAP;
  for(uint16_t i = 0; i < desc_set.size() && i < kSKILL_DESC_LINES; i++)
  {
    if(i == (kSKILL_DESC_LINES - 1) && desc_set.size() > kSKILL_DESC_LINES)
      t2.setText(renderer, Text::splitLine(font_subheader,
                                           desc_set[i] + " " + desc_set[i + 1],
                                           line_width, true).front(),
                 color);
    else
      t2.setText(renderer, desc_set[i], color);
    t2.render(renderer, text_x,
              text_y + (t2.getHeight() + kSKILL_DESC_SEP) * i);
  }

  /* Return the new texture */
  SDL_SetRenderTarget(renderer, nullptr);

  return texture;
}

bool BattleMenu::renderActionTypes(uint32_t x, uint32_t y, uint32_t w,
                                   uint32_t h)
{
  /* Fonts */
  auto font_header = config->getFontTTF(FontName::BATTLE_HEADER);

  /* Variable Construction */
  SDL_Color color{255, 255, 255, 255};
  bool success{true};
  Text* t{new Text(font_header)};

  /* Calculate starting Y co-ordinate */
  int32_t start_y = 0;
  t->setText(renderer, "Test", color);

  if(valid_action_types.size() >= kTYPE_MAX)
  {
    start_y = y + kTYPE_MARGIN;
  }
  else
  {
    start_y = y;
    start_y +=
        (h - valid_action_types.size() * (t->getHeight() + kTYPE_MARGIN * 2)) /
        2;
  }

  for(uint32_t i = 0; i < valid_action_types.size() && i < kTYPE_MAX; i++)
  {
    /* Set the text */
    int32_t index = i + element_index;

    /* Action Type at index */
    auto type = valid_action_types.at(i);

    std::string text_str = Helpers::actionTypeToStr(type);
    success &= t->setText(renderer, text_str, color);

    /* Calculate X/Y locations */
    int32_t text_x = x + kTYPE_MARGIN * 2;
    int32_t text_y =
        start_y + kTYPE_MARGIN * (i + 1) + (t->getHeight() + kTYPE_MARGIN) * i;

    /* If selected, draw background box */
    if((menu_layer == BattleMenuLayer::TYPE_SELECTION &&
        index == element_index) ||
       (menu_layer != BattleMenuLayer::TYPE_SELECTION &&
        type == selected_action_type))
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 45);
      SDL_Rect text_rect;
      text_rect.x = text_x - kTYPE_SELECT;
      text_rect.y = text_y - kTYPE_SELECT;
      text_rect.w = t->getWidth() + kTYPE_SELECT * 2;
      text_rect.h = t->getHeight() + kTYPE_SELECT * 2;
      SDL_RenderFillRect(renderer, &text_rect);
    }

    /* Render the Text */
    success &= t->render(renderer, text_x, text_y);

    /* Render the scrollbar, if needed */
    if(valid_action_types.size() > kTYPE_MAX && (i == 0 || i == kTYPE_MAX - 1))
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
      uint16_t center_x = x + w - kTYPE_MARGIN * 2;
      uint16_t center_y = text_y + t->getHeight() / 2;

      /* Top of scroll */
      if(i == 0)
      {
        if(element_index == 0)
        {
          success &= Frame::renderCircleFilled(center_x - 1, center_y,
                                               kSCROLL_R, renderer);
        }
        else
        {
          center_y -= 1;
          success &= Frame::renderTriangle(
              center_x, center_y - kSCROLL_R + 1, center_x - kSCROLL_R,
              center_y + kSCROLL_R, center_x + kSCROLL_R, center_y + kSCROLL_R,
              renderer);
        }
      }

      /* Bottom of scroll */
      if(i == kTYPE_MAX - 1)
      {
        uint16_t bottom_index = element_index + kTYPE_MAX;

        if(bottom_index == valid_action_types.size())
        {
          success &= Frame::renderCircleFilled(center_x - 1, center_y,
                                               kSCROLL_R, renderer);
        }
        else
        {
          center_y += 1;
          success &= Frame::renderTriangle(
              center_x, center_y + kSCROLL_R - 1, center_x - kSCROLL_R,
              center_y - kSCROLL_R, center_x + kSCROLL_R, center_y - kSCROLL_R,
              renderer);
        }
      }
    }
  }

  delete t;

  return success;
}

bool BattleMenu::renderSkills(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
  bool success{true};

  /* Start X/Y  */
  int32_t text_x = x + kTYPE_MARGIN * 2;
  int32_t text_y = 0;

  if(frames_skill_name.size() >= kTYPE_MAX)
  {
    text_y = y + kTYPE_MARGIN;
  }
  else
  {
    for(const auto& frame_skill_name : frames_skill_name)
      text_y += frame_skill_name->getHeight();

    text_y = y + (h - text_y) / 2;
  }

  for(uint32_t i = 0; i < frames_skill_name.size() && i < kTYPE_MAX; i++)
  {
    int32_t index = i + element_index;

    if(index == element_index)
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 45);
      SDL_Rect text_rect;
      text_rect.x = text_x - kTYPE_SELECT;
      text_rect.y = text_y;
      text_rect.w = frames_skill_name[index]->getWidth() + kTYPE_SELECT * 2;
      text_rect.h = frames_skill_name[index]->getHeight();
      SDL_RenderFillRect(renderer, &text_rect);
    }

    success &= frames_skill_name[index]->render(renderer, text_x, text_y);

    if(frames_skill_name.size() > kTYPE_MAX && (i == 0 || i == kTYPE_MAX - 1))
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
      uint16_t center_x = x + w - kTYPE_MARGIN * 2;
      uint16_t center_y = text_y + frames_skill_name[i]->getHeight() / 2;

      /* Top of scroll */
      if(i == 0)
      {
        if(element_index == 0)
          success &= Frame::renderCircleFilled(center_x - 1, center_y,
                                               kSCROLL_R, renderer);
        else
        {
          center_y -= 1;
          success &= Frame::renderTriangle(
              center_x, center_y - kSCROLL_R + 1, center_x - kSCROLL_R,
              center_y + kSCROLL_R, center_x + kSCROLL_R, center_y + kSCROLL_R,
              renderer);
        }
      }

      /* Bottom of scroll */
      if(i == kTYPE_MAX - 1)
      {
        uint16_t bottom_index = element_index + kTYPE_MAX;
        if(bottom_index == frames_skill_name.size())
          success &= Frame::renderCircleFilled(center_x - 1, center_y,
                                               kSCROLL_R, renderer);
        else
        {
          center_y += 1;
          success &= Frame::renderTriangle(
              center_x, center_y + kSCROLL_R - 1, center_x - kSCROLL_R,
              center_y - kSCROLL_R, center_x + kSCROLL_R, center_y - kSCROLL_R,
              renderer);
        }
      }
    }

    text_y += frames_skill_name[index]->getHeight();
  }

  return success;
}

/*=============================================================================
 * PUBLIC FUNCTIONS - OPERATION
 *============================================================================*/

/* Clears the information in the menu - for a new person/turn */
void BattleMenu::clear()
{
  actor = nullptr;
  clearSkillFrames();
  setFlag(BattleMenuState::SELECTION_COMPLETE, false);
  setFlag(BattleMenuState::SKILL_FRAMES_BUILT, false);
  menu_layer = BattleMenuLayer::ZEROTH_LAYER;
  selected_action_scope = ActionScope::NO_SCOPE;
  selected_action_type = ActionType::NONE;
  selected_battle_skill = nullptr;
  selected_battle_item = nullptr;
  status_window = WindowStatus::HIDING;
  valid_action_types.clear();
  valid_battle_items.clear();
  valid_battle_skills.clear();
  element_index = -1;
}

bool BattleMenu::buildData()
{
  if(battle_display_data)
  {
    frame_qd = Frame(battle_display_data->getFrameQD());

    return true;
  }



  return false;
}

BattleActor* BattleMenu::getActor()
{
  return actor;
}

bool BattleMenu::getFlag(const BattleMenuState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

bool BattleMenu::setActor(BattleActor* actor)
{
  this->actor = actor;

  return this->actor;
}

/* Assigns the configuration of the BattleMenu */
bool BattleMenu::setConfig(Options* config)
{
  this->config = config;

  return this->config;
}

bool BattleMenu::setDisplayData(BattleDisplayData* battle_display_data)
{
  this->battle_display_data = battle_display_data;

  return this->battle_display_data;
}

void BattleMenu::setFlag(BattleMenuState flag, const bool& set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/* Assigns the Renderer of BattleMenu elements */
bool BattleMenu::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;

  return this->renderer;
}

void BattleMenu::setSelectableSkills(std::vector<BattleSkill*> menu_skills)
{
  this->valid_battle_skills = menu_skills;
}

void BattleMenu::setSelectableItems(std::vector<BattleItem*> menu_items)
{
  this->valid_battle_items = menu_items;
}

/*=============================================================================
 * PUBLIC FUNCTIONS - RENDERING
 *============================================================================*/

// TODO: Colours/grayness for various BattleSkill validness? For items too
bool BattleMenu::createSkillFrames(uint32_t width_left, uint32_t width_right)
{
  /* A renderer and configuration must be assigned */
  assert(renderer && config);

  SDL_Color color{255, 255, 255, 255};
  SDL_Color invalid_color{100, 100, 100, 255};
  bool success{true};
  uint32_t text_height{0};
  uint32_t text_width{width_left - kTYPE_MARGIN * 8};

  Text* t = new Text(config->getFontTTF(FontName::BATTLE_HEADER));

  /* Delete frames for skills if skills are already rendered */
  clearSkillFrames();

  for(auto& skill : valid_battle_skills)
  {
    /* Skill must have (a) valid pointer(s) */
    assert(t && skill && skill->skill);

    frames_skill_name.push_back(new Frame());
    frames_skill_info.push_back(new Frame());

    /* ValidStatus of the Battle skill -> reflects the colour of Skill frame
     */
    // TODO: Alternate ValidStatus colours depending on Skill condition?
    if(skill->valid_status == ValidStatus::VALID)
      success &= t->setText(renderer, skill->skill->getName(), color);
    else
      success &= t->setText(renderer, skill->skill->getName(), invalid_color);

    if(text_height == 0)
      text_height = t->getHeight() + kTYPE_MARGIN * 2;

    /* Create rendering texture */
    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, text_width, text_height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* Render the text */
    success &= t->render(renderer, 0, kTYPE_MARGIN);

    /* Render the QD */
    if(skill->valid_status == ValidStatus::VALID)
      frame_qd.setAlpha(128);

    uint32_t qd_x = text_width - frame_qd.getWidth();
    success &= frame_qd.render(renderer, qd_x, kTYPE_MARGIN + 1);
    frame_qd.setAlpha(255);

    if(skill->valid_status == ValidStatus::VALID)
    {
      success &=
          t->setText(renderer, std::to_string(skill->skill->getCost()), color);
    }
    else
    {
      success &= t->setText(renderer, std::to_string(skill->skill->getCost()),
                            invalid_color);
    }

    success &=
        t->render(renderer, qd_x - t->getWidth() - kSKILL_SEP, kTYPE_MARGIN);

    frames_skill_name.back()->setTexture(texture);
    SDL_SetRenderTarget(renderer, nullptr);

    /* Create the detailed skill information for this skill */
    frames_skill_info.back()->setTexture(createSkillFrame(
        skill, width_right - kTYPE_MARGIN * 2 - kBIGBAR_R_OFFSET,
        kBIGBAR_OFFSET + kBIGBAR_CHOOSE - kMENU_SEPARATOR_T -
            kMENU_SEPARATOR_B));
  }

  /* Delete the created text */
  delete t;

  if(success)
    setFlag(BattleMenuState::SKILL_FRAMES_BUILT, true);

  return success;
}

bool BattleMenu::render()
{
  assert(renderer && config);
  bool success{true};

  if(status_window != WindowStatus::HIDING)
  {
    uint16_t screen_width = config->getScreenWidth();
    uint16_t screen_height = config->getScreenHeight();

    uint16_t bar_height = kBIGBAR_OFFSET + kBIGBAR_CHOOSE;
    uint16_t section1_w = screen_width * kBIGBAR_L;

    /* Render separator */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_Rect rect;
    rect.x = section1_w;
    rect.y = screen_height - bar_height + kMENU_SEPARATOR_T;
    rect.w = 1;
    rect.h = screen_height - rect.y - kMENU_SEPARATOR_B;
    SDL_RenderFillRect(renderer, &rect);

    /* Render the second separator */
    uint16_t section2_w = screen_width * kBIGBAR_M1;
    SDL_Rect rect2;
    rect2.x = rect.x + section2_w;
    rect2.y = rect.y;
    rect2.w = 1;
    rect2.h = rect.h;
    SDL_RenderFillRect(renderer, &rect2);

    /* Get actions */
    success &= renderActionTypes(rect.x, rect.y, section2_w, rect2.h);

    /* Render the third section */
    if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
    {
      /* Render the third separator */
      uint16_t section3_w = screen_width * kBIGBAR_M2;
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
      SDL_Rect rect3;
      rect3.x = rect2.x + section3_w;
      rect3.y = rect.y;
      rect3.w = 1;
      rect3.h = rect.h;
      SDL_RenderFillRect(renderer, &rect3);

      /* Render the actions/items, depending on category */
      if(selected_action_type == ActionType::SKILL)
      {
        success &= renderSkills(rect2.x, rect2.y, section3_w, rect3.h);
        success &= frames_skill_info[element_index]->render(
            renderer, rect3.x + kTYPE_MARGIN, rect3.y);
      }
      if(selected_action_type == ActionType::ITEM)
      {
        // TODO: Render Items magically [07-13-15]
        success = false;
      }
    }
  }

  return success;
}

/*
 * Description: Decrement a menu layer to a given layer index. Performs all
 *              necessary updating and reworking to do this.
 *
 * Inputs: int32_t new_layer_index - index of new layer to go to.
 * Output: bool - true if the decrement was successful.
 */
// bool BattleMenu::decrementLayer(const int32_t& new_layer_index)
// {
//   auto success = false;

//   if(new_layer_index <= 3)
//   {
//     valid_targets.clear();
//     selected_targets.clear();

//     element_index = 0;
//     // TODO: element index must be valid [08-03-14]

//     layer_index = 3;

//     setMenuFlag(MenuState::TARGETS_ASSIGNED, false);
//     setMenuFlag(MenuState::SCOPE_ASSIGNED, false);

//     success = true;
//   }

//   if(new_layer_index <= 2)
//   {
//     if(qtdr_cost_paid != 0)
//     {
// #ifdef UDEBUG
//       std::cout << "Replacing " << qtdr_cost_paid << " QD paid for action."
//                 << std::endl;
// #endif

//       current_user->getCurr().alterStat("QTDR", qtdr_cost_paid);
//       qtdr_cost_paid = 0;
//     }

//     setMenuFlag(MenuState::SKILL_SELECTED, false);
//     setMenuFlag(MenuState::ACTION_SELECTED, false);

//     action_scope = ActionScope::NO_SCOPE;
//     layer_index = 2;

//     success = true;
//   }

//   if(new_layer_index <= 1)
//   {
//     action_type = ActionType::NONE;
//     layer_index = 1;

//     success = true;
//   }

//   return success;
// }

/*
 * Description: Increments the BattleMenu to a given layer index. Performs
 *              all necessary updating to make this happen,
 *
 * Inputs: int32_t - new layer index for the Battle menu
 * Output: bool - true if the incrementing occurs correctly
 */
// bool BattleMenu::incrementLayer(const int32_t& new_layer_index)
// {
//   if(layer_index != new_layer_index)
//     element_index = 0;

//   if(new_layer_index == 2)
//   {
//     layer_index = 2;

//     return true;
//   }

//   else if(new_layer_index == 3)
//   {
//     setMenuFlag(MenuState::SKILL_SELECTED, true);
//     setMenuFlag(MenuState::ACTION_SELECTED, true);

//     layer_index = 3;

//     return true;
//   }

//   else if(new_layer_index == 4)
//   {
//     setMenuFlag(MenuState::SELECTION_VERIFIED, true);
//     setMenuFlag(MenuState::SKILL_SELECTED, true);
//     setMenuFlag(MenuState::ACTION_SELECTED, true);

//     layer_index = 4;

//     return true;
//   }

//   return false;
// }

/*
 * Description: Adds a new target by inded value to the array of selected
 *              targets and removes this new target from the vector of remaining
 *              valid targets.
 *
 * Inputs: int32_t - index [can be +/- depending on party] of target to add
 * Output: bool - true if the target was added succesfully.
 */
// bool BattleMenu::addTarget(const int32_t& new_target)
// {
//   auto it = std::find(begin(valid_targets), end(valid_targets), new_target);

//   if(it != end(valid_targets))
//   {
//     selected_targets.push_back(*it);
//     valid_targets.erase(it);

//     if(valid_targets.size() > 0 && isValidIndex(0))
//       element_index = 0;
//     else
//       element_index = -1;

//     return true;
//   }

//   return false;
// }

/*
 * Description: Add all the targets of the same party of a given Index (based
 *              on whether the given index is positive or negative) to the
 *              vector of selected targets and remove them all from valid
 *              targets (although the selection should be complete)
 *
 * Inputs: int32_t party_index - given index of member to be added to targets
 * Output: bool - true if all party targets were added.
 */
// bool BattleMenu::addPartyTargets(const int32_t& party_index)
// {
//   selected_targets = getPartyTargets(party_index);

//   return true;
// }

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
// std::vector<int32_t> BattleMenu::getPartyTargets(int32_t party_index)
// {
//   std::vector<int32_t> highlight_targets;

//   for(auto it = begin(valid_targets); it != end(valid_targets); ++it)
//     if((*it < 0 && party_index < 0) || (*it > 0 && party_index > 0))
//       highlight_targets.push_back(*it);

//   return highlight_targets;
// }

/*
 * Description: Attempts to remove the last added target.
 *
 * Inputs: bool clear_all - whether to clear all the target assignments.
 * Output: bool - true if ???
 */
// bool BattleMenu::removeLastTarget(const bool& clear_all)
// {
//   if(clear_all && selected_targets.size() == 0)
//   {
//     return true;
//   }
//   else if(clear_all)
//   {
//     valid_targets.clear();
//     selected_targets.clear();

//      Make sure Battle update will assign new targets
//     setMenuFlag(MenuState::TARGETS_ASSIGNED, false);

//     return false;
//   }

//   if(selected_targets.size() > 0)
//   {
//     valid_targets.push_back(*(end(selected_targets) - 1));
//     selected_targets.pop_back();

//     return false;
//   }

//   return true;
// }

/*
 * Description: Jump to element index based on alha numeric keys.
 *
 * Inputs:
 * Output:
 */
/* Methods for containing code for each key action */
// void BattleMenu::keyDownAlpha(const char& c)
// {
//   // auto index = menu_skills->getIndexOfAlpha(c);
//   (void)c;
//   auto index = -1; // TODO: [08-24-14] Redo menu alpha skill selection.

//   if(index != -1)
//     element_index = index;
// }

/*
 * Description: Method for holding what happens when the player hits the
 *              cancel Key.
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::keyDownCancel()
// {
//   auto decrement_to_layer = -1;

//   if(layer_index == 2)
//   {
//     decrement_to_layer = 1;
//   }
//   else if(layer_index == 3)
//   {
//     /* Back on a SKILL or ITEM action will remove the last added target,
//      * which depends on the scope of the action:
//      *
//      *    TWO_ENEMIES   - previous single target cleared
//      *    TWO_ALLIES    - previous single target cleared
//      *
//      *    NO_SCOPE      - invalid
//      *
//      *    all others    - clear vector
//      */
//     if(action_type == ActionType::SKILL || action_type == ActionType::ITEM)
//     {
//       if(action_scope == ActionScope::NO_SCOPE)
//       {
//         decrement_to_layer = 2;
//       }
//       else if(action_scope == ActionScope::TWO_ENEMIES ||
//               action_scope == ActionScope::TWO_ALLIES)
//       {
//          If the size is greater than 0, pop the last selected target off,
//            or go back to the skill selection menu
//         if(selected_targets.size() > 0)
//           removeLastTarget();
//         else
//           decrement_to_layer = 2;
//       }
//       else
//       {
//         if(removeLastTarget(true))
//         {
//           decrement_to_layer = 2;
//         }
//       }
//     }
//     /* Hitting cancel on a GUARD action will reset the action_type */
//     else if(action_type == ActionType::GUARD ||
//             action_type == ActionType::DEFEND ||
//             action_type == ActionType::PASS)
//     {
//       decrement_to_layer = 1;
//     }
//   }
//   else if(layer_index == 4)
//   {
//     if(action_type == ActionType::DEFEND || action_type == ActionType::RUN ||
//        action_type == ActionType::PASS)
//       decrement_to_layer = 1;
//     else
//       decrement_to_layer = 3;
//   }

//   if(decrement_to_layer != -1)
//     decrementLayer(decrement_to_layer);
// }

/*
 * Description: Method for what happens when the player hits the decrement key.
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::keyDownDecrement()
// {
//   /* Move to the last index if on the first index */
//   if(element_index == 0)
//   {
//     if(getLayerIndex() > 2)
//     {
//       element_index = getMaxIndex();

//       if(action_type != ActionType::NONE && valid_targets.empty())
//         for(; !indexHasTargets() && element_index > 0;)
//           element_index--;
//     }
//   }
//   else
//   {
//     element_index--;

//     if(action_type != ActionType::NONE && valid_targets.empty())
//     {
//       for(; !indexHasTargets() && element_index > 0;)
//         element_index--;
//     }
//   }
// }

/*
 * Description: Method for what happens when the player hits the increment key.
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::keyDownIncrement()
// {
//   /* Increment the viewing index if it is less than the max index */
//   if(element_index < getMaxIndex())
//   {
//     element_index++;

//     if(action_type != ActionType::NONE && valid_targets.empty())
//     {
//       for(; !indexHasTargets() && element_index < getMaxIndex();)
//         element_index++;

//        If there are no valid targets at the end of the list, search
//        * through again starting at the top of the vector until one has
//        * been found. This next step ASSUMES there exists at least one
//        * valid skill (which should have been checked already).

//       if(element_index == getMaxIndex() && !indexHasTargets())
//       {
//         element_index = 0;

//         for(; !indexHasTargets() && element_index < getMaxIndex();)
//           element_index++;
//       }
//     }
//   }
//   /* Otherwise, set to the top index */
//   else if(getLayerIndex() > 2)
//   {
//     element_index = 0;

//     if(action_type != ActionType::NONE && valid_targets.empty())
//     {
//       for(; !indexHasTargets();)
//         element_index++;
//     }
//   }
// }

/*
 * Description: Method for what happens when the player hits the select key.
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::keyDownSelect()
// {
//   auto layer_to_increment = -1;

//   if(layer_index == 1)
//   {
//     if(static_cast<uint32_t>(element_index) < valid_actions.size())
//       action_type = valid_actions.at(element_index);

// #ifdef UDEBUG
//     std::cout << "Attempting selection of current action t+pe: "
//               << Helpers::actionTypeToStr(action_type) << std::endl;
// #endif

//     /* Assert there are skills if the chosen action type is SKILL */
//     if(action_type == ActionType::SKILL)
//     {
//       if(menu_skills.size() > 0 && someIndexHasTargets())
//         layer_to_increment = 2;

//       if(layer_to_increment == -1)
//       {
//         action_type = ActionType::NONE;

// #ifdef UDEBUG
//         std::cout << "No valid skills available, please select another
//         action"
//                   << std::endl;
// #endif
//       }
//     }

//     /* Else, assert there are items if the chosen action type is ITEM */
//     else if(action_type == ActionType::ITEM)
//     {
//       if(menu_items.size() > 0 && someIndexHasTargets())
//         layer_to_increment = 2;

//       if(layer_to_increment == -1)
//       {
//         action_type = ActionType::NONE;

// #ifdef UDEBUG
//         std::cout << "No items available please select another action!"
//                   << std::endl;
// #endif
//       }
//     }

//     /* If the selected action is GUARD, increment directly to target layer */
//     else if(action_type == ActionType::GUARD)
//     {
//       if(num_allies > 1)
//         layer_to_increment = 3;
//       else
//       {
//         action_type = ActionType::NONE;

// #ifdef UDEBUG
//         std::cout << "No allies to guard!" << std::endl;
// #endif
//       }
//     }

//     /* DEFEND, RUN, PASS actions require no other menus -> done */
//     else if(action_type == ActionType::DEFEND ||
//             action_type == ActionType::RUN || action_type ==
//             ActionType::PASS)
//     {
//       layer_to_increment = 4;
//     }
//   }

//   else if(layer_index == 2)
//   {
//     /* Selection of skill index -> move to target menu */
//     if(action_type == ActionType::SKILL)
//     {
//       if(static_cast<uint32_t>(element_index) < menu_skills.size())
//       {
//         if(indexHasTargets())
//         {
//           /* Grab the selected skill */
//           selected_skill = menu_skills.at(element_index);
//           setMenuFlag(MenuState::SKILL_SELECTED, true);

//           /* Decrease the current user's QD by the cost required */
//           auto true_cost = current_user->getTrueCost(selected_skill.skill);

//           if(true_cost <= current_user->getCurr().getStat("QTDR"))
//           {
// #ifdef UDEBUG
//             std::cout << "Decreasing " << current_user->getName() << "s QTDR"
//                       << " by " << true_cost << "." << std::endl;
// #endif

//             layer_to_increment = 3;
//             qtdr_cost_paid = true_cost;
//             current_user->getCurr().alterStat(Attribute::QTDR, -true_cost);
//           }
//           else
//           {
// #ifdef UDEBUG
//             std::cout << "Cannot afford that skill!" << std::endl;
// #endif
//           }
//         }
// #ifdef UDEBUG
//         std::cout << "Selected skill index has invalid targets" << std::endl;
// #endif
//       }
//     }
//     else if(action_type == ActionType::ITEM)
//     {
//       if(static_cast<uint32_t>(element_index) < menu_items.size())
//       {
//         layer_to_increment = 3;

//         selected_item = menu_items.at(element_index).item;
//       }
//     }
//   }
//   else if(layer_index == 3)
//   {
//     if(action_type == ActionType::SKILL || action_type == ActionType::ITEM)
//     {
//       if(action_scope == ActionScope::ONE_PARTY)
//       {
//         addPartyTargets(valid_targets.at(element_index));

//         if(selected_targets.size() > 0)
//           layer_to_increment = 4;
//       }
//       else if(action_scope == ActionScope::ALL_ENEMIES ||
//               action_scope == ActionScope::ALL_ALLIES ||
//               action_scope == ActionScope::ALL_ALLIES_KO ||
//               action_scope == ActionScope::ALL_TARGETS ||
//               action_scope == ActionScope::ALL_NOT_USER)
//       {
//         while(valid_targets.size() > 0 && element_index != -1)
//           addTarget(valid_targets.at(valid_targets.size() - 1));

//         if(selected_targets.size() > 0)
//           layer_to_increment = 4;
//       }
//       else if(action_scope == ActionScope::TWO_ENEMIES ||
//               action_scope == ActionScope::TWO_ALLIES)
//       {
//         if(valid_targets.size() > 0 && element_index != -1)
//           addTarget(valid_targets.at(element_index));

//         if(selected_targets.size() == 2)
//           layer_to_increment = 4;
//       }
//       else
//       {
//         if(valid_targets.size() > 0 && element_index != -1)
//         {
//           addTarget(valid_targets.at(element_index));
//           layer_to_increment = 4;
//         }
//       }
//     }
//     else if(action_type == ActionType::DEFEND ||
//             action_type == ActionType::GUARD ||
//             action_type == ActionType::IMPLODE)
//     {
//       if(valid_targets.size() > 0 && element_index != -1)
//       {
//         addTarget(valid_targets.at(element_index));
//         layer_to_increment = 4;
//       }
//     }
//     else if(action_type == ActionType::RUN || action_type ==
//     ActionType::PASS)
//     {
//       layer_to_increment = 4;
//     }
//   }
//   else if(layer_index == 4)
//   {
//     setMenuFlag(MenuState::SELECTION_VERIFIED, true);
//   }

//   if(layer_to_increment != -1)
//     incrementLayer(layer_to_increment);
// }

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
// int32_t BattleMenu::getPartyTargetIndex(bool opposite)
// {
//   if(opposite)
//   {
//     for(int16_t index = valid_targets.size() - 1; index >= 0; index--)
//       if(valid_targets.at(index) < 0)
//         return index;
//   }
//   else
//   {
//     for(size_t index = 0; index < valid_targets.size(); index++)
//       if(valid_targets.at(index) > 0)
//         return index;
//   }

//   return 0;
// }

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Selects a random action among the list of available menu.
 *              This function is used in lieu of the menu for ailments
 *              such as
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::selectRandomAction()
// {
//   if(menu_skills.size() > 0)
//   {
//     auto rand_elm = Helpers::randU(0, menu_skills.size() - 1);
//     selected_skill = menu_skills.at(rand_elm);
//     setMenuFlag(MenuState::SKILL_SELECTED, true);

//     action_type = ActionType::SKILL;
//     setMenuFlag(MenuState::ACTION_SELECTED, true);
//   }
// }

// bool BattleMenu::isValidIndex(int32_t check_index)
// {
//   auto it = std::find(begin(valid_targets), end(valid_targets), check_index);
//   return it != end(valid_targets);
// }

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::swapPartyIndex()
// {
//   auto temp_index = person_index;

//   if(person_index < 0)
//   {
//     person_index = 1;
//     mostLeftIndex();
//   }
//   else if(person_index > 0)
//   {
//     person_index = -1;
//     mostRightIndex();
//   }

//   if(person_index == 0)
//     person_index = temp_index;

//   element_index = person_index;
// }

/*
 * Description: Prints out the available items for the BattleMenu.
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::printItems()
// {
//   auto index = 0;

//   for(auto it = begin(menu_items); it != end(menu_items); ++it, ++index)
//   {
//     if(index == element_index)
//       std::cout << "[X]";
//     else
//       std::cout << "[ ]";

//     std::cout << " --- " << (*it).item->getName() << " -- [ " << (*it).amount
//               << " ] -- [";

//     for(auto jt = begin((*it).all_targets); jt != end((*it).all_targets);
//     ++jt)
//     {
//       std::cout << (*jt)->getName() << ", ";
//     }

//     std::cout << "]" << std::endl;
//   }
// }

/*
 * Description: Method for processing keydown events.
 *
 * Inputs: SDL_KeyboardEvent event - the key down event.
 * Output: bool - true if ???
 */
// bool BattleMenu::keyDownEvent(SDL_KeyboardEvent event)
// {
//   auto change_index = false;

//   if(event.keysym.sym == SDLK_UP || event.keysym.sym == SDLK_DOWN ||
//      event.keysym.sym == SDLK_LEFT || event.keysym.sym == SDLK_RIGHT)
//   {
//     change_index = true;

//     if(action_type == ActionType::SKILL)
//     {
//       if(layer_index == 3 && action_scope == ActionScope::ONE_PARTY)
//       {
//         change_index = false;

//         std::cout << element_index << std::endl;
//         if(element_index == 0)
//           element_index = getMaxIndex();
//         else if(element_index == getMaxIndex())
//           element_index = 0;
//         std::cout << element_index << ":" << getMaxIndex() << std::endl;
//       }
//     }

//     if(layer_index == 4)
//       change_index = false;
//   }

//   if(change_index)
//   {
//     if(event.keysym.sym == SDLK_UP || event.keysym.sym == SDLK_RIGHT)
//     {
//       if(layer_index == 3)
//         keyDownIncrement();
//       else
//         keyDownDecrement();
//     }
//     else if(event.keysym.sym == SDLK_DOWN || event.keysym.sym == SDLK_LEFT)
//     {
//       if(layer_index == 3)
//         keyDownDecrement();
//       else
//         keyDownIncrement();
//     }
//   }
//   else if(event.keysym.sym == SDLK_SPACE)
//     keyDownSelect();
//   else if(event.keysym.sym == SDLK_BACKSPACE)
//     keyDownCancel();
//   else if(event.keysym.sym == SDLK_HOME)
//     printMenuState();
//   else if(static_cast<int>(event.keysym.sym) >= 'a' &&
//           static_cast<int>(event.keysym.sym) <= 'z')
//   {
//     keyDownAlpha(static_cast<int>(event.keysym.sym));
//   }
//   else
//   {
// #ifdef UDEBUG
//     std::cout << "CANNOT CHANGE INDEX" << std::endl;
// #endif
//   }

// #ifdef UDEBUG
//   if(current_user != nullptr)
//   {
//     std::cout << "Selecting action for person index: " << person_index
//               << " named: " << current_user->getName() << std::endl;
//   }

//   if(getMenuFlag(MenuState::SELECTION_VERIFIED))
//     std::cout << "Selection has been verified!\n" << std::endl;
// #endif

//   return false;
// }

/*
 * Description: Prints out the state of the menu for a non-gui battle based
 *              on the current layering index.
 *
 * Inputs: none
 * Output: none
 */
// void BattleMenu::printMenuState()
// {
//   if(layer_index == 1)
//     printValidActions();
//   else if(layer_index == 2 && action_type == ActionType::SKILL)
//     printSkills();
//   else if(layer_index == 2 && action_type == ActionType::ITEM)
//     printItems();
//   else if(layer_index == 3)
//     printTargets(true);
//   else if(layer_index == 4)
//   {
//     std::cout << "Action Type: " << Helpers::actionTypeToStr(action_type)
//               << " with name: ";

//     if(action_type == ActionType::SKILL &&
//        getMenuFlag(MenuState::SKILL_SELECTED))
//     {
//       std::cout << selected_skill.skill->getName();
//     }
//     else if(action_type == ActionType::ITEM)
//       std::cout << selected_item->getName();
//     else
//       std::cout << "NONE";

//     std::cout << std::endl
//               << "Targeting person indexes: ";

//     for(auto it = begin(selected_targets); it != end(selected_targets); ++it)
//       std::cout << (*it) << ", ";

//     std::cout << std::endl
//               << "Select this action?" << std::endl;
//   }
// }

/*
 * Description: Returns enumerated action type for the selected action. This
 *              will be ActionType::NONE if no action type is currently
 *              selected.
 *
 * Inputs:
 * Output:
 */
// ActionType BattleMenu::getActionType()
// {
//   return action_type;
// }

/*
 * Description: Returns the list of action target indexes which were selected
 *              by the user of the action.
 *
 * Inputs: none
 * Output: std::vector<int32_t> - vector of indexes of selected targets
 */
// std::vector<int32_t> BattleMenu::getActionTargets()
// {
//   return selected_targets;
// }

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
// Person* BattleMenu::getCurrentUser()
// {
//   return current_user;
// }

/* Get targets hovered over during the selection process */
// TODO: Comment
// std::vector<int32_t> BattleMenu::getHoverTargets()
// {
//   std::vector<int32_t> hover_targets;

// if(action_type == ActionType::SKILL || action_type == ActionType::GUARD ||
//    action_type == ActionType::ITEM)
// {
//   for(uint16_t i = 0; i < valid_targets.size(); i++)
//   {
//     /* If the index matches the element index or if the action scope is
//     always
//      * highlighting, display an 'X' on it
//      * The following action scopes will always choose all selectable targets:
//      * ALL_ENEMIES, ALL_ALLIES, ALL_ALLIES_KO, ALL_TARGETS, ALL_NOT_USER */
//     if(action_scope == ActionScope::ALL_ALLIES ||
//        action_scope == ActionScope::ALL_ENEMIES ||
//        action_scope == ActionScope::ALL_ALLIES_KO ||
//        action_scope == ActionScope::ALL_TARGETS ||
//        action_scope == ActionScope::ALL_NOT_USER ||
//        (i == element_index && action_scope != ActionScope::ONE_PARTY))
//     {
//       hover_targets.push_back(valid_targets[i]);
//     }
//   }

//   if(action_scope == ActionScope::ONE_PARTY)
//   {
//     if(valid_targets.size() > static_cast<size_t>(element_index))
//       hover_targets = getPartyTargets(valid_targets.at(element_index));
//   }
// }

//   return hover_targets;
// }

/*
 * Description: Returns the current value for maximum selection index based
 *              on the layering of hte menu.
 *
 * Inputs: none
 * Output: int32_t - the maximum selectable index
 */
// int32_t BattleMenu::getMaxIndex()
// {
//   if(layer_index == 1)
//     return valid_actions.size() - 1;
//   else if(layer_index == 2 && action_type == ActionType::SKILL)
//     return menu_skills.size() - 1;
//   else if(layer_index == 2 && action_type == ActionType::ITEM)
//     return menu_items.size() - 1;
//   else if(layer_index == 2)
//     return 0;
//   else if(layer_index == 3)
//     return valid_targets.size() - 1;

//   return 0;
//
/*
* Description: Returns the pointer to the currently selected Skill object.
*
* Inputs: none
* Output: Skill* - pointer to the currently selected Skill object.
*/
// BattleSkill* BattleMenu::getSelectedSkill()
// {
//   return selected_skill;
// }

/*
 * Description: Returns the pointer to the currently selected item objet.
 *
 * Inputs: none
 * Output: Item* - pointer to the Item object selected.
 */
// BattleItem* BattleMenu::getSelectedItem()
// {
//   return selected_item;
// }

/*
 * Description: Compiles randomized targets for the selected skill. (ex., for
 *              attacking while confused)
 *
 * Inputs: none
 * Output: std::vector<int32_t> - vector of targets
 */
// std::vector<int32_t> BattleMenu::getRandomTargets()
// {
//   std::vector<int32_t> random_targets;

//   if(action_scope == ActionScope::USER ||
//      action_scope == ActionScope::ALL_ENEMIES ||
//      action_scope == ActionScope::ALL_ALLIES ||
//      action_scope == ActionScope::ALL_ALLIES_KO ||
//      action_scope == ActionScope::ALL_TARGETS ||
//      action_scope == ActionScope::ALL_NOT_USER)
//   {
//     random_targets = valid_targets;
//   }
//   else if(action_scope == ActionScope::ONE_TARGET ||
//           action_scope == ActionScope::ONE_ENEMY ||
//           action_scope == ActionScope::ONE_ALLY ||
//           action_scope == ActionScope::ONE_ALLY_NOT_USER ||
//           action_scope == ActionScope::ONE_ALLY_KO ||
//           action_scope == ActionScope::NOT_USER)
//   {
//     random_targets = Helpers::getRandElements(valid_targets, 1);
//   }
//   else if(action_scope == ActionScope::TWO_ENEMIES ||
//           action_scope == ActionScope::TWO_ALLIES)
//   {
//     random_targets = Helpers::getRandElements(valid_targets, 2);
//   }
//   else if(action_scope == ActionScope::ONE_PARTY)
//   {
//     if(Helpers::flipCoin())
//     {
//       for(const auto& target : valid_targets)
//         if(target > 0)
//           random_targets.push_back(target);
//     }
//     else
//     {
//       for(const auto& target : valid_targets)
//         if(target < 0)
//           random_targets.push_back(target);
//     }
//   }

//   return random_targets;
// }

/*
 * Description: Assigns a vector of selectable targets (index values in vec).
 *
 * Inputs: std::vector<int32_t> new_menu_targets - the taget indexes
 * Output: bool - true if the targets are not empty.
 */
// bool BattleMenu::setSelectableTargets(std::vector<int32_t> new_menu_targets)
// {
//   valid_targets = new_menu_targets;

//   if(isValidIndex(1) && isValidIndex(2))
//     std::swap(valid_targets[0], valid_targets[1]);

//   if(isValidIndex(-1) && isValidIndex(-2))
//   {
//     auto neg_one = std::find(begin(valid_targets), end(valid_targets), -1);
//     auto neg_two = std::find(begin(valid_targets), end(valid_targets), -2);
//     std::iter_swap(neg_one, neg_two);
//   }

//   if(action_type == ActionType::SKILL &&
//      getMenuFlag(MenuState::SKILL_SELECTED) && selected_skill.skill !=
//      nullptr)
//   {
//     /* Default offensive skills to opposing party, vice versa for defensive
//     */
//     if(selected_skill.skill->getFlag(SkillFlags::OFFENSIVE))
//       element_index = getPartyTargetIndex(true);
//     else
//       element_index = getPartyTargetIndex(false);
//   }
//   else
//   {
//   }

//   if(action_type == ActionType::ITEM && selected_item != nullptr)
//   {
//     if(selected_item->getUseSkill()->getFlag(SkillFlags::OFFENSIVE))
//       element_index = getPartyTargetIndex(true);
//     else
//       element_index = getPartyTargetIndex(false);
//   }

//   return (!valid_targets.empty());
// }
