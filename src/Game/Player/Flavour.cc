/*******************************************************************************
* Class Name: Flavour [Declaration]
* Date Created: November 24th, 2013
* Inheritance: None
* Description: Bubbies of certain "flavours" (BubbyFlavours) have the same
*              AttributeSet bonus values as well as SkillSet unlocks as
*              other Bubbies of the same flavour.
*
* Notes
* -----
*
* [1]: 
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Flavour.h"

/*=============================================================================
 * CONSTANTS - See .h file for details
 *============================================================================*/
const size_t Flavour::kMAX_DESC_LENGTH = 100;

const std::vector<float> Flavour::kTIER_MODIFIERS = 
{0.0, 1.0, 1.25, 1.5};

const std::vector<uint32_t> Flavour::kTIER_LEVELS =
{5, 10, 15, 20};

const std::vector<float> Flavour::kTIER_MASSES = 
{1.0, 1.0, 2.0, 4.0};

std::vector<Flavour*> Flavour::flavour_list = {};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Creates a default Flavour object
 *
 * Inputs: none
 */
Flavour::Flavour()
  : base_stats(AttributeSet()),
    base_mass(0.0),
    description(""),
    name(""),
    skill_list(nullptr)
{

}

/*
 * Description: Creates a normal Flavour object given a name and stats, also
 *              adds this object to the list of flavours if a flavour of the 
 *              same name hasn't been created before.
 *
 * Inputs: flavour_name - string name of the flavour to be created
 *         min_stats - an attribute set representing he default stats
 *         min_mass - the mass of a Tier 0 Bubby of this flavour
 *         skills - pointer to a set of skills the Flavour unlocks for a Person
 */
Flavour::Flavour(const std::string &flavour_name, const AttributeSet &min_stats,
	             const double &min_mass, SkillSet* skills)
  : base_stats(min_stats),
    base_mass(min_mass),
    description(""),
    name(flavour_name),
    skill_list(skills)
{
  if (!addFlavour(this))
    std::cerr << "Error: Duplicating flavour" << this->getName() << std::endl;
}

/*
 * Description: Annihilates a Flavour object
 */
Flavour::~Flavour() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add a ptr to a flavour object to the static list of
 *              created flavours. Calls isFlavour first.
 *
 * Inputs: new_flavour - ptr to the flavour to add to the list
 * Output: bool - true if the flavour was added to the list
 */
bool Flavour::addFlavour(Flavour* new_flavour)
{
  if (!isFlavour(new_flavour))
  {
    flavour_list.push_back(new_flavour);

    return true;
  }

  return false;
}

/*
 * Description: Evaluates whether a given flavour has already been added to the
 *              static list of those created.
 *
 * Inputs: flavour_check - flavour to be checked for existence
 * Output: bool - true if the flavour has already been create\a
 */
bool Flavour::isFlavour(Flavour* flavour_check)
{
  for (Flavour* flavour : flavour_list)
  {
    std::string f_name = flavour->getName();

    if (f_name == flavour_check->getName() && f_name != "")
      return true;
  }

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns a reference to the attribute set for the base stats of
 *              the Flavour.
 *
 * Inputs: none
 * Output: AttributeSet - const ref. to the base stats
 */
AttributeSet Flavour::getBaseStats()
{
  return base_stats;
}

/*
 * Description: Returns the description of the flavour
 *
 * Inputs: none
 * Output: std::string - const ref. to the description of the flavour
 */
const std::string& Flavour::getDescription()
{
  return description;
}

/*
 * Description: Returns the mass of a Bubby of the flavour of a given tier
 *              by calculating the factors increasing the mass for each tier.
 *
 * Inputs: tier - the tier of the Bubby to determine the mass of
 * Output: double - the mass of a Bubby of the given tier
 */
double Flavour::getMass(const uint32_t &tier)
{
  double mass = base_mass;

  if (tier < kTIER_MASSES.size())
  {
    for (auto it = kTIER_MASSES.begin(); it != kTIER_MASSES.end(); ++it)
      mass *= (*it);

    return mass;
  }

  return 0;
}

/*
 * Description: Returns the string name of the Flavour.
 *
 * Inputs: none
 * Output: string - const ref to the string of the flavour 
 */
const std::string& Flavour::getName()
{
  return name;
}

/*
 * Description: Returns the skills the Flavour unlocks at a given
 *              level by using the skill set function.
 *
 * Inputs: at_level - the level to determine skill unlocks for
 * Output: std::deque<SSE> - skill set elements unlocked by the given level
 */
std::deque<SkillSetElement> Flavour::getSkills(const uint32_t &at_level)
{
  return skill_list->getElements(at_level);
}

/*
 * Description: Returns the SkillSet pointer of the Flavour
 *
 * Inputs: none
 * Output: SkillSet* - ptr to the SkillSet the flavour unlocks
 */
SkillSet* Flavour::getSkillSet()
{
  if (skill_list != nullptr)
    return skill_list;

  return nullptr;
}

/*
 * Description: Returns an AttributeSet describing the alterations the Bubby
 *              flavour will provide when equipped on a Signature given a
 *              tier to determine the stats for.
 *
 * Inputs: tier - the tier to determine the attribute set for
 * Output: AttributeSet - the AttributeSet of the flavour at the given tier
 */
AttributeSet Flavour::getStats(const uint32_t &tier)
{
  AttributeSet stats = getBaseStats();

  if (tier < kTIER_MODIFIERS.size())
  {
    for (uint32_t i = 0; i < tier; i++)
    {
      for (uint32_t j = 0; j < AttributeSet::getSize(); j++)
      {
        auto value = stats.getStat(j) * kTIER_MODIFIERS.at(i + 1);
        stats.setStat(j, value);
      }
    }

    return stats;
  }

  return AttributeSet();
}

/*
 * Description: Returns a pointer to the thumbnail of the Flavour at a given tier.
 *
 * Inputs: index - the tier of the Bubby to return the thumbnail for.
 * Output: Frame* - ptr to the thumbnail of the sprite of the flavour tier.
 */
Frame* Flavour::getThumb(const uint32_t &index)
{
  if (index < thumbnails.size())
    return thumbnails.at(index);

  return nullptr;
}

/*
 * Description: Attempts to assign a new description to the flavour, and returns
 *              true if the asignment was within range.
 *
 * Inputs: new_description - the string to attempt to assign as a description
 * Output: bool - evaluation of the new assignment
 */
bool Flavour::setDescription(const std::string& new_description)
{
  if (new_description.size() <= kMAX_DESC_LENGTH)
  {
    description = new_description;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a new SkillSet (via pointer) to the flavours, and 
 *              returns true if the new SkillSet is not null.
 *
 * Inputs: new_skills - ptr to the new SkillSet
 * Output: bool - true if the SkillSet is null.
 */
bool Flavour::setSkills(SkillSet* new_skills)
{
  skill_list = new_skills;

  return (skill_list != nullptr);
}

/*
 * Description: Attempts to assign a new vector of Frame ptrs for
 *              thumbnails for the various tiers of the flavour.
 *
 * Inputs: new_thumbnails - vector of Frame* representing the thumbnails
 * Output: bool - true if the thumbnails were added.
 */
bool Flavour::setThumbs(const std::vector<Frame*> &new_thumbnails)
{
  bool can_add = true;

  if (new_thumbnails.size() == kTIER_LEVELS.size())
    for (auto it = new_thumbnails.begin(); it != new_thumbnails.end(); ++it)
      can_add &= !((*it) == nullptr);
  
  if (can_add)
    thumbnails = new_thumbnails;

  return can_add;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the static vector of Flavour ptrs of all flavours
 *              which have already been created.
 *
 * Inputs: none
 * Output: std::vector<Flavour*> - vector of ptrs to created flavours
 */
std::vector<Flavour*> Flavour::getFlavours()
{
  std::vector<Flavour*> temp_strings;

  for (auto it = flavour_list.begin(); it != flavour_list.end(); ++it)
    temp_strings.push_back(*it);

  return temp_strings;
}