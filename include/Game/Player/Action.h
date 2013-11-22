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
* - !!! CLASS UNTESTED - Testing [11-20-13]
* - Conventions [11-19-13]
*******************************************************************************/

#ifndef ACTION_H
#define ACTION_H

#include <climits> /* INT_MAX */
#include <string>  /* std::stoi */
#include <vector>

#include "EnumDb.cc"
#include "EnumFlags.h"
#include "Helpers.h"

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

  /* Annihilates an action object */
  ~Action();

private:

  ActionFlags action_flags;

  Attribute attribute;

  Infliction ailment;

  int base;

  int id;

  IgnoreFlags ignore_atk;
  IgnoreFlags ignore_def;

  int min_duration;
  int max_duration;

  int variance;

  /* ------------ Constants --------------- */
  static const int kDEFAULT_ID;  /* ID for a default Action object */
  static const char kDELIMITER;  /* The delimiter for string parsing */
  static const char kDELIMITER_2; /* The secondary delimiter */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

  bool parse(const std::string &raw);

  bool parseAilment(const std::string &ailm_parse);

  bool parseActionKeyword(const std::string &action_keyword);

  bool parseAttribute(const std::string &attr_parse);
  
  void parseIgnoreFlags(IgnoreFlags& flag_set, const std::string &flags);

  bool setDuration(const int &min_value, const int &max_value);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  Attribute getAttribute();

  Infliction getAilment();

  int getBase();

  ActionFlags getActionFlags();

  int getID();

  IgnoreFlags getIgnoreAtk();
  IgnoreFlags getIgnoreDef();

  int getMin();
  int getMax();

  float getVariance();
};

#endif //ACTION_H
