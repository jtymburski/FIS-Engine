/*******************************************************************************
* Class Name: BattleActor [Implementation]
* Date Created: June 22, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/Battle/BattleActor.h"
#include "Game/Battle/AIModule.h"
#include "Game/Battle/BattleStats.h"
#include "Game/Player/Ailment.h"
#include "Game/Player/Person.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const float BattleActor::kVELOCITY_X{-1.550};
const SDL_Color BattleActor::kFLASHING_DAMAGE_COLOR{225, 10, 10, 65};
const SDL_Color BattleActor::kFLASHING_POISON_COLOR{84, 152, 80, 65};
const SDL_Color BattleActor::kFLASHING_INFLICT_COLOR{96, 88, 133, 65};
const SDL_Color BattleActor::kFLASHING_KO_COLOR{200, 20, 20, 105};
const SDL_Color BattleActor::kFLASHING_RELIEVE_COLOR{200, 200, 200, 125};

const float BattleActor::kREGEN_RATE_ZERO_PC = 0.00;
const float BattleActor::kREGEN_RATE_WEAK_PC = 0.03;
const float BattleActor::kREGEN_RATE_NORMAL_PC = 0.04;
const float BattleActor::kREGEN_RATE_STRONG_PC = 0.05;
const float BattleActor::kREGEN_RATE_GRAND_PC = 0.06;

const uint8_t BattleActor::kACTOR_KO_ALPHA{50};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
* Description:
 *
 * Inputs:
 */
BattleActor::BattleActor(Person* person_base, int32_t battle_index,
                         int32_t menu_index, bool is_ally, bool can_run)
    : ai{nullptr},
      action_element{ActionElement()},
      active_sprite{ActiveSprite::NONE},
      battle_index{battle_index},
      flags{static_cast<ActorState>(0)},
      frame_info{nullptr},
      menu_index{menu_index},
      person_base{person_base},
      sprite_ally{nullptr},
      sprite_ally_defensive{nullptr},
      sprite_ally_offensive{nullptr},
      sprite_dialog{nullptr},
      sprite_face{nullptr},
      sprite_foe{nullptr},
      sprite_foe_defensive{nullptr},
      sprite_foe_offensive{nullptr},
      state_active_sprite{SpriteState::HIDDEN},
      state_death_fade{FadeState::NOT_SHOWN},
      state_elapsed_time{0},
      state_flashing{FlashingState()},
      state_guarding{GuardingState::NONE},
      state_living{LivingState::REMOVED},
      state_selection{SelectionState::NOT_SELECTED},
      state_upkeep{UpkeepState::COMPLETE},
      stats_actual{AttributeSet(), AttributeSet()},
      stats_rendered{AttributeSet(), AttributeSet()},
      temp_alpha{255},
      dialog_x{0},
      dialog_y{0}
{
  /* The base person must be a non-nullptr person */
  assert(person_base);

  /* Prepare the person for a Battle */
  battleSetup(is_ally, can_run);
  createSprites();
}

/*
* Description:
 *
 * Inputs:
 */
BattleActor::BattleActor(Person* person_base)
    : BattleActor(person_base, 0, 0, true, false)
{
}

/*
 * Description: Annihilates a BattleActor object
 */
BattleActor::~BattleActor()
{
  clearActionFrame();
  clearAilments();
  clearBattleItems();
  clearBattleSkills();
  clearInfoFrame();
  clearFlashing();
  clearSprites();

  if(ai)
  {
    delete ai;
    ai = nullptr;
  }
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleActor::battleSetup(bool is_ally, bool can_run)
{
  /* Calculate the Signature statistics */
  // auto sig_stats = person_base->calcSignatureStats();
  // TOOD [10-08-16] Implement Personal Signature

  auto curr_stats = person_base->getCurr();


  // std::cout << "Current stats for: " << getBasePerson()->getName() << std::endl;
  // curr_stats.print();
  std::cout << std::endl;

  stats_actual = BattleStats(curr_stats, curr_stats);
  stats_rendered = BattleStats(curr_stats, curr_stats);

  /* If the person's current VITA is >0, they are not KO'd */
  if(person_base->getCurr().getStat(Attribute::VITA) > 0)
    setStateLiving(LivingState::ALIVE);

  setFlag(ActorState::REVIVABLE, true);
  setFlag(ActorState::RUN_ENABLED, true);

  /* Set up for allies */
  if(is_ally)
  {
    setFlag(ActorState::ALLY, true);
    active_sprite = ActiveSprite::ALLY;
  }
  /* Set up for enemies */
  else
  {
    setFlag(ActorState::ALLY, false);
    active_sprite = ActiveSprite::FOE;
  }

  /* Reset the action types useable by the battle actor */
  resetActionTypes();

  ai = new AIModule();
  ai->setParent(this);

  // TODO
  (void)can_run;
}

void BattleActor::clearAilments()
{
  for(auto& clear_ailment : ailments)
  {
    if(clear_ailment)
      delete clear_ailment;
    clear_ailment = nullptr;
  }

  ailments.clear();
}

void BattleActor::clearBattleItems()
{
  for(auto& battle_item : battle_items)
  {
    if(battle_item)
      delete battle_item;
    battle_item = nullptr;
  }

  battle_items.clear();
}

void BattleActor::clearBattleSkills()
{
  for(auto& battle_skill : battle_skills)
  {
    if(battle_skill)
      delete battle_skill;
    battle_skill = nullptr;
  }

  battle_skills.clear();
}

void BattleActor::clearActionFrame()
{
  if(action_element.frame_action)
    delete action_element.frame_action;
  action_element.frame_action = nullptr;
}

void BattleActor::clearInfoFrame()
{
  if(frame_info)
    delete frame_info;
  frame_info = nullptr;
}

void BattleActor::clearFlashing()
{
  if(state_flashing.element)
    delete state_flashing.element;

  state_flashing.element = nullptr;
}

void BattleActor::clearSprites()
{
  if(sprite_ally)
    delete sprite_ally;
  if(sprite_ally_defensive)
    delete sprite_ally_defensive;
  if(sprite_ally_offensive)
    delete sprite_ally_offensive;
  if(sprite_dialog)
    delete sprite_dialog;
  if(sprite_face)
    delete sprite_face;
  if(sprite_foe_defensive)
    delete sprite_foe_defensive;
  if(sprite_foe_offensive)
    delete sprite_foe_offensive;

  sprite_ally = nullptr;
  sprite_ally_defensive = nullptr;
  sprite_ally_offensive = nullptr;
  sprite_dialog = nullptr;
  sprite_face = nullptr;
  sprite_foe_defensive = nullptr;
  sprite_foe_offensive = nullptr;
}

// Construct copies of the base person's sprites for each sprite needed (if
// available) for the battle actor.
void BattleActor::createSprites()
{
  if(person_base)
  {
    if(person_base->getSpriteAlly())
      sprite_ally = new Sprite(*(person_base->getSpriteAlly()));

    if(person_base->getSpriteAllyDefensive())
    {
      sprite_ally_defensive =
          new Sprite(*(person_base->getSpriteAllyDefensive()));
    }

    if(person_base->getSpriteAllyOffensive())
    {
      sprite_ally_offensive =
          new Sprite(*(person_base->getSpriteAllyOffensive()));
    }

    /* Construct the dialog sprite */
    if(person_base->getSpriteDialog())
    {
      dialog_x = person_base->getActionX();
      dialog_y = person_base->getActionY();

      sprite_dialog = new Sprite(*(person_base->getSpriteDialog()));
    }

    /* Construct the space sprite */
    if(person_base->getSpriteFace())
      sprite_face = new Sprite(*(person_base->getSpriteDialog()));

    if(person_base->getSpriteFoe())
    {
      sprite_foe = new Sprite(*(person_base->getSpriteFoe()));
    }
    if(person_base->getSpriteFoeDefensive())
    {
      sprite_foe_defensive =
          new Sprite(*(person_base->getSpriteFoeDefensive()));
    }

    if(person_base->getSpriteFoeOffensive())
    {
      sprite_foe_offensive =
          new Sprite(*(person_base->getSpriteFoeOffensive()));
    }
  }
}

bool BattleActor::resetActionTypes()
{
  if(person_base && person_base->getClass())
  {
    // TODO [09-06-15] Action types temporarily offline.
    setFlag(ActorState::ITM_ENABLED, false); // Build item functionality
    setFlag(ActorState::INS_ENABLED, false); // Build inspect functionality
    setFlag(ActorState::RUN_ENABLED, false); // Get from enemy party
    setFlag(ActorState::GRD_ENABLED, false); // Get from category

    /* Build defend and implode based on category */
    setFlag(ActorState::DEF_ENABLED,
            person_base->getClass()->getFlag(CategoryState::DEF_ENABLED));
    setFlag(ActorState::IMP_ENABLED,
            person_base->getClass()->getFlag(CategoryState::IMP_ENABLED));

    /* Everyone can pass */
    setFlag(ActorState::PAS_ENABLED, true);

    return true;
  }

  return false;
}

void BattleActor::updateActionElement(int32_t cycle_time)
{
  auto delta_x = std::floor(cycle_time * kVELOCITY_X);

  /* Approach the end position */
  auto& end = action_element.position_end;
  auto& start = action_element.position_start;
  auto& curr = action_element.position_curr;

  if(action_element.element_state == SpriteState::SLIDING_IN)
  {
    curr.x += delta_x;

    if(curr.x < end.x)
    {
      curr.x = end.x;
      action_element.element_state = SpriteState::SLID_IN;
    }
  }
  /* Approach the begin position */
  else if(action_element.element_state == SpriteState::SLIDING_OUT)
  {
    curr.x -= delta_x;

    if(curr.x > start.x)
    {
      curr.x = start.x;
      action_element.element_state = SpriteState::SLID_OUT;
    }
  }
}

void BattleActor::updateSpriteBobbing(int32_t cycle_time)
{
  (void)cycle_time; // TODO: Warning
  // Change the getActorX in battle to be in the battle actor class.
  // This will allow the getActorX to include the pass offset for
  // battle to render.
}

void BattleActor::updateSpriteFlashing(int32_t cycle_time)
{
  if(state_flashing.element && getActiveSprite())
  {
    state_flashing.element->update(cycle_time);

    if(state_flashing.element->time_left > 0)
    {
      auto color = getFlashingColor(state_flashing.flashing_type);
      auto alpha = state_flashing.element->alpha;

      if(state_flashing.element->render_type == RenderType::RGB_SPRITE_DEATH)
        updateSpriteKO(cycle_time);

      getActiveSprite()->setColorBalance(Helpers::calcColorRed(color, alpha),
                                         Helpers::calcColorGreen(color, alpha),
                                         Helpers::calcColorBlue(color, alpha));
    }
    else
    {
      endFlashing();
    }
  }
}

void BattleActor::updateSpriteKO(int32_t cycle_time)
{
  if(getActiveSprite())
  {
    auto c_opacity = getActiveSprite()->getOpacity();
    auto d_opacity = std::round((float)cycle_time / 15.0);

    if(c_opacity > kACTOR_KO_ALPHA && c_opacity - d_opacity >= 0)
      getActiveSprite()->setOpacity(c_opacity - d_opacity);
    else if(c_opacity < kACTOR_KO_ALPHA && c_opacity + d_opacity <= 255)
      getActiveSprite()->setOpacity(c_opacity + d_opacity);
  }
}

void BattleActor::updateStats(int32_t cycle_time)
{
  int32_t vita = stats_rendered.getBaseValue(Attribute::VITA);
  int32_t act_vita = stats_actual.getBaseValue(Attribute::VITA);
  int32_t qtdr = stats_rendered.getBaseValue(Attribute::QTDR);
  int32_t act_qtdr = stats_actual.getBaseValue(Attribute::QTDR);

  auto vita_diff = std::ceil(std::abs(vita - act_vita) / 100);
  auto qtdr_diff = std::ceil(std::abs(qtdr - act_qtdr) / 100);
  auto vita_amt = (uint32_t)((1 + vita_diff) * (cycle_time / 15));
  auto qtdr_amt = (uint32_t)((1 + qtdr_diff) * (cycle_time / 15));

  if(vita > act_vita)
  {
    stats_rendered.setBaseValue(Attribute::VITA,
                                std::max((uint32_t)1, vita - vita_amt));
  }
  else if(vita < act_vita)
  {
    stats_rendered.setBaseValue(Attribute::VITA,
                                std::max((uint32_t)1, vita + vita_amt));
  }

  if(qtdr > act_qtdr)
  {
    stats_rendered.setBaseValue(Attribute::QTDR,
                                std::max((uint32_t)1, qtdr - qtdr_amt));
  }
  else if(qtdr < act_qtdr)
  {
    stats_rendered.setBaseValue(Attribute::QTDR,
                                std::max((uint32_t)1, qtdr + qtdr_amt));
  }

  if(vita == 1 && act_vita == 0)
    stats_rendered.setBaseValue(Attribute::VITA, 0);
  if(qtdr == 1 && act_qtdr == 0)
    stats_rendered.setBaseValue(Attribute::QTDR, 0);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void BattleActor::addAilment(Infliction type, int32_t min_turns,
                             int32_t max_turns, double chance)
{
  auto new_ailment =
      new Ailment(type, &stats_actual, person_base->getPrimary(),
                  person_base->getSecondary(), min_turns, max_turns, chance);

  if(Ailment::getClassOfInfliction(type) == AilmentClass::BUFF)
    new_ailment->applyBuffs();

  ailments.push_back(new_ailment);
}

bool BattleActor::buildBattleItems(Inventory* inv,
                                   std::vector<BattleActor*> a_targets)
{
  bool success = true;
  clearBattleItems();

  if(inv && person_base)
  {
    auto items = inv->getItems();

    for(auto& item : items)
    {
      if(item.first)
      {
        auto skill = item.first->getUseSkill();

        if(skill && (item.first->getOccasion() == ActionOccasion::BATTLE ||
                     item.first->getOccasion() == ActionOccasion::ALWAYS))
        {
          auto targets =
              getTargetsFromScope(this, skill->getScope(), a_targets);
          auto battle_item = new BattleItem();

          battle_item->valid_status = ValidStatus::VALID;
          battle_item->item = item.first;
          battle_item->amount = item.second;
          battle_item->targets = targets;

          if(item.second == 0)
            battle_item->valid_status = ValidStatus::ZERO_COUNT;

          battle_items.push_back(battle_item);

          success &= true;
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

bool BattleActor::buildBattleSkills(std::vector<BattleActor*> a_targets)
{
  /* Updates the base person skills */
  person_base->updateBaseSkills();

  /* Clear BattleSkills if they were previously created */
  clearBattleSkills();

  bool success = person_base;

  if(success)
  {
    auto curr_skills = person_base->getCurrSkills();
    auto useable_skills = curr_skills->getElements(person_base->getLevel());

    for(auto& element : useable_skills)
    {
      auto battle_skill = new BattleSkill();
      battle_skill->skill = element.skill;

      if(battle_skill->skill)
      {
        auto skill = battle_skill->skill;
        auto targets = getTargetsFromScope(this, skill->getScope(), a_targets);

        battle_skill->targets = targets;
        battle_skill->true_cost = getSkillCost(skill);

        bool is_valid{!battle_skill->targets.empty()};

        if(is_valid)
        {
          battle_skill->valid_status = ValidStatus::VALID;

          if(battle_skill->true_cost > stats_actual.getValue(Attribute::QTDR))
            battle_skill->valid_status = ValidStatus::NOT_AFFORDABLE;
          if(isInflicted(Infliction::SILENCE) && battle_skill->true_cost > 0)
            battle_skill->valid_status = ValidStatus::SILENCED;
        }
        else
        {
          battle_skill->valid_status = ValidStatus::NO_TARGETS;
        }

        battle_skills.push_back(battle_skill);
      }
      else
      {
        delete battle_skill;
        battle_skill = nullptr;
      }
    }
  }

  return success;
}

void BattleActor::cleanUpForTurn()
{
  setFlag(ActorState::SELECTION_RANDOM, false);
  setFlag(ActorState::SELECTION_SKIP, false);
}

bool BattleActor::dealDamage(int32_t damage_amount)
{
  auto curr_vita = stats_actual.getValue(Attribute::VITA);
  auto curr_base_vita = stats_actual.getBaseValue(Attribute::VITA);

  if(curr_vita != curr_base_vita)
  {
    std::cout << "[WARNING] : Damage to base value with modified curr vita."
              << std::endl;
  }

  std::cout << "Dealing " << damage_amount << " damage with " << curr_vita
            << " current vita." << std::endl;
  if(damage_amount >= (int32_t)curr_vita)
  {
    stats_actual.setBaseValue(Attribute::VITA, 0);

    if(state_living == LivingState::ALIVE)
      state_living = LivingState::KO;

    return true;
  }

  stats_actual.setBaseValue(Attribute::VITA, curr_vita - damage_amount);

  return false;
}

void BattleActor::dealQtdr(int32_t dealt_amount)
{
  (void)dealt_amount; // TODO
}

void BattleActor::restoreQtdr(int32_t amount)
{
  auto curr_value = stats_actual.getBaseValue(Attribute::QTDR);
  auto max_value = stats_actual.getBaseValue(Attribute::MQTD);

  if(amount > 0 && amount + curr_value <= max_value)
    stats_actual.setBaseValue(Attribute::QTDR, curr_value + amount);
}

void BattleActor::restoreVita(int32_t amount)
{
  auto curr_value = stats_actual.getBaseValue(Attribute::VITA);
  auto max_value = stats_actual.getBaseValue(Attribute::MVIT);

  std::cout << "--- Restoring ---: " << amount << std::endl;
  std::cout << "----- With current: " << curr_value << std::endl;
  std::cout << "----- Max value:    " << max_value << std::endl;

  if(amount > 0 && amount + curr_value <= max_value)
    stats_actual.setBaseValue(Attribute::VITA, curr_value + amount);
}

void BattleActor::endFlashing()
{
  clearFlashing();
  auto koing = (state_flashing.flashing_type == FlashingType::KOING);

  if(getActiveSprite() && !koing)
  {
    getActiveSprite()->revertColorBalance();
    state_active_sprite = SpriteState::SLID_IN;
  }
  else if(getActiveSprite() && koing)
  {
    state_active_sprite = SpriteState::KOED;
  }

  state_flashing = FlashingState();
}

/* Checks whether the BattleActor would be immune to a given Infliction */
bool BattleActor::isImmune(Infliction type)
{
  /* Check Battle Class Immunity */
  if(person_base && person_base->getClass())
    if(person_base->getClass()->isImmune(type))
      return true;

  /* Check Race Immunity */
  if(person_base && person_base->getRace())
    if(person_base->getRace()->isImmune(type))
      return true;

  return false;
}

bool BattleActor::isInflicted(Infliction test_infliction)
{
  for(const auto& ailment : ailments)
    if(ailment && ailment->getType() == test_infliction)
      return true;

  return false;
}

Ailment* BattleActor::nextUpdateAilment()
{
  for(auto& ailment : ailments)
  {
    if(ailment && ailment->getUpdateStatus() == AilmentStatus::INCOMPLETE)
      return ailment;
  }

  return nullptr;
}

// TODO: [09-06-15] Removing ailments. Corner cases?

void BattleActor::clearAilment(Ailment* remove_ailment)
{
  delete remove_ailment;
  remove_ailment = nullptr;
}

bool BattleActor::removeAilment(Ailment* remove_ailment)
{
  if(remove_ailment)
  {
    /* Remove the modifier from the BattleStats, if a buff */
    auto ail_class = Ailment::getClassOfInfliction(remove_ailment->getType());
    if(ail_class == AilmentClass::BUFF)
      stats_actual.removeLinked(remove_ailment);

    ailments.erase(
        std::remove_if(ailments.begin(), ailments.end(),
                       [&](Ailment* a) -> bool { return a == remove_ailment; }),
        ailments.end());

    return true;
  }

  return false;
}

void BattleActor::removeAilmentsKO()
{
  for(auto& ailment : ailments)
  {
    if(ailment && ailment->getFlag(AilState::CURABLE_KO))
    {
      delete ailment;
      ailment = nullptr;
    }

    /* TODO: Deal with corner cases for removing ailments */
  }

  ailments.clear();
}

/* Sets up start of flashingness */
void BattleActor::startFlashing(FlashingType flashing_type, int32_t time_left)
{
  clearFlashing();

  auto active_sprite = getActiveSprite();

  active_sprite->revertColorBalance();
  auto color = getFlashingColor(flashing_type);

  if(active_sprite)
  {
    state_active_sprite = SpriteState::FLASHING;
    active_sprite->setTempColorBalance(active_sprite->getColorRed(),
                                       active_sprite->getColorGreen(),
                                       active_sprite->getColorBlue());
  }

  state_flashing.flashing_type = flashing_type;
  state_flashing.is_flashing = true;

  if(!state_flashing.element)
  {
    state_flashing.element = new RenderElement();

    if(flashing_type != FlashingType::KOING)
      state_flashing.element->createAsSpriteFlash(color, time_left);
    else if(flashing_type == FlashingType::KOING)
      state_flashing.element->createAsSpriteDeath(color, 3000, 500, 2000);
  }
}

void BattleActor::turnSetup()
{
  state_selection = SelectionState::NOT_SELECTED;

  if(ai)
  {
    // person_base->getAI()->incrementTurns();
    // person_base->getAI()->resetForNewTurn(person_base);
  }
}

bool BattleActor::update(int32_t cycle_time)
{
  updateActionElement(cycle_time);
  updateStats(cycle_time);

  if(state_active_sprite == SpriteState::FLASHING)
    updateSpriteFlashing(cycle_time);
  else if(state_active_sprite == SpriteState::BOBBING)
    updateSpriteBobbing(cycle_time);

  if(getActiveSprite())
    getActiveSprite()->update(cycle_time);

  return true;
}

// void BattleActor::updateSpriteGeneral(int32_t cycle_time)
// {
//   if(getActiveSprite())
//   {
//     if(getFlag(ActorState::IS_CURR_TARGET))
//       getActiveSprite()->setOpacity(80);
//     else if(getFlag(ActorState::IS_CURR_USER))
//       getActiveSprite()->setOpacity(120);
//     else if(getFlag(ActorState::IS_OTHER))
//       getActiveSprite()->setOpacity(255);
//   }
// }

Frame* BattleActor::getActionFrame()
{
  return action_element.frame_action;
}

int32_t BattleActor::getActionFrameX()
{
  return action_element.position_curr.x;
}

int32_t BattleActor::getActionFrameY()
{
  return action_element.position_curr.y;
}

Sprite* BattleActor::getActiveSprite()
{
  /* Grab the defensive ally sprite, unless it's nullptr */
  if(active_sprite == ActiveSprite::ALLY_DEFENSIVE && sprite_ally_defensive)
    return sprite_ally_defensive;
  else if(active_sprite == ActiveSprite::ALLY_DEFENSIVE)
    return sprite_ally;

  /* Grab the offensive ally sprite, unless it's nullptr */
  if(active_sprite == ActiveSprite::ALLY_OFFENSIVE && sprite_ally_offensive)
    return sprite_ally_offensive;
  else if(active_sprite == ActiveSprite::ALLY_OFFENSIVE)
    return sprite_ally;

  if(active_sprite == ActiveSprite::ALLY)
    return sprite_ally;

  /* Grab the defensive foe sprite, unless it's nullptr */
  if(active_sprite == ActiveSprite::FOE_DEFENSIVE && sprite_foe_defensive)
    return sprite_foe_defensive;
  else if(active_sprite == ActiveSprite::FOE_DEFENSIVE)
    return sprite_foe;

  /* Grab the offensive foe sprite, unless it's nullptr */
  if(active_sprite == ActiveSprite::FOE_OFFENSIVE && sprite_foe_offensive)
    return sprite_foe_offensive;
  else if(active_sprite == ActiveSprite::FOE_OFFENSIVE)
    return sprite_foe;

  if(active_sprite == ActiveSprite::FOE)
    return sprite_foe;

  return nullptr;
}

std::string BattleActor::getDisplayName()
{
  if(person_base)
    auto display_name = person_base->getNameDisplay();

  return "";
}

std::vector<ActionType> BattleActor::getValidActionTypes()
{
  resetActionTypes();

  std::vector<ActionType> valid_types;

  valid_types.push_back(ActionType::SKILL);

  // TODO: ITM_ENABLED flag
  valid_types.push_back(ActionType::ITEM);

  if(getFlag(ActorState::PAS_ENABLED))
    valid_types.push_back(ActionType::PASS);
  // if(getFlag(ActorState::RUN_ENABLED))
  //  valid_types.push_back(ActionType::RUN);

  // TODO: Other action types [10-04-15]

  return valid_types;
}

AIModule* BattleActor::getAI()
{
  return ai;
}

std::vector<Ailment*> BattleActor::getAilments()
{
  return ailments;
}

Person* BattleActor::getBasePerson()
{
  return person_base;
}

std::vector<BattleItem*> BattleActor::getBattleItems()
{
  return battle_items;
}

std::vector<BattleSkill*> BattleActor::getBattleSkills()
{
  return battle_skills;
}

bool BattleActor::getFlag(const ActorState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

int32_t BattleActor::getIndex()
{
  return battle_index;
}

int32_t BattleActor::getMenuIndex()
{
  return menu_index;
}

Frame* BattleActor::getInfoFrame()
{
  return frame_info;
}

Sprite* BattleActor::getDialogSprite()
{
  return sprite_dialog;
}

int32_t BattleActor::getDialogX()
{
  return dialog_x;
}

int32_t BattleActor::getDialogY()
{
  return dialog_y;
}

Sprite* BattleActor::getFaceSprite()
{
  return sprite_face;
}

GuardingState BattleActor::getGuardingState()
{
  return state_guarding;
}

SelectionState BattleActor::getSelectionState()
{
  return state_selection;
}

// TODO: Bubbification cost
// TODO: Halfcost infliction cost
uint32_t BattleActor::getSkillCost(Skill* test_skill)
{
  if(test_skill)
    return test_skill->getCost();

  return AttributeSet::getMaxValue();
}

SpriteState BattleActor::getStateActiveSprite()
{
  return state_active_sprite;
}

SpriteState BattleActor::getStateActionFrame()
{
  return action_element.element_state;
}

LivingState BattleActor::getStateLiving()
{
  return state_living;
}

UpkeepState BattleActor::getStateUpkeep()
{
  return state_upkeep;
}

BattleStats& BattleActor::getStats()
{
  return stats_actual;
}

BattleStats& BattleActor::getStatsRendered()
{
  return stats_rendered;
}

/* Returns the % of vitality the actor has available (rounded) */
uint32_t BattleActor::getPCVita()
{
  auto vita = stats_rendered.getValue(Attribute::VITA);
  auto max_vita = stats_rendered.getValue(Attribute::MVIT);
  float proportion = 0.0;

  if(max_vita != 0)
    proportion = (float)vita / (float)max_vita;

  return std::round(proportion * 100);
}

/* Returns the % of quantum drive the actor has available (rounded) */
uint32_t BattleActor::getPCQtdr()
{
  auto qtdr = stats_rendered.getValue(Attribute::QTDR);
  auto max_qtdr = stats_rendered.getValue(Attribute::MQTD);
  float proportion = 0.0;

  if(max_qtdr != 0)
    proportion = (float)qtdr / (float)max_qtdr;

  return std::ceil(proportion * 100);
}

float BattleActor::getRegenFactor(RegenRate regen_rate)
{
  if(regen_rate == RegenRate::WEAK)
    return kREGEN_RATE_WEAK_PC;
  else if(regen_rate == RegenRate::NORMAL)
    return kREGEN_RATE_NORMAL_PC;
  else if(regen_rate == RegenRate::STRONG)
    return kREGEN_RATE_STRONG_PC;
  else if(regen_rate == RegenRate::GRAND)
    return kREGEN_RATE_GRAND_PC;

  return kREGEN_RATE_ZERO_PC;
}

void BattleActor::setActionFrame(Frame* frame_action)
{
  clearActionFrame();

  action_element.frame_action = frame_action;
}

void BattleActor::setActionFrameEnd(int32_t new_x, int32_t new_y)
{
  action_element.position_end.x = new_x;
  action_element.position_end.y = new_y;
}

void BattleActor::setActionFrameStart(int32_t new_x, int32_t new_y)
{
  action_element.position_start.x = new_x;
  action_element.position_start.y = new_y;
  action_element.position_curr.x = new_x;
  action_element.position_curr.y = new_y;
}

void BattleActor::setStateActionFrame(SpriteState new_state)
{
  action_element.element_state = new_state;
}

void BattleActor::setStateLiving(LivingState new_state)
{
  state_living = new_state;
}

void BattleActor::setActiveSprite(ActiveSprite new_active_sprite)
{
  auto old_active_sprite = getActiveSprite();

  if(old_active_sprite)
    old_active_sprite->setAtFirst();

  active_sprite = new_active_sprite;
  auto sprite = getActiveSprite();

  if(sprite && old_active_sprite && state_flashing.is_flashing)
  {
    sprite->setTempColorBalance(old_active_sprite->getTempColorRed(),
                                old_active_sprite->getTempColorGreen(),
                                old_active_sprite->getTempColorBlue());

    sprite->setColorBalance(old_active_sprite->getColorRed(),
                            old_active_sprite->getColorGreen(),
                            old_active_sprite->getColorBlue());

    old_active_sprite->revertColorBalance();
  }

  /* Reset the animation timer and set for one loops for the active sprite
   * to play one loop, if it is set and not nullptr */
  if(active_sprite == ActiveSprite::ALLY_OFFENSIVE && sprite_ally_offensive)
  {
    sprite_ally_offensive->setNumLoops(1);
    sprite_ally_offensive->resetLoops();
  }
  else if(active_sprite == ActiveSprite::ALLY_DEFENSIVE &&
          sprite_ally_defensive)
  {
    sprite_ally_defensive->setFreezeAtTail(true);
    sprite_ally_defensive->setNumLoops(1);
    sprite_ally_defensive->resetLoops();
  }
  else if(active_sprite == ActiveSprite::FOE_OFFENSIVE && sprite_foe_offensive)
  {
    sprite_foe_offensive->setNumLoops(1);
    sprite_foe_offensive->resetLoops();
  }
  else if(active_sprite == ActiveSprite::FOE_DEFENSIVE && sprite_ally_defensive)
  {
    // sprite_ally_defensive->setFreezeAtTail(true);
    sprite_foe_defensive->setNumLoops(1);
    sprite_foe_defensive->resetLoops();
  }
}

void BattleActor::setFlag(ActorState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

void BattleActor::setGuardingState(GuardingState state_guarding)
{
  this->state_guarding = state_guarding;
}

void BattleActor::setInfoFrame(Frame* frame_info)
{
  clearInfoFrame();
  this->frame_info = frame_info;
}

void BattleActor::setSelectionState(SelectionState state_selection)
{
  this->state_selection = state_selection;
}

void BattleActor::setUpkeepState(UpkeepState state_upkeep)
{
  this->state_upkeep = state_upkeep;
}

SDL_Color BattleActor::getFlashingColor(FlashingType flashing_type)
{
  if(flashing_type == FlashingType::DAMAGE)
    return kFLASHING_DAMAGE_COLOR;
  else if(flashing_type == FlashingType::KOING)
    return kFLASHING_KO_COLOR;
  else if(flashing_type == FlashingType::POISON)
    return kFLASHING_POISON_COLOR;
  else if(flashing_type == FlashingType::RELIEVE)
    return kFLASHING_RELIEVE_COLOR;

  return kFLASHING_INFLICT_COLOR;
}

/* Grab all allied targets for given user */
std::vector<BattleActor*>
BattleActor::getAllyTargets(BattleActor* user,
                            std::vector<BattleActor*> targets)
{
  assert(user);
  std::vector<BattleActor*> valid_targets;

  for(const auto& target : targets)
  {
    if(target)
    {
      auto user_is_ally = user->getFlag(ActorState::ALLY);
      auto target_is_ally = target->getFlag(ActorState::ALLY);

      if(user_is_ally == target_is_ally)
        valid_targets.push_back(target);
    }
  }

  return valid_targets;
}

/* Grab all enemy targets for given user */
std::vector<BattleActor*>
BattleActor::getEnemyTargets(BattleActor* user,
                             std::vector<BattleActor*> targets)
{
  assert(user);
  std::vector<BattleActor*> valid_targets;

  for(const auto& target : targets)
  {
    if(target)
    {
      auto user_is_ally = user->getFlag(ActorState::ALLY);
      auto target_is_ally = target->getFlag(ActorState::ALLY);

      if(user_is_ally != target_is_ally)
        valid_targets.push_back(target);
    }
  }

  return valid_targets;
}

std::vector<BattleActor*>
BattleActor::getLivingTargets(std::vector<BattleActor*> targets)
{
  std::vector<BattleActor*> living_targets;

  for(const auto& target : targets)
    if(target && target->getStateLiving() == LivingState::ALIVE)
      living_targets.push_back(target);

  return living_targets;
}

std::vector<BattleActor*>
BattleActor::getRemovedUser(BattleActor* user,
                            std::vector<BattleActor*> targets)
{
  targets.erase(std::remove(begin(targets), end(targets), user), end(targets));

  return targets;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

std::vector<BattleActor*>
BattleActor::getTargetsFromScope(BattleActor* user, ActionScope scope,
                                 std::vector<BattleActor*> targets)
{
  assert(user);

  std::vector<BattleActor*> valid_targets;

  /* User Scope - only the user is selectable */
  if(scope == ActionScope::USER)
  {
    valid_targets.push_back(user);
  }
  /* One Target - any one of the valid targets */
  else if(scope == ActionScope::ONE_TARGET || scope == ActionScope::ONE_PARTY ||
          scope == ActionScope::ALL_TARGETS)
  {
    valid_targets = getLivingTargets(targets);
  }
  /* One Enemy - any enemy opponent whose index sign is opposite the user */
  else if(scope == ActionScope::ONE_ENEMY ||
          scope == ActionScope::TWO_ENEMIES ||
          scope == ActionScope::ALL_ENEMIES)
  {
    // std::cout << "Checking living on target size:" << targets.size()
    //           << std::endl;
    auto living_targets = getLivingTargets(targets);

    // std::cout << "Living targets size:" << living_targets.size() <<
    // std::endl;
    valid_targets = getEnemyTargets(user, living_targets);
    // std::cout << "Valid targets size: " << valid_targets.size() << std::endl;
  }
  else if(scope == ActionScope::ONE_ALLY || scope == ActionScope::TWO_ALLIES ||
          scope == ActionScope::ALL_ALLIES)
  {
    auto living_targets = getLivingTargets(targets);
    valid_targets = getAllyTargets(user, living_targets);
  }
  else if(scope == ActionScope::ONE_ALLY_NOT_USER)
  {
    auto living_targets = getLivingTargets(targets);
    auto ally_targets = getAllyTargets(user, living_targets);
    valid_targets = getRemovedUser(user, ally_targets);
  }
  else if(scope == ActionScope::ONE_ALLY_KO ||
          scope == ActionScope::ALL_ALLIES_KO)
  {
    auto allies = getAllyTargets(user, targets);
    auto living = getLivingTargets(targets);

    /* Subtract Living Targets */
    std::set_difference(begin(allies), end(allies), begin(living), end(living),
                        std::inserter(valid_targets, begin(valid_targets)));
  }
  else if(scope == ActionScope::NOT_USER || scope == ActionScope::ALL_NOT_USER)
  {
    auto living_targets = getLivingTargets(targets);
    valid_targets = getRemovedUser(user, living_targets);
  }

  return valid_targets;
}

int32_t BattleActor::calcTurnRegen(Attribute attr, int32_t outnumbered_val)
{
  float reg_fac = 0.0f;

  if(person_base && attr == Attribute::VITA)
    reg_fac = getRegenFactor(person_base->getVitaRegenRate());
  if(person_base && attr == Attribute::QTDR)
    reg_fac = getRegenFactor(person_base->getQDRegenRate());

  if(attr == Attribute::VITA && outnumbered_val != 0)
    reg_fac += (0.004 * outnumbered_val);

  int32_t max_attr_val = 0;
  int32_t amount = 0;
  int32_t max = 0;

  if(attr == Attribute::VITA)
  {
    max_attr_val = stats_actual.getValue(Attribute::MVIT);
    amount = reg_fac * max_attr_val;
    max = max_attr_val - stats_actual.getValue(Attribute::VITA);
  }
  else if(attr == Attribute::QTDR)
  {
    max_attr_val = stats_actual.getValue(Attribute::MQTD);
    amount = reg_fac * max_attr_val;
    max = max_attr_val - stats_actual.getValue(Attribute::QTDR);
  }

  /* Cannot return more than the maximum possible regeneration value */
  return std::min(max, amount);
}
