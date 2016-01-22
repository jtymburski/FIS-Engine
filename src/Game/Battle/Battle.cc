/*******************************************************************************
* Class Name: Battle [Declaration]
* Date Created: June 22, 2014
* Date Redesigned: September 5, 2015
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
#include "Game/Battle/Battle.h"
#include "Game/Battle/RenderElement.h"

/*=============================================================================
 * CONSTANTS - Battle Operations
 *============================================================================*/

/* Maximum Ailments (Total)
 * Maximum Each Ailments (Per Person)
 *
 * Base Run Chance (in %)
 * User Run Modifier (Modify user value per point of momentum)
 * Ally Run Modifier (Modify user value per point of momentum)
 * Enemy Run Modifier (Modify user value per point of momentum)
 * Run PC Per Poitn (% to alter run chance by per point of momentum)
 *
 * Defend Modifier (Base Damage Mod While Defending)
 * Guard Modifier (Base Damage Mod While Being Guarded)
 */
const uint16_t Battle::kBIGBAR_CHOOSE{100};
const size_t Battle::kMAX_AILMENTS = 50;
const size_t Battle::kMAX_EACH_AILMENTS = 5;
const float Battle::kBASE_RUN_CHANCE = 0.25;
const float Battle::kUSER_RUN_MODIFIER = 2.00;
const float Battle::kALLY_RUN_MODIFIER = 1.00;
const float Battle::kENEMY_RUN_MODIFIER = 1.00;
const float Battle::kRUN_PC_PER_POINT = 0.003;

/* ------------ Battle Outcome Modifiers ---------------
 * kENEMY_RUN_EXP_PC - %EXP to maintain on pyrric victory (enemies run)
 * kRUN_PC_EXP_PENALTY - %EXP (MAX) penalty when the Allies run from Battle.
 * kALLY_KO_EXP_PC - %EXP which KO member get for winning a Battle.
 */
const int16_t Battle::kALLY_KO_EXP_PC = 50;
const int16_t Battle::kENEMY_RUN_EXP_PC = 25;
const int16_t Battle::kRUN_PC_EXP_PENALTY = 5;

/*=============================================================================
 * CONSTANTS - Battle Display
 *============================================================================*/

const float Battle::kDELAY_SLOW_FACTOR{1.50};
const float Battle::kDELAY_NORM_FACTOR{1.00};
const float Battle::kDELAY_FAST_FACTOR{0.55};

const uint16_t Battle::kACTION_BORDER = 10;
const uint16_t Battle::kACTION_COLOR_A = 175;
const uint16_t Battle::kACTION_COLOR_G = 201;
const uint16_t Battle::kACTION_CORNER_X = 18;
const uint16_t Battle::kACTION_CORNER_Y = 4;
const uint16_t Battle::kACTION_H = 408;
const uint16_t Battle::kACTION_W = 359;
const uint16_t Battle::kACTION_Y = 291;

const uint8_t Battle::kAILMENT_BORDER = 1;
const uint8_t Battle::kAILMENT_GAP = 2;
const uint8_t Battle::kAILMENT_OPACITY = 128;

const uint8_t Battle::kALLY_HEALTH_H = 18;
const uint8_t Battle::kALLY_HEALTH_TRIANGLE = 9;
const uint8_t Battle::kALLY_HEALTH_W = 132;
const uint16_t Battle::kALLY_HEIGHT = 70;
const uint8_t Battle::kALLY_QD_H = 12;
const uint8_t Battle::kALLY_QD_OFFSET = 3;
const uint8_t Battle::kALLY_QD_TRIANGLE = 6;
const uint8_t Battle::kALLY_QD_W = 94;

const uint16_t Battle::kANIMATION_PROCESS = 2000;

// const uint16_t Battle::kBIGBAR_CHOOSE = 100;
const float Battle::kBIGBAR_L = 0.2;
const float Battle::kBIGBAR_M1 = 0.1;
const float Battle::kBIGBAR_M2 = 0.3;
const uint16_t Battle::kBIGBAR_OFFSET = 88;
const float Battle::kBIGBAR_R = 0.4;
const uint16_t Battle::kBIGBAR_R_OFFSET = 25;

const uint16_t Battle::kBOB_AMOUNT = 10;
const float Battle::kBOB_RATE = 0.01;
const uint32_t Battle::kBOB_TIME = 1000;

const uint16_t Battle::kRUN_AMOUNT{20};
const float Battle::kRUN_RATE{0.03};
const uint32_t Battle::kRUN_TIME{750};

const uint8_t Battle::kCOLOR_BASE = 150;
const float Battle::kCYCLE_RATE = 0.003;

const uint16_t Battle::kDELAY_DAMAGE = 650;
const uint16_t Battle::kDELAY_SKILL = 1500;

const uint8_t Battle::kINFO_BORDER = 2;
const uint8_t Battle::kINFO_GREY = 200;
const uint16_t Battle::kINFO_H = 50;
const uint8_t Battle::kINFO_OPACITY = 166;
const uint8_t Battle::kINFO_TRIANGLE = 6;
const uint16_t Battle::kINFO_W = 180;

const uint8_t Battle::kENEMY_BAR_H = 8;
const uint8_t Battle::kENEMY_BAR_OFFSET = 2;
const uint8_t Battle::kENEMY_BAR_TRIANGLE = 5;
const uint16_t Battle::kENEMY_BAR_W = 126;
const uint16_t Battle::kENEMIES_BAR_GAP = 12;
const uint16_t Battle::kENEMIES_OFFSET = 94;
const uint16_t Battle::kALLIES_OFFSET = 328;

const uint8_t Battle::kMAX_CHARS = 5;
const uint8_t Battle::kMAX_LAYERS = 10;
const uint16_t Battle::kPERSON_SPREAD = 200;
const uint16_t Battle::kPERSON_WIDTH = 256;

// const uint8_t Battle::kTYPE_MARGIN = 7;
// const uint8_t Battle::kTYPE_MAX = 5;
// const uint8_t Battle::kTYPE_SELECT = 3;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Battle::Battle()
    : background{nullptr},
      bar_offset{0},
      battle_display_data{nullptr},
      battle_menu{nullptr},
      battle_buffer{nullptr},
      config{nullptr},
      delay{0},
      event{nullptr},
      flags_combat{static_cast<CombatState>(0)},
      flags_render{static_cast<RenderState>(0)},
      frame_enemy_backdrop{nullptr},
      music_id{static_cast<int>(Sound::kID_MUSIC_BATTLE)},
      party_allies{nullptr},
      party_enemies{nullptr},
      outcome{OutcomeType::NONE},
      renderer{nullptr},
      turn_state{TurnState::STOPPED},
      time_elapsed{0},
      turns_elapsed{0},
      upkeep_actor{nullptr},
      upkeep_ailment{nullptr},
      victory_screen{nullptr}
{
  /* Create a new action buffer and a menu object */
  battle_buffer = new Buffer();
  battle_menu = new BattleMenu();
}

/*
 * Description:
 */
Battle::~Battle()
{
  clearBattleActors();
  clearBackground();
  clearElements();
  clearEnemyBackdrop();
  clearEvent();

  if(battle_buffer)
    delete battle_buffer;
  battle_buffer = nullptr;

  if(battle_menu)
    delete battle_menu;
  battle_menu = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS - Battle Operations
 *============================================================================*/

void Battle::actionStateBegin()
{
  event->actor->setStateActionFrame(SpriteState::SLIDING_IN);
  event->action_state = ActionState::SLIDE_IN;
  addDelay(250);
}

void Battle::actionStateSlideIn()
{
  /* Create the fading-in action text */
  auto action_font = config->getFontTTF(FontName::BATTLE_ACTION);
  auto element = new RenderElement(renderer, action_font);

  element->createAsActionText(event->getActionName());
  render_elements.push_back(element);
  event->action_state = ActionState::FADE_IN_TEXT;
  addDelay(350);
}

void Battle::actionStateFadeInText()
{
  if(event->actor->getStateActionFrame() == SpriteState::SLID_IN)
  {
    event->actor->setStateActionFrame(SpriteState::SLIDING_OUT);
    event->action_state = ActionState::SLIDE_OUT;
  }

  addDelay(400);
}

void Battle::actionStateSlideOut()
{
  if(event->actor->getFlag(ActorState::ALLY))
    event->actor->setActiveSprite(ActiveSprite::ACTION);

  event->action_state = ActionState::SWITCH_SPRITE;
  addDelay(250);
}

void Battle::actionStateSwitchSprite()
{
  auto skill_hit_status = event->doesSkillHit();

  if(skill_hit_status == SkillHitStatus::HIT)
    processEventSkill();
  else
    event->action_state = ActionState::SKILL_MISS;

  event->action_state = ActionState::ACTION_START;

  addDelay(200);
}

/* Create the fading-in action text */
void Battle::actionStateSkillMiss()
{
  std::cout << "[Skill Miss!]" << std::endl;
  auto action_font = config->getFontTTF(FontName::BATTLE_ACTION);
  auto element = new RenderElement(renderer, action_font);
  auto action_text = event->actor->getBasePerson()->getName() + " Missed";
  element->createAsActionText(action_text);
  render_elements.push_back(element);
  event->action_state = ActionState::DONE;

  addDelay(1000);
}

void Battle::actionStateActionStart()
{
  bool done = true;

  for(auto& outcome : event->actor_outcomes)
  {
    if(outcome.actor_outcome_state == ActionState::ACTION_MISS)
      outcomeStateActionMiss(outcome);
    else if(outcome.actor_outcome_state == ActionState::INFLICTION_MISS)
      outcomeStateActionMiss(outcome);
    else if(outcome.actor_outcome_state == ActionState::PLEP)
      outcomeStatePlep(outcome);
    else if(outcome.actor_outcome_state == ActionState::DAMAGE_VALUE)
      outcomeStateDamageValue(outcome);
    else if(outcome.actor_outcome_state == ActionState::SPRITE_FLASH)
      outcomeStateSpriteFlash(outcome);
    else if(outcome.actor_outcome_state == ActionState::INFLICT_FLASH)
      outcomeStateInflictFlash(outcome);
    else if(outcome.actor_outcome_state == ActionState::OUTCOME)
      outcomeStateActionOutcome(outcome);

    done &= outcome.actor_outcome_state == ActionState::ACTION_END;
  }

  if(done)
  {
    if(event->setNextAction())
      processEventSkill();
    // event->action_state = ActionState::SWITCH_SPRITE;
    else
      event->action_state = ActionState::DONE;

    addDelay(250);
  }
}

void Battle::addDelay(int32_t delay_amount)
{
  if(delay_amount > 0)
  {
    auto to_add = delay_amount;

    delay += (to_add * kDELAY_FAST_FACTOR);
  }
}

bool Battle::bufferMenuSelection()
{
  auto actor = battle_menu->getActor();
  auto action_type = battle_menu->getSelectedType();
  auto targets = battle_menu->getTargetsSelected();
  auto success = (actor != nullptr);

  if(success && action_type == ActionType::SKILL)
  {
    auto skill = battle_menu->getSelectedBattleSkill();

    if(skill && skill->skill)
    {
      auto cooldown = skill->skill->getCooldown();
      auto true_cost = skill->true_cost;
      auto curr_qd = actor->getStats().getValue(Attribute::QTDR);

      /* Have the actor pay the required QD */
      actor->getStats().setBaseValue(Attribute::QTDR, curr_qd - true_cost);

      /* Buffer the skill selection */
      battle_buffer->addSkill(actor, skill, targets, cooldown, turns_elapsed);
    }
  }
  else if(success && action_type == ActionType::ITEM)
  {
    auto battle_item = battle_menu->getSelectedBattleItem();

    if(battle_item && battle_item->item)
    {
      /* Grab the item and the item's Game ID */
      auto item = battle_item->item;
      auto item_id = item->getGameID();

      /* Buffer the Item selection and transfer ownership of Item memory */
      battle_buffer->addItem(actor, battle_item, targets);

      /* Remove the item from the inventory after use */
      success &= party_allies->getInventory()->removeID(item_id);
    }
  }
  else if(success && action_type == ActionType::PASS)
  {
    battle_buffer->addPass(actor, turns_elapsed);
  }

  return success;
}

bool Battle::bufferModuleSelection()
{
  auto curr_actor = getCurrentModuleActor();
  auto curr_module = getCurrentModule();

  if(curr_actor && curr_module)
  {
    auto action_type = curr_module->getActionType();
    auto targets = curr_module->getChosenTargets();

    if(action_type == ActionType::DEFEND)
    {
      battle_buffer->addDefend(curr_actor);
    }
    else if(action_type == ActionType::ITEM && curr_module->getSelectedItem())
    {
      auto item = curr_module->getSelectedBattleItem();
      battle_buffer->addItem(curr_actor, item, targets);
    }
    else if(action_type == ActionType::SKILL && curr_module->getSelectedSkill())
    {
      auto skill = curr_module->getSelectedBattleSkill();
      auto cooldown = skill->skill->getCooldown();
      battle_buffer->addSkill(curr_actor, skill, targets, cooldown,
                              turns_elapsed);

      /* Pay the required QTDR cost for the Skill */
      auto true_cost = curr_module->getSelectedBattleSkill()->true_cost;
      auto curr_qd = curr_actor->getStats().getValue(Attribute::QTDR);
      curr_actor->getStats().setBaseValue(Attribute::QTDR, curr_qd - true_cost);
    }
    else if(action_type == ActionType::PASS)
    {
      battle_buffer->addPass(curr_actor, turns_elapsed);
    }
    else if(action_type == ActionType::RUN)
    {
      battle_buffer->addRun(curr_actor);
    }
  }
  else
  {
    return false;
  }

  return true;
}

// TODO: Determine can run
void Battle::buildBattleActors(Party* allies, Party* enemies)
{
  for(uint32_t i = 0; i < allies->getSize(); i++)
  {
    auto new_ally =
        new BattleActor(allies->getMember(i), getBattleIndex(i),
                        getMenuIndex(i, true), true, true, renderer);

    /* Add the new ally to the vector of actors of the Battle */
    actors.push_back(new_ally);
  }
  for(uint32_t i = 0; i < enemies->getSize(); i++)
  {
    auto new_enemy =
        new BattleActor(enemies->getMember(i), getBattleIndex(i),
                        getMenuIndex(i, false), false, true, renderer);

    /* Add the new enemy to the vector of actors of the Battle */
    actors.push_back(new_enemy);
  }
}

void Battle::cleanUpTurn()
{
  // Reset each member.
  // -- For each battle actor:
  // ----- clear Defend/Guard/Guarding/Temp ailment efffects

  battle_buffer->clearForTurn(turns_elapsed);
  battle_buffer->updateCooldowns();
  battle_menu->clear();

  for(auto& actor : actors)
    if(actor)
      actor->cleanUpForTurn();

  turns_elapsed++;
  addDelay(500);
  setFlagCombat(CombatState::PHASE_DONE, true);
}

void Battle::clearBattleActors()
{
  for(auto& battle_actor : actors)
  {
    if(battle_actor)
      delete battle_actor;
    battle_actor = nullptr;
  }

  actors.clear();
}

bool Battle::checkAlliesDeath()
{
  bool dead = true;

  for(const auto& ally : getAllies())
    dead &= ally && (ally->getStateLiving() != LivingState::ALIVE);

  return dead;
}

bool Battle::checkEnemiesDeath()
{
  bool dead = true;

  for(const auto& foe : getEnemies())
    dead &= foe && (foe->getStateLiving() != LivingState::ALIVE);

  return dead;
}

void Battle::checkIfOutcome()
{
  auto allies_dead = checkAlliesDeath();
  auto enemies_dead = checkEnemiesDeath();

  if(enemies_dead)
    outcome = OutcomeType::VICTORY;
  else if(allies_dead)
    outcome = OutcomeType::DEFEAT;
}

void Battle::clearEvent()
{
  if(event)
    delete event;

  event = nullptr;
}

void Battle::createDamageElement(BattleActor* actor, DamageType damage_type,
                                 uint32_t amount)
{
  if(actor && config)
  {
    auto font = config->getFontTTF(FontName::BATTLE_DAMAGE);
    auto element = new RenderElement(renderer, font);

    element->createAsDamageValue(amount, damage_type, config->getScreenHeight(),
                                 getActorX(actor), getActorY(actor));
    render_elements.push_back(element);
  }
}

bool Battle::doesActorNeedToSelect(BattleActor* actor)
{
  bool to_select = (actor != nullptr);

  if(to_select)
  {
    to_select &= (actor->getStateLiving() == LivingState::ALIVE);
    to_select &= (actor->getSelectionState() == SelectionState::NOT_SELECTED);
    to_select &= (!battle_buffer->hasCoolingSkill(actor));
    to_select &= (!actor->isInflicted(Infliction::HIBERNATION));

    /* Not paralyzed/stunned/etc. */
    to_select &= (!actor->getFlag(ActorState::SELECTION_SKIP));
  }

  return to_select;
}

bool Battle::doesActorNeedToUpkeep(BattleActor* actor)
{
  bool to_upkeep = (actor != nullptr);

  if(to_upkeep)
  {
    to_upkeep &= actor->getStateLiving() == LivingState::ALIVE;
    to_upkeep &= actor->getStateUpkeep() == UpkeepState::VITA_REGEN;
  }

  return to_upkeep;
}

void Battle::updateGeneralUpkeep()
{
  /* First turn -> skip updates, other turns -> add to update */
  prepareActorUpkeeps();

  for(auto& actor : actors)
    if(actor)
      actor->setSelectionState(SelectionState::NOT_SELECTED);

  /* Upkeep before the personal upkeeps start */
  setFlagCombat(CombatState::PHASE_DONE);
}

bool Battle::isBufferElementValid()
{
  bool valid = true;
  auto actor = battle_buffer->getUser();

  if(actor && actor->getStateLiving() != LivingState::ALIVE)
    valid = false;

  if(valid)
  {
    for(auto& target : battle_buffer->getTargets())
    {
      // TODO: If target death matches death scope, also permit.
      if(target && target->getStateLiving() == LivingState::ALIVE)
      {
        ActionScope buffer_scope = ActionScope::NO_SCOPE;
        auto skill = battle_buffer->getSkill();
        auto item = battle_buffer->getItem();

        if(skill && skill->skill)
          buffer_scope = skill->skill->getScope();
        else if(item && item->getUseSkill())
          buffer_scope = item->getUseSkill()->getScope();

        if(buffer_scope == ActionScope::ALL_ALLIES_KO ||
           buffer_scope == ActionScope::ONE_ALLY_KO)
        {
          valid = false;
        }
      }
      else if(target && target->getStateLiving() != LivingState::ALIVE)
        valid = false;
    }
  }

  return valid;
}

void Battle::loadBattleEvent()
{
  /* Clear the event if it's already been created */
  clearEvent();

  auto action_type = battle_buffer->getActionType();
  auto user = battle_buffer->getUser();
  auto targets = battle_buffer->getTargets();

  std::cout << "Action type? " << Helpers::actionTypeToStr(action_type)
            << std::endl;
  std::cout << "User? " << battle_buffer->getUser()->getBasePerson()->getName()
            << std::endl;

  auto to_build = (action_type != ActionType::NONE);
  to_build &= (user != nullptr);

  if(to_build)
  {
    if(action_type == ActionType::SKILL || action_type == ActionType::ITEM)
      event = new BattleEvent(action_type, user, targets);
    else if(action_type == ActionType::PASS)
      event = new BattleEvent(action_type, user);

    if(action_type == ActionType::SKILL)
      event->event_skill = battle_buffer->getSkill();
    else if(action_type == ActionType::ITEM)
      event->event_item = battle_buffer->getItem();

    battle_buffer->setStarted();
  }
}

bool Battle::loadMenuForActor(BattleActor* actor)
{
  bool success = (actor != nullptr);
  success &= (battle_menu != nullptr);

  if(success && party_allies)
  {
    std::cout << "Building battle items for player!" << std::endl;
    success &= actor->buildBattleItems(party_allies->getInventory(), actors);

    for(auto& battle_item : actor->getBattleItems())
    {
      battle_item->print();
    }

    success &= actor->buildBattleSkills(actors);

    if(success)
    {
      std::cout << "Clearing the menu for a new actor." << std::endl;
      battle_menu->clear();

      success &= battle_menu->setActor(actor);
      battle_menu->setSelectableTypes(actor->getValidActionTypes());
      battle_menu->setSelectableItems(actor->getBattleItems());
      battle_menu->setSelectableSkills(actor->getBattleSkills());
      battle_menu->ready();
    }
  }

  return success;
}

void Battle::outcomeStateActionMiss(ActorOutcome& outcome)
{
  auto damage_font = config->getFontTTF(FontName::BATTLE_DAMAGE);
  auto element = new RenderElement(renderer, damage_font);

  element->createAsDamageText(
      "Miss", DamageType::ACTION_MISS, config->getScreenHeight(),
      getActorX(outcome.actor), getActorY(outcome.actor));
  render_elements.push_back(element);

  outcome.actor_outcome_state = ActionState::ACTION_END;
  addDelay(600);
}

void Battle::outcomeStatePlep(ActorOutcome& outcome)
{
  auto x = getActorX(outcome.actor);
  auto y = getActorY(outcome.actor);

  /* Create the render element sprite */
  if(event->getCurrAction()->actionFlag(ActionFlags::DAMAGE) ||
     event->getCurrAction()->actionFlag(ActionFlags::ALTER))
  {
    auto skill = event->getCurrSkill();
    auto element = new RenderElement(renderer, skill->getAnimationPath(),
                                     skill->getAnimationFrames(),
                                     skill->getAnimationTime(), 1, {x, y});

    render_elements.push_back(element);
    event_handler->triggerSound(Sound::kID_SOUND_BTL_PLEP,
                                SoundChannels::TRIGGERS);
  }
  else if(event->getCurrAction()->actionFlag(ActionFlags::INFLICT))
  {
    auto type = event->getCurrAction()->getAilment();
    auto animation = battle_display_data->getPlepAilment(type);
    auto element = new RenderElement(renderer, animation, 1, x, y);

    playInflictionSound(type);
    render_elements.push_back(element);
    outcome.actor_outcome_state = ActionState::INFLICT_FLASH;
  }

  /* Set the next state based on the action flag */
  if(event->getCurrAction()->actionFlag(ActionFlags::DAMAGE))
    outcome.actor_outcome_state = ActionState::DAMAGE_VALUE;
  if(event->getCurrAction()->actionFlag(ActionFlags::ALTER))
    outcome.actor_outcome_state = ActionState::DAMAGE_VALUE;
  if(event->getCurrAction()->actionFlag(ActionFlags::INFLICT))
    outcome.actor_outcome_state = ActionState::DAMAGE_VALUE;

  addDelay(150);
}

void Battle::outcomeStateDamageValue(ActorOutcome& outcome)
{
  auto damage_font = config->getFontTTF(FontName::BATTLE_DAMAGE);
  auto element = new RenderElement(renderer, damage_font);

  if(event->getCurrAction())
  {
    if(event->getCurrAction()->actionFlag(ActionFlags::DAMAGE))
    {
      auto damage_type = DamageType::BASE;

      if(outcome.critical)
        damage_type = DamageType::CRITICAL;

      element->createAsDamageValue(
          outcome.damage, damage_type, config->getScreenHeight(),
          getActorX(outcome.actor), getActorY(outcome.actor));
    }
    else if(event->getCurrAction()->actionFlag(ActionFlags::ALTER))
    {
      std::cout << "[RenderElement] Alter Value Creation" << std::endl;
      // TODO: Non-VITA altering stats?
      element->createAsDamageValue(
          std::abs(outcome.damage), DamageType::HEALING,
          config->getScreenHeight(), getActorX(outcome.actor),
          getActorY(outcome.actor));
    }
  }

  render_elements.push_back(element);
  outcome.actor_outcome_state = ActionState::SPRITE_FLASH;
  addDelay(300);
}

void Battle::outcomeStateSpriteFlash(ActorOutcome& outcome)
{
  if(outcome.actor->dealDamage(outcome.damage))
    outcome.causes_ko = true;
  else
    outcome.actor->startFlashing(FlashingType::DAMAGE, 750);

  outcome.actor_outcome_state = ActionState::OUTCOME;
}

void Battle::outcomeStateInflictFlash(ActorOutcome& outcome)
{
  if(outcome.infliction_status == InflictionStatus::INFLICTION)
  {
    auto flashing_type = FlashingType::INFLICT;

    if(outcome.infliction_type == Infliction::POISON)
      flashing_type = FlashingType::POISON;

    outcome.actor->startFlashing(flashing_type, 750);
    outcome.actor_outcome_state = ActionState::OUTCOME;
    addDelay(100);
  }
  else if(outcome.infliction_status == InflictionStatus::IMMUNE)
  {
    // TODO: Factor this out
    auto damage_font = config->getFontTTF(FontName::BATTLE_DAMAGE);
    auto element = new RenderElement(renderer, damage_font);

    element->createAsDamageText(
        "Immune", DamageType::IMMUNE, config->getScreenHeight(),
        getActorX(outcome.actor), getActorY(outcome.actor));
    render_elements.push_back(element);
    addDelay(600);
    outcome.actor_outcome_state = ActionState::ACTION_END;
  }
  else if(outcome.infliction_status == InflictionStatus::ALREADY_INFLICTED)
  {
    // TODO: Factor this out
    auto damage_font = config->getFontTTF(FontName::BATTLE_DAMAGE);
    auto element = new RenderElement(renderer, damage_font);

    element->createAsDamageText(
        "Fizzle", DamageType::ALREADY_INFLICTED, config->getScreenHeight(),
        getActorX(outcome.actor), getActorY(outcome.actor));
    render_elements.push_back(element);
    addDelay(600);
    outcome.actor_outcome_state = ActionState::ACTION_END;
  }
  else if(outcome.infliction_status == InflictionStatus::FIZZLE)
  {
    outcome.actor_outcome_state = ActionState::ACTION_END;
  }
}

void Battle::outcomeStateActionOutcome(ActorOutcome& outcome)
{
  /* If Person's VITA is 0 -> they are KOed) */
  if(outcome.causes_ko)
  {
    if(this->outcome != OutcomeType::NONE)
      addDelay(2000);

    event_handler->triggerSound(Sound::kID_SOUND_BTL_DEATH,
                                SoundChannels::TRIGGERS);
    outcome.actor->startFlashing(FlashingType::KOING);
    outcome.actor->removeAilmentsKO();
  }

  /* If the outcome causes an infliction -> inflict that person */
  if(outcome.infliction_type != Infliction::INVALID)
    processInfliction(outcome.actor, outcome.infliction_type);

  outcome.actor_outcome_state = ActionState::ACTION_END;
}

int32_t Battle::outnumberedVal()
{
  if(getEnemies().size() > getAllies().size())
  {
    return getEnemies().size() - getAllies().size();
  }

  return 0;
}

void Battle::prepareActorUpkeeps()
{
  for(auto& actor : actors)
  {
    if(actor)
    {
      actor->setUpkeepState(UpkeepState::VITA_REGEN);

      auto ailments = actor->getAilments();

      for(auto& ailment : ailments)
      {
        if(ailment)
          ailment->setUpdateStatus(AilmentStatus::INCOMPLETE);
      }
    }
  }
}

bool Battle::calculateEnemySelection(BattleActor* next_actor,
                                     AIModule* curr_module)
{
  auto success = true;

  success &= (next_actor != nullptr);
  success &= (curr_module != nullptr);

  if(success && party_enemies)
  {
    // TODO: Enemy items probably broken.
    // success &=
    //     next_actor->buildBattleItems(party_enemies->getInventory(), actors);
    success &= next_actor->buildBattleSkills(actors);

    std::cout << "----- Enemy Valid Battle Skills ----- " << std::endl;
    auto battle_skills = next_actor->getBattleSkills();

    for(auto battle_skill : battle_skills)
      battle_skill->print();

    // curr_module->setItems(next_actor->getBattleItems());
    curr_module->setSkills(next_actor->getBattleSkills());
    curr_module->calculateAction();
    curr_module->calculateTargets();
  }
  else
  {
    success = false;
  }

  return success;
}

void Battle::updateBegin()
{
  setFlagCombat(CombatState::PHASE_DONE, true);
}

void Battle::updateBufferNext()
{
  auto has_element = battle_buffer->setNext();

  if(has_element && !isBufferElementValid())
  {
    bool found_good = false;

    while(has_element && !found_good)
    {
      has_element = battle_buffer->setNext();

      if(has_element)
        found_good = isBufferElementValid();
      else if(event)
        event->action_state = ActionState::DONE;
    }
  }

  if(!has_element)
    setFlagCombat(CombatState::PHASE_DONE, true);
}

void Battle::updateDelay(int32_t decrement_delay)
{
  if(delay > static_cast<uint32_t>(decrement_delay))
    delay -= decrement_delay;
  else
    delay = 0;
}

/* Update the current Buffer element */
void Battle::updateEvent()
{
  if(event->action_type == ActionType::SKILL ||
     event->action_type == ActionType::ITEM)
  {
    if(event->action_state == ActionState::BEGIN)
      actionStateBegin();
    else if(event->action_state == ActionState::SLIDE_IN)
      actionStateSlideIn();
    else if(event->action_state == ActionState::FADE_IN_TEXT)
      actionStateFadeInText();
    else if(event->action_state == ActionState::SLIDE_OUT)
      actionStateSlideOut();
    else if(event->action_state == ActionState::SWITCH_SPRITE)
      actionStateSwitchSprite();
    else if(event->action_state == ActionState::SKILL_MISS)
      actionStateSkillMiss();
    else if(event->action_state == ActionState::ACTION_START)
      actionStateActionStart();
  }
  else if(event->action_type == ActionType::PASS)
  {
    event->action_state = ActionState::DONE;
  }
}

void Battle::processEventItem()
{
  // TODO: Do the same thing?
  processEventSkill();
}

void Battle::processEventSkill()
{
  auto curr_action = event->getCurrAction();
  event->actor_outcomes.clear();

  if(curr_action)
  {
    for(auto& target : event->actor_targets)
      processEventAction(curr_action, target);
  }
}

void Battle::processEventAction(Action* curr_action, BattleActor* target)
{
  ActorOutcome outcome;
  outcome.actor = target;

  auto action_hits = event->doesActionHit(target);

  if(action_hits)
  {
    // std::cout << "Action Hits!" << std::endl;
    if(curr_action->actionFlag(ActionFlags::DAMAGE))
    {
      outcome.critical = event->doesActionCrit(target);

      float crit_factor = 1.00;

      if(outcome.critical)
        crit_factor = event->calcCritFactor(target);

      outcome.damage = event->calcDamage(target, crit_factor);
      outcome.actor_outcome_state = ActionState::PLEP;
    }
    else if(curr_action->actionFlag(ActionFlags::INFLICT))
    {
      auto type = curr_action->getAilment();

      outcome.infliction_type = curr_action->getAilment();

      if(outcome.actor->isInflicted(type))
        outcome.infliction_status = InflictionStatus::ALREADY_INFLICTED;
      else if(outcome.actor->isImmune(type))
        outcome.infliction_status = InflictionStatus::IMMUNE;

      outcome.infliction_status = InflictionStatus::INFLICTION;
      outcome.actor_outcome_state = ActionState::PLEP;
    }
    else if(curr_action->actionFlag(ActionFlags::ALTER))
    {
      std::cout << "[CALCULATING THE ALTER VALUE]" << std::endl;
      outcome.damage = event->calcAltering(target);
      outcome.actor_outcome_state = ActionState::PLEP;
    }
  }
  else
  {
    outcome.damage = 0;
    outcome.actor_outcome_state = ActionState::ACTION_MISS;

    if(curr_action->actionFlag(ActionFlags::INFLICT))
      outcome.actor_outcome_state = ActionState::INFLICTION_MISS;
  }

  event->actor_outcomes.push_back(outcome);
}

void Battle::processInfliction(BattleActor* target, Infliction type)
{
  assert(target && event);
  auto curr_action = event->getCurrAction();
  auto min = curr_action->getMin();
  auto max = curr_action->getMax();

  target->addAilment(type, min, max, curr_action->getBase());
}

void Battle::updateEnemySelection()
{
  auto module_actor = getCurrentModuleActor();
  auto curr_module = getCurrentModule();

  if(module_actor && curr_module)
  {
    if(curr_module->getFlag(AIState::SELECTION_COMPLETE))
    {
      assert(bufferModuleSelection());
      updateSelectingState(module_actor, true);
    }
  }
  else
  {
    auto next_actor = getNextModuleActor();
    auto next_module = getModuleOfActor(next_actor);

    if(next_actor && next_module)
    {
      next_module->resetForNewTurn(next_actor);
      updateSelectingState(next_actor, false);
      calculateEnemySelection(next_actor, next_module);
    }
    else
    {
      setFlagCombat(CombatState::PHASE_DONE);
    }
  }
}

void Battle::updateFadeInText()
{
  auto font = config->getFontTTF(FontName::BATTLE_TURN);
  auto element = new RenderElement(renderer, font);

  std::string turn_text = "";
  uint32_t random = Helpers::randU(1, 85);

  if(random == 2)
    turn_text = "Just Run Away";
  else if(random == 3)
    turn_text = "Did You Turn The Oven Off";
  else if(random == 4)
    turn_text = "Why Even Try";
  else if(random == 5)
    turn_text = "Never Go Backwards";
  else if(random == 6)
    turn_text = "Embrace Your Fate";
  else if(random == 7)
    turn_text = "Destroy";
  else if(random == 8)
    turn_text = "Where Is Boopster";
  else if(random == 9)
    turn_text = "Bearly Even Difficult";
  else if(random == 10)
    turn_text = "Eat Your Fate Cookies";
  else
    turn_text = "Decide Your Fate";

  element->createAsEnterText(turn_text, config->getScreenHeight(),
                             config->getScreenWidth());
  render_elements.push_back(element);
  addDelay(1700);
  setFlagCombat(CombatState::PHASE_DONE, true);
}

void Battle::updateOutcome(int32_t cycle_time)
{
  if(outcome == OutcomeType::VICTORY)
  {
    /* If victory screen is set, update it */
    if(victory_screen)
    {
      victory_screen->update(cycle_time);

      if(victory_screen->getStateVictory() == VictoryState::FADE_IN_HEADER)
      {
        if(!getFlagCombat(CombatState::CREATED_VICTORY_TEXT))
        {
          auto header_font =
              config->getFontTTF(FontName::BATTLE_VICTORY_HEADER);
          auto header_text = new RenderElement(renderer, header_font);
          header_text->createAsVictoryText("VICTORIOUS",
                                           config->getScreenHeight(),
                                           config->getScreenWidth());
          render_elements.push_back(header_text);
          victory_screen->setDimTime(10000 * 0.75);
          setFlagCombat(CombatState::CREATED_VICTORY_TEXT, true);
        }
      }
      else if(victory_screen->getStateVictory() == VictoryState::SLIDE_IN_CARD)
      {
        setFlagCombat(CombatState::PHASE_DONE, true);
      }
    }
    else
    {
      /* If victory screen is not set, create it */
      victory_screen = new Victory(config, renderer, getAllies(), getEnemies());

      /* Dim the Battle a little - infinite render element */
      auto dim_element = new RenderElement();
      auto dim_time = 2000;

      dim_element->createAsRGBOverlay({0, 0, 0, 150}, dim_time, dim_time,
                                      2 * dim_time, config->getScreenHeight(),
                                      config->getScreenWidth());
      dim_element->setTimeable(false);
      render_elements.push_back(dim_element);
      victory_screen->setDimTime(dim_time * 0.75);

      // for(auto& ally : getAllies())
      // {
      //   auto level_up_occured = false;

      //   if(ally && ally->getBasePerson())
      //   {
      //     for(auto& ailment : ally->getAilments())
      //       ally->removeAilment(ailment);

      //     auto name = ally->getBasePerson()->getName();

      //     auto old_level = ally->getBasePerson()->getLevel();
      //     ally->getBasePerson()->addExp(total_exp_drop);
      //     auto new_level = ally->getBasePerson()->getLevel();

      //     if(new_level != old_level)
      //       level_up_occured = true;

      //     for(int i = (new_level - old_level); i > 0; i--)
      //       std::cout << name << " has leveled up!" << std::endl;

      //     if(!level_up_occured)
      //     {
      //       auto base = ally->getBasePerson();

      //       if(base)
      //       {
      //         auto equip_stats = base->calcEquipStats();
      //         auto max_health =
      //             (uint32_t)base->getCurrMax().getStat(Attribute::VITA);

      //         auto curr_health =
      //         ally->getStats().getBaseValue(Attribute::VITA);
      //         auto max_qtdr =
      //             (uint32_t)base->getCurrMax().getStat(Attribute::QTDR);
      //         auto curr_qtdr =
      //         ally->getStats().getBaseValue(Attribute::QTDR);

      //         if(curr_health <= max_health)
      //           base->getCurr().setStat(Attribute::VITA, curr_health);
      //         if(curr_qtdr <= max_qtdr)
      //           base->getCurr().setStat(Attribute::QTDR, curr_qtdr);
      //       }
      //     }
      //   }
      // }
    }
  }
}

void Battle::updatePersonalUpkeep()
{
  if(upkeep_actor)
  {
    auto state = upkeep_actor->getStateUpkeep();

    if(state == UpkeepState::COMPLETE)
      upkeep_actor = nullptr;
    else if(state == UpkeepState::VITA_REGEN)
      updatePersonalVitaRegen();
    else if(state == UpkeepState::QTDR_REGEN)
      updatePersonalQtdrRegen();
    else if(state != UpkeepState::COMPLETE)
      updatePersonalAilments();
  }
  else
  {
    upkeep_actor = getNextUpkeepActor();

    if(!upkeep_actor)
      setFlagCombat(CombatState::PHASE_DONE, true);
    else
      addDelay(200);
  }
}

void Battle::updatePersonalVitaRegen()
{
  // Calculate and create the vita regen for the upkeep_actor
  auto vita_regen =
      upkeep_actor->calcTurnRegen(Attribute::VITA, outnumberedVal());

  if(vita_regen > 0)
  {
    upkeep_actor->restoreVita(vita_regen);
    auto font = config->getFontTTF(FontName::BATTLE_DAMAGE);
    auto element = new RenderElement(renderer, font);

    element->createAsRegenValue(
        vita_regen, DamageType::VITA_REGEN, config->getScreenHeight(),
        getActorX(upkeep_actor), getActorY(upkeep_actor));

    render_elements.push_back(element);
    addDelay(600);
  }

  upkeep_actor->setUpkeepState(UpkeepState::QTDR_REGEN);
}

void Battle::updatePersonalQtdrRegen()
{
  auto qtdr_regen = upkeep_actor->calcTurnRegen(Attribute::QTDR);

  if(qtdr_regen > 0)
  {
    upkeep_actor->restoreQtdr(qtdr_regen);
    auto font = config->getFontTTF(FontName::BATTLE_DAMAGE);
    auto element = new RenderElement(renderer, font);

    element->createAsRegenValue(
        qtdr_regen, DamageType::QTDR_REGEN, config->getScreenHeight(),
        getActorX(upkeep_actor), getActorY(upkeep_actor));
    render_elements.push_back(element);
    addDelay(600);
  }
  // Calculate and create the qtdr regen for the upkeep_actor
  upkeep_actor->setUpkeepState(UpkeepState::AILMENT_BEGIN);
}

void Battle::updatePersonalAilments()
{
  if(upkeep_ailment)
  {
    if(upkeep_actor->getStateUpkeep() == UpkeepState::AILMENT_BEGIN)
    {
      setFlagCombat(CombatState::AILMENT_CLEARS, false);
      upkeep_ailment->update();

      if(upkeep_ailment->getUpdateStatus() == AilmentStatus::TO_REMOVE)
        upkeep_actor->setUpkeepState(UpkeepState::AILMENT_CLEAR);
      else if(upkeep_ailment->getUpdateStatus() == AilmentStatus::TO_DAMAGE)
        upkeep_actor->setUpkeepState(UpkeepState::AILMENT_PLEP);
      else if(upkeep_ailment->getUpdateStatus() == AilmentStatus::SKIP)
      {
        upkeep_actor->setUpkeepState(UpkeepState::AILMENT_PLEP);
        upkeep_actor->setFlag(ActorState::SELECTION_SKIP, true);
      }
      else if(upkeep_ailment->getUpdateStatus() == AilmentStatus::RANDOM)
      {
        upkeep_actor->setUpkeepState(UpkeepState::AILMENT_PLEP);
        upkeep_actor->setFlag(ActorState::SELECTION_RANDOM, true);
      }
      else if(upkeep_ailment->getUpdateStatus() == AilmentStatus::NOTHING)
        upkeep_actor->setUpkeepState(UpkeepState::AILMENT_OUTCOME);
    }
    else if(upkeep_actor->getStateUpkeep() == UpkeepState::AILMENT_CLEAR)
      upkeepAilmentClear();
    else if(upkeep_actor->getStateUpkeep() == UpkeepState::AILMENT_PLEP)
      upkeepAilmentPlep();
    else if(upkeep_actor->getStateUpkeep() == UpkeepState::AILMENT_FLASH)
      upkeepAilmentFlash();
    else if(upkeep_actor->getStateUpkeep() == UpkeepState::AILMENT_DAMAGE)
      upkeepAilmentDamage();
    else if(upkeep_actor->getStateUpkeep() == UpkeepState::AILMENT_OUTCOME)
      upkeepAilmentOutcome();
  }
  else
  {
    upkeep_ailment = upkeep_actor->nextUpdateAilment();

    if(!upkeep_ailment)
      upkeep_actor->setUpkeepState(UpkeepState::COMPLETE);
    else
      upkeep_actor->setUpkeepState(UpkeepState::AILMENT_BEGIN);
  }
}

void Battle::updateProcessing()
{
  /* Sort the buffer, if it hasn't been for the turn */
  if(!battle_buffer->isSorted())
  {
    battle_buffer->reorder();
    battle_buffer->print(false);
  }

  /* Check if the current event is finished processing */
  if(event && event->action_state == ActionState::DONE)
  {
    /* Clear the item memory if the action type completed was an item */
    if(battle_buffer->getActionType() == ActionType::ITEM)
      battle_buffer->clearItem();

    battle_buffer->setProcessed();

    if(event->actor && event->actor->getFlag(ActorState::ALLY))
      event->actor->setActiveSprite(ActiveSprite::FIRST_PERSON);

    clearEvent();
  }
  else if(battle_buffer->isIndexProcessed())
    updateBufferNext();
  else if(battle_buffer->isIndexStarted() && event)
    updateEvent();
  else if(battle_buffer->getCooldown() == 0)
    loadBattleEvent();
  else
    updateBufferNext();
}

void Battle::updateScreenDim()
{
  auto element = new RenderElement();
  element->createAsRGBOverlay({0, 0, 0, 150}, 3000, 500, 1250,
                              config->getScreenHeight(),
                              config->getScreenWidth());
  render_elements.push_back(element);
  addDelay(750);

  setFlagCombat(CombatState::PHASE_DONE);
}

void Battle::updateSelectingState(BattleActor* actor, bool set_selected)
{
  if(actor)
  {
    auto state = actor->getSelectionState();

    if(set_selected)
    {
      if(state == SelectionState::SELECTING)
        actor->setSelectionState(SelectionState::SELECTED_ACTION);
      else if(state == SelectionState::SELECTING_2ND_ACTION)
        actor->setSelectionState(SelectionState::SELECTED_2ND_ACTION);
      else if(state == SelectionState::SELECTING_3RD_ACTION)
        actor->setSelectionState(SelectionState::SELECTED_3RD_ACTION);
    }
    else
    {
      if(state == SelectionState::SELECTED_ACTION)
        actor->setSelectionState(SelectionState::SELECTING_2ND_ACTION);
      else if(state == SelectionState::SELECTED_2ND_ACTION)
        actor->setSelectionState(SelectionState::SELECTING_3RD_ACTION);
      else
        actor->setSelectionState(SelectionState::SELECTING);
    }
  }
}

void Battle::updateUserSelection()
{
  auto menu_actor = battle_menu->getActor();

  /* If menu has a valid actor, update their selection */
  if(menu_actor)
  {
    if(battle_menu->getFlag(BattleMenuState::SELECTION_COMPLETE))
    {
      /* Add the current menu selection to the buffer */
      assert(bufferMenuSelection());

      /* Update selecting state of user [SELECTED, 2ND_SELECTED, etc.] */
      updateSelectingState(menu_actor, true);

      /* Clear the menu for a new turn */
      battle_menu->clear();
    }
  }
  /* If the menu does not have a valid actor, load their data in */
  else
  {
    /* If there exists a menu actor requiring selection, load menu, else ->
     * go to the next phase of battle (enemy selection / outcome phases */
    auto next_actor = getNextMenuActor();

    if(next_actor)
    {
      updateSelectingState(next_actor, false);

      /* If confused -> put in a random selection, else allow selection */
      if(next_actor->getFlag(ActorState::SELECTION_RANDOM))
      {
        // std::cout << " Attempting to choose a a random action for actor: "
        //           << next_actor->getBasePerson()->getName() << "." <<
        //           std::endl;

        auto next_module = getModuleOfActor(next_actor);
        next_module->resetForNewTurn(next_actor);
        next_actor->buildBattleSkills(actors);

        auto battle_skills = next_actor->getBattleSkills();

        for(auto& battle_skill : battle_skills)
          battle_skill->print();

        next_module->setSkills(next_actor->getBattleSkills());

        next_module->calculateAction();
        next_module->calculateTargets();

        bufferModuleSelection();
        updateSelectingState(next_actor, true);
      }
      else
      {
        /* Load the menu for the actor */
        loadMenuForActor(next_actor);
      }
    }
    else
    {
      battle_menu->clear();
      setFlagCombat(CombatState::PHASE_DONE);
    }
  }
}

//
int32_t Battle::getBattleIndex(int32_t index)
{
  if(index == 1)
    return 0;
  if(index == 0)
    return 1;

  return index;
}

// Party Index
//

// Battle Index
//  4  3  2  1  0
//  0  1  2  3  4

// Menu Index
// 9 8 7 6 5
// 4 3 2 1 0
int32_t Battle::getMenuIndex(int32_t index, bool allies)
{
  auto battle_index = getBattleIndex(index);

  if(allies)
  {
    if(battle_index == 1)
      return 3;
    if(battle_index == 0)
      return 4;
    if(battle_index == 2)
      return 2;
    if(battle_index == 3)
      return 1;
    if(battle_index == 4)
      return 0;
  }
  else
  {
    if(battle_index == 4)
      return 9;
    if(battle_index == 3)
      return 8;
    if(battle_index == 2)
      return 7;
    if(battle_index == 1)
      return 6;
    if(battle_index == 0)
      return 5;
  }

  return 0;
}

AIModule* Battle::getCurrentModule()
{
  auto curr_actor = getCurrentModuleActor();

  if(curr_actor)
    return curr_actor->getAI();

  return nullptr;
}

BattleActor* Battle::getCurrentModuleActor()
{
  for(const auto& enemy : actors)
    if(enemy && enemy->getSelectionState() == SelectionState::SELECTING)
      return enemy;

  return nullptr;
}

AIModule* Battle::getModuleOfActor(BattleActor* actor)
{
  if(actor)
    return actor->getAI();

  return nullptr;
}

BattleActor* Battle::getNextModuleActor()
{
  for(const auto& enemy : getEnemies())
    if(doesActorNeedToSelect(enemy))
      return enemy;

  return nullptr;
}

BattleActor* Battle::getNextMenuActor()
{
  for(const auto& ally : getAllies())
    if(doesActorNeedToSelect(ally))
      return ally;

  return nullptr;
}

BattleActor* Battle::getNextUpkeepActor()
{
  for(const auto& actor : actors)
    if(doesActorNeedToUpkeep(actor))
      return actor;

  return nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS - Battle Display
 *============================================================================*/

void Battle::buildActionFrame(BattleActor* actor)
{
  uint16_t width = kACTION_W;
  uint16_t height = kACTION_H;
  uint16_t x1 = width;
  uint16_t y1 = 0;
  uint16_t x2 = 0;
  uint16_t y2 = kACTION_Y;
  uint16_t x3 = width;
  uint16_t y3 = height;
  uint16_t border = kACTION_BORDER;
  uint16_t cx1 = kACTION_CORNER_X;
  uint16_t cy1 = kACTION_CORNER_Y;
  uint16_t cx2 = cx1 - cy1 * 2;
  uint16_t cy2 = cy1 / 2;

  /* Create main rendering texture */
  Frame* rendered_frame = new Frame();
  SDL_Texture* texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

  /* Create underlay rendering texture */
  SDL_Texture* texture2 =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetTextureBlendMode(texture2, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture2);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  /* Draw middle triangle */
  SDL_SetRenderDrawColor(renderer, kACTION_COLOR_G, kACTION_COLOR_G,
                         kACTION_COLOR_G, 255);
  Frame::renderTriangle(x1, y1 + border, x2 + cx1, y2 - cy1, x3, y3 - border,
                        renderer);

  /* Draw the inner triangle */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  Frame::renderTriangle(x1, y1 + border * 2, x2 + cx1 * 2, y2 - cy1 * 2, x3,
                        y3 - border * 2, renderer);

  /* Render base to the main texture */
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetTextureAlphaMod(texture2, kACTION_COLOR_A);
  SDL_RenderCopyEx(renderer, texture2, nullptr, nullptr, 0.0, nullptr,
                   SDL_FLIP_NONE);
  SDL_DestroyTexture(texture2);
  texture2 = nullptr;

  /* Render top black border */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for(uint16_t i = 0; i <= border; i++)
    SDL_RenderDrawLine(renderer, x2, y2 + i, x1, y1 + i);

  /* Render the person */
  Sprite* action_frames = actor->getDialogSprite();
  if(action_frames && action_frames->isFramesSet())
  {
    action_frames->render(renderer, actor->getDialogX(), actor->getDialogY(),
                          action_frames->getCurrent()->getWidth(),
                          action_frames->getCurrent()->getHeight());
  }

  /* Try and chop out the base of the person */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
  for(uint16_t i = 0; i < height - y2; i++)
    SDL_RenderDrawLine(renderer, x2, y2 + i, x3, y3 + i - border / 2);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  /* Render bottom black border */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for(uint16_t i = 0; i <= border; i++)
    SDL_RenderDrawLine(renderer, x2 + cx2, y2 + cy2 - i, x3, y3 - i);

  /* Finalize the frame */
  rendered_frame->setTexture(texture);
  SDL_RenderPresent(renderer);
  SDL_SetRenderTarget(renderer, nullptr);

  actor->setActionFrame(rendered_frame);

  auto end_x = config->getScreenWidth() - rendered_frame->getWidth();
  auto end_y = 381 - 291; // TODO : Proportional?
  actor->setActionFrameEnd(end_x, end_y);
  actor->setActionFrameStart(config->getScreenWidth(), end_y);
}

void Battle::buildEnemyBackdrop()
{
  /* Sizing variables */
  uint16_t width = kINFO_W;
  uint16_t height = kINFO_H;
  uint8_t black_opacity = kINFO_OPACITY;
  uint8_t triangle_width = kINFO_TRIANGLE;
  uint8_t border = kINFO_BORDER;
  uint8_t grey_color = kINFO_GREY;

  /* Create rendering texture */
  Frame* rendered_frame = new Frame();
  SDL_Texture* texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  /* Draw center rectangle */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, black_opacity);
  SDL_Rect rect;
  rect.x = border + triangle_width - 1;
  rect.y = border - 1;
  rect.w = width - triangle_width * 2 - border;
  rect.h = height - border;
  SDL_RenderFillRect(renderer, &rect);

  /* Draw top right triangle */
  uint16_t x_top = rect.x + rect.w;
  uint16_t y_top = rect.y;
  Frame::renderTriangle(x_top, y_top, x_top, y_top + triangle_width,
                        x_top + triangle_width, y_top + triangle_width,
                        renderer);

  /* Draw right rectangle */
  SDL_Rect rect2;
  rect2.x = rect.x + rect.w;
  rect2.y = y_top + triangle_width + 1;
  rect2.w = triangle_width;
  rect2.h = rect.h - triangle_width * 2 - border;
  SDL_RenderFillRect(renderer, &rect2);

  /* Draw right bottom triangle */
  y_top = rect2.y + rect2.h;
  Frame::renderTriangle(x_top, y_top, x_top + triangle_width, y_top, x_top,
                        y_top + triangle_width, renderer);

  /* Draw left top triangle */
  x_top = rect.x;
  y_top = rect.y;
  Frame::renderTriangle(x_top, y_top, x_top, y_top + triangle_width,
                        x_top - triangle_width, y_top + triangle_width,
                        renderer);

  /* Draw left rectangle */
  rect2.x = x_top - triangle_width;
  SDL_RenderFillRect(renderer, &rect2);

  /* Draw left bottom triangle */
  y_top = rect2.y + rect2.h;
  Frame::renderTriangle(x_top, y_top, x_top - triangle_width, y_top, x_top,
                        y_top + triangle_width, renderer);

  /* First single pixel border */
  SDL_SetRenderDrawColor(renderer, grey_color, grey_color, grey_color, 255);
  SDL_Point points[9];
  points[0].x = rect.x;
  points[0].y = rect.y;
  points[1].x = points[0].x + rect.w - 1;
  points[1].y = points[0].y;
  points[2].x = points[1].x + triangle_width;
  points[2].y = points[1].y + triangle_width;
  points[3].x = points[2].x;
  points[3].y = points[2].y + rect2.h + 1;
  points[4].x = points[3].x - triangle_width;
  points[4].y = points[3].y + triangle_width;
  points[5].x = points[4].x - rect.w + 1;
  points[5].y = points[4].y;
  points[6].x = points[5].x - triangle_width;
  points[6].y = points[5].y - triangle_width;
  points[7].x = points[6].x;
  points[7].y = points[6].y - rect2.h - 1;
  points[8].x = points[7].x + triangle_width;
  points[8].y = points[7].y - triangle_width;
  SDL_RenderDrawLines(renderer, points, 9);

  /* Second single pixel border */
  points[0].y -= 1;
  points[1].y -= 1;
  points[2].x += 1;
  points[3].x += 1;
  points[4].y += 1;
  points[5].y += 1;
  points[6].x -= 1;
  points[7].x -= 1;
  points[8].y -= 1;
  SDL_RenderDrawLines(renderer, points, 9);

  /* Finalize the frame */
  SDL_SetRenderTarget(renderer, nullptr);
  rendered_frame->setTexture(texture);
  frame_enemy_backdrop = rendered_frame;
}

// TODO: grab fonts
void Battle::buildInfoAlly(BattleActor* ally)
{
  /* Sizing variables */
  uint8_t health_height = kALLY_HEALTH_H;
  uint8_t health_triangle = kALLY_HEALTH_TRIANGLE;
  uint8_t health_width = kALLY_HEALTH_W;
  uint16_t height = kALLY_HEIGHT;
  uint8_t qd_height = kALLY_QD_H;
  uint8_t qd_offset = kALLY_QD_OFFSET;
  uint8_t qd_triangle = kALLY_QD_TRIANGLE;
  uint8_t qd_width = kALLY_QD_W;
  uint16_t width = kINFO_W;

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

  /* Render the health bar border */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Point border[5];
  border[0].x = (width - health_width) / 2;
  border[0].y = (height - health_height) / 2;
  border[1].x = border[0].x + health_width + health_triangle;
  border[1].y = border[0].y;
  border[2].x = border[1].x - health_triangle;
  border[2].y = border[1].y + health_height;
  border[3].x = border[2].x - health_width - health_triangle;
  border[3].y = border[2].y;
  border[4].x = border[0].x;
  border[4].y = border[0].y;
  SDL_RenderDrawLines(renderer, border, 5);

  /* Render background of QD Bar */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  uint8_t qd_x =
      border[0].x + health_width - qd_offset - qd_triangle - qd_width;
  uint8_t qd_y = border[0].y + health_height - (qd_height / 2);
  Frame::renderBar(qd_x + 1, qd_y, qd_width + qd_triangle, qd_height,
                   (float)qd_triangle / qd_height, renderer);

  /* Render the ally name */
  SDL_Color color = {255, 255, 255, 255};
  Text* t = new Text(font_header);
  t->setText(renderer, ally->getBasePerson()->getName(), color);
  t->render(renderer, (width - t->getWidth()) / 2,
            (border[0].y - t->getHeight()) / 2);
  delete t;

  /* Render the ally level */
  t = new Text(font_subheader);
  t->setText(renderer,
             "Level " + std::to_string(ally->getBasePerson()->getLevel()),
             color);
  t->render(renderer, (width - t->getWidth()) / 2,
            border[0].y + health_height + qd_height / 2 +
                (height - border[0].y - health_height - qd_height / 2 -
                 t->getHeight()) /
                    2);
  delete t;

  /* Set the new frame */
  Frame* ally_info = new Frame();
  ally_info->setTexture(texture);

  /* Clear render connection */
  SDL_SetRenderTarget(renderer, nullptr);

  ally->setInfoFrame(ally_info);
}

/* Build the enemy info for a given enemy actor */
void Battle::buildInfoEnemy(BattleActor* enemy)
{
  /* Sizing variables */
  uint16_t bar_width = kENEMY_BAR_W;
  uint8_t bar_triangle_width = kENEMY_BAR_TRIANGLE;
  uint8_t bar_height = kENEMY_BAR_H;
  uint8_t bar_offset = kENEMY_BAR_OFFSET;

  /* Fonts */
  auto font_header = config->getFontTTF(FontName::BATTLE_HEADER);
  auto font_subheader = config->getFontTTF(FontName::BATTLE_SUBHEADER);

  /* Create rendering texture */
  SDL_Texture* texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, kINFO_W, kINFO_H);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  /* Render the health bar border */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Point border[5];
  border[0].x = (kINFO_W - bar_width) / 2;
  border[0].y = (kINFO_H - bar_height) / 2 + bar_offset;
  border[1].x = border[0].x + bar_width + bar_triangle_width;
  border[1].y = border[0].y;
  border[2].x = border[1].x - bar_triangle_width;
  border[2].y = border[1].y + bar_height;
  border[3].x = border[2].x - bar_width - bar_triangle_width;
  border[3].y = border[2].y;
  border[4].x = border[0].x;
  border[4].y = border[0].y;
  SDL_RenderDrawLines(renderer, border, 5);

  /* Render the enemy name */
  SDL_Color color = {255, 255, 255, 255};
  Text* t = new Text(font_header);

  auto name = enemy->getBasePerson()->getName();

  t->setText(renderer, enemy->getBasePerson()->getName(), color);
  t->render(renderer, (kINFO_W - t->getWidth()) / 2,
            (border[0].y - t->getHeight()) / 2);

  delete t;

  /* Render the enemy level */
  if(name != "Arcadius")
  {
    t = new Text(font_subheader);
    t->setText(renderer,
               "Level " + std::to_string(enemy->getBasePerson()->getLevel()),
               color);
    t->render(renderer, (kINFO_W - t->getWidth()) / 2,
              border[0].y + bar_height +
                  (kINFO_H - border[0].y - bar_height - t->getHeight()) / 2);
    delete t;
  }

  /* Set the new frame */
  Frame* enemy_info = new Frame();
  enemy_info->setTexture(texture);

  /* Clear render connection */
  SDL_SetRenderTarget(renderer, nullptr);

  /* Set the info frame */
  enemy->setInfoFrame(enemy_info);
}

void Battle::clearBackground()
{
  if(background)
    delete background;

  background = nullptr;
}

void Battle::clearElements()
{
  for(auto& element : render_elements)
  {
    if(element)
      delete element;

    element = nullptr;
  }

  render_elements.clear();
}

void Battle::clearEnemyBackdrop()
{
  if(frame_enemy_backdrop)
    delete frame_enemy_backdrop;

  frame_enemy_backdrop = nullptr;
}

void Battle::clearElementsTimedOut()
{
  render_elements.erase(
      std::remove_if(begin(render_elements), end(render_elements),
                     [&](RenderElement* element) -> bool
                     {
                       if(element)
                       {
                         return (element->status == RenderStatus::TIMED_OUT);
                       }
                       return true;
                     }),
      end(render_elements));
}

void Battle::playInflictionSound(Infliction type)
{
  if(type == Infliction::CONFUSE)
  {
    event_handler->triggerSound(Sound::kID_SOUND_BTL_CONFUSE,
                                SoundChannels::TRIGGERS);
  }
  else if(type == Infliction::POISON)
  {
    event_handler->triggerSound(Sound::kID_SOUND_BTL_POISON,
                                SoundChannels::TRIGGERS);
  }
  else if(type == Infliction::HIBERNATION)
  {
    event_handler->triggerSound(Sound::kID_SOUND_BTL_HIBERNATE,
                                SoundChannels::TRIGGERS);
  }
  else if(type == Infliction::SILENCE)
  {
    event_handler->triggerSound(Sound::kID_SOUND_BTL_SILENCE,
                                SoundChannels::TRIGGERS);
  }
  else
  {
    event_handler->triggerSound(Sound::kID_SOUND_BTL_RAISE,
                                SoundChannels::TRIGGERS);
  }
}

void Battle::createOverlay(std::string path, int anim_time, 
                           float velocity_x, float velocity_y)
{
  if(path != "" && config)
  {
    Box location;
    location.point.x = 0;
    location.point.y = 0;
    location.width = config->getScreenWidth();
    location.height = config->getScreenHeight();

    Floatinate velocity(velocity_x, velocity_y);

    // TODO: NEED TO REVISE FOR ANIMATION BASED SPRITE OVERLAY
    // "sprites/Battle/Midlays/rain|5|.png" with 175ms time and same location,
    // velocity for example
    auto element = new RenderElement(renderer, path, location,
                                     RenderType::OVERLAY, velocity, 245);

    render_elements.push_back(element);
  }
}

void Battle::createMidlay(std::string path, int anim_time, 
                          float velocity_x, float velocity_y)
{
  if(path != "" && config)
  {
    Box location;
    location.point.x = 0;
    location.point.y = 704;
    location.width = config->getScreenWidth();
    location.height = config->getScreenHeight();

    Floatinate velocity(velocity_x, velocity_y);
    
    // TODO: NEED TO REVISE FOR ANIMATION BASED SPRITE MIDLAY
    // "sprites/Battle/Midlays/rain|5|.png" with 175ms time and same location,
    // velocity for example
    auto element = new RenderElement(renderer, path, location,
                                     RenderType::MIDLAY, velocity, 245);

    render_elements.push_back(element);

    location.point.x = 1216;
    location.point.y = 704;

    element = new RenderElement(renderer, path, location, RenderType::MIDLAY,
                                velocity, 245);
    render_elements.push_back(element);
  }
}

// NOTE: On menu rendering, render the friend info of the selected ally
// ex. //
/* Render the selecting person info */
// uint16_t x = (section1_w - kINFO_W) / 2;
// uint16_t y = screen_height - bar_height + (bar_height - kALLY_HEIGHT) / 2;
//   success &= renderFriendInfo(screen_height, x, y, true);

// Other todos:
bool Battle::render()
{
  auto success = false;

  if(config && turn_state != TurnState::FINISHED)
  {
    auto height = config->getScreenHeight();
    auto width = config->getScreenWidth();

    success = true;

    /* Bottom layer is the background */
    if(background)
      success &= background->render(renderer, 0, 0, width, height);

    // FUTURE - Render overlays here.

    /* Render the enemies in their present state */
    success &= renderEnemies();
    success &= renderEnemiesInfo();

    /* Render the midlays */
    renderMidlays();

    /* Render the allies in their present states */
    success &= renderAllies();

    /* Render the action frame */
    if(turn_state == TurnState::PROCESS_ACTIONS)
      success &= renderActionFrame();
    
    /* Render the overlays */
    renderOverlays();

    /* Render battle bar (on bottom) */
    success &= renderBattleBar();

    /* Render the menu (if needed) */
    success &= renderMenu();

    /* Render the render elements (damage text values) etc. */
    success &= renderElements();

    if(victory_screen)
      victory_screen->render();
  }

  return success;
}

bool Battle::renderActionFrame()
{
  auto success = true;

  for(const auto& actor : actors)
  {
    if(renderer && actor &&
       actor->getStateActionFrame() != SpriteState::HIDDEN &&
       actor->getStateActionFrame() != SpriteState::SLID_OUT)
    {
      auto frame = actor->getActionFrame();
      auto x = actor->getActionFrameX();
      auto y = actor->getActionFrameY();

      if(frame)
        success &= frame->render(renderer, x, y);
    }
  }

  return success;
}

bool Battle::renderBattleBar()
{
  if(renderer && battle_display_data && battle_display_data->isDataBuilt())
  {
    auto frame = battle_display_data->getBattleBar();

    frame->render(renderer, 0,
                  config->getScreenHeight() - kBIGBAR_OFFSET - bar_offset,
                  config->getScreenWidth());

    return true;
  }

  return false;
}

bool Battle::renderAilmentsActor(BattleActor* actor, uint32_t x, uint32_t y,
                                 bool full_border)
{
  /* Big problems if calling with nullptr actor */
  assert(actor);

  /* Get ailment(s) and render holding box */
  auto ailments = actor->getAilments();

  if(ailments.size() > 0 && ailments.front())
  {
    /* Front ailment frame */
    auto fr = battle_display_data->getFrameAilment(ailments.front()->getType());

    /* Sizing variables */
    uint16_t gap = kAILMENT_GAP;
    uint16_t border = kAILMENT_BORDER;

    /* Render rectangle */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, kAILMENT_OPACITY);
    SDL_Rect rect;
    rect.w = (fr->getWidth() + gap * 2) * ailments.size() + border * 2;
    rect.h = (fr->getHeight() + gap * 2) + border;
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h;
    SDL_RenderFillRect(renderer, &rect);

    /* Render rectangle border */
    if(!actor->getFlag(ActorState::ALLY))
      SDL_SetRenderDrawColor(renderer, kINFO_GREY, kINFO_GREY, kINFO_GREY, 255);
    else
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Point points[5];
    points[0].x = rect.x;
    points[0].y = rect.y + rect.h - 1;
    points[1].x = rect.x;
    points[1].y = rect.y;
    points[2].x = rect.x + rect.w - 1;
    points[2].y = rect.y;
    points[3].x = points[2].x;
    points[3].y = points[0].y;
    if(full_border)
    {
      points[0].y += 1;
      points[3].y = points[0].y;
      points[4].x = points[0].x;
      points[4].y = points[0].y;
      SDL_RenderDrawLines(renderer, points, 5);
    }
    else
    {
      SDL_RenderDrawLines(renderer, points, 4);
    }

    /* Render the ailments */
    rect.x += border + gap;
    rect.y += border + gap;

    for(auto& ailment : ailments)
    {
      if(ailment)
      {
        auto f = battle_display_data->getFrameAilment(ailment->getType());
        f->render(renderer, rect.x, rect.y);
        rect.x += f->getWidth() + gap * 2;
      }
    }

    return true;
  }

  return false;
}

// TODO
bool Battle::renderAllies()
{
  bool success = true;

  /* Render each ally */
  for(auto& ally : getAllies())
  {
    success &= ally->getActiveSprite()->render(renderer, getActorX(ally),
                                               getActorY(ally));
  }

  return success;
}

bool Battle::renderElements()
{
  for(auto& element : render_elements)
  {
    assert(element);
    auto ty = element->render_type;

    if(ty == RenderType::ACTION_TEXT || ty == RenderType::DAMAGE_VALUE ||
       ty == RenderType::DAMAGE_TEXT)
      renderElementText(element);
    else if(ty == RenderType::RGB_OVERLAY)
      renderElementRGBOverlay(element);
    else if(ty == RenderType::PLEP)
      renderElementPlep(element);
  }

  for(auto& element : render_elements)
  {
    if(element->render_type == RenderType::ENTER_TEXT ||
       element->render_type == RenderType::VICTORY_TEXT)
      renderElementText(element);
  }

  return false;
}

void Battle::renderElementPlep(RenderElement* element)
{
  if(element->element_sprite)
  {
    auto point = element->location.point;
    element->element_sprite->render(renderer, point.x, point.y);
  }
}

void Battle::renderElementRGBOverlay(RenderElement* element)
{
  auto color = element->color;
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, element->alpha);
  SDL_Rect overlay_rect;
  overlay_rect.x = element->location.point.x;
  overlay_rect.y = element->location.point.y;
  overlay_rect.w = element->location.width;
  overlay_rect.h = element->location.height;
  SDL_RenderFillRect(renderer, &overlay_rect);
}

void Battle::renderElementText(RenderElement* element)
{
  if(element->element_font)
  {
    Text t(element->element_font);
    auto point = element->location.point;
    auto shadow = element->shadow_offset;

    t.setText(renderer, element->text_string, element->color);
    t.setAlpha(element->alpha);
    t.render(renderer, point.x, point.y);

    if(element->has_shadow)
    {
      t.setText(renderer, element->text_string, element->shadow_color);
      t.setAlpha(element->alpha);
      t.render(renderer, point.x + shadow.x, point.y + shadow.y);
    }
  }
}

// TODO
bool Battle::renderEnemies()
{
  bool success = true;

  /* Render each enemy */
  for(const auto& enemy : getEnemies())
  {
    if(enemy)
    {
      auto active_sprite = enemy->getActiveSprite();
      auto x_factor = 1.00;
      auto y_factor = 1.00;

      if(event && event->actor == enemy)
      {
        x_factor = 1.09;
        y_factor = 1.07;
      }

      if(active_sprite && active_sprite->getCurrent())
      {
        auto old_w = active_sprite->getCurrent()->getWidth();
        auto old_h = active_sprite->getCurrent()->getHeight();

        auto w = (int)(old_w * x_factor);
        auto h = (int)(old_h * y_factor);

        success &= enemy->getActiveSprite()->render(
            renderer, getActorX(enemy) - (w - old_w),
            getActorY(enemy) + (h - old_h), w, h);
      }
    }
  }

  return success;
}

// TODO: Comment
bool Battle::renderEnemiesInfo()
{
  bool success = true;
  int32_t y = 0;

  /* Render the box */
  if(frame_enemy_backdrop)
    y = kENEMIES_OFFSET - kENEMIES_BAR_GAP - frame_enemy_backdrop->getHeight();
  else
    success = false;

  for(auto& enemy : getEnemies())
  {
    bool to_render = enemy && enemy->getInfoFrame() && enemy->getBasePerson();

    if(to_render)
    {
      /* Calculate health bar amount and color */
      auto x = getActorX(enemy);

      /* Render enemy backdrop */
      success &= frame_enemy_backdrop->render(renderer, x, y);

      /* Get the percent of vitality, and set it at least at 1% */
      auto health_pc = (float)enemy->getPCVita() / 100.0;
      health_pc = Helpers::setInRange(health_pc, 0.0, 1.0);

      setupHealthDraw(enemy, health_pc);

      /* Calculate health bar render amount */
      auto health_am = (kENEMY_BAR_W + kENEMY_BAR_TRIANGLE) * health_pc;

      if(health_am == 0 && health_pc > 0.0)
        health_am = 1;
      else if(health_am == (kENEMY_BAR_W + kENEMY_BAR_TRIANGLE) &&
              health_pc < 1.0)
        --health_am;

      /* Render health bar */
      Frame::renderBar(x + (kINFO_W - kENEMY_BAR_W) / 2 + 1,
                       y + (kINFO_H - kENEMY_BAR_H) / 2 + kENEMY_BAR_OFFSET,
                       health_am, kENEMY_BAR_H,
                       (float)kENEMY_BAR_TRIANGLE / kENEMY_BAR_H, renderer);

      /* Render foe info */
      success &= enemy->getInfoFrame()->render(renderer, x, y);

      /* Render ailments */
      renderAilmentsActor(enemy, x + kINFO_W / 2, y, true);
    }
  }

  return success;
}

void Battle::renderMidlays()
{
  for(auto& element : render_elements)
  {
    if(element && element->render_type == RenderType::MIDLAY &&
       element->element_sprite)
    {
      auto point = element->location.point;
      element->element_sprite->render(renderer,
                                      point.x - config->getScreenWidth(),
                                      point.y - config->getScreenHeight());
    }
  }
}

bool Battle::renderMenu()
{
  auto width = config->getScreenWidth();
  auto success = true;

  /* Determine whether the menu should be rendered */
  auto to_render_menu = (turn_state == TurnState::SELECT_ACTION_ALLY);
  auto curr_layer = battle_menu->getMenuLayer();
  to_render_menu &= !getFlagCombat(CombatState::PHASE_DONE);
  to_render_menu &= !(curr_layer == BattleMenuLayer::TARGET_SELECTION);

  if(to_render_menu)
  {
    if(battle_menu->getFlag(BattleMenuState::READY))
    {
      if(!battle_menu->getFlag(BattleMenuState::SKILL_FRAMES_BUILT))
        battle_menu->createSkillFrames(width * kBIGBAR_M2, width * kBIGBAR_R);
      if(!battle_menu->getFlag(BattleMenuState::ITEM_FRAMES_BUILT))
        battle_menu->createItemFrames(width * kBIGBAR_M2, width * kBIGBAR_R);
    }

    /* Render the selecting person info */
    if(battle_menu->getActor())
      success &= renderAllyInfo(battle_menu->getActor(), true);

    /* Render the menu */
    success &= battle_menu->render();
  }
  else
  {
    success &= renderAlliesInfo();
  }

  return success;
}

void Battle::renderOverlays()
{
  for(auto& element : render_elements)
  {
    if(element && element->render_type == RenderType::OVERLAY &&
       element->element_sprite)
    {
      auto point = element->location.point;
      element->element_sprite->render(renderer, point.x, point.y);
    }
  }
}

// TODO: Comment
bool Battle::renderAllyInfo(BattleActor* ally, bool for_menu)
{
  auto font_subheader = config->getFontTTF(FontName::BATTLE_SUBHEADER);
  bool success = true;

  auto x = 0;
  auto y = 0;

  if(for_menu)
  {
    auto b_height = kBIGBAR_OFFSET + kBIGBAR_CHOOSE;
    x = ((config->getScreenWidth() * kBIGBAR_L) - kINFO_W) / 2;
    y = config->getScreenHeight() - b_height + (b_height - kALLY_HEIGHT) / 2;
  }
  else
  {
    x = (ally->getIndex() * kPERSON_SPREAD) + (kPERSON_WIDTH - kINFO_W) / 2;
    y = config->getScreenHeight() - kALLY_HEIGHT;
  }

  /* Get the percent of vitality, and set it at least at 1% */
  auto health_pc = (float)ally->getPCVita() / 100.0;
  health_pc = Helpers::setInRange(health_pc, 0.0, 1.0);

  setupHealthDraw(ally, health_pc);

  uint16_t health_x = x + (kINFO_W - kALLY_HEALTH_W) / 2;
  uint16_t health_y = y + (kALLY_HEIGHT - kALLY_HEALTH_H) / 2;

  /* Calculate health bar render amount */
  int health_amount = (kALLY_HEALTH_W + kALLY_HEALTH_TRIANGLE - 1) * health_pc;

  if(health_amount == 0 && health_pc > 0.0)
    health_amount = 1;
  else if(health_amount == (kALLY_HEALTH_W + kALLY_HEALTH_TRIANGLE) &&
          health_pc < 1.0)
    health_amount--;

  /* Render health bar */
  success &=
      Frame::renderBar(health_x + 1, health_y, health_amount, kALLY_HEALTH_H,
                       (float)kALLY_HEALTH_TRIANGLE / kALLY_HEALTH_H, renderer);

  /* Render friends info */
  success &= ally->getInfoFrame()->render(renderer, x, y);

  /* Calculate qd bar amount and color */
  auto qd_percent = (float)ally->getPCQtdr() / 100.0;
  qd_percent = Helpers::setInRange(qd_percent, 0.0, 1.0);
  SDL_SetRenderDrawColor(renderer, 58, 170, 198, 255);
  uint16_t qd_x = health_x + kALLY_HEALTH_W - kALLY_QD_OFFSET -
                  kALLY_QD_TRIANGLE - kALLY_QD_W;
  uint16_t qd_y = health_y + kALLY_HEALTH_H - (kALLY_QD_H / 2);

  /* Calculate qd bar render amount */
  int qd_amount = (kALLY_QD_W + kALLY_QD_TRIANGLE - 1) * qd_percent;
  if(qd_amount == 0 && qd_percent > 0.0)
    qd_amount = 1;
  else if(qd_amount == (kALLY_QD_W + kALLY_QD_TRIANGLE) && qd_percent < 1.0)
    qd_amount--;

  /* Render the qd bar */
  success &= Frame::renderBar(qd_x, qd_y, qd_amount + 2, kALLY_QD_H,
                              (float)kALLY_QD_TRIANGLE / kALLY_QD_H, renderer);

  /* Render the qd bar border */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Point border[5];
  border[0].x = qd_x;
  border[0].y = qd_y;
  border[1].x = border[0].x + kALLY_QD_W + kALLY_QD_TRIANGLE;
  border[1].y = border[0].y;
  border[2].x = border[1].x - kALLY_QD_TRIANGLE;
  border[2].y = border[1].y + kALLY_QD_H;
  border[3].x = border[2].x - kALLY_QD_W - kALLY_QD_TRIANGLE;
  border[3].y = border[2].y;
  border[4].x = border[0].x;
  border[4].y = border[0].y;
  SDL_RenderDrawLines(renderer, border, 5);

  /* Health Text Amount */
  SDL_Color color = {255, 255, 255, 255};
  Text* t = new Text(font_subheader);
  success &= t->setText(
      renderer,
      std::to_string(ally->getStatsRendered().getValue(Attribute::VITA)),
      color);
  success &= t->render(
      renderer, health_x + (kALLY_HEALTH_W - t->getWidth()) / 2, health_y);

  /* QD Text Amount */
  success &= t->setText(
      renderer,
      std::to_string(ally->getStatsRendered().getValue(Attribute::QTDR)),
      color);
  success &= t->render(renderer, qd_x + (kALLY_QD_W - t->getWidth()) / 2, qd_y);
  delete t;

  /* Render ailments */
  if(for_menu && ally->getAilments().size() > 0)
  {
    auto frame = battle_display_data->getFrameAilment(Infliction::SILENCE);
    auto frame_size = 0;

    if(frame)
      frame_size = frame->getHeight();

    auto ailment_y =
        y + kALLY_HEIGHT + kAILMENT_GAP * 2 + kAILMENT_BORDER * 2 + frame_size;

    success &= renderAilmentsActor(ally, x + kINFO_W / 2, ailment_y, true);
  }
  else if(!for_menu && ally->getAilments().size() > 0)
  {
    auto ailment_y = config->getScreenHeight() - kBIGBAR_OFFSET;
    success &= renderAilmentsActor(ally, x + (kINFO_W / 2), ailment_y, false);
  }

  return success;
}

bool Battle::setupHealthDraw(BattleActor* actor, float health_pc)
{
  if(renderer && actor && actor->getBasePerson())
  {
    if(health_pc >= 0.5)
    {
      SDL_SetRenderDrawColor(renderer, kCOLOR_BASE * ((1 - health_pc) * 2),
                             kCOLOR_BASE, 0, 255);
    }
    else
    {
      SDL_SetRenderDrawColor(renderer, kCOLOR_BASE, kCOLOR_BASE * health_pc * 2,
                             0, 255);
    }

    return true;
  }

  return false;
}

bool Battle::renderAlliesInfo()
{
  bool success = true;

  for(const auto& ally : getAllies())
    if(ally && ally->getBasePerson() && ally->getInfoFrame())
      success &= renderAllyInfo(ally);

  return success;
}

void Battle::updateBarOffset()
{
  if(turn_state == TurnState::SELECT_ACTION_ALLY)
  {
    if(battle_menu->getMenuLayer() == BattleMenuLayer::TARGET_SELECTION ||
       !battle_menu->getFlag(BattleMenuState::READY))
    {
      bar_offset = 0;
    }
    else
      bar_offset = kBIGBAR_CHOOSE;
  }
  else
    bar_offset = 0;
}

void Battle::updateRendering(int32_t cycle_time)
{
  /* Update render elements */
  for(auto& element : render_elements)
    if(element)
      element->update(cycle_time);

  updateRenderSprites(cycle_time);
  updateBarOffset();
}

void Battle::updateRenderSprites(int32_t cycle_time)
{
  auto brightness = 1.0;

  for(auto& actor : actors)
  {
    if(actor && actor->getActiveSprite())
    {
      actor->update(cycle_time);

      if(turn_state == TurnState::SELECT_ACTION_ALLY)
      {
        brightness = 0.25;

        /* Brightness for selecting person */
        if(battle_menu->getMenuLayer() != BattleMenuLayer::TARGET_SELECTION)
        {
          if(battle_menu->getActor() == actor)
          {
            brightness = 0.3 * sin((float)time_elapsed * kCYCLE_RATE);
            brightness = 0.7 + fabs(brightness);
          }
        }
        /* Brightness for selected targets */
        else if(battle_menu->getMenuLayer() ==
                BattleMenuLayer::TARGET_SELECTION)
        {
          if(actor->getFlag(ActorState::MENU_HOVERED))
            brightness = 1.0;
        }
      }

      actor->getActiveSprite()->setBrightness(brightness);
    }
  }
}

void Battle::upkeepAilmentClear()
{
  upkeep_actor->startFlashing(FlashingType::RELIEVE, 750);
  upkeep_actor->setUpkeepState(UpkeepState::AILMENT_OUTCOME);

  // Remove the ailment - How to deal with consequences?
  upkeep_actor->removeAilment(upkeep_ailment);
  setFlagCombat(CombatState::AILMENT_CLEARS, true);
  addDelay(400);
}

void Battle::upkeepAilmentDamage()
{
  // TODO [12-04-15] -- Damage types for different ailments
  if(upkeep_ailment->getType() == Infliction::POISON)
  {
    createDamageElement(upkeep_actor, DamageType::POISON,
                        upkeep_ailment->getDamageAmount());
  }
  else if(upkeep_ailment->getType() == Infliction::HIBERNATION)
  {
    createDamageElement(upkeep_actor, DamageType::HIBERNATION_REGEN,
                        upkeep_ailment->getDamageAmount());
  }

  upkeep_actor->setUpkeepState(UpkeepState::AILMENT_OUTCOME);
  addDelay(300);
}

void Battle::upkeepAilmentFlash()
{
  // TODO [12-04-15] -- Flashing types for different ailments
  if(upkeep_ailment->getType() == Infliction::POISON)
  {
    upkeep_actor->startFlashing(FlashingType::POISON, 750);
  }
  else
    upkeep_actor->startFlashing(FlashingType::INFLICT, 750);

  upkeep_actor->setUpkeepState(UpkeepState::AILMENT_DAMAGE);
  addDelay(750);
}

void Battle::upkeepAilmentOutcome()
{
  /* If the ailment has caused amage, deal the damage that it is caused */
  if(upkeep_ailment->getDamageAmount() > 0)
  {
    if(upkeep_ailment->getType() == Infliction::HIBERNATION)
    {
      upkeep_actor->restoreVita(upkeep_ailment->getDamageAmount());
      upkeep_actor->setFlag(ActorState::SELECTION_SKIP, true);
    }
    /* Check if the ailment damage kills the actor */
    else if(upkeep_actor->dealDamage(upkeep_ailment->getDamageAmount()) &&
            upkeep_ailment->getFlag(AilState::CURABLE_KO))
    {
      event_handler->triggerSound(Sound::kID_SOUND_BTL_DEATH,
                                  SoundChannels::TRIGGERS);
      upkeep_actor->startFlashing(FlashingType::KOING);
      upkeep_actor->removeAilmentsKO();
      addDelay(1200);
    }
  }
  else
  {
    addDelay(100);
  }

  upkeep_ailment->setUpdateStatus(AilmentStatus::COMPLETED);

  if(getFlagCombat(CombatState::AILMENT_CLEARS))
    upkeep_actor->clearAilment(upkeep_ailment);

  /* Unset the upkeep ailment for updatePersonalAilments() to grab the next */
  upkeep_ailment = nullptr;
}

void Battle::upkeepAilmentPlep()
{
  auto type = upkeep_ailment->getType();
  auto plep = battle_display_data->getPlepAilment(type);

  /* Create the plep on the upkeep actor */
  if(plep)
  {
    playInflictionSound(type);
    auto x = getActorX(upkeep_actor);
    auto y = getActorY(upkeep_actor);
    render_elements.push_back(new RenderElement(renderer, plep, 2, x, y));
  }

  upkeep_ailment->setUpdateStatus(AilmentStatus::COMPLETED);
  upkeep_actor->setUpkeepState(UpkeepState::AILMENT_FLASH);

  // TODO: Delay based on the animation time?
  addDelay(450);
}

int32_t Battle::getActorX(BattleActor* actor)
{
  if(actor && actor->getFlag(ActorState::ALLY))
  {
    for(const auto& ally : getAllies())
      if(ally == actor)
        return actor->getIndex() * kPERSON_SPREAD;
  }
  else if(actor)
  {
    for(const auto& enemy : getEnemies())
    {
      if(enemy == actor)
      {
        return config->getScreenWidth() - kPERSON_WIDTH -
               std::abs(actor->getIndex()) * kPERSON_SPREAD;
      }
    }
  }

  return -1;
}

int32_t Battle::getActorY(BattleActor* actor)
{
  if(actor && actor->getFlag(ActorState::ALLY))
  {
    for(const auto& ally : getAllies())
      if((ally == actor) && config)
        return config->getScreenHeight() - kALLIES_OFFSET;
  }
  else if(actor)
  {
    for(const auto& enemy : getEnemies())
      if((enemy == actor))
        return kENEMIES_OFFSET;
  }

  return -1;
}

/*=============================================================================
 * PUBLIC FUNCTIONS - Battle Operations
 *============================================================================*/

bool Battle::keyDownEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_INSERT)
  {
    if(party_allies && party_allies->getInventory())
      party_allies->getInventory()->print(false);
  }

  if(turn_state == TurnState::SELECT_ACTION_ALLY)
    battle_menu->keyDownEvent();
  else
  {
    if(event.keysym.sym == SDLK_DELETE && battle_buffer)
      battle_buffer->print(false);
    else if(event.keysym.sym == SDLK_INSERT && battle_buffer)
      battle_buffer->print(true);
  }

  return false;
}

bool Battle::startBattle(Party* friends, Party* foes, std::string bg_path)
{
  /* Assert  all essentials are not nullptr. We want Battle to fail */
  assert(battle_display_data);
  assert(config);
  assert(event_handler);
  assert(renderer);
  assert(friends);
  assert(foes);

  party_allies = friends;
  party_enemies = foes;

  /* Music trigger */
  if(music_id >= 0)
    event_handler->triggerMusic(music_id);
  else
    event_handler->triggerAudioStop(SoundChannels::MUSIC1);

  /* Construct the Battle actor objects based on the persons in the parties */
  buildBattleActors(friends, foes);

  /* Build ally and enemy info frames, action frames */
  for(auto& actor : actors)
  {
    buildActionFrame(actor);

    if(actor->getFlag(ActorState::ALLY))
      buildInfoAlly(actor);
    else
      buildInfoEnemy(actor);
  }

  /* `et the sprite for the Battle background */
  // setBackground(bg_path);

  // TODO: [01-09-2015]: Temporary background building
  //setBackground(bg_path + "sprites/Battle/Backdrop/battlebg06.png");

  // Create an midlay
  //auto path =
  //    bg_path + "sprites/Map/EnviromentEffects/Overlays/smog_overlay.png";
  //createMidlay(path, 0.10, 0.000);

  /* Construct the enemy backdrop */
  buildEnemyBackdrop();

  turn_state = TurnState::BEGIN;
  delay = 0;

  return true;
}

void Battle::stopBattle()
{
  /* Destroy the battle actors at the end of a Battle */
  clearEvent();
  clearBattleActors();
  clearBackground();
  clearElements();
  clearEnemyBackdrop();

  if(battle_menu)
    battle_menu->clear();

  if(battle_buffer)
    battle_buffer->clear();

  flags_combat = static_cast<CombatState>(0);
  delay = 0;
  music_id = Sound::kID_MUSIC_BATTLE;
  outcome = OutcomeType::NONE;
  turn_state = TurnState::STOPPED;
  time_elapsed = 0;
  turns_elapsed = 0;
  party_allies = nullptr;
  party_enemies = nullptr;
  upkeep_actor = nullptr;
  upkeep_ailment = nullptr;
  victory_screen = nullptr;
}

std::vector<BattleActor*> Battle::getAllies()
{
  std::vector<BattleActor*> temp_actors;

  for(const auto& actor : actors)
    if(actor->getFlag(ActorState::ALLY))
      temp_actors.push_back(actor);

  return temp_actors;
}

std::vector<BattleActor*> Battle::getEnemies()
{
  std::vector<BattleActor*> temp_actors;

  for(const auto& actor : actors)
    if(!actor->getFlag(ActorState::ALLY))
      temp_actors.push_back(actor);

  return temp_actors;
}

bool Battle::getFlagCombat(CombatState test_flag)
{
  return static_cast<bool>((flags_combat & test_flag) == test_flag);
}

bool Battle::getFlagRender(RenderState test_flag)
{
  return static_cast<bool>((flags_render & test_flag) == test_flag);
}

OutcomeType Battle::getOutcomeType()
{
  return outcome;
}

TurnState Battle::getTurnState()
{
  return turn_state;
}

bool Battle::setConfig(Options* config)
{
  bool success{config};

  this->config = config;

  if(battle_menu)
    success &= battle_menu->setConfig(config);

  return success;
}

bool Battle::setDisplayData(BattleDisplayData* battle_display_data)
{
  bool success{config};

  this->config = config;

  if(battle_menu)
    success &= battle_menu->setConfig(config);

  if(battle_display_data)
  {
    this->battle_display_data = battle_display_data;

    if(battle_menu)
      return battle_menu->setDisplayData(battle_display_data);
  }

  return success;
}

bool Battle::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;

  if(battle_menu && this->event_handler)
    battle_menu->setEventHandler(event_handler);

  return this->event_handler;
}

void Battle::setFlagCombat(CombatState flag, const bool& set_value)
{
  (set_value) ? (flags_combat |= flag) : (flags_combat &= ~flag);
}

void Battle::setFlagRender(RenderState flag, const bool& set_value)
{
  (set_value) ? (flags_render |= flag) : (flags_render &= ~flag);
}

/*=============================================================================
 * PUBLIC FUNCTIONS - Battle Display
 *============================================================================*/

bool Battle::setRenderer(SDL_Renderer* renderer)
{
  bool success{renderer};

  this->renderer = renderer;

  if(battle_menu)
    success &= battle_menu->setRenderer(renderer);

  return success;
}

bool Battle::setBackground(std::string bg_path)
{
  clearBackground();

  if(bg_path != "")
  {
    background = new Sprite(bg_path, renderer);
    background->setNonUnique(true, 1);
    background->createTexture(renderer);
  }

  return false;
}

/* Assigns the music trigger ID */
void Battle::setMusicID(int id)
{
  music_id = id;
}

void Battle::setNextTurnState()
{
  /* Set the CURRENT_STATE to incomplete */
  setFlagCombat(CombatState::PHASE_DONE, false);

  if(outcome != OutcomeType::NONE)
  {
    if(turn_state != TurnState::OUTCOME)
      turn_state = TurnState::OUTCOME;
    else
      turn_state = TurnState::FINISHED;
  }
  else if(turn_state == TurnState::BEGIN)
    turn_state = TurnState::ENTER_DIM;
  else if(turn_state == TurnState::ENTER_DIM)
    turn_state = TurnState::FADE_IN_TEXT;
  else if(turn_state == TurnState::FADE_IN_TEXT)
    turn_state = TurnState::SELECT_ACTION_ALLY;
  else if(turn_state == TurnState::GENERAL_UPKEEP)
    turn_state = TurnState::UPKEEP;
  else if(turn_state == TurnState::UPKEEP)
    turn_state = TurnState::SELECT_ACTION_ALLY;
  else if(turn_state == TurnState::SELECT_ACTION_ALLY)
    turn_state = TurnState::SELECT_ACTION_ENEMY;
  else if(turn_state == TurnState::SELECT_ACTION_ENEMY)
    turn_state = TurnState::PROCESS_ACTIONS;
  else if(turn_state == TurnState::PROCESS_ACTIONS)
    turn_state = TurnState::CLEAN_UP;
  else if(turn_state == TurnState::CLEAN_UP)
    turn_state = TurnState::GENERAL_UPKEEP;
  else if(turn_state == TurnState::OUTCOME)
    turn_state = TurnState::FINISHED;

  std::cout << "[Turn] " << Helpers::turnStateToStr(turn_state) << std::endl;
}

bool Battle::update(int32_t cycle_time)
{
  time_elapsed += cycle_time;

  updateDelay(cycle_time);
  updateRendering(cycle_time);

  if(turn_state != TurnState::FINISHED && turn_state != TurnState::STOPPED)
  {
    if(outcome == OutcomeType::NONE && delay == 0)
      checkIfOutcome();

    bool allow_set_next = true;

    if(event && event->action_state != ActionState::OUTCOME)
      allow_set_next = false;
    else if(!getFlagCombat(CombatState::PHASE_DONE))
      allow_set_next = false;
    else if(delay > 0)
      allow_set_next = false;

    if(allow_set_next)
      setNextTurnState();

    if(delay == 0)
    {
      if(turn_state == TurnState::BEGIN)
        updateBegin();
      else if(turn_state == TurnState::ENTER_DIM)
        updateScreenDim();
      else if(turn_state == TurnState::FADE_IN_TEXT)
        updateFadeInText();
      else if(turn_state == TurnState::GENERAL_UPKEEP)
        updateGeneralUpkeep();
      else if(turn_state == TurnState::UPKEEP)
        updatePersonalUpkeep();
      else if(turn_state == TurnState::SELECT_ACTION_ALLY)
        updateUserSelection();
      else if(turn_state == TurnState::SELECT_ACTION_ENEMY)
        updateEnemySelection();
      else if(turn_state == TurnState::PROCESS_ACTIONS)
        updateProcessing();
      else if(turn_state == TurnState::CLEAN_UP)
        cleanUpTurn();
      else if(turn_state == TurnState::OUTCOME)
        updateOutcome(cycle_time);
    }
  }

  clearElementsTimedOut();

  return false;
}
