/*******************************************************************************
* Class Name: SkillSet [Implementation]
* Date Created: November 23rd, 2013
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/SkillSet.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const bool   SkillSet::kENABLED_DEFAULT  = true;
const size_t SkillSet::kMAX_SKILLS       =  400;
const uint   SkillSet::kMAX_UNLOCK_LEVEL =  127;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
SkillSet::SkillSet() {}

/*
 * Description:
 *
 * Inputs:
 */
SkillSet::SkillSet(const SkillSet &source)
{
  copySelf(source);
}

/*
 * Description:
 *
 * Inputs:
 */
SkillSet::SkillSet(const Skill* skill, const uint &level = 1)
{
  addSkill(skill, level, kENABLED_DEFAULT);
}

/*
 * Description:
 *
 * Inputs:
 */
SkillSet::SkillSet(const std::vector<Skill*> &skills, const std::vector<uint> &levels)
{
  std::vector<bool> enabled_values(levels.size(), kENABLED_DEFAULT);
  addSkills(skills, levels, enabled_values);
}

/*
 * Description:
 */
SkillSet::~SkillSet() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

uint SkillSet::calcLowestLevel(const &uint skill_id)
{

}

uint SkillSet::calcUniques(const std::deque<SkillSetElements> &skill_elements)
{

}
   
void SkillSet::cleanUp()
{

}

void SkillSet::copySelf(const SkillSet &source)
{

}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool SkillSet::addSkill(const Skill* skill, const uint &req_level = 1, 
  	                    const bool &enabled = true)
{

}

bool SkillSet::addSkills(const std::vector<Skill*> skills, 
  	                     const std::vector<uint> &req_levels,
  	                     const std::vector<bool> &enabled)
{

}

bool SkillSet::addSkills(const std::deque<SkillSetElements> &skill_elements)
{

}

void SkillSet::clear()
{

}

bool SkillSet::removeSkill(const uint &index)
{

}

bool SkillSet::removeSkill(const uint &id)
{

}

//TODO
// sort()

std::vector<bool> SkillSet::getAllEnabled()
{

}

std::deque<SkillSetElements> SkillSet::getElements()
{

}

bool SkillSet::getEnabled(const uint &index)
{

}

std::string SkillSet::getName(const uint &index)
{

}

std::vector<std::string> SkillSet::getNames()
{

}

uint SkillSet::getLevel(const uint &index)
{

}

std::vector<uint> SkillSet::getLevels()
{

}

bool SkillSet::setState(const uint &index, const bool &state = true)
{

}

/*=============================================================================
 * OPERATOR
 *============================================================================*/

 SkillSet& SkillSet::operator=(const SkillSet& source)
 {

 }

SkillSet& SkillSet::operator+=(const SkillSet &rhs)
{

}

SkillSet& SkillSet::operator-=(const SkillSet &rhs)
{

}

inline SkillSet operator+(SkillSet lhs, const SkillSet &rhs)
{

}

inline SkillSet operator-(SkillSet lhs, const SkillSet &rhs)
{

}