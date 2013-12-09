/*******************************************************************************
* Class Name: SkillSet [Implementation]
* Date Created: November 23rd, 2013
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]: A SkillSet may also have individually enabled/disabled Skills.
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/SkillSet.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const bool   SkillSet::kENABLED_DEFAULT  =  true;
const size_t SkillSet::kMAX_SKILLS       = 10000;
const size_t SkillSet::kMAX_UNLOCK_LEVEL =   127;
const size_t SkillSet::kMIN_UNLOCK_LEVEL =     1;

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
SkillSet::SkillSet(Skill* skill, const uint32_t &level)
{
  addSkill(skill, level, kENABLED_DEFAULT);
}

/*
 * Description:
 *
 * Inputs:
 */
SkillSet::SkillSet(const std::vector<Skill*> &skills, const std::vector<uint32_t> &levels)
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

uint32_t SkillSet::calcLowestLevel(const uint32_t &skill_id)
{
  uint32_t min_level = kMAX_UNLOCK_LEVEL;

  for (SkillSetElement element : skill_elements)
    if (element.skill->getID() == static_cast<int>(skill_id))
      if (element.level_available < min_level)
        min_level = element.level_available;
  
  return min_level;
}

//STATIC
std::deque<SkillSetElement> SkillSet::calcUniques(const std::deque<SkillSetElement> &check_elements)
{
  std::vector<uint32_t> uniques;
  std::deque<SkillSetElement> unique_elements;

  for (SkillSetElement element : check_elements)
  {
    const uint32_t &curr_id = element.skill->getID();

    if (std::find(uniques.begin(), uniques.end(), curr_id) != uniques.end())
    {
      uniques.push_back(curr_id);
      unique_elements.push_back(element);
    }
  }

  return unique_elements;
}
   
void SkillSet::cleanUp()
{
  sort(SkillSorts::ID);

  std::deque<SkillSetElement> unique_elements = calcUniques(skill_elements);

  for (auto it = unique_elements.begin(); it != unique_elements.begin(); ++it)
    if (!(*it).skill->isValid())
      unique_elements.erase(it);

  skill_elements = unique_elements;
}

void SkillSet::copySelf(const SkillSet &source)
{
  skill_elements = source.skill_elements;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool SkillSet::addSkill(Skill* skill, const uint32_t &req_level, 
  	                    const bool enabled)
{
  if (skill_elements.size() < kMAX_SKILLS)
  {
    if (skill->getFlag(SkillFlags::VALID))
    {
      SkillSetElement new_element;

      new_element.skill = skill;
      new_element.level_available = req_level;
      new_element.enabled = enabled;

      skill_elements.push_back(new_element);

      return true;
    }
  }

  return false;
}

bool SkillSet::addSkills(const std::vector<Skill*> skills, 
  	                     const std::vector<uint32_t> &req_levels,
  	                     const std::vector<bool> &enabled)
{
  bool success = true;

  if (skills.size() == req_levels.size() && req_levels.size() == enabled.size())
  {
    auto it_s = skills.begin();
    auto it_r = req_levels.begin();
    auto it_e = enabled.begin();
    bool done = false;

    while (success && !done)
    {
      ++it_s;
      ++it_r;
      ++it_e;

      if (it_s != skills.end() && it_r != req_levels.end() && it_e != enabled.end())
        success &= addSkill((*it_s), (*it_r), (*it_e));
      else
        done = true;
    }
  }

  cleanUp();

  return success;
}

bool SkillSet::addSkills(const std::deque<SkillSetElement> &new_elements)
{
  std::deque<SkillSetElement> unique_elements = calcUniques(new_elements);

  if (skill_elements.size() + unique_elements.size() < kMAX_SKILLS)
  {
    for (SkillSetElement element : unique_elements)
    {
      skill_elements.push_back(element);
      cleanUp();

      return true;
    }
  }

  return false;
}

void SkillSet::clear()
{
  skill_elements.clear();
}

void SkillSet::print()
{
  uint32_t index = 0;

  for (SkillSetElement element : skill_elements)
  {
    std::cout << "Element Index: " << index++ << std::endl;
    std::cout << "Skill Name: " << element.skill->getName() << std::endl;
    std::cout << "Lev. Req. " << element.level_available << std::endl;
    std::cout << "Enabled? " << element.enabled << std::endl;
  }
}

int SkillSet::getIndexOfID(const uint32_t &id)
{
  int i = 0;

  for (auto it = skill_elements.begin(); it != skill_elements.end(); ++it, i++)
    if ((*it).skill->getID() == static_cast<int>(id))
      return i;

  return -1;
}

bool SkillSet::rSkillIndex(const uint32_t &index)
{
  if (index < skill_elements.size())
  {
    skill_elements.erase(skill_elements.begin() + index);

    return true;
  }

  return false;
}

bool SkillSet::rSkillID(const uint32_t &id)
{
  int index = getIndexOfID(id);

  if (index != -1)
    skill_elements.erase(skill_elements.begin() + index);

  return !(index == -1);
}

void SkillSet::sort(const SkillSorts &sort_type, bool ascending)
{
  /*
  if (sort_type == SkillSorts::COOLDOWN)
  {
    std::sort(skill_elements.begin(), skill_elements.end(), 
    	      [](SkillSet a, SkillSet b)
    	      {
    	        if (ascending)
    	          return a.skill->getCooldown() < b.skill->getCooldown();

    	        return a.skill->getCooldown() > b.skill->getCooldown();
    	      });
  }
  else if (sort_type == SkillSorts::COST)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                  return a.skill->getCost() < b.skill->getCost();

                return a.skill->getCost() > b.skill->getCost();
    	      });
  }
  else if (sort_type == SkillSorts::ID)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                  return a.skill->getID() < b.skill->getID();

                return a.skill->getID() > b.skill->getID();
    	      });
  }
  else if (sort_type == SkillSorts::NAME)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                  return a.skill->getName() < b.skill->getName();

                return a.skill->getName() > b.skill->getName();
    	      });
  }
  else if (sort_type == SkillSorts::PRIMARY)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                {
                  return static_cast<uint32_t>(a.skill->getPrimary()) < 
                         static_cast<uint32_t>(b.skill->getPrimary());
                }

                return static_cast<uint32_t>(a.skill->getPrimary()) > 
                       static_cast<uint32_t>(b.skill->getPrimary())
    	      });
  }
  else if (sort_type == SkillSorts::SECONDARY)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                {
                  return static_cast<uint32_t>(a.skill->getSecondary()) < 
                         static_cast<uint32_t>(b.skill->getSecondary());
                }

                return static_cast<uint32_t>(a.skill->getSecondary()) > 
                       static_cast<uint32_t>(b.skill->getSecondary())
    	      });
  }
  else if (sort_type == SkillSorts::POINT_VALUE)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                  return a.skill->getValue() < b.skill->getValue();

                return a.skill->getValue() > b.skill->getValue();
    	      });
  }
  else if (sort_type == SkillSorts::LEVEL_REQ)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                  return a.level_required < b.level_required;

                return a.level_required > b.level_required;
    	      });
  }
  else if (sort_type == SkillSorts::ENABLED)
  {
    std::sort(skill_elements.begin(), skill_elements.end(),
    	      [](SkillSet a, SkillSet b)
    	      {
                if (ascending)
                  return a.enabled < b.enabled;

                return a.enabled > b.enabled;
    	      });
  }
  */
}

std::vector<bool> SkillSet::getAllEnabled()
{
  std::vector<bool> enabled;

  for (SkillSetElement element : skill_elements)
    enabled.push_back(element.enabled);

  return enabled;
}

std::deque<SkillSetElement> SkillSet::getElements(const uint32_t &at_level)
{
  std::deque<SkillSetElement> skills;

  for (auto it = skill_elements.begin(); it != skill_elements.end(); ++it)
    if ((*it).level_available <= at_level)
      skills.push_back((*it));

  return skills;
}

bool SkillSet::getEnabled(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index).enabled;

  return false;
}

std::string SkillSet::getName(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index).skill->getName();

  return "";
}

std::vector<std::string> SkillSet::getNames()
{
  std::vector<std::string> names;

  for (SkillSetElement element : skill_elements)
    names.push_back(element.skill->getName());

  return names;
}

uint32_t SkillSet::getLevel(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index).level_available;

  return kMAX_UNLOCK_LEVEL + 1;
}

std::vector<uint32_t> SkillSet::getLevels()
{
  std::vector<uint32_t> levels;

  for (SkillSetElement element : skill_elements)
    levels.push_back(element.level_available);

  return levels;
}

bool SkillSet::setState(const uint32_t &index, const bool &state)
{
  if (index < skill_elements.size())
  {
    skill_elements[index].enabled = state;

    return true;
  }

  return false;
}

/*=============================================================================
 * OPERATOR
 *============================================================================*/

 SkillSet& SkillSet::operator=(const SkillSet& source)
 {
  /* Check for self assignment */
  if (this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
 }

SkillSet& SkillSet::operator+=(const SkillSet &rhs)
{
  addSkills(rhs.skill_elements);

  return *this;
}

SkillSet& SkillSet::operator-=(const SkillSet &rhs)
{
  for (SkillSetElement element : rhs.skill_elements)
    rSkillID(element.skill->getID());

  return *this;
}

inline SkillSet operator+(SkillSet lhs, const SkillSet &rhs)
{
  lhs += rhs;

  return lhs;
}

inline SkillSet operator-(SkillSet lhs, const SkillSet &rhs)
{
  lhs -= rhs;

  return lhs;
}