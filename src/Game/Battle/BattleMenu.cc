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

/* Text Colors */
const SDL_Color BattleMenu::kTEXT_STANDARD{255, 255, 255, 255};
const SDL_Color BattleMenu::kTEXT_INVALID{100, 100, 100, 255};
const SDL_Color BattleMenu::kTEXT_PRICEY{200, 100, 100, 255};
const SDL_Color BattleMenu::kTEXT_NO_TARGETS{255, 255, 255, 255};

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
      flags{static_cast<BattleMenuState>(0)},
      menu_layer{BattleMenuLayer::ZEROTH_LAYER},
      renderer{nullptr},
      selected_action_scope{ActionScope::NO_SCOPE},
      selected_action_type{ActionType::NONE},
      status_window{WindowStatus::HIDING},
      element_index{-1}
{
}

BattleMenu::~BattleMenu()
{
  clearSkillFrames();
  clearItemFrames();
}

/*=============================================================================
 * PRIVATE FUNCTIONS - RENDERING
 *============================================================================*/

void BattleMenu::setRectBot(SDL_Rect& rect, uint32_t height)
{
  rect.x = kSKILL_BORDER;
  rect.h = kSKILL_FRAME_S;
  rect.y = height - kSKILL_BORDER - rect.h;
  rect.w = kSKILL_FRAME_L;
}

void BattleMenu::setRectTop(SDL_Rect& rect)
{
  rect.x = kSKILL_BORDER;
  rect.y = kSKILL_BORDER;
  rect.w = kSKILL_FRAME_L;
  rect.h = kSKILL_FRAME_L;
}

/*=============================================================================
 * PRIVATE FUNCTIONS - OPERATION
 *============================================================================*/

BattleActor* BattleMenu::actorOfElementIndex(int32_t index)
{
  for(const auto& target : getSelectableTargets())
    if(target && elementIndexOfActor(target) == index)
      return target;

  return nullptr;
}

bool BattleMenu::isActionOffensive()
{
  if(selected_action_type == ActionType::SKILL)
  {
    if(selected_battle_skill && selected_battle_skill->skill)
      return selected_battle_skill->skill->getFlag(SkillFlags::OFFENSIVE);
  }
  else if(selected_action_type == ActionType::ITEM)
  {
    if(selected_battle_item)
    {
      auto use_skill = selected_battle_item->item->getUseSkill();
      return use_skill->getFlag(SkillFlags::OFFENSIVE);
    }
  }

  return false;
}

bool BattleMenu::isActorMatch(BattleActor* target, bool same)
{
  if(target && actor)
  {
    auto user_is_ally = actor->getFlag(ActorState::ALLY);
    auto target_is_ally = target->getFlag(ActorState::ALLY);

    if(same)
      return user_is_ally == target_is_ally;
    else
      return user_is_ally != target_is_ally;
  }

  return false;
}

bool BattleMenu::isIndexValid(int32_t index)
{
  if(menu_layer == BattleMenuLayer::TYPE_SELECTION)
    return ((uint32_t)index < valid_action_types.size());

  if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    if(selected_action_type == ActionType::SKILL)
    {
      return (valid_battle_skills.at(index)->valid_status ==
              ValidStatus::VALID);
    }

    if(selected_action_type == ActionType::ITEM)
    {
      return (valid_battle_items.at(index)->valid_status == ValidStatus::VALID);
    }
  }

  return false;
}

bool BattleMenu::isTargetValid(BattleActor* check_target)
{
  if(check_target)
  {
    if(selected_action_scope == ActionScope::ONE_ALLY_KO ||
       selected_action_scope == ActionScope::ALL_ALLIES_KO)
    {
      if(check_target->getStateLiving() != LivingState::ALIVE)
        return true;

      return false;
    }
    else if(selected_action_scope != ActionScope::NO_SCOPE)
    {
      if(check_target->getStateLiving() == LivingState::ALIVE)
        return true;

      return false;
    }
  }

  return false;
}

int32_t BattleMenu::elementIndexOfActor(BattleActor* check_actor)
{
  int32_t index = 0;

  for(const auto& actor : getSelectableTargets())
  {
    if(actor == check_actor)
      return index;

    ++index;
  }

  return -1;
}

BattleActor* BattleMenu::targetOfOrderedIndex(std::vector<BattleActor*> actors,
                                              int32_t ordered)
{
  for(const auto& actor : actors)
    if(actor && (actor->getIndex() == ordered))
      return actor;

  return nullptr;
}

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

void BattleMenu::keyDownCancel()
{
  event_handler->triggerSound(Sound::kID_SOUND_MENU_PREV, SoundChannels::MENUS);

  if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    menu_layer = BattleMenuLayer::TYPE_SELECTION;
    selected_action_type = ActionType::NONE;
    element_index = 0;
  }
  else if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  {

    unsetHoverTargets();
    menu_layer = BattleMenuLayer::ACTION_SELECTION;

    selected_targets.clear();
    element_index = 0;
    selected_action_scope = ActionScope::NO_SCOPE;
    selected_battle_skill = nullptr;
    selected_battle_item = nullptr;
  }
}

void BattleMenu::keyDownDecrement()
{
  event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG, SoundChannels::MENUS);

  if(selected_action_scope != ActionScope::USER)
    element_index = validPrevious();

  if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
    setHoverTargets();
}

void BattleMenu::keyDownIncrement()
{
  event_handler->triggerSound(Sound::kID_SOUND_MENU_CHG, SoundChannels::MENUS);

  if(selected_action_scope != ActionScope::USER)
    element_index = validNext();

  if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
    setHoverTargets();
}

// First valid index
int32_t BattleMenu::validFirst()
{
  if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    if(selected_action_type == ActionType::SKILL)
    {
      for(size_t i = 0; i < valid_battle_skills.size(); ++i)
        if(isIndexValid(i))
          return i;
    }
    else if(selected_action_type == ActionType::ITEM)
    {
      for(size_t i = 0; i < valid_battle_items.size(); ++i)
        if(isIndexValid(i))
          return i;
    }
  }

  if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  {
    auto selectable = getSelectableTargets();

    if(isActionOffensive())
    {
      for(const auto& target : selectable)
        if(target)
          return elementIndexOfActor(target);
    }
    else if(!isActionOffensive())
    {
      for(auto it = selectable.rbegin(); it != selectable.rend(); ++it)
        if(*it)
          return elementIndexOfActor(*it);
    }
  }

  return 0;
}

// Last valid index
int32_t BattleMenu::validLast()
{
  if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    if(selected_action_type == ActionType::SKILL)
    {
      for(int32_t i = valid_battle_skills.size() - 1; i >= 0; --i)
        if(isIndexValid(i))
          return i;
    }
    else if(selected_action_type == ActionType::ITEM)
    {
      for(int32_t i = valid_battle_items.size() - 1; i >= 0; --i)
        if(isIndexValid(i))
          return i;
    }
  }

  // if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  // {
  //   auto curr_hover = actorOfElementIndex(element_index);
  //   auto is_ally = false;

  //   if(curr_hover)
  //     is_ally = curr_hover->getFlag(ActorState::ALLY);

  //   if(is_ally)
  //     return elementIndexOfActor(getMostLeft(true));
  //   else
  //     return elementIndexOfActor(getMostRight(false));
  // }

  return -1;
}

std::vector<BattleActor*> BattleMenu::getSelectableTargets()
{
  std::vector<BattleActor*> action_targets;
  std::vector<BattleActor*> selectable_targets;

  if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  {
    if(selected_action_type == ActionType::SKILL && selected_battle_skill)
      action_targets = selected_battle_skill->targets;
    else if(selected_action_type == ActionType::ITEM && selected_battle_item)
      action_targets = selected_battle_item->targets;

    for(auto& target : action_targets)
      if(target && isTargetValid(target))
        selectable_targets.push_back(target);
  }

  /* Sub. the already selected targets from the selectable targets vector */
  std::sort(begin(selectable_targets), end(selectable_targets));
  std::sort(begin(selected_targets), end(selected_targets));

  std::vector<BattleActor*> difference;

  std::set_difference(begin(selectable_targets), end(selectable_targets),
                      begin(selected_targets), end(selected_targets),
                      std::back_inserter(difference));

  /* Sort the selectable targets by their menu index.
   *   Enemy Menu Index:  [9] [8] [7] [6] [5]
   *   Ally Enemy Index:  [4] [3] [2] [1] [0] */
  std::sort(begin(selectable_targets), end(selectable_targets),
            [&](BattleActor* a, BattleActor* b) -> bool
            {
              if(a && b)
                return (a->getMenuIndex() > b->getMenuIndex());

              return false;
            });

  return selectable_targets;
}

std::vector<BattleActor*>
BattleMenu::getPartyTargets(std::vector<BattleActor*> actors, bool allies)
{
  std::vector<BattleActor*> party_targets;

  for(const auto& actor : actors)
    if(actor->getFlag(ActorState::ALLY) == allies)
      party_targets.push_back(actor);

  return party_targets;
}

int32_t BattleMenu::getMaxIndex()
{
  if(menu_layer == BattleMenuLayer::TYPE_SELECTION)
    return valid_action_types.size() - 1;

  if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    if(selected_action_type == ActionType::SKILL)
      return valid_battle_skills.size() - 1;
    else if(selected_action_type == ActionType::ITEM)
      return valid_battle_items.size() - 1;
  }

  if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
    return getSelectableTargets().size() - 1;

  return -1;
}

int32_t BattleMenu::validNext()
{
  if(menu_layer == BattleMenuLayer::TYPE_SELECTION)
  {
    if((uint32_t)element_index + 1 < valid_action_types.size())
      return element_index + 1;
    else
      return 0;
  }
  else if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    for(int32_t i = element_index + 1; i <= getMaxIndex(); ++i)
      if(isIndexValid(i))
        return i;

    return validFirst();
  }
  else if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  {
    auto curr_hover = actorOfElementIndex(element_index);
    auto selectable_targets = getSelectableTargets();

    if(selectable_targets.size() < 2)
    {
      return element_index;
    }
    else if(curr_hover)
    {
      auto curr_index = curr_hover->getMenuIndex();
      auto next_menu_actor = nextMenuIndex(curr_index, selectable_targets);

      if(next_menu_actor)
        return elementIndexOfActor(next_menu_actor);
      else
        return elementIndexOfActor(selectable_targets.front());
    }
  }

  return -1;
}

int32_t BattleMenu::validPrevious()
{
  if(menu_layer == BattleMenuLayer::TYPE_SELECTION)
  {
    if(element_index > 0)
      return element_index - 1;
    else
      return valid_action_types.size() - 1;
  }

  else if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    for(auto i = element_index - 1; i >= 0; --i)
    {
      if(isIndexValid(i))
        return i;
    }

    return validLast();
  }

  else if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  {
    auto curr_hover = actorOfElementIndex(element_index);

    /* Get the selectable targets, sorted highest -> lowest index */
    auto selectable_targets = getSelectableTargets();

    if(selectable_targets.size() < 2)
    {
      return element_index;
    }
    else if(curr_hover)
    {
      auto curr_index = curr_hover->getMenuIndex();
      auto prev_menu_actor = prevMenuIndex(curr_index, selectable_targets);

      if(prev_menu_actor)
        return elementIndexOfActor(prev_menu_actor);
      else
        return elementIndexOfActor(selectable_targets.back());
    }
  }

  return -1;
}

/*=============================================================================
 * PRIVATE FUNCTIONS - Display
 *============================================================================*/

void BattleMenu::clearItemFrames()
{
  for(auto& item_frame : frames_item_info)
  {
    if(item_frame)
      delete item_frame;
    item_frame = nullptr;
  }

  for(auto& name_frame : frames_item_name)
  {
    if(name_frame)
      delete name_frame;
    name_frame = nullptr;
  }

  frames_item_info.clear();
  frames_item_name.clear();
}

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

SDL_Texture* BattleMenu::createItemFrame(BattleItem* battle_item,
                                         uint32_t width, uint32_t height)
{
  assert(config);

  /* Grab the skill pointer, and QD frame from display data */
  auto use_item = battle_item->item;
  auto skill = use_item->getUseSkill();

  /* Fonts */
  auto font_subheader = config->getFontTTF(FontName::BATTLE_SUBHEADER);

  Text t2(font_subheader);
  Text t5(font_subheader);
  t2.setText(renderer, use_item->getName(), kTEXT_STANDARD);

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
  setRectTop(rect_top);

  if(use_item->getThumb())
  {
    auto thumb = use_item->getThumb();
    thumb->render(renderer, rect_top.x + rect_top.w / 2 - thumb->getWidth(),
                  rect_top.y + rect_top.h / 2 - thumb->getHeight(),
                  thumb->getWidth() * 2, thumb->getHeight() * 2);
  }

  Frame::renderRect(rect_top, kSKILL_BORDER_WIDTH, renderer, true);

  /* Render the action scope */
  auto scope_frame = battle_display_data->getFrameScope(skill->getScope());

  SDL_Rect rect_bot;
  setRectBot(rect_bot, height);

  if(scope_frame)
    scope_frame->render(renderer, rect_bot.x, rect_bot.y);

  /* Render the primary element */
  auto primary_frame =
      battle_display_data->getFrameElement(skill->getPrimary());

  rect_bot.x += rect_bot.w + kSKILL_BORDER;
  rect_bot.w = kSKILL_FRAME_S;
  if(primary_frame)
    primary_frame->render(renderer, rect_bot.x, rect_bot.y);

  /* Render the secondary element */
  auto secondary_frame =
      battle_display_data->getFrameElement(skill->getSecondary());
  rect_bot.x += rect_bot.w + kSKILL_BORDER;

  if(secondary_frame)
    secondary_frame->render(renderer, rect_bot.x, rect_bot.y);

  /* Render the name */
  uint16_t text_x = rect_top.x + rect_top.w + kSKILL_BORDER;
  uint16_t text_y = 3;
  t2.render(renderer, text_x, text_y);

  /* Render the description */
  uint16_t line_width = width - text_x;
  std::vector<std::string> desc_set =
      Text::splitLine(font_subheader, use_item->getDescription(), line_width);

  text_y += t2.getHeight() + kSKILL_DESC_GAP;
  for(uint16_t i = 0; i < desc_set.size() && i < kSKILL_DESC_LINES; i++)
  {
    if(i == (kSKILL_DESC_LINES - 1) && desc_set.size() > kSKILL_DESC_LINES)
      t5.setText(renderer, Text::splitLine(font_subheader,
                                           desc_set[i] + " " + desc_set[i + 1],
                                           line_width, true).front(),
                 kTEXT_STANDARD);
    else
      t5.setText(renderer, desc_set[i], kTEXT_STANDARD);
    t5.render(renderer, text_x,
              text_y + (t2.getHeight() + kSKILL_DESC_SEP) * i);
  }

  /* Return the new texture */
  SDL_SetRenderTarget(renderer, nullptr);

  return texture;
}

SDL_Texture* BattleMenu::createSkillFrame(BattleSkill* battle_skill,
                                          uint32_t width, uint32_t height)
{
  assert(battle_display_data->getFrameQD() && config);

  /* Grab the skill pointer, and QD frame from display data */
  auto frame_qd = battle_display_data->getFrameQD();
  auto skill = battle_skill->skill;

  /* Fonts */
  auto font_header = config->getFontTTF(FontName::BATTLE_HEADER);
  auto font_subheader = config->getFontTTF(FontName::BATTLE_SUBHEADER);

  Text t1(font_header);
  Text t2(font_subheader);
  Text t3(font_subheader);
  Text t4(font_subheader);
  Text t5(font_subheader);

  t1.setText(renderer, std::to_string(skill->getCost()), kTEXT_STANDARD);
  t2.setText(renderer, skill->getName(), kTEXT_STANDARD);
  t3.setText(renderer, std::to_string(skill->getCooldown()), kTEXT_STANDARD);
  t4.setText(renderer, std::to_string((int)skill->getChance()), kTEXT_STANDARD);

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
  setRectTop(rect_top);
  if(skill->getThumbnail())
    skill->getThumbnail()->render(renderer, rect_top.x, rect_top.y);

  Frame::renderRect(rect_top, kSKILL_BORDER_WIDTH, renderer, true);

  /* Render the action scope */
  auto scope_frame = battle_display_data->getFrameScope(skill->getScope());

  SDL_Rect rect_bot;
  setRectBot(rect_bot, height);

  if(scope_frame)
    scope_frame->render(renderer, rect_bot.x, rect_bot.y);

  /* Render the primary element */
  auto primary_frame =
      battle_display_data->getFrameElement(skill->getPrimary());

  rect_bot.x += rect_bot.w + kSKILL_BORDER;
  rect_bot.w = kSKILL_FRAME_S;
  if(primary_frame)
    primary_frame->render(renderer, rect_bot.x, rect_bot.y);

  /* Render the secondary element */
  auto secondary_frame =
      battle_display_data->getFrameElement(skill->getSecondary());
  rect_bot.x += rect_bot.w + kSKILL_BORDER;

  if(secondary_frame)
    secondary_frame->render(renderer, rect_bot.x, rect_bot.y);

  /* Render the cost */
  uint16_t qd_x = width - kSKILL_BORDER - frame_qd->getWidth();
  uint16_t qd_y = kSKILL_QD_GAP;
  frame_qd->render(renderer, qd_x, qd_y);

  qd_x -= t1.getWidth() + kSKILL_SEP;
  t1.render(renderer, qd_x, qd_y - 1);

  /* Render the name */
  uint16_t text_x = rect_top.x + rect_top.w + kSKILL_BORDER;
  uint16_t text_y = qd_y - 1;
  t2.render(renderer, text_x, text_y);

  /* Render the cooldown */
  auto frame_time = battle_display_data->getFrameTime();
  auto frame_percent = battle_display_data->getFramePercent();

  if(frame_time && frame_percent)
  {
    uint16_t time_x = width - kSKILL_BORDER - frame_time->getWidth();
    uint16_t time_y = height - kSKILL_TIME_GAP - frame_time->getHeight();

    frame_time->render(renderer, time_x, time_y);
    time_x -= t3.getWidth() + kSKILL_SEP;
    t3.render(renderer, time_x, time_y - 1);

    /* Render the chance of success */
    time_x -= frame_percent->getWidth() + kSKILL_SUCCESS;
    frame_percent->render(renderer, time_x, time_y);

    t4.render(renderer, time_x - t4.getWidth() - kSKILL_SEP, time_y - 1);
  }

  /* Render the description */
  uint16_t line_width = width - text_x;

  auto desc_set =
      Text::splitLine(font_subheader, skill->getDescription(), line_width);

  text_y += t4.getHeight() + kSKILL_DESC_GAP;

  for(uint16_t i = 0; i < desc_set.size() && i < kSKILL_DESC_LINES; i++)
  {
    if(i == (kSKILL_DESC_LINES - 1) && desc_set.size() > kSKILL_DESC_LINES)
    {
      t5.setText(renderer, Text::splitLine(font_subheader,
                                           desc_set[i] + " " + desc_set[i + 1],
                                           line_width, true).front(),
                 kTEXT_STANDARD);
    }
    else
    {
      t5.setText(renderer, desc_set[i], kTEXT_STANDARD);
    }

    t5.render(renderer, text_x,
              text_y + (t2.getHeight() + kSKILL_DESC_SEP) * i);
  }

  /* Return the new texture */
  SDL_SetRenderTarget(renderer, nullptr);

  return texture;
}

bool BattleMenu::renderActionTypes(uint32_t x, uint32_t y, uint32_t w,
                                   uint32_t h)
{
  if(element_index >= (index_types + kTYPE_MAX))
    index_types++;
  else if(element_index < index_types)
    index_types--;

  /* Fonts */
  auto font_header = config->getFontTTF(FontName::BATTLE_HEADER);

  /* Variable Construction */
  bool success{true};
  Text* t{new Text(font_header)};
  uint32_t valid_size{(uint32_t)valid_action_types.size()};

  /* Calculate starting Y co-ordinate */
  int32_t start_y{0};
  t->setText(renderer, "Test", kTEXT_STANDARD);

  if(valid_size >= kTYPE_MAX)
  {
    start_y = y + kTYPE_MARGIN;
  }
  else
  {
    start_y = y;
    start_y += (h - valid_size * (t->getHeight() + kTYPE_MARGIN * 2)) / 2;
  }

  for(uint32_t i = 0; i < valid_size && i < kTYPE_MAX; i++)
  {
    // TODO int32_t index = i + index_types;

    /* Action Type at index */
    auto type = valid_action_types.at(i);

    std::string text_str = Helpers::actionTypeToStr(type);
    success &= t->setText(renderer, text_str, kTEXT_STANDARD);

    /* Calculate X/Y locations */
    int32_t text_x = x + kTYPE_MARGIN * 2;
    int32_t text_y =
        start_y + kTYPE_MARGIN * (i + 1) + (t->getHeight() + kTYPE_MARGIN) * i;

    /* If selected, draw background box */
    if((menu_layer == BattleMenuLayer::TYPE_SELECTION &&
        i == (uint32_t)element_index) ||
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
        if(index_types == 0)
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

bool BattleMenu::renderItems(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
  if(element_index >= (index_actions + kTYPE_MAX))
    index_actions++;
  else if(element_index < index_actions)
    index_actions--;

  bool success{true};
  int64_t text_x{x + kTYPE_MARGIN * 2};
  int32_t text_y{0};

  if(frames_item_name.size() >= kTYPE_MAX)
  {
    text_y = y + kTYPE_MARGIN;
  }
  else
  {
    for(const auto& frame_item_name : frames_item_name)
      text_y += frame_item_name->getHeight();

    text_y = y + (h - text_y) / 2;
  }

  for(uint32_t i = 0; i < frames_item_name.size() && i < kTYPE_MAX; i++)
  {
    int32_t index = i + index_actions;

    if(index == element_index)
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 45);
      SDL_Rect text_rect;
      text_rect.x = text_x - kTYPE_SELECT;
      text_rect.y = text_y;
      text_rect.w = frames_item_name[index]->getWidth() + kTYPE_SELECT * 2;
      text_rect.h = frames_item_name[index]->getHeight();
      SDL_RenderFillRect(renderer, &text_rect);
    }

    success &= frames_item_name[index]->render(renderer, text_x, text_y);

    if(frames_item_name.size() > kTYPE_MAX && (i == 0 || i == kTYPE_MAX - 1))
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
      uint16_t center_x = x + w - kTYPE_MARGIN * 2;
      uint16_t center_y = text_y + frames_item_name[i]->getHeight() / 2;

      /* Top of scroll */
      if(i == 0)
      {
        if(index_actions == 0)
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
        uint16_t bottom_index = index_actions + kTYPE_MAX;

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

    text_y += frames_item_name[index]->getHeight();
  }

  return success;
}

bool BattleMenu::renderSkills(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
  if(element_index >= (index_actions + kTYPE_MAX))
    index_actions++;
  else if(element_index < index_actions)
    index_actions--;

  /* Success, Start X/Y  */
  bool success{true};
  int64_t text_x{x + kTYPE_MARGIN * 2};
  int64_t text_y{0};

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
    int32_t index = i + index_actions;

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
        if(index_actions == 0)
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
        uint16_t bottom_index = index_actions + kTYPE_MAX;

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
  unsetHoverTargets();
  clearSkillFrames();
  clearItemFrames();
  setFlag(BattleMenuState::READY, false);
  setFlag(BattleMenuState::SELECTION_COMPLETE, false);
  setFlag(BattleMenuState::SKILL_FRAMES_BUILT, false);
  setFlag(BattleMenuState::ITEM_FRAMES_BUILT, false);
  menu_layer = BattleMenuLayer::ZEROTH_LAYER;
  selected_action_scope = ActionScope::NO_SCOPE;
  selected_action_type = ActionType::NONE;
  selected_battle_skill = nullptr;
  selected_battle_item = nullptr;
  selected_targets.clear();
  status_window = WindowStatus::HIDING;
  valid_action_types.clear();
  valid_battle_items.clear();
  valid_battle_skills.clear();
  element_index = -1;
  index_actions = -1;
  index_types = -1;
}

void BattleMenu::ready()
{
  status_window = WindowStatus::SHOWING;
  element_index = 0;
  index_actions = 0;
  index_types = 0;
  menu_layer = BattleMenuLayer::TYPE_SELECTION;
  setFlag(BattleMenuState::READY, true);
}

bool BattleMenu::keyDownEvent()
{
  auto& key_handler = event_handler->getKeyHandler();

  key_handler.update(0);

  if(key_handler.isDepressed(GameKey::MOVE_UP))
  {
    if(menu_layer != BattleMenuLayer::TARGET_SELECTION)
      keyDownDecrement();
    key_handler.setHeld(GameKey::MOVE_UP);
  }
  else if(key_handler.isDepressed(GameKey::MOVE_DOWN))
  {
    if(menu_layer != BattleMenuLayer::TARGET_SELECTION)
      keyDownIncrement();
    key_handler.setHeld(GameKey::MOVE_DOWN);
  }
  else if(key_handler.isDepressed(GameKey::MOVE_LEFT))
  {
    if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
      keyDownDecrement();
    key_handler.setHeld(GameKey::MOVE_LEFT);
  }
  else if(key_handler.isDepressed(GameKey::MOVE_RIGHT))
  {
    if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
      keyDownIncrement();
    key_handler.setHeld(GameKey::MOVE_RIGHT);
  }
  else if(key_handler.isDepressed(GameKey::ACTION))
  {
    keyDownSelect();
    key_handler.setHeld(GameKey::ACTION);
  }
  else if(key_handler.isDepressed(GameKey::CANCEL))
  {
    keyDownCancel();
    key_handler.setHeld(GameKey::CANCEL);
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

BattleMenuLayer BattleMenu::getMenuLayer()
{
  return menu_layer;
}

BattleSkill* BattleMenu::getSelectedBattleSkill()
{
  return selected_battle_skill;
}

BattleItem* BattleMenu::getSelectedBattleItem()
{
  return selected_battle_item;
}

ActionType BattleMenu::getSelectedType()
{
  return selected_action_type;
}

std::vector<BattleActor*> BattleMenu::getTargetsHovered()
{
  std::vector<BattleActor*> hovered_targets;

  auto selectable_targets = getSelectableTargets();

  for(auto& target : selectable_targets)
    if(target && target->getFlag(ActorState::MENU_HOVERED))
      hovered_targets.push_back(target);

  return hovered_targets;
}

void BattleMenu::unsetHoverTargets()
{
  auto selectable_targets = getSelectableTargets();

  /* Unset current hovering targets */
  for(auto& target : selectable_targets)
    if(target)
      target->setFlag(ActorState::MENU_HOVERED, false);
}

void BattleMenu::setHoverTargets()
{
  auto selectable_targets = getSelectableTargets();
  unsetHoverTargets();

  if(selected_action_scope == ActionScope::USER ||
     selected_action_scope == ActionScope::ONE_TARGET ||
     selected_action_scope == ActionScope::ONE_ENEMY ||
     selected_action_scope == ActionScope::ONE_ALLY ||
     selected_action_scope == ActionScope::TWO_ENEMIES ||
     selected_action_scope == ActionScope::TWO_ALLIES ||
     selected_action_scope == ActionScope::NOT_USER ||
     selected_action_scope == ActionScope::ONE_ALLY_KO ||
     selected_action_scope == ActionScope::ONE_ALLY_NOT_USER)
  {
    auto hovered_actor = actorOfElementIndex(element_index);

    if(hovered_actor)
      hovered_actor->setFlag(ActorState::MENU_HOVERED, true);
  }
  else if(selected_action_scope == ActionScope::ONE_PARTY)
  {
    auto hovered_actor = actorOfElementIndex(element_index);
    auto hovered_party =
        BattleActor::getAllyTargets(hovered_actor, selectable_targets);

    for(auto& target : hovered_party)
      if(target)
        target->setFlag(ActorState::MENU_HOVERED, true);
  }
  else if(selected_action_scope == ActionScope::ALL_NOT_USER)
  {
    for(auto& target : selectable_targets)
      if(target != actor)
        target->setFlag(ActorState::MENU_HOVERED, true);
  }
}

std::vector<BattleActor*> BattleMenu::getTargetsSelected()
{
  return selected_targets;
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

bool BattleMenu::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;

  return this->event_handler;
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

void BattleMenu::setSelectableTypes(std::vector<ActionType> valid_action_types)
{
  this->valid_action_types = valid_action_types;
}

void BattleMenu::setSelectableSkills(std::vector<BattleSkill*> menu_skills)
{
  this->valid_battle_skills = menu_skills;
}

void BattleMenu::setSelectableItems(std::vector<BattleItem*> menu_items)
{
  this->valid_battle_items = menu_items;
}

void BattleMenu::setWindowStatus(WindowStatus status_window)
{
  this->status_window = status_window;
}

/*=============================================================================
 * PUBLIC FUNCTIONS - RENDERING
 *============================================================================*/

bool BattleMenu::createItemFrames(uint32_t width_left, uint32_t width_right)
{
  assert(renderer && config);

  bool success{true};
  uint32_t text_height{0};
  uint32_t text_width{width_left - kTYPE_MARGIN * 8};
  Text t = Text(config->getFontTTF(FontName::BATTLE_HEADER));
  Text t2 = Text(config->getFontTTF(FontName::BATTLE_HEADER));
  uint32_t count_x{text_width};

  clearItemFrames();

  for(auto& battle_item : valid_battle_items)
  {
    auto item = battle_item->item;

    frames_item_name.push_back(new Frame());
    frames_item_info.push_back(new Frame());

    /* Set up the name colours for the item name display */
    if(battle_item->valid_status == ValidStatus::VALID)
      success &= t.setText(renderer, item->getName(), kTEXT_STANDARD);
    else if(battle_item->valid_status == ValidStatus::NO_TARGETS)
      success &= t.setText(renderer, item->getName(), kTEXT_NO_TARGETS);
    else if(battle_item->valid_status == ValidStatus::ZERO_COUNT)
      success &= t.setText(renderer, item->getName(), kTEXT_PRICEY);

    /* Set up the count for render */
    if(battle_item->valid_status == ValidStatus::VALID)
    {
      success &= t2.setText(renderer, std::to_string(battle_item->amount),
                            kTEXT_STANDARD);
    }
    else
    {
      success &= t2.setText(renderer, std::to_string(battle_item->amount),
                            kTEXT_INVALID);
    }

    /* Text height */
    if(text_height == 0)
      text_height = t.getHeight() + kTYPE_MARGIN * 2;

    /* Create rendering texture */
    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, text_width, text_height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* Render the text */
    success &= t.render(renderer, 0, kTYPE_MARGIN);
    success &=
        t2.render(renderer, count_x - t2.getWidth() - kSKILL_SEP, kTYPE_MARGIN);

    /* Set texture and clear render target back to main */
    frames_item_name.back()->setTexture(texture);
    SDL_SetRenderTarget(renderer, nullptr);

    /* Create the detailed skill information for this skill */
    auto info_texture = createItemFrame(
        battle_item, width_right - kTYPE_MARGIN * 2 - kBIGBAR_R_OFFSET,
        kBIGBAR_OFFSET + kBIGBAR_CHOOSE - kMENU_SEPARATOR_T -
            kMENU_SEPARATOR_B);

    frames_item_info.back()->setTexture(info_texture);
  }

  if(success)
    setFlag(BattleMenuState::ITEM_FRAMES_BUILT, true);

  return true;
}

// TODO: Colours/grayness for various BattleSkill validness? For items too
bool BattleMenu::createSkillFrames(uint32_t width_left, uint32_t width_right)
{
  /* A renderer and configuration must be assigned */
  assert(renderer && config);

  bool success{true};
  uint32_t text_height{0};
  uint32_t text_width{width_left - kTYPE_MARGIN * 8};
  Text t = Text(config->getFontTTF(FontName::BATTLE_HEADER));
  Text t2 = Text(config->getFontTTF(FontName::BATTLE_HEADER));
  auto frame_qd = battle_display_data->getFrameQD();
  uint32_t qd_x{text_width - frame_qd->getWidth()};

  /* Delete frames for skills if skills are already rendered */
  clearSkillFrames();

  for(auto& skill : valid_battle_skills)
  {
    frames_skill_name.push_back(new Frame());
    frames_skill_info.push_back(new Frame());

    /* ValidStatus of the Battle skill -> reflects the color of skil.frm */
    if(skill->valid_status == ValidStatus::VALID)
      success &= t.setText(renderer, skill->skill->getName(), kTEXT_STANDARD);
    else if(skill->valid_status == ValidStatus::NOT_AFFORDABLE)
      success &= t.setText(renderer, skill->skill->getName(), kTEXT_PRICEY);
    else if(skill->valid_status == ValidStatus::SILENCED)
      success &= t.setText(renderer, skill->skill->getName(), kTEXT_INVALID);
    else if(skill->valid_status == ValidStatus::NO_TARGETS)
      success &= t.setText(renderer, skill->skill->getName(), kTEXT_NO_TARGETS);

    /* Render cost */
    if(skill->valid_status == ValidStatus::VALID)
    {
      success &= t2.setText(renderer, std::to_string(skill->skill->getCost()),
                            kTEXT_STANDARD);
    }
    else
    {
      success &= t2.setText(renderer, std::to_string(skill->skill->getCost()),
                            kTEXT_INVALID);
    }

    /* Text height */
    if(text_height == 0)
      text_height = t.getHeight() + kTYPE_MARGIN * 2;

    /* Create rendering texture */
    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, text_width, text_height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* Render the text */
    success &= t.render(renderer, 0, kTYPE_MARGIN);

    /* Render the QD */
    if(skill->valid_status != ValidStatus::VALID)
      frame_qd->setAlpha(128);
    else
      frame_qd->setAlpha(255);
    success &= frame_qd->render(renderer, qd_x, kTYPE_MARGIN + 1);

    /* Render the cost */
    success &=
        t2.render(renderer, qd_x - t2.getWidth() - kSKILL_SEP, kTYPE_MARGIN);

    /* Set texture and clear render target back to main */
    frames_skill_name.back()->setTexture(texture);
    SDL_SetRenderTarget(renderer, nullptr);

    /* Create the detailed skill information for this skill */
    auto info_texture = createSkillFrame(
        skill, width_right - kTYPE_MARGIN * 2 - kBIGBAR_R_OFFSET,
        kBIGBAR_OFFSET + kBIGBAR_CHOOSE - kMENU_SEPARATOR_T -
            kMENU_SEPARATOR_B);

    frames_skill_info.back()->setTexture(info_texture);
  }

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
    uint32_t screen_width{config->getScreenWidth()};
    uint32_t screen_height{config->getScreenHeight()};
    uint32_t bar_height{kBIGBAR_OFFSET + kBIGBAR_CHOOSE};
    uint32_t section1_w{(uint32_t)(screen_width * kBIGBAR_L)};
    uint32_t section2_w{(uint32_t)(screen_width * kBIGBAR_M1)};

    /* Render separator */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    SDL_Rect rect;
    rect.x = section1_w;
    rect.y = screen_height - bar_height + kMENU_SEPARATOR_T;
    rect.w = 1;
    rect.h = screen_height - rect.y - kMENU_SEPARATOR_B;
    SDL_RenderFillRect(renderer, &rect);

    /* Render the second separator */
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

      /* Render the skills/items, depending on category */
      if(selected_action_type == ActionType::SKILL)
      {
        success &= renderSkills(rect2.x, rect2.y, section3_w, rect3.h);

        if((uint32_t)element_index < frames_skill_info.size())
        {
          success &= frames_skill_info[element_index]->render(
              renderer, rect3.x + kTYPE_MARGIN, rect3.y);
        }
      }
      if(selected_action_type == ActionType::ITEM)
      {
        success &= renderItems(rect2.x, rect2.y, section3_w, rect3.h);

        if((uint32_t)element_index < frames_item_info.size())
        {
          success &= frames_item_info[element_index]->render(
              renderer, rect3.x + kTYPE_MARGIN, rect3.y);
        }
      }
      else
      {
        success = false;
      }
    }
  }

  return success;
}

void BattleMenu::keyDownSelect()
{
  event_handler->triggerSound(Sound::kID_SOUND_MENU_NEXT, SoundChannels::MENUS);

  if(menu_layer == BattleMenuLayer::TYPE_SELECTION)
  {
    if(element_index <= getMaxIndex())
    {
      selected_action_type = valid_action_types.at(element_index);

      if(selected_action_type == ActionType::ITEM &&
         valid_battle_items.size() == 0)
      {
        // TODO: Fire error sound.
        selected_action_type = ActionType::NONE;
      }
    }

    if(selected_action_type == ActionType::PASS)
    {
      setFlag(BattleMenuState::SELECTION_COMPLETE);
    }
    else if(selected_action_type != ActionType::NONE)
    {
      menu_layer = BattleMenuLayer::ACTION_SELECTION;
      element_index = validFirst();
    }
  }
  else if(menu_layer == BattleMenuLayer::ACTION_SELECTION)
  {
    if(selected_action_type == ActionType::SKILL)
    {
      if((uint32_t)element_index < valid_battle_skills.size())
        selected_battle_skill = valid_battle_skills.at(element_index);
    }
    else if(selected_action_type == ActionType::ITEM)
    {
      if((uint32_t)element_index < valid_battle_items.size())
        selected_battle_item = valid_battle_items.at(element_index);
    }

    if(selected_action_type == ActionType::SKILL ||
       selected_action_type == ActionType::ITEM)
    {

      if(selected_action_type == ActionType::SKILL)
        selected_action_scope = selected_battle_skill->skill->getScope();
      else if(selected_action_type == ActionType::ITEM)
      {
        auto skill = selected_battle_item->item->getUseSkill();
        selected_action_scope = skill->getScope();
      }

      menu_layer = BattleMenuLayer::TARGET_SELECTION;

      /* If the selected scope is the user, then the EI */
      if(selected_action_scope == ActionScope::USER)
        element_index = elementIndexOfActor(actor);
      else
        element_index = validFirst();

      setHoverTargets();
    }
  }
  else if(menu_layer == BattleMenuLayer::TARGET_SELECTION)
  {
    if(selected_action_scope != ActionScope::TWO_ALLIES &&
       selected_action_scope != ActionScope::TWO_ENEMIES)
    {
      selected_targets = getTargetsHovered();
      setFlag(BattleMenuState::SELECTION_COMPLETE);
    }
  }
}

/*=============================================================================
 * PRIVATE STATIC FUNCTIONS
 *============================================================================*/

// Assumes ordered in decrementing order
BattleActor* BattleMenu::nextMenuIndex(int32_t curr,
                                       std::vector<BattleActor*> selectable)
{
  for(const auto& target : selectable)
    if(target && target->getMenuIndex() < curr)
      return target;

  return nullptr;
}

// Assumes ordered in decrementing order
BattleActor* BattleMenu::prevMenuIndex(int32_t curr,
                                       std::vector<BattleActor*> selectable)
{
  for(auto it = selectable.rbegin(); it != selectable.rend(); ++it)
    if((*it) && (*it)->getMenuIndex() > curr)
      return *it;

  return nullptr;
}
