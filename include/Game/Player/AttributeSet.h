/******************************************************************************
* Class Name: AttributeSet
* Date Created: March 5th, 2013
* Inheritance: None
* Description: The attribute set is a container for all the statistics used
*              by race, category, person, etc. It holds vectors of their:
*              names, abbreviations, values, and maximum values.
******************************************************************************/

#ifndef ATTRIBUTESET_H
#define ATTRIBUTESET_H

#include <QtGui/QWidget>
#include <QDebug>

class AttributeSet
{
public:
  /* Constructs an AttributeSet object */
  AttributeSet();

  /* Annihilates an AttributeSet object */
  ~AttributeSet();

private:
  /* Absolute maximum value of an attribute */
  static const ushort kMAX_VALUE = 25000;

  /* Containers for names and values */
  QVector<QString> names;
  QVector<QString> short_names;
  QVector<ushort> values;
  QVector<ushort> max_values;

public:
  /* Alters a given stat by a certain amount {Damage, Buff} */
  const bool changeStat(QString name, int amount);
  const bool changeStat(int index, int amount);

  /* Initial setup for constructor */
  void setUpNames();

  /* Gets the string names of statistics */
  QString getName(int index);
  QString getShortName(int index);

  /* Returns the number of attributes */
  int getSize();

  /* Returns the maximum values of a given stat */
  short getMax(QString name);
  short getMax(int index);

  /* Returns the index of a given stat */
  int getIndex(QString name);

  /* Returns the normal values of a given stat */
  short getStat(QString name);
  short getStat(int index);

  /* Sets all the stats as a plethora of values */
  void setAll(ushort a, ushort b, ushort c, ushort d, ushort e, ushort f,
              ushort g, ushort h, ushort i, ushort j, ushort k, ushort l,
              ushort m, ushort n, ushort o, ushort p, ushort q, ushort r,
              ushort s, bool max = FALSE);

  /* Sets the maximum and normal values of a given stat */
  const bool setMax(QString name, ushort value);
  const bool setMax(int index, ushort value);
  const bool setStat(QString name, ushort value);
  const bool setStat(int index, ushort value);
};

#endif // ATTRIBUTESET_H
