/*******************************************************************************
* Class Name: SkillSet [Implementation]
* Date Created: November 23rd, 2013
* Inheritance: None
* Description: A SkillSet is a container for a list of skills and the level
*              required to be usable. A SkillSet is contained in a BubbyFlavour,
*              Race, Category, Person, Equipment, Item. A SkillList can be
*              sorted by level, by cost of the skill, by the assigned point
*              value (for AI), by the elements the skill uses, etc.
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

const bool     SkillSet::kADDITION_BYPASS  =  true;
const bool     SkillSet::kENABLED_DEFAULT  =  true;
const uint32_t SkillSet::kID_BUBBIFIED     =     0;
const size_t   SkillSet::kMAX_SKILLS       =   500;
const size_t   SkillSet::kMAX_UNLOCK_LEVEL =   127;
const size_t   SkillSet::kMIN_UNLOCK_LEVEL =     1;
const int      SkillSet::kUNSET_ID         =    -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default constructor. Default.
 *
 * Inputs: int new_id - starting ID. Defualt to 0.
 */
SkillSet::SkillSet(int new_id)
{
  id = kUNSET_ID;

  setID(new_id);
}

/*
 * Description: Single Skill SkillSet constructor - constructs a SkillSet
 *              with one SetElement
 *
 * Inputs: Skill* - pointer to the skill to add
 *         level  - the level the Skill will become available
 */
SkillSet::SkillSet(Skill* skill, const uint32_t &level) : SkillSet()
{
  addSkill(skill, level, kENABLED_DEFAULT);
}

/*
 * Description: Multi Skill SkillSet constructor - constructs a SkillSet with
 *              multiple SetElements
 *
 * Inputs: skills - vector of skill pointers to be used
 *         levels - vector of req. levels corresponding to the vector of skills
 */
SkillSet::SkillSet(const std::vector<Skill*> &skills,
                   const std::vector<uint32_t> &levels) : SkillSet()
{
  std::vector<bool> enabled_values(levels.size(), kENABLED_DEFAULT);
  addSkills(skills, levels, enabled_values);
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Calculates the lowest level of a Skill of a given ID in the
 *              SkillSet (ex. Maul - Lv. 5 and Maul Lv. 4 - the Maul Lv. 4 will
 *              be the lowest level)
 *
 * TODO - When adding, just overwrite the other Skill? [01-26-14]
 *
 * Inputs: skill_id - the ID of the skill to check
 * Output: uint32_t - the lowest level found of the given skill ID
 */
uint32_t SkillSet::calcLowestLevel(const uint32_t &skill_id)
{
  uint32_t min_level = kMAX_UNLOCK_LEVEL;

  for (SetElement element : skill_elements)
    if (element.skill->getID() == static_cast<int>(skill_id))
      if (element.level_available < min_level)
        min_level = element.level_available;

  return min_level;
}

/*
 * Description: Calculates a unique set of SetElements by checking all the IDs
 *              of Skills in the SkillSet and only adding the first occurence of
 *              each ID, then returns the resultant vector.
 *
 * Inputs: std::vector<SetElement> - a vector of set elements to find uniques of
 * Output: std::vector<SetElement> - a vector of unique set elements
 */
std::vector<SetElement> SkillSet::calcUniques(const std::vector<SetElement>
                                              &check_elements)
{
  std::vector<uint32_t> uniques;
  std::vector<SetElement> unique_elements;

  for (SetElement element : check_elements)
  {
    const uint32_t curr_id = element.skill->getID();

    auto result = std::find(uniques.begin(), uniques.end(), curr_id);

    if (result == uniques.end())
    {
      uniques.push_back(curr_id);
      unique_elements.push_back(element);
    }
  }

  return unique_elements;
}

/*
 * Description: Cleans up the SkillSet by erasing Skills which are invalid. This
 *              function checks the validity of each Skill by running its valid
 *              function again.
 *
 * Inputs: none
 * Output: none
 */
void SkillSet::cleanUp()
{
  sort(SkillSorts::ID);

  std::vector<SetElement> unique_elements = calcUniques(skill_elements);

  for (auto it = unique_elements.begin(); it != unique_elements.begin(); ++it)
    if (!(*it).skill->isValid())
      unique_elements.erase(it);

  skill_elements = unique_elements;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds a Single skill to the SkillSet given a pointer to a Skill,
 *              a level required, and a starting enabled value.
 *
 * Inputs: skill - pointer to the Skill to be added
 *         req_level - the level required to unlock the Skill
 *         enabled - whether the Skill will initially be useable
 * Output: bool - true if the Skill was added successfully
 */
bool SkillSet::addSkill(Skill* skill, const uint32_t &req_level,
  	                    const bool enabled)
{
  if (skill != nullptr && skill_elements.size() < kMAX_SKILLS)
  {
    if (skill->getFlag(SkillFlags::VALID))
    {
      SetElement new_element;

      new_element.skill = skill;

      if (req_level <= kMAX_UNLOCK_LEVEL)
        new_element.level_available = req_level;
      else
        new_element.level_available = kMAX_UNLOCK_LEVEL;

      new_element.enabled  = enabled;
      new_element.silenced = false;

      skill_elements.push_back(new_element);

      return true;
    }
  }

  return false;
}

/*
 * Description: Adds multiple skills to the SkillSet given a vector of Skill
 *              pointers, a vector of required levels and a vector of enabled
 *              values. cleanUp is called at the end of this to check for
 *              unique Skills.
 *
 * Inputs: skills - vector of Skill pointers
 *         req_levels - vector of corresponding required levels
 *         enabled - vector of enabled values
 * Output: bool - true if all skills were added successfully.
 */
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

    for(; done ; )
    {
      if (it_s != skills.end() && it_r != req_levels.end() && it_e != enabled.end())
        success &= addSkill((*it_s), (*it_r), (*it_e));
      else
        done = true;

      ++it_s;
      ++it_r;
      ++it_e;
    }
  }

  cleanUp();

  return success;
}

/*
 * Description: Adds skills by vector of SetElements (useful for operator
 *              overloading). May bypas the kMAX_SKILLS check when temporarily
 *              combining mass amounts of skils.
 *
 * Inputs: new_elements - vector of SetElements to be added
 * Output: bool - true if all SetElements were added successfully
 */
bool SkillSet::addSkills(const std::vector<SetElement> &new_elements,
                         const bool &bypass)
{
  std::vector<SetElement> unique_elements = calcUniques(new_elements);

  if (skill_elements.size() + unique_elements.size() < kMAX_SKILLS ||
      bypass)
  {
    for (auto element : unique_elements)
      skill_elements.push_back(element);

    cleanUp();

    return true;
  }

  return false;
}

/*
 * Description: Clears all the SetElements from the vector.
 *
 * Inputs: none
 * Output: none
 */
void SkillSet::clear()
{
  skill_elements.clear();
}

/*
 * Description: Prints out the state of the object
 *
 * Inputs: simple - boolean whether to output a simplified version
 * Output: none
 */
void SkillSet::print(const bool &simple)
{
  uint32_t index = 0;

  std::cout << "--- Skill Set ---" << std::endl;
  std::cout << "ID: " << id << std::endl;
  std::cout << "Skill Set Elements: " << skill_elements.size() << std::endl;

  for (SetElement element : skill_elements)
  {
    if (simple)
    {
      std::cout << index++ << " ID: " << element.skill->getID() << " N: "
                << element.skill->getName() << std::endl;
    }
    else
    {
      std::cout << "Element Index: " << index++ << std::endl;
      std::cout << "Skill Name: " << element.skill->getName() << std::endl;
      std::cout << "Lev. Req. " << element.level_available << std::endl;
      std::cout << "Enabled? " << element.enabled << std::endl;
      std::cout << "Silenced? " << element.silenced << std::endl;
    }
  }

  std::cout << "--- / Skill Set ---" << std::endl;
}

/*
 * Description: Returns the index on the vector of a Skill of a given ID or if
 *              not found, returns -1.
 *
 * Inputs: id - the Skill ID to check the vector for
 * Output: int32_t - the index of a given SkillID, or -1 if not found.
 */
int32_t SkillSet::getIndexOfID(const uint32_t &id)
{
  int i = 0;

  for (auto it = begin(skill_elements); it != end(skill_elements); ++it, i++)
    if ((*it).skill->getID() == static_cast<int>(id))
      return i;

  return -1;
}

/*
 * Description: Returns the index along the vector of skill elements whose name
 *              matches the given char.
 *
 * Inputs: alpha - beginning char to find name of Skill for
 * Output: int32_t - first index of the skill found with that name
 */
int32_t SkillSet::getIndexOfAlpha(const char &alpha)
{
  int i = 0;

  for (auto it = begin(skill_elements); it != end(skill_elements); ++it, i++)
  {
    if ((*it).skill->getName().size() > 0 &&
        tolower((*it).skill->getName().at(0)) == alpha)
    {
      return i;
    }
  }

  return -1;
}

/*
 * Description: Removes a SetElement from the vector at a given index and
 *              returns true if the element was removed.
 *
 * Inputs: index - index of the vector of the SetElement to be removed
 * Output: bool - true if removal happened
 */
bool SkillSet::removeIndex(const uint32_t &index)
{
  if (index < skill_elements.size())
  {
    skill_elements.erase(skill_elements.begin() + index);

    return true;
  }

  return false;
}

/*
 * Description: Removes a SetElement from the vector by a given ID value and
 *              returns true if the element was found and removed.
 *
 * Inputs: id - the ID of the SetElement to be removed
 * Output: bool - true if the ID was found and the SetElement was removed
 */
bool SkillSet::removeID(const uint32_t &id)
{
  int index = getIndexOfID(id);

  if (index != -1)
    skill_elements.erase(skill_elements.begin() + index);

  return !(index == -1);
}

/*
 * Description: Sorts the vector of skills by a given sort type, and either in
 *              an ascending or descending manner.
 *
 * Inputs: sort_type - enumerated SkillSorts to be performed
 *         ascending - whether to sort in asc. or desc. order
 * Output: none
 */
bool SkillSet::sort(const SkillSorts &sort_type, bool ascending)
{
  auto sort_status = false;

  /* Sort the SSEs by their Skill's cooldown value */
  if (sort_type == SkillSorts::COOLDOWN)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompSkillCooldown());
    sort_status = true;
  }

  /* Sort the SSEs by their Skill's QD cost */
  else if (sort_type == SkillSorts::COST)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompSkillCost());
    sort_status = true;
  }

  /* Sort the SSEs by their Skill's ID # */
  else if (sort_type == SkillSorts::ID)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompSkillID());
    sort_status = true;
  }

  /* Sort the SSEs by their Skill's name */
  else if (sort_type == SkillSorts::NAME)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompSkillName());
    sort_status = true;
  }

  /* Sort the SSEs by their Skill's primary elemental attribute
       Ascending Order: NONE, PHYS, TH, PO, PR, CY, DI, VO */
  else if (sort_type == SkillSorts::PRIMARY)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompSkillPrimary());
    sort_status = true;
  }

  /* Sort the SSEs by their Skill's secondary elemental attribute
       Ascending Order: NONE, PHYS, TH, PO, PR, CY, DI, VO */
  else if (sort_type == SkillSorts::SECONDARY)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompSkillSecondary());
    sort_status = true;
  }

  /* Sort the SSEs by their Skill's arbitrary point value */
  else if (sort_type == SkillSorts::POINT_VALUE)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompSkillPoint());
    sort_status = true;
  }

  /* Sort the SSEs by the level required for their use */
  else if (sort_type == SkillSorts::LEVEL_REQ)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompLevelRequired());
    sort_status = true;
  }

  /* Sort the SSEs by their enabled value. Ascending Order: Enable, Disable */
  else if (sort_type == SkillSorts::ENABLED)
  {
    std::sort(begin(skill_elements), end(skill_elements),
              Helpers::CompEnabled());
    sort_status = true;
  }

  if (sort_status && !ascending)
    std::reverse(begin(skill_elements), end(skill_elements));

  return sort_status;
}

/*
 * Description: Compiles and returns a vector of the enabled values.
 *
 * Inputs: none
 * Output: std::vector<bool> - the constructed vector of enabled values
 */
std::vector<bool> SkillSet::getAllEnabled()
{
  std::vector<bool> enabled;

  for (SetElement element : skill_elements)
    enabled.push_back(element.enabled);

  return enabled;
}

/*
 * Description: Returns a set element at a given index if it exists.
 *
 * Inputs: index - index to check for a skill set element
 * Output: SetElement - the element at the index or a default element.
 */
SetElement SkillSet::getElement(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index);

  SetElement empty;
  return empty;
}

/*
 * Description: Compiles and returns a vector of SetElements which have
 *              level required values at or below a given value.
 *
 * Inputs: at_level - the upper level bound to search for
 * Output: std::vector<SetElement> - SetElements within the level bound
 */
std::vector<SetElement> SkillSet::getElements(const uint32_t &at_level)
{
  std::vector<SetElement> skills;

  for (auto it = skill_elements.begin(); it != skill_elements.end(); ++it)
    if ((*it).level_available <= at_level)
      skills.push_back((*it));

  return skills;
}

/*
 * Description: Returns the enabled value at a given index.
 *
 * Inputs: index - index to find the enabled value for
 * Output: bool - the enabled value at the given index
 */
bool SkillSet::getEnabled(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index).enabled;

  return false;
}

/*
 * Description: Returns the silenced value at a given index.
 *
 * Inputs: index - index to find the enabled value for
 * Output: bool - the silenced value at the given index
 */
bool SkillSet::getSilenced(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index).silenced;

  return false;
}

/*
 * Description: Returns whether a SkillSet element is currently useable by
 *              a person. This means the skill must both be enabled and not
 *              be silenced.
 *
 * Inputs: index - index to find the useable truth value for
 * Output: bool - true if the skill cane be used at this time
 */
bool SkillSet::getUseable(const uint32_t &index)
{
  auto useable = false;

  if (index < skill_elements.size())
  {
    useable |= skill_elements.at(index).enabled;
    useable &= !skill_elements.at(index).silenced;
  }

  return useable;
}

/*
 * Description: Returns the ID of the Skill Set.
 *
 * Inputs: none
 * Output: int - the ID of the Skill Set.
 */
int SkillSet::getID()
{
  return id;
}

/*
 * Description: Returns the string name at a given index
 *
 * Inputs: index - the index to find the name of the skill of
 * Output: std::string - the name of the skill at the given index
 */
std::string SkillSet::getName(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index).skill->getName();

  return "";
}

/*
 * Description: Returns the vector of string names of all Skills
 *
 * Inputs: none
 * Output: std::vector<std::string> - vector of names of the Skills
 */
std::vector<std::string> SkillSet::getNames()
{
  std::vector<std::string> names;

  for (SetElement element : skill_elements)
    names.push_back(element.skill->getName());

  return names;
}

/*
 * Description: Returns the required level at a given index
 *
 * Inputs: index - index to find the required level for
 * Output: uint32_t - the required level of the Skill at the given index
 */
uint32_t SkillSet::getLevel(const uint32_t &index)
{
  if (index < skill_elements.size())
    return skill_elements.at(index).level_available;

  return kMAX_UNLOCK_LEVEL + 1;
}

/*
 * Description: Compiles and returns the vector of required levels for all
 *              Skills
 *
 * Inputs: none
 * Output: std::vector<uint32_t> - the vector of required levels
 */
std::vector<uint32_t> SkillSet::getLevels()
{
  std::vector<uint32_t> levels;

  for (SetElement element : skill_elements)
    levels.push_back(element.level_available);

  return levels;
}

/*
 * Description: Returns the number of elements in the skill set
 *
 * Inputs: none
 * Output: uint32_t - the size of the skill set
 */
uint32_t SkillSet::getSize()
{
  return skill_elements.size();
}

/*
 * Description: Returns the vector of values in the SkillSet
 *
 * Inputs: none
 * Output: std::vector<uint32_t> - the vector of values of skills in the set
 */
std::vector<uint32_t> SkillSet::getValues()
{
  std::vector<uint32_t> skill_values;

  for (auto it = begin(skill_elements); it != end(skill_elements); ++it)
    if ((*it).skill != nullptr)
      skill_values.push_back((*it).skill->getValue());

  return skill_values;
}

/*
 * Description: Assigns a SetElement of a given index a given enabled state
 *
 * Inputs: index - the index to assign the enabled state
 *         bool - the enabled state (true -> enabled)
 * Output: bool - true if the index was valid and the state was set
 */
bool SkillSet::setEnabled(const uint32_t &index, const bool &state)
{
  if (index < skill_elements.size())
  {
    skill_elements[index].enabled = state;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a new id to the skill set.
 *
 * Inputs: int new_id - the ID to assign the skill set to
 * Output: none
 */
void SkillSet::setID(const int &new_id)
{
  if(new_id > kUNSET_ID)
    id = new_id;
  else
    id = kUNSET_ID;
}

/*
 * Description: Assigns a SetElement element at a given index to a given
 *              silenced state (when silence is inflicted or relieved)
 *
 * Inputs: index - the index to assign the enabled state
 *         bool - the enabled state (true -> enabled)
 * Output: bool - true if the index was valid and the state was set
 */
bool SkillSet::setSilenced(const uint32_t &index, const bool &state)
{
  if (index < skill_elements.size())
  {
    skill_elements[index].silenced = state;

    return true;
  }

  return false;
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

/*
 * Description: Compound assignment operator - adds another SkillSet to the
 *              current object. If kADDITION_BYPASS is set, it will bypass
 *              the maximum number of SetElements possible.
 *
 * Inputs: rhs - const ref. to the SkillSet to be added
 * Output: SkillSet& - ref. to the combined object
 */
SkillSet& SkillSet::operator+=(const SkillSet &rhs)
{
  addSkills(rhs.skill_elements, kADDITION_BYPASS);

  return *this;
}

/*
 * Description: Compound subtraction operator - subtracts a SkillSet from
 *              the corrent object. This will remove  all Skills from the
 *              current object whose IDs match those in the rhs object.
 *
 * Inputs: rhs - const ref to the SkillSet to be subtracted
 * Output: SkillSet& - ref to the object after subtraction
 */
SkillSet& SkillSet::operator-=(const SkillSet &rhs)
{
  for (SetElement element : rhs.skill_elements)
    removeID(element.skill->getID());

  return *this;
}

/*
 * Description: See compound assignment operator
 *
 * Inputs: rhs - const ref. to the SkillSet to be added
 * Output: SkillSet& - ref to the combined object
 */
SkillSet& SkillSet::operator+(const SkillSet &rhs)
{
  *this += rhs;

  return *this;
}

/*
 * Description: See compound subtraction operator
 *
 * Inputs: rhs - const ref. to the SkillSet to be subtracted
 * Output: SkillSet& - ref. to the object after subtraction
 */
SkillSet& SkillSet::operator-(const SkillSet &rhs)
{
  *this -= rhs;

  return *this;
}
