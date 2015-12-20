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
#include "Game/Player/Ailment.h"
#include "Game/Player/Person.h"
#include "Game/Battle/BattleActor.h"
#include "Game/Battle/BattleStats.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const float BattleActor::kVELOCITY_X{-1.550};
const SDL_Color BattleActor::kFLASHING_DAMAGE_COLOR{225, 10, 10, 85};
const SDL_Color BattleActor::kFLASHING_POISON_COLOR{84, 152, 80, 85};
const SDL_Color BattleActor::kFLASHING_INFLICT_COLOR{96, 88, 133, 85};
const SDL_Color BattleActor::kFLASHING_KO_COLOR{200, 20, 20, 115};
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
BattleActor::BattleActor(Person *person_base, int32_t battle_index,
                         bool is_ally, bool can_run, SDL_Renderer *renderer)
    : action_element{ActionElement()},
      active_sprite{ActiveSprite::NONE},
      battle_index{battle_index},
      flags{static_cast<ActorState>(0)},
      frame_info{nullptr},
      person_base{person_base},
      sprite_first_person{nullptr},
      sprite_third_person{nullptr},
      sprite_action{nullptr},
      sprite_dialog{nullptr},
      state_active_sprite{SpriteState::HIDDEN},
      state_death_fade{FadeState::NOT_SHOWN},
      state_elapsed_time{0},
      state_flashing{FlashingState()},
      state_guarding{GuardingState::NONE},
      state_living{LivingState::REMOVED},
      state_selection{SelectionState::NOT_SELECTED},
      state_upkeep{UpkeepState::COMPLETE},
      stats_actual{AttributeSet()},
      stats_rendered{AttributeSet()},
      temp_alpha{255},
      dialog_x{0},
      dialog_y{0}
{
  /* The base person must be a non-nullptr person */
  assert(person_base);

  /* Prepare the person for a Battle */
  battleSetup(is_ally, can_run);
  createSprites(renderer);
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
  /* Recalculate the current stats of the base person, create BattleStats */
  person_base->updateBaseStats();
  person_base->updateBaseSkills();

  auto equip_stats = person_base->calcEquipStats();

  stats_actual = BattleStats(equip_stats);
  stats_rendered = BattleStats(equip_stats);

  /* If the person's current VITA is >0, they are not KO'd */
  if(person_base->getCurr().getStat(Attribute::VITA) > 0)
    setStateLiving(LivingState::ALIVE);

  setFlag(ActorState::REVIVABLE, true);
  setFlag(ActorState::RUN_ENABLED, true);

  /* Set up for allies */
  if(is_ally)
  {
    setFlag(ActorState::ALLY, true);
    active_sprite = ActiveSprite::FIRST_PERSON;
  }
  /* Set up for enemies */
  else
  {
    setFlag(ActorState::ALLY, false);
    active_sprite = ActiveSprite::THIRD_PERSON;
  }

  /* Reset the action types useable by the battle actor */
  resetActionTypes();

  // TODO
  (void)can_run;
}

void BattleActor::clearAilments()
{
  for(auto &clear_ailment : ailments)
  {
    if(clear_ailment)
      delete clear_ailment;
    clear_ailment = nullptr;
  }

  ailments.clear();
}

void BattleActor::clearBattleItems()
{
  for(auto &battle_item : battle_items)
  {
    if(battle_item)
      delete battle_item;
    battle_item = nullptr;
  }

  battle_items.clear();
}

void BattleActor::clearBattleSkills()
{
  for(auto &battle_skill : battle_skills)
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
  if(sprite_action)
    delete sprite_action;
  if(sprite_first_person)
    delete sprite_first_person;
  if(sprite_third_person)
    delete sprite_third_person;

  sprite_action = nullptr;
  sprite_first_person = nullptr;
  sprite_third_person = nullptr;
}

void BattleActor::createSprites(SDL_Renderer *renderer)
{
  if(person_base)
  {
    /* Build the action sprite */
    if(person_base->getActionSprite())
    {
      sprite_action = new Sprite(*(person_base->getActionSprite()));

      if(sprite_action)
      {
        sprite_action->setNonUnique(true,
                                    person_base->getActionSprite()->getSize());
        sprite_action->createTexture(renderer);
      }
    }

    /* Build the first person sprite, if one exists */
    if(person_base->getFirstPerson())
    {
      sprite_first_person = new Sprite(*(person_base->getFirstPerson()));

      if(sprite_first_person)
      {
        sprite_first_person->setNonUnique(
            true, person_base->getFirstPerson()->getSize());
        sprite_first_person->createTexture(renderer);
      }
    }

    /* Build the third person sprite, if one exists */
    if(person_base->getThirdPerson())
    {
      sprite_third_person = new Sprite(*(person_base->getThirdPerson()));

      if(sprite_third_person)
      {
        sprite_third_person->setNonUnique(
            true, person_base->getThirdPerson()->getSize());
        sprite_third_person->createTexture(renderer);
      }
    }

    if(person_base->getDialogSprite())
    {
      dialog_x = person_base->getActionX();
      dialog_y = person_base->getActionY();

      sprite_dialog = new Sprite(*(person_base->getDialogSprite()));

      if(sprite_dialog)
      {
        sprite_dialog->setNonUnique(true,
                                    person_base->getDialogSprite()->getSize());
        sprite_dialog->createTexture(renderer);
      }
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
  auto &end = action_element.position_end;
  auto &start = action_element.position_start;
  auto &curr = action_element.position_curr;

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
      new Ailment(type, &stats_actual, min_turns, max_turns, chance);

  ailments.push_back(new_ailment);
}

// TODO: Build battle Items [09-07-15]
bool BattleActor::buildBattleItems(std::vector<BattleActor *> all_targets)
{
  (void)all_targets;
  bool success = person_base;

  return success;
}

bool BattleActor::buildBattleSkills(std::vector<BattleActor *> a_targets)
{
  person_base->updateBaseSkills();

  /* Clear BattleSkills if they were previously created */
  clearBattleSkills();

  bool success = person_base;

  if(success)
  {
    auto curr_skills = person_base->getCurrSkills();
    auto useable_skills = curr_skills->getElements(person_base->getLevel());

    for(auto &element : useable_skills)
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

          if(isInflicted(Infliction::SILENCE) && battle_skill->true_cost > 0)
          {
            std::cout << "[Silenced Skill!] " << std::endl;
            battle_skill->valid_status = ValidStatus::SILENCED;
          }
          if(battle_skill->true_cost > stats_actual.getValue(Attribute::QTDR))
            battle_skill->valid_status = ValidStatus::NOT_AFFORDABLE;
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
    std::cout << "[WARNING] : Damage do base value with modified curr vita."
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
  for(const auto &ailment : ailments)
    if(ailment && ailment->getType() == test_infliction)
      return true;

  return false;
}

Ailment *BattleActor::nextUpdateAilment()
{
  for(auto &ailment : ailments)
  {
    if(ailment && ailment->getUpdateStatus() == AilmentStatus::INCOMPLETE)
      return ailment;
  }

  return nullptr;
}

// TODO: [09-06-15] Removing ailments. Corner cases?

void BattleActor::clearAilment(Ailment *remove_ailment)
{
  delete remove_ailment;
  remove_ailment = nullptr;
}

bool BattleActor::removeAilment(Ailment *remove_ailment)
{
  if(remove_ailment)
  {
    ailments.erase(std::remove_if(ailments.begin(), ailments.end(),
                                  [&](Ailment *a) -> bool
                                  {
                                    return a == remove_ailment;
                                  }),
                   ailments.end());

    return true;
  }

  return false;
}

void BattleActor::removeAilmentsKO()
{
  for(auto &ailment : ailments)
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
      state_flashing.element->createAsSpriteFlash(this, color, time_left);
    else if(flashing_type == FlashingType::KOING)
      state_flashing.element->createAsSpriteDeath(this, color, 3000, 500, 2000);
  }
}

void BattleActor::turnSetup()
{
  state_selection = SelectionState::NOT_SELECTED;

  if(person_base && person_base->getAI())
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
  // else
  //   updateSpriteGeneral(cycle_time);

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

Frame *BattleActor::getActionFrame()
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

Sprite *BattleActor::getActiveSprite()
{
  if(active_sprite == ActiveSprite::FIRST_PERSON)
    return sprite_first_person;
  if(active_sprite == ActiveSprite::THIRD_PERSON)
    return sprite_third_person;
  if(active_sprite == ActiveSprite::ACTION)
    return sprite_action;

  return nullptr;
}

std::vector<ActionType> BattleActor::getValidActionTypes()
{
  resetActionTypes();

  std::vector<ActionType> valid_types;

  valid_types.push_back(ActionType::SKILL);

  if(getFlag(ActorState::PAS_ENABLED))
    valid_types.push_back(ActionType::PASS);

  // TODO: Other action types [10-04-15]

  return valid_types;
}

std::vector<Ailment *> BattleActor::getAilments()
{
  return ailments;
}

Person *BattleActor::getBasePerson()
{
  return person_base;
}

std::vector<BattleItem *> BattleActor::getBattleItems()
{
  return battle_items;
}

std::vector<BattleSkill *> BattleActor::getBattleSkills()
{
  return battle_skills;
}

bool BattleActor::getFlag(const ActorState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

int32_t BattleActor::getIndex()
{
  return battle_index;
}

Frame *BattleActor::getInfoFrame()
{
  return frame_info;
}

Sprite *BattleActor::getDialogSprite()
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
uint32_t BattleActor::getSkillCost(Skill *test_skill)
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

BattleStats &BattleActor::getStats()
{
  return stats_actual;
}

BattleStats &BattleActor::getStatsRendered()
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

  return std::round(proportion * 100);
}

float BattleActor::getRegenRate(Attribute attr)
{
  if(person_base && attr == Attribute::VITA)
    return getRegenFactor(person_base->getVitaRegenRate());
  if(person_base && attr == Attribute::QTDR)
    return getRegenFactor(person_base->getQDRegenRate());

  return 0.00;
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

void BattleActor::setActionFrame(Frame *frame_action)
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
}

void BattleActor::setFlag(ActorState set_flags, const bool &set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

void BattleActor::setGuardingState(GuardingState state_guarding)
{
  this->state_guarding = state_guarding;
}

void BattleActor::setInfoFrame(Frame *frame_info)
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
std::vector<BattleActor *>
BattleActor::getAllyTargets(BattleActor *user,
                            std::vector<BattleActor *> targets)
{
  assert(user);
  std::vector<BattleActor *> valid_targets;

  for(const auto &target : targets)
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

/* Grab all enemy targets for given user */
std::vector<BattleActor *>
BattleActor::getEnemyTargets(BattleActor *user,
                             std::vector<BattleActor *> targets)
{
  assert(user);
  std::vector<BattleActor *> valid_targets;

  for(const auto &target : targets)
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

std::vector<BattleActor *>
BattleActor::getLivingTargets(std::vector<BattleActor *> targets)
{
  targets.erase(std::remove_if(begin(targets), end(targets),
                               [&](BattleActor *actor) -> bool
                               {
                                 if(actor)
                                 {
                                   return actor->getStateLiving() ==
                                          LivingState::ALIVE;
                                 }

                                 return true;
                               }),
                end(targets));

  return targets;
}

std::vector<BattleActor *>
BattleActor::getRemovedUser(BattleActor *user,
                            std::vector<BattleActor *> targets)
{
  targets.erase(std::remove(begin(targets), end(targets), user), end(targets));

  return targets;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

std::vector<BattleActor *>
BattleActor::getTargetsFromScope(BattleActor *user, ActionScope scope,
                                 std::vector<BattleActor *> targets)
{
  assert(user);

  std::vector<BattleActor *> valid_targets;

  /* User Scope - only the user is selectable */
  if(scope == ActionScope::USER)
  {
    valid_targets.push_back(user);
  }
  /* One Target - any one of the valid targets */
  else if(scope == ActionScope::ONE_TARGET || scope == ActionScope::ONE_PARTY ||
          scope == ActionScope::ALL_TARGETS)
  {
    valid_targets = targets;
  }
  /* One Enemy - any enemy opponent whose index sign is opposite the user */
  else if(scope == ActionScope::ONE_ENEMY ||
          scope == ActionScope::TWO_ENEMIES ||
          scope == ActionScope::ALL_ENEMIES)
  {
    valid_targets = getEnemyTargets(user, targets);
  }
  else if(scope == ActionScope::ONE_ALLY || scope == ActionScope::TWO_ALLIES ||
          scope == ActionScope::ALL_ALLIES)
  {
    valid_targets = getAllyTargets(user, targets);
  }
  else if(scope == ActionScope::ONE_ALLY_NOT_USER)
  {
    valid_targets = getAllyTargets(user, targets);
    valid_targets = getRemovedUser(user, valid_targets);
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
    valid_targets = getRemovedUser(user, targets);
  }

  return valid_targets;
}

int32_t BattleActor::calcTurnRegen(Attribute attr)
{
  auto regen_rate = getRegenRate(attr);
  int32_t max_attr_val = 0;
  int32_t amount = 0;
  int32_t max = 0;

  if(attr == Attribute::VITA)
  {
    max_attr_val = stats_actual.getValue(Attribute::MVIT);
    amount = regen_rate * max_attr_val;
    max = max_attr_val - stats_actual.getValue(Attribute::VITA);
  }
  else if(attr == Attribute::QTDR)
  {
    max_attr_val = stats_actual.getValue(Attribute::MQTD);
    amount = regen_rate * max_attr_val;
    max = max_attr_val - stats_actual.getValue(Attribute::VITA);
  }

  /* Cannot return more than the maximum possible regeneration value */
  if(max > 0)
    return std::min(max, amount);
  else
    return amount;
}
