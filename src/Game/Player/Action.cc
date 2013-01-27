/******************************************************************************
* Class Name: Action Implementation
* Date Created: December 2nd, 2012
* Inheritance: 
* Description: Display information on the screen about battle events
*
* TODO: CONSTRUCTORS TO BE FINISHED
* TODO: ACTION CATEGORY? (+ getFunction) [12-02-12]
*
* Notes: - Possibility to target person with most skills
*     - num_targets could be that value or less based on the number of 
*     legal targets or user option
*
* Parse layout: IF{[Element Type], [Greater/Lesser Qualifier], [Value]} : 
* [Upper/Lower or GIVE/TAKE] , [Skill Element 1] , [Skill Element 2] ,
* [Ignore Attack], [Ignore Atk Elm] . [Ignore Atk Elm 2] . [Etc.] , 
* [Ignore Def Elm 1] . [Ignore Def Elm 2] . [Etc.] , [Base Damage Value] , 
* [Variance] , [Chance]
*
* while (line available)
* { 
* string 1: string 2
* if (switch) boolean valid
* effect
* }
*
******************************************************************************/

#include "Game/Player/Action.h"

/* Description: Constructor for an action object
 *
 * Inputs: none
 */
Action::Action(QWidget* pointer)
{
}

/* Description: Annihilates an action object
 */
Action::~Action()
{
}

/* Evaluates the FRIENDLY flag */
bool Action::isFriendly()
{
  return FRIENDLY;
}

/* Attempts to perform the action as defined by sequence, return false if it is invalid */
bool Action::use()
{
}

/* Returns the brief description */
QString Action::getBriefDescription()
{
  return brief_description;
}

/* Returns the category (for menu grouping) */
/* TODO: Category data member? [12-02-12] */
QString Action::getCategory()
{
}

/* Returns the actions cost */
unsigned int Action::getCost()
{
  return cost;
}

/* Returns the description */
QString Action::getDescription()
{
  return description;
}

/* Returns the action type */
ActionType Action::getMode()
{
  return type;
}

/* Returns the name */
QString Action::getName()
{
  return name;
}

/* Returns the number of targets this will require */
unsigned int Action::getNumTargets()
{
  return num_targets;
}

/* Sets the action's cost */
void Action::setCost(unsigned int new_cost)
{
  cost = new_cost;
}

/* Sets the action's mode */
void Action::setMode(ActionType new_type)
{
  type = new_type;
}

/* Sets the action name */
void Action::setName(QString new_name)
{
  name = new_name;
}

/* Sets the number of targets */
void Action::setNumTargets(unsigned int new_number_targets)
{
  num_targets = new_number_targets;
}
