/******************************************************************************
* Class Name: SkillSet
* Date Created: March 6th, 2013
* Inheritance:
* Description: Container class for Skills. Contains all methods needed ot hold
*              a list of skills and levels required to use them. Removes
*              duplicates, etc.
// TODO: Remove duplicates from Skills. [03-09-13]
******************************************************************************/
#include "Game/Player/SkillSet.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default SkillSet constructor
 *
 * Inputs: none
 */
SkillSet::SkillSet() {}

/*
 * Description: Constructs a SkillSet object given a single Skill* and a value
 *              at which point that skill becomes available (defaulted to 1)
 *
 * Inputs: Skill* - pointer to skill to be added
 *         ushort - value that the skill becomes available
 */
SkillSet::SkillSet(Skill *skill, ushort level)
{
  addSkill(skill, level);
  cleanUp();
}

/*
 * Description: Constructs a SkillSet object given a vector of skill pointers
 *              and a vector of ushorts describing at which level those skills
 *              become available.
 *
 * Inputs: QVector<Skill*> - the vector of skill pointers
 *         QVector<ushort> - corresponding vector of levels required for skills
 */
SkillSet::SkillSet(QVector<Skill*> skills, QVector<ushort> levels)
{
  // assert(skills.size() == levels.size());
  addSkills(skills, levels);
  cleanUp();
}

/*
 * Description: Annihilates a SkillSet object
 */
SkillSet::~SkillSet() {}

/*=============================================================================
 * Functions
 *============================================================================*/

/*
 * Description: Adds a skill given a new pointer an level required
 *
 * Inputs: Skill* - pointer to skill to be added
 *         ushort - level that the skill is required at
 * Output: bool   - TRUE if successful
 */
bool SkillSet::addSkill(Skill* new_skill, ushort req_level)
{
  if (skills_available.size() < kMAX_SKILLS)
  {
    skills.append(new_skill);
    skills_available.append(req_level);
    cleanUp();
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Adds multiple skills to the SkillSet, given vectors
 *
 * Inputs: QVector<Skill* > - vector of skills to be added
 *         QVector<ushort>  - vector of levels corresponding
 * Output: bool             - TRUE is successful
 */
bool SkillSet::addSkills(QVector<Skill* > new_skills,
                               QVector<ushort> new_levels)
{
  if (new_skills.size() != new_levels.size())
    return FALSE;
  if (new_skills.size() + skills_available.size() <= kMAX_SKILLS)
  {
    for (int i = 0; i < new_skills.size(); i++)
    {
      skills.append(new_skills.at(i));
      skills_available.append(new_levels.at(i));
    }
    cleanUp();
    return TRUE;
  }
  return FALSE;
}

/*
 * Description: Cleans up the SkillSet by calling Sort, then by removing
 *              duplicates (by taking the lowest level value for skills of the
 *              same name).
 *
 * Inputs: none
 * Output: none
 */
void SkillSet::cleanUp()
{
  if (skills.size() > 1)
  {
    sortSkills();

    // TODO: Remove duplicates from Skills. [03-09-13]
  }
}

/*
 * Description: Removes a skill from the SkillSet, given an index
 *
 * Inputs: int - index of the skill to be removed
 * Output: bool - TRUE is removal was successful
 */
bool SkillSet::removeSkill(int index)
{
  if (index > -1 && index < skills.size())
    return removeSkill(skills.at(index)->getName());
  return FALSE;
}

/*
 * Description: Removes a skill from the SkillSet given a name.
 *
 * Inputs: QString - name of the skill to be removed.
 * Output: bool -  TRUE if the removal was successful.
 */
bool SkillSet::removeSkill(QString name)
{
  for (int i = 0; i < skills.size(); i++)
  {
    if (skills.at(i)->getName() == name)
    {
      delete skills.at(i);
      skills[i] = NULL;
      skills.remove(i);
      skills_available.remove(i);
      return TRUE;
    }
  }
  return FALSE;
}

/*
 * Description: Sorts the skills, by ascending name or level
 *
 * Inputs: QString - type of sort to be performed
 * Output: bool    - TRUE if sort took place
 */
bool SkillSet::sortSkills(QString sort_type)
{
  if (!(sort_type == "NAME" && sort_type == "LEVEL") || skills.size() < 2)
    return FALSE;

  QVector<Skill*> temp_skills;
  QVector<ushort> temp_levels;
  int size = skills_available.size();

  if (sort_type == "NAME")
  {
    for (int i = 0; i < size; i++)
    {
      QString min = "ZZZZ";
      int index   = -1;
      for (int j = 0; j < skills.size(); j++)
      {
        if (skills.at(j)->getName() < min)
        {
          min = skills.at(j)->getName();
          index = j;
        }
      }

      temp_skills.append(skills.at(index));
      temp_levels.append(skills_available.at(index));
      removeSkill(index);
    }
  }
  else if (sort_type == "LEVEL")
  {
    for (int i = 0; i < size; i++)
    {
      int min   = skills_available.at(0);
      int index = 0;
      for (int j = 1; j < skills_available.size(); j++)
      {
        if (skills_available.at(j) > min)
        {
          min = skills_available.at(j);
          index = j;
        }
      }

      temp_skills.append(skills.at(index));
      temp_levels.append(skills_available.at(index));
      removeSkill(index);
    }
  }

  /* Cleanup */
  qDeleteAll(skills.begin(), skills.end());
  skills.clear();

  skills = temp_skills;
  qDeleteAll(temp_skills.begin(), skills.end());
  temp_skills.clear();
  skills_available = temp_levels;
  return TRUE;
}

/*
 * Description: Returns the vector of skills
 *
 * Inputs: none
 * Output: QVector<Skill* > - the skills of the SkillSet
 */
QVector<Skill*> SkillSet::getSkills()
{
  return skills;
}

/*
 * Description: Returns the vector corresponding to levels required for skills
 *
 * Inputs: none
 * Output: QVector<ushort> - the levels required for skills
 */
QVector<ushort> SkillSet::getSkillLevels()
{
  return skills_available;
}

/*
 * Description: Returns a skill pointer at an index in the vector
 *
 * Inputs: int - index to be checked
 * Output: Skill* - pointer to skill at given index.
 */
Skill* SkillSet::getSkill(int index)
{
  return skills.at(index);
}

/*
 * Description: Returns the level required of a skill at a certain index.
 *
 * Inputs: int - index
 * Output: ushort - level required of the skill at a given index.
 */
ushort SkillSet::getSkillLevel(int index)
{
  return skills_available.at(index);
}
