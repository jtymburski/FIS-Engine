/*******************************************************************************
* Class Name: Action [Implementation]
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
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Action.h"

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/

const int  Action::kDEFAULT_ID  = INT_MAX;
const char Action::kDELIMITER   = ',';
const char Action::kDELIMITER_2 = '.';

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a default (invalid) Action object
 *
 * Inputs: none
 */
Action::Action()
    : action_flags(static_cast<ActionFlags>(0)),
      attribute(Attribute::NONE),
      ailment(Infliction::NONE),
      base(0),
      id(kDEFAULT_ID),
      ignore_atk(static_cast<IgnoreFlags>(0)),
      ignore_def(static_cast<IgnoreflagS>(0)),
      min_duration(0),
      max_duration(0),
      variance(0)
{
  action_flags &= static_cast<ActionFlags>(0);
  action_flags &= static_cast<ActionFlags>(0);
}

Action::Action(const std::string &raw)
    : action_flags(static_cast<ActionFlags>(0)),
      attribute(Attribute::NONE),
      ailment(Infliction::NONE),
      base(0),
      id(kDEFAULT_ID),
      ignore_atk(static_cast<IgnoreFlags>(0)),
      ignore_def(static_cast<IgnoreflagS>(0)),
      min_duration(0),
      max_duration(0),
      variance(0)
{
  if (parse(raw))
    action_flags |= ActionFlags::VALID;
}

/*
 * Description: Annihilates an action object
 */
Action::~Action() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Action::parse(const std::string &raw)
{
  std::vector<std::string> sub_strings;
  bool valid_action = true;

  Helpers::split(raw, kDELIMITER, sub_strings);

  if (sub_strings.size() == 8)
  {
  	/* Parse the ID */
    id = std::stoi(sub_string.at(0));

  	/* Parse the initial action keyword */
    valid_action &= parseActionKeyword(sub_strings.at(1));

    /* ALTER and ASSIGN keywords relate to attributes */
    if (action_flags == ActionFlags::ALTER || 
    	action_flags == ActionFlags::ASSIGN)
    {
      valid_action &= parseAttribute(sub_strings.at(2));
    }

    /* INFLICT and RELIEVE keywords relate to ailments */
    else if (action_flags == ActionFlags::INFLICT || 
    	     action_flags == ActionFlags::RELIEVE)
    {
      valid_action &= parseAilment(sub_strings.at(3));
    }
    
    /* Parse min & max durations */
    if (sub_strings.at(4) != "")
    {
      std::vector<std::string> duration;

      Helpers::split(sub_string.at(4), kDELIMITER_2, duration);

      valid_action &= setDuration(std::stoi(duration.at(0)), 
      	                          std::stoi(duration.at(1)));
    }

    /* Parse ignore_atk flags */
    if (sub_strings.at(5) != "")
      parseIgnoreFlags(ignore_atk, sub_strings.at(5));

    /* Parse ignore def flags */
    if (sub_string.at(6) != "")
      parseIgnoreFlags(ignore_def, sub_strings.at(6));

    /* Parse base change and variance */
    std::vector<std::string> base_values;
    std::vector<std::string> variance_values;

    Helpers::split(sub_strings.at(7), kDELIMITER_2, base_values);
    Helpers::split(sub_strings.at(8), kDELIMITER_2, variance_values);
    
    if (base_values.at(0) == "PC")
      action_flags |= Flags::BASE_PC;
    if (variance_values.at(0) == "PC")
      action_flags |= ActionFlags::VARI_PC;

    base = std::stoi(base_values.at(1));
    variance = std::stoi(variance_values.at(1));
  }

  return valid_action;
}

//TODO: Boost enum string? [11-20-13]
bool Action::parseAilment(const std::string &ailm_parse)
{
  ailment = Ailment::POISON;

  return true;
}

bool Action::parseActionKeyword(const std::string &action_keyword)
{
  if (action_keyword == "ALTER")
    action_flags |= ActionFlags::ALTER;
  else if (action_keyword == "INFLICT")
    action_flags |= ActionFlags::INFLICT;
  else if (action_keyword == "RELIEVE")
    action_flags |= ActionFlags::RELIEVE;
  else if (action_keyword == "ASSIGN")
    action_flags |= ActionFlags::ASSIGN;
  else if (action_keyword == "REVIVE")
    action_flags |= ActionFlags::REVIVE;
  else
    return false;

  return true;
}

//TODO: Boost enum string? [11-20-13]
bool Action::parseAttribute(const std::string &attr_parse)
{
  attribute = Attribute::THAG;

  return true;
}

void Action::parseIgnoreFlags(IgnoreFlags& flag_set, const std::string &flags)
{
  std::vector<std::string> sub_strings;

  Helpers::split(flags, kDELIMITER2, sub_strings);

  for (std::string s : sub_strings)
  {
    if (s == "ALL" || s == "ELEMENTAL")
    {
      flag_set |= IgnoreFlags::PHYSICAL | IgnoreFlags::THERMAL    | 
                  IgnoreFlags::POLAR    | IgnoreFlags::PRIMAL     |
                  IgnoreFlags::CHARGED  | IgnoreFlags::CYBERNETIC |
                  IgnoreFlags::NIHIL;
    }
    if (s == "ELEMENTAL")
      flag_set &= ~IgnoreFlags::PHYSICAL;
    else if (s == "PHYSICAL")
      flag_set |= IgnoreFlags::PHYSICAL;
    else if (s == "THERMAL")
      flag_set |= IgnoreFlags::THERMAL;
    else if (s == "POLAR")
      flag_set |= IgnoreFlags::POLAR;
    else if (s == "PRIMAL")
      flag_set |= IgnoreFlags::PRIMAL;
    else if (s == "CHARGED")
      flag_set |= IgnoreFlags::CHARGED;
    else if (s == "CYBERNETIC")
      flag_set |= IgnoreFlags::CYBERNETIC;
    else if (s == "NIHIL")
      flag_set |= IgnoreFlags::NIHIL;
  }
}

bool Action::setDuration(const int &min_value, const int &max_value)
{
  if (max_value >= min_value)
  {
    min_duration = min_value;
    max_duration = max_value;
    return true;
  }

  min_duration = -1;
  max_duration = -1;

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

Attribute Action::getAttribute()
{
  return attribute;
}

Infliction Action::getAilment()
{
  return ailment;
}

int Action::getBase()
{
  return base;
}

ActionFlags Action::getFlags()
{
  return action_flags;
}

int Action::getID()
{
  return id;
}

IgnoreFlags Action::getIgnoreAtk()
{
  return ignore_atk;
}

IgnoreFlags Action::getIgnoreDef()
{
  return ignore_def;
}

int Action::getMin()
{
  return min_duration;
}

int Action::getMax()
{
  return max_duration;
}

float Action::getVariance()
{
  return variance;
}