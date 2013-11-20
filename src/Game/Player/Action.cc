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
*   - IGNORE DEF ELEMENTs - a list of valid elements which when set, will not
*                           include the action target's corresponding defensive
*                           elemental statistics into battle calculations.
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

const int Action::kDEFAULT_ID = INT_MAX;
const char Action::kDELIMITER = ',';

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a default (invalid) Action object
 *
 * Inputs: none
 */
Action::Action()
    : attribute(Attribute::NONE),
      ailment(Infliction::NONE),
      base(0),
      id(kDEFAULT_ID),
      min_duration(0),
      max_duration(0),
      variance(0)
{

}

Action::Action(const std::string &raw)
    : attribute(Attribute::NONE),
      ailment(Infliction::NONE),
      base(0),
      id(kDEFAULT_ID),
      min_duration(0),
      max_duration(0),
      variance(0)
{
  if (parse(raw))
  {
    // VALID	
  }

  // INVALID
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

  Helpers::split(raw, kDELIMITER, sub_strings);

  if (sub_strings.size() == 11)
  {

  }

}

void Action::setAttribute(Attribute new_attribute)
{
  attribute = new_attribute;
}

void Action::setBase(const int &value)
{
  base = value;
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

void Action::setID(const int &value)
{
  id = value;
}

void Action:setFlags(ActionFlags new_flags)
{
  flags = new_flags;
}

void Action::setIgnoreAtk(Element new_ignore_atk)
{
  ignore_atk = new_ignore_atk;
}

void Action::setIgnoreDef(Element new_ignore_def)
{
  ignore_def = new_ignore_def;
}
 
void Action::setVariance(const int &new_variance)
{
  variance = new_variance;
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
  return flags;
}

int Action::getID()
{
  return id;
}

Element Action::getIgnoreAtk()
{
  return ignore_atk;
}

Element Action::getIgnoreDef()
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