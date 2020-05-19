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
 ******************************************************************************/
#ifndef ATTRIBUTESET_H
#define ATTRIBUTESET_H

#include <iostream>
#include <vector>

#include "EnumDb.h"
#include "EnumFlags.h"
#include "Helpers.h"

/* AttributeState flags */
ENUM_FLAGS(AttributeState)
enum class AttributeState
{
  PERSONAL = 1 << 0, /* Is this AttributeSet restricted to Personal values?   */
  CONSTANT = 1 << 1  /* Is this AttributeSet restricted to unchanging values? */
};

class AttributeSet
{
public:
  /* Default constructor */
  AttributeSet();

  /* Default move-constructor */
  AttributeSet(AttributeSet&&) = default;

  /* Default copy-constructor */
  AttributeSet(const AttributeSet&) = default;

  /* Preset constructor */
  AttributeSet(const int32_t& preset_level, const bool& personal = false,
               const bool& constant = false);

  /* Construct and AttributeSet from a vector of values */
  AttributeSet(const std::vector<int32_t>& new_values,
               const bool& personal = false, const bool& constant = false);

  /* Default destructor */
  ~AttributeSet() = default;

private:
  /* Flags for the state of the Attribtue */
  AttributeState flags{static_cast<AttributeState>(0)};

  /* The values of the statistics within the AttributeSet */
  std::vector<int32_t> values{};

  /* ------------ Constants --------------- */
  static const std::vector<std::string> kSHORT_NAMES; /* The names of Attrs  */
  static const std::vector<std::string> kLONG_NAMES;  /* Full names of Attrs */
  static const std::vector<std::vector<int32_t>> kPRESETS; /* Pre-built stats */

  /* ------------ PUBLIC Constants --------------- */
public:
  static const int32_t kDEFAULT;       /* Default value for a min stat   */
  static const int32_t kMIN_P_VALUE;   /* The minimum value for a p. set */
  static const int32_t kMIN_VALUE;     /* The minimum value of any stat  */
  static const uint32_t kMAX_VALUE;    /* The maximum value of any stat  */
  static const uint32_t kNUM_ELEMENTS; /* Number of elements in set */

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Basic setup for construction */
  void classSetup(const bool& personal, const bool& constant);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Method for storing and building AttributeSets from preset values */
  void buildAsPreset(const size_t& level = 0);

  /* Assert all values are within range--including size of values */
  void cleanUp();

  /* Print out the values of the Attribute Set */
  void print(const bool& simple = false);

  /* Methods for altering a stat by a given amount given either an index,
   * enumerated value of the Attribute, or a string name (short or long */
  bool alterStat(const int32_t& index, const int32_t& amount);
  bool alterStat(const Attribute& stat, const int32_t& amount);
  bool alterStat(const std::string& name, const int32_t& amount);

  /* Evaluate a given AttributeState enumerated flag */
  bool getFlag(AttributeState test_flag) const;

  /* Methods for returning the value of a stat given either an index, an
     enumerated value, or a string name (short or long) */
  int getStat(const int32_t& index = 0) const;
  int getStat(const Attribute& stat) const;
  int getStat(const std::string& name) const;

  /* Sets flags of the attribute set */
  void setFlag(const AttributeState& flag, const bool& state = true);

  /* Methods for assigning a new value to a stat given either an index, an
   * enumerated value, or a string name (short or long) */
  bool setStat(const int32_t& index, const int32_t& value);
  bool setStat(const Attribute& stat, const int32_t& value);
  bool setStat(const std::string& name, const int32_t& value);

  /*===================== PUBLIC STATIC FUNCTIONS ============================*/
public:
  // /* Returns all offensive attributes */
  // static std::vector<Attribute> getAllOffensive();

  // /* Returns all defensive attributes */
  // static std::vector<Attribute> getAllDefensive();

  /* Method for getting the index of an attribute by an enum  */
  static int32_t getIndex(const Attribute& stat);

  // /* Returns the element of the corresponding offensive statistic for an elmt */
  // static int32_t getOffensiveIndex(const Element& stat);

  // /* Returns the element of the corresponding defensive statistic for an elmt */
  // static int32_t getDefensiveIndex(const Element& stat);

  /* Method for returning the index of an attribute given a name */
  static int32_t getIndex(const std::string& name);

  /* Returns the max value for an attribute */
  static uint32_t getMaxValue();

  /* Returns the size of an attribute set (static -- returns kNUM_VALUES) */
  static size_t getSize();

  /* Returns the long form name of an attribtue given an enumerated value */
  static std::string getLongName(const Attribute& s);

  /* Returns the long-form name of an attribute at a given index */
  static std::string getLongName(const size_t& index);

  /* Returns the short form name of an attribute given an enumerated value */
  static std::string getName(const Attribute& stat);

  /* Returns the short name form of a stat at a given index */
  static std::string getName(const size_t& index);

  /* Create set from comma delimited string, or create delimited set */
  static AttributeSet setFromStr(const std::string& str);
  static std::string setToStr(const AttributeSet& set);

  /*========================= OPERATOR FUNCTIONS =============================*/
public:
  /* Default move-assignment operator */
  AttributeSet& operator=(AttributeSet&&) = default;

  /* Default assignment operator */
  AttributeSet& operator=(const AttributeSet&) = default;

  /* Overloaded += operator for compound assignment */
  AttributeSet& operator+=(const AttributeSet& rhs);

  /* Overloaded + operator for AttributeSet additions */
  AttributeSet& operator+(const AttributeSet& rhs);

  /* Overloaded - operator for Attribute subtractions */
  AttributeSet& operator-(const AttributeSet& rhs);

  /* Overloaded -= operator for subtractive assignment */
  AttributeSet& operator-=(const AttributeSet& rhs);

  /* Overloaded == operator for comparing AttributeSets */
  bool operator==(const AttributeSet& rhs);

  /* Overloade != operator for comparing AttributeSets */
  bool operator!=(const AttributeSet& rhs);
};

#endif // ATTRIBUTESET_H
