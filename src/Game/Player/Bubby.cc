/*******************************************************************************
* Class Name: Bubby
* Date Created: November 4th, 2012 (Rewritten March 6th, 2013)
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, etc.
* Notes : This uses the following formula: Exp(Level) = 50 + Exp(Level – 1)
*         x [1 + Multiplier / 100] Multiplier: 10-25
* TODO: Method of "storing" and creating different types of Bubby [02-07-13]
*******************************************************************************/
#include "Game/Player/Bubby.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a Bubby object
 *
 * Inputs: BubbyFlavour* - pointer to the type of the Bubby
 */
int Bubby::id = 0;  // Bubbies start at ID 0.

Bubby::Bubby(BubbyFlavour* type) : myId(setId())  //Increment Bubby's ID
{
  setType(type);
  setLevel(0);
  setTier(0);
  setExperience(0);
}

/*
 * Description: Destroy a Bubby object
 */
Bubby::~Bubby()
{
  delete type;
  type = NULL;
}

/*=============================================================================
 * FUNCTIONS
 *============================================================================*/

// TODO: Level up unfinished [03-07-13]
void Bubby::addExperience(uint amount)
{

}

/*
 * Description: Returns the ID of the Bubby
 *
 * Inputs: none
 * outputs: int - the ID of the Bubby
 */
int Bubby::getId()
{
  return id;
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
 * Description: Returns the level of the Bubby
 *
 * Inputs: none
 * Output: uint - level of the Bubby
 */
ushort Bubby::getLevel()
{
  return level;
}

/*
 * Description: Returns the tier of the Bubby
 *
 * Inputs: none
 * Output: uint - tier of the Bubby
 */
ushort Bubby::getTier()
{
  return tier;
}

BubbyFlavour* Bubby::getType()
{
  return type;
}

/*
 * Description: Returns the ID of the Bubby and increments it
 *
 * Inputs: none
 * Output: static int - the ID of the Bubby
 */
int Bubby::setId()
{
  return id++;
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
void Bubby::setLevel(ushort new_level)
{
  level = new_level;
}

/*
 * Description: Sets the tier of the Bubby
 *
 * Inputs: uint - new value of Tier of the Bubby
 * Output: none
 */
void Bubby::setTier(ushort new_tier)
{
  tier = new_tier;
}

void Bubby::setType(BubbyFlavour* new_type)
{
  type = new_type;
}
