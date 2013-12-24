/*******************************************************************************
* Class Name: Person [Implementation]
* Date Created: 
* Inheritance: 
* Description:
*
* Notes
* -----
*
* [1]: AttributeSets outside of and during Battle:
*
* In & Out of Battle
* -------------
* base_stats - the level 1 stats of the Person calculated by battle_class + race
* max_base_stats - the level max stats of the Person calc by battle_class + race
* 
* Out of Battle
* -------------
* curr_sats      - unused
* curr_max_stats - normal stats between base and base max at the person's level
* temp           - curr_max_stats + equipmemt
*
* In Battle
* ---------
* curr_stats     - displayed value of stats after Battle modifiers
* curr_max_stats - possible reachable stat values after Battle modifiers
* temp           - the normal value of stats before Batle modifiers
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Person.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
static const size_t   Person::kNUM_LEVELS{127};     
static const size_t   Person::kNUM_EQUIP_SLOTS{5}; 
static const uint32_t Person::kMIN_EXP{0};        
static const uint32_t Person::kMAX_EXP{1000000000}; /* 1 billion */       
static const uint32_t Person::kMIN_LVL_EXP{10};   
static const uint32_t Person::kMAX_LVL_EXP{100000000}; /* 100 million */   
static conts uint32_t Person::kMAX_EXP_DROP{1000000}; /* 1 million */
static const size_t   Person::kMAX_ITEM_DROPS{5};
static const float Person::kMIN_DMG_MODI{ 0.01};    
static const float Person::kMAX_DMG_MODI{10.00}
static const float Person::kMIN_EXP_MODI{ 0.10};
static const float Person::kMAX_EXP_MODI{10.00}; 

/* XS, S, A, B, C, D */
static const std::vector<float> Person::kPRIM_MODS =
{ 1.425, 1.400, 1.375, 1.350, 1.325, 1.300};

static const std::vector<float> Person::kSECD_MODS =
{ 1.375, 1.300, 1.250, 1.225, 1.200, 1.175};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Person::Person(const uint32_t &game_id, const std::string &name, 
               Category* const battle_class, Category* const race_class)
  : game_id{game_id}
  , my_id{++id}
  , base_person{nullptr}
  , battle_class{battle_class}
  , race{race_class}
  , name{name}
{
  setupClass();
}

/*
 * Description:
 *
 * Inputs:
 */
Person::Person(Person* const source)
  : game_id{source.game_id}
  , my_id{++id}
  , base_person{source}
{
  setupClass();
}

/*
 * Description:
 *
 */
Person::~Person()
{
  if (base_person != nullptr)
  {
    /* Delete the equipments that were created from the base person */
    for (auto equipment : equipments)
    {
      if (equipment != nullptr)
      {
        delete equipment;
        equipment = nullptr;
      }
    }
  }

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

/* Sets up the class, based on whether base_person is assigned or not */
void Person::setupClass()
{
  if (exp_table.empty())
    buildExpTable();

  /* Setup the class for a Base Person */
  if (base_person == nullptr)
  {
    battle_flags = static_cast<BattleState>(0);
    person_flags = static_cast<PersonState(0);
    //person_record{};

    updateRank();

    primary = Element::NONE;
    secondary = Element::NONE;
    primary_curve = ElementCurve::D;
    secondary_curve = ElementCurve::D;

    total_exp = kMIN_LVL_EXP;
    updateLevel();
    updateBaseStats();

    curr_stats     = base_stats;
    curr_max_stats = base_stats;
    temp_max_stats = base_stats;

    learned_skills = SkillSet();
    updateBaseSkills();
    curr_skills = base_skills;

    dmg_mod = 1.000;
    exp_mod = 1.000;

    for (size_t = 0; size_t < kNUM_EQUIP_SLOTS)
      equipments.push_back(nullptr);

    credit_drop = 0;
    exp_drop = kMIN_EXP_DROP;

    first_person = nullptr;
    third_person = nullptr;
    fp_bubbified_sprite = nullptr;
    tp_bubbified_sprite = nullptr;
  }

  /* Setup the class as a copy of the Base Person */
  else
  {
    battle_flags = base_person->battle_flags;
    person_flags = base_person->person_flags;
    //person_record = base-person->person_record;
    battle_class = base_person->battle_class;
    race = base_person->race;
    name = base_person->name;
    rank = base_person->rank;
    primary = base_person->primary;
    secondary = base_person->secondary;
    primary_curve = base_person->primary_curve;
    secondary_curve = base_person->secondary_curve;
    base_stats = base_person->base_stats;
    base_max_stats = base_person->base_max_stats;
    curr_stats = base_person->curr_stats;
    curr_max_stats = base_person->curr_max_stats;
    temp_max_stats = base_person->temp_max_stats;
    base_skills = base_person->base_skills;
    curr_skills = base_person->curr_skills;
    learned_skills = base_person->learned_skills;
    dmg_mod = base_person->dmg_mod;
    exp_mod = base_person->exp_mod;

    /* Equipments are deep-copied */
    for (const auto equipment : base_person->equipments)
    {
      if (equipment != nullptr)
        equipments.push_back(new Equipment(equipments));
      else
        equipments.push_back(nullptr);
    }

    item_drops = base_person->item_drops;
    credit_drop = base_person->credit_drop;
    exp_drop = base_person->exp_drop;
    level = base_person->level;
    total_exp = base_person->total_exp;

    first_person = base_person->first_person;
    third_person = base_person->third_person;
    fp_bubbified_sprite = base_person->fp_bubbified_sprite;
    tp_bubbified_sprite = base_person->tp_bubbified_sprite;
  }
}

/* Recalculates the Person's base and base_max stats based on categories */
void Person::updateBaseStats()
{
  AttributeSet temp = battle_class->getBaseSet() + race->getBaseSet();
  AttributeSet temp_max = battle_class->getTopSet() + race->getTopSet();

  std::vector<int32_t> prim_indexes;
  std::vector<int32_t> secd_indexes;
  
  prim_indexes.push_back(AttributeSet::getOffensiveIndex(primary));
  prim_indexes.push_back(AttributeSet::getDefensiveIndex(primary));
  secd_indexes.push_back(AttributeSet::getOffensiveIndex(secondary));
  secd_indexes.push_back(AttributeSet::getDefensiveIndex(secondary));

  auto p_mod& = getCurveModifier(primary_curve, true);
  auto s_mod& = getCurveModifier(secondary_curve, false);

  for (auto index : prim_indexes)
  {
    if (index != -1)
    {
      auto a = std::floor(static_cast<float>(temp.getStat(index)) * p_mod);
      auto b = std::floor(static_cast<float>(temp_max.getStat(index)) * p_mod);

      temp.setStat(index, a);
      temp_max.setStat(index, b);
    }
  }
  for (auto index : secd_indexes)
  {
    if (index != -1)
    {
      auto a = std::floor(static_cast<float>(temp.getStat(index)) * s_mod);
      auto b = std::floor(static_cast<float>(temp_max.getStat(index)) * s_mod);

      temp.setStat(index, a);
      temp_max.setStat(index, b);
    }
  }

  temp.cleanUp();
  temp_max.cleanUp();

  base_stats = temp;
  base_max_stats = temp_max;

  updateStats();
}

void Person::updateBaseSkills()
{
  base_skills.clear();
  base_skills = SkillSet();

  if (battle_class->getSkills() != nullptr)
    base_skills += *(battle_class->getSkills());
  if (race_class->getSkills() != nullptr)
    base_skills += *(race->getSkills());

  updateSkills();
}

/* Updates the level of the person based on their current total experience */
void Person::updateLevel()
{
  if (getPersonFlag(PersonState::CAN_LEVEL_UP)))
  {
    auto before = level;

    level = getLevelAt(total_exp);

    if (level == kNUM_LEVELS)
      setPersonFlag(PersonState::MAX_LVL));

    if (level != before)
      updateStats();
  }
}

/* Updates the current stats of the Person based on their current level */
void Person::updateStats()
{
  if (level == 1)
  {
    curr_stats = base_stats;
    curr_max_stats = base_stats;
  }
  else if (level == kNUM_LEVELS)
  {
    curr_stats = base_max_stats;
    curr_max_stats = base_max_stats;
  }
  else
  {
    curr_stats = AttributeSet(0, true);
    curr_max_stats = AttributeSet(0, true);

    for (size_t i = 0; i < AttributeSet::getSize(); i++)
    {
      std::vector<uint32_t> stat_values{};
      stat_values = Helpers::buildExpTable(base_stats.at(i), 
                                           base_max_stats.at(i), 
                                           kNUM_LEVELS);
      curr_stats.setStat(i, stat_values.at(level));
    }

    curr_max_stats = curr_stats;
  }

  temp_max_stats = curr_max_stats;

  for (auto equipment : equipments)
    if (equipment != nullptr)
      temp_max_stats += equipment->getStats();

  curr_stats.cleanUp();
  curr_max_stats.cleanUp();
  temp_max_stats.cleanUp();
}

void Person::updateSkills()
{
  curr_skills.clear();
  curr_skills  = learned_skills;
  curr_skills += base_skills;
}

/* Updates the rank of the Person based on their Person record */
void Person::updateRank()
{
  //rank = person_record.getRank();
}

/* Constructs the table of experience values given the current constants */
void Person::buildExpTable()
{
  exp_table = Helpers::buildExpTable(kMIN_LVL_EXP, kMAX_LVL_EXP, kNUM_LEVELS);
}

/* Returns the curve modifier given a curve value and whether to check prim */
double Person::getCurveModifier(const ElementCurve &curve, 
                                const bool primary)
{
  if (primary) 
    return kPRIM_MODS.at(static_cast<uint8_t>(curve));
  return kSECD_MODS.at(static_cast<uint8_t>(curve));
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Adds an amount of experience and may update the level */
bool Person::addExp(const uint32_t &amount, const bool &update)
{
  
}

/* Prepares the person for entering Battle (flags, Attributes etc.) */
void Person::battlePrep()
{
  
}

/* Clears the skills the player has learned */
void Person::clearLearnedSkills()
{
  learned_skills.clear();
  updateSkills();
}

/* Shorthand function for dealing damage, returns true if the Person KO's */
bool Person::doDmg(const uint32_t &amount)
{
  
}

/* Method for printing out the data of the class */
void Person::print(const bool &simple = true, const bool &equips = false,
 const bool &flags = false, const bool &skills = false)
{
  
}


/* Removes the equipment from a given slot */
bool Person::removeEquip(const EquipSlots &equip_slot)
{
  
}


/* Returns the game_id (not unique) of the Person */
uint32_t Person::getGameID()
{
  
}


/* Returns the my_id (unique) of the Person */
uint32_t Person::getMyID()
{
  
}


/* Evaluates and returns the state of a given BattleState flag */
bool Person::getBattleFlag(const BattleState &test_flag)
{
  
}


/* Evaluates and returns the state of a given PersonState flag */
bool Person::getPersonFlag(cost PersonState &test_flag)
{
  
}


/* Returns a pointer to the assigned base person */
Person* Person::getBasePerson()
{
  
}


/* Returns a pointer to the battle class category */
Category* Person::getClass()
{
  
}


/* Returns a pointer to the race category */
Category* Person::getRace()
{
  
}


/* Returns the string name */
std::string Person::getName()
{
  
}


/* Returns the enumerated rank of the Person */
PersonRanks Person::getRank()
{
  
}


/* Returns the primary elemental affiliation */
Element Person::getPrimary()
{
  
}


/* Return the secondary elemental affiliation */
Element Person::getSecondary()
{
  
}


/* Return the enumerated curve of prim. elemental progression */
ElementCurve Person::getPrimaryCurve()
{
  
}


/* Return the enumerated curve of secd. elemental progression */
ElementCurve Person::getSecondaryCurve()
{
  
}


/* Returns a reference to the base stats */
AttributeSet& Person::getBase()
{
  
}


/* Returns a ref to the base max stats */
AttributeSet& Person::getBaseMax()
{
  
}


/* Returns a ref to the curr stats */
AttributeSet& Person::getCurr()
{
  
}


/* Returns a ref to the curr max stats */
AttributeSet& Person::getCurrMax()
{
  
}


/* Returns the temp stats */
AttributeSet& Person::getTemp()
{
  
}


/* Returns the base skills of the Person */
SkillSet* Person::getBaseSkills()
{
  
}


/* Returns the assigned current skills of the Person */
SkillSet* Person::getCurrSkills()
{
  
}


/* Returns the damage modifier value */
float Person::getExpMod()
{
  
}


/* Returns the experience modifier value */
float Person::getExpMod()
{
  
}


/* Returns a pointer to the equipment a given slot */
Equipment* Person::getEquip(const EquipSlots &equip_slot)
{
  
}


/* Returns the credit drop reward */
uint32_t Person::getCreditDrop()
{
  
}


/* Returns the exp drop reward */
uint32_t Person::getExpDrop()

/* Returns the current level */
uint8_t Person::getLevel()
{
  
}


/* Returns the total experience earned */
uint32_t Person::getTotalExp()
{
  
}


/* Grabs the curr frame (based BATTLE/BUBBIFIED flags) */
Frame* Person::getCurrFrame()
{
  
}


/* Grabs the vector of item IDs the person can drop */
std::vector<const uint32_t> Person::getItemDrops()
{
  
}


/* Evaluates and returns a given battle state flag */
void Person::setBattleFlag(const BattleState &flag, const bool set_value = true)
{
  
}


/* Evaluates and returns a given person state flag */
void Person::setPersonFlag(const PersonState &flag, const bool set_value = true)
{
  
}


/* Assigns a new curr Attr set */
void Person::setCurr(const AttributeSet& new_curr_set)
{
  
}


/* Assigns a new curr max Attr set */
void Person::setCurrMax(const AttributeSet& new_curr_max_set)
{
  
}


/* Assigns a new temp attr set */
void Person::setTemp(const AttributeSet& new_temp_set)
{
  
}


/* Assigns a new damage modifier value */
bool Person::setDmgMod(const float &new_dmg_modi)
{
  
}


/* Assigns a new experience modifier value */
bool setExpMod(const float &new_exp_modi)
{
  
}


/* Attempts to assign a given equipment slot a given equipment pointer */
bool Person::setEquip(const EquipSlots &equip_slot, Equipment* const &new_equipment)
{
  
}


/* Attempts to assign a new loop set for the person */
bool Person::setLoot(const uint32_t &new_credit_drop, const uint32_t &new_exp_drop, 
 const std::vector<const uint32_t> &new_item_drops)
{
  
}


/* Assigns the sprite pointers for the person */
bool Person::setSprites(Frame* const new_fp = nullptr, Frame* const new_tp = nullptr,
                  Frame* const new_fp_bubby = nullptr, 
                  Frame* const new_tp_bubby = nullptr)
{
  
}


/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
/* Grabs the experience required to reach a given level */
uint32_t Person::getExpAt(const uint8_t &level)
{
  
}


/* Grabs the number the total number of levels for Person progression */
size_t Person::getNumLevels()
{
  
}


/* Grabs the number of equipment slots a Person has */
size_t Person::getNumEquipSlots()
{
  
}


/* Returns the minimum experience possible */
uint32_t Person::getMinExp()
{
  
}


/* Returns the maximum experience possible */
uint32_t Person::getMaxExp()
{
  
}


/* Returns the minimum level experience */
uint32_t Person::getMinLvlExp()
{
  
}


/* Returns theexperience at the final level */
uint32_t Person::getMaxLvlExp()
{
  
}


/* Returns the maximum experience a Person can reward */
uint32_t Person::getMaxExpDrop()
{
  
}


/* Returns the maximmum # of item drops a Person can reward */
uint32_t Person::getMaxItemDrops()
{
  
}


/* Returns the minimum damage modifier possible */
float Person::getMinDmgModi()
{
  
}


/* Returns the maximum damage modifier possible */
float Person::getMaxDmgModi()
{
  
}


/* Returns the minimum experience modifier possible */
float Person::getMinExpModi()
{
  
}


/* Returns the maximum experience modifier possible */
float Person::getMaxExpModi()
{
  
}