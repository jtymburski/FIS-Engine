/******************************************************************************
* Class Name: Bubby Implementation
* Date Created: December 2nd, 2012
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, etc.
* Notes : This uses the following formula: Exp(Level) = 50 + Exp(Level – 1)
*         x [1 + Multiplier / 100] Multiplier: 10-25
* TODO: Finish constructor
******************************************************************************/
#include "Game/Player/Bubby.h"

/*
 * Description: Constructs a Bubby object
 *
 * Inputs:
 * Output:
 *
 * TODO: FINISH CONSTRUCTOR
 */

Bubby::Bubby()
{
  setLevel(1);
  setTier(0);
  setExperience(0);

  /* Increment the Bubby's ID counter */}

/*
 * Description: Destroy a Bubby object
 */
Bubby::~Bubby()
{
  //for (uint i = 0; i < action_list.size(); i++)
  // {
  //  delete action_list.at(i);
  //    action_list.at(i) = NULL;
  //}
}

/*
 * Description: Gets a list of actions that the Bubby can offer
 *
 * Inputs: none
 * Output: QVector of action pointers
 */
QVector<Action*> Bubby::getActionList()
{
  return action_list;
}

/*
 * Description: Returns the experience of the Bubby
 *
 * Inputs: none
 * Output: uint - experience value of the Bubby
 */
uint Bubby::getExp()
{
  return experience;
}

/*
 * Description: Returns the ID of the Bubby
 *
 * Inputs: none
 * Output: uint - the ID of the Bubby
 */
//uint Bubby::getId()
//{
//  return id;
//}

/*
 * Description: Returns the level of the Bubby
 *
 * Inputs: none
 * Output: uint - level of the Bubby
 */
uint Bubby::getLevel()
{
  return level;
}

/*
 * Description: Returns the tier of the Bubby
 *
 * Inputs: none
 * Output: uint - tier of the Bubby
 */
uint Bubby::getTier()
{
  return tier;
}

/*
 * Description: Sets the experience of the Bubby
 *
 * Inputs: uint - new value for experience of the Bubby
 * Output: none
 */
void Bubby::setExperience(uint new_experience)
{
    experience = new_experience;
}

/*
 * Description: Sets the level of the Bubby
 *
 * Inputs: uint - new value for level of the Bubby
 * Output: none
 */
void Bubby::setLevel(uint new_level)
{
  level = new_level;
}

/*
 * Description: Sets the tier of the Bubby
 *
 * Inputs: uint - new value of Tier of the Bubby
 * Output: none
 */
void Bubby::setTier(uint new_tier)
{
  tier = new_tier;
}
