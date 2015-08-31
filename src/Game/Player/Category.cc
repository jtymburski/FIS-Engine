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

/* Constants. See header for description */
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
const int32_t Category::kUNSET_ID = -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Category::Category()
        : base_stats{AttributeSet()}
        , top_stats{AttributeSet()}
        , skill_set{nullptr}
        , id{kUNSET_ID}
{
  if(!attr_sets_built)
    buildAttrSets();

  cleanUpStats();
  classSetup();
}

/*
 * Description: Constructs a basic Category object given a string name
 *
 * Inputs: my_id - the unique ID of the category
 *         name  - the unique name of the category
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
 *         my_id - ID of the Category
 */
Category::Category(const int32_t &my_id, const std::string &name, 
    const std::string &denonym, const AttributeSet &base_stats, 
    const AttributeSet &top_stats, SkillSet* const skills)
      : base_stats{base_stats}
      , top_stats{top_stats}
      , description{StringDb::kDEFAULT_CAT_DESC}
      , denonym{denonym}
      , name{name}
      , qtdr_regen_rate{RegenRate::ZERO}
      , vita_regen_rate{RegenRate::ZERO}
      , skill_set{skills}
      , id{my_id}
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
  setFlag(CategoryState::GRD_ENABLED, false);
  setFlag(CategoryState::IMP_ENABLED, false);
  setFlag(CategoryState::POWER_DEFENDER, false);
  setFlag(CategoryState::POWER_GUARDER, false);
  setFlag(CategoryState::E_STAFF, false);
  setFlag(CategoryState::E_SWORD, false);

  vita_regen_rate = RegenRate::ZERO;
  qtdr_regen_rate = RegenRate::ZERO;
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

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const Category &source - the reference category class
 * Output: none
 */
void Category::copySelf(const Category &source)
{
  base_stats = source.base_stats;
  top_stats = source.top_stats;

  cat_flags = source.cat_flags;

  description = source.description;
  denonym = source.denonym;
  name = source.name;

  qtdr_regen_rate = source.qtdr_regen_rate;
  vita_regen_rate = source.vita_regen_rate;

  skill_set = source.skill_set;

  id = source.id;
  immunities = source.immunities;

  /* Finalize the copy */
  buildAttrSets();
  cleanUpStats();
  classSetup();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds an infliction immunity to the category. This function
 *              will use isImmune() to check for duplicate immunities.
 *
 * Inputs: Infliction - the immunity to add to the category.
 * Output: bool - whether the immunity addition was added
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
 * Description: Determines whether a given piece of Equipment can be equipped
 *              based on the EquipState flags like Staff and Sword, etc. Some
 *              categories should not be able to equip all types of equipment.
 *
 * Inputs: Equipment* const check - equipment to check 
 * Output: bool - whether persons of the category can equip the equipment
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
 * Description: Determines whether a person of the given Category is immune
 *              to a given infliction type.
 *
 * Inputs: Infliction - enumerated infliction to check for immunity
 * Output: bool - true if a person of this Category is immune
 */
bool Category::isImmune(const Infliction &check_immunity)
{
  for (auto ailment : immunities)
    if (ailment == check_immunity)
      return true;

 return false;
}
  
/*
 * Description: Loads the data from file associated with the category.
 *
 * Inputs: XmlData data - the xml data structure
 *         int index - the element reference index
 *         SDL_Renderer* renderer - the rendering engine
 * Output: bool - true if load was successful
 */
bool Category::loadData(XmlData data, int index, SDL_Renderer* renderer)
{
  (void)renderer;
  bool success = true;

  /* ---- DENONYM ---- */
  if(data.getElement(index) == "denonym")
  {
    setDenonym(data.getDataString(&success));
  }
  /* ---- DESCRIPTION ---- */
  else if(data.getElement(index) == "description")
  {
    setDescription(data.getDataString(&success));
  }
  /* ---- IMMUNITIES ---- */
  else if(data.getElement(index) == "immunities")
  {
    bool status = data.getDataBool(&success);
    if(status)
      addImmunity(Helpers::ailmentFromStr(data.getElement(index + 1)));
    else
      removeImmunity(Helpers::ailmentFromStr(data.getElement(index + 1)));
  }
  /* ---- FLAGS ---- */
  else if(data.getElement(index) == "flags")
  {
    /* -- DEF ENABLED -- */
    if(data.getElement(index + 1) == "def_enabled")
      setFlag(CategoryState::DEF_ENABLED, data.getDataBool(&success));
    /* -- E-BIG-ARMS -- */
    else if(data.getElement(index + 1) == "e_big_arms")
      setFlag(CategoryState::E_BIG_ARMS, data.getDataBool(&success));
    /* -- E-CLAWS -- */
    else if(data.getElement(index + 1) == "e_claws")
      setFlag(CategoryState::E_CLAWS, data.getDataBool(&success));
    /* -- E-HEAVY-ARMOUR -- */
    else if(data.getElement(index + 1) == "e_heavy_armor")
      setFlag(CategoryState::E_HEAVY_ARMOR, data.getDataBool(&success));
    /* -- E-LIGHT-ARMOUR -- */
    else if(data.getElement(index + 1) == "e_light_armor")
      setFlag(CategoryState::E_LIGHT_ARMOR, data.getDataBool(&success));
    /* -- E-LONG-ARMS -- */
    else if(data.getElement(index + 1) == "e_long_arms")
      setFlag(CategoryState::E_LONG_ARMS, data.getDataBool(&success));
    /* -- E-MID-ARMOUR -- */
    else if(data.getElement(index + 1) == "e_mid_armor")
      setFlag(CategoryState::E_MID_ARMOR, data.getDataBool(&success));
    /* -- E-SMALL-ARMS -- */
    else if(data.getElement(index + 1) == "e_small_arms")
      setFlag(CategoryState::E_SMALL_ARMS, data.getDataBool(&success));
    /* -- E-STAFF -- */
    else if(data.getElement(index + 1) == "e_staff")
      setFlag(CategoryState::E_STAFF, data.getDataBool(&success));
    /* -- E-SWORD -- */
    else if(data.getElement(index + 1) == "e_sword")
      setFlag(CategoryState::E_SWORD, data.getDataBool(&success));
    /* -- GRD ENABLED -- */
    else if(data.getElement(index + 1) == "grd_enabled")
      setFlag(CategoryState::GRD_ENABLED, data.getDataBool(&success));
    /* -- IMP ENABLED -- */
    else if(data.getElement(index + 1) == "imp_enabled")
      setFlag(CategoryState::IMP_ENABLED, data.getDataBool(&success));
    /* -- POWER DEFENDER -- */
    else if(data.getElement(index + 1) == "power_defender")
      setFlag(CategoryState::POWER_DEFENDER, data.getDataBool(&success));
    /* -- POWER GUARDER -- */
    else if(data.getElement(index + 1) == "power_guarder")
      setFlag(CategoryState::POWER_GUARDER, data.getDataBool(&success));
  }
  /* ---- NAME ---- */
  else if(data.getElement(index) == "name")
  {
    setName(data.getDataString(&success));
  }
  /* ---- QD REGEN ---- */
  else if(data.getElement(index) == "qd_regen")
  {
    setQDRegenRate(Helpers::regenRateFromStr(data.getDataString(&success)));
  }
  /* ---- STATS BASE ---- */
  else if(data.getElement(index) == "stats_base")
  {
    base_stats = AttributeSet::setFromStr(data.getDataString(&success));
    cleanUpStats();
  }
  /* ---- STATS MAX ---- */
  else if(data.getElement(index) == "stats_max")
  {
    top_stats = AttributeSet::setFromStr(data.getDataString(&success));
    cleanUpStats();
  }
  /* ---- VITALITY REGEN ---- */
  else if(data.getElement(index) == "vita_regen")
  {
    setVitaRegenRate(Helpers::regenRateFromStr(data.getDataString(&success)));
  }

  return success;
}

/*
 * Description: Prints out the information of the Category.
 *
 * Inputs: simple - whether to print out a simple version of the category
 *         flags - whether to print out the flags of the category
 * Output: none
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

    std::cout << "Vita Regen: " << Helpers::regenRateToStr(vita_regen_rate)
              << std::endl;
    std::cout << "QD Regen: " << Helpers::regenRateToStr(qtdr_regen_rate)
              << std::endl;
    std::cout << std::endl;
  }

  if (flags)
  {
    std::cout << "DEF? " << getFlag(CategoryState::DEF_ENABLED);
    std::cout << "\nGRD? " << getFlag(CategoryState::GRD_ENABLED);
    std::cout << "\nIMP? " << getFlag(CategoryState::IMP_ENABLED);
    std::cout << "\nPOWER_DEFENDER? " << getFlag(CategoryState::POWER_DEFENDER);
    std::cout << "\nPOWER_GUARDER? " << getFlag(CategoryState::POWER_GUARDER);
    std::cout << "\nE_STAFF? " << getFlag(CategoryState::E_STAFF);
    std::cout << "\nE_SWORD? " << getFlag(CategoryState::E_SWORD);
    std::cout << "\nE_CLAWS? " << getFlag(CategoryState::E_CLAWS) << "\n";
  }
}

/*
 * Description: Attempts to remove a given Infliction enumeration from
 *              the immunties of the Category. Returns true if the infliction
 *              was remove successfully.
 *
 * Inputs: Infliction - enumerated infliction to be removed from immunities. 
 * Output: bool - true if the immunity was found and removed
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
 * Description: This enabled the edit mode of the category; primarily regarding
 *              the base and max attribute sets and allows them to be modified.
 *
 * Inputs: bool enabled - true if edit mode enabled. default to true
 * Output: none
 */
void Category::triggerEditMode(bool enabled)
{
  base_stats.setFlag(AttributeState::PERSONAL, !enabled);
  base_stats.setFlag(AttributeState::CONSTANT, !enabled);

  top_stats.setFlag(AttributeState::PERSONAL, !enabled);
  top_stats.setFlag(AttributeState::CONSTANT, !enabled);
}

/*
 * Description: Returns the string description of the Category
 *
 * Inputs: none
 * Output: std::string - description of the Category
 */
std::string Category::getDescription()
{
  return description;
}

/*
 * Description: Returns the string denonym of the Category
 *
 * Inputs: none
 * Output: std::string - denonym of the Category (ex. Humans)
 */
std::string Category::getDenonym()
{
  return denonym;
}

/*
 * Description: Returns a given CategoryState flag.
 *
 * Inputs: CategoryState test_flag - flag to find the boolean value for
 * Output: bool - the boolean value of the flag
 */
bool Category::getFlag(const CategoryState &test_flag)
{
  return static_cast<bool>((cat_flags & test_flag) == test_flag);
}

/*
 * Description: Returns the ID of the Category
 *
 * Inputs: none
 * Output: int32_t - the ID of the Category
 */
int32_t Category::getID() const
{
  return id;
}

/*
 * Description: Returns the string name of the Category
 *
 * Inputs: none
 * Output: std::string - name of the Category
 */
std::string Category::getName() const
{
  return name;
}

/*
 * Description: Returns the base attribute set for the Category
 *
 * Inputs: none
 * Output: AttributeSet& - ref. to the base attribute set
 */
AttributeSet& Category::getBaseSet()
{
  return base_stats;
}

/*
 * Description: Returns the enumerated quantum drive regeneration rate.
 *
 * Inputs: none
 * Output: RegenRate - enumerated RegenRate for quantum drive
 */
RegenRate Category::getQDRegenRate() const
{
  return qtdr_regen_rate;
}

/*
 * Description: Returns the top attribute set (max level stats) for Persons
 *              of this Category.
 *
 * Inputs: none
 * Output: AttributeSet& - top attribute set values 
 */
AttributeSet& Category::getTopSet()
{
  return top_stats;
}

/*
 * Description: Returns the enumerated vitality regeneration rate.
 *
 * Inputs: none
 * Output: RegenRate - enuemrated RegenRate for vitality
 */
RegenRate Category::getVitaRegenRate() const
{
  return vita_regen_rate;
}

/*
 * Description: Returns the SkillSet which persons of the Category are granted
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the SkillSet
 */
SkillSet* Category::getSkills()
{
  return skill_set;
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
 * Description: Assigns a description to the Category
 *
 * Inputs: std::string new_description - the description for the Category
 * Output: bool - whether the description is within size requirements
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

/*
 * Description: Assigns the ID for the category.
 *
 * Inputs: int32_t id - the new ID
 * Output: none
 */
void Category::setID(int32_t id)
{
  if(id < 0)
    this->id = kUNSET_ID;
  else
    this->id = id;
}
  
/*
 * Description: Sets the immunity to either enabled or disabled. 
 *
 * Inputs: Infliction immunity - the infliction to modify the enabled state
 *         bool enabled - true if immune. false if not. default to true.
 * Output: none
 */
void Category::setImmunity(const Infliction &immunity, const bool &enabled)
{
  if(enabled)
    addImmunity(immunity);
  else
    removeImmunity(immunity);
}

/*
 * Description: Assigns a category name.
 *
 * Inputs: std::string name - the new name
 * Output: none
 */
void Category::setName(const std::string &name)
{
  this->name = name;
}

/*
 * Description: Assigns a new enumerated quantum drive regeneration rate.
 *
 * Inputs: const RegenRate - ref. to a const enumerated RegenRate
 * Output: none
 */
void Category::setQDRegenRate(const RegenRate &new_regen_rate)
{
  qtdr_regen_rate = new_regen_rate;
}
  
/*
 * Description: Sets the skill set which the category unlocks.
 *
 * Inputs: Skillset* set - the set to add to the category
 * Output: none
 */
void Category::setSkills(SkillSet* set)
{
  skill_set = set;
}

/*
 * Description:
 *
 * Inputs: const &RefRate - ref. to a const enumerated RegenRate
 * Output: none
 */
void Category::setVitaRegenRate(const RegenRate &new_regen_rate)
{
  vita_regen_rate = new_regen_rate;
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
