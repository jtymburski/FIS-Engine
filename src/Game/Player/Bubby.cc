/******************************************************************************
* Class Name: Bubby Implementation
* Date Created: December 2nd, 2012
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, etc.
* Notes : This uses the following formula: Exp(Level) = 50 + Exp(Level – 1)
*         x [1 + Multiplier / 100] Multiplier: 10-25
******************************************************************************/
#include "Game/Player/Action.h"
#include "Game/Player/Item.h"

/* Constructor function */
Bubby::Bubby()
{
}

/* Destructor function */
Bubby::~Bubby()
{
}

/* Gets the list of bubby actions (Used for total action lists in battle)*/
QVector<Action*> Bubby::getActionList()
{
  return action_list;
}

/* Gets the bubbies exp */
int Bubby::getExp()
{
  return exp;
}

/* Gets the bubbies level */
int Bubby::getLevel()
{
  return level;
}

/* Gets the bubbies tier */
int Bubby::getTier()
{
  return tier;
}

/* Sets the exp of the bubby based on use in battle */
void Bubby::setExperience(int exp)
{
  return experience;
}

/* Sets the level of the bubby based on exp amounts */
void Bubby::setLevel(int lvl)
{
  level = lvl;;
}

/* Sets the tier of the bubby based on leveling */
void Bubby::setTier(int tier_num)
{
  tier = tier_num;
}

