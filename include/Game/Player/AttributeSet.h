/******************************************************************************
* Class Name: AttributeSet
* Date Created: March 5th, 2013
* Inheritance: QWidget
* Description: The attribute set is a container for all the statistics used
*              by race, category, person, etc. It holds vectors of their values,
*              maximum values, short names. The class also holds data for
*              AttributeSet presets.
******************************************************************************/
#ifndef ATTRIBUTESET_H
#define ATTRIBUTESET_H

#include <QtGui/QWidget>
#include <QDebug>
#include "EnumDatabase.h"

class AttributeSet
{
public:
  /* Constructor: Constructs a default attribute set object */
  AttributeSet();

  /* Constructor: Constructs an attribute set given a vector of values. */
  AttributeSet(QVector<ushort> stat_values);

  /* Annihilates an AttributeSet object */
  ~AttributeSet();

private:
  /*------------------- Constants -----------------------*/
  static const ushort kDEFAULT_MIN; /* Default value for a min stat */
  static const ushort kDEFAULT_MAX; /* Default value for a max stat */
  static const ushort kMAX_VALUE;   /* The maximum value of any stat */

  /* Container for short names of Attributes */
  static QVector<QString> short_names;

  /* Containers for the values and the maximum values of the Attribute Set */
  QVector<ushort> values;
  QVector<ushort> max_values;

  /* Initial setup for presets -- builds preset vectors of stats */
  static void buildPresets();

  /* Initial setup of short name vectors */
  static void setUpNames();

public:
  /* Alters a given stat by a certain amount {Damage, Buff} */
  bool changeStat(Attribute type, int amount);
  bool changeStat(QString name, int amount);
  bool changeStat(int index, int amount);

  /* Enum to String and vice versa functions for Attributes */
  static QString getAttrStr(Attribute type);
  static Attribute getAttr(QString name);

  /* Returns the index of a given stat */
  int getIndex(Attribute type);
  int getIndex(QString name);

  /* Gets the string names of statistics */
  static QString getName(Attribute type);
  static QString getName(int index);

  /* Returns the maximum values of a given stat */
  short getMax(Attribute type);
  short getMax(QString name);
  short getMax(int index);

  /* Returns the number of attributes */
  static int getSize();

  /* Returns the normal values of a given stat */
  short getStat(Attribute type);
  short getStat(QString name);
  short getStat(int index);

  /* Overridden functions for setting all the values in a stat set */
  void setAll(QVector<ushort> stat_values, bool max = FALSE);
  void setAll(ushort a, ushort b, ushort c, ushort d, ushort e, ushort f,
              ushort g, ushort h, ushort i, ushort j, ushort k, ushort l,
              ushort m, ushort n, ushort o, ushort p, ushort q, ushort r,
              ushort s, bool max = FALSE);

  /* Sets the maximum values of the Attribute Set */
  bool setMax(Attribute type, ushort value);
  bool setMax(QString name, ushort value);
  bool setMax(int index, ushort value);

  /* Sets the normal stat values of the AttributeSet */
  bool setStat(Attribute type, ushort value);
  bool setStat(QString name, ushort value);
  bool setStat(int index, ushort value);
};

#endif // ATTRIBUTESET_H
