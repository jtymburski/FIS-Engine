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
* - Testing [11-23-13]
* - Conventions [11-23-13]
*******************************************************************************/

#ifndef SKILLSET_H
#define SKILLSET_H

#include <algorithm> /* std::sort, std::find */
#include <deque>
#include <vector>

#include "Game/Player/Skill.h"
#include "EnumDb.h"
#include "Helpers.h"

struct SkillSetElement
{
  Skill* skill;

  size_t level_available;

  bool enabled;
};

class SkillSet
{
public:

  SkillSet();

  SkillSet(const SkillSet &source);

  SkillSet(Skill* skill, const uint32_t &level);

  SkillSet(const std::vector<Skill*> &skills, const std::vector<uint32_t> &levels); 

  ~SkillSet();

private:

  std::deque<SkillSetElement> skill_elements;

  /* ------------ Constants --------------- */
  static const bool   kENABLED_DEFAULT;  /* Are skills enabled by default? */
  static const size_t kMAX_SKILLS;       /* ? Problems with adding mass skills */
  static const size_t kMIN_UNLOCK_LEVEL; /* Minimum level a skill can be unlocked at */
  static const size_t kMAX_UNLOCK_LEVEL; /* Maximum level a skill can be unlocked at */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
 private:
   uint32_t calcLowestLevel(const uint32_t &skill_id);

   static std::deque<SkillSetElement> 
                calcUniques(const std::deque<SkillSetElement> &check_elements);
   
   void cleanUp();

   void copySelf(const SkillSet &source);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  bool addSkill(Skill* skill, const uint32_t &req_level, 
  	            const bool enabled = true);

  bool addSkills(const std::vector<Skill*> skills, 
  	             const std::vector<uint32_t> &req_levels,
  	             const std::vector<bool> &enabled);

  bool addSkills(const std::deque<SkillSetElement> &new_elements);

  void clear();

  void print(const bool simple = false);

  bool rSkillIndex(const uint32_t &index);

  bool rSkillID(const uint32_t &id);

  void sort(const SkillSorts &sort_type, bool ascending = true);

  std::vector<bool> getAllEnabled();

  std::deque<SkillSetElement> getElements(const uint32_t &at_level);

  bool getEnabled(const uint32_t &index);

  int getIndexOfID(const uint32_t &id);

  std::string getName(const uint32_t &index);

  std::vector<std::string> getNames();

  uint32_t getLevel(const uint32_t &index);

  std::vector<uint32_t> getLevels();

  bool setState(const uint32_t &index, const bool &state = true);

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