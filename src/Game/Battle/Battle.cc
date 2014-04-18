/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: February 23rd, 2014
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

#include "Game/Battle/Battle.h"

/*=============================================================================
 * CONSTANT
 *============================================================================*/

/* ------------ Menu Constants --------------- */
const uint16_t Battle::kGENERAL_UPKEEP_DELAY = 500; /* Time prior info bar msg */
const uint16_t Battle::kBATTLE_MENU_DELAY    = 400; /* Personal menu delay */

/* ------------ Battle Damage Calculation Modifiers ---------------
 *
 * Offensive Primary Element Modifier
 * Defensive Primary Element Modifier
 * Offensive Secondary Element Modifier
 * Defensive Secondary Element Modifier
 * Offensive Critical Hit Chance [Unbearability] Modifier
 * Defensive Critical Hit Chance [Unbearability] Modifier
 * Base Critical Hit Chance [Unbearability] Modifier
 * Dodge Chance [Limbertude] Modifier
 * Dodge Chance [Limbertude] Per Level Modifier
 * Primary Elemental Advantage Modifier
 * Primary Elemental Disadvantage Modifier
 * Secondary Elemental Advantage Modifier
 * Secondary Elemental Disadvantage Modifier
 * Double Elemental Advantage Modifier
 * Double Elemental Disadvantage Modifier
 */
const uint16_t Battle::kMAX_AILMENTS             =    50;
const uint16_t Battle::kMAX_EACH_AILMENTS        =     5;
const uint16_t Battle::kMINIMUM_DAMAGE           =     1;
const uint16_t Battle::kMAXIMUM_DAMAGE           = 29999;
const float    Battle::kOFF_PRIM_ELM_MODIFIER    =  1.07;
const float    Battle::kDEF_PRIM_ELM_MODIFIER    =  1.04;
const float    Battle::kOFF_SECD_ELM_MODIFIER    =  1.05;
const float    Battle::kDEF_SECD_ELM_MODIFIER    =  1.03;
const float    Battle::kOFF_CRIT_MODIFIER        =  1.10;
const float    Battle::kDEF_CRIT_MODIFIER        =  0.90;
const float    Battle::kBASE_CRIT_MODIFIER       =  1.25;
const float    Battle::kDODGE_MODIFIER           =  1.10;
const float    Battle::kDODGE_PER_LEVEL_MODIFIER =  1.04;
const float    Battle::kPRIM_ELM_ADV_MODIFIER    =  1.15;
const float    Battle::kPRIM_ELM_DIS_MODIFIER    =  0.87;
const float    Battle::kSECD_ELM_ADV_MODIFIER    =  1.10;
const float    Battle::kSECD_ELM_DIS_MODIFIER    =  0.93;
const float    Battle::kDOUBLE_ELM_ADV_MODIFIER  =  1.30;
const float    Battle::kDOUBLE_ELM_DIS_MODIFIER  =  0.74;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
/* Constructs a party given two parties and configured options */
Battle::Battle(Party* const friends, Party* const foes)
  : friends(friends)
  , foes(foes)
{
  setupClass();
  determineTurnMode();
  loadBattleStateFlags();

  // info_bar = new BattleInfg, this);
  
  // status_bar = new BattleStatusBar(getFriends(), getScreenWidth(), 
  //                                  getScreenHeight(), this);
  action_buffer = new Buffer();
  menu          = new BattleMenu();

  setBattleFlag(CombatState::PHASE_DONE, true);
}

/*
 * Description:
 */
Battle::~Battle()
{
  for (auto it = begin(ailments); it != end(ailments); ++it)
    if ((*it).second != nullptr)
      delete (*it).second;
  ailments.clear();

  // if (info_bar != nullptr)
  //   delete info_bar;
  // info_bar = nullptr;

  if (menu != nullptr)
    delete menu;
  menu = nullptr;

  // if (status_bar != nullptr)
  //   delete status_bar;
  // status_bar = nullptr;

  if (action_buffer != nullptr)
    delete action_buffer;
  action_buffer = nullptr;

  // for (auto it = begin(enemy_bars); it != end(enemy_bars); ++it)
  //   if ((*it) != nullptr)
  //     delete (*it);
  // enemy_bars.clear();

  // if (status_bar_bg != nullptr)
  //   delete status_bar_bg;
  // status_bar_bg = nullptr;
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
/* Attempts to add an ailment to the vector of ailments */
bool Battle::addAilment(Ailment* const new_ailment)
{
  auto can_add = ailments.size() < kMAX_AILMENTS;
  auto person_ailments = getPersonAilments(new_ailment->getVictim()).size();
  can_add &= person_ailments < kMAX_EACH_AILMENTS;

  if (can_add)
  {
    auto vic_name = new_ailment->getVictim()->getName();
    auto ail_name = static_cast<int32_t>(new_ailment->getType());

#ifdef UDEBUG
    std::cout << "Inflicting ailment: " << + ail_name + " on " + vic_name 
              << "\n";
#else
  if (battle_mode == BattleMode::TEXT)
    std::cout << "Inflicting ailment: " << ail_name << " on " << vic_name 
              << "\n";
#endif
  
    //TODO: Add ailment infliction to Info Bar [03-16-14]
  }

  return can_add;
}

/* Called when the Battle has been won */
void Battle::battleWon()
{
  //call victory
    // for each person on friends
      // increase exp by enemyTotalExp
      // levelUp() --> show update info
      // for each equipment
        // for each bubby
          // increase exp by enemyTotalExp
          // levelUp() --> show update info
   // process loot
     // money
     // items

#ifdef UDEBUG
  std::cout << "Battle victorious! :-)\n";
#endif

  setBattleFlag(CombatState::OUTCOME_DONE);
  setNextTurnState();
}

/* Called when the Battle has been lost */
void Battle::battleLost()
{
  // return to title?

#ifdef UDEBUG
  std::cout << "Battle lost! :-(\n";
#endif
  setBattleFlag(CombatState::OUTCOME_DONE);
  setNextTurnState();
}

/* Returns enumeration of party death [None, Friends, Foes, Both] */
bool Battle::checkPartyDeath(Party* const check_party)
{
  if (check_party->getLivingMembers().size() == 0)
    return true;

  return false;
}

/* Cleanup before the end of a Battle turn */
void Battle::cleanUp()
{
  action_buffer->clearAll();

  // TODO: clear any other data upon turn end? [03-16-14]

  /* Increment the turn counter */
  turns_elapsed++;

  action_buffer->update();

  setBattleFlag(CombatState::PHASE_DONE, true);

  // TODO: Auto win turns elapsed [03-01-14]
  if (turns_elapsed == 7)
    setBattleFlag(CombatState::VICTORY);
}

/* Determines the turn progression of the Battle (based on speed) */
void Battle::determineTurnMode()
{
  if (friends->getTotalSpeed() > foes->getTotalSpeed())
    setTurnMode(TurnMode::FRIENDS_FIRST);
  else if (friends->getTotalSpeed() < foes->getTotalSpeed())
    setTurnMode(TurnMode::ENEMIES_FIRST);
  else
  {
    if (Helpers::flipCoin())
      setTurnMode(TurnMode::FRIENDS_FIRST);
    else
      setTurnMode(TurnMode::ENEMIES_FIRST);
  }
}

/* Deals with general upkeep (i.e. weather) */
void Battle::generalUpkeep()
{
#ifdef UDEBUG
  printPartyState();
#else 
  if (battle_mode == BattleMode::TEXT)
    printPartyState();
#endif

  //TODO: Weather updates [03-01-14]

  /* General upkeep phase complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Sets the flags of BattleState at the beginning of the Battle */
void Battle::loadBattleStateFlags()
{
  setBattleFlag(CombatState::RANDOM_ENCOUNTER, true);

  if (foes->hasMiniBoss())
  {
    setBattleFlag(CombatState::RANDOM_ENCOUNTER, false);
    setBattleFlag(CombatState::MINI_BOSS, true);
  }
  if (foes->hasBoss())
  {
    setBattleFlag(CombatState::RANDOM_ENCOUNTER, false);
    setBattleFlag(CombatState::MINI_BOSS, false);
    setBattleFlag(CombatState::BOSS, true);
  }
  if (foes->hasFinalBoss())
  {
    setBattleFlag(CombatState::RANDOM_ENCOUNTER, false);
    setBattleFlag(CombatState::MINI_BOSS, false);
    setBattleFlag(CombatState::BOSS, false);
    setBattleFlag(CombatState::FINAL_BOSS, true);
  }

  setBattleFlag(CombatState::PHASE_DONE, false);
  setBattleFlag(CombatState::LOSS, false);
  setBattleFlag(CombatState::VICTORY, false);
  setBattleFlag(CombatState::OUTCOME_DONE, false);
  setBattleFlag(CombatState::ERROR_STATE, false);
  setBattleFlag(CombatState::FLAGS_CONFIGURED, true);
}

/* Orders the actions on the buffer by speed of the aggressor */
void Battle::orderActions()
{
  std::cout << "Action state prior to sorting: " << std::endl;
  action_buffer->print();
  std::cout << std::endl;

  /* Re-order item actions first, and skills by momentum of the user */
  action_buffer->reorder(BufferSorts::ITEM_FIRST, BufferSorts::MOMENTUM);

  std::cout << "Action state after sorting: " << std::endl;
  action_buffer->print();
  std::cout << std::endl;

  /* Order action state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Actually performs the actions in the buffer */
void Battle::performAction()
{
  //TODO: Perform actions [03-01-14]
}

/* Deals with character related upkeep */
void Battle::personalUpkeep(Person* const target)
{
  (void)target; //TOOD: [Warning]
  // clear flags for new turn (temp flags?)
  // process ailments
    // damage ailments
    // flag setting ailments
    // recalulate ailment factors
}

/* Process the actions (Items & Skills) in the buffer */
void Battle::processActions()
{
  // get list of actions for skills and items
  // if run, attempt to run
  // if defend, incresae def. by some factor
  // do Action   [+ critical, + miss rate]
  // if cooldown == 0
    // if offensive attack
      // find stats related to the skill of the user
      // find stats related to the skill of each target
      // for each target
        // deal damage based on these skill values
          // NOTES: CHECK FOR IGNORE FLAGS ALONG THE WAY
          // primary factor modifier * prim element of user
          // secondary factor modifier * secd element of user
          // find elemental strengths/weaknesses
            // add modifiers (one way, two ways, one way or one way
          // determine crit chance
            // if so, multiply by crit factor
          // determine hit rate
          // if so, add variance
          // determine ratio of target based on prim/secd elements
          // deal the damage
        // check party death upon each damage dealt
        // output info to BIB
    // if infliction
      // check for immunities
        // if not immune, chance for infliction
        // if successful, recalculate ailments
        // if failed, output message
    // stat changing? TBD
      // find each stat to change
      // find by amount or by factor
        // incr. stats by amt. or factor
    // if relieving, if chance occurs,
      // remove ailment if exists
      // update ailments

  /* Process Action stae complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Recalculates the ailments after they have been altered */
void Battle::recalculateAilments(Person* const target)
{
  (void)target; //TODO: [Warning]
  // find base stats of person
  // find all buff factors
  // OR find bubbify factor (bubbify == NO BUFFS)
  // disenstubulate factor
}

/* Calculates enemy actions and add them to the buffer */
void Battle::selectEnemyActions()
{
  // Calculate method by which for AI to choose actions
    // Easy AI:
      // Choose from skill list
      // Prioritize by skill value, each value point adds chance of using
      // offensive factor if HP > 35%
      // defensive factor if HP < 35%

  /* Select enemy action state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Calculates user actions and add them to the buffer */
void Battle::selectUserActions()
{
  /* Choose actions for each person */
  auto users = friends->getLivingMembers();
  std::vector<Skill*> available_skills;
  std::vector<std::pair<Item*, uint16_t>> available_items;

  for (auto it = begin(users); it != end(users); ++ it)
  {
    /* Obtain the users currently available skills */
    available_skills = friends->getMember(*it)->getUseableSkills();

    if (friends->getInventory() != nullptr)
      available_items  = friends->getInventory()->getBattleItems();

    /* Assign the skills currently selectable on the BattleMenu */
    menu->reset();
    menu->setSelectableSkills(available_skills);
    menu->setSelectableItems(available_items);

    /* Get a user selected choice from the menu (choice is index of skills) */
    if (menu->selectAction())
    {
      // auto action_type    = menu->getActionType();
      // auto action_index   = menu->getActionIndex();
      // auto action_targets = menu->getActionTargets();
    }

  }

    // Find valid targets for the skill
      // Select target from list
    // Add skill from skill buffer
    // Remove required QD from person

    // Find Battle Usable items in inventory
    // Select item
      // Find valid targets
      // Choose from targets
      // Remove item from inventory

  /* Select user action state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Load default configuration of the battle */
bool Battle::setupClass()
{
  // info_bar = nullptr;
  // menu = nullptr;
  // status_bar = nullptr;

  action_buffer = nullptr;
  bg            = nullptr;
  config        = nullptr;
  status_bar_bg = nullptr;

  ailment_update_mode = BattleOptions::FOREST_WALK;
  hud_display_mode    = BattleOptions::FOREST_WALK;
  battle_mode         = BattleMode::TEXT;
  turn_mode           = TurnMode::FRIENDS_FIRST;
  flags = static_cast<CombatState>(0);

  screen_height          = 0;
  screen_width           = 0;
  time_elapsed           = 0;
  time_elapsed_this_turn = 0;
  turns_elapsed          = 0;

  turn_state = TurnState::BEGIN;
  setBattleFlag(CombatState::CONFIGURED, true);

  return true;
}

/* Method which calls personal upkeeps */
void Battle::upkeep()
{
  /* Friends update */
  for (uint32_t i = 0; i < friends->getSize(); i++)
    personalUpkeep(friends->getMember(i));

  /* Foes update */
  for (uint32_t i = 0; i < foes->getSize(); i++)
    personalUpkeep(foes->getMember(i));

  /* Personal upkeep state complete */
  setBattleFlag(CombatState::PHASE_DONE);
}

/* Assigns a new value to the ailment update mode */
void Battle::setAilmentUpdateMode(const BattleOptions &new_value)
{
  ailment_update_mode = new_value;
}

/* Assigns a new value to the battle output mode */
void Battle::setBattleMode(const BattleMode &new_value)
{
  battle_mode = new_value;
}

/* Assigns the friends party of the Battle */
bool Battle::setFriends(Party* const new_friends)
{
  if (new_friends != nullptr)
  {
    friends = new_friends;

    return true;
  }

  return false;
}

/* Assigns the foes party of the Battle */
bool Battle::setFoes(Party* const new_foes)
{
  if (new_foes != nullptr)
  {
    foes = new_foes;

    return true;
  }

  return false;
}

/* Assigns a new value to the hud display mode */
void Battle::setHudDisplayMode(const BattleOptions &new_value)
{
  hud_display_mode = new_value;
}

/* Updates the Battle to the next state */
void Battle::setNextTurnState()
{
 /* Set the CURRENT_STATE to incomplete */
  setBattleFlag(CombatState::PHASE_DONE, false);

  /* If the Battle victory/loss has been complete, go to Destruct */
  if (getBattleFlag(CombatState::OUTCOME_DONE))
  {
    setTurnState(TurnState::DESTRUCT);
    //Todo: Eventhandler battle finish signal?
  }

  if (getTurnState() != TurnState::DESTRUCT)
  {
    /* If the Battle has been won, go to victory */
    if (getBattleFlag(CombatState::VICTORY))
    {
      setTurnState(TurnState::VICTORY);
      battleWon();
    }

    /* If the Battle has been lost, GAME OVER */
    if (getBattleFlag(CombatState::LOSS))
    {
      setTurnState(TurnState::LOSS);
      battleLost();
    }

    /* After the Battle Begins, the general turn loop begins at General Upkeep */
    if (turn_state == TurnState::BEGIN)
    {
      setTurnState(TurnState::GENERAL_UPKEEP);
      generalUpkeep();
    }

    /* After general upkeep, each personal upkeep takes place */
    else if (turn_state == TurnState::GENERAL_UPKEEP)
    {
      setTurnState(TurnState::UPKEEP);
      upkeep();
    }

    /* After presonal upkeeps, the first turn order party selects actions */
    else if (turn_state == TurnState::UPKEEP)
    {
      if (turn_mode == TurnMode::FRIENDS_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ALLY);
        selectUserActions();
      }
      else if (turn_mode == TurnMode::ENEMIES_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ENEMY);
        selectEnemyActions();
      }
    }

    /* After the user selects actions, the enemy party may still need to
       select actions, or if not, order actions is called  */
    else if (turn_state == TurnState::SELECT_ACTION_ALLY)
    {
      if (turn_mode == TurnMode::FRIENDS_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ENEMY);
        selectEnemyActions();
      }
      else if (turn_mode == TurnMode::ENEMIES_FIRST)
      {
        setTurnState(TurnState::ORDER_ACTIONS);
        orderActions();
      }
    }

    /* After enemies select actions, the users may still need to select actios,
       or if not, order actions is called  */
    else if (turn_state == TurnState::SELECT_ACTION_ENEMY)
    {
      if (turn_mode == TurnMode::FRIENDS_FIRST)
      {
        setTurnState(TurnState::ORDER_ACTIONS);
        orderActions();
      }
      else if (turn_mode == TurnMode::ENEMIES_FIRST)
      {
        setTurnState(TurnState::SELECT_ACTION_ALLY);
        selectUserActions();
      }
    }

    /* After the actions are ordered, the actions are processed */
    else if (turn_state == TurnState::ORDER_ACTIONS)
    {
      setTurnState(TurnState::PROCESS_ACTIONS);
      cleanUp();
    }

    /* After the actions are processed, Battle turn clean up occurs */
    else if (turn_state == TurnState::PROCESS_ACTIONS)
    {
      setTurnState(TurnState::CLEAN_UP);
      cleanUp();
    }

    /* After the end of the turn, loop restarts at general upkeep */
    else if (turn_state == TurnState::CLEAN_UP)
    {
      setTurnState(TurnState::GENERAL_UPKEEP);
      generalUpkeep();
    }
  }
}

/* Assigns a new value for the screen width */
void Battle::setScreenHeight(const uint16_t &new_value)
{
  screen_height = new_value;
}

/* Assigns a new value for the screen width */
void Battle::setScreenWidth(const uint16_t &new_value)
{
  screen_width = new_value;
}

/* Assigns a new value to the elapsed time */
void Battle::setTimeElapsed(const int32_t &new_value)
{
  time_elapsed = new_value;
}

/* Assigns thee time elapsed this turn */
void Battle::setTimeElapsedThisTurn(const int32_t &new_value)
{
  time_elapsed_this_turn = new_value;
}

/* Assigns a new value to the turns elapsed */
void Battle::setTurnsElapsed(const uint16_t &new_value)
{
  turns_elapsed = new_value;
}

/* Assigns a new turn mode to the Battle */
void Battle::setTurnMode(const TurnMode &new_turn_mode)
{
  turn_mode = new_turn_mode;
}

/* Updates the turn state of the Battle */
void Battle::setTurnState(const TurnState &new_turn_state)
{
  turn_state = new_turn_state;

#ifdef UDEBUG
  printTurnState();
#endif

}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Methods to print information about the Battle */
void Battle::printAll(const bool &simple, const bool &flags, const bool &party)
{
  std::cout << "==== Battle ====\n";

  if (simple)
  {

  }
  else
  {
    if (ailment_update_mode == BattleOptions::FOREST_WALK)
      std::cout << "Ailment update mode: FOREST_WALK\n";
    else if (ailment_update_mode == BattleOptions::BEARLY_DIFFICULT)
      std::cout << "Ailment update mode: BEARLY_DIFFICULT\n";
    else if (ailment_update_mode == BattleOptions::GRIZZLY)
      std::cout << "Ailment update mode: GRIZZLY\n";

    if (hud_display_mode == BattleOptions::FOREST_WALK)
      std::cout << "Hud display mode: FOREST_WALK\n";
    else if (hud_display_mode == BattleOptions::BEARLY_DIFFICULT)
      std::cout << "Hud display mode: BEARLY_DIFFICULT\n";
    else if (hud_display_mode == BattleOptions::GRIZZLY)
      std::cout << "Hud display mode: GRIZZLY\n";

    std::cout << "Friends Size: " << friends->getSize();
    std::cout << "\nFoes Size: " << foes->getSize();
    std::cout << "\nScreen Height: " << screen_height;
    std::cout << "\nScreen Width: " << screen_width;
    std::cout << "\nTime Elapsed: " << time_elapsed;
    std::cout << "\nTurns Elapsed: " << turns_elapsed;
    //std::cout << "\n# Enemy Bars: " << enemy_bars.size();
    std::cout << "\n\n";

    if (flags)
    {
      std::cout << "CONFIGURED: " << getBattleFlag(CombatState::CONFIGURED);
      std::cout << "\nFLAGS_CONFIGURED: " 
                << getBattleFlag(CombatState::FLAGS_CONFIGURED);
      std::cout << "\nPHASE_DONE: " << getBattleFlag(CombatState::PHASE_DONE);
      std::cout << "\nVICTORY: " << getBattleFlag(CombatState::VICTORY);
      std::cout << "\nLOSS: " << getBattleFlag(CombatState::LOSS);
      std::cout << "\nOUTCOME_DONE: " 
                << getBattleFlag(CombatState::OUTCOME_DONE);
      std::cout << "\nERROR_STATE: " << getBattleFlag(CombatState::ERROR_STATE);
      std::cout << "\n\n";
    }

    if (party)
      printPartyState();

    // menu->printInfo() TODO: [03-01-14]
  }

  std::cout << "==== / Battle ====\n\n";
}

void Battle::printPartyState()
{
  std::cout << "---- Friends ----\n";
  for (uint32_t i = 0; i < friends->getSize(); i++)
  {
    auto temp = friends->getMember(i);
    std::cout << temp->getName() << " " << temp->getCurr().getStat(0) << "\n";
  }

  std::cout << "---- Foes ----\n";
  for (uint32_t i = 0; i < foes->getSize(); i++)
  {
    auto temp = foes->getMember(i);
    std::cout << temp->getName() << " " << temp->getCurr().getStat(0) << "\n";
  }
}

void Battle::printTurnState()
{
  std::cout << "Current battle state: ";
  if (turn_state == TurnState::BEGIN)
    std::cout << "BEGIN\n";
  else if (turn_state == TurnState::GENERAL_UPKEEP) 
    std::cout << "GENERAL_UPKEEP\n";
  else if (turn_state == TurnState::UPKEEP)
    std::cout << "UPKEEP\n";
  else if (turn_state == TurnState::SELECT_ACTION_ALLY)
    std::cout << "SELECT_ACTION_ALLY\n";
  else if (turn_state == TurnState::SELECT_ACTION_ENEMY)
    std::cout << "SELECT_ACTION_ENEMY\n";
  else if (turn_state == TurnState::ORDER_ACTIONS)
    std::cout << "ORDER_ACTIONS\n";
  else if (turn_state == TurnState::PROCESS_ACTIONS)
    std::cout << "PROCESS_ACTIONS\n";
  else if (turn_state == TurnState::CLEAN_UP)
    std::cout << "CLEAN_UP\n";
  else if (turn_state == TurnState::LOSS)
    std::cout << "LOSS\n";
  else if (turn_state == TurnState::VICTORY)
    std::cout << "VICTORY\n";
  else if (turn_state == TurnState::DESTRUCT)
    std::cout << "DESTRUCT\n";
}

/* Update the cycle time of Battle */
bool Battle::update(int32_t cycle_time)
{
  setTimeElapsed(cycle_time);

  // update(); TODO: Update the battle interface

  if (getBattleFlag(CombatState::PHASE_DONE))
  {
    std::cout << "setting next turn state" << std::endl;
    setNextTurnState();
  }

  return false;
}

/* Returns the ailment update mode currently set */
BattleOptions Battle::getAilmentUpdateMode()
{
  return ailment_update_mode;
}

/* Returns the assigned Battle display mode */
BattleMode Battle::getBattleMode()
{
  return battle_mode;
}

/* Return the value of a given CombatState flag */
bool Battle::getBattleFlag(CombatState test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/* Returns the friends pointer of the Battle */
Party* Battle::getFriends()
{
  return friends;
} 

/* Returns the foes pointer of the Battle */
Party* Battle::getFoes()
{
  return foes;
}

/* Returns the hud display mode currently set */
BattleOptions Battle::getHudDisplayMode()
{
  return hud_display_mode;
}

/* Returns the value of the screen height */
uint32_t Battle::getScreenHeight()
{
  return screen_height;
}

/* Returns the value of the screen width */
uint32_t Battle::getScreenWidth()
{
  return screen_width;
}

/* Evaluates and returns a vector of ailments for a given person */
std::vector<Ailment*> Battle::getPersonAilments(Person* const target)
{
  std::vector<Ailment*> person_ailments;

  if (target != nullptr)
    for (auto it = begin(ailments); it != end(ailments); ++it)
      if ((*it).second->getVictim() == target)
        person_ailments.push_back((*it).second);

  return person_ailments;
}

/* Returns the value of the turns elapsed */
uint32_t Battle::getTurnsElapsed()
{
  return turns_elapsed;
}

/* Returns the elapsed time of the Battle */
uint32_t Battle::getTimeElapsed()
{
  return time_elapsed;
}

/* Returns the enumerated turn state of the Battle */
TurnState Battle::getTurnState()
{
  return turn_state;
}

/* Assings the running config */
bool Battle::setConfiguration(Options* const new_config)
{
  if (config != nullptr)
  {
    config = new_config;

    if (menu != nullptr)
      menu->setConfiguration(config);

    setScreenHeight(config->getScreenHeight());
    setScreenWidth(config->getScreenWidth());

    setAilmentUpdateMode(config->getAilmentUpdateState());
    setHudDisplayMode(config->getBattleHudState());
    setBattleMode(config->getBattleMode());

    return true;
  }
  return false;
}

/* Assign a value to a CombatState flag */
void Battle::setBattleFlag(CombatState flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/* Public static gets for menu constant values */
uint32_t Battle::getGenUpkeepDelay()
{
  return kGENERAL_UPKEEP_DELAY;
}

uint32_t Battle::getBattleMenuDelay()
{
  return kBATTLE_MENU_DELAY;
}

/* Public static gets for battle modifier values */
uint32_t Battle::getMaxAilments()
{
  return kMAX_AILMENTS;  
}

uint32_t Battle::getMaxEachAilments()
{
  return kMAX_EACH_AILMENTS;
}

uint32_t Battle::getMaxDamage()
{
  return kMAXIMUM_DAMAGE;
}

uint32_t Battle::getMinDamage()
{
  return kMINIMUM_DAMAGE;
}

float Battle::getOffPrimElmMod()
{
  return kOFF_PRIM_ELM_MODIFIER;
}

float Battle::getDefPrimElmMod()
{
  return kDEF_PRIM_ELM_MODIFIER;
}

float Battle::getOffSecdElmMod()
{
  return kOFF_SECD_ELM_MODIFIER;
}

float Battle::getDefSecdElmMod()
{
  return kDEF_SECD_ELM_MODIFIER;
}

float Battle::getOffCritMod()
{
  return kOFF_CRIT_MODIFIER;
}

float Battle::getDefCritMod()
{
  return kDEF_CRIT_MODIFIER;
}

float Battle::getBaseCritMod()
{
  return kBASE_CRIT_MODIFIER;
}

float Battle::getDodgeMod()
{
  return kDODGE_MODIFIER;
}

float Battle::getDogePerLvlMod()
{
  return kDODGE_PER_LEVEL_MODIFIER;
}

float Battle::getPrimElmAdvMod()
{
  return kPRIM_ELM_ADV_MODIFIER;
}

float Battle::getPrimElmDisMod()
{
  return kPRIM_ELM_DIS_MODIFIER;
}

float Battle::getSecdElmAdvMod()
{
  return kSECD_ELM_ADV_MODIFIER;
}

float Battle::getSecdElmDisMod()
{
  return kSECD_ELM_DIS_MODIFIER;
}

float Battle::getDoubleElmAdvMod()
{
  return kDOUBLE_ELM_ADV_MODIFIER;
}

float Battle::getDoubleElmDisMod()
{
  return kDOUBLE_ELM_DIS_MODIFIER;
}