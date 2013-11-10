/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: December 2nd, 2012 - Rewritten October 12, 2013
* Inheritance: QWidget
* Description: Battle is a class which contains all the events handled and
*              happening in a classic RPG battle between two Parties.
*
* Notes:
*
* See Header for TODOs.
*******************************************************************************/

#include "Game/Battle/Battle.h"

/*==============================================================================
 * CONSTANTS
 *============================================================================*/

/* ------------ Menu Constants --------------- */
const ushort Battle::kDEFAULT_SCREEN_HEIGHT = 1216; /* The default height */
const ushort Battle::kDEFAULT_SCREEN_WIDTH  =  708; /* The default width */
const ushort Battle::kGENERAL_UPKEEP_DELAY  =  500; /* Time prior info bar msg */
const ushort Battle::kBATTLE_MENU_DELAY     =  400; /* Personal menu delay */

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

const ushort Battle::kMINIMUM_DAMAGE          =     1;
const ushort Battle::kMAXIMUM_DAMAGE          = 29999;
const float Battle::kOFF_PRIM_ELM_MODIFIER    =  1.07;
const float Battle::kDEF_PRIM_ELM_MODIFIER    =  1.04;
const float Battle::kOFF_SECD_ELM_MODIFIER    =  1.05;
const float Battle::kDEF_SECD_ELM_MODIFIER    =  1.03;
const float Battle::kOFF_CRIT_MODIFIER        =  1.10;
const float Battle::kDEF_CRIT_MODIFIER        =  0.90;
const float Battle::kBASE_CRIT_MODIFIER       =  1.25;
const float Battle::kDODGE_MODIFIER           =  1.10;
const float Battle::kDODGE_PER_LEVEL_MODIFIER =  1.04;
const float Battle::kPRIM_ELM_ADV_MODIFIER    =  1.15;
const float Battle::kPRIM_ELM_DIS_MODIFIER    =  0.87;
const float Battle::kSECD_ELM_ADV_MODIFIER    =  1.10;
const float Battle::kSECD_ELM_DIS_MODIFIER    =  0.93;
const float Battle::kDOUBLE_ELM_ADV_MODIFIER  =  1.30;
const float Battle::kDOUBLE_ELM_DIS_MODIFIER  =  0.74;

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Normal battle constructor - constructs a Battle between two
 *              parties - friends vs. foes, also given the Options config to
 *              assign various options for the Battle
 *
 * Inputs: Party* friends - the player's party in the Battle
 *         Party* foes - the player's enemies in the Battle
 *         Options - the configuration passed on to the Battle
 */
Battle::Battle(Party *friends, Party *foes, Options config, QWidget *parent)
    : QWidget(parent),
      friends(friends),
      foes(foes)
{
  setTurnState(Battle::BATTLE_BEGIN);

  setScreenHeight(config.getScreenHeight());
  setScreenWidth(config.getScreenWidth());
  setFixedSize(getScreenWidth(), getScreenHeight());

  setTimeElapsed(0);
  setTurnsElapsed(0);

  setAilmentUpdateMode(config.getAilmentUpdateState());
  setHudDisplayMode(config.getBattleHudState());
  setBattleMode(config.getBattleMode());

  setBattleFlag(Battle::CONFIGURED);
  loadBattleStateFlags();

  /* Create battle GUI elements */
  info_bar    = new BattleInfoBar(getScreenWidth(), getScreenHeight(), this);
  menu        = new BattleMenu(this);
  status_bar  = new BattleStatusBar(getFriends(), getScreenWidth(),
                                    getScreenHeight(), this);
  /* Create buffers */
  item_buffer  = new ItemBuffer();
  skill_buffer = new SkillBuffer();

  /* Battle Begin state complete */
  setBattleFlag(Battle::CURRENT_STATE_COMPLETE);
}

/*
 * Description: Annihilates a Battle object
 *
 */
Battle::~Battle()
{
  if (info_bar != 0)
    delete info_bar;
  info_bar = 0;

  if (menu != 0)
    delete menu;
  menu = 0;

  if (status_bar != 0)
    delete status_bar;
  status_bar = 0;

  if (item_buffer != 0)
    delete item_buffer;
  item_buffer = 0;

  if (skill_buffer != 0)
    delete skill_buffer;
  skill_buffer = 0;

  if (!enemy_status_bars.isEmpty())
    qDeleteAll(enemy_status_bars);

 // if (bg != 0)
 //   delete bg;
 // bg = 0;

 // if (status_bar_bg != 0)
 //   delete status_bar_bg;
 // status_bar_bg = 0;
}

/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::battleWon()
{
  //  clear ailments, clear other data

  // call victory
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

  if (getBattleMode() == Options::DEBUG)
    qDebug() << "Battle Victorious! :-)";

  setBattleFlag(Battle::BATTLE_OUTCOME_COMPLETE);
  setNextTurnState();
}


/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::battleLost()
{
  // clear party members, clear ailments
  // return to title?

  if (getBattleMode() == Options::DEBUG)
    qDebug() << "Battle Lost! :-(";

  setBattleFlag(Battle::BATTLE_OUTCOME_COMPLETE);
  setNextTurnState();
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Battle::checkPartyDeath(Party* check_party)
{
  for (int i = 0; i < check_party->getPartySize(); i++)
    if (check_party->getMember(i)->getStats()->getStat("VITA") == 0)
      return true;

  return false;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::cleanUp()
{
  // clears stacks of actions
  // clear other data



  /* Increment the turn counter */
  turns_elapsed++;

  /* Clean Up state complete */
  setBattleFlag(CURRENT_STATE_COMPLETE);

  //TODO: Battle wins after 20 turns [11-09-13]
  if (turns_elapsed == 20)
    setBattleFlag(BATTLE_WON);
}

/*
 * Description: This function deals with general battle upkeep, such as
 *              factors like gravity, weather, or other functions
 *              which affect the entire party or both parties.
 *
 * Inputs: none
 * Output: none
 */
void Battle::generalUpkeep()
{
  /* Print out the party state in debug mode */
  if (getBattleMode() == Options::DEBUG)
    printPartyState();

  //TODO: Weather updates [11-09-13]

  /* General Upkeep state complete */
  setBattleFlag(CURRENT_STATE_COMPLETE);
}

/*
 * Description: Loads a default configuration of the Battle, if it has not
 *              already been configured.
 *
 * Inputs: none
 * Output: bool - true if the Battle was set for the first time
 */
bool Battle::loadDefaults()
{
  setScreenHeight(kDEFAULT_SCREEN_HEIGHT);
  setScreenWidth(kDEFAULT_SCREEN_WIDTH);
  setFixedSize(getScreenWidth(), getScreenHeight());

  setTimeElapsed(0);
  setTurnsElapsed(0);

  setAilmentUpdateMode(Options::BEARWALK);
  setHudDisplayMode(Options::BEARWALK);
  setBattleMode(Options::DEBUG);

  if (getBattleFlag(Battle::FLAGS_CONFIGURED))
    return false;

  setBattleFlag(Battle::FLAGS_CONFIGURED, true);
  setBattleFlag(Battle::CURRENT_STATE_COMPLETE, false);
  setBattleFlag(Battle::BATTLE_WON, false);
  setBattleFlag(Battle::BATTLE_WON, false);

  setTurnState(GENERAL_UPKEEP);
  return true;
}

/*
 * Description: Finds and assigns the proper BattleState flags at the beginning
 *              of a Battle.
 *
 * Inputs: none
 * Output: bool - true if the flags were set for the first time
 */
bool Battle::loadBattleStateFlags()
{
  setBattleFlag(Battle::RANDOM_ENCOUNTER, true);

  if (foes->hasMiniBoss())
  {
    setBattleFlag(Battle::RANDOM_ENCOUNTER, false);
    setBattleFlag(Battle::MINI_BOSS, true);
  }
  if (foes->hasBoss())
  {
    setBattleFlag(Battle::RANDOM_ENCOUNTER, false);
    setBattleFlag(Battle::MINI_BOSS, false);
    setBattleFlag(Battle::BOSS, true);
  }
  if (foes->hasFinalBoss())
  {
    setBattleFlag(Battle::RANDOM_ENCOUNTER, false);
    setBattleFlag(Battle::MINI_BOSS, false);
    setBattleFlag(Battle::BOSS, false);
    setBattleFlag(Battle::FINAL_BOSS, false);
  }

  if (getBattleFlag(Battle::FLAGS_CONFIGURED))
    return false;

  setBattleFlag(Battle::FLAGS_CONFIGURED, true);
  return true;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::orderActions()
{
  // Items are fast -- item buffer first
  // Skills are slow
    // Sort skills by momentum of the skill user

  /* Order action state complete */
  setBattleFlag(Battle::CURRENT_STATE_COMPLETE);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::performAction()
{


}

/*
 * Description: Deals with the upkeep specific to a person (given a uint
 *              target), such as ailment updates, etc.
 *
 * Inputs: uint target - the target of personal upkeep to take place.
 * Output: none
 */
void Battle::personalUpkeep(uint target)
{
  (void)target;//WARNING
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::processActions()
{
  // get list of actions for skills and items
  // if run, attempt to run
  // if defend, incresae def. by some factor
  // do Action   [+ critical, + miss rate]
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

    //

  /* Process Action stae complete */
  setBattleFlag(CURRENT_STATE_COMPLETE);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::recalculateAilments()
{
  // find base stats of person
  // find all buff factors
  // OR find bubbify factor (bubbify == NO BUFFS)
  // disenstubulate factor
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::selectEnemyActions()
{
  // Calculate method by which for AI to choose actions
    // Easy AI:
      // Choose from skkill list
      // Prioritize by skill value, each value point adds chance of using
      // offensive factor if HP > 35%
      // defensive factor if HP < 35%

  /* Select enemy action state complete */
  setBattleFlag(CURRENT_STATE_COMPLETE);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::selectUserActions()
{
  // Choose actions from menu
    // Choose skill from list of valid skills available
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
  setBattleFlag(CURRENT_STATE_COMPLETE);
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::upkeep()
{

  /* Personal upkeep state complete */
  setBattleFlag(Battle::CURRENT_STATE_COMPLETE);
}

/*
 * Description: Assigns a new value to the ailment update mode
 *
 * Inputs: Options::BattleOptions - enumerated value for ailment update mode
 * Output: none
 */
 void Battle::setAilmentUpdateMode(Options::BattleOptions new_value)
 {
   ailment_update_mode = new_value;
 }

 /*
 * Description: Assigns a new value to the battle output mode
 *
 * Inputs: Options::BattleMode - enumerated value of battle output mode
 * Output: none
 */
 void Battle::setBattleMode(Options::BattleMode new_value)
 {
   battle_mode = new_value;
 }

/*
 * Description: Assigns a new value to the elapsed time.
 *
 * Inputs: uint - the new value of the elapsed time
 * Output: none
 */
void Battle::setTimeElapsed(uint new_value)
{
  time_elapsed = new_value;
  emit battleTimerUpdate();
}

/*
 * Description: Assigns the friends pointer of the Battle
 *
 * Inputs: new_friends - Party pointer for the new friends of the Battle
 * Output: none
 */
/* Assigns the friends party of the Battle */
void Battle::setFriends(Party* new_friends)
{
  if (!getBattleFlag(Battle::CONFIGURED))
    friends = new_friends;
  else
    emit battleError("Attempted reconfigure of friends");
}

/*
 * Description: Assigns the foes pointer of the Battle.
 *
 * Inputs: new_foes - Party pointer for the new foes of the Battle
 * Output: none
 */
/* Assigns the foes party of the Battle */
void Battle::setFoes(Party* new_foes)
{
  if (!getBattleFlag(Battle::CONFIGURED))
    foes = new_foes;
  else
    emit battleError("Attempted reconfigure of foes");
}

/*
 * Description: Assigns a new value to the hud display mode
 *
 * Inputs: Options::BattleOptions - enumerated value for hud display mode
 * Output: none
 */
void Battle::setHudDisplayMode(Options::BattleOptions new_value)
{
  hud_display_mode = new_value;
}

/*
 * Description: Updates the state of the Battle
 *
 * Notes [1]: This function should be called after CURRENT_STATE_COMPLETE
 *            flag is set.
 *
 * Inputs: none
 * Output: none
 */
void Battle::setNextTurnState()
{
  /* Set the CURRENT_STATE to incomplete */
  setBattleFlag(Battle::CURRENT_STATE_COMPLETE, false);

  /* If the Battle victory/loss has been complete, go to Destruct */
  if (getBattleFlag(Battle::BATTLE_OUTCOME_COMPLETE))
  {
    setTurnState(BATTLE_DESTRUCT);
    emit finished();
  }

  if (getTurnState() != BATTLE_DESTRUCT)
  {
    /* If the Battle has been won, go to victory */
    if (getBattleFlag(Battle::BATTLE_WON))
    {
      setTurnState(BATTLE_VICTORY);
      battleWon();
    }

    /* If the Battle has been lost, GAME OVER */
    if (getBattleFlag(Battle::BATTLE_LOST))
    {
      setTurnState(BATTLE_LOSS);
      battleLost();
    }

    /* After the Battle Begins, the general turn loop begins at General Upkeep */
    if (turn_state == BATTLE_BEGIN)
    {
      setTurnState(GENERAL_UPKEEP);
      generalUpkeep();
    }

    /* After general upkeep, each personal upkeep takes place */
    else if (turn_state == GENERAL_UPKEEP)
    {
      setTurnState(UPKEEP);
      upkeep();
    }

    /* After presonal upkeeps, the user selects actions from the Battle Menu */
    else if (turn_state == UPKEEP)
    {
      setTurnState(SELECT_USER_ACTION);
      selectUserActions();
    }

    /* After the user selects actions, the enemy chooses actions */
    else if (turn_state == SELECT_USER_ACTION)
    {
      setTurnState(SELECT_ENEMY_ACTION);
      selectEnemyActions();
    }

    /* After enemies select actions, the actions are ordered */
    else if (turn_state == SELECT_ENEMY_ACTION)
    {
      setTurnState(ORDER_ACTIONS);
      orderActions();
    }

    /* After the actions are ordered, the actions are processed */
    else if (turn_state == ORDER_ACTIONS)
    {
      setTurnState(PROCESS_ACTIONS);
      cleanUp();
    }

    /* After the actions are processed, Battle turn clean up occurs */
    else if (turn_state == PROCESS_ACTIONS)
    {
      setTurnState(CLEAN_UP);
      cleanUp();
    }

    /* After the end of the turn, loop restarts at general upkeep */
    else if (turn_state == CLEAN_UP)
    {
      setTurnState(GENERAL_UPKEEP);
      generalUpkeep();
    }
  }
}

/*
 * Description: Assigns a new value for the screen height
 *
 * Inputs: new_value - uint value for the screen height
 * Output: none
 */
void Battle::setScreenHeight(ushort new_value)
{
  if (new_value != 0)
    screen_height = new_value;
  else
    screen_height = kDEFAULT_SCREEN_HEIGHT;
}

/*
 * Description: Assigns a new value for the screen width
 *
 * Inputs: new_value - uint value for the screen width
 * Output: none
 */
void Battle::setScreenWidth(ushort new_value)
{
  if (new_value != 0)
    screen_width = new_value;
  else
    screen_height = kDEFAULT_SCREEN_WIDTH;
}

/*
 * Description: Attempts to assign a new value to the currently selected
 *              targeting index. Returns true if this was assigned, false
 *              otherwise (as in the index in the currently selected party does
 *              not exist).
 *
 * Inputs: uint - new value to assign the party index to
 * Output: bool - true if the targeting index was set
 */
bool Battle::setTargetIndex(uint new_value)
{
  /* Check for Friends Index */
  if (getTargetingMode() == ACTIVE_OVER_FOES)
  {
    if (new_value < foes->getPartySize())
    {
      target_index = new_value;
      return true;
    }
  }

  /* Check for Foes Index */
  else if (getTargetingMode() == ACTIVE_OVER_FRIENDS)
  {
    if (new_value < friends->getPartySize())
    {
      target_index = new_value;
      return true;
    }
  }

  return false;
}


/*
 * Description: Assigns a new targeting mode value
 *
 * Inputs: TargetingMode - enumerated value to set the targeting value to.
 * Output: none
 */
 void Battle::setTargetingMode(TargetingMode new_value)
 {
   targeting_mode = new_value;
 }

/*
 * Description: Assigns a new value for turns elapsed
 *
 * Inputs: new_value - ushort value of new turns elapsed
 * Output: none
 */
void Battle::setTurnsElapsed(ushort new_value)
{
  if (new_value > turns_elapsed + 1 || new_value < turns_elapsed)
    emit battleTurnsReset();

  turns_elapsed = new_value;
}

/*
 * Description: Assigns a new enumerated value to the Battle turn state
 *
 * Inputs: TurnState - enumerated turn state of the current turn
 * Output: none
 */
void Battle::setTurnState(TurnState new_turn_state)
{
  /* Update the turn state */
  turn_state = new_turn_state;

  /* Output the new turn state in DEBUG mode */
  if (getBattleMode() == Options::DEBUG)
    printTurnState();
}

/*==============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*
 * Description: Handles all the key press events for Battle
 *
 * Inputs: QKeyEvent* - pointer to the key press event being handled
 * Output: none
 */
void Battle::keyPressEvent(QKeyEvent* event)
{
   // Temp - Delete on final build
   // // // // // //
   // Current Mapping of keys
   //
   // Escape - End the Battle
   //
   // End Temp

  /* Switch between possible keys */
  switch(event->key())
  {
    case Qt::Key_Escape:
      emit closeBattle();
      break;

    default:
      break;
  }
}

/*
 * Description: Returns the ailment update mode
 *
 * Inputs: none
 * Output: Options::BattleOptions - the currently set ailment update mode
 */
Options::BattleOptions Battle::getAilmentUpdateMode()
{
  return ailment_update_mode;
}

/*
 * Description: Returns the battle output mode currently set
 *
 * Inputs: none
 * Output: Options::BattleMode - the currently set Battle update mode
 */
Options::BattleMode Battle::getBattleMode()
{
  return battle_mode;
}

/*
 * Description: Returns the elapsed time of the Battle
 *
 * Inputs: none
 * Output: uint - the elapsed time of the Battle
 */
uint Battle::getTimeElapsed()
{
  return time_elapsed;
}

/*
 * Description: Returns the currently assigned enumerated Battle turn state
 *
 * Inputs: none
 * Output: Battle::TurnState - enumerated turn state of current turn
 */
Battle::TurnState Battle::getTurnState()
{
  return turn_state;
}

/*
 * Description: Returns the friends pointer of the Battle
 *
 * Inputs: none
 * Output: Party* - pointer to the friends of the Battle
 */
Party* Battle::getFriends()
{
  return friends;
}

/*
 * Description: Returns the foes pointer of the Battle
 *
 * Inputs: none
 * Output: Party* - pointer to the foes of the Battle
 */
Party* Battle::getFoes()
{
  return foes;
}

/*
 * Description: Returns the hud display mode
 *
 * Inputs: none
 * Output: Options::BattleOptions - the currently set hud display mode
 */
Options::BattleOptions Battle::getHudDisplayMode()
{
  return hud_display_mode;
}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
bool Battle::getPartyDeath()
{
  if (checkPartyDeath(friends) && checkPartyDeath(foes))
  {
    if (getBattleMode() == Options::DEBUG)
    {
      setBattleFlag(Battle::ERROR_STATE);
      qDebug() << "[ERROR] - Both parties are dead!";
    }
  }

  if (checkPartyDeath(friends))
    setBattleFlag(Battle::BATTLE_WON);
  if (checkPartyDeath(foes))
    setBattleFlag(Battle::BATTLE_LOST);

  return false;
}

/*
 * Description: Returns the screen height of the Battle
 *
 * Inputs: none
 * Output: uint - the height of the Battle screen
 */
ushort Battle::getScreenHeight()
{
  return screen_height;
}

/*
 * Description: Returns the screen width of the Battle
 *
 * Inputs: none
 * Output: uint - the width of the Battle screen
 */
ushort Battle::getScreenWidth()
{
  return screen_width;
}

/*
 * Description: Returns the currently assigned targeting mode
 *
 * Inputs: none
 * Output: TargetingMode - enumerated targeting mode value
 */
Battle::TargetingMode Battle::getTargetingMode()
{
  return targeting_mode;
}

/*
 * Description: Returns the turns elapsed of Battle
 *
 * Inputs: none
 * Output: ushort - the turns elapsed since the start of the Battle
 */
ushort Battle::getTurnsElapsed()
{
  return turns_elapsed;
}

/*==============================================================================
 * PUBLIC SLOTS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */

/*==============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Prints out all the information of the battle by calling
 *              the sub-print functions.
 *
 * Inputs: none
 * Output: none
 */
 void Battle::printAll()
 {
  qDebug() << "--- Battle ---";
  printFlags();
  printInfo();
  printPartyState();
  printOther();
  qDebug() << "--- /Battle ---";
 }

/*
 * Description: Prints all the values of the flags of the Battle
 *
 * Inputs: none
 * Output: none
 */
void Battle::printFlags()
{
  qDebug() << "CONFIGURED: "       << getBattleFlag(Battle::CONFIGURED);
  qDebug() << "FLAGS_CONFIGURED: " << getBattleFlag(Battle::FLAGS_CONFIGURED);
  qDebug() << "RANDOM_ENOUNTER: "  << getBattleFlag(Battle::RANDOM_ENCOUNTER);
  qDebug() << "MINI_BOSS: "        << getBattleFlag(Battle::MINI_BOSS);
  qDebug() << "BOSS: "             << getBattleFlag(Battle::BOSS);
  qDebug() << "FINAL_BOSS: "       << getBattleFlag(Battle::FINAL_BOSS);
  qDebug() << "CURRENT_STATE_COMPLETE: "
           << getBattleFlag(Battle::CURRENT_STATE_COMPLETE);
  qDebug() << "BATTLE_LOST: " << getBattleFlag(Battle::BATTLE_LOST);
  qDebug() << "BATTLE_WON: " << getBattleFlag(Battle::BATTLE_WON);
  qDebug() << "BATTLE_OUTCOME_COMPLETE"
           << getBattleFlag(Battle::BATTLE_OUTCOME_COMPLETE);
  qDebug() << "ERROR_STATE: " << getBattleFlag(Battle::ERROR_STATE);
}

/*
 * Description: Prints ou the general information about the Battle.
 *
 * Inputs: none
 * Output: none
 */
void Battle::printInfo()
{
  if (ailment_update_mode == Options::BEARWALK)
    qDebug() << "Ailment Update Mode: BEARWALK";
  if (ailment_update_mode == Options::BEARLY_DIFFICULT)
    qDebug() << "Ailment Update Mode: BEARLY_DIFFICULT";
  if (ailment_update_mode == Options::GRIZZLY)
    qDebug() << "Ailment Update Mode: GRIZZLY";

  if (hud_display_mode == Options::BEARWALK)
    qDebug() << "Hud Display Mode: BEARWALK";
  else if (hud_display_mode == Options::BEARLY_DIFFICULT)
    qDebug() << "Hud Display Mode: BEARLY_DIFFICULT";
  else if (hud_display_mode == Options::GRIZZLY)
    qDebug() << "Hud Display Mode: GRIZZLY";

  qDebug() << "Friends Size: "      << friends->getPartySize();
  qDebug() << "Foes Size: "         << foes->getPartySize();
  qDebug() << "Screen Height: "     << screen_height;
  qDebug() << "Screen Width: "      << screen_width;
  qDebug() << "Time Elapsed: "      << time_elapsed;
  qDebug() << "Turns Elapsed: "     << turns_elapsed;
  qDebug() << "Enemy Status Bars: " << enemy_status_bars.size();
}

/*
 * Description: Prints out a more in depth state of the Parties which are
 *              currently in the Battle.
 *
 * Inputs: none
 * Output: none
 */
 void Battle::printPartyState()
 {
   Person* temp;

   qDebug() << "--- Friends ---";
   for (int i = 0; i < friends->getPartySize(); i++)
   {
     temp = friends->getMember(i);
     qDebug() << temp->getName() << temp->getTemp()->getStat(0);
   }

   qDebug() << "--- Foes ---";
   for (int i = 0; i < foes->getPartySize(); i++)
   {
     temp = foes->getMember(i);
     qDebug() << temp->getName() << temp->getTemp()->getStat(0);
   }
}

/*
 * Description: Prints out the enumerated turn state of the Battle
 *
 * Inputs: none
 * Output: none
 */
 void Battle::printTurnState()
 {
  if (turn_state == BATTLE_BEGIN)
    qDebug() << "Current Battle State:: Battle Begin";
  if (turn_state == GENERAL_UPKEEP)
    qDebug() << "Current Battle State: General Upkeep";
  if (turn_state == UPKEEP)
    qDebug() << "Current Battle State:: Upkeep";
  if (turn_state == SELECT_USER_ACTION)
    qDebug() << "Current Battle State: Select User Action";
  if (turn_state == SELECT_ENEMY_ACTION)
    qDebug() << "Current Battle State: Select Enemy Action";
  if (turn_state == ORDER_ACTIONS)
    qDebug() << "Current Battle State: Order Actions";
  if (turn_state == PROCESS_ACTIONS)
    qDebug() << "Current Battle State: Process Actions";
  if (turn_state == CLEAN_UP)
    qDebug() << "Current Battle State: Clean Up";
  if (turn_state == BATTLE_LOSS)
    qDebug() << "Current Battle State: Battle Loss";
  if (turn_state == BATTLE_VICTORY)
    qDebug() << "Current Battle State: Battle Victory";
  if (turn_state == BATTLE_DESTRUCT)
    qDebug() << "Current Battle State: Battle Destruct";
 }

/*
 * Description: Prints out information about the menus relating to the Battle
 *
 * Inputs: none
 * Output: none
 */
void Battle::printOther()
{
  // if (info_bar != 0)
  //   info_bar->printInfo();

  // if (menu != 0)
  //  menu->printInfo();

  // if (staus_bar != 0)
  //   status_bar->printAll();
}

/*
 * Description: Updates the cycle time of the Battle
 *
 * Inputs: int - cycle time of the Battle (from Game)
 * Output: none
 */
void Battle::updateBattle(int cycle_time)
{
  setTimeElapsed(cycle_time);
  //TODO [10-12-13] updateGL();

  if (getBattleFlag(Battle::CURRENT_STATE_COMPLETE))
    setNextTurnState();
}

/*
 * Description: Evaluates a given BattleState flag
 *
 * Inputs: BattleState - flag to be evaluated
 * Output: Boolean evaluation of the flag
 */
bool Battle::getBattleFlag(BattleState flags)
{
  return flag_set.testFlag(flags);
}

/*
 * Description: Sets a BattleState flag to a given boolean value
 *
 * Inputs: BattleState flag
 *         Boolean value to set the flag to
 * Output: none
 */
void Battle::setBattleFlag(BattleState flags, bool set_value)
{
  (set_value) ? (flag_set |= flags) : (flag_set ^= flags);
}
