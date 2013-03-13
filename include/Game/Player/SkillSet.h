/******************************************************************************
* Class Name: SkillSet
* Date Created: March 6th, 2013
* Inheritance:
* Description: Container class for Skills. Contains all methods needed ot hold
*              a list of skills and levels required to use them. Removes
*              duplicates, etc.
******************************************************************************/
#ifndef SKILLSET_H
#define SKILLSET_H

#include <QtGui>
#include <algorithm>
#include <QtAlgorithms>
#include "Game/Player/Skill.h"

class SkillSet
{
public:
  /* Constructs a skill set object */
  SkillSet();
  SkillSet(Skill* skill, ushort level = 1);
  SkillSet(QVector<Skill*> skills, QVector<ushort> levels);

  /* Annihilates a SkillSet object */
  ~SkillSet();

private:
  /* Maximum number of skills allowed */
  static const int kMAX_SKILLS = 100;

  /* Vectors holding the skills and the level they're available */
  QVector<Skill*> skills;
  QVector<ushort> skills_available;

public:
  /* Methods to add skills */
  const bool addSkill(Skill* skill, ushort req_level = 1);
  const bool addSkills(QVector<Skill*> new_skills, QVector<ushort> new_levels);

  /* Removes duplicates and cleans up vectors */
  void cleanUp();

  /* Methods to remove skills */
  const bool removeSkill(int index);
  const bool removeSkill(QString name);

  /* Sorts the skill vector */
  const bool sortSkills(QString sort_type = "NAME");

  /* Methods to obtain skills */
  QVector<Skill*> getSkills();
  QVector<ushort> getSkillLevels();
  Skill* getSkill(int index);
  ushort getSkillLevel(int index);
};


#endif
