/******************************************************************************
* Class Name: Action
* Date Created: Sunday, October 28th, 2012
* Inheritance:
* Description: Action is an element of a skill (skill can have up to 10 actions
* that will do alter stats, flip flags of a person, etc., or do damage to them.
*
* TODO: Constructors to be finished [02-17-13]
* TODO: Need to write action parser [02-17-13]
******************************************************************************/

#include "Game/Player/Action.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

Action::Action(QString raw_language, QWidget *pointer)
{
}

Action::~Action()
{
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

void Action::setId(int id)
{
  this->id = id;
}

void Action::setDuration(uint min, uint max)
{
  min_duration = min;
  max_duration = max;
}

void Action::setBaseChange(uint new_value)
{
  base_change = new_value;
}

void Action::setVariance(float new_value)
{
  if (new_value <= 1)
    variance = new_value;
  else
    variance = 1;
}

void Action::toggleIgnoreAtkFlag(IgnoreAttack flags)
{
  setIgnoreAtkFlag(flags, !getIgnoreAtkFlag(flags));
}

void Action::toggleIgnoreDefFlag(IgnoreDefense flags)
{
  setIgnoreDefFlag(flags, !getIgnoreDefFlag(flags));
}

void Action::toggleActionFlag(ActionType flags)
{
  setActionFlag(flags, !getActionFlag(flags));
}

uint Action::getId()
{
  return id;
}

const bool Action::getIgnoreAtkFlag(IgnoreAttack flags)
{
  return ignore_atk_flags.testFlag(flags);
}

const bool Action::getIgnoreDefFlag(IgnoreDefense flags)
{
  return ignore_def_flags.testFlag(flags);
}

const bool Action::getActionFlag(ActionType flags)
{
  return action_flags.testFlag(flags);
}

void Action::setIgnoreAtkFlag(IgnoreAttack flags, const bool set_value)
{
  (set_value) ? (ignore_atk_flags |= flags) : (ignore_atk_flags ^= flags);
}

void Action::setIgnoreDefFlag(IgnoreDefense flags, const bool set_value)
{
  (set_value) ? (ignore_def_flags |= flags) : (ignore_def_flags ^= flags);
}

void Action::setActionFlag(ActionType flags, const bool set_value)
{
  (set_value) ? (action_flags |= flags) : (action_flags ^= flags);
}
