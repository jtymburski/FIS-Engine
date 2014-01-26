/*******************************************************************************
* Class Name: Action [Declaration]
* Date Created: November 19th, 2013
* Inheritance: None
* Description: An action is an element of a skill used buy a person in a Battle.
*              A skill may have a number of actions which have various effects:
*              altering stats, flipping flags, inflict ailments/buffs. An action
*              is constructed from parsing a string from a file. Actions have
*              unique IDs.
*
* Notes
* -----
*
* [1]: The Action is parsed from a string with convention as follows:
*
* [ID],[ALTER/INFLICT/RELIEVE/ASSIGN],[ATTRIBUTE/AILMENT],[MIN].[MAX],
* [IGNORE ATK ELEMENT 1].[IGNORE ATK ELEMENT 2]...,
* [IGNORE DEF ELEMENT 1].[IGNORE DEF ELEMENT 2]...,
* [AMOUNT/PC].[BASE],[AMOUNT/PC].[VARIANCE]
*
* Where:
*   - ID - the unique ID that represents the action
*   - ALTER/INFLICT/RELIEVE/ASSIGN/REVIVE keywords - see ActionFlags enum
*   - ATTRIBUTE/AILMENT - the affected attribute or ailment by the key words
*   - MIN.MAX - the duration an inflicted ailment will persist
*   - IGNORE ATK ELMENTs - a list of valid elements which when set, will not
*                          include the action user's corresponding offensive
*                          elemental statistics into battle calculations.
*                         - can use ALL for all elements or ELEMENTAL for
*                           everything non-physical
*   - IGNORE DEF ELEMENTs - a list of valid elements which when set, will not
*                           include the action target's corresponding defensive
*                           elemental statistics into battle calculations.
*                         - can use ALL for all elements or ELEMENTAL for
*                           everything non-physical
*   - BASE - the base power of the action (amount an attribute is affected)\
*          - negative values will be used only when the ALTER key word is set
*   - AMOUNT/PC - decides between utilizing base and variance as an amount 
*                 (+ or -) value or as a factor (%) value
*   - VARIANCE - the variance (even distribution) which base may change by
*              - may be set to -1 for the highest possible variance
* -----
*
* [2]: Example actions and their intended effects:
*
* 1,ALTER,THAG,,,,AMOUNT.50,AMOUNT.15
* will alter Thermal Aggression of target by 50 +/- 15
*
* 1,ALTER,VITA,,PHYSICAL,PHYSICAL.THERMAL,100,AMOUNT.50,AMOUNT.10
* will damage target ignoring user's phys. atk and target's phys. and ther. def
* at 50 +/- 15 points
*
* 1,INFLICT,POISON,2.7,,,,,
* will inflict poison lasting 2-7 turns
*
* 1,RELIEVE,CURSE,,,,,,
* will relieve curse
*
* 1,REVIVE,,,,,PC.25,AMOUNT.50
* will revive a KO'd target with 25% +/- 15 VITA
*
* TODO
* ----
* - [12-08-13] Print the string of the attribute the action alters or assigns
* - [12-08-13] Print the string of the ailment the action inflicts or relieves 
*******************************************************************************/

#ifndef ACTION_H
#define ACTION_H

#include <iostream>

#include "EnumFlags.h"
#include "Helpers.h"

/* ActionFlags for storing the type of Action */
ENUM_FLAGS(ActionFlags)
enum class ActionFlags
{
  ALTER    = 1 << 0, /* ALTER an attribute by a given value */
  INFLICT  = 1 << 1, /* INFLICT a given ailment for a duration */
  RELIEVE  = 1 << 2, /* RELIEVE a given ailment */
  ASSIGN   = 1 << 3, /* ASSIGN an attribute to a given value */
  REVIVE   = 1 << 4, /* REVIVE un-KOs target with base HP */
  BASE_PC  = 1 << 5, /* True if the base is a % value and not an abs. one */
  VARI_PC  = 1 << 6, /* True if the variance is a % value and not an abs. one */
  VALID    = 1 << 7  /* The validity of the action */
};

/* IgnoreFlags for storing which elemental atk/def stats are ignored */
ENUM_FLAGS(IgnoreFlags)
enum class IgnoreFlags
{
  PHYSICAL   = 1 << 0,
  THERMAL    = 1 << 1,
  POLAR      = 1 << 2,
  PRIMAL     = 1 << 3,
  CHARGED    = 1 << 4,
  CYBERNETIC = 1 << 5,
  NIHIL      = 1 << 6
};

class Action
{
public:
  /* Constructs a default Action object */
  Action();

  /* Constructs an Action object by parsing a string */
  Action(const std::string &raw);

  /* Annihilates an action object - default destructor */
  ~Action() = default;

private:
  /* Set of ActionFlags for the current action */
  ActionFlags action_flags;

  /* Enumerated attribute which the action may alter */
  Attribute attribute;

  /* Enumerated infliction which the action may inflict/relieve */
  Infliction ailment;

  /* Base value (amt. or pc in flag) by which to change the attribute by */
  int32_t base;

  /* ID of the current Action [parsed in] */
  int32_t id;

  /* Two ignore flag sets in relation to atk & def attributes */
  IgnoreFlags ignore_atk;
  IgnoreFlags ignore_def;

  /* Minimum and maximum duration an infliction will last for */
  int min_duration;
  int max_duration;

  /* Amount of variance on the base value that can take place */
  uint32_t variance;

  /* ------------ Constants --------------- */
  static const bool     kDEBUG_ENABLED; /* Show parse warning messages? */
  static const int32_t  kDEFAULT_MIN; /* Default turn minimum for ailment */
  static const int32_t  kDEFAULT_MAX; /* Default turn maximum for ailment */
  static const char     kDELIMITER;   /* The delimiter for string parsing */
  static const char     kDELIMITER_2; /* The secondary delimiter */
  static const int32_t  kMAX_BASE_PC; /* Max % value for Base change */
  static const uint32_t kMAX_VARIANCE_PC; /* Max % value for Variance */
  static const int32_t  kUNSET_ID;  /* ID for a default Action object */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Main function for parsing the raw string */
  bool parse(const std::string &raw);

  /* Sub-method for parsing the string containing the ailment */
  bool parseAilment(const std::string &ailm_parse);

  /* Sub-method for parsing the string containing the action kewyord */
  bool parseActionKeyword(const std::string &action_keyword);

  /* Sub-method for parsing the string containing the affected attribute */
  bool parseAttribute(const std::string &attr_parse);
  
  /* Sub-method for parsing the ignore atk and ignore def flags */
  void parseIgnoreFlags(IgnoreFlags& flag_set, const std::string &flags);

  /* Emits a warning from a given string and turns off the VALID flag */
  void parseWarning(const std::string &warning, const std::string &raw);

  /* Assigns an infliction duration returning the validity of the assignment */
  bool setDuration(const int &min_value, const int &max_value);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Prints out the state of the current Action */
  void print();

  /* Evaluates a given action flag */
  bool actionFlag(ActionFlags test_flag);

  /* Evaluates a given ignore atk flag */
  bool atkFlag(IgnoreFlags test_flag);

  /* Evaluates a given ignore def flag */
  bool defFlag(IgnoreFlags test_flag);

  /* Returns the Attribute the action may alter/assign */
  Attribute getAttribute();

  /* Returns the infliction the action may inflict/relieve */
  Infliction getAilment();

  /* Returns the base change */
  int getBase();

  /* Returns the ID of the Action */
  int getID();

  /* Methods for returning the min and max portions of the duration */
  int getMin();
  int getMax();

  /* Returns the variance of the Action */
  uint32_t getVariance();
};

#endif //ACTION_H
