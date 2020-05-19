/*******************************************************************************
* Class Name: Action [Declaration]
* Date Created: November 19th, 2013
* Inheritance: None
* Description: An action is an element of a skill used buy a person in a Battle.
*              A skill may have a number of actions which have various effects:
*              altering stats, flipping flags, inflict ailments/buffs. An action
*              is constructed from parsing a string from a file. Actions have
*              unique IDs.
* Notes
* ------------------------------------------------------------------------------
*
* [1]: The Action is parsed from a string with convention as follows:
*
* [ID],[DAMAGE/ALTER/INFLICT/RELIEVE/ASSIGN/REVIVE],[MIN].[MAX],
* [IGNORE ATK ELEMENT 1].[IGNORE ATK ELEMENT 2]...,
* [IGNORE DEF ELEMENT 1].[IGNORE DEF ELEMENT 2]...,
* [USER ATTR/AILMENT],[AMOUNT/PC].[BASE],[AMOUNT/PC].[VARIANCE],
* [TARGET ATTR],[CHANCE]
*
* Where:
*   - ID - the unique ID that represents the action
*   - DAMAGE/ALTER/INFLICT/RELIEVE/ASSIGN/REVIVE keywords - see ActionFlags enum
*   - ATTRIBUTE/AILMENT - the affected attribute or ailment by the key words
*   - MIN.MAX - the duration an inflicted ailment will persist
*   - IGNORE ATK ELMENTs - a list of valid elements which when set, will not
*                          include the action user's corresponding offensive
*                          elemental statistics into battle calculations.
*                         - can use ALL for all elements+phys or ELEMENTAL for
*                           everything non-physical
*   - IGNORE DEF ELEMENTs - a list of valid elements which when set, will not
*                           include the action target's corresponding defensive
*                           elemental statistics into battle calculations.
*                         - can use ALL for all elements+phys or ELEMENTAL for
*                           everything non-physical
*   - BASE - the base power of the action (amount an attribute is affected)
*          - negative values will be used only when the ALTER key word is set
*   - USER.ATTR/AILMENT - the user of the action's attribute or ailment which
*                         will be mutated
*                       - in the case of an ailment, this is the ailment
*                         which will be inflicted on to the user.
*   - TARGET.ATTR - the user of the action or target of the action's
*                   tribute as a factor of calculation to alter the
*                   mutated attribute
*   - AMOUNT/PC - decides between utilizing base and variance as an amount
*                 (+ or -) value or as a factor (%) value
*   - VARIANCE - the variance (even distribution) which base may change by
*              - may be set to -1 for the highest possible variance
* ------------------------------------
*
* [2]: Example actions and their intended effects:
*
*   [2.1]: Damage Actions
*     100,DAMAGE,,,,,AMOUNT.20,AMOUNT.5,,95
*        - Action power of 20 +/- 5 95% chance to hit
*     101,DAMAGE,,,,,AMOUNT.20,PC.5,,98
*        - Action power of 20 +/- 5% 98% chance to hit
*     102,DAMAGE,,PHYSICAL,,,AMOUNT.20,PC.5,,96
*       - Ignore phys atk in calc. plus action power of 20 +/-5% 96% chance
*     103,DAMAGE,,,ELEMENTAL,,,AMOUNT.300,AMOUNT.20,,100
*       - Ignore all elemental def in calc. plus power of 300 +/- 20 100%
*  -------
*
*   [2.2]: Alter Actions
*     [2.2.1]: When altering by percentage, the percentage altered is the
*              user's percentage of MAX values, or target's percentage of
*              MAX values. When both attributes are defined, the targets's
*              percentage of MAX will be altered BY the user's percentage
*              of MAX.
*     200,ALTER,,,,VITA,AMOUNT.100,AMOUNT.10,,99
*       - Restore 100 +/- 10 VITA to USER with 99% chance
*     201,ALTER,,,,,PC.25,AMOUNT.20,VITA,99
*       - Restores 25% +/- 20 VITA to TARGET with 99% chance
*     202,ALTER,,,,VITA,PC.10,AMOUNT.20,VITA,95
*       - Restores user's Vita by 10% +/- 20 of Target's VITA w/ 95% chance
*     203,ALTER,,,,,PC.-12,AMOUNT.20,VITA,100
*       - Alters target's vitality by -12% +/- 20 with 100% chance
*     204,ALTER,,,,QTDR,PC.10,AMOUNT.20,THAG,96
*       - Alters user's QTDR by 10% +/- 20 of target's THAG with 96% chance
*     205,ALTER-FLIP,,,,QTDR,PC.10,AMOUNT.20,THAG,97
*       - Alters target's QTDR by 10% +/- 20 of user's THAG with 97% chance
*
*   [2.3]: Assign Actions
*     [2.3.1]: When assigning percentages, the percentage altered is the
*              user's percentage of CURR values (ex. [5]/10VITA) not the max.
*              Likewise for target's, and when both attributes are assigned
*              the user's percentage of CURR values will be altered by the
*              target's percentage of CURR values.
*     300,ASSIGN,,,,VITA,AMOUNT.1,AMOUNT.0,,100
*       - Assign the user's VITA to a value of 1 with 100% accuracy.
*     301,ASSIGN,,,,VITA,AMOUNT.0,AMOUNT.0,,1
*       - Assign the user's VITA to a value of 0 with 1% accuracy (death action)
*     302,ASSIGN,,,,,PC.20,PC.5,QTDR,1
*       - Assign a target's QTDR to a value of 50% of max +/-
*     303,ASSIGN-FLIP,,,,VITA,PC.20,PC.5,QTDR,1
*       - Assign the target's VITA to a value of 20% +/- 5% of the user's QTDR
*
*   [2.4]: Revive Actions
*     [2.4.1]: The variance should NOT be greater than the base value.
*     400,REVIVE,,,,,AMOUNT.50,AMOUNT.10,,80
*       - Revive a target with 50 +/- 10 VITA with 80% efficiency
*     400,REVIVE,,,,,PC.25,AMOUNT.10,,90
*       - Revive a target with 25% +/- 10 VITA with 90% efficiency
*
*   [2.5]: Inflict Actions
*     500,INFLICT,2.7,,POISON,,,,,90
*       - Inflict POISON lasting 2-7 turns on the target with 90% chance
*     501,INFLICT,1.3,,BURN,,,,,100
*       - Burn the target for 1-3 turns with 100% chance
*
*   [2.6]: Relieve Actions
*     600,RELIEVE,,POISON,,,,,100
*       - Relieve the target of poison with 100% chance
* --------
*
* [3]: When building a Skill with several actions, take into account the effects
*      of just the skill processing (example, chance of skill missing) for the
*      actions to have a desired effect.
*
*      Example, lifesteal requires two actions
*      to complete: 1 to remove VITA from target, 1 to add VITA to user: it
*      would be irregular for once action to miss and the other to hit, so
*      the actions should take place at 100% and the skill hit rate set to
*      the desired value.
*
*      Another example, a sword strike may be accompanies by an infliction
*      such as poison, but the sword strike may have a high rate of accuracy
*      whereas the rate of poison is diminished. In this case, the damage
*      action should be 100% and the poison action lower, and the rate of
*      skill hit assigned accordingly in the skill.
*
* TODO
* ----
* [11-23-14] - Retest everything/conventionivize
* [11-23-14] - All types of action examples
* [11-23-14] - Cut through target's equipment
* [11-23-14] - Keywords for AILMENT types? (ex. NEG / POS / ALL / BUFF) ??????
*******************************************************************************/
#ifndef ACTION_H
#define ACTION_H

#include <iostream>

#include "EnumFlags.h"
#include "Helpers.h"

class AttributeSet;

/* ActionFlags for storing the type of Action */
ENUM_FLAGS(ActionFlags)
enum class ActionFlags
{
  DAMAGE    = 1 << 0,  /* DAMAGE the VITA of the target */
  ALTER     = 1 << 1,  /* ALTER an attribute by a given value */
  INFLICT   = 1 << 2,  /* INFLICT a given ailment for a duration */
  RELIEVE   = 1 << 3,  /* RELIEVE a given ailment */
  ASSIGN    = 1 << 4,  /* ASSIGN an attribute to a given value */
  REVIVE    = 1 << 5,  /* REVIVE un-KOs target with base HP */
  BASE_PC   = 1 << 6,  /* True if the base is a % value and not an abs one */
  VARI_PC   = 1 << 7,  /* True if variance is a % value and not an abs one */
  FLIP_ATTR = 1 << 8,  /* Flip the user/target attributes for assign/alter? */
  VALID     = 1 << 9  /* The validity of the action */
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
  NIHIL      = 1 << 6,
  LUCK       = 1 << 7,
  CYBERNETIC = 1 << 8,
  ARMOR      = 1 << 9  // TODO [11-23-14] - Cut through target's equipment
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

  /* Enumerated attributes which the action may alter */
  Attribute target_attribute;
  Attribute user_attribute;

  /* Enumerated infliction which the action may inflict/relieve */
  Infliction ailment;

  /* Base value (amt. or pc in flag) by which to change the attribute by */
  int32_t base;

  /* Chance of the action occuring */
  float chance;

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
  static const int32_t  kDEFAULT_MIN; /* Default turn minimum for ailment */
  static const int32_t  kDEFAULT_MAX; /* Default turn maximum for ailment */
  static const char     kDELIMITER;   /* The delimiter for string parsing */
  static const char     kDELIMITER_2; /* The secondary delimiter */
  static const int32_t  kMAX_BASE_PC; /* Max % value for Base change */
  static const uint32_t kMAX_VARIANCE_PC; /* Max % value for Variance */
  static const int32_t  kUNSET_ID;  /* ID for a default Action object */

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Does the check against the indicated ignore flag */
  bool atkDefFlag(IgnoreFlags test_flag, bool attack = true);

  /* Returns the ignore flags in a period delimited list */
  std::string ignoreToStr(bool attack = true);

  /* Sub-method for parsing the string containing the ailment */
  bool parseAilment(const std::string &ailm_parse);

  /* Sub-method for parsing the string containing the action kewyord */
  bool parseActionKeyword(const std::string &action_keyword);

  /* Sub-method for parsing the string containing the affected attribute */
  bool parseAttribute(const std::string &attr_parse, const bool &target);

  /* Parse the chance occuring */
  void parseChance(const float &parse_chance);

  /* Sub-method for parsing the ignore atk and ignore def flags */
  void parseIgnoreFlags(IgnoreFlags& flag_set, const std::string &flags);

  /* Emits a warning from a given string and turns off the VALID flag */
  void parseWarning(const std::string &warning, const std::string &raw);

  /* Assigns an infliction duration returning the validity of the assignment */
  bool setDuration(const int &min_value, const int &max_value);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Prints out the state of the current Action */
  void print(const bool &print_action = true, const bool &print_ignore = false);

  /* Evaluates a given action flag */
  bool actionFlag(ActionFlags test_flag);

  /* Evaluates a given ignore atk flag */
  bool atkFlag(IgnoreFlags test_flag);

  /* Clears the class info */
  void clear();

  /* Evaluates a given ignore def flag */
  bool defFlag(IgnoreFlags test_flag);

  /* Returns the Attribute the action may alter/assign for user and target */
  Attribute getUserAttribute() const;
  Attribute getTargetAttribute() const;

  /* Returns the infliction the action may inflict/relieve */
  Infliction getAilment() const;

  /* Returns the base change */
  int getBase() const;

  /* Return the chance of the action occuring */
  float getChance() const;

  /* Returns the ID of the Action */
  int getID() const;

  /* Methods for returning the min and max portions of the duration */
  int getMin() const;
  int getMax() const;

  /* Returns the variance of the Action */
  uint32_t getVariance() const;

  /* The output string (to store in file) */
  std::string outputString();

  /* Main function for parsing the raw string */
  bool parse(const std::string &raw);

  /* Sets an action flag */
  void setActionFlag(ActionFlags set_flag, bool set);

  /* Sets the ailment and duration */
  void setAilment(Infliction ailment);
  bool setAilmentDuration(int min, int max);

  /* Sets the attributes */
  void setAttributeTarget(Attribute target);
  void setAttributeUser(Attribute user);

  /* Sets the base value and variance (amount or pc in flag) to change
   * attribute by */
  void setBaseValue(int32_t value, bool percent = false);
  void setBaseVariance(uint32_t variance, bool percent = false);

  /* Sets the chance of the action occuring */
  bool setChance(float chance);

  /* Sets the ID */
  void setID(int32_t id);

  /* Sets the ignore flags for attack and defense */
  void setIgnoreAttack(IgnoreFlags flag, bool set);
  void setIgnoreDefense(IgnoreFlags flag, bool set);
};

#endif //ACTION_H
