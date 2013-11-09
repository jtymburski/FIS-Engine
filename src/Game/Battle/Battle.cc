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

}


/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::battleLost()
{

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

}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::processEnemyActions()
{

}

/*
 * Description:
 *
 * Inputs: none
 * Output: none
 */
void Battle::updateScene()
{

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
  if (getTargetingMode() == ACTIVE_OVER_FRIENDS)
  {
    if (new_value < foes->getPartySize())
    {
      target_index = new_value;
      return true;
    }
  }

  /* Check for Foes Index */
  else if (getTargetingMode() == ACTIVE_OVER_FOES)
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
  turn_state = new_turn_state;
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
 * SIGNALS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */


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
 * PUBLIC STATIC FUNCTIONS
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

  qDebug() << "Friends Size: "  << friends->getPartySize();
  qDebug() << "Foes Size: "     << foes->getPartySize();
  qDebug() << "Screen Height: " << screen_height;
  qDebug() << "Screen Width: "  << screen_width;
  qDebug() << "Time Elapsed: "  << time_elapsed;
  qDebug() << "Turns Elapsed: " << turns_elapsed;
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
   qDebug() << "--- /Friends ---";

   qDebug() << "--- Foes ---";
   for (int i = 0; i < foes->getPartySize(); i++)
   {
     temp = foes->getMember(i);
     qDebug() << temp->getName() << temp->getTemp()->getStat(0);
   }
   qDebug() << "--- /Foes ---";
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
  (set_value) ? (flag_set |= flags) : (flag_set &= flags);
}

