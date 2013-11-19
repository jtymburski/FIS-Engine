/*******************************************************************************
* Class Name: AttributeSet [Declaration]
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
* TODO
* ----
* - More Presets [11-18-13]
* - Conventions [11-18-13]
*******************************************************************************/
#ifndef ATTRIBUTESET_H
#define ATTRIBUTESET_H

#include <vector>

#include "EnumDb.h"
#include "MathHelper.h"

class AttributeSet
{
public:
  /* Default constructor, can also be used to build presets */
  AttributeSet(const int &preset_level = 0);

  /* Copy constructor */
  AttributeSet(const AttributeSet &source);

  /* Construct and AttributeSet from a vector of values */
  AttributeSet(std::vector<uint> new_values);

  /* Annihilates an AttributeSet object */
  ~AttributeSet();

private:
  /* The values of the statistics within the AttributeSet */
  std::vector<uint> values;

  /* ------------ Constants --------------- */
  static const std::vector<std::string> kSHORT_NAMES; /* The names of Attrs */
  static const std::vector<std::string> kLONG_NAMES; /* Full names of Attrs */
  static const std::vector<uint> kPRESET1;
  static const std::vector<uint> kPRESET2;
  static const std::vector<uint> kPRESET3;

  static const uint kDEFAULT;    /* Default value for a min stat */
  static const uint kNUM_VALUES; /* The number of values in an AttributeSet */
  static const uint kMIN_VALUE;  /* The minimum value of any stat */
  static const uint kMAX_VALUE;  /* The maximum value of any stat */
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

  bool alterStat(uint index, int amount);

  bool alterStat(Attribute stat, int amount);

  bool alterStat(std::string name, int amount);

  void printValues();

  int getIndex(Attribute stat);

  int getIndex(std::string name);

  static std::string getLongName(uint index);

  std::string getName(Attribute stat);

  int getStat(uint index = 0);

  int getStat(Attribute stat);

  int getStat(std::string name);

  bool setStat(uint index, int value);

  bool setStat(Attribute stat, int value);

  bool setStat(std::string name, int value);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:

  static uint getSize();

  std::string getLongName(Attribute stat);

  static std::string getName(uint index);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/ 
public:
  /* Overloaded = operator for copying */
  AttributeSet& operator=(const AttributeSet &source);

  /* Overloaded += operator for compound assignment */
  AttributeSet& operator+=(const AttributeSet& rhs);
};

/* Overloaded + operator for AttributeSet additions */
inline AttributeSet& operator+(AttributeSet lhs, const AttributeSet &rhs);

#endif //ATTRIBUTESET_H