/******************************************************************************
* Class Name: PersonStatusBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: StatusBar
* Description: Displays information on persons on the main battle menu,
*              including their health, thumbnails of inflicted statu ailments,
*              and their level.
******************************************************************************/
#include "Game/Player/AttributeSet.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/
QVector<QString> AttributeSet::names;
QVector<QString> AttributeSet::short_names;

/*
 * Description: Constructs an attribute set (default min 0 stats and max kMAX)
 *
 * Inputs: none
 */
AttributeSet::AttributeSet()
{
  if (names.empty())
    setUpNames();
  for (int i = 0; i < names.size(); i++)
  {
      values.append(0);
      max_values.append(kMAX_VALUE - 1);
  }
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
 * Description: Instantiates the names and short_names vectors on the first
 *              initialization of an AttributeSet
 *
 * Inputs: none
 * Output: none
 */
void AttributeSet::setUpNames()
{
  names << "Vitality" << "Quantum Drive" << "Physical Aggression"
        << "Physical Fortitude" << "Thermal Aggression" << "Thermal Fortitude"
        << "Primal Aggression" << "Primal Fortitude" << "Polar Aggression"
        << "Polar Fortitude" << "Charged Aggression" << "Charged Fortitude"
        << "Cybernetic Aggression" << "Cybernetic Fortitude"
        << "Nihil Aggression" << "Nihil Fortitude" << "Limbertude"
        << "Momentum" << "Unbearability";
  short_names << "VITA" << "QTDR" << "PHAG" << "PHFD" << "THAG" << "THFD"
              << "POAG" << "POFD" << "PRAG" << "PRFD" << "CHAG" << "CHFD"
              << "CYAG" << "CYFD" << "NIAG" << "NIFD" << "LIMB" << "MMTM"
              << "UNBR";
  names.squeeze();
  short_names.squeeze();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Changes a given stat name by a given value
 *
 * Inputs: QString - the name or short name of the stat to be changed
 *         int     - the value by which to change the stat
 * Output: bool - TRUE if the stat was found and changed, FALSE otherwise
 */
const bool AttributeSet::changeStat(QString name, int amount)
{
  if (getIndex(name) != -1)
  {
    if (amount < 0 && -amount >= values.at(getIndex(name)))
        values[getIndex(name)] = 0;
    else if (amount > 0 && amount >= max_values.at(getIndex(name)))
        values[getIndex(name)] = max_values.at(getIndex(name));
    else
        values[getIndex(name)] += amount;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Changes a given stat by name (overloaded functon)
 *
 * Inputs: int index - index of the stat to be changed.
 *         int amount - amount by which to change the stat
 * Output: bool - TRUE if the stat was found and changed, FALSE otherwise
 */
const bool AttributeSet::changeStat(int index, int amount)
{
  if (index < names.size())
    return changeStat(names.at(index), amount);
}

/*
 * Description: Gets the name of an attribute at a given index
 *
 * Inputs: int - index of the names to be checked
 * Output: QString - name of the attribute at the given index
 */
QString AttributeSet::getName(int index)
{
  if (index < names.size())
    return names.at(index);
  return "";
}

/*
 * Description: Gets the short name of an attribute at a give index
 *
 * Inputs: int - index of the short names to be checked
 * Output: QString - short name of the attribute at a given index
 */
QString AttributeSet::getShortName(int index)
{
 if (index < short_names.size())
   return short_names.at(index);
 return "";
}

/*
 * Description: Returns the size of the AttributeSet
 *
 * Inputs: int - index of the short names to be checked
 * Output: QString - short name of the attribute at a given index
 */
int AttributeSet::getSize()
{
  return names.size();
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
  return getMax(names.at(index));
}

/*
 * Description: Gets the index of a stat given a name
 *
 * Inputs: QString - name (or short name) to be checked
 * Output: int - the index of the stat if found in either name vector, or -1
 */
int AttributeSet::getIndex(QString name)
{
  for (int i = 0; i < names.size(); i++)
    if (name == names.at(i) || name == short_names.at(i))
      return i;
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
  return getStat(names.at(index));
}

/*
 * Description: Sets all the stats at once given 19 ushorts, and a bool
 *              whether you are setting the max stats or regular
 *
 * Inputs: ushort a through s - shorts describing the attributes
 *         bool max - whether setting the max or normal stats (default FALSE)
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
   setStat(14, o);
   setMax(15, p);
   setMax(16, q);
   setMax(17, r);
   setMax(18, s);
 }
}

/*
 * Description: Sets the maximum value of an Attribute of a given name
 *
 * Inputs: QString - name of the attribute to be set
 *         ushort - value to set the attribute
 * Output: bool - TRUE if the attribute was set, FALSE otherwise
 */
const bool AttributeSet::setMax(QString name, ushort value)
{
  if (getIndex(name) != -1)
  {
    if (value <= kMAX_VALUE && value >= values.at(getIndex(name)))
      max_values[getIndex(name)] = value;
    else
      max_values[getIndex(name)] = kMAX_VALUE;
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Sets the maximum value of an Attribute of a given index
 *
 * Inputs: int - index of the stat to be set
 *         ushort - value to set the attribute
 * Output: bool - TRUE if the attribute was set, FALSE otherwise
 */
const bool AttributeSet::setMax(int index, ushort value)
{
 return setMax(names.at(index), value);
}

/*
 * Description: Sets the value of an AttributeSet of a given name
 *
 * Inputs: QString - name of the attribute to be set
 *         ushort - value to set the attribute
 * Output: bool - TRUE if the attribute was set, FALSE otherwise
 */
const bool AttributeSet::setStat(QString name, ushort value)
{
  if (getIndex(name) != -1)
  {
    if (value <= max_values.at(getIndex(name)) && value < kMAX_VALUE)
    {
      values[getIndex(name)] = value;
      return TRUE;
    }
  }
  return FALSE;
}

/*
 * Description: Sets the value of an AttributeSet of a given index
 *
 * Inputs: int - index of the Attribute to be checked
 *         ushort - value to set the attribute
 * Output: bool - TRUE if the attribute was set, FALSE otherwise
 */
const bool AttributeSet::setStat(int index, ushort value)
{
  return setStat(names.at(index), value);
}
