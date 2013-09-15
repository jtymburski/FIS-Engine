/******************************************************************************
* Class Name: AttributeSet
* Date Created: March 5th, 2013
* Inheritance: None
* Description: The attribute set is a container for all the statistics used
*              by race, category, person, etc. It holds vectors of their values,
*              maximum values, short names. The class also holds data for
*              AttributeSet presets.
******************************************************************************/
#include "Game/Player/AttributeSet.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
QList<QString> AttributeSet::short_names;      /* Vector of Stat names */
const uint AttributeSet::kDEFAULT_MIN =     3; /* Default min of a stat */
const uint AttributeSet::kDEFAULT_MAX = 20000; /* Default max of a stat */
const uint AttributeSet::kMIN_VALUE   =     1; /* Abs. min value of a stat */
const uint AttributeSet::kMAX_VALUE   = 99999; /* Abs. max value of a stat */

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs an attribute set (default min 0 stats and max kMAX)
 *
 * Inputs: none
 */
AttributeSet::AttributeSet()
{
  if (short_names.empty())
    setUpNames();

  QList<QString>::iterator iter;
  for (iter = short_names.begin(); iter < short_names.end(); ++iter)
    values.append(kDEFAULT_MIN);
}

/*
 * Description: Constructor: Constructs an attribute set given a vector of
 *              values. The constructor will assign values for each name, then
 *              will assign max values for each name, or use default values
 *              when needed. The constructor will only use the first size x 2
 *              values.
 *
 * Inputs: QVector<uint> stat_values - values of stats to be set.
 *         QWidget* parent - the parent of the object
 */
AttributeSet::AttributeSet(QList<uint> stat_values)
{
  if (short_names.empty())
    setUpNames();

  setAll(stat_values);
}

/*
 * Description: Annihilates an AttributeSet object
 */
AttributeSet::~AttributeSet() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Instantiates the short_names lsit on the first
 *              initialization of an AttributeSet
 * Note[1]: Static Function
 *
 * Inputs: none
 * Output: none
 */
void AttributeSet::setUpNames()
{
  short_names.clear();
  short_names << "VITA" << "QTDR" << "PHAG" << "PHFD" << "THAG" << "THFD"
              << "POAG" << "POFD" << "PRAG" << "PRFD" << "CHAG" << "CHFD"
              << "CYAG" << "CYFD" << "NIAG" << "NIFD" << "LIMB" << "MMTM"
              << "UNBR";
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Changes a givne AttributeStat given an enumerated stat to change
 *              and amount to change it by. Finds the index of the given
 *              enumerated type and calls overloaded changeStat(int, int) to
 *              minimize string compares.
 *
 * Inputs: Attribute type - the type of
 * Output: bool - true if the stat was found and changed, false otherwise
 */
bool AttributeSet::changeStat(EnumDb::Attribute type, int amount)
{
  int attr_index = getIndex(type);
  if (attr_index != -1)
    return changeStat(attr_index, amount);
  return false;
}

/*
 * Description: Changes a given stat name by a given value. Finds the index
 *              of the given QString and calls overloaded changeStat(int, int)
 *              to minimize string compares.
 *
 * Inputs: QString - the name or short name of the stat to be changed
 *         int     - the value by which to change the stat
 * Output: bool - true if the stat was found and changed, false otherwise
 */
bool AttributeSet::changeStat(QString name, int amount)
{
  int attr_index = getIndex(name);
  if (attr_index != -1)
    return changeStat(attr_index, amount);
  return false;
}

/*
 * Description: Changes a given stat by name (overloaded functon)
 *
 * Inputs: int index - index of the stat to be changed.
 *         int amount - amount by which to change the stat
 * Output: bool - true if the stat was found and changed, false otherwise
 */
bool AttributeSet::changeStat(int index, int amount)
{
  if (index < getSize() && index > 0)
  {
    if (amount < 0 && -amount >= (int)values.at(index))
      values[index] = 0;
    else if (amount > 0 && amount >= (int)kMAX_VALUE)
      values[index] = kMAX_VALUE;
    else
      values[index] += amount;
    return true;
  }
  return false;
}

/*
 * Description: Returns the value of a regular stat given an enumerated Attr.
 *
 * Inputs: Attribute type - enumerated attribute to be checked
 * Output: short - value of the normal value stat if found, -1 otherwise
 */
short AttributeSet::getStat(EnumDb::Attribute type)
{
  int attr_index = getIndex(type);
  if (attr_index != -1)
    return values.at(attr_index);
  return -1;
}

/*
 * Description: Gets the value of a stat of a given string
 *
 * Inputs: QString - name (or short namE) to be checked
 * Output: short - value of the stat at the given index, or -1 if not found
 */
short AttributeSet::getStat(QString name)
{
  if (getIndex(name) != -1)
    return values.at(getIndex(name));
  return -1;
}

/*
 * Description: Gets the stat at a given index
 *
 * Inputs: int - index of the stat to be checked
 * Output: short - value of the stat at the index, or -1 if not found
 */
short AttributeSet::getStat(int index)
{
  if (index >= 0 && index < values.size())
    return values.at(index);
  return -1;
}

/*
 * Description: Sets all values of stats given a vector of uints, and a bool
 *              whether you are starting from setting max stats or not.
 *
 * Inputs: QVector<uint> - vector of uints for stats to add.
 *         bool max - if true, start setting only max values.
 *
 * Output: none
 */
void AttributeSet::setAll(QList<uint> stat_values)
{
    values.clear();

    for (int i = 0; i < short_names.size(); i++)
    {
      if (stat_values.size() > i)
        values.append(stat_values.at(i));
    }
}

/*
 * Description: Sets the value of a regular stat given an Attribute enumerated
 *              type. Calls the overloaded setStat(int, uint) to minimize
 *              string compares.
 *
 * Inputs: Attribute type - the Attribute enumerated to be checked
 *         uint - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setStat(EnumDb::Attribute type, uint value)
{
  return (getIndex(type), value);
}

/*
 * Description: Sets the value of a regular stat given a string of the
 *              attribute. Calls the overloaded setStat(int, uint) to minimize
 *              string compares.
 *
 * Inputs: QString - name of the attribute to be set
 *         uint - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setStat(QString name, uint value)
{
  return (getIndex(name), value);
}

/*
 * Description: Sets the value of an AttributeSet of a given index
 *
 * Inputs: int - index of the Attribute to be checked
 *         uint - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setStat(int index, uint value)
{
  if (index != -1 && value < kMAX_VALUE)
  {
      values[index] = value;
    return true;
  }
  return false;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Converts an enumerated Attribute value to a QString using
 *              EnumString instead of string compares.
 *
 * Inputs: Attribute type - enumerated Attribute type to get the QString for
 * Output: QString - the QString value of the enumerated Attribute type
 */
QString AttributeSet::getAttrStr(EnumDb::Attribute type)
{
  const std::string &attribute_string = 
                                    EnumString<EnumDb::Attribute>::From(type);
  QString attribute_qstring(attribute_string.c_str());
  return attribute_qstring;
}

/*
 * Description: Converts a QString to an enumerated Attribute value using
 *              EnumString instead of string compare.
 *
 * Inputs: Qstring name - the string to be converted to an enumerated Attribute
 * Output: Attribute - the enumerated type corresponding to the QString given
 */
EnumDb::Attribute AttributeSet::getAttr(QString name)
{
  const std::string &attribute_string = name.toUtf8().constData();
  EnumDb::Attribute attribute_type = EnumDb::VITA;
  EnumString<EnumDb::Attribute>::To(attribute_type, attribute_string);
  return attribute_type;
}

/*
 * Description: Gets the index of a stat given an enumerated type
 *
 * Inputs: Attribute type - enumerated attribute to be checked
 * Output: int - the index of the enumerated if found, -1 otherwise
 */
int AttributeSet::getIndex(EnumDb::Attribute type)
{
  switch (type)
  {
    case EnumDb::VITA: return  0;
    case EnumDb::QTDR: return  1;
    case EnumDb::PHAG: return  2;
    case EnumDb::PHFD: return  3;
    case EnumDb::THAG: return  4;
    case EnumDb::THFD: return  5;
    case EnumDb::POAG: return  6;
    case EnumDb::POFD: return  7;
    case EnumDb::PRAG: return  8;
    case EnumDb::PRFD: return  9;
    case EnumDb::CHAG: return 10;
    case EnumDb::CHFD: return 11;
    case EnumDb::CYAG: return 12;
    case EnumDb::CYFD: return 13;
    case EnumDb::NIAG: return 14;
    case EnumDb::NIFD: return 15;
    case EnumDb::MMNT: return 16;
    case EnumDb::LIMB: return 17;
    case EnumDb::UNBR: return 18;
  }
  return -1;
}

/*
 * Description: Gets the index of a stat given a name
 *
 * Inputs: QString - name (or short name) to be checked
 * Output: int - the index of the stat if found in either name vector, or -1
 */
int AttributeSet::getIndex(QString name)
{
  for (int i = 0; i < getSize(); i++)
    if (name == short_names.at(i))
      return i;
  return -1;
}

/*
 * Description: Gets the long name of an attribute of a given Enumerated type.
 *
 * Inputs:
 * Output: QString - name of the attribute at the given index
 */
QString AttributeSet::getName(EnumDb::Attribute type)
{
  return getAttrStr(type);
}

/*
 * Description: Gets the name of an attribute at a given index
 *
 * Inputs: int - index of the names to be checked
 * Output: QString - name of the attribute at the given index
 */
QString AttributeSet::getName(int index)
{
  if (index < getSize())
      return short_names.at(index);
  return "";
}

/*
 * Description: Returns the size of the AttributeSet
 *
 * Inputs: none
 * Output: int - the number of attributes in short_name
 */
int AttributeSet::getSize()
{
  return short_names.size();
}

/*
 * Description: Prints out the stats (normal and max values) of the attribute
 *              set.
 *
 * Inputs: none
 * Output: none
 */
void AttributeSet::printInfo()
{
  qDebug() << " --- Attr Set ---";
  for (int i = 0; i < getSize(); i++)
    qDebug() <<  short_names.at(i) << " < " << getStat(i);
  qDebug() << " --- / Attr Set --- ";
}
