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

/*=============================================================================
 * CONSTANTS - See .h file for details
 *============================================================================*/v
const size_t Flavour::kMAX_DESC_LENGTH = 100;

const std::vector<float> Flavour::kTIER_MODIFIERS = 
{0.0, 1.0, 1.25, 1.5};

const std::vector<uint> Flavour::kTIER_LEVELS =
{5, 10, 15, 20};

const std::vector<float> Flavour::kTIER_MASSES = 
{1.0, 2.0, 2.0};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Flavour::Flavour()
{

}

Flavour::Flavour(const std::string &flavour_name, const AttributeSet &min_stats,
	             const double &min_mass, const SkillSet* skills)
  : base_stats(min_stats),
    base_mass(min_mass),
    description(""),
    name(flavour_name),
    skill_list(skills)
{

}

/*
 * Description:
 */
Flavour::~Flavour() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
static bool Flavour::addFlavour(const BubbyFlavour* new_flavour)
{
  if (!isFlavour(new_flavour))
  {
    flavour_list.push_back(new_flavour);

    return true;
  }

  return false;
}

static bool Flavour::isFlavour(const BubbyFlavour* flavour_check)
{
  for (BubbyFlavour* flavour : flavour_list)
    if (flavour->getName() == flavour_check->getName())
      return true;

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

const AtributeSet& Flavour::getBaseStats()
{
  return base_stats;
}

std::string Flavour::getDescription()
{
  return description;
}

double Flavour::getMass(const uint &tier)
{
  double mass = base_mass;

  for (uint i = 0; i < tier; i++)
    mass *= kTIER_MASSES.at(i);

  return mass;
}

const std::string& Flavour::getName()
{
  return name;
}

std::deque<SkillSetElement> Flavour::getSkills(const uint &at_level)
{
  return skill_list->getElements(at_level);
}

SkillSet* Flavour::getSkillSet()
{
  if (skill_list != nullptr)
    return skill_list;

  return nullptr;
}

AttributeSet Flavour::getStats(const uint &tier)
{
  AttributeSet stats = getBaseStats();

  if (tier >= 0)
  {
    stats += AttributeSet();
  }
  for (uint i = 0; i < tier; i++)
    for (uint j = 0; j < AttributeSet::getSize(); j++)
      stats.setStat(j, getStat(j) * kTIER_MODIFIERS(i + 1));

  return stats;
}

Frame* getThumb(const uint &index)
{
  if (index < thumbnails.size())
    retrn thumbnails.at(index);
}

bool Flavour::setDescription(const std::string& new_description)
{
  if (new_description.size() <= kMAX_DESC_LENGTH)
  {
    description = new_description;

    return true;
  }

  return false;
}

bool Flavour::setSkills(const SkillSet* new_skills)
{
  skill_list = new_skills;

  return (skill_list == nullptr);
}

bool Flavour::setThumbs(const std::vector<Frame*> &new_thumbnails)
{
  bool can_add = true;

  if (new_thumbnails.size() == kTIER_LEVELS.size())\
    for (auto it = new_thumbnails.begin(); it != new_thumbnails.end(); ++it)
      can_add &= !((*it) == nullptr);
  
  if (can_add)
    thumbnails = new_thumbnails;

  return can_add;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

static std::vector<BubbyFlavour*> Flavour::getFlavours()
{
  return flavour_list;
}
