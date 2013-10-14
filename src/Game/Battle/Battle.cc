/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: December 2nd, 2012 - Rewritten October 12, 2013
* Inheritance: QWidget
* Description:
*
* Notes:
*
* See Header for TODOs.
******************************************************************************/

#include "Game/Battle/Battle.h"

/*==============================================================================
 * CONSTANTS
 *============================================================================*/

const ushort Battle::kDEFAULT_SCREEN_HEIGHT = 1216;
const ushort Battle::kDEFAULT_SCREEN_WIDTH  =  708;

/*==============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Battle::Battle()
{
  loadDefaults();
}

/*
 * Description:
 *
 * Inputs:
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

  loadBattleStateFlags();
}

/*
 * Description:
 *
 */
Battle::~Battle()
{

}


/*==============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

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

  if (getBattleFlag(Battle::CONFIGURED))
    return false;

  setBattleFlag(Battle::CONFIGURED, true);
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

