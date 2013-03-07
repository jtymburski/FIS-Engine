/*******************************************************************************
* Class Name: Category
* Date Created: Sunday, October 28th, 2012
* Inheritance: None
* Description: Implementation for Category. Every Person has a Category, which
*              is equivalent to their "Battle Class". This holds their starting
*              and ending values for attributes (In an attribute set), and a
*              set of skills which will become available to the person at
*              various levels.
* // TODO: SKILL SORTING [03-05-13]
* // TODO: ADD CATEGORY FLAGS [03-05-13]
*******************************************************************************/

#include "Game/Player/Category.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor object. Constructs a Category given a string and
 *             a full set of base stat values. None of the Category constructors
 *             set the description of the Category.
 *
 * Inputs: QString - name of the Category
 *         AttributeSet - statistic set to add to the category
 */
Category::Category(QString name, AttributeSet attributes, SkillSet* skills)
{
  setName(name);
  setAttrSet(attributes);
  if (skills != NULL)
      setSkillSet(skills);
  cleanUp();
}

/*
 * Description: Default Category object constructor. Constructs a category with
 *              a default set of attributes.
 *
 * Inputs: QString - name of the category
 */
Category::Category(QString name)
{
  setName(name);
  skill_set = NULL;
  cleanUp();
}

/*
 * Description: Destroys a Category object
 */
Category::~Category()
{
  delete skill_set;
  skill_set = NULL;
}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

void Category::cleanUp()
{
  // getSkillSet()->cleanUp();
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
 * Description: Returns the description of the Category
 *
 * Inputs: none
 * Output: QString - description of the Category
 */
QString Category::getDescription()
{
  return description;
}

/*
 * Description: Returns the name of the Category
 *
 * Inputs: none
 * Output: QString - name of the Category
 */
QString Category::getName()
{
  return name;
}

/*
 * Description: Returns the set of attributes
 *
 * Inputs: none
 * Output: AttributeSet - attribute set of the Category
 */
AttributeSet Category::getAttrSet()
{
  return stat_set;
}

/*
 * Description: Returns the SkillSet*
 *
 * Inputs: none
 * Output: SkillSet* - the skills of the Category
 */
SkillSet* Category::getSkillSet()
{
  return skill_set;
}

/*
 * Description: Sets the Category bonus value of the thermal attack stat
 *
 * Inputs: QString - string of the Category description
 * Output: none
 */
void Category::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the name of the Category
 *
 * Inputs: QString - new name to be set
 * Output: none
 */
void Category::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the AttributeSet of the Category
 *
 * Inputs: AttributeSet - new stat set
 * Output: none
 */
void Category::setAttrSet(AttributeSet new_stat_set)
{
  stat_set = new_stat_set;
  cleanUp();
}

/*
 * Description: Sets the SkillSet of the Category
 *
 * Inputs: SkillSet* - pointer to the new skill set
 * Output: none
 */
void Category::setSkillSet(SkillSet* new_skill_set)
{
  skill_set = new_skill_set;
  cleanUp();
}
