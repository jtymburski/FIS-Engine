/*******************************************************************************
* Class Name: AttributeSet [Implementation]
* Date Created: November 16th, 2013
* Inheritance: None
* Description: AttributeSet is a container for all the statistics (variables)
*              which are used by other classes to calculate the outcome of
*              combat states.
*
* Notes:
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/AttributeSet.h"

/*=============================================================================
 * CONSTANTS - See implementation for descriptions
 *============================================================================*/

const std::vector<std::string> AttributeSet::kSHORT_NAMES =
{
  "VITA", "QTDR", "PHAG", "PHFD", 
  "THAG", "THFD", "POAG", "POFD", 
  "PIAG", "PIFD", "CHAG", "CHFD", 
  "CYAG", "CYFD", "NIAG", "NIFD", 
  "LIMB", "MMTM", "UNBR", "MANN"
};

const std::vector<std::string> AttributeSet::kLONG_NAMES =
{
  "VITALITY", 
  "QUANTUM DRIVE", 
  "PHYSICIAL AGGRESSION",
  "PHYSICIAL FORTITUDE",
  "THERMAL AGGRESSION", 
  "THERMAL FORTITUDE", 
  "POLAR AGGRESSION", 
  "POLAR FORTITUDE", 
  "PRIMAL AGGRESSION", 
  "PRIMAL FORTITUDE", 
  "CHARGED AGGRESSION", 
  "CHARGED FORTITUDE",
  "CYBERNETIC AGGRESSION", 
  "CYBERNETIC FORTITUDE", 
  "NIHIL AGGRESSION", 
  "NIHIL FORTITUDE",
  "LIMBERTUDE", 
  "MOMENTUM", 
  "UNBEARABILITY", 
  "MANNA"
};

const std::vector<uint> AttributeSet::kPRESET1 =
  {100, 30, 15, 10, 
    15, 10, 15, 10, 
    25, 15, 15, 10, 
    15, 10, 15, 10, 
    10, 10, 5, 1};

const std::vector<uint> AttributeSet::kPRESET2 =
  {200, 55, 35, 25,
   20, 14, 20, 14,
   35, 28, 20, 10,
   18, 7, 18, 7,
   20, 20, 10, 1 };

const uint AttributeSet::kDEFAULT     =        0;
const uint AttributeSet::kNUM_VALUES  =       20;
const uint AttributeSet::kMIN_VALUE   =        0;
const uint AttributeSet::kMAX_VALUE   =    99999; 
const uint AttributeSet::kNUM_PRESETS =        3;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
AttributeSet::AttributeSet(const int &preset_level)
{
  buildAsPreset(preset_level);
  cleanUp();
}

/*
 * Description:
 *
 * Inputs:
 */
AttributeSet::AttributeSet(const AttributeSet &source)
{
  copySelf(source);
}

/*
 * Description:
 *
 * Inputs:
 */
AttributeSet::AttributeSet(std::vector<uint> new_values)
{
  if (new_values.size() == kNUM_VALUES)
    values = new_values;
  else
    for (uint i = 0; i < kNUM_VALUES; i++)
      values.push_back(kDEFAULT);

  cleanUp();
}

/*
 * Description:
 *
 */
AttributeSet::~AttributeSet() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void AttributeSet::buildAsPreset(const uint &level)
{
  std::vector<uint> default_values(kNUM_VALUES, kDEFAULT);

  if (level == 0 || kNUM_PRESETS <= level)
    values = default_values;
 else if (level == 1)
    values = kPRESET1;
 else if (level == 2)
    values = kPRESET2;
}

void AttributeSet::copySelf(const AttributeSet &source)
{
  values = source.values;
}

void AttributeSet::cleanUp()
{
  for (auto it = values.begin(); it != values.end(); ++it)
    setWithinRange((*it), kMIN_VALUE, kMAX_VALUE);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool AttributeSet::alterStat(uint index, int amount)
{
  if ((int)index != -1 && index < kNUM_VALUES)
  {
    values[index] += amount;
    setWithinRange(values[index], kMIN_VALUE, kMAX_VALUE);
    return true;
  }

  return false;
}

bool AttributeSet::alterStat(Attribute stat, int amount)
{
  return alterStat(static_cast<uint>(stat), amount);
}

bool AttributeSet::alterStat(std::string name, int amount)
{
  return alterStat(getIndex(name), amount);
}

int AttributeSet::getIndex(Attribute stat)
{
  if (stat < Attribute::NONE)
    return static_cast<uint>(stat);

  return -1;
}

int AttributeSet::getIndex(std::string name)
{
  /* Find the index in the vector of short or long names */
  for (uint i = 0; i < kNUM_VALUES; i++)
    if (kSHORT_NAMES[i] == name || kLONG_NAMES[i] == name)
      return i;

  /* Else return an invalid index */
  return -1; 
}

int AttributeSet::getStat(uint index)
{
  if (index < values.size())
    return values[index];

  return -1;
}

int AttributeSet::getStat(Attribute stat)
{
  auto index = getIndex(stat);

  if (index != -1)
    return getStat(index);

  return -1;
}

int AttributeSet::getStat(std::string name)
{
  auto index = getIndex(name);

  if (index != -1)
    return getStat(index);

  return -1;
}

bool AttributeSet::setStat(uint index, int value)
{
  if (index < kNUM_VALUES)
  {
    setWithinRange(value, kMIN_VALUE, kMAX_VALUE);
    values[index] = value;
    return true;
  }

  return false;
}

bool AttributeSet::setStat(Attribute stat, int value)
{
  auto index = getIndex(stat);

  if (index != -1)
  {
    return setStat(index, value);
    return true;
  }

  return false;
}

bool AttributeSet::setStat(std::string name, int value)
{
  auto index = getIndex(name);

  if (index != -1)
  {
    return setStat(index, value);
    return false;
  }

  return false;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

uint AttributeSet::getSize()
{
  return kNUM_VALUES;
}

std::string AttributeSet::getName(uint index)
{
  if (index < kNUM_VALUES)
    return kSHORT_NAMES[index];

  return "";
}

std::string AttributeSet::getName(Attribute stat)
{
  auto index = getIndex(stat);

  if (index != -1)
    return kSHORT_NAMES[index];

  return "";
}

std::string AttributeSet::getLongName(uint index)
{
  if (index < kNUM_VALUES)
    return kLONG_NAMES[index];

  return "";
}

std::string AttributeSet::getLongName(Attribute stat)
{
  auto index = getIndex(stat);

  if (index != -1)
    return kLONG_NAMES[index];

  return "";
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

/*
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
AttributeSet& AttributeSet::operator= (const AttributeSet &source)
{
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy and return the copied object */
  copySelf(source);
  return *this;
}

/*
AttributeSet& operator+=(const AttributeSet& rhs)
{
  for (int i = 0; i < kNUM_VALUES; i++)
    this->values[i] = this.values[i] + rhs.values[i];

  return *this;
}

inline AttributeSet& AttributeSet::operator+ (AttributeSet lhs, 
                                              const AttributeSet &rhs)
{
  lhs += rhs;

  return lhs;
}
*/