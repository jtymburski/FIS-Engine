/*******************************************************************************
* Class Name: Bubby
* Date Created: November 4th, 2012 (Rewritten March 6th, 2013)
* Inheritance: Item
* Description: The Bubby specification under Item that adds the extra
*              details to define experience, level cap, etc.
*******************************************************************************/
#include "Game/Player/Bubby.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
QVector<int> Bubby::exp_table;

const ushort Bubby::kTIER1_LVL    =       9;
const ushort Bubby::kTIER2_LVL    =      19;
const ushort Bubby::kLEVEL_CAP    =      20;
const uint Bubby::kMIN_LVL_EXP    =      75;
const uint Bubby::kMAX_LVL_EXP    =  450000;
const uint Bubby::kMAX_EXP        = 1000000; /* 10^6 */
const double Bubby::kBASE_VALUE       =    3.14;
const double Bubby::kVALUE_PER_LEVEL  =   20.00;
const double Bubby::kVALUE_PER_TIER   =  200.00;
const double Bubby::kVALUE_MULTIPLIER =   1.100;
const double Bubby::kTIER_1_MASS      =   0.500;
const double Bubby::kTIER_2_MASS      =   2.000;
const double Bubby::kTIER_3_MASS      =   4.500;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS [PRIVATE CONSTRUCTOR]
 *============================================================================*/

/*
 * Description: Constructs a Bubby object
 *
 * Inputs: BubbyFlavour* - pointer to the type of the Bubby
 */
Bubby::Bubby(BubbyFlavour* type)
    : Item(type->getName(), 0, 0, kTIER_1_MASS),
    type(type),
    level(0),
    tier(0),
    total_exp(0)
{
  /* Build the experience table the first time a Bubby is created */
  if (exp_table.isEmpty())
    exp_table = buildExponentialTable(kMIN_LVL_EXP, kMAX_LVL_EXP, kLEVEL_CAP);

  updateTierSprite();
  calcNewValue();
}

/*
 * Description: Destroy a Bubby object
 */
Bubby::~Bubby() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Calculate and set a new value for the Bubby on level up. The
 * value of a Bubby is based upon its level.
 *
 * Inputs: none
 * Output: none
 */
void Bubby::calcNewValue()
{
  long double temp_value   = kBASE_VALUE;
  long double level_amount = kVALUE_PER_LEVEL;
  long double tier_amount  = kVALUE_PER_TIER;

  /* Add the Level value to the base value */
  for (int i = 0; i < getLevel(); i++)
  {
    temp_value += level_amount;
    level_amount *= kVALUE_MULTIPLIER;
  }

  /* Then add the Tier value */
  for (int i = 1; i < getTier(); i++)
  {
    temp_value += tier_amount;
    tier_amount *= kVALUE_MULTIPLIER;
  }

  if (getLevel() == kLEVEL_CAP)
    temp_value += kVALUE_PER_TIER;

  setValue(floor(temp_value));
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
bool Bubby::updateTierSprite()
{
  bool updated = false;

  // int size = getType()->getSprites().size();

  /*
  if (size >= getTier())
  {
    setThumb(getType()->getSprites().at(getTier() - 1));
    updated = true;
  }
  else if (!getType()->getSprites().isEmpty())
  {
    setThumb(getType()->getSprites().at(size - 1));
    updated = true;
  }*/

  return updated;
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
  if (total_exp + value > kMAX_EXP)
    total_exp = kMAX_EXP;
  else
    total_exp += value;

  /* Level the bubby to the proper value (if necessary) */
  while (getLevel() < kLEVEL_CAP && total_exp >= getExpAt(getLevel() + 1))
  {
    setLevel(getLevel() + 1);
    calcNewValue();
  }
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
  if (new_experience <= kMAX_EXP)
    total_exp = new_experience;
  else
    total_exp = kMAX_EXP;

  if (new_experience == kMAX_EXP)
      emit maxExp();
}

/*
 * Description: Sets the level of the Bubby
 *
 * Inputs: uint - new value for level of the Bubby
 * Output: none
 */
void Bubby::setLevel(ushort new_level)
{
  bool level_up  = false;
  bool tier_up   = false;

  if (getTier() == 0)
  {
    if (new_level < kTIER1_LVL)
      level_up = true;
    if (new_level + 1 == kTIER1_LVL)
      tier_up = true;
  }

  if (getTier() == 1)
  {
    if (new_level < kTIER2_LVL)
      level_up = true;
    if (new_level + 1 == kTIER2_LVL)
      tier_up = true;
  }

  if (getTier() == 2)
  {
    if (new_level < kLEVEL_CAP)
      level_up = true;
    if (new_level + 1 == kLEVEL_CAP)
      tier_up   = true;
  }

  if (level_up)
  {
    level = new_level;
    emit levelUp();
    calcNewValue();
  }
  if (level_up && tier_up)
    emit maxLevelForTier();
}

/*
 * Description: Sets the tier of the Bubby
 *
 * Inputs: uint - new value of Tier of the Bubby
 * Output: none
 */
bool Bubby::setTier(ushort new_tier)
{
  bool can_tier_up = true;

  /* Conditional checks */
  if (getTier() == 1 && getLevel() != kTIER1_LVL)
    can_tier_up = false;
  if (getTier() == 2 && getLevel() != kTIER2_LVL)
    can_tier_up = false;
  if (getTier() >= BubbyFlavour::kTIER_CAP)
    can_tier_up = false;

  /* Commence necessary tier-up functions */
  if (can_tier_up)
  {
    tier = new_tier;
    emit tierUp();
    updateTierSprite();
  }

  return can_tier_up;
}
