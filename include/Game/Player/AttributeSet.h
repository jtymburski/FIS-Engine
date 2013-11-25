/*******************************************************************************
* Class Name: AttributeSet [Declaration]
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
* TODO
* ----
* - Build more unique flavoured presets for Battle testing/generics [11-18-13]
*******************************************************************************/
#ifndef ATTRIBUTESET_H
#define ATTRIBUTESET_H

#include <vector>

#include "EnumDb.h"
#include "Helpers.h"

class AttributeSet
{
public:
  /* Default constructor */
  AttributeSet();

  /* Preset constructor */
  AttributeSet(const int &preset_level);

  /* Copy constructor */
  AttributeSet(const AttributeSet &source);

  /* Construct and AttributeSet from a vector of values */
  AttributeSet(const std::vector<uint> &new_values);

  /* Annihilates an AttributeSet object */
  ~AttributeSet();

private:
  /* The values of the statistics within the AttributeSet */
  std::vector<uint> values;

  /* ------------ Constants --------------- */
  static const std::vector<std::string> kSHORT_NAMES; /* The names of Attrs */
  static const std::vector<std::string> kLONG_NAMES;  /* Full names of Attrs */
  static const std::vector<uint> kPRESET1; /* Basic starting stats */
  static const std::vector<uint> kPRESET2; /* Weak stats */
  static const std::vector<uint> kPRESET3; /* Medium stats */

  static const uint kDEFAULT;     /* Default value for a min stat */
  static const uint kNUM_VALUES;  /* The number of values in an AttributeSet */
  static const uint kMIN_VALUE;   /* The minimum value of any stat */
  static const uint kMAX_VALUE;   /* The maximum value of any stat */
  static const uint kNUM_PRESETS; /* The number of presets one may build from */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Method for storing and building AttributeSets from preset values */
  void buildAsPreset(const uint &level = 0);

  /* Method for copy constructor / assignment operator */
  void copySelf(const AttributeSet &source);

  /* Assert all values are within range--including size of values */
  void cleanUp();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Methods for altering a stat by a given amount given either an index,
   * enumerated value of the Attribute, or a string name (short or long */
  bool alterStat(const uint &index, const int &amount);
  bool alterStat(const Attribute &stat, const int &amount);
  bool alterStat(const std::string &name, const int &amount);

  /* Method for getting the index of an attribute by an enum  */
  int getIndex(const Attribute &stat);

  /* Returns the long form name of an attribtue given an enumerated value */
  static std::string getLongName(const Attribute &stat);

  /* Returns the short form name of an attribute given an enumerated value */
  static std::string getName(const Attribute &stat);

  /* Methods for returning the value of a stat given either an index, an
     enumerated value, or a string name (short or long) */
  int getStat(const uint &index = 0);
  int getStat(const Attribute &stat);
  int getStat(const std::string &name);

  /* Methods for assigning a new value to a stat given either an index, an
   * enumerated value, or a string name (short or long) */
  bool setStat(const uint &index, int value);
  bool setStat(const Attribute &stat, const int &value);
  bool setStat(const std::string &name, const int &value);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Method for returning the index of an attribute given a name */
  static int getIndex(const std::string &name);

  /* Returns the size of an attribute set (static -- returns kNUM_VALUES) */
  static uint getSize();

  /* Returns the long-form name of an attribute at a given index */
  static std::string getLongName(const uint &index);

  /* Returns the short name form of a stat at a given index */
  static std::string getName(const uint &index);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/ 
public:
  /* Overloaded = operator for copying */
  AttributeSet& operator=(const AttributeSet &source);

  /* Overloaded += operator for compound assignment */
  AttributeSet& operator+=(const AttributeSet& rhs);

  /* Overloaded -= operator for subtractive assignment */
  AttributeSet& operator-=(const AttributeSet& rhs);
};

/* Overloaded + operator for AttributeSet additions */
inline AttributeSet operator+(AttributeSet lhs, const AttributeSet &rhs);

/* Overloaded - operator for AttributeSet subtractions */
inline AttributeSet operator-(AttributeSet lhs, const AttributeSet &rhs);

#endif //ATTRIBUTESET_H