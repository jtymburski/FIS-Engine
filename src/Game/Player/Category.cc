/*******************************************************************************
* Class Name: Category [Implementation]
* Date Created: December 21st, 2013
* Inheritance: None
* Description: Header for the Category class. Every Person has a Category, which
*              is equivalent to their "Battle Class". This holds their starting
*              and ending values for attributes (In an attribute set), and a
*              set of skills which will become available to the person at
*              various levels.
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Category.h"

/*=============================================================================
 * CONSTANTS - See .h file for description
 *============================================================================*/

bool Category::attr_set_built = false;

const std::vector<int> Category::kMIN_VALUES =
  {50, 15,  7,  7,
    5,  3,  5,  3,
    5,  3,  5,  3,
    5,  3,  5,  3,
    4,  4,  4,  0};
  
const std::vector<int> Category::kMAX_VALUES =
 {50000, 5000, 7000, 7000,
   5500, 5500, 5500, 5500,
   5500, 5500, 5500, 5500,
   5500, 5500, 5500, 5500,
   2000, 2000, 2000,  200};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Category::Category(const std::string &name)
  : base_stats(AttributeSet())
  , top_stats(AttributeSet())
  , description(StringDB::kDEFAULT_CAT_DESC)
  , denonym(StringDB::kDEFAULT_DENONYM)
  , name(StringDB::kDEFAULT_CAT_NAME)
  , skill_set(nullptr)
{
  if (!attr_set_built)
    buildAttrSets();

  cleanUpStats();
}

/*
 * Description:
 *
 * Inputs:
 */
Category::Category(const std::string &name, const std::string &denonym, 
  	               const AttributeSet &base_stats, const AttributeSet &top_stats, 
  	               SkillSet* const skills)
  : base_stats(base_stats)
  , top_stats(top_stats)
  , description(kDEFAULT_CAT_DESC)
  , denonym(denonym)
  , name(name)
  , skill_set(skills)
{
  if (!attr_set_built)
    buildAttrSets();

  cleanUpStats();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Category::buildAttrSets()
{
  max_stats = AttributeSet(kMIN_VALUES);
  max_stats.cleanUp();
  min_stats = AttributeSet(kMIN_VALUES);
  min_stats.cleanUp();
}

void Category::cleanUpStats()
{
  max_stats.cleanUp();
  min_stats.cleanUp();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Category::addImmunity(const Infliction &new_immunity)
{
  if (!isImmune(new_immunity))
  {
    immunities.push_back(new_immunity);

    return true;
  }

  return false;
}

bool Category::isImmune(const Infliction &check_immunity)
{
  for (auto ailment : immunities)
    if (ailment == check_immunity)
      return true;

 return false;
}

bool Category::removeImmunity(const Infliction &rem_immunity)
{
  for (auto it = begin(immunities); it != end(immunities); ++it)
  {
    if ((*it) == rem_immunity)
    {
      immunities.erase(it);

      return true;
    }
  }

  return false;
}

void print(const bool &simple)
{
  std::cout << "--- Category ---\n";
  std::cout << "Name: " << name << "\n";
  std::cout << "Description: " << description << "\n";
  std::cout << "Denonym: " << denonym << "\n";
  std::cout << "Skill Set? " << (skill_Set != nullptr) << "\n";
  std::cout << "Attr Sets Built? " << attr_sets_built << "\n";
  std::cout << "Immunities Size: " << immunities.size() << "\n";
  
  if (!simple)
  {
    std::cout << "Base Stats: \n";
    base_stats.print();
    std::cout << "Top Stats: \n";
    top_stats.print();
  }
}

std::string Category::getDescription()
{
  return description;
}

std::string Category::getDenonym()
{
  return denonym;
}

std::string Category::getName()
{
  return name;
}

AttributeSet& Category::getBaseSet()
{
  return base_stats;
}

SkillSet* Category::getSkills()
{
  return skill_set;
}

AttributeSet& Category::getTopSet()
{
  return top_stats;
}

bool Category::setDescription(const std::string &new_description)
{
  if (new_description.size() <= StringDB::kMAX_BRIEF_DESC)
  {
    description = new_description;

    return true;
  }

  return false;
}

bool Category::setDenonym(const std::string &new_denonym)
{
  if (new_denonym.size() <= StringDB::kMAX_NAME)
  {
    denonym = new_denonym;

    return true;
  }

  return false;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

AttributeSet& Category::getMaxSet()
{
  return max_stats;
}

AttributeSet& Category::getMinSet()
{
  return min_stats;
}