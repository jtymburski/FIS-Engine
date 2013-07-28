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
  static const uint kMAX_SKILLS;
  static const uint kMAX_LEVEL;

  /* Vectors holding the skills and the level they're available */
  QVector<Skill*> skills;
  QVector<ushort> skills_available;
  QVector<bool> enabled;

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Methods to add skills */
  bool addSkill(Skill* skill, ushort req_level = 1);
  bool addSkills(QVector<Skill*> new_skills, QVector<ushort> new_levels);

  /* Calculates the lowest level of a skill contained in the set */
  int calcLowestLevel(QString skill_name);

  /* Calculates the end number of unique skills before an append of new ones */
  uint calcUniqueSkills(QVector<Skill*> new_skills);

  /* Cleans up vectors */
  void cleanUp();

  /* Method to print out the info of the Skill Set */
  void printInfo();

  /* Methods to remove skills */
  bool removeSkill(int index);
  bool removeSkill(QString name);

  /* Sorts the skill vector */
  bool sortSkills(QString sort_type = "NAME");

  /* Creates and returns a vector of names of skills */
  QVector<QString> getNames();

  /* Methods to obtain skills */
  QVector<Skill*> getSkills();
  QVector<ushort> getSkillLevels();
  Skill* getSkill(int index);
  ushort getSkillLevel(int index);

  /* Methods to enable/disable skills */
  void setSkillState(int index, bool state = true);
  bool setSkillState(QString name, bool state = true);

/*=============================================================================
  * SIGNALS
  *============================================================================*/
signals:
    /* Signal emitted when the set would be full */
    void fullSet(QString reasons);

    /* Signl emitted when the set is sorted */
    void sorted();
};


#endif
