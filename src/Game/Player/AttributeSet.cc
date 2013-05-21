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
QVector<QString> AttributeSet::short_names;      /* Vector of Stat names */
const ushort AttributeSet::kDEFAULT_MIN =     0; /* Default min of a stat */
const ushort AttributeSet::kDEFAULT_MAX = 20000; /* Default max of a stat */
const ushort AttributeSet::kMAX_VALUE   = 50000; /* Abs. max value of a stat */

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

  for (int i = 0; i < short_names.size(); i++)
  {
      values.append(kDEFAULT_MIN);
      max_values.append(kDEFAULT_MAX);
  }

  values.squeeze();
  max_values.squeeze();
}

/*
 * Description: Constructor: Constructs an attribute set given a vector of
 *              values. The constructor will assign values for each name, then
 *              will assign max values for each name, or use default values
 *              when needed. The constructor will only use the first size x 2
 *              values.
 *
 * Inputs: QVector<ushort> stat_values - values of stats to be set.
 *         QWidget* parent - the parent of the object
 */
AttributeSet::AttributeSet(QVector<ushort> stat_values)
{
  if (short_names.empty())
    setUpNames();

  setAll(stat_values);

 values.squeeze();
 max_values.squeeze();
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
 *
 * Inputs: none
 * Output: none
 */
void AttributeSet::setUpNames()
{
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
bool AttributeSet::changeStat(Attribute type, int amount)
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
    if (amount < 0 && -amount >= values.at(index))
      values[index] = 0;
    else if (amount > 0 && amount >= max_values.at(index))
      values[index] = max_values.at(index);
    else
      values[index] += amount;
    return true;
  }
  return false;
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
 * Description: Returns the maximum value of a given enumerated Attribute type
 *
 * Inputs: Attribute type - enumerated attribute to be checked for
 * Output: shot - +value of the maximum stat (if exists), -1 otherwise
 */
short AttributeSet::getMax(Attribute type)
{
  int attr_index = getIndex(type);
  if (attr_index != -1)
    return max_values.at(attr_index);
  return -1;
}

/*
 * Description: Returns the maximum value of a stat of a given name
 *
 * Inputs: QString - name of the stat to be checked
 * Output: short - +value of the maximum stat (if exists), -1 otherwise
 */
short AttributeSet::getMax(QString name)
{
  if (getIndex(name) != -1)
    return max_values.at(getIndex(name));
  return -1;
}

/*
 * Description: Returns the maximum value of a stat at a given index
 *
 * Inputs: int - index of the short names to be checked
 * Output: shot - +value of the maximum stat (if exists), -1 otherwise
 */
short AttributeSet::getMax(int index)
{
  if (index > 0 && index < values.size())
    return max_values.at(index);
  return -1;
}

/*
 * Description: Returns the value of a regular stat given an enumerated Attr.
 *
 * Inputs: Attribute type - enumerated attribute to be checked
 * Output: short - value of the normal value stat if found, -1 otherwise
 */
short AttributeSet::getStat(Attribute type)
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
  if (index > 0 && index < values.size())
    return values.at(index);
  return -1;
}

/*
 * Description: Sets all values of stats given a vector of ushorts, and a bool
 *              whether you are starting from setting max stats or not.
 *
 * Inputs: QVector<ushort> - vector of ushorts for stats to add.
 *         bool max - if true, start setting only max values.
 *
 * Output: none
 */
void AttributeSet::setAll(QVector<ushort> stat_values, bool max)
{
  if (!max)
  {
    int index = 0;
    while (values.size() < short_names.size())
    {
      if (values.size() < stat_values.size())
        values.append(stat_values.at(index));
      else
        values.append(kDEFAULT_MIN);
      index++;
    }

    while (max_values.size() < short_names.size())
    {
      if (max_values.size() < stat_values.size() + values.size())
        max_values.append(stat_values.at(index));
      else
        max_values.append(kDEFAULT_MAX);
      index++;
    }
  }
  else
  {
    int index = 0;
    while (max_values.size() < short_names.size())
    {
      if (max_values.size() < stat_values.size())
        max_values.append(stat_values.at(index));
      else
        max_values.append(kDEFAULT_MAX);
      index++;
    }
  }
}

/*
 * Description: Sets all the stats at once given 19 ushorts, and a bool
 *              whether you are setting the max stats or regular
 *
 * Inputs: ushort a through s - shorts describing the attributes
 *         bool max - whether setting the max or normal stats (default false)
 * Output: none
 */
void AttributeSet::setAll(ushort a, ushort b, ushort c, ushort d, ushort e,
                          ushort f, ushort g, ushort h, ushort i, ushort j,
                          ushort k, ushort l, ushort m, ushort n, ushort o,
                          ushort p, ushort q, ushort r, ushort s, bool max)
{
 if (!max)
 {
   setStat(0, a);
   setStat(1, b);
   setStat(2, c);
   setStat(3, d);
   setStat(4, e);
   setStat(5, f);
   setStat(6, g);
   setStat(7, h);
   setStat(8, i);
   setStat(9, j);
   setStat(10, k);
   setStat(11, l);
   setStat(12, m);
   setStat(13, n);
   setStat(14, o);
   setStat(15, p);
   setStat(16, q);
   setStat(17, r);
   setStat(18, s);
 }
 else
 {
   setMax(0, a);
   setMax(1, b);
   setMax(2, c);
   setMax(3, d);
   setMax(4, e);
   setMax(5, f);
   setMax(6, g);
   setMax(7, h);
   setMax(8, i);
   setMax(9, j);
   setMax(10, k);
   setMax(11, l);
   setMax(12, m);
   setMax(13, n);
   setMax(14, o);
   setMax(15, p);
   setMax(16, q);
   setMax(17, r);
   setMax(18, s);
 }
}

/*
 * Description: Sets the maximum value of an Attribute of a enumerated type.
 *?             Calls the overloaded setMax(int, ushort) to min. string compares
 *
 * Inputs: Attribute type - type of attribute to be set
 *         ushort - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setMax(Attribute type, ushort value)
{
  return setMax(getIndex(type), value);
}

/*
 * Description: Sets the maximum value of an Attribute of a given name. Calls
 *              the overloaded setMax(int, ushort) to minimize string compares.
 *
 * Inputs: QString - name of the attribute to be set
 *         ushort - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setMax(QString name, ushort value)
{
  return setMax(getIndex(name), value);
}

/*
 * Description: Sets the maximum value of an Attribute of a given index
 *
 * Inputs: int - index of the stat to be set
 *         ushort - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setMax(int index, ushort value)
{
  if (index != -1)
  {
    if (value <= kMAX_VALUE && value >= values.at(index))
      max_values[index] = value;
    else
      max_values[index] = kMAX_VALUE;
    return true;
  }
  return false;
}

/*
 * Description: Sets the value of a regular stat given an Attribute enumerated
 *              type. Calls the overloaded setStat(int, ushort) to minimize
 *              string compares.
 *
 * Inputs: Attribute type - the Attribute enumerated to be checked
 *         ushort - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setStat(Attribute type, ushort value)
{
  return (getIndex(type), value);
}

/*
 * Description: Sets the value of a regular stat given a string of the
 *              attribute. Calls the overloaded setStat(int, ushort) to minimize
 *              string compares.
 *
 * Inputs: QString - name of the attribute to be set
 *         ushort - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setStat(QString name, ushort value)
{
  return (getIndex(name), value);
}

/*
 * Description: Sets the value of an AttributeSet of a given index
 *
 * Inputs: int - index of the Attribute to be checked
 *         ushort - value to set the attribute
 * Output: bool - true if the attribute was set, false otherwise
 */
bool AttributeSet::setStat(int index, ushort value)
{
  if (index != -1 && value <= max_values.at(index) && value < kMAX_VALUE)
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
QString AttributeSet::getAttrStr(Attribute type)
{
  const std::string &attribute_string = EnumString<Attribute>::From(type);
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
Attribute AttributeSet::getAttr(QString name)
{
  const std::string &attribute_string = name.toUtf8().constData();
  Attribute attribute_type;
  EnumString<Attribute>::To(attribute_type, attribute_string);
  return attribute_type;
}

/*
 * Description: Gets the index of a stat given an enumerated type
 *
 * Inputs: Attribute type - enumerated attribute to be checked
 * Output: int - the index of the enumerated if found, -1 otherwise
 */
int AttributeSet::getIndex(Attribute type)
{
  switch (type)
  {
    case VITALITY:  return 0;
    case QUANTUMDRIVE: return 1;
    case PHYSICALAGGRESSION: return 2;
    case PHYSICALFORTITUDE: return 3;
    case THERMALAGGRESSION: return 4;
    case THERMALFORTITUDE: return 5;
    case POLARAGGRESSION: return 6;
    case POLARFORTITUDE: return 7;
    case PRIMALAGGRESSION: return 8;
    case PRIMALFORTITUDE: return 9;
    case CHARGEDAGGRESSION: return 10;
    case CHARGEDFORTITUDE: return 11;
    case CYBERNETICAGGRESSION: return 12;
    case CYBERNETICFORTITUDE: return 13;
    case NIHILAGGRESSION: return 14;
    case NIHILFORTITUDE: return 15;
    case MOMENTUM: return 16;
    case LIMBERTUDE: return 17;
    case UNBEARABILITY: return 18;
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
QString AttributeSet::getName(Attribute type)
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
 * Description: Prints out the stats (normal and max values) of the attribute
 *              set.
 *
 * Inputs: none
 * Output: none
 */
void AttributeSet::printInfo()
{
  qDebug() << " --- Attr Set:  < Normal Value | Max Value > ---";

  /* Loop through each stat and print the values */
  for (int i = 0; i < getSize(); i++)
  {
    qDebug() <<  short_names.at(i) << " < " << getStat(i) << " | "
              << getMax(i) << " >";
  }

  qDebug() << " --- / Attr Set --- ";
}
