/******************************************************************************
* Class Name: AttributeSet
* Date Created: March 5th, 2013
* Inheritance: None
* Description: The attribute set is a container for all the statistics used
*              by race, category, person, etc. It holds vectors of their values,
*              maximum values, short names. The class also holds data for
*              AttributeSet presets.
******************************************************************************/
#ifndef ATTRIBUTESET_H
#define ATTRIBUTESET_H

#include <QDebug>
#include <QWidget>

#include "EnumDb.h"

class AttributeSet
{
public:
  /* Constructor: Constructs a default attribute set object */
  AttributeSet();

  /* Constructor: Constructs an attribute set given a vector of values. */
  AttributeSet(QList<uint> stat_values);

  /* Annihilates an AttributeSet object */
  ~AttributeSet();

private:
  /* Container for short names of Attributes */
  static QList<QString> short_names;

  /* Containers for the values and the maximum values of the Attribute Set */
  QList<uint> values;

  /*------------------- Constants -----------------------*/
  static const uint kDEFAULT_MIN; /* Default value for a min stat */
  static const uint kDEFAULT_MAX; /* Default value for a max stat */
  static const uint kMIN_VALUE;
  static const uint kMAX_VALUE;   /* The maximum value of any stat */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Initial setup of short name vectors */
  static void setUpNames();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Alters a given stat by a certain amount {Damage, Buff} */
  bool changeStat(EnumDb::Attribute type, int amount);
  bool changeStat(QString name, int amount);
  bool changeStat(int index, int amount);

  /* Returns the index of a statistic given the 4-character QString */
  int getIndex(QString name);

  /* Returns the normal values of a given stat give an enum, string, or int*/
  short getStat(EnumDb::Attribute type);
  short getStat(QString name);
  short getStat(int index);

  /* Overridden functions for setting all the values in a stat set */
  void setAll(QList<uint> stat_values);

  /* Sets the normal stat values of the AttributeSet */
  bool setStat(EnumDb::Attribute type, uint value);
  bool setStat(QString name, uint value);
  bool setStat(int index, uint value);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Converts an Attribute enumeration to a QString (full name of attribute) */
  static QString getAttrStr(EnumDb::Attribute type);

  /* Converts a QString to the corresponding Attribute enumeration */
  static EnumDb::Attribute getAttr(QString name);

  /* Returns the index of a statistic given an Attribute enumeration */
  static int getIndex(EnumDb::Attribute type);

  /* Obtains the 4-character QString corresponding to an Attribute enum */
  static QString getName(EnumDb::Attribute type);

  /* Obtains the 4-character QString corresponding to the index of attributes */
  static QString getName(int index);

  /* Returns the number of attributes */
  static int getSize();

  /* Prints out the stats of the Attribute set */
  void printInfo();
};

#endif // ATTRIBUTESET_H
