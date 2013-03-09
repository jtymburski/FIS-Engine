/*******************************************************************************
* Class Name: Race
* Date Created: Sunday, October 28th, 2012
* Inheritance: None
* Description: Implementation for Race. Every Person has a Race, which
*              is equivalent to their "Battle Class". This holds their starting
*              and ending values for attributes (In an attribute set), and a
*              set of skills which will become available to the person at
*              various levels.
* // TODO: SKILL SORTING [03-05-13]
* // TODO: ADD Race FLAGS [03-05-13]
*******************************************************************************/

#include "Game/Player/Race.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor object. Constructs a Race given a string and
 *             a full set of base stat values. None of the Race constructors
 *             set the description of the Race.
 *
 * Inputs: QString - name of the Race
 *         AttributeSet - statistic set to add to the Race
 */
Race::Race(QString name, AttributeSet attributes, SkillSet* skills)
{
  setName(name);
  setAttrSet(attributes);
  setSkillSet(skills);
  if (skills != NULL)
    cleanUp();
}

/*
 * Description: Default Race object constructor. Constructs a Race with
 *              a default set of attributes.
 *
 * Inputs: QString - name of the Race
 */
Race::Race(QString name)
{
  setName(name);
  skill_set = NULL;
  cleanUp();
}

/*
 * Description: Destroys a Race object
 */
Race::~Race()
{
  delete skill_set;
  skill_set = NULL;
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Cleans up the skill vector by calling its clean up function,
 *              and also ensures all the maximum attribute values are within
 *              the limits specified by the consts
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Race::cleanUp()
{
  getSkillSet()->cleanUp();
  if (stat_set.getMax("VITALITY") > kMAX_VITALITY)
      stat_set.setMax("VITALITY", kMAX_VITALITY);
  if (stat_set.getMax("QUANTUM DRIVE") > kMAX_QD)
      stat_set.setMax("QUANTUM DRIVE", kMAX_QD);
  for (int i = stat_set.getIndex("PHAG"); i <= stat_set.getIndex("PHFD"); i++)
      if (stat_set.getMax(i) > kMAX_PHYSICAL)
          stat_set.setMax(i, kMAX_PHYSICAL);
  for (int i = stat_set.getIndex("THAG"); i <= stat_set.getIndex("NIFD"); i++)
      if (stat_set.getMax(i) > kMAX_ELEMENTAL)
          stat_set.setMax(i, kMAX_ELEMENTAL);
  for (int i = stat_set.getIndex("NIFD") + 1; i < stat_set.getSize(); i++)
      if (stat_set.getMax(i) > kMAX_SPECIAL)
          stat_set.setMax(i, kMAX_SPECIAL);
}

/*
 * Description: Returns the description of the Race
 *
 * Inputs: none
 * Output: QString - description of the Race
 */
QString Race::getDescription()
{
  return description;
}

/*
 * Description: Returns the name of the Race
 *
 * Inputs: none
 * Output: QString - name of the Race
 */
QString Race::getName()
{
  return name;
}

/*
 * Description: Returns the set of attributes
 *
 * Inputs: none
 * Output: AttributeSet - attribute set of the Race
 */
AttributeSet Race::getAttrSet()
{
  return stat_set;
}

/*
 * Description: Returns the SkillSet*
 *
 * Inputs: none
 * Output: SkillSet* - the skills of the Race
 */
SkillSet* Race::getSkillSet()
{
  return skill_set;
}

/*
 * Description: Sets the Race bonus value of the thermal attack stat
 *
 * Inputs: QString - string of the Race description
 * Output: none
 */
void Race::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the name of the Race
 *
 * Inputs: QString - new name to be set
 * Output: none
 */
void Race::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the AttributeSet of the Race and calls clean up
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Race::setAttrSet(AttributeSet new_stat_set)
{
  stat_set = new_stat_set;
}

/*
 * Description: Sets the SkillSet* and calls clean up
 *
 * Inputs: SkillSet* - pointer to a new skill set
 * Output: none
 */
void Race::setSkillSet(SkillSet* new_skill_set)
{
  skill_set = new_skill_set;
}


