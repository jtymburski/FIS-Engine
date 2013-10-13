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
 * Inputs:
 * Output:
 */
bool Battle::loadDefaults()
{

}

/*
 * Description: Assigns a new value to the elapsed time.
 *
 * Inputs: uint - the new value of the elapsed time
 * Output: none
 */
void Battle::setTimeElapsed(uint new_value)
{

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
  friends = new_friends;
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
  foes = new_foes;
}

/*
 * Description: Assigns a new value for the screen height
 *
 * Inputs: new_value - uint value for the screen height
 * Output: none
 */
void Battle::setScreenHeight(uint new_value)
{
  screen_height = new_value;
}

/*
 * Description: Assigns a new value for the screen width
 *
 * Inputs: new_value - uint value for the screen width
 * Output: none
 */
/* Assign a new value for the screen width */
void Battle::setScreenWidth(uint new_value)
{
  screen_width = new_value;
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
 * Description: Returns the screen height of the Battle
 *
 * Inputs: none
 * Output: uint - the height of the Battle screen
 */
uint Battle::getScreenHeight()
{
  return screen_height;
}

/*
 * Description: Returns the screen width of the Battle
 *
 * Inputs: none
 * Output: uint - the width of the Battle screen
 */
uint Battle::getScreenWidth()
{
  return screen_width;
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

