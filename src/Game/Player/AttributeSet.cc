/******************************************************************************
* Class Name: PersonStatusBar
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: StatusBar
* Description: Displays information on persons on the main battle menu,
*              including their health, thumbnails of inflicted statu ailments,
*              and their level.
******************************************************************************/

#include "Game/Player/AttributeSet.h"

AttributeSet::AttributeSet()
{
  setUpNames();
  for (int i = 0; i < names.size(); i++)
  {
      values.append(0);
      max_values.append(kMAX_VALUE - 1);
  }
  names.squeeze();
  short_names.squeeze();
  values.squeeze();
  max_values.squeeze();
}

const bool AttributeSet::changeStat(QString name, int amount)
{  for (int i = 0; i < names.size(); i++)
  {
    if (name == names.at(i) || name == short_names.at(i))
    {
      if (amount < 0 && -amount >= values.at(i))
        values[i] = 0;
      else if (amount > 0 && amount >= max_values.at(i))
        values[i] = max_values.at(i);
      else
      {
        values[i] = amount;
        qDebug() << amount;
      }
      return TRUE;
    }
  }
  return FALSE;
}

const bool AttributeSet::changeStat(int index, int amount)
{
  return changeStat(names.at(index), amount);
}

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
}

QString AttributeSet::getName(int index)
{
  if (index < names.size())
    return names.at(index);
  return "";
}

QString AttributeSet::getShortName(int index)
{
 if (index < short_names.size())
   return short_names.at(index);
 return "";
}

int AttributeSet::getSize()
{
  return names.size();
}

short AttributeSet::getMax(QString name)
{
  for (int i = 0; i < names.size(); i++)
    if (name == names.at(i) || name == short_names.at(i))
      return max_values.at(i);
  return -1;
}

short AttributeSet::getMax(int index)
{
    return getMax(names.at(index));
}

int AttributeSet::getIndex(QString name)
{
  for (int i = 0; i < names.size(); i++)
    if (name == names.at(i) || name == short_names.at(i))
      return i;
  return -1;
}

short AttributeSet::getStat(QString name)
{
  for (int i = 0; i < names.size(); i++)
    if (name == names.at(i) || name == short_names.at(i))
      return values.at(i);
  return -1;
}

short AttributeSet::getStat(int index)
{
  if (index < values.size())
    return values.at(index);
  return -1;
}

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

const bool AttributeSet::setMax(QString name, ushort value)
{
  for (int i = 0; i < names.size(); i++)
  {
    if (name == names.at(i) || name == short_names.at(i))
    {
      if (value < kMAX_VALUE && value >= values.at(i))
        max_values[i] = value;
      else
        max_values[i] = kMAX_VALUE;
      return TRUE;
    }
  }
  return FALSE;
}

const bool AttributeSet::setMax(int index, ushort value)
{
 return setMax(names.at(index), value);
}

const bool AttributeSet::setStat(QString name, ushort value)
{
  for (int i = 0; i < names.size(); i++)
  {
    if (name == names.at(i) || name == short_names.at(i))
    {
      if (value < max_values.at(i) && value < kMAX_VALUE)
      {
          values[i] = value;
          return TRUE;
      }
      else
        return FALSE;
    }
  }
  return FALSE;
}

const bool AttributeSet::setStat(int index, ushort value)
{
    return setStat(names.at(index), value);
}
