/*******************************************************************************
* Class Name: Bubby
* Date Created: November 4th, 2012 (Rewritten March 6th, 2013)
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, etc.
* // TODO: Bubby Attributes and LevelUp paths [03-09-13]
*******************************************************************************/
#include "Game/Player/Bubby.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
QVector<uint> Bubby::exp_table;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a Bubby object
 *
 * Inputs: BubbyFlavour* - pointer to the type of the Bubby
 */
Bubby::Bubby(BubbyFlavour* type) : Item(type->getName())
{
  /* Build the experience table the first time a Bubby is created */
  if (exp_table.isEmpty())
    calcExpTable();

  /* Basic initialization */
  setName(type->getName());
  setType(type);

  if (!type->getSprites().isEmpty())
    setThumb(type->getSprites().at(0));

  setValue(0); // TODO: Bubby values;
  setMass(mass);
  setLevel(0);
  setTier(0);
  setExperience(0);
  setSprite();
}

/*
 * Description: Destroy a Bubby object
 */
Bubby::~Bubby() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Calculates and builds the experience table for a person
 *
 * Inputs: none
 * Output: none
 */
void Bubby::calcExpTable()
{
  double b = log((double)kMAX_LVL_EXP / kMIN_LVL_EXP) / (kLEVEL_CAP - 1);
  double a = (double)kMIN_LVL_EXP / (exp(b) - 1.0);
  for (uint i = 1; i <= kLEVEL_CAP + 1; i++)
  {
    int old_exp = round(a * exp(b * (i - 1)));
    int new_exp = round(a * exp(b * i));
    exp_table.push_back(new_exp - old_exp);
  }
}

/*
 * Description: Sets the pointer to the current sprite for the Bubby
 *              corresponding to its current tier (if one exists), else sets
 *              it to the highest tier sprite it can (if one exists), else
 *              sets the pointer to 0.
 *
 * Inputs: none
 * Output: bool - true if a sprite was actually set
 */
bool Bubby::setSprite()
{
  int size = getType()->getSprites().size();
  if (size >= getTier())
  {
    current_sprite = getType()->getSprites().at(getTier() - 1);
    return true;
  }
  else if (!getType()->getSprites().isEmpty())
  {
    current_sprite = getType()->getSprites().at(size - 1);
    return true;
  }
  current_sprite = 0;
  return false;
}


/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds an amount to the experience of the bubby and calls
 *              setLevel() if necessary to deal with level ups accordingly
 *              Will deal with multiple level ups.
 *
 * Inputs: value - amount of experience to be added
 * Output: none
 */
void Bubby::addExperience(uint value)
{
  if (total_exp + value > kMAX_EXPERIENCE)
    total_exp = kMAX_EXPERIENCE;
  else
    total_exp += value;

  /* Level the bubby to the proper value (if necessary) */
  while (getLevel() < kLEVEL_CAP && total_exp >= getExpAt(getLevel() + 1))
    setLevel(getLevel() + 1);
}

/*
 * Description: Prints all the info pertaining to the Bubby
 *
 * Inputs: none
 * Outputs: none
 */
void Bubby::printInfo()
{
  qDebug() << "Bubby ID #: " << my_id;
  qDebug() << "Total Exp: " << total_exp;
  qDebug() << "Level: " << level;
  qDebug() << "Tier: " << tier;
  qDebug() << "Name: " << name;
  qDebug() << "Description: " << description;
  qDebug() << "Duration: " << duration;
  qDebug() << "Value: " << value;
  qDebug() << "Mass: " << mass;
}

/*
 * Description: Returns the value of the experience table at a given point
 *
 * Inputs: ushort - level to look up on the table
 * Output: uint   - total experience required for given level
 */
uint Bubby::getExpAt(ushort level)
{
  if (level < exp_table.size())
    return exp_table.at(level);
  return 0;
}

/*
 * Description: Returns the experience of the Bubby
 *
 * Inputs: none
 * Output: uint - experience value of the Bubby
 */
uint Bubby::getExp()
{
  return total_exp;
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
 * Description: Returns the current sprite for the tier of the Bubby.
 *
 * Inputs: none
 * Output: Sprite* - pointer to the current sprite of the Bubby (for its tier)
 */
Sprite* Bubby::getSprite()
{
  return current_sprite;
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

/*
 * Description: Returns the flavour of the Bubby
 *
 * Inputs: none
 * Output: BubbyFlavour - the flavour of the Bubby
 */
BubbyFlavour* Bubby::getType()
{
  return type;
}

/*
 * Description: Sets the experience of the Bubby
 *
 * Inputs: uint - new value for experience of the Bubby
 * Output: none
 */
void Bubby::setExperience(uint new_experience)
{
  total_exp = new_experience;
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

/*
 * Description: Sets the type (flavour) of the Bubby
 *
 * Inputs: BubbyFlavour* - pointer to the new flavour of the Bubby
 * Output: none
 */
void Bubby::setType(BubbyFlavour* new_type)
{
  type = new_type;
}
