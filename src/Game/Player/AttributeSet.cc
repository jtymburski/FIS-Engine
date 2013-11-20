/*******************************************************************************
* Class Name: AttributeSet [Implementation]
* Date Created: November 16th, 2013
* Inheritance: None
* Description: AttributeSet is a container for all the statistics (variables)
*              which are used by other classes to calculate the outcome of
*              combat states.
*
* Notes
* -----
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

const std::vector<uint> AttributeSet::kPRESET3 =
  {1000, 200, 75, 45,
   60, 40, 60, 40,
   80, 75, 80, 75,
   60, 40, 60, 40,
   40, 50, 25, 5};

const uint AttributeSet::kDEFAULT     =        0;
const uint AttributeSet::kNUM_VALUES  =       20;
const uint AttributeSet::kMIN_VALUE   =        0;
const uint AttributeSet::kMAX_VALUE   =    99999; 
const uint AttributeSet::kNUM_PRESETS =        3;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default constructor, intitializes a default Attribute set
 *
 * Inputs: none
 */
 AttributeSet::AttributeSet()
{
  buildAsPreset(0);

  /* Assert the values are within range */
  cleanUp();
}

/*
 * Description: Construct an AS based on presets which are pre-initialized
 *              and configured values of stats.
 *
 * Inputs: const int &preset_level - const ref to the preset to set
 */
AttributeSet::AttributeSet(const int &preset_level)
{
  buildAsPreset(preset_level);

  /* Assert the values are within range */
  cleanUp();
}

/*
 * Description: Copy constructor
 *
 * Inputs: const Attribute &source - const ref to the source object
 */
AttributeSet::AttributeSet(const AttributeSet &source)
{
  /* Common copy function between assignment operator */
  copySelf(source);
}

/*
 * Description: Normal constructor: constructs an AttributeSet given a
 *              std::vector<uint> of values to be assigned. This
 *              vector must be of size kNUM_VALUES or the values will
 *              be set to default.
 *
 * Inputs: std::vector<uint> new_values - vector of values to be set
 */
AttributeSet::AttributeSet(const std::vector<uint> &new_values)
{
  if (new_values.size() == kNUM_VALUES)
    values = new_values;
  else
    for (uint i = 0; i < kNUM_VALUES; i++)
      values.push_back(kDEFAULT);

  cleanUp();
}

/*
 * Description: Annihilates an AttributeSet object
 */
AttributeSet::~AttributeSet() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Constructs an AttributeSet based on presets which are 
 *              pre-initialized and configured values of stats.
 *
 * Inputs: const uint &level - const ref to which preset to use
 * Output: none
 */
void AttributeSet::buildAsPreset(const uint &level)
{
  std::vector<uint> default_values(kNUM_VALUES, kDEFAULT);

  if (level == 0 || level > kNUM_PRESETS)
    values = default_values;
  else if (level == 1)
    values = kPRESET1;
  else if (level == 2)
    values = kPRESET2;
  else if (level == 3)
    values = kPRESET3;
}

/*
 * Description: Common function for copy constructor and assignment operator.
 *              Coppies the current object from the source's values.
 *
 * Inputs: const Attribute &source - const ref of the object to be copied
 * Output: none
 */
void AttributeSet::copySelf(const AttributeSet &source)
{
  values = source.values;
}

/*
 * Description: Assigns all the values (statS) contained in the AttributeSet to
 *              values within kMIN_VALUE and kMAX_VALUE.
 *
 * Inputs: none
 * Output: none
 */
void AttributeSet::cleanUp()
{
  for (auto it = values.begin(); it != values.end(); ++it)
    Helpers::setWithinRange((*it), kMIN_VALUE, kMAX_VALUE);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Alters the stat at a given index by an amount
 *
 * Inputs: const uint &index - the index of the stat to be altered
 *         const int &amount - the amount by which to alter the stat (+/-)
 * Output: bool - true if the index was a valid stat
 */
bool AttributeSet::alterStat(const uint &index, const int &amount)
{
  if (static_cast<int>(index) != -1 && index < kNUM_VALUES)
  {
    auto temp = static_cast<int>(values[index]) + amount;

    if (temp < 0)
      temp = 0;

    Helpers::setWithinRange(temp, kMIN_VALUE, kMAX_VALUE);
    values[index] = temp;

    return true;
  }

  return false;
}

/*
 * Description: Alters the stat of a given enumerated value by an amount.
 *              Calls the alterStat(uint index, int amount) function.
 *
 * Inputs: const Attribute &stat - enumerated Attribute value to be altered
 *         const int &amount - the amount by which to alter the stat (+/-) 
 * Output: bool - true if the enumeration was a valid attribute
 */
bool AttributeSet::alterStat(const Attribute &stat, const int &amount)
{
  return alterStat(static_cast<uint>(stat), amount);
}

/*
 * Description: Alters the stat of a given name (short or long name) by an
 *              amount. Uses the alterStat function above after obtaining 
 *              the right index.
 *
 * Inputs: const std::string &name - the name form of the attribute
 *         const int &amount - the amount by which to alter the stat 
 * Output: bool - true if the name relates to a valid stat
 */
bool AttributeSet::alterStat(const std::string &name, const int &amount)
{
  return alterStat(getIndex(name), amount);
}

/*
 * Description: Returns the index of a given enumerated Attribute, or
 *              returns -1 if the index was not found.
 *
 * Inputs: const Attribute &stat - enumerated attribute to find the index for
 * Output: int - the index of the corresponding attribute (if it exists)
 */
int AttributeSet::getIndex(const Attribute &stat)
{
  if (stat < Attribute::NONE)
    return static_cast<uint>(stat);

  return -1;
}

/*
 * Description: Returns the long form name version of an attribute given
 *              an enumerated attribute. If not found, returns the empty string.
 *
 * Inputs: const Attribute &stat - the enum. stat to find the name for
 * Output: std::string - the long version of the attributes name (if it exists)
 */
std::string AttributeSet::getLongName(const Attribute &stat)
{
  auto index = getIndex(stat);

  if (index != -1)
    return kLONG_NAMES[index];

  return "";
}

/*
 * Description: Returns the short form name version of an attribute given
 *              an enumerated attribute. If not found, returns the empty string.
 *
 * Inputs: const Attribute &stat - the enumerated attribute to find the name for
 * Output: std::string - the short version of the attributes name (if it exists)
 */
std::string AttributeSet::getName(const Attribute &stat)
{
  auto index = getIndex(stat);

  if (index != -1)
    return kSHORT_NAMES[index];

  return "";
}

/*
 * Description: Returns the value of a stat at a given index, if the index
 *              is within range, else it returns -1 (an impossible value)
 *
 * Inputs: const uint &index - const ref. to the index to be retrieved (def. 0)
 * Output: int - the value of the stat at the index, else -1
 */
int AttributeSet::getStat(const uint &index)
{
  if (index < values.size())
    return values[index];

  return -1;
}

/*
 * Description: Returns the value of a stat represented by a given enumerated
 *              attribute, or if not a valid attribute, returns -1 (impossible
 *              value for a stat)
 *
 * Inputs: const Attribute &stat - enumerated value representing an attribute
 * Output: int - the value of the stat corresponding to the enum value, or -1
 */
int AttributeSet::getStat(const Attribute &stat)
{
  auto index = getIndex(stat);

  if (index != -1)
    return getStat(index);

  return -1;
}

/*
 * Description: Returns the value of a stat given the name (short or long form)
 *              of a stat.
 *
 * Inputs: const std::string &name - name of stat to check value for
 * Output: int - the value of the stat if it exists, else -1
 */
int AttributeSet::getStat(const std::string &name)
{
  auto index = getIndex(name);

  if (index != -1)
    return getStat(index);

  return -1;
}

/*
 * Description: Attempts to assign a stat at a given index to a value.
 *              Returns the truth of the assignment.
 *
 * Inputs: const uint &index - the index of the stat to be assigned
 *         const int &value - the value to assign the stat to
 * Output: bool - the truth of the assignment
 */
bool AttributeSet::setStat(const uint &index, const int &value)
{
  if (index < kNUM_VALUES)
  {
    Helpers::setWithinRange(value, kMIN_VALUE, kMAX_VALUE);
    values[index] = value;
    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a stat of a given enum value to a value.
 *              Returns the truth of the assignment.
 *
 * Inputs: Attribute stat - enum. value of the stat to be assigned
 *         const int &value - the value to assign to the stat
 * Output: bool - the truth of the assignment
 */
bool AttributeSet::setStat(const Attribute &stat, int value)
{
  auto index = getIndex(stat);

  if (index != -1)
  {
    return setStat(index, value);
    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a stat of a given name (long or short form)
 *              to a given value. Returns the truth of the assignment.
 *
 * Inputs: const std::string &name - the name of the stat to be assigned
 *         const int &value - the value to assign to the stat
 * Output: bool - the truth of the assignment
 */
bool AttributeSet::setStat(const std::string &name, const int &value)
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

/*
 * Description: Returns the index of an attribute given a name (short or long)
 *              form representing the attribute.
 *
 * Inputs: const std::string &name - the name of the attribute to find index of
 * Output: int - the index of the corresponding attribute (if it exists)
 */
int AttributeSet::getIndex(const std::string &name)
{
  /* Find the index in the vector of short or long names */
  for (uint i = 0; i < kNUM_VALUES; i++)
    if (kSHORT_NAMES[i] == name || kLONG_NAMES[i] == name)
      return i;

  /* Else return an invalid index */
  return -1; 
}

/*
 * Description: Returns the size of an attribute set (the value of kNUM_VALUES)
 *
 * Inputs: none
 * Output: uint - the number of values contained in an attribute set
 */
uint AttributeSet::getSize()
{
  return kNUM_VALUES;
}

/*
 * Description: Returns the long-form name of an attribute at a given index.
 *
 * Inputs: const uint &index - the index to return the long-form name of
 * Output: std::string - the string stored in kLONG_NAMES[index]
 */
std::string AttributeSet::getLongName(const uint &index)
{
  if (index < kNUM_VALUES)
    return kLONG_NAMES[index];

  return "";
}

/*
 * Description: Returns the short-form name of an attribute at a given index
 *
 * Inputs: const uint &index - the index to return the short-form name of
 * Output: std::string - the string stored in kLONG_NAMES[index]
 */
std::string AttributeSet::getName(const uint &index)
{
  if (index < kNUM_VALUES)
    return kSHORT_NAMES[index];

  return "";
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

/*
 * Description: Overloaded assignment operator. Checks for self-assignment, then
 *              calls copySelf on the given object as the common copying
 *              method between this function and the copy constructor.
 *
 * Inputs: const AttributeSet& source - the source object to be copied
 * Output: AttributeSet& - ref to the copied object
 */
AttributeSet& AttributeSet::operator= (const AttributeSet &source)
{
  /* Check for self assignment */
  if (this == &source)
    return *this;

  copySelf(source);
  
  /* Return the copied object by ref */
  return *this;
}

/*
 * Description: Overloaded compound assignment operator. Adds a given 
 *              AttributeSet (rhs) to the current object and returns itself.
 *
 * Inputs: const AttributeSet& rhs - the set to be added to the current set
 * Output: AttributeSet& - ref to the current object (after compounding)
 */
AttributeSet& AttributeSet::operator+=(const AttributeSet& rhs)
{
  for (uint i = 0; i < kNUM_VALUES; i++)
    this->values[i] = this->values[i] + rhs.values[i];

  /* Assert the new values are within range */
  this->cleanUp();

  /* Return the result by ref */
  return *this;
}

/*
 * Description: Overloaded subtractive assignment operator. Subtracts a given
 *              AttributeSet (rhs) from the current object and returns itself
.e *
 * Inputs: const AttributeSet& rhs - the set to be subtracted from the current
 * Output: AttributeSet& - ref to the current object (after subtraction)
 */
AttributeSet& AttributeSet::operator-=(const AttributeSet& rhs)
{
  for (uint i = 0; i < kNUM_VALUES; i++)
    this->values[i] = this->values[i] - rhs.values[i];

  /* Assert the new values are within range */
  this->cleanUp();

  /* Return the result by ref */
  return *this;
}

/*
 * Description: Overloaded + operator. Adds two given Attribute sets together
 *              and returns the added set (re-uses the compound assignment)
 *
 * Inputs: AttributeSet lhs - the left expression AttributeSet
 *         const AttributeSet &rhs - the right side expression AttributeSet
 * Output: AttributeSet - the new object after addition
 */
inline AttributeSet operator+(AttributeSet lhs, const AttributeSet &rhs)
{
  /* Reuse compound addition operator */
  lhs += rhs;

  return lhs;
}

/*
 * Description: Overlaoded subtraction operator. Subtracts the rhs AS from the
 *              lhs AS and returns a ref to the rhs object after.
 *
 * Inputs: AttributeSet lhs - the left expression AS
 *         const AttributeSet& rhs - the right expression AS
 * Output: AttributeSet - the left hand expression after subtration
 */
inline AttributeSet operator-(AttributeSet lhs, const AttributeSet& rhs)
{
  /* Reuse the compound subtraction operator */
  lhs -= rhs;

  return lhs;
}