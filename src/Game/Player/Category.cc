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

bool Category::attr_sets_built{false};

AttributeSet Category::max_stats{AttributeSet()};
AttributeSet Category::min_stats{AttributeSet()};

const std::vector<int> Category::kMIN_VALUES =
{  50, 15,  7,  7,
    5,  3,  5,  3,
    5,  3,  5,  3,
    5,  3,  5,  3,
    4,  4,  4,  0};
  
const std::vector<int> Category::kMAX_VALUES =
 { 50000, 5000, 7000, 7000,
    5500, 5500, 5500, 5500,
    5500, 5500, 5500, 5500,
    5500, 5500, 5500, 5500,
    2000, 2000, 2000,  200};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a basic Category object given a string name
 *
 * Inputs: my_id - the unique ID of the category
 *         name  - the unique name of the category
 *
 */
Category::Category(const int32_t &my_id, const std::string &name)
  : base_stats{AttributeSet()}
  , top_stats{AttributeSet()}
  , description{StringDb::kDEFAULT_CAT_DESC}
  , denonym{StringDb::kDEFAULT_DENONYM}
  , name{name}
  , skill_set{nullptr}
  , id{my_id}
{
  if (!attr_sets_built)
    buildAttrSets();

  cleanUpStats();
  classSetup();
}

/*
 * Description: Constructs a normal Category object given starting and ending
 *              stat values.
 *
 * Inputs: my_id - the unique ID of the Category
 *         name - string name of the Category
 *         denoym - the denonym for the persons of the Category (ex. Bears)
 *         base_stats - the level min stats for the Category
 *         top_stats - the level max stats for the Category
 *         skills - pointer to the SkillSet the Category graints to a person
 */
Category::Category(const int32_t &my_id, const std::string &name, 
                   const std::string &denonym, const AttributeSet &base_stats, 
                   const AttributeSet &top_stats, SkillSet* const skills)
  : base_stats{base_stats}
  , top_stats{top_stats}
  , description{StringDb::kDEFAULT_CAT_DESC}
  , denonym{denonym}
  , name{name}
  , skill_set{skills}
  , id(my_id)
{
  if (!attr_sets_built)
    buildAttrSets();

  cleanUpStats();
  classSetup();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Constructs the maximum and minimum values for AttributeSets.
 *
 * Inputs: none
 * Output: none
 */
void Category::buildAttrSets()
{
  max_stats = AttributeSet(kMAX_VALUES, true, true);
  max_stats.cleanUp();
  min_stats = AttributeSet(kMIN_VALUES, true, true);
  min_stats.cleanUp();

  attr_sets_built = true;
}

/*
 * Description: Method to set up default flags for the Category.
 *
 * Inputs: none
 * Output: none
 */
void Category::classSetup()
{
  cat_flags = static_cast<CategoryState>(0);
  setFlag(CategoryState::DEF_ENABLED, true);
  setFlag(CategoryState::GRD_ENABLED, true);
  setFlag(CategoryState::IMP_ENABLED, false);
  setFlag(CategoryState::E_STAFF, false);
  setFlag(CategoryState::E_SWORD, false);
}

/*
 * Description: Cleans up the AttributeSets of the Category, the stats at
 *              starting level and the stats at maximum level.
 *
 * Inputs: none
 * Output: none
 */
void Category::cleanUpStats()
{
  base_stats.cleanUp();
  top_stats.cleanUp();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
bool Category::canEquip(Equipment* const check)
{
  //TODO: Actual classes of equipment [01-21-14]
  if (check == nullptr)
    return false;

  bool can_equip = true;

  if (check->getEquipFlag(EquipState::STAFF))
    can_equip &=  getFlag(CategoryState::E_STAFF);
  if (check->getEquipFlag(EquipState::SWORD))
    can_equip &= getFlag(CategoryState::E_SWORD);

  return can_equip;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
bool Category::addImmunity(const Infliction &new_immunity)
{
  if (!isImmune(new_immunity))
  {
    immunities.push_back(new_immunity);

    return true;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
bool Category::isImmune(const Infliction &check_immunity)
{
  for (auto ailment : immunities)
    if (ailment == check_immunity)
      return true;

 return false;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
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

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
void Category::print(const bool &simple, const bool &flags)
{
  std::cout << "--- Category ---\n";
  std::cout << "Name: " << name << "\n";
  std::cout << "Description: " << description << "\n";
  std::cout << "Denonym: " << denonym << "\n";
  std::cout << "Skill Set Attached? " << (skill_set != nullptr) << "\n";
  //std::cout << "Attr Sets Built? " << attr_sets_built << "\n";
  std::cout << "Immunities Size: " << immunities.size() << "\n\n";

  if (!simple)
  {
    std::cout << "Base Stats: \n";
    base_stats.print(true);
    std::cout << "\nTop Stats: \n";
    top_stats.print(true);
    std::cout << "\n";
  }

  if (flags)
  {
    std::cout << "DEF? " << getFlag(CategoryState::DEF_ENABLED) << "\n";
    std::cout << "GRD? " << getFlag(CategoryState::GRD_ENABLED) << "\n";
    std::cout << "IMP? " << getFlag(CategoryState::IMP_ENABLED) << "\n";
    std::cout << "E_STAFF? " << getFlag(CategoryState::E_STAFF) << "\n";
    std::cout << "E_SWORD? " << getFlag(CategoryState::E_SWORD) << "\n\n";
  }
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
std::string Category::getDescription()
{
  return description;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
std::string Category::getDenonym()
{
  return denonym;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
bool Category::getFlag(const CategoryState &test_flag)
{
  return static_cast<bool>((cat_flags & test_flag) == test_flag);
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
std::string Category::getName()
{
  return name;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
AttributeSet& Category::getBaseSet()
{
  return base_stats;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
SkillSet* Category::getSkills()
{
  return skill_set;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
AttributeSet& Category::getTopSet()
{
  return top_stats;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
bool Category::setDescription(const std::string &new_description)
{
  if (new_description.size() <= StringDb::kMAX_BRIEF_DESC)
  {
    description = new_description;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a new string denonym to the category
 *
 * Inputs: new_denonym - string denonym to be assigned
 * Output: bool - true if the assignment is successful
 */
bool Category::setDenonym(const std::string &new_denonym)
{
  if (new_denonym.size() <= StringDb::kMAX_NAME)
  {
    denonym = new_denonym;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a given CategoryState flag to a given value.
 *
 * Inputs: flags - the given flag to be assigned a value
 *         set_value - the boolean value to assign to the flag
 * Output: none
 */
void Category::setFlag(const CategoryState &flags, const bool &set_value)
{
  (set_value) ? (cat_flags |= flags) : (cat_flags &= ~flags);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns a reference to the absolute maximum stats for a 
 *              Category.
 *
 * Inputs: none
 * Output: AttributeSet& - reference to the maximum set
 */
AttributeSet& Category::getMaxSet()
{
  return max_stats;
}

/*
 * Description: Returns a reference to the absolute minimum stats for a
 *              Category.
 *
 * Inputs: none
 * Output: AttributeSet& - reference to the minimum set
 */
AttributeSet& Category::getMinSet()
{
  return min_stats;
}