/*******************************************************************************
* Class Name: SkillSet [Declaration]
* Date Created: November 23rd, 2013
* Inheritance: None
* Description: A SkillSet is a container for a list of skills and the level
*              required to be usable. A SkillSet is contained in a BubbyFlavour,
*              Race, Category, Person, Equipment, Item. A SkillList can be
*              sorted by level, by cost of the skill, by the assigned point
*              value (for AI), by the elements the skill uses, etc.
*              
*
* Notes
* -----
*
* [1]: A SkillSet may also have individually enabled/disabled Skills.
*
* TODO
* ----
* - Sorting [11-23-13]
* - Testing [11-23-13]
* - Conventions [11-23-13]
*******************************************************************************/

#ifndef SKILLSET_H
#define SKILLSET_H

#include <algorithm> /* std::sort */
#include <deque>
#include <vector>

#include "Game/Player/Skill.h"

struct SkillSetElement
{
  Skill* skill;

  uint level_available;

  bool enabled;
};

class SkillSet
{
public:

  SkillSet();

  SkillSet(const SkillSet &source);

  SkillSet(const Skill* skill, const uint &level = 1);

  SkillSet(const std::vector<Skill*> &skills, const std::vector<uint> &levels); 

  ~SkillSet();

  /* ------------ Constants --------------- */
  const bool   kENABLED_DEFAULT;
  const size_t kMAX_SKILLS;
  const uint   kMAX_UNLOCK_LEVEL;

private:

  std::deque<SkillSetElement> skill_elements;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
 private:
   uint calcLowestLevel(const &uint skill_id);

   static uint calcUniques(const std::deque<SkillSetElements> &skill_elements);
   
   void cleanUp();

   void copySelf(const SkillSet &source);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  bool addSkill(const Skill* skill, const uint &req_level = 1, 
  	            const bool &enabled = true);

  bool addSkills(const std::vector<Skill*> skills, 
  	             const std::vector<uint> &req_levels,
  	             const std::vector<bool> &enabled);

  bool addSkills(const std::deque<SkillSetElements> &skill_elements);

  void clear();

  bool removeSkill(const uint &index);

  bool removeSkill(const uint &id);

  //TODO
  // sort()

  std::vector<bool> getAllEnabled();

  std::deque<SkillSetElements> getElements();

  bool getEnabled(const uint &index);

  std::string getName(const uint &index);

  std::vector<std::string> getNames();

  uint getLevel(const uint &index);

  std::vector<uint> getLevels();

  bool setState(const uint &index, const bool &state = true);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  SkillSet& operator=(const SkillSet& source);

  SkillSet& operator+=(const SkillSet &rhs);

  SkillSet& operator-=(const SkillSet &rhs);
};

inline SkillSet operator+(SkillSet lhs, const SkillSet &rhs);

inline SkillSet operator-(SkillSet lhs, const SkillSet &rhs);


#endif //SKILLSET_H