/*******************************************************************************
* Class Name: Battle
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

/*=============================================================================
 * CONSTANTS - Battle Operations
 *============================================================================*/

/* Maximum Ailments (Total)
 * Maximum Each Ailments (Per Person)
 * Minimum Damage (Possible)
 * Maximum Damage (Possible)
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
const uint16_t Battle::kMINIMUM_DAMAGE = 1;
const uint16_t Battle::kMAXIMUM_DAMAGE = 29999;

const float Battle::kBASE_RUN_CHANCE = 0.25;
const float Battle::kUSER_RUN_MODIFIER = 2.00;
const float Battle::kALLY_RUN_MODIFIER = 1.00;
const float Battle::kENEMY_RUN_MODIFIER = 1.00;
const float Battle::kRUN_PC_PER_POINT = 0.003;

const int16_t Battle::kREGEN_RATE_ZERO_PC = 0;
const int16_t Battle::kREGEN_RATE_WEAK_PC = 3;
const int16_t Battle::kREGEN_RATE_NORMAL_PC = 4;
const int16_t Battle::kREGEN_RATE_STRONG_PC = 5;
const int16_t Battle::kREGEN_RATE_GRAND_PC = 6;

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

const uint16_t Battle::kACTION_BORDER = 10;
const uint16_t Battle::kACTION_CENTER = 381;
const uint16_t Battle::kACTION_COLOR_A = 175;
const uint16_t Battle::kACTION_COLOR_G = 201;
const uint16_t Battle::kACTION_COLOR_R = 175;
const uint16_t Battle::kACTION_CORNER_X = 18;
const uint16_t Battle::kACTION_CORNER_Y = 4;
const uint16_t Battle::kACTION_H = 408;
const uint16_t Battle::kACTION_TEXT_SHADOW = 3;
const uint16_t Battle::kACTION_TEXT_X = 800;
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
const uint8_t Battle::kPERSON_KO_ALPHA = 50;

// const uint8_t Battle::kTYPE_MARGIN = 7;
// const uint8_t Battle::kTYPE_MAX = 5;
// const uint8_t Battle::kTYPE_SELECT = 3;

/* ---- Color Constants ---- */
const SDL_Color Battle::kSTRD_DMG_COLOR = {225, 225, 225, 255};
const SDL_Color Battle::kCRIT_DMG_COLOR = {255, 255, 0, 255};
const SDL_Color Battle::kPOIS_DMG_COLOR = {138, 43, 226, 255};
const SDL_Color Battle::kBURN_DMG_COLOR = {172, 0, 0, 255};
const SDL_Color Battle::kVITA_REGEN_COLOR = {50, 205, 50, 255};
const SDL_Color Battle::kQTDR_REGEN_COLOR = {0, 128, 255, 255};
const SDL_Color Battle::kHIBERNATION_REGEN_COLOR = {75, 205, 50, 255};

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
      outcome{OutcomeType::NONE},
      renderer{nullptr},
      turn_state{TurnState::STOPPED},
      time_elapsed{0},
      turns_elapsed{0}
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

// ALPHA
// Constructs AI
void Battle::aiBuild()
{
  for(auto& enemy : getEnemies())
  {
    if(enemy && enemy->getBasePerson())
    {
      if(!enemy->getBasePerson()->getAI())
      {
        AIModule* new_ai_module = new AIModule();
        new_ai_module->setParent(enemy);
        enemy->getBasePerson()->setAI(new_ai_module);
      }
    }
  }
}

// ALPHA
// Clears AI.
void Battle::aiClear()
{
  for(auto& enemy : getEnemies())
  {
    if(enemy && enemy->getBasePerson() && enemy->getBasePerson()->getAI())
    {
      auto module = enemy->getBasePerson()->getAI();
      delete module;
      module = nullptr;
    }
  }
}

bool Battle::bufferMenuSelection()
{
  std::cout << "Buffering menu selection!" << std::endl;

  auto success = true;
  auto actor = battle_menu->getActor();
  auto action_type = battle_menu->getSelectedType();
  auto targets = battle_menu->getTargetsSelected();

  success &= (actor != nullptr);

  if(success && action_type == ActionType::SKILL)
  {
    auto battle_skill = battle_menu->getSelectedBattleSkill();

    success &= (battle_skill && battle_skill->skill);

    if(success)
    {
      auto skill = battle_skill->skill;
      auto cooldown = skill->getCooldown();

      battle_buffer->addSkill(actor, skill, targets, cooldown, turns_elapsed);
    }
  }

  return success;
}

bool Battle::bufferModuleSelection()
{
  auto success = true;
  auto curr_actor = getCurrentModuleActor();
  auto curr_module = getCurrentModule();

  if(curr_actor && curr_module)
  {
    std::cout << "Buffering module selection." << std::endl;
    auto action_type = curr_module->getActionType();
    auto targets = curr_module->getChosenTargets();

    if(action_type == ActionType::DEFEND)
    {
      battle_buffer->addDefend(curr_actor);
    }
    else if(action_type == ActionType::ITEM && curr_module->getSelectedItem())
    {
      auto item = curr_module->getSelectedItem();
      battle_buffer->addItem(curr_actor, item, targets);
    }
    else if(action_type == ActionType::SKILL && curr_module->getSelectedSkill())
    {
      auto skill = curr_module->getSelectedSkill();
      auto cooldown = skill->getCooldown();
      battle_buffer->addSkill(curr_actor, skill, targets, cooldown,
                              turns_elapsed);

      /* Pay the required QTDR cost for the Skill */
      auto true_cost = curr_module->getSelectedBattleSkill()->true_cost;
      auto curr_qd = curr_actor->getStats().getBaseValue(Attribute::QTDR);

      curr_actor->getStats().setBaseValue(Attribute::QTDR, curr_qd - true_cost);
    }
    else if(action_type == ActionType::PASS)
    {
      battle_buffer->addPass(curr_actor);
    }
    else if(action_type == ActionType::RUN)
    {
      battle_buffer->addRun(curr_actor);
    }
  }
  else
  {
    success = false;
  }

  return success;
}

// TODO: Determine can run
void Battle::buildBattleActors(Party* allies, Party* enemies)
{
  for(uint32_t i = 0; i < allies->getSize(); i++)
  {
    auto new_ally = new BattleActor(allies->getMember(i), getBattleIndex(i),
                                    true, true, renderer);

    /* Add the new ally to the vector of actors of the Battle */
    actors.push_back(new_ally);
  }
  for(uint32_t i = 0; i < enemies->getSize(); i++)
  {
    auto new_enemy = new BattleActor(enemies->getMember(i), getBattleIndex(i),
                                     false, true, renderer);

    /* Add the new enemy to the vector of actors of the Battle */
    actors.push_back(new_enemy);
  }
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

void Battle::clearEvent()
{
  if(event)
    delete event;

  event = nullptr;
}

bool Battle::doesActorNeedToSelect(BattleActor* actor)
{
  bool to_select = (actor != nullptr);

  if(to_select)
    to_select &= (actor->getSelectionState() == SelectionState::NOT_SELECTED);

  return to_select;
}

void Battle::generalUpkeep()
{
#ifdef UDEBUG
  std::cout << "\n=============\n";
  std::cout << "  TURN " << Helpers::numToRoman(turns_elapsed + 1)
            << "\n=============" << std::endl;
#endif

  /* First turn -> skip updates, other turns -> add to update */
  if(turns_elapsed > 0)
  {
    for(auto& actor : actors)
      if(actor)
        actor->setUpkeepState(UpkeepState::UPKEEP_BEGIN);
  }

  setFlagCombat(CombatState::PHASE_DONE);
}

void Battle::loadBattleEvent()
{
  /* Clear the event if it's already been created */
  clearEvent();

  auto action_type = battle_buffer->getActionType();
  auto user = battle_buffer->getUser();
  auto targets = battle_buffer->getTargets();

  auto to_build = (action_type != ActionType::NONE);
  to_build &= (user != nullptr);

  if(to_build)
  {
    BattleEvent* event = new BattleEvent(action_type, user, targets);

    if(action_type == ActionType::SKILL)
    {
      event->event_skill = battle_buffer->getSkill();
    }
    else if(action_type == ActionType::ITEM)
    {
      event->event_item = battle_buffer->getItem();
    }
  }
}

bool Battle::loadMenuForActor(BattleActor* actor)
{
  bool success = (actor != nullptr);
  success &= (battle_menu != nullptr);

  if(success)
  {
    success &= actor->buildBattleItems(actors);
    success &= actor->buildBattleSkills(actors);

    if(success)
    {
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

bool Battle::calculateEnemySelection(BattleActor* next_actor,
                                     AIModule* curr_module)
{
  auto success = true;

  success &= (next_actor != nullptr);
  success &= (curr_module != nullptr);

  if(success)
  {
    success &= next_actor->buildBattleItems(actors);
    success &= next_actor->buildBattleSkills(actors);

    curr_module->setItems(next_actor->getBattleItems());
    curr_module->setSkills(next_actor->getBattleSkills());
    curr_module->calculateAction();
    curr_module->calculateTargets();
  }

  return success;
}

void Battle::updateBufferNext()
{
  auto done = battle_buffer->setNext();

  if(done)
    setFlagCombat(CombatState::PHASE_DONE);
}

void Battle::updateDelay(int32_t decrement_delay)
{
  if(delay > static_cast<uint32_t>(decrement_delay))
    delay -= decrement_delay;
  else
    delay = 0;
}

void Battle::updateElement()
{
  // if(battle_buffer->getActionType() == ActionType::SKILL)
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

void Battle::updateProcessing()
{
  assert(battle_buffer);

  /* Sort the buffer, if it hasn't been for the turn */
  if(!battle_buffer->isSorted())
    battle_buffer->reorder();

  if(delay == 0)
  {
    if(battle_buffer->isIndexProcessed())
      updateBufferNext();
    else if(battle_buffer->isIndexStarted())
      updateElement();
    else if(battle_buffer->getCooldown() == 0)
      loadBattleEvent();
    else
      updateBufferNext();
  }
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
      loadMenuForActor(next_actor);
      updateSelectingState(next_actor, false);
    }
    else
      setFlagCombat(CombatState::PHASE_DONE);
  }
}

int32_t Battle::getBattleIndex(int32_t index)
{
  if(index == 1)
    return 0;
  if(index == 0)
    return 1;

  return index;
}

AIModule* Battle::getCurrentModule()
{
  auto curr_actor = getCurrentModuleActor();

  if(curr_actor && curr_actor->getBasePerson())
    return curr_actor->getBasePerson()->getAI();

  return nullptr;
}

BattleActor* Battle::getCurrentModuleActor()
{
  for(const auto& enemy : getEnemies())
    if(enemy && enemy->getSelectionState() == SelectionState::SELECTING)
      return enemy;

  return nullptr;
}

AIModule* Battle::getModuleOfActor(BattleActor* actor)
{
  if(actor && actor->getBasePerson())
    return actor->getBasePerson()->getAI();

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
  Frame::renderBar(qd_x, qd_y, qd_width + qd_triangle, qd_height,
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
  t->setText(renderer, enemy->getBasePerson()->getName(), color);
  t->render(renderer, (kINFO_W - t->getWidth()) / 2,
            (border[0].y - t->getHeight()) / 2);

  delete t;

  /* Render the enemy level */
  t = new Text(font_subheader);
  t->setText(renderer,
             "Level " + std::to_string(enemy->getBasePerson()->getLevel()),
             color);
  t->render(renderer, (kINFO_W - t->getWidth()) / 2,
            border[0].y + bar_height +
                (kINFO_H - border[0].y - bar_height - t->getHeight()) / 2);
  delete t;

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

// NOTE: On menu rendering, render the friend info of the selected ally
// ex. //
/* Render the selecting person info */
// uint16_t x = (section1_w - kINFO_W) / 2;
// uint16_t y = screen_height - bar_height + (bar_height - kALLY_HEIGHT) / 2;
//   success &= renderFriendInfo(screen_height, x, y, true);

// Other todos:
// -- Midlays
// -- Overlays
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

    // FUTURE - Midlays go over the enemies, but under the allies

    /* Render the allies in their present states */
    success &= renderAllies();

    //   /* Render any death animations below the menu */
    //   for(auto &element : render_elements)
    //   {
    //     if(element && element->getType() == RenderType::RGB_SPRITE_DEATH)
    //     {
    //       auto death_sprite = element->getSprite();
    //       auto flasher      = element->getFlasher();

    //       if(death_sprite && flasher)
    //       {
    //         if(element->getAlpha() >= kPERSON_KO_ALPHA)
    //           death_sprite->setOpacity(element->getAlpha());
    //         else
    //           death_sprite->setOpacity(kPERSON_KO_ALPHA);

    //         death_sprite->setColorBalance(element->calcColorRed(),
    //                                       element->calcColorGreen(),
    //                                       element->calcColorBlue());

    //         death_sprite->render(renderer, element->getX(),
    //         element->getY());
    //       }
    //     }
    //   }

    /* Render battle bar (on bottom) */
    success &= renderBattleBar();

    /* --------------- RENDER BATTLE MENU -------------- */

    /* Determine whether the menu should be rendered */
    auto to_render_menu = (turn_state == TurnState::SELECT_ACTION_ALLY);
    auto curr_layer = battle_menu->getMenuLayer();
    to_render_menu &= !getFlagCombat(CombatState::PHASE_DONE);
    to_render_menu &= !(curr_layer == BattleMenuLayer::TARGET_SELECTION);
    // to_render_menu &=
    //     !(battle_menu->getFlag(BattleMenuState::SELECTION_COMPLETE));

    if(to_render_menu)
    {
      if(battle_menu->getFlag(BattleMenuState::READY) &&
         !battle_menu->getFlag(BattleMenuState::SKILL_FRAMES_BUILT))
      {
        battle_menu->createSkillFrames(width * kBIGBAR_M2, width * kBIGBAR_R);
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
  }

  //   for(const auto &element : render_elements)
  //   {
  //     // auto alpha = 255;

  //     if(element->getType() == RenderType::ACTION_TEXT ||
  //        element->getType() == RenderType::DAMAGE_VALUE)
  //     {
  //       auto element_font = element->getFont();

  //       if(element_font != nullptr)
  //       {
  //         Text t(element->getFont());
  //         t.setText(renderer, element->getText(), element->getColor());
  //         t.setAlpha(element->getAlpha());
  //         t.render(renderer, element->getX(), element->getY());

  //         if(element->hasShadow())
  //         {
  //           t.setText(renderer, element->getText(),
  //           element->getShadowColor());
  //           t.setAlpha(element->getAlpha());
  //           t.render(renderer, element->getShadowX(),
  //           element->getShadowY());
  //         }
  //       }
  //     }
  //     else if(element->getType() == RenderType::RGB_OVERLAY)
  //     {
  //       SDL_Color color = element->getColor();
  //       SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b,
  //                              element->getAlpha());

  //       SDL_Rect rgb_overlay_rect;
  //       rgb_overlay_rect.x = element->getX();
  //       rgb_overlay_rect.y = element->getY();
  //       rgb_overlay_rect.w = element->getSizeX();
  //       rgb_overlay_rect.h = element->getSizeY();
  //       SDL_RenderFillRect(renderer, &rgb_overlay_rect);
  //     }
  //     else if(element->getType() == RenderType::ACTION_FRAME)
  //     {
  //       auto action_frame = element->getActionFrame();

  //       if(action_frame != nullptr)
  //         action_frame->render(renderer, element->getX(), element->getY());
  //     }
  //   }

  //   for(auto &element : render_elements)
  //   {
  //     if(element->getType() == RenderType::PLEP)
  //     {
  //       if(element->getSprite() != nullptr)
  //       {
  //         element->getSprite()->render(renderer, element->getX(),
  //                                      element->getY());
  //       }
  //     }
  //   }

  //   return success;
  // }

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

// TODO
bool Battle::renderEnemies()
{
  bool success = true;

  /* Render each enemy */
  for(const auto& enemy : getEnemies())
  {
    success &= enemy->getActiveSprite()->render(renderer, getActorX(enemy),
                                                getActorY(enemy));
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
    if(enemy && enemy->getInfoFrame() && enemy->getBasePerson())
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

// TODO
bool Battle::renderEnemyInfo(BattleActor* actor)
{
  (void)actor;

  return true;
}

// TODO: Comment
bool Battle::renderAllyInfo(BattleActor* ally, bool for_menu)
{
  auto font_subheader = config->getFontTTF(FontName::BATTLE_SUBHEADER);
  bool success = true;
  // bool below = true;

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

  uint16_t health_x = x + 1 + (kINFO_W - kALLY_HEALTH_W) / 2;
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
      Frame::renderBar(health_x, health_y, health_amount, kALLY_HEALTH_H,
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
  success &= Frame::renderBar(qd_x + 1, qd_y, qd_amount, kALLY_QD_H,
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
  // if(below)
  // {
  //   uint16_t ailment_y = y + kALLY_HEIGHT + kAILMENT_GAP * 2 +
  //                        kAILMENT_BORDER * 2 +
  //                        ailments.front().getHeight();
  //   success &= renderAilment(renderer, state->self, x + kINFO_W / 2,
  //   ailment_y,
  //                            false, true);
  // }
  // else
  // {
  //   success &= renderAilment(renderer, state->self, x + kINFO_W / 2,
  //                            screen_height - kBIGBAR_OFFSET);
  // }

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

// if(background)
//   background->update(cycle_time);

// if(turn_state == TurnState::CLEAN_UP)
// {
//   // index_person = 0;
//   bar_offset   = 0;
//   render_flags = static_cast<RenderState>(0);
// }

//  /* Update event processing delays */
//   if(processing_delay > 0)
//   {
//     processing_delay -= cycle_time;

//     if(processing_delay > 0)
//       delay = true;
//   }

//   /*-------------------------------------------------------------------------
//    * BEGIN state
//    *-----------------------------------------------------------------------*/
//   if(rendering_state == TurnState::BEGIN)
//   {
//     bar_offset = 0;

//     setRenderFlag(RenderState::BEGIN_RENDERING, false);

//     rendering_state = battle_state;
//   }
//   /*-------------------------------------------------------------------------
//    * GENERAL_UPKEEP state
//    *-----------------------------------------------------------------------*/
//   else if(rendering_state == TurnState::GENERAL_UPKEEP)
//   {
//     bar_offset = 0;

//     /* Upon general upkeep state, the screen needs to be dimmed and the
//     text
//      * "Turn X: Decide Your Fate" displayed on the screen. This screen dim
//      * will use a RGB_Overlay RenderElement with fade-in and fade-out
//      */
//     if(getRenderFlag(RenderState::SCREEN_DIM) &&
//        !getRenderFlag(RenderState::TURN_TEXT_CREATED))
//     {
//       processing_delay = 3500;

//       SDL_Color shadow_color = {194, 59, 34, 255};
//       RenderElement *turn_text =
//           new RenderElement(RenderType::ACTION_TEXT, 1300, 500, 300);

//       turn_text->setColor({0, 0, 0, 255});
//       turn_text->setShadowColor(shadow_color);
//       turn_text->setShadow();

//       auto turn_string = Helpers::numToRoman(battle->getTurnsElapsed() +
//       1);

//       // TODO: Remove [07-11-15]
//       auto rand_int = Helpers::randInt(10);

//       if(rand_int == 9)
//         turn_string = "Turn " + turn_string + " Kevins a Schweeb";
//       else
//         turn_string = "Turn " + turn_string + "  Decide Your Fate";
//       // END TODO

//       turn_text->setFont(font_turn);
//       turn_text->setText(turn_string);

//       Text t(font_turn);
//       t.setText(renderer, turn_text->getText(), shadow_color);

//       auto text_x = system_options->getScreenWidth() / 2;
//       text_x -= t.getWidth() / 2;
//       auto text_y = system_options->getScreenHeight() / 2;
//       text_y -= t.getHeight() / 2;

//       turn_text->setCoordinates(text_x, text_y);
//       turn_text->setShadowCoordinates(kACTION_TEXT_SHADOW + 2,
//                                       kACTION_TEXT_SHADOW + 2);

//       render_elements.push_back(turn_text);

//       setRenderFlag(RenderState::TURN_TEXT_CREATED, true);
//     }

//     /* If the screen is not currently dimming, append a new render elmt */
//     else if(!getRenderFlag(RenderState::SCREEN_DIMMING))
//     {
//       processing_delay = 750;

//       SDL_Color screen_dim_color = {0, 0, 0, 185};

//       RenderElement *dim_element =
//           new RenderElement(RenderType::RGB_OVERLAY, 2000, 400, 300);

//       dim_element->setColor(screen_dim_color);
//       dim_element->setCoordinates(0, 0);
//       dim_element->setSizeX(system_options->getScreenWidth());
//       dim_element->setSizeY(system_options->getScreenHeight());

//       render_elements.push_back(dim_element);

//       setRenderFlag(RenderState::SCREEN_DIMMING, true);
//     }
//     else
//     {
//       if(!delay <= 0 && !getRenderFlag(RenderState::TURN_TEXT_CREATED))
//       {
//         setRenderFlag(RenderState::SCREEN_DIM, true);
//       }
//       else if(processing_delay <= 0 &&
//               getRenderFlag(RenderState::TURN_TEXT_CREATED))
//       {
//         battle->setBattleFlag(CombatState::RENDERING_COMPLETE, true);
//       }
//     }

//     rendering_state = battle_state;
//   }
//   /*-------------------------------------------------------------------------
//    * UPKEEP state
//    *-----------------------------------------------------------------------*/
//   else if(rendering_state == TurnState::UPKEEP)
//   {
//     bar_offset = 0;

//     if(!getRenderFlag(RenderState::BEGIN_RENDERING))
//     {
//       setRenderFlag(RenderState::BEGIN_RENDERING, true);
//       processing_delay = 350;
//     }
//     else if(!delay)
//     {
//       if(buffer->setRenderIndex())
//       {
//         curr_event = buffer->getCurrentEvent();

//         if(curr_event != nullptr)
//         {
//           updateEvent();
//           buffer->setRendered(buffer->getIndex());
//         }
//       }
//       else
//       {
//         battle->setBattleFlag(CombatState::RENDERING_COMPLETE, true);
//         setRenderFlag(RenderState::BEGIN_RENDERING, false);
//       }
//     }

//     rendering_state = battle_state;
//   }
//   /*-------------------------------------------------------------------------
//    * SELECT_ACTION_ALLY state
//    *-----------------------------------------------------------------------*/
//   else if(rendering_state == TurnState::SELECT_ACTION_ALLY)
//   {
//     /* Resetting index */
//     if((index_layer == 3 || index_layer == 4) && menu->getLayerIndex() ==
//     1)
//     {
//       index_actions = 0;
//       index_types = 0;
//     }

//     /* -- CHOOSING SKILLS -- */
//     else if(menu->getLayerIndex() == 1 || menu->getLayerIndex() == 2)
//     {
//       /* Modify the indexes */
//       if(menu->getLayerIndex() == 1)
//       {
//         if(menu->getElementIndex() >= (index_types + kTYPE_MAX))
//           index_types++;
//         else if(menu->getElementIndex() < index_types)
//           index_types--;
//       }
//       if(menu->getLayerIndex() == 2)
//       {
//         if(menu->getElementIndex() >= (index_actions + kTYPE_MAX))
//           index_actions++;
//         else if(menu->getElementIndex() < index_actions)
//           index_actions--;
//       }

//       bar_offset = kBIGBAR_CHOOSE;
//     }
//     /* -- CHOOSING TARGETS -- */
//     else if(menu->getLayerIndex() == 3)
//     {
//       bar_offset = 0;
//     }
//     /* -- CONFIRM CHOICE -- */
//     else
//     {
//       bar_offset = 0;
//     }

//     rendering_state = battle_state;
//     index_layer = menu->getLayerIndex();
//   }
//   /*-------------------------------------------------------------------------
//    * SELECT_ACTION_ENEMY state
//    *-----------------------------------------------------------------------*/
//   else if(rendering_state == TurnState::SELECT_ACTION_ENEMY)
//   {
//     bar_offset = 0;
//     rendering_state = battle_state;
//   }
//   /*-------------------------------------------------------------------------
//    * PROCESS_ACTIONS state
//    *-----------------------------------------------------------------------*/
//   else if(rendering_state == TurnState::PROCESS_ACTIONS)
//   {
//     bar_offset = 0;
//     rendering_state = battle_state;

//     if(!getRenderFlag(RenderState::BEGIN_RENDERING))
//     {
//       setRenderFlag(RenderState::BEGIN_RENDERING, true);
//       processing_delay = 500;
//     }
//     else if(!delay)
//     {
//       /* Render */
//       if(buffer->setRenderIndex())
//       {
//         curr_event = buffer->getCurrentEvent();

//         if(curr_event != nullptr)
//         {
//           updateEvent();
//           buffer->setRendered(buffer->getIndex());
//         }
//       }
//       else
//       {
//         battle->setBattleFlag(CombatState::RENDERING_COMPLETE, true);
//         setRenderFlag(RenderState::BEGIN_RENDERING, false);
//       }
//     }
//   }

//   /*-------------------------------------------------------------------------
//    * DESTRUCT state
//    *-----------------------------------------------------------------------*/
//   else if(rendering_state == TurnState::DESTRUCT)
//   {
//     stopBattle();
//   }
// }

void Battle::updateBarOffset()
{
  if(turn_state == TurnState::SELECT_ACTION_ALLY)
  {
    if(battle_menu->getMenuLayer() == BattleMenuLayer::TARGET_SELECTION)
      bar_offset = 0;
    else
      bar_offset = kBIGBAR_CHOOSE;
  }
  else
    bar_offset = 0;
}

void Battle::updateRendering(int32_t cycle_time)
{
  updateRenderSprites(cycle_time);
  updateBarOffset();
}

void Battle::updateRenderSprites(int32_t cycle_time)
{
  (void)cycle_time;
  auto brightness = 1.0;

  for(auto& actor : actors)
  {
    if(actor && actor->getActiveSprite())
    {
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

void Battle::updateRenderElements(int32_t cycle_time)
{
  (void)cycle_time;
  // // TODO: [09-06-15] - Update the render elements
  // bool success = true;

  // for(auto &element : render_elements)
  // {
  //   if(element != nullptr)
  //   {
  //     success &= element->update(cycle_time);

  //     if(element->getType() == RenderType::RGB_SPRITE_FLASH)
  //     {
  //       auto sprite = element->getSprite();

  //       if(sprite != nullptr && !sprite->isFlashing())
  //       {
  //         sprite->setTempColorBalance(sprite->getColorRed(),
  //                                     sprite->getColorGreen(),
  //                                     sprite->getColorBlue());

  //         sprite->setFlashing(true);
  //       }
  //       else if(sprite->isFlashing())
  //       {
  //         sprite->setColorBalance(element->calcColorRed(),
  //                                 element->calcColorGreen(),
  //                                 element->calcColorBlue());
  //       }
  //     }

  //     if(element->getStatus() == RenderStatus::FADING_IN)
  //     {
  //       if(element->getFadeInTime() != 0)
  //       {
  //         float alpha_diff = element->getColor().a * 1.0 /
  //                            element->getFadeInTime() * cycle_time;

  //         alpha_diff = std::max(1.0, (double)alpha_diff);

  //         if(element->getAlpha() + alpha_diff > element->getColor().a)
  //           element->setAlpha(element->getColor().a);
  //         else
  //           element->setAlpha(element->getAlpha() + alpha_diff);
  //       }
  //     }
  //     else if(element->getStatus() == RenderStatus::DISPLAYING)
  //     {
  //       element->setAlpha(element->getColor().a);
  //     }
  //     else if(element->getStatus() == RenderStatus::FADING_OUT)
  //     {
  //       if(element->getFadeOutTime() != 0)
  //       {
  //         float alpha_diff = element->getColor().a * 1.0 /
  //                            element->getFadeOutTime() * cycle_time;

  //         alpha_diff = std::max(1.0, (double)alpha_diff);

  //         if(alpha_diff > element->getAlpha())
  //           element->setAlpha(0);
  //         else if(element->getAlpha() - alpha_diff >= 0)
  //           element->setAlpha(element->getAlpha() - alpha_diff);
  //       }
  //     }
  //   }
  // }

  // /* Clear render elements which have timed out */
  // std::vector<RenderElement *> temp_elements;

  // for(auto &element : render_elements)
  // {
  //   if(element != nullptr)
  //   {
  //     /* If the elements is not timed out, just add it to the temp array,
  //      * else, delete the element after performing any needed cleanup */
  //     if(!element->isTimedOut())
  //     {
  //       temp_elements.push_back(element);
  //     }
  //     else
  //     {
  //       /* If the element to be deleted is an RGB Sprite flash, the states'
  //        * 'was flashing' flag must be set to change the sprite's color
  //        * balance back on the next cycle */
  //       if(element->getType() == RenderType::RGB_SPRITE_FLASH)
  //       {
  //         PersonState *state = getState(element->getFlasher());

  //         /* Assigns the state's 'was flashing' bool to true -> revert */
  //         if(state != nullptr)
  //           state->was_flashing = true;
  //       }

  //       delete element;
  //       element = nullptr;
  //     }
  //   }
  // }

  // /* Clear and swap to the temp elements (now containing only non-timed out
  //  * values) */
  // render_elements.clear();
  // render_elements = temp_elements;

  // return success;
}

// TODO: [09-06-15] - Update the enemy renderings
//   for(auto &state : foes_state)
// {
//   if(state && state->tp && state->self)
//   {
//     if(state->was_flashing)
//     {
//       state->tp->revertColorBalance();
//       state->tp->setFlashing(false);
//       state->was_flashing = false;
//     }

//     state->tp->setOpacity(updatePersonOpacity(state->self, cycle_time));
//     state->tp->setBrightness(calcPersonBrightness(state->self));

// TODO: Action sprites for enemies? [07-11-15]
// Update: Shake them instead of an action sprite.
// if (state->self->getBFlag(BState::IS_ATTACKING))
// {
//   if (state->self->getActionSprite() != nullptr)
//     state->tp = state->self->getActionSprite();
// }
// else
// {
//   state->tp = state->self->getThirdPerson();
// }
//   }
// }

// TODO: [09-06-15] : Update the allies renderings
// for(auto& state : friends_state)
// {
//   if(state && state->fp && state->self)
//   {
//     if(state->was_flashing)
//     {
//       state->fp->revertColorBalance();
//       state->fp->setFlashing(false);
//       state->was_flashing = false;
//     }

//     state->fp->setOpacity(updatePersonOpacity(state->self, cycle_time));
//     state->fp->setBrightness(calcPersonBrightness(state->self));

//     /* Determine which sprite to use (Attacking/Normal) */
//     if(state->self->getBFlag(BState::IS_ATTACKING))
//     {
//       if(state->self->getActionSprite())
//       {
//         state->active_sprite = state->as;
//       }
//     }
//     else
//     {
//       state->active_sprite = state->fp;
//     }

//     /* Update position if bobbing */
//     if(state->bobbing)
//     {
//       state->elapsed_time += cycle_time;

//       if(state->elapsed_time >= kBOB_TIME)
//       {
//         state->elapsed_time = 0;
//         state->bobbing = false;
//         state->x = 0;
//         state->y = 0;
//       }
//       else
//       {
//         state->x = getActorX(state->self) +
//                    kBOB_AMOUNT * sin(state->elapsed_time * kBOB_RATE);
//         state->y = getPersonY(state->self);
//       }
//     }
//     else if(state->running)
//     {
//       state->elapsed_time += cycle_time;

//       if(state->elapsed_time >= kRUN_TIME)
//       {
//         state->elapsed_time = 0;
//         state->running = false;
//         state->x = 0;
//         state->y = 0;
//       }
//       else
//       {
//         state->x = getActorX(state->self) +
//                    kRUN_AMOUNT * sin(state->elapsed_time * kRUN_RATE);
//         state->y = getPersonY(state->self);
//       }
//     }
//   }
// }

// return false;

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
  if(turn_state == TurnState::SELECT_ACTION_ALLY)
    battle_menu->keyDownEvent(event);
  else
  {
    if(event.keysym.sym == SDLK_DELETE && battle_buffer)
      battle_buffer->print(false);
    else if(event.keysym.sym == SDLK_INSERT && battle_buffer)
      battle_buffer->print(true);
  }

  return false;
}

bool Battle::startBattle(Party* friends, Party* foes, Sprite* background)
{
  /* Assert everything important is not nullptr */
  assert(config);
  assert(renderer);
  assert(friends);
  assert(foes);

  /* Construct the Battle actor objects based on the persons in the parties */
  buildBattleActors(friends, foes);

  /* Build AI Modules */
  aiBuild();

  /* Build ally and enemy info frames, action frames */
  for(auto& actor : actors)
  {
    buildActionFrame(actor);

    if(actor->getFlag(ActorState::ALLY))
      buildInfoAlly(actor);
    else
      buildInfoEnemy(actor);
  }

  /* Set the sprite for the Battle background */
  setBackground(background);

  /* Construct the enemy backdrop */
  buildEnemyBackdrop();

  turn_state = TurnState::BEGIN;

  return true;
}

void Battle::stopBattle()
{
  /* Destroy the battle actors at the end of a Battle */
  aiClear();
  clearBattleActors();
  clearEvent();

  turn_state = TurnState::STOPPED;
  time_elapsed = 0;
  turns_elapsed = 0;
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

bool Battle::setBackground(Sprite* background)
{
  if(background && background->isFramesSet())
  {
    clearBackground();
    this->background = background;

    return true;
  }

  return false;
}

/*=============================================================================
 * TO REFACTOR
 *============================================================================*/

// // TODO COMMENTS
// bool Battle::updateEvent()
// {
//   if(curr_event->type == EventType::SKILL_USE)
//   {
//     if(curr_event->skill_use != nullptr)
//     {
//       createActionText(curr_event->skill_use->getName());
//     }
//     if(curr_event->user != nullptr)
//     {
//       auto state = getState(curr_event->user);

//       if(state != nullptr)
//       {
//         RenderElement *action_frame =
//             new RenderElement(RenderType::ACTION_FRAME, 2000, 700, 700);
//         action_frame->setX(system_options->getScreenWidth() -
//                            state->action->getWidth());
//         action_frame->setY(kACTION_CENTER - kACTION_Y);
//         action_frame->setActionFrame(state->action);

//         render_elements.push_back(action_frame);
//       }
//     }

//     processing_delay = kDELAY_SKILL;
//   }
//   else if(curr_event->type == EventType::HEAL_HEALTH)
//   {
//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0))
//     {
//       createRegenValue(curr_event->targets.at(0), curr_event->amount);
//       processing_delay = 750;
//     }
//   }
//   else if(curr_event->type == EventType::HIBERNATION)
//   {
//     // TODO Hibernation update plep.

//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0))
//     {
//       createRegenValue(curr_event->targets.at(0), curr_event->amount);
//       processing_delay = 750;
//     }
//   }
//   else if(curr_event->type == EventType::PASS)
//   {
//     if(curr_event->user)
//     {
//       if(getState(curr_event->user))
//       {
//         getState(curr_event->user)->bobbing = true;
//         getState(curr_event->user)->elapsed_time = 0;
//         getState(curr_event->user)->x = getActorX(curr_event->user);
//         getState(curr_event->user)->y = getPersonY(curr_event->user);
//       }
//     }

//     processing_delay = kBOB_TIME;
//   }
//   else if(curr_event->type == EventType::ATTEMPT_RUN)
//   {
//     if(curr_event->user)
//     {
//       if(getState(curr_event->user))
//       {
//         getState(curr_event->user)->running = true;
//         getState(curr_event->user)->elapsed_time = 0;
//         getState(curr_event->user)->x = getActorX(curr_event->user);
//         getState(curr_event->user)->y = getPersonY(curr_event->user);
//       }
//     }

//     processing_delay = kRUN_TIME;
//   }
//   else if(curr_event->type == EventType::SUCCEED_RUN)
//   {
//   }
//   else if(curr_event->type == EventType::ACTION_BEGIN)
//   {
//     curr_event->user->setBFlag(BState::IS_ATTACKING, true);

//     if(battle->getCurrSkill() != nullptr &&
//        battle->getCurrSkill()->getAnimation() != nullptr)
//     {
//       auto animation = battle->getCurrSkill()->getAnimation();
//       createPlep(curr_event->targets.at(0), animation);

//       processing_delay = animation->getAnimationTime();
//     }
//   }
//   else if(curr_event->type == EventType::INFLICTION)
//   {
//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0) &&
//        curr_event->action_use)
//     {
//       auto infliction = curr_event->action_use->getAilment();

//       auto plep = getAilmentPlep(infliction);

//       SDL_Color flash_color = {100, 100, 100, 235};

//       if(plep)
//         createPlep(curr_event->targets.at(0), plep);

//       if(infliction == Infliction::POISON)
//         flash_color = kPOIS_DMG_COLOR;
//       else if(infliction == Infliction::HIBERNATION)
//         flash_color = kHIBERNATION_REGEN_COLOR;
//       else if(infliction == Infliction::CONFUSE)
//         flash_color = {0, 75, 75, 235};
//       else if(infliction == Infliction::PARALYSIS)
//         flash_color = {0, 255, 255, 235};

//       createSpriteFlash(curr_event->targets.at(0), flash_color, 450);
//       processing_delay = 700;
//     }
//   }
//   else if(curr_event->type == EventType::INFLICTION_FIZZLE)
//   {
//     createDamageText(curr_event->targets.at(0), "Fizzle");

//     processing_delay = 400;
//   }
//   else if(curr_event->type == EventType::CURE_INFLICTION)
//   {
//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0))
//     {
//       createSpriteFlash(curr_event->targets.at(0), {255, 255, 255, 255},
//       450);
//       processing_delay = 400;
//     }
//   }
//   else if(curr_event->type == EventType::ACTION_END)
//   {
//     processing_delay = 50;
//   }
//   else if(curr_event->type == EventType::SKILL_COOLDOWN)
//   {
//     if(curr_event->user != nullptr)
//     {
//       createDamageText(curr_event->user, "Cooldown");
//       createSpriteFlash(curr_event->user, {0, 0, 255, 235}, 450);
//     }

//     processing_delay = 400;
//   }
//   else if(curr_event->type == EventType::BEGIN_DEFEND)
//   {
//     if(curr_event->user != nullptr)
//     {
//       auto animation = getEventPlep(EventType::BEGIN_DEFEND);

//       if(animation)
//         createPlep(curr_event->user, animation);

//       // createDamageText(curr_event->user, "Defending");
//       createSpriteFlash(curr_event->user, {255, 255, 255, 245}, 450);
//     }

//     processing_delay = 400;
//   }
//   else if(curr_event->type == EventType::BREAK_DEFEND)
//   {
//     if(curr_event->user != nullptr)
//     {
//       auto animation = getEventPlep(EventType::BREAK_DEFEND);

//       if(animation)
//         createPlep(curr_event->user, animation);

//       // createDamageText(curr_event->user, "Break Defend");
//       createSpriteFlash(curr_event->user, {177, 177, 30, 190}, 450);
//     }

//     processing_delay = 300;
//   }
//   else if(curr_event->type == EventType::PERSIST_DEFEND)
//   {
//     if(curr_event->user != nullptr)
//     {
//       auto animation = getEventPlep(EventType::PERSIST_DEFEND);

//       if(animation)
//         createPlep(curr_event->user, animation);

//       // createDamageText(curr_event->user, "Defend Persists");
//       createSpriteFlash(curr_event->user, {255, 255, 255, 245}, 450);
//     }
//   }
//   else if(curr_event->type == EventType::IMPLODE)
//   {
//     if(curr_event->user && curr_event->targets.size() > 0 &&
//        curr_event->targets.at(0))
//     {
//       auto animation = getEventPlep(EventType::IMPLODE);

//       if(animation)
//       {
//         createPlep(curr_event->targets.at(0), animation);
//         createPlep(curr_event->user, animation);
//       }

//       createDamageValue(curr_event->user, curr_event->amount);
//       createSpriteFlash(curr_event->user, {255, 0, 0, 225}, 450);
//       createSpriteFlash(curr_event->targets.at(0), {255, 0, 0, 225}, 450);
//     }

//     processing_delay = 1200;
//   }

//   else if(curr_event->type == EventType::STANDARD_DAMAGE ||
//           curr_event->type == EventType::CRITICAL_DAMAGE ||
//           curr_event->type == EventType::BURN_DAMAGE ||
//           curr_event->type == EventType::HITBACK_DAMAGE ||
//           curr_event->type == EventType::METABOLIC_DAMAGE)
//   {
//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0) !=
//     nullptr)
//     {
//       createDamageValue(curr_event->targets.at(0), curr_event->amount);
//       createSpriteFlash(curr_event->targets.at(0), {177, 10, 30, 190},
//       450);
//     }

//     processing_delay = kDELAY_DAMAGE;
//   }
//   else if(curr_event->type == EventType::POISON_DAMAGE)
//   {
//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0) !=
//     nullptr)
//     {
//       createDamageValue(curr_event->targets.at(0), curr_event->amount);
//       createSpriteFlash(curr_event->targets.at(0), {10, 150, 150, 190},
//       550);
//     }

//     processing_delay = kDELAY_DAMAGE;
//   }
//   else if(curr_event->type == EventType::SKILL_MISS ||
//           curr_event->type == EventType::ACTION_MISS)
//   {
//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0) !=
//     nullptr)
//     {
//       createDamageText(curr_event->targets.at(0), "Miss");
//     }

//     processing_delay = kDELAY_DAMAGE;
//   }
//   else if(curr_event->type == EventType::DEATH)
//   {
//     if(curr_event->targets.size() > 0 && curr_event->targets.at(0) !=
//     nullptr)
//       if(getState(curr_event->targets.at(0)) != nullptr)
//         getState(curr_event->targets.at(0))->dying = true;

//     createDeath(curr_event->targets.at(0));
//     processing_delay = 1100;
//   }
//   else if(curr_event->type == EventType::REGEN_VITA ||
//           curr_event->type == EventType::REGEN_QTDR)
//   {
//     if(curr_event->amount > 0 && curr_event->targets.size() > 0 &&
//        curr_event->targets.at(0) != nullptr)
//     {
//       createRegenValue(curr_event->targets.at(0), curr_event->amount);
//       processing_delay = 750;
//     }
//   }

//   return false;
// }

// void Battle::battleLost()
// {
// #ifdef UDEBUG
//   printPartyState();
//   std::cout << "Battle lost! :-(\n";
// #endif

//   setBattleFlag(CombatState::OUTCOME_PROCESSED);
//   setNextTurnState();

//   // TODO: [11-14-14] Return to title after battle lost.
// }

// void Battle::battleRun()
// {
//   if(outcome == OutcomeType::ALLIES_RUN)
//   {
//     /* For each person on the friends team, incur a % penalty against the
//      * experience to the next level */
//     for(uint32_t i = 0; i < friends->getSize(); i++)
//     {
// #ifdef UDEBUG
//       std::cout << friends->getMember(i)->getName() << " has lost "
//                 << kRUN_PC_EXP_PENALTY << "% EXP towards next level.\n";
// #endif

//       friends->getMember(i)->loseExpPercent(kRUN_PC_EXP_PENALTY);
//       // TODO [11-06-14] Update personal record run from battle count
//     }

// #ifdef UDEBUG
//     std::cout << "\n{ALLIES RUN} The allied team has ran from Battle!
//     :-/\n";
// #endif
//   }
//   else if(outcome == OutcomeType::ENEMIES_RUN)
//   {
// #ifdef UDEBUG
//     std::cout << "{ENEMIES RUN} The foes team has ran from Battle! :-/\n";
// #endif
//   }

//   setBattleFlag(CombatState::OUTCOME_PROCESSED);
//   setNextTurnState();
// }

// void Battle::battleWon()
// {
// #ifdef UDEBUG
//   printPartyState();
//   std::cout << std::endl;
//   std::cout << "----------------------------" << std::endl;
//   std::cout << "===== BATTLE VICTORIOUS ====" << std::endl;
//   std::cout << "----------------------------" << std::endl;
// #endif

//   /* Cleanup the current Battle state -- This includes unapplying ailments
//   */
//   for(auto ailment : ailments)
//     removeAilment(ailment);

//   // TODO - the following code is temporary [04-03-15]
//   auto living_members = friends->getLivingMemberPtrs();

//   for(auto& member : living_members)
//   {
//     auto exp_to_add = calcExperience(member);
//     member->addExp(exp_to_add);

// #ifdef UDEBUG
//     std::cout << "{EXP GAIN} -- " << member->getName() << "has gained "
//               << exp_to_add << "." << std::endl;
// #endif
//   }

//   // TODO [04-03-15]
//   // Update the personal record for each member, including battle counts
//   // and what battles they have won
//   // Future: Bestiary index

//   // For each living member in friends
//   // Add experience to the member (event loop processing?)
//   // Find out if a level up occurs
//   // Find out how many level ups occur
//   // Append each as a level up event
//   // Find out differences in skill gains
//   // Append skill differences to a skill differences vector
//   // Show total skill difference with current skills at end
//   // For each equipment
//   // For each Bubby
//   // Add exp to the bubby -> will level up
//   // For the loot
//   // Find out what items will be received -> add to inventory
//   // Refuse gain of items?
//   // Findo out how much money will be received -> add to inventory

//   setBattleFlag(CombatState::OUTCOME_PROCESSED);
//   setBattleFlag(CombatState::OUTCOME_PERFORMED); // TODO [04-03-15]
//   setNextTurnState();
// }

// float Battle::calcCritFactor()
// {
//   /* Base crit factor */
//   auto crit_factor = kOFF_CRIT_FACTOR;

//   /* Unbearability modifier */
//   auto unbr = curr_user->getTemp().getStat(Attribute::UNBR);

//   crit_factor += unbr * kCRIT_MODIFIER;

//   /* Level difference modifier */
//   std::vector<Person*> target_vec = {curr_target};
//   crit_factor += calcLevelDifference(target_vec) * kCRIT_LVL_MODIFIER;

//   if(curr_target->getBFlag(BState::DEFENDING))
//     crit_factor *= kCRIT_DEFENDING_MODIFIER;
//   if(curr_target->getBFlag(BState::GUARDED))
//     crit_factor *= kCRIT_GUARDED_MODIFIER;

//   /* Crits should be between 100 and 1000% always */
//   crit_factor = Helpers::setInRange(crit_factor, 1, 10);

//   return crit_factor;
// }

// int32_t Battle::calcExperience(Person* ally)
// {
//   int32_t exp = 0;

//   auto foes_members = foes->getMembers();

//   for(auto& foe : foes_members)
//     exp += foe->getExpDrop();

//   return static_cast<int32_t>(exp * ally->getExpMod());
// }

// int16_t Battle::calcLevelDifference(std::vector<Person*> targets)
// {
//   if(curr_user)
//   {
//     auto total_lvl = 0;

//     for(const auto& target : targets)
//       if(target)
//         total_lvl += target->getLevel();

//     if(total_lvl != 0)
//       return curr_user->getLevel() - (total_lvl / targets.size());
//   }

//   return 0;
// }

// bool Battle::checkPartyDeath(Party* const check_party, Person* target)
// {
//   auto party_death = false;

//   if(check_party != nullptr && target != nullptr)
//   {
//     auto living_members = check_party->getLivingMembers();

//     party_death = living_members.size() < 2;

//     if(living_members.size() == 1)
//     {
//       party_death &= (check_party->getMember(living_members.at(0)) ==
//       target);
//     }
//   }

//   return party_death;
// }

// /*
//  * Description: Cleanup before the end of a Battle turn. Clears the action
//  *              buffer, resets person index, the action variables. Updates
//  *              the turns elapsed and menu fora new turn.
//  *
//  * Inputs: none
//  * Output: none
//  */
// void Battle::cleanUp()
// {
//   action_buffer->update(false);
//   action_buffer->print(false);

//   upkeep_persons.clear();
//   temp_ailments.clear();
//   person_index = 0;

//   setBattleFlag(CombatState::BEGIN_PROCESSING, false);
//   setBattleFlag(CombatState::BEGIN_ACTION_PROCESSING, false);
//   setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, false);
//   setBattleFlag(CombatState::LAST_INDEX, false);
//   setBattleFlag(CombatState::ALL_PROCESSING_COMPLETE, false);
//   setBattleFlag(CombatState::BEGIN_PERSON_UPKEEPS, false);
//   setBattleFlag(CombatState::PERSON_UPKEEP_COMPLETE, false);
//   setBattleFlag(CombatState::BEGIN_AILMENT_UPKEEPS, false);
//   setBattleFlag(CombatState::CURRENT_AILMENT_STARTED, false);
//   setBattleFlag(CombatState::CURRENT_AILMENT_COMPLETE, false);
//   setBattleFlag(CombatState::COMPLETE_AILMENT_UPKEEPS, false);
//   setBattleFlag(CombatState::ALL_UPKEEPS_COMPLETE, false);
//   setBattleFlag(CombatState::CURR_TARG_DEAD, false);
//   setBattleFlag(CombatState::RENDERING_COMPLETE, false);

//   /* Clean all action processing related variables */
//   clearActionVariables();

//   /* Increment the turn counter */
//   turns_elapsed++;

//   menu->unsetAll();
//   curr_module = nullptr;

//   /* Cleanup for each member of friends and persons */
//   for(size_t i = 0; i < friends->getSize(); i++)
//   {
//     auto member = friends->getMember(i);
//     member->resetDefend();
//     member->resetGuard();
//     member->resetGuardee();
//     member->setAilFlag(PersonAilState::SKIP_NEXT_TURN, false);
//   }

//   for(size_t i = 0; i < foes->getSize(); i++)
//   {
//     auto member = foes->getMember(i);
//     member->resetAI();
//     member->resetDefend();
//     member->resetGuard();
//     member->resetGuardee();
//     member->setAilFlag(PersonAilState::SKIP_NEXT_TURN, false);
//   }

//   /* Reset the update processed flag for all ailments at the end of the
//   turn
//   */
//   for(auto ailment : ailments)
//     ailment->setFlag(AilState::UPDATE_PROCESSED, false);

//   setBattleFlag(CombatState::PHASE_DONE, true);
// }

// bool Battle::doesCurrPersonRun()
// {
//   auto can_run_happen = true;
//   auto run_happens = false;

//   /* First, determine if a run is possible */
//   can_run_happen &= (foes->getPartyType() == PartyType::REGULAR_FOE);
//   can_run_happen &= curr_user->getBFlag(BState::ALIVE);
//   can_run_happen &= curr_user->getBFlag(BState::RUN_ENABLED);

//   if(can_run_happen)
//   {
//     /* Next, determine the chance for a run to occur. Users with higher
//      * momentum will have a higher chance of running. Parties with higher
//      * momentum will have a higher chance of running. Enemies with higher
//      * momentum will decrease the chance of a successful run */
//     std::vector<Person*> other_allies;
//     Party* other_enemies;
//     double mmnt_p = 0;

//     if(person_index < 0)
//     {
//       other_allies = foes->findMembersExcept(curr_user, true);
//       other_enemies = friends;
//     }
//     else
//     {
//       other_allies = friends->findMembersExcept(curr_user, true);
//       other_enemies = foes;
//     }

//     mmnt_p +=
//         curr_user->getCurr().getStat(Attribute::MMNT) * kUSER_RUN_MODIFIER;

//     /* Add each other allies momentum by the factor of ally run modifier */
//     for(const auto& ally : other_allies)
//       mmnt_p += ally->getCurr().getStat(Attribute::MMNT) *
//       kALLY_RUN_MODIFIER;

//     /* Add each enemies momentum by the factor of enemy run modifier */
//     mmnt_p += other_enemies->getTotalSpeed() * kENEMY_RUN_MODIFIER;

//     /* Determine the percent change based on run mmnt pc modifier */
//     auto pc_change = kRUN_PC_PER_POINT * mmnt_p;
//     auto run_chance = (int32_t)(pc_change * 100) + (kBASE_RUN_CHANCE *
//     100);
//     run_happens = Helpers::chanceHappens(run_chance, 100);
//   }

//   return run_happens;
// }

// void Battle::loadBattleStateFlags()
// {
//   setBattleFlag(CombatState::PHASE_DONE, false);
//   setBattleFlag(CombatState::OUTCOME_PROCESSED, false);
//   setBattleFlag(CombatState::OUTCOME_PERFORMED, false);
// }

// void Battle::performEvents()
// {
// #ifdef UDEBUG
//   std::cout << "---- Performing Events ----" << std::endl;
// #endif

//   /* Assert there is at least one index of events to perform */
//   auto valid_next = true;
//   valid_next &= event_buffer->getCurrentSize() > 0;

//   /* Assert the current event exists and has not been performed already */
//   if(valid_next)
//     valid_next &= !event_buffer->getCurrentEvent()->performed;

//   while(valid_next)
//   {
//     // auto current_size = event_buffer->getCurrentSize();
//     auto event = event_buffer->getCurrentEvent();
//     auto index = event_buffer->getIndex();

//     // TODO [04-04-15] Temporary code
//     event_buffer->printEvent(index);

//     if(event->type == EventType::SKILL_COOLDOWN)
//     {
// #ifdef UDEBUG
//       std::cout << "{COOLDOWN} -- The skill " <<
//       event->skill_use->getName()
//                 << " being used by " << event->user->getName()
//                 << " will cooldown "
//                 << " from " << event->amount << " to " << event->amount - 1
//                 << "." << std::endl;
// #endif
//     }
//     else if(event->type == EventType::ACTION_BEGIN)
//     {
//       if(event->user)
//         setUserAttacking(event->user);
//     }
//     else if(event->type == EventType::ACTION_END)
//     {
//       unsetActorsAttacking();
//     }
//     else if(event->type == EventType::MISS_TURN)
//     {
// #ifdef UDEBUG
//       std::cout << "{MISS TURN} -- The user " << event->user
//                 << " misses their turn!" << std::endl;
// #endif
//     }
//     if(event->type == EventType::IMPLODE)
//     {
//       performDamageEvent(event);
//       // TODO [08-30-15]: Move the member to reserve (not needed until
//       revive)
//     }
//     else if(event->type == EventType::INSPECT)
//     {
//       // TODO [02-08-15]: Determine inspect outcome
//     }
//     else if(event->type == EventType::SUCCEED_RUN)
//     {
//       /* In a succeed run event, the happens flag is used for "allies" */
//       if(event->allies)
//         setOutcome(OutcomeType::ALLIES_RUN);
//       else
//         setOutcome(OutcomeType::ENEMIES_RUN);

//       setBattleFlag(CombatState::PHASE_DONE, true);
//     }
//     else if(event->type == EventType::STANDARD_DAMAGE ||
//             event->type == EventType::CRITICAL_DAMAGE ||
//             event->type == EventType::POISON_DAMAGE ||
//             event->type == EventType::BURN_DAMAGE ||
//             event->type == EventType::HITBACK_DAMAGE ||
//             event->type == EventType::METABOLIC_DAMAGE)
//     {
//       performDamageEvent(event);
//     }
//     else if(event->type == EventType::METABOLIC_KILL)
//     {
//       event->user->setBFlag(BState::ALIVE, false);
//     }
//     else if(event->type == EventType::DEATH_COUNTDOWN)
//     {
// #ifdef UDEBUG
//       std::cout << "{DEATHTIMER} " << event->ailment_use->getTurnsLeft()
//                 << " turns until death! " << std::endl;
// #endif
//     }
//     else if(event->type == EventType::BOND)
//     {
//       // TODO [02-14-15] To Hell with this bond effect
//     }
//     else if(event->type == EventType::BONDING)
//     {
//       // TODO [02-14-15] To Hell with this bonding effect
//     }
//     else if(event->type == EventType::BEGIN_DEFEND)
//     {
//       event->user->setBFlag(BState::DEFENDING, true);

// #ifdef UDEBUG
//       std::cout << "{DEFEND} " << event->user->getName()
//                 << " is now defending themselves from damage." <<
//                 std::endl;
// #endif
//     }
//     else if(event->type == EventType::BREAK_DEFEND)
//     {
//       event->user->resetDefend();

// #ifdef UDEBUG
//       std::cout << "{BREAK DEFEND} " << curr_target->getName()
//                 << " is no longer defending from damage.\n\n";
// #endif
//     }
//     else if(event->type == EventType::PERSIST_DEFEND)
//     {
// #ifdef UDEBUG
//       std::cout << "{PERSIST DEFEND} " << curr_target->getName()
//                 << " continues to defend themselves from damage.\n\n";
// #endif
//     }
//     else if(event->type == EventType::BEGIN_GUARD)
//     {
//       /* Update the buffer to swap out Guard <--> Guardee
//        * as targets for remaining actions to be processed & performed */
//       auto success = performGuard(event);

//       if(success)
//       {
//         action_buffer->injectGuardTargets(event->user->getGuard(),
//         event->user);

// #ifdef UDEBUG
//         std::cout << "{GUARD} " << event->user->getGuard()->getName()
//                   << " is now guarding " << event->user->getName()
//                   << " from some damage.\n";
// #endif
//       }
//       else
//       {
// #ifdef UDEBUG
//         std::cerr << "[ERROR] Guard pair not made successfully" <<
//         std::endl;
// #endif
//       }
//     }
//     else if(event->type == EventType::PERSIST_GUARD)
//     {
// #ifdef UDEBUG
//       std::cout << "{PERSIST GUARD " << curr_target->getName() << "
//       continues"
//                 << " to protect " << curr_target->getGuardee()->getName()
//                 << " from damage" << std::endl;
// #endif
//     }
//     else if(event->type == EventType::BREAK_GUARD)
//     {
//       action_buffer->rejectGuardTargets(event->user->getGuard());

//       curr_target->getGuardee()->resetGuard();
//       curr_target->resetGuardee();

// #ifdef UDEBUG
//       std::cout << "{BREAK GUARD} " << event->user->getGuard()->getName()
//                 << " is no longer guarding " << event->user->getName()
//                 << " from some damage.\n";
// #endif
//     }
//     else if(event->type == EventType::DEATH)
//     {
//       event->targets.at(0)->setBFlag(BState::ALIVE, false);

//       /* Enable all skills corner case (silence cure on death) */
//       auto skills = curr_target->getCurrSkills();
//       for(size_t i = 0; i < skills->getSize(); i++)
//         skills->setSilenced(i, false);
//     }
//     else if(event->type == EventType::PARTY_DEATH)
//     {
//       std::cout << "{PARTY DEATH} A party death has occured!" << std::endl;

//       /* If a party death occurs on the allies -> victory, else -> loss */
//       if(event->allies)
//         setOutcome(OutcomeType::DEFEAT);
//       else
//         setOutcome(OutcomeType::VICTORY);
//     }
//     else if(event->type == EventType::INFLICTION)
//     {
//       assert(event->targets.size() > 0 && event->targets.at(0));

//       curr_target = event->targets.at(0);

//       addAilment(event->action_use->getAilment(), event->user,
//                  event->action_use->getMin(), event->action_use->getMax(),
//                  event->action_use->getBase());
//     }
//     else if(event->type == EventType::CURE_INFLICTION)
//     {
//       removeAilment(event->ailment_use);
//     }
//     else if(event->type == EventType::ALTERATION)
//     {
//       auto target_attr = event->action_use->getTargetAttribute();
//       auto amount = event->amount;

//       /* Perform the alteration on the target */
//       event->targets.at(0)->getCurr().alterStat(target_attr, amount);

// #ifdef UDEBUG
//       std::cout << "{ALTER}" << event->targets.at(0)->getName() << "'s"
//                 << AttributeSet::getName(target_attr) << " has been altered
//                 by "
//                 << amount << "." << std::endl;
// #endif
//     }
//     else if(event->type == EventType::ASSIGNMENT)
//     {
//       /* Assign event takes the attribute of the action use of the event
//       and
//        * assigns it the value of the event's amount */
//       auto assign_attr = event->action_use->getTargetAttribute();
//       auto amount = event->amount;

//       // TODO: Why change temp/curr stats assignment?
//       //  if (base_pc)
//       // ction_target->getCurr().setStat(targ_attr, set_value);
//       // else
//       //   action_target->getTemp().setStat(targ_attr, set_value);
//       event->targets.at(0)->getCurr().setStat(assign_attr, amount);

// #ifdef UDEBUG
//       std::cout << "{ASSIGN} " << event->targets.at(0)->getName() << "'s"
//                 << AttributeSet::getName(assign_attr) << " has been altered
//                 by "
//                 << amount << "." << std::endl;
// #endif
//     }
//     else if(event->type == EventType::REVIVAL)
//     {
//       /* A revive event contains a target to be revived (which needs their
//        * alive flag to be reset) and an amount of VITA to assign their curr
//        * VITA to */
//       event->targets.at(0)->setBFlag(BState::ALIVE, true);
//       curr_target->getCurr().setStat(Attribute::VITA, event->amount);

// #ifdef UDEBUG
//       std::cout << "{REVIVE} " << event->targets.at(0)->getName()
//                 << " has been brought back from KO with "
//                 << curr_target->getCurr().getStat(Attribute::VITA) << "
//                 VITA."
//                 << std::endl;
// #endif
//     }
//     else if(event->type == EventType::HEAL_HEALTH)
//     {
//       if(event->targets.size() > 0 && event->targets.at(0))
//       {
//         auto new_val =
//             event->targets.at(0)->getCurr().getStat(Attribute::VITA) +
//             event->amount;

//         event->targets.at(0)->getCurr().setStat(Attribute::VITA, new_val);
//       }
//     }
//     else if(event->type == EventType::REGEN_VITA ||
//             event->type == EventType::HIBERNATION)
//     {
//       auto new_val =
//       event->targets.at(0)->getCurr().getStat(Attribute::VITA)
//       +
//                      event->amount;

//       event->targets.at(0)->getCurr().setStat(Attribute::VITA, new_val);

// #ifdef UDEBUG
//       if(event->amount > 0)
//       {
//         std::cout << "{REGEN} " << event->targets.at(0)->getName()
//                   << " has restored " << event->amount << " VITA." <<
//                   std::endl;
//       }
// #endif
//     }
//     else if(event->type == EventType::REGEN_QTDR)
//     {
//       auto new_val =
//       event->targets.at(0)->getCurr().getStat(Attribute::QTDR)
//       +
//                      event->amount;

//       event->targets.at(0)->getCurr().setStat(Attribute::QTDR, new_val);

// #ifdef UDEBUG
//       if(event->amount != 0)
//       {
//         std::cout << "{REGEN} " << event->targets.at(0)->getName()
//                   << " has regained " << event->amount << " QTDR." <<
//                   std::endl;
//       }
// #endif
//     }
//     /* Deathtimer death -> the countdown has reached zero and the target
//     will
//      * instantly be killed */
//     else if(event->type == EventType::DEATHTIMER_DEATH)
//     {
//       event->targets.at(0)->getCurr().setStat(Attribute::VITA, 0);
//       event->targets.at(0)->setBFlag(BState::ALIVE, false);
//     }

//     event_buffer->setPerformed(index);
//     valid_next = event_buffer->setNextIndex();
//   }
// }

// void Battle::performDamageEvent(BattleEvent* event)
// {
// #ifdef UDEBUG
//   auto str_damage = "";

//   if(event->type == EventType::STANDARD_DAMAGE)
//     str_damage = "STND DMG";
//   else if(event->type == EventType::CRITICAL_DAMAGE)
//     str_damage = "CRIT DMG";
//   else if(event->type == EventType::POISON_DAMAGE)
//     str_damage = "POSN DMG";
//   else if(event->type == EventType::BURN_DAMAGE)
//     str_damage = "BURN DMG";
//   else if(event->type == EventType::HITBACK_DAMAGE)
//     str_damage = "HITB DMG";
//   else if(event->type == EventType::METABOLIC_DAMAGE)
//     str_damage = "META DMG";
// #endif

//   auto amount = event->amount;
//   auto targets = event->targets;

//   if(event->type == EventType::IMPLODE)
//   {
//     auto temp = event->user;
//     event->user = event->targets.at(0);
//     targets.at(0) = temp;
//   }

//   /* Do the actual damage to the person */
//   if(targets.size() > 0)
//   {
//     targets.at(0)->doDmg(amount);

// #ifdef UDEBUG
//     std::cout << "{" << str_damage << "} " << targets.at(0)->getName()
//               << " struck with " << amount << " damage." << std::endl;
// #endif
//   }
// }

// void Battle::resetTurnFlags(Person* user)
// {
//   user->setAilFlag(PersonAilState::MISS_NEXT_TARGET, false);
//   user->setAilFlag(PersonAilState::NEXT_ATK_NO_EFFECT, false);
// }

// void Battle::personalUpkeep(Person* const target)
// {
//   curr_target = target;

//   /* If ailment processing hasn't begun for this person, reset the
//   processing
//    * index and stash all the ailments they are inflicted with in a temp
//    vec.
//    */
//   if(!getBattleFlag(CombatState::BEGIN_AILMENT_UPKEEPS))
//   {
//     pro_index = 0;
//     temp_ailments = getPersonAilments(target);

//     setBattleFlag(CombatState::CURRENT_AILMENT_COMPLETE, false);
//     setBattleFlag(CombatState::BEGIN_AILMENT_UPKEEPS, true);

//     if(temp_ailments.size() == 0)
//       setBattleFlag(CombatState::COMPLETE_AILMENT_UPKEEPS);
//   }

//   /* If there were > 0 ailment, and the processing index is in bounds,
//   update
//    * the ailment using its update function (which may cure the ailmenmt),
//    * then process its effects for rendering (ex. POISON inflicting damage)
//    */
//   if(!getBattleFlag(CombatState::COMPLETE_AILMENT_UPKEEPS) &&
//      pro_index < temp_ailments.size())
//   {
//     processAilment();

//     /* Increment the ailment processing index */
//     if(getBattleFlag(CombatState::CURRENT_AILMENT_COMPLETE))
//     {
//       setBattleFlag(CombatState::CURRENT_AILMENT_COMPLETE, false);
//       pro_index++;
//     }

//     if(pro_index >= temp_ailments.size())
//       setBattleFlag(CombatState::COMPLETE_AILMENT_UPKEEPS);
//   }
//   else if(getBattleFlag(CombatState::COMPLETE_AILMENT_UPKEEPS))
//   {
//     /* Calculate the turn regen values and append "negative" damage events
//      * (Regen events) to the event buffer, completing the upkeep of person
//      */
//     auto vita_regen = calcTurnRegen(target, Attribute::VITA);
//     auto qtdr_regen = calcTurnRegen(target, Attribute::QTDR);

//     event_buffer->createDamageEvent(EventType::REGEN_VITA, target,
//     vita_regen);
//     event_buffer->createDamageEvent(EventType::REGEN_QTDR, target,
//     qtdr_regen);

//     target->battleTurnPrep();
//     setBattleFlag(CombatState::PERSON_UPKEEP_COMPLETE);
//   }

//   setBattleFlag(CombatState::READY_TO_RENDER, true);
// }

// void Battle::processBuffer()
// {
//   /* If Buffer index == 0, don't increment, else, increment */
//   if(getBattleFlag(CombatState::BEGIN_PROCESSING) &&
//      getBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE))
//   {
//     setBattleFlag(CombatState::LAST_INDEX, false);

//     if(!action_buffer->setNext())
//       setBattleFlag(CombatState::LAST_INDEX, true);

//     setBattleFlag(CombatState::BEGIN_ACTION_PROCESSING, false);
//     setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, false);
//   }
//   /* If begin processing has not been assigned (after checking for complete
//    * action processing, then the action index should be set to 0 and flag
//    * set to show that processing has begun) */
//   else if(!getBattleFlag(CombatState::BEGIN_PROCESSING))
//   {
//      Clear action variables to ensure a clean slate for processing
//     clearActionVariables();

//     curr_action_index = 0;
//     setBattleFlag(CombatState::BEGIN_PROCESSING, true);
//   }

//   auto curr_action_type = ActionType::NONE;
//   auto curr_targets = action_buffer->getTargets();
//   auto cooldown = action_buffer->getCooldown();
//   curr_action_type = action_buffer->getActionType();
//   curr_user = action_buffer->getUser();
//   curr_skill = action_buffer->getSkill();

//   /* Assert the current user is alive. If the current user is alive, the
//    * processing of their action on the Buffer will be void */
//   if(curr_user != nullptr && curr_user->getBFlag(BState::ALIVE))
//   {

//     if(curr_action_type == ActionType::SKILL)
//     {
//       /* Only process the skill if its cooldown is at zero, else: create a
//       skill
//        * cooldown event to cool the skill processing down */
//       if(cooldown == 0)
//       {
//         processSkill(curr_targets);
//       }
//       else
//       {
//         event_buffer->createCooldownEvent(curr_user, curr_skill, cooldown);
//         setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE);
//       }
//     }
//     else if(curr_action_type == ActionType::ITEM)
//     {
//       processItem(curr_targets);
//     }
//     else if(curr_action_type == ActionType::DEFEND)
//     {
//       event_buffer->createDefendEvent(EventType::BEGIN_DEFEND, curr_user);
//       setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }
//     else if(curr_action_type == ActionType::GUARD)
//     {
//       curr_target = curr_targets.at(0);
//       bool good_guard = processGuard();

//       /* Create begin guard or fail guard events based on the guard
//       processing
//        */
//       if(good_guard)
//       {
//         event_buffer->createGuardEvent(EventType::BEGIN_GUARD, curr_user,
//                                        action_buffer->getTargets().at(0));
//       }
//       else
//       {
//         event_buffer->createGuardEvent(EventType::FAIL_GUARD, curr_user,
//                                        action_buffer->getTargets().at(0));
//       }

//       setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }
//     else if(curr_action_type == ActionType::IMPLODE)
//     {
//       assert(curr_targets.size() > 0);
//       processImplode(curr_targets);
//     }
//     else if(curr_action_type == ActionType::RUN)
//     {
//       auto allies = friends->isInParty(curr_user);
//       event_buffer->createRunEvent(EventType::ATTEMPT_RUN, curr_user,
//       allies);

//       if(doesCurrPersonRun())
//         event_buffer->createRunEvent(EventType::SUCCEED_RUN, curr_user,
//         allies);
//       else
//         event_buffer->createRunEvent(EventType::FAIL_RUN, curr_user,
//         allies);

//       setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }
//     else if(curr_action_type == ActionType::PASS)
//     {
//       event_buffer->createPassEvent(curr_user);
//       setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }
//   }
//   else
//     setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE);

//   /* Complete the processing of the last of events and move to clean up */
//   if(getBattleFlag(CombatState::LAST_INDEX) &&
//      getBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE))
//   {
//     setBattleFlag(CombatState::ALL_PROCESSING_COMPLETE, true);
//   }

//   setBattleFlag(CombatState::READY_TO_RENDER, true);
// }

// /*
//  * Description: Checks to see whether a good guard pair can be made between
//  the
//  *              current target and current user pointers and returns the
//  result
//  *
//  * Inputs: none
//  * Output: bool - the correctness of a guard pair between cur user and cur
//  targ
//  */
// bool Battle::processGuard()
// {
//   auto can_guard = true;

//   if(curr_target == nullptr || curr_user == nullptr)
//   {

// #ifdef UDEBUG
//     std::cerr << "[Error] Curr target/user is null for guard processing"
//               << std::endl;
// #endif

//     can_guard = false;
//   }

//   /* A guard/guardee combo can only be made if both the guard and guardee
//   are
//    * not being guarded by, or guarding any persons */
//   if(can_guard)
//   {
//     can_guard &= curr_target->getGuard() == nullptr;
//     can_guard &= curr_target->getGuardee() == nullptr;
//     can_guard &= curr_user->getGuard() == nullptr;
//     can_guard &= curr_user->getGuardee() == nullptr;
//   }

//   return can_guard;
// }

// bool Battle::performGuard(BattleEvent* guard_event)
// {
//   /* If a guard ca be made, attempt to assign guardee and guarding persons.
//    * This should assign the GUARDING/GUARDED flags properly */
//   if(guard_event != nullptr && guard_event->type == EventType::BEGIN_GUARD)
//   {
//     auto guard = guard_event->targets.at(0);
//     auto guardee = guard_event->user;
//     auto good_guard = guard->setGuardee(guardee);
//     good_guard &= guardee->setGuard(guard);

//     return good_guard;
//   }

//   return false;
// }

// bool Battle::processAction(BattleEvent* battle_event)
// {
//   auto can_process = false;
//   auto done = false;
//   auto target_alive = curr_target->getBFlag(BState::ALIVE);
//   std::vector<Person*> target_vec{curr_target};

//   /* Non-revive actions can only be processed against targets who are alive
//   */
//   if(!target_alive && curr_action->actionFlag(ActionFlags::REVIVE))
//     can_process = true;
//   else if(target_alive && !curr_action->actionFlag(ActionFlags::REVIVE))
//     can_process = true;

//   auto action_hits = false;
//   battle_event->happens = action_hits;

//   if(can_process)
//   {
//     action_hits = doesActionHit();
//   }

//   /* Action/Factor targets for Alteration/Assignment */
//   if(can_process && action_hits)
//   {
//     if(curr_action->actionFlag(ActionFlags::ALTER) ||
//        curr_action->actionFlag(ActionFlags::ASSIGN))
//     {
//       auto action_target = curr_user;
//       auto factor_target = curr_user;

//       user_attr = curr_action->getUserAttribute();
//       targ_attr = curr_action->getTargetAttribute();

//       /* If the user's attribute is defined and the target's is not, the
//        * alteration will be on the user's %/value up to their MAX attrs*/
//       if(user_attr != Attribute::NONE && targ_attr == Attribute::NONE)
//         targ_attr = user_attr;
//       /* If the target's attribute is defined and the user's is not, the
//        * alteration or assignment will be on the target's %/value up to
//        their
//        * MAX or CURR amount, respectively */
//       else if(user_attr == Attribute::NONE && targ_attr != Attribute::NONE)
//       {
//         action_target = curr_target;
//         factor_target = curr_target;
//         user_attr = targ_attr;
//       }
//       /* If both the user and target's attributes are defined, the
//       alteration
//        * will alter the target's value by a percentage of the user's stat
//        */
//       else if(user_attr != Attribute::NONE && targ_attr != Attribute::NONE)
//         action_target = curr_target;
//       else
//         std::cerr << "[Error] - Critical error in Battle processing.\n";

//       if(curr_action->actionFlag(ActionFlags::FLIP_ATTR))
//       {
//         std::cout << "Flipping!" << std::endl;
//         std::swap(action_target, factor_target);
//         std::swap(user_attr, targ_attr);
//         std::cout << "Action Target:" << action_target->getName() <<
//         std::endl;
//         std::cout << "Factor Target: " << factor_target->getName() <<
//         std::endl;
//       }

//       if(curr_action->actionFlag(ActionFlags::ALTER))
//       {
//         auto alter_event =
//             event_buffer->createAlterEvent(curr_action, action_target, 0);

//         done = processAlterAction(alter_event, action_target,
//         factor_target);
//       }
//       else
//       {
//         auto assign_event =
//             event_buffer->createAssignEvent(curr_action, action_target, 0);

//         done = processAssignAction(assign_event, action_target,
//         factor_target);
//       }
//     }
//     else if(curr_action->actionFlag(ActionFlags::DAMAGE))
//     {
//       auto damage_event = event_buffer->createDamageEvent(
//           EventType::STANDARD_DAMAGE, curr_target, 0);

//       done = processDamageAction(damage_event);
//     }
//     else if(curr_action->actionFlag(ActionFlags::INFLICT))
//     {
//       done = processInflictAction();
//     }
//     else if(curr_action->actionFlag(ActionFlags::RELIEVE))
//     {
//       done = processRelieveAction();
//     }
//     else if(curr_action->actionFlag(ActionFlags::REVIVE))
//     {
//       auto revive_event = event_buffer->createReviveEvent(curr_target, 0);
//       done = processReviveAction(revive_event);
//     }
//   }
//   else if(!can_process)
//   {
//     auto fizzle = event_buffer->createFizzleEvent(EventType::FIZZLE,
//     curr_user,
//                                                   target_vec);
//     fizzle->action_use = curr_action;
//   }
//   else if(can_process && !action_hits)
//   {
//     auto miss = event_buffer->createMissEvent(EventType::ACTION_MISS,
//     curr_user,
//                                               target_vec);
//     miss->action_use = curr_action;
//   }

//   return done;
// }

// bool Battle::processAilment()
// {
//   /* The current ailment pointer being processed is the processing index of
//   the
//    * temporary vector for the current person having their upkeep processed
//    */
//   auto ail = temp_ailments[pro_index];

//   bool allies = friends->isInParty(curr_target);
//   bool party_death = false;

//   if(ail != nullptr)
//   {
// #ifdef UDEBUG
//     std::cout << "--- Ailment Proc: " <<
//     Helpers::ailmentToStr(ail->getType())
//               << " ---" << std::endl;
// #endif

//     /* If the ailment has not yet been updated this turn, perform standard
//      * updates (flag setting/turn incrementing && damages) */
//     if(!ail->getFlag(AilState::UPDATE_PROCESSED) &&
//        ail->getFlag(AilState::TO_UPDATE))
//     {
//       party_death = processAilmentUpdate(ail);
//     }

//     /* If a party death didn't occur, continue processing on the flags that
//      * were set during ailment updates/other outcomes (deaths, etc.) */
//     if(!party_death && ail->getFlag(AilState::UPDATE_PROCESSED))
//     {
//       auto to_kill = ail->getFlag(AilState::TO_KILL);
//       auto to_cure = ail->getFlag(AilState::TO_CURE);

//       /* Process DeathTimer outcomes */
//       if(ail->getType() == Infliction::DEATHTIMER)
//       {
//         if(to_kill)
//         {
//           event_buffer->createDeathEvent(EventType::DEATHTIMER_DEATH,
//                                          curr_target, allies);
//         }
//         else
//         {
//           event_buffer->createAilmentEvent(EventType::DEATH_COUNTDOWN,
//           nullptr,
//                                            curr_target, nullptr, ail);
//         }
//       }
//       else if(ail->getType() == Infliction::METATETHER && to_kill)
//       {
//         event_buffer->createDeathEvent(EventType::METABOLIC_KILL,
//         curr_target,
//                                        allies);
//       }

//       /* Check for party death */
//       if(to_kill)
//       {
//         /* If the person is to die, their upkeeping has been processed */
//         setBattleFlag(CombatState::PERSON_UPKEEP_COMPLETE, true);

//         event_buffer->createDeathEvent(EventType::DEATH, curr_target,
//         allies);

//         party_death = processPersonDeath(allies);
//       }

//       /* Cure the ailment */
//       if(to_cure)
//       {
//         event_buffer->createAilmentEvent(EventType::CURE_INFLICTION,
//         nullptr,
//                                          curr_target, nullptr, ail);
//       }
//     }
//   }

//   setBattleFlag(CombatState::CURRENT_AILMENT_COMPLETE);

//   return party_death;
// }

// bool Battle::processAilmentUpdate(Ailment* ail)
// {
//   /* Update the ailment with turn updating true (this sets the update
//    * processed flag in the update call which is cleaned up in cleanUp() */
//   ail->update(true);

//   if(ail->getFlag(AilState::DEALS_DAMAGE) &&
//      curr_target->getBFlag(BState::ALIVE))
//   {
//     auto damage_amount = ail->getDamageAmount();
//     auto event_type = EventType::STANDARD_DAMAGE;

//     if(ail->getType() == Infliction::POISON)
//       event_type = EventType::POISON_DAMAGE;
//     else if(ail->getType() == Infliction::BURN)
//       event_type = EventType::BURN_DAMAGE;
//     else if(ail->getType() == Infliction::METATETHER)
//       event_type = EventType::METABOLIC_DAMAGE;
//     else if(ail->getType() == Infliction::HIBERNATION)
//       event_type = EventType::HIBERNATION;

//     /* Create an event for the amount of damage the ailment will inflict */
//     event_buffer->createDamageEvent(event_type, curr_target,
//     damage_amount);

//     /* Process death outcomes for the damage and return party death truth
//     */
//     if(ail->getType() != Infliction::HIBERNATION)
//       return processDamageAmount(damage_amount);
//   }

//   return false;
// }

// bool Battle::processAlterAction(BattleEvent* alter_event, Person*
// action_target,
//                                 Person* factor_target)
// {
//   auto ally_target = friends->isInParty(curr_target);
//   auto base_pc = curr_action->actionFlag(ActionFlags::BASE_PC);
//   auto vari_pc = curr_action->actionFlag(ActionFlags::VARI_PC);
//   int32_t base = curr_action->getBase();
//   int32_t vari = curr_action->getVariance();

//   int32_t cur_value = 0;
//   int32_t set_value = 0;
//   int32_t var_value = 0;
//   float one_pc = 0.0;
//   auto party_death = false;

//   auto max_value = action_target->getTemp().getStat(targ_attr);
//   cur_value = action_target->getCurr().getStat(targ_attr);

//   one_pc = static_cast<float>(factor_target->getTemp().getStat(user_attr));
//   one_pc /= 100;

//   set_value =
//       (base_pc) ? (std::floor(static_cast<int32_t>(one_pc * base))) :
//       (base);
//   var_value =
//       (vari_pc) ? (std::floor(static_cast<int32_t>(one_pc * vari))) :
//       (vari);

//   var_value = Helpers::randU(-var_value, var_value);
//   auto alt_value = set_value + var_value;

//   /* The altered amount cannot be > the dif between max and current */
//   if(alt_value > 0 && (alt_value + cur_value) > max_value)
//   {
//     alt_value = max_value - cur_value;
//   }

//   /* Assign the alteration amount to the alter event */
//   if(alt_value > 0 && event_buffer->getLastEvent())
//     event_buffer->getLastEvent()->type = EventType::HEAL_HEALTH;

//   /* The altered amount cannot be such that it would decrease a stat to neg
//   */
//   else if(alt_value < 0 && alt_value < -cur_value)
//   {
//     alt_value = -cur_value;

//     /* If the altered attribute is VITA, a death will occur here */
//     if(targ_attr == Attribute::VITA)
//     {
//       event_buffer->createDeathEvent(EventType::DEATH, curr_target,
//                                      ally_target);

//       /* If a death occurs, processing needs to end after here */
//       setBattleFlag(CombatState::CURR_TARG_DEAD, true);

//       party_death = processPersonDeath(ally_target);
//     }
//   }

//   alter_event->amount = alt_value;
//   alter_event->happens = true;

//   return party_death;
// }

// bool Battle::processAssignAction(BattleEvent* assign_event,
//                                  Person* action_target, Person*
//                                  factor_target)
// {
//   auto ally_target = friends->isInParty(action_target);
//   auto base_pc = curr_action->actionFlag(ActionFlags::BASE_PC);
//   auto vari_pc = curr_action->actionFlag(ActionFlags::VARI_PC);
//   int32_t base = curr_action->getBase();
//   int32_t vari = curr_action->getVariance();
//   int32_t set_value = 0;
//   int32_t var_value = 0;
//   float one_pc = 0.0;
//   auto party_death = false;
//   auto max_value = factor_target->getTemp().getStat(targ_attr);

//   /* Find one percent of the given attribute */
//   one_pc = static_cast<float>(factor_target->getCurr().getStat(user_attr));
//   one_pc /= 100;

//   /* Find percentages of base and variance values of the attributes */
//   set_value =
//       (base_pc) ? (std::floor(static_cast<int32_t>(one_pc * base))) :
//       (base);
//   var_value =
//       (vari_pc) ? (std::floor(static_cast<int32_t>(one_pc * vari))) :
//       (vari);

//   set_value += Helpers::randU(-var_value, var_value);
//   set_value = Helpers::setInRange(set_value, 0, max_value);

//   assign_event->amount = set_value;
//   assign_event->happens = true;

//   /* If the set value is 0? What if not setting VITA? //TODO [02-01-15] */
//   if(set_value == 0)
//   {
//     event_buffer->createDeathEvent(EventType::DEATH, action_target,
//                                    ally_target);

//     setBattleFlag(CombatState::CURR_TARG_DEAD, true);

//     party_death = processPersonDeath(ally_target);
//   }

//   return party_death;
// }

// bool Battle::processDamageAction(BattleEvent* damage_event)
// {
//   auto party_death = false;
//   auto actual_crit_factor = 1.00;
//   bool crit_happens = doesActionCrit();

//   if(crit_happens)
//   {
//     damage_event->type = EventType::CRITICAL_DAMAGE;
//     actual_crit_factor = calcCritFactor();
//   }
//   else
//     damage_event->type = EventType::STANDARD_DAMAGE;

//   auto base_damage = calcBaseDamage(actual_crit_factor);
//   auto damage_mod = curr_user->getDmgMod();
//   auto damage = static_cast<int32_t>(base_damage * damage_mod);

//   damage = Helpers::setInRange(damage, kMINIMUM_DAMAGE, kMAXIMUM_DAMAGE);
//   damage_event->amount = damage;

//   /* Send damage processing (death calculations to process damage amount
//   fn)
//   */
//   party_death = processDamageAmount(damage);

//   if(hasInfliction(Infliction::BERSERK, curr_user))
//   {
//     auto hitback = base_damage * Ailment::getBerserkHitbackPC();

//     event_buffer->createDamageEvent(EventType::HITBACK_DAMAGE, curr_user,
//                                     hitback);

//     party_death = processDamageAmount(damage);
//   }

//   return party_death;
// }

// bool Battle::processDamageAmount(int32_t amount)
// {
//   // TODO: The comments really need to be updated here. [03-07-15]
//   /* If doDmg returns true, the actor has died. Update guarding and other
//    * corner cases and check for party death. Else, an actor has not died
//    * but guard and defending flags, etc. may need to be recalcuted */
//   auto party_death = false;
//   auto ally_target = friends->isInParty(curr_target);

//   /* The party of the target will be considered dead if they are the only
//    * living member of the party presently and are going to die */
//   if(amount >= curr_target->getCurr().getStat(Attribute::VITA))
//   {
//     event_buffer->createDeathEvent(EventType::DEATH, curr_target,
//     ally_target);

//     setBattleFlag(CombatState::CURR_TARG_DEAD, true);
//     party_death = processPersonDeath(ally_target);
//   }
//   else
//   {
//     updateTargetDefense();
//   }

//   return party_death;
// }

// bool Battle::processPersonDeath(bool ally_target)
// {
//   std::vector<Person*> living_members;

//   if(ally_target)
//     living_members = friends->getLivingMemberPtrs();
//   else
//     living_members = foes->getLivingMemberPtrs();

//   /* True: If the Event buffer contains all event deaths with a curr target
//    * matching each living friend member, then a party death will occur. */
//   auto party_death = true;

//   for(const auto& member : living_members)
//     party_death &= event_buffer->hasPersonDeathEvent(member);

//   if(party_death)
//   {
//     event_buffer->createDeathEvent(EventType::PARTY_DEATH, curr_target,
//                                    ally_target);
//     party_death = true;
//   }

//   auto person_ailments = getPersonAilments(curr_target);

//   for(auto ailment : person_ailments)
//   {
//     if(ailment->getFlag(AilState::CURE_ON_DEATH))
//     {
//       event_buffer->createAilmentEvent(EventType::CURE_INFLICTION,
//       curr_user,
//                                        curr_target, curr_action, ailment);
//     }
//   }

//   return party_death;
// }

// bool Battle::processRelieveAction()
// {
//   /* If the ailment attempting to be relieved is inflicted upon the current
//    * target, loop through all existing ailments until the ailment is found
//    * and create a relieving event for that ailment */
//   if(canRelieve(curr_action->getAilment()))
//   {
//     for(auto ill : ailments)
//     {
//       if(ill->getVictim() == curr_target)
//       {
//         event_buffer->createAilmentEvent(EventType::CURE_INFLICTION,
//         curr_user,
//                                          curr_target, curr_action, ill);
//       }
//     }

//     return true;
//   }
//   /* If the target cannot be relieved of the given ailment, create an
//    * infliction fizzle event on the buffer */
//   else
//   {
//     std::vector<Person*> targets{curr_target};
//     event_buffer->createFizzleEvent(EventType::INFLICTION_FIZZLE,
//     curr_user,
//                                     targets);
//   }

//   return false;
// }

// bool Battle::processReviveAction(BattleEvent* revive_event)
// {
//   auto heal_value = 1;
//   auto one_pc = curr_target->getTemp().getStat(Attribute::VITA);
//   auto base_pc = curr_action->actionFlag(ActionFlags::BASE_PC);
//   auto vari_pc = curr_action->actionFlag(ActionFlags::VARI_PC);

//   /* Calculate the amount of health to revive the person to based on
//    * base/variance values of the revive action */
//   int32_t base_val = 0;
//   int32_t vari_val = 0;

//   base_val =
//       (base_pc) ? (one_pc * curr_action->getBase()) :
//       (curr_action->getBase());
//   vari_val = (vari_pc) ? (one_pc * curr_action->getVariance())
//                        : (curr_action->getVariance());

//   vari_val = Helpers::randU(-vari_val, +vari_val);

//   /* Append the revival event on to the buffer */
//   if((base_val + vari_val) > heal_value)
//     heal_value = base_val + vari_val;

//   revive_event->amount = heal_value;
//   revive_event->happens = true;

//   return false;
// }

// bool Battle::processInflictAction()
// {
//   if(canInflict(curr_action->getAilment()))
//   {
//     /* If a person is about to be bubbified, their current ailments must be
//      * removed -- thus event buffer needs to be populated with remove
//      events
//      */
//     if(curr_action->getAilment() == Infliction::BUBBIFY)
//     {
//       for(auto ill : ailments)
//       {
//         if(ill->getVictim() == curr_target)
//         {
//           event_buffer->createAilmentEvent(EventType::CURE_INFLICTION,
//                                            curr_user, curr_target,
//                                            curr_action,
//                                            ill);
//         }
//       }
//     }

//     /* If or not if bubbified, create the infliction event */
//     event_buffer->createAilmentEvent(EventType::INFLICTION, curr_user,
//                                      curr_target, curr_action, nullptr);
//   }
//   else
//   {
//     /* If the person cannot be inflicted with the ailment, create an
//     ailment
//      * fizzling event */
//     event_buffer->createFizzleEvent(EventType::INFLICTION_FIZZLE,
//     curr_user,
//                                     curr_target);
//   }

//   return false;
// }

// bool Battle::canRelieve(Infliction test_infliction)
// {
//   auto person_ailments = getPersonAilments(curr_target);
//   auto can_cure = false;

//   for(auto person_ill : person_ailments)
//     if(person_ill->getType() == test_infliction)
//       can_cure |= person_ill->getFlag(AilState::CURABLE);

//   return can_cure;
// }

// void Battle::processItem(std::vector<Person*> targets)
// {
//   // TODO [04-10-15]
//   (void)targets;

//   curr_item = action_buffer->getItem();
//   curr_skill = curr_item->getUseSkill();

//   if(!getBattleFlag(CombatState::BEGIN_ACTION_PROCESSING))
//   {
//     setBattleFlag(CombatState::BEGIN_ACTION_PROCESSING);

//     auto event =
//         event_buffer->createItemEvent(curr_item, curr_user, curr_target);

//     // TODO [04-10-15]
//     (void)event;

//     // TODO [04-10-15]
//     // - What situations of items on targets will fizzle?
//     // - Abstract out processing of actions with skill for commong
//     //   function(s) between skill and item?
//     // - Can items miss?
//     // - Should items deal a damage different than based on the regular
//     //   damage formula? Standard damage except through armor?
//   }
// }

// void Battle::processSkill(std::vector<Person*> targets)
// {
//   /* Grab the current skill from the action buffer */
//   curr_skill = action_buffer->getSkill();

// #ifdef UDEBUG
//   std::cout << "{Skill} Processing: " << curr_skill->getName() <<
//   std::endl;
//   printPartyState();
// #endif

//   auto process_first_index = false;

//   // TODO
//   auto process_action = false;

//   /* If processing the first action, append begin skill use event */
//   if(!getBattleFlag(CombatState::BEGIN_ACTION_PROCESSING))
//   {
//     setBattleFlag(CombatState::BEGIN_ACTION_PROCESSING);

//     auto event =
//         event_buffer->createSkillEvent(curr_skill, curr_user, targets,
//         false);

//     auto to_process_skill = true;
//     process_first_index = true;

//     event->user = curr_user;
//     event->targets = targets;

//     if(to_process_skill)
//     {
//       auto skill_hits = doesSkillHit(targets);

//       if(skill_hits)
//       {
//         // Add action variable setup code here? ////????
//         event->happens = true;

//         if(hasInfliction(Infliction::BERSERK, curr_user) &&
//            !curr_skill->isBerserkSkill())
//         {
//           event_buffer->createFizzleEvent(EventType::SKILL_USE_FIZZLE,
//                                           curr_user, targets);
//         }
//       }
//       /* If there exists 0 current actions, then the next event will be
//       miss
//       */
//       else if(event_buffer->getCurrentSize() < 1)
//       {
//         event_buffer->createMissEvent(EventType::SKILL_MISS, curr_user,
//                                       targets);
//       }

//       /* If the skill misses, go to the next action processing after render
//       */
//       if(!skill_hits)
//         setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }
//     else
//     {
//       setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }

//     std::cout << "Setting current action index to zero" << std::endl;
//     curr_action_index = 0;
//   }
//   else
//   {
//     process_action = true;
//   }

//   if(process_first_index || process_action)
//   {
//     /* Else, process action index and increment the action index */
//     auto effects = curr_skill->getEffects();

//     /* Build the variables for primary and secondary off/def attributes,
//     etc.
//     */
//     buildActionVariables(ActionType::SKILL, targets);

//     /* Assert the current action index is within effects range, then
//      * compute the outcome for the effect for every target, and prepare
//      * the outcomes for rendering */
//     if(curr_action_index < effects.size())
//     {
//       curr_action = effects.at(curr_action_index);

//       for(auto it = begin(targets); it != end(targets); ++it)
//       {
//         curr_target = *it;
//         setBattleFlag(CombatState::CURR_TARG_DEAD, false);
//         auto event = event_buffer->createActionEvent(
//             EventType::ACTION_BEGIN, curr_action, curr_skill, curr_user,
//             curr_target, true);

//         processAction(event);

//         event_buffer->createActionEvent(EventType::ACTION_END, curr_action,
//                                         curr_skill, curr_user, curr_target,
//                                         true);

//         if(getBattleFlag(CombatState::CURR_TARG_DEAD))
//         {
//           // End the skill processing here?
//           setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE);
//         }
//       }
//     }
//     else
//     {
//       // End the skill processing here?
//       setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }

//     /* Increment the current action index [for multiple action skills] */
//     curr_action_index++;
//     std::cout << "Incrementing action index: " << curr_action_index - 1
//               << " to " << curr_action_index << std::endl;

//     if(effects.size() <= curr_action_index)
//     {
//       std::cout << "Setting action processing complete!" << std::endl;
//       setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE, true);
//     }
//   }
// }

// bool Battle::processImplode(std::vector<Person*> targets)
// {
//   /* */
//   auto party_death = false;
//   auto process = true;

//   /* Assign the target, assert that the target is valid */
//   assert(targets.size() > 0 && targets.at(0));
//   curr_target = targets.at(0);

//   buildActionVariables(ActionType::IMPLODE, targets);

//   if(process)
//   {
//     /* Create the initial damage amount against user, killing 'dem ' */
//     auto user_damage_amount =
//     curr_user->getCurr().getStat(Attribute::VITA);
//     auto targ_damage_amount = calcImplodeDamage();

//     auto temp_curr_user = curr_user;
//     auto temp_curr_targ = curr_target;

//     curr_target = temp_curr_user;
//     party_death &= processDamageAmount(user_damage_amount);

//     /* Create the imploding damage event for the user of the imploding */
//     curr_target = temp_curr_targ;
//     auto implode_event =
//         event_buffer->createImplodeEvent(curr_user, curr_target);
//     implode_event->amount = user_damage_amount;

//     /* Create the damage event on the target */
//     event_buffer->createDamageEvent(EventType::STANDARD_DAMAGE,
//     curr_target,
//                                     targ_damage_amount);

//     setBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE);
//   }

//   return party_death;
// }

// void Battle::reCalcAilments(Person* const target)
// {
//   auto temp_vita = target->getCurr().getStat(0);
//   auto temp_qtdr = target->getCurr().getStat(1);
//   auto base_max_stats = target->getCurrMax();

//   target->setCurr(base_max_stats);
//   target->getCurr().setStat(0, temp_vita);
//   target->getCurr().setStat(1, temp_qtdr);
//   target->setTemp(base_max_stats);

//   auto ills = getPersonAilments(target);

//   for(auto ill : ills)
//     if(ill->getFlag(AilState::BUFF))
//       ill->apply();
// }

// void Battle::upkeep()
// {
//   if(!(getBattleFlag(CombatState::BEGIN_PERSON_UPKEEPS)))
//   {
//     setBattleFlag(CombatState::BEGIN_PERSON_UPKEEPS, true);
//   }
//   if(upkeep_persons.size() > 0)
//   {
//     /* If there is still people to upkeep, upkeep the first person */
//     personalUpkeep(upkeep_persons.at(0));
//     /* If that prerson's upkeep is complete, reset the flags and erase the
//      * person from the upkeep persons vector */
//     if(getBattleFlag(CombatState::PERSON_UPKEEP_COMPLETE))
//     {
//       setBattleFlag(CombatState::BEGIN_AILMENT_UPKEEPS, false);
//       setBattleFlag(CombatState::COMPLETE_AILMENT_UPKEEPS, false);
//       setBattleFlag(CombatState::PERSON_UPKEEP_COMPLETE, false);
//       setBattleFlag(CombatState::CURRENT_AILMENT_COMPLETE, false);
//       upkeep_persons.erase(begin(upkeep_persons));
//     }
//   }
//   /* If there are no remaining upkeep persons, the personal upkeep phase is
//    * complete -> reset flags so update() will set the next turn state */
//   if(upkeep_persons.size() == 0)
//     setBattleFlag(CombatState::ALL_UPKEEPS_COMPLETE, true);
// }

// bool Battle::updatePartyDeaths(Person* target)
// {
//   if(target != nullptr)
//     return (checkPartyDeath(friends, target) || checkPartyDeath(foes,
//     target));

//   return false;
// }

// void Battle::unsetActorsAttacking()
// {
//   for(auto& ally : friends->getMembers())
//     if(ally)
//       ally->setBFlag(BState::IS_ATTACKING, false);
//   for(auto& foe : foes->getMembers())
//     if(foe)
//       foe->setBFlag(BState::IS_ATTACKING, false);
// }

// void Battle::unsetActorsSelecting()
// {
//   for(auto& ally : friends->getMembers())
//     if(ally)
//       ally->setBFlag(BState::IS_SELECTING, false);
//   for(auto& foe : foes->getMembers())
//     if(foe)
//       foe->setBFlag(BState::IS_SELECTING, false);
// }

// bool Battle::updateTargetDefense()
// {
//   auto can_process = true;

//   can_process &= curr_target != nullptr;
//   can_process &= curr_user != nullptr;

//   if(can_process)
//   {
//     auto defending = curr_target->getBFlag(BState::DEFENDING);

//     /* If the person was defending, unless they'r pow. def., reset def
//     status
//     */
//     if(defending && !curr_target->isPowerDefender())
//       event_buffer->createDefendEvent(EventType::BREAK_DEFEND,
//       curr_target);
//     else if(defending && curr_target->isPowerDefender())
//       event_buffer->createDefendEvent(EventType::PERSIST_DEFEND,
//       curr_target);

//     auto guarding = curr_target->getBFlag(BState::GUARDING);

//     /* If the person was guarding, unless they're pow. grd., reset grd
//     status
//     */
//     if(guarding && !curr_target->isPowerGuarder())
//     {
//       event_buffer->createGuardEvent(EventType::BREAK_GUARD, curr_user,
//                                      curr_target);
//     }
//     else if(guarding && curr_target->isPowerGuarder())
//     {
//       event_buffer->createGuardEvent(EventType::PERSIST_GUARD, curr_user,
//                                      curr_target);
//     }
//   }

//   return can_process;
// }

// /*
//  * Description:
//  *
//  * Inputs:
//  * Output:
//  */
// void Battle::setUserAttacking(Person* target)
// {
//   unsetActorsAttacking();

//   if(target != nullptr)
//     target->setBFlag(BState::IS_ATTACKING, true);
// }

void Battle::setNextTurnState()
{
  /* Set the CURRENT_STATE to incomplete */
  setFlagCombat(CombatState::PHASE_DONE, false);

  //   /* If the Battle victory/loss has been complete, go to Destruct */
  //   if(getBattleFlag(CombatState::OUTCOME_PROCESSED))
  //   {
  //     setTurnState(TurnState::DESTRUCT);
  //     menu->unsetAll(true);
  //     // TODO [11-0614]: Eventhandler battle finish signal?
  //   }

  if(turn_state != TurnState::FINISHED)
  {
    if(turn_state == TurnState::BEGIN)
      turn_state = TurnState::GENERAL_UPKEEP;
    else if(turn_state == TurnState::GENERAL_UPKEEP)
      turn_state = TurnState::UPKEEP;
    else if(turn_state == TurnState::UPKEEP)
      turn_state = TurnState::SELECT_ACTION_ALLY;
    else if(turn_state == TurnState::SELECT_ACTION_ALLY)
      turn_state = TurnState::SELECT_ACTION_ENEMY;
    else if(turn_state == TurnState::SELECT_ACTION_ENEMY)
      turn_state = TurnState::PROCESS_ACTIONS;
  }

  std::cout << "Setting turn state: " << Helpers::turnStateToStr(turn_state)
            << std::endl;
}

//     /* If the Battle has been won, go to victory */
//     if(outcome == OutcomeType::VICTORY)
//     {
//       setTurnState(TurnState::VICTORY);
//       battleWon();
//     }
//     /* If the Battle has been lost, GAME OVER */
//     if(outcome == OutcomeType::DEFEAT)
//     {
//       setTurnState(TurnState::LOSS);
//       battleLost();
//     }

//     if(outcome == OutcomeType::ALLIES_RUN ||
//        outcome == OutcomeType::ENEMIES_RUN)
//     {
//       setTurnState(TurnState::RUNNING);
//       battleRun();
//     }

//     /* After the user selects actions, the enemy party may still need to
//        select actions, or if not, order actions is called  */
//     else if(turn_state == TurnState::SELECT_ACTION_ALLY)
//     {
//       if(turn_mode == TurnMode::FRIENDS_FIRST)
//       {
//         setTurnState(TurnState::SELECT_ACTION_ENEMY);
//         person_index = -1;

//         if(anyUserSelection(false))
//           selectEnemyActions();
//         else
//           setBattleFlag(CombatState::PHASE_DONE);
//       }
//     }

//     /* After enemies select actions, the users may still need to select
//     actios,
//        or if not, order actions is called  */
//     else if(turn_state == TurnState::SELECT_ACTION_ENEMY)
//     {
//       else if(turn_mode == TurnMode::ENEMIES_FIRST)
//       {
//         setTurnState(TurnState::SELECT_ACTION_ALLY);
//         person_index = 1;

//         if(anyUserSelection(true))
//           selectUserActions();
//         else
//           setBattleFlag(CombatState::PHASE_DONE);
//       }
//     }

//     /* After the actions are ordered, the actions are processed */
//     /* After the actions are processed, Battle turn clean up occurs */
//     else if(turn_state == TurnState::PROCESS_ACTIONS)
//     {
//       setTurnState(TurnState::CLEAN_UP);
//       cleanUp();
//     }

//     /* After the end of the turn, loop restarts at general upkeep */
//     else if(turn_state == TurnState::CLEAN_UP)
//     {
//       setTurnState(TurnState::GENERAL_UPKEEP);
//     }
//   }
// }

// // TODO [04-12-15] Conventions
// bool Battle::setOutcome(OutcomeType new_outcome)
// {
//   if(outcome == OutcomeType::NONE)
//   {
//     outcome = new_outcome;

//     return true;
//   }

//   return false;
// }

// /*
//  * Description: General update-tick call for Battle. Sends the update call
//  *              down to update ally selection, update enemy selection
//  sections
//  *              in those states of battle.
//  *
//  * Inputs: int32_t - the input cycle time of the battle.
//  * Output: bool - false while not done
//  */
bool Battle::update(int32_t cycle_time)
{
  time_elapsed += cycle_time;

  updateDelay(cycle_time);
  updateRendering(cycle_time);

  // std::cout << "Current Battle State:: " <<
  // Helpers::turnStateToStr(turn_state);

  // TODO: REMOVE THIS
  if(turns_elapsed % 100 == 0 && turn_state != TurnState::SELECT_ACTION_ALLY &&
     turn_state != TurnState::SELECT_ACTION_ENEMY)
    setFlagCombat(CombatState::PHASE_DONE);

  if(getFlagCombat(CombatState::PHASE_DONE))
    setNextTurnState();

  /* ------------------------ GENERAL UPKEEP -------------------------------- */

  /* ---------------------------- UPKEEP  ----------------------------------- */

  /* ----------------------- SELECT ACTION ALLY ----------------------------- */

  /* ----------------------- SELECT ACTION ALLY ----------------------------- */

  if(turn_state == TurnState::SELECT_ACTION_ALLY)
    updateUserSelection();

  /* ----------------------- SELECT ACTION ENEMY ---------------------------- */

  else if(turn_state == TurnState::SELECT_ACTION_ENEMY)
    updateEnemySelection();

  /* ----------------------- PROCESS ACTIONS -------------------------------- */

  else if(turn_state == TurnState::PROCESS_ACTIONS)
    updateProcessing();

  return false;
}

//   else if(turn_state == TurnState::GENERAL_UPKEEP)
//   {
//     if(getBattleFlag(CombatState::RENDERING_COMPLETE))
//     {
//       setBattleFlag(CombatState::RENDERING_COMPLETE, false);

//       generalUpkeep();
//     }
//   }
//   else if(turn_state == TurnState::UPKEEP)
//   {
//     /* If the phase was not yet done and rendering complete, there exists
//     more
//      * personal upkeeps to perform -> reset rendering flag and continue */
//     if(getBattleFlag(CombatState::RENDERING_COMPLETE))
//     {
//       setBattleFlag(CombatState::RENDERING_COMPLETE, false);
//       setBattleFlag(CombatState::READY_TO_RENDER, false);

//       if(!getBattleFlag(CombatState::PHASE_DONE))
//       {
//         event_buffer->setCurrentIndex();
//         performEvents();
//       }

//       if(getBattleFlag(CombatState::ALL_UPKEEPS_COMPLETE))
//         setBattleFlag(CombatState::PHASE_DONE);
//     }
//     /* Continue processing upkeeps if they are not yet ready to render or
//     there
//      * if there is no rendering to take place */
//     else if(!getBattleFlag(CombatState::READY_TO_RENDER))
//     {
//       upkeep();
//     }
//   }

//   else if(turn_state == TurnState::PROCESS_ACTIONS)
//   {
//     menu->setWindowStatus(WindowStatus::OFF);

//     /* If rendering is complete -> perform events on the stack and then
//      * continue processing battle events */
//     if(getBattleFlag(CombatState::RENDERING_COMPLETE))
//     {
//       setBattleFlag(CombatState::RENDERING_COMPLETE, false);
//       setBattleFlag(CombatState::READY_TO_RENDER, false);

//       if(!getBattleFlag(CombatState::PHASE_DONE))
//       {
//         event_buffer->setCurrentIndex();
//         performEvents();
//       }

//       /* If all processing is complete, after performing -> move state */
//       if(getBattleFlag(CombatState::ACTION_PROCESSING_COMPLETE))
//       {
//         auto phase_done =
//         getBattleFlag(CombatState::ALL_PROCESSING_COMPLETE);

//         phase_done |= (outcome != OutcomeType::NONE);

//         setBattleFlag(CombatState::PHASE_DONE, phase_done);
//       }
//     }
//     else if(!getBattleFlag(CombatState::READY_TO_RENDER))
//     {
//       processBuffer();
//     }
//   }
//   else if(turn_state == TurnState::RUNNING)
//   {
//   }
//   else if(turn_state == TurnState::DESTRUCT)
//   {
//     // TODO [11-19-14] EventHandlers - return state to map or wherever nicely
//     return false;
//   }

//   return false;
// }
