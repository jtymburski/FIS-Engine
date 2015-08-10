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
 * base_stats - the level 1 stats of the Person calculated by battleclass + race
 * max_base_stats - the level max stats of the Person calc by battleclass + race
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
 ******************************************************************************/
#include "Game/Battle/AIModule.h"
#include "Game/Player/Person.h"

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/
const uint8_t  Person::kACTION_X = 60;
const uint8_t  Person::kACTION_Y = 128;
const size_t   Person::kNUM_LEVELS{127};
const size_t   Person::kNUM_EQUIP_SLOTS{5};
const uint32_t Person::kMAX_CREDIT_DROP{1000000}; /* 1 million */
const uint32_t Person::kMAX_EXP{1000000000}; /* 1 billion */
const uint32_t Person::kMAX_EXP_DROP{1000000}; /* 1 million */
const size_t   Person::kMAX_ITEM_DROPS{5};
const uint32_t Person::kMAX_LVL_EXP{100000000}; /* 100 million */
const uint32_t Person::kMIN_EXP{0};
const uint32_t Person::kMIN_LVL_EXP{500};
const float Person::kMIN_DMG_MODI{ 0.01};
const float Person::kMAX_DMG_MODI{10.00};
const float Person::kMIN_EXP_MODI{ 0.10};
const float Person::kMAX_EXP_MODI{10.00};
const int32_t Person::kUNSET_ID = -1;

/* Static ID counter */
int Person::id{0};

std::vector<uint32_t> Person::exp_table{};

/* XS, S, A, B, C, D */
const std::vector<float> Person::kPRIM_MODS =
{ 1.425, 1.400, 1.375, 1.350, 1.325, 1.300};

const std::vector<float> Person::kSECD_MODS =
{ 1.375, 1.300, 1.250, 1.225, 1.200, 1.175};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs an empty person object.
 *
 * Inputs: none
 */
Person::Person()
      : my_id{++id}
      , base_person{nullptr}
      , battle_class{nullptr}
      , race_class{nullptr}
{
  setupClass();
}

/*
 * Description: Constructs a base person object given a unique ID, name,
 *              battle_class, race_class.
 *
 * Inputs: game_id - the game ID of the base person to be created
 *         name - string name of the person to be created
 *         battle_class - pointer to the battle class
 *         race_class - pointer to the race class
 */
Person::Person(const int32_t &game_id, const std::string &name,
    Category* const battle_class, Category* const race_class)
      : game_id{game_id}
      , my_id{++id}
      , base_person{nullptr}
      , battle_class{battle_class}
      , race_class{race_class}
      , name{name}
{
  setupClass();
}

/*
 * Description: Creates a copy of a base person from a pointer to a base person
 *
 * Inputs: Person* - pointer to the base person
 */
Person::Person(Person* const source)
    : game_id{source->game_id}
    , my_id{++id}
    , base_person{source}
{
  setupClass();
}

/*
 * Description: Annihilates a Person object
 */
Person::~Person()
{
  /* Unset everything and clear memory */
  unsetAll(true);
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Loads the default settings for a Person
 *
 * Inputs: none
 * Output: none
 */
void Person::loadDefaults()
{
  action_x = kACTION_X;
  action_y = kACTION_Y;

  ai_module = nullptr;

  guardee = nullptr;
  guard   = nullptr;

  ailment_flags = static_cast<PersonAilState>(0);
  battle_flags  = static_cast<BState>(0);
  person_flags  = static_cast<PState>(0);

  setPFlag(PState::CAN_GAIN_EXP, true);
  setPFlag(PState::CAN_LEVEL_UP, true);
  setPFlag(PState::CAN_LEARN_SKILLS, true);
  setPFlag(PState::CAN_CHANGE_EQUIP, true);

  //person_record{};

  updateRank();

  primary         = Element::NONE;
  secondary       = Element::NONE;
  primary_curve   = ElementCurve::D;
  secondary_curve = ElementCurve::D;

  total_exp = kMIN_LVL_EXP;
  updateLevel();

  if(battle_class != nullptr && race_class != nullptr)
    updateBaseStats();

  curr_stats     = base_stats;
  curr_max_stats = base_stats;
  temp_max_stats = base_stats;

  if (base_skills != nullptr)
    std::cerr << "[Warning]: Missing deletion of base skills\n";
  if (curr_skills != nullptr)
    std::cerr << "[Warning]: Missing deletion of curr skills\n";
  if (learned_skills != nullptr)
    std::cerr << "[Warning]: Missing deletion of temp skills\n";

  base_skills    = nullptr;
  curr_skills    = nullptr;
  learned_skills = nullptr;
  temp_skills    = nullptr;

  if(battle_class != nullptr && race_class != nullptr)
    updateBaseSkills();

  dmg_mod = 1.000;
  exp_mod = 1.000;

  for (size_t a = 0; a < kNUM_EQUIP_SLOTS; a++)
    equipments.push_back(nullptr);

  credit_drop = 0;
  exp_drop = 0;

  action_sprite = nullptr;
  first_person  = nullptr;
  third_person  = nullptr;
  dialog_sprite = nullptr;
}

/*
 * Description: Performs the basic setup for a person such as building the
 *              experience table and dynamically allocates equipment. The base
 *              setup changes whether the Person is a base person or not.
 *
 * Inputs: none
 * Output: none
 */
void Person::setupClass()
{
  base_skills    = nullptr;
  curr_skills    = nullptr;
  learned_skills = nullptr;
  temp_skills    = nullptr;

  if (exp_table.empty())
    buildExpTable();

  /* Setup the class for a Base Person */
  if (base_person == nullptr)
  {
    loadDefaults();
  }

  /* Setup the class as a copy of the Base Person */
  else
  {
    action_x = base_person->action_x;
    action_y = base_person->action_y;

    /* Deep copied AI Module */
    if (base_person->ai_module != nullptr)
    {
      ai_module = new AIModule(base_person->ai_module->getDifficulty(),
                               base_person->ai_module->getPrimPersonality(),
                               base_person->ai_module->getSecdPersonality());
    }

    guardee       = base_person->guardee;
    guard         = base_person->guard;
    ailment_flags = base_person->ailment_flags;
    battle_flags  = base_person->battle_flags;
    person_flags  = base_person->person_flags;
    // TODO: Dynamic initialization of record? [10-25-14]
    //person_record = base-person->person_record;
    battle_class = base_person->battle_class;
    race_class = base_person->race_class;
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
    temp_skills = base_person->temp_skills;
    dmg_mod = base_person->dmg_mod;
    exp_mod = base_person->exp_mod;

    /* Equipments are deep-copied */
    for (auto equipment : base_person->equipments)
    {
      if (equipment != nullptr)
      {
        auto new_equip = new Equipment(equipment);
        equipments.push_back(new Equipment(new_equip));
      }
      else
        equipments.push_back(nullptr);
    }

    item_drops = base_person->item_drops;
    credit_drop = base_person->credit_drop;
    exp_drop = base_person->exp_drop;
    level = base_person->level;
    total_exp = base_person->total_exp;

    /* Comes from base person, if not null */
    first_person = base_person->first_person;
    third_person = base_person->third_person;
    dialog_sprite = base_person->dialog_sprite;
  }
}

/*
 * Description: Unsets all the parameters of a person.
 *
 * Inputs: bool - clears the memory of anything dynamically allocated by Person
 * Output: none
 */
void Person::unsetAll(const bool &clear)
{
  if (clear)
  {
    /* Delete the AI module */
    if (ai_module != nullptr)
      delete ai_module;

    /* Delete the equipments contained within the person */
    for (auto equipment_index : equipments)
    {
      if (equipment_index != nullptr)
      {
        if (!(equipment_index->getEquipFlag(EquipState::TWO_HANDED) &&
              equipment_index == getEquip(EquipSlots::RARM)))
          delete equipment_index;
      }

      equipment_index = nullptr;
    }

    /* Delete the skills sets */
    if (base_skills != nullptr)
      delete base_skills;
    if (curr_skills != nullptr)
      delete curr_skills;
    if (learned_skills != nullptr)
      delete learned_skills;
    if (temp_skills != nullptr)
      delete temp_skills;
  }

  //unsetSprites();

  ai_module      = nullptr;
  base_skills    = nullptr;
  curr_skills    = nullptr;
  learned_skills = nullptr;
  temp_skills    = nullptr;
}

void Person::unsetSprites()
{
  if(action_sprite != nullptr)
    if(base_person == nullptr || base_person->action_sprite != action_sprite)
      delete action_sprite;

  if(first_person != nullptr)
    if(base_person == nullptr || base_person->first_person != first_person)
      delete first_person;

  if(third_person != nullptr)
    if(base_person == nullptr || base_person->third_person != third_person)
      delete third_person;

  if(dialog_sprite != nullptr)
    if(dialog_sprite == nullptr || base_person->dialog_sprite != dialog_sprite)
      delete dialog_sprite;

  action_sprite = nullptr;
  first_person  = nullptr;
  third_person  = nullptr;
  dialog_sprite = nullptr;
}

/*
 * Description: Recalculates the Person's base stats and base max stats based
 *              on their battle class and their race class.
 *
 * Inputs: none
 * Output: none
 */
void Person::updateBaseStats()
{
  auto temp     = battle_class->getBaseSet();
  auto temp_max = battle_class->getTopSet();
  temp     += race_class->getBaseSet();
  temp_max += race_class->getTopSet();

  std::vector<int32_t> prim_indexes;
  std::vector<int32_t> secd_indexes;

  if (primary != Element::NONE)
  {
    prim_indexes.push_back(AttributeSet::getOffensiveIndex(primary));
    prim_indexes.push_back(AttributeSet::getDefensiveIndex(primary));

    auto p_mod = getCurveModifier(primary_curve, true);

    for (auto index : prim_indexes)
    {
      if (index != -1)
      {
        auto a = std::floor(static_cast<float>(temp.getStat(index)) * p_mod);
        auto b = std::floor(static_cast<float>(temp_max.getStat(index))*p_mod);

        temp.setStat(index, a);
        temp_max.setStat(index, b);
      }
    }
  }

  if (secondary != Element::NONE)
  {
    secd_indexes.push_back(AttributeSet::getOffensiveIndex(secondary));
    secd_indexes.push_back(AttributeSet::getDefensiveIndex(secondary));

    auto s_mod = getCurveModifier(secondary_curve, false);

    for (auto index : secd_indexes)
    {
      if (index != -1)
      {
        auto a = std::floor(static_cast<float>(temp.getStat(index)) * s_mod);
        auto b = std::floor(static_cast<float>(temp_max.getStat(index))*s_mod);

        temp.setStat(index, a);
        temp_max.setStat(index, b);
      }
    }
  }

  temp.cleanUp();
  temp_max.cleanUp();

  base_stats = temp;
  base_max_stats = temp_max;

  updateStats();
}

/*
 * Description: Updates the base skills of the Person based on the battle class
 *              and the race class.
 *
 * Inputs: none
 * Output: none
 */
void Person::updateBaseSkills()
{
  if (base_skills != nullptr)
    base_skills->clear();
  else
    base_skills = new SkillSet();

  if (battle_class->getSkills() != nullptr)
    *base_skills = *base_skills + *(battle_class->getSkills());

  if (race_class->getSkills() != nullptr)
    *base_skills = *base_skills + *(race_class->getSkills());

  updateSkills();
}

/*
 * Description: Updates the temp_max_stats attribute set to reflect the
 *              curr_max_stat attribute plus the buff sets for each of the
 *              equipped equipment.
 *
 * Inputs: none
 * Output: none
 */
void Person::updateEquipStats()
{
  auto equip_bonus = AttributeSet();

  for (auto it = begin(equipments); it != end(equipments); ++it)
    if ((*it) != nullptr)
      equip_bonus += (*it)->getStats();

  temp_max_stats = curr_max_stats + equip_bonus;
}

/*
 * Description: Updates he level of the Person based on their curren total
 *              experience.
 *
 * Inputs: none
 * Output: none
 */
/* Updates the level of the person based on their current total experience */
void Person::updateLevel()
{
  if (getPFlag(PState::CAN_LEVEL_UP))
  {
    auto before = level;

    level = getLevelAt(total_exp);

    if (level == kNUM_LEVELS)
      setPFlag(PState::MAX_LVL);

    if (level != before)
      updateStats();
  }
}

/*
 * Description: Updates the current stats and current max stats of the Person
 *              based on their level.
 *
 * Inputs: none
 * Output: none
 */
void Person::updateStats()
{
  if (level == 1)
  {
    curr_stats     = base_stats;
    curr_max_stats = base_stats;
  }
  else if (level == kNUM_LEVELS)
  {
    curr_stats     = base_max_stats;
    curr_max_stats = base_max_stats;
  }
  else
  {
    curr_stats     = AttributeSet(0, true);
    curr_max_stats = AttributeSet(0, true);

    for (size_t i = 0; i < AttributeSet::getSize(); i++)
    {
      std::vector<uint32_t> stat_values{};
      stat_values = Helpers::buildExpTable(base_stats.getStat(i),
                                           base_max_stats.getStat(i),
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

/*
 * Description: Updates the current total SkillSet of the person. This means
 *              adding the skills granted by equipment and the learned skills
 *              of the Person to the base skills.
 *
 * Inputs: none
 * Output: none
 */
void Person::updateSkills()
{
  if (curr_skills != nullptr)
    curr_skills->clear();
  else
    curr_skills = new SkillSet();

  if (learned_skills != nullptr)
    *curr_skills  += *learned_skills;

  if (base_skills != nullptr)
    *curr_skills  += *base_skills;

  for (auto equipment : equipments)
    if (equipment != nullptr)
      *curr_skills += equipment->getSkills();
}

/*
 * Description: Updates the rank of the Person based on their person record.
 *
 * Inputs: none
 * Output: nones
 */
void Person::updateRank()
{
  //rank = person_record.getRank();
}

/*
 * Description: Constructs the static table of experience values given the
 *              current level constants.
 *
 * Inputs: none
 * Output: none
 */
void Person::buildExpTable()
{
  exp_table = Helpers::buildExpTable(kMIN_LVL_EXP, kMAX_LVL_EXP, kNUM_LEVELS);
}

/*
 * Description: Returns the curve modifier value of a given curve value based
 *              on a bool whether to check the primary or secondary values.
 *
 * Inputs: curve - enumerated element Curve to be checked.
 *         bool - true if to check primary, false for secondary.
 * Output: float - the obtained curve modifier
 */
/* Returns the curve modifier given a curve value and whether to check prim */
float Person::getCurveModifier(const ElementCurve &curve,
                               const bool primary)
{
  auto index = static_cast<uint8_t>(curve);

  if (primary && index < kPRIM_MODS.size())
    return kPRIM_MODS.at(index);

  if (index < kSECD_MODS.size())
    return kSECD_MODS.at(index);

  return 0;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds an unsigned amount of experience to the Person and a levels
 *              them up (if necessary) if the update bool is assigned.
 *
 * Inputs: amount - the amount of experience to be added
 *         update - a boolean value whether to be updated
 * Output: bool - true if the experience was added successfully
 */
bool Person::addExp(const uint32_t &amount, const bool &update)
{
  auto can_add = false;

  if (getPFlag(PState::CAN_GAIN_EXP))
  {
    auto mod_amount = std::floor(amount * exp_mod);

    if (total_exp + mod_amount < kMAX_EXP)
    {
      total_exp += mod_amount;
      can_add = true;
    }
    else if (total_exp != kMAX_EXP)
    {
      total_exp = kMAX_EXP;
      can_add = true;
    }

    for (auto equipment : equipments)
      if (equipment != nullptr)
        equipment->getSignature()->addExp(amount);
  }

  if (can_add && update)
    updateLevel();

  return can_add;
}

float Person::calcVitaPercentAtVal(uint32_t target_value)
{
  auto target_vita = static_cast<int32_t>(target_value);
  auto max_vita = getTemp().getStat(Attribute::VITA);

  return (max_vita != 0) ? ((float)target_vita / (float)max_vita) : (0);
}

float Person::calcQtdrPercentAtVal(uint32_t target_value)
{
  auto target_qtdr = static_cast<int32_t>(target_value);
  auto max_qtdr = getTemp().getStat(Attribute::QTDR);

  return (max_qtdr != 0) ? ((float)target_qtdr / (float)max_qtdr) : (0);
}

/*
 * Description: Calculate and determine the percentage of experience
 *              gained towards the next level. If the person is at max level,
 *              the experience towards the next level will be 100 forever.
 *
 * Inputs: none
 * Output: uint16_t - percentage towards the next level
 */
uint16_t Person::findExpPercent()
{
  uint16_t pc_exp = 0;

  if (level < kNUM_LEVELS)
  {
    auto curr_exp = findExpThisLevel();
    auto need_exp = Person::getExpAt(level + 1) - Person::getExpAt(level);

    pc_exp = static_cast<uint16_t>(floor((curr_exp * 100)/ need_exp));
  }
  else
  {
    pc_exp = 100;
  }

  return pc_exp;
}

/*
 * Description: Find the amount of experience per percentage of this level.
 *
 * Inputs: none
 * Output: uint32_t - amount of experience for 1% of this level
 */
int32_t Person::findExpPerPC()
{
  if (level < kNUM_LEVELS)
  {
    auto level_exp = Person::getExpAt(level + 1) - Person::getExpAt(level);
    return static_cast<uint32_t>(floor(level_exp / 100));
  }

  return -1;
}

/*
 * Description: Calculates and returns the experience (amount value) gained
 *              this level, or 0 if the person is at max level.
 *
 * Inputs: none
 * Output: uint16_t - experience gained at this level
 */
uint16_t Person::findExpThisLevel()
{
  auto curr_exp = 0;

  if (level < kNUM_LEVELS)
  {
    auto level_below_exp = Person::getExpAt(level);
    curr_exp = total_exp - level_below_exp;
  }

  return curr_exp;
}

/*
 * Description: Removes an amount of experience from the person but not from
 *              their equipment.
 *
 * Inputs: amount - the amount to take away
 *         update - whether to update the level
 * Output: bool - whether the loss of experience was possible
 */
bool Person::loseExp(const uint32_t &amount, const bool &update)
{
  if (amount >= total_exp)
    total_exp -= amount;

  if (update)
    updateLevel();

  return true;
}

/*
 * Description: Removes a certain percentage of gained experience towards the
 *              next level. This function will not de-level the person, nor
 *              will it remove experience from equipment. Giving any value
 *              larger 99 will remove all gained experience towards the next
 *              level
 *
 * Inputs: percentage - the percentage to take away gained experience.
 * Output: bool - whether a loss of experience happened properly
 */
bool Person::loseExpPercent(const uint16_t &percent)
{
  auto lost_exp = false;
  auto exp_pc = findExpPercent();

  /* If the % to lose is greater than % gained, remove all exp this level */
  if (percent > exp_pc)
  {
    auto exp_gained = findExpThisLevel();
    total_exp -= exp_gained;
    lost_exp = true;
  }
  else if (percent > 0)
  {
    total_exp -= findExpPerPC() * percent;
    lost_exp = true;
  }

  return lost_exp;
}

/*
 * Description: Prepares a person for entering Battle (flags, Attributes, etc.)
 *
 * Inputs: none
 * Output: none
 */
void Person::battlePrep()
{
  curr_stats = curr_max_stats;
  updateEquipStats();
  updateSkills();
  resetSkills();

  setAilFlag(PersonAilState::RANDOM_SELECTION, false);
  setAilFlag(PersonAilState::SKIP_NEXT_TURN, false);
  setAilFlag(PersonAilState::MISS_NEXT_TARGET, false);
  setAilFlag(PersonAilState::NEXT_ATK_NO_EFFECT, false);
  setAilFlag(PersonAilState::TWO_SKILLS, false);
  setAilFlag(PersonAilState::THREE_SKILLS, false);
  setAilFlag(PersonAilState::HALF_COST, false);
  setAilFlag(PersonAilState::REFLECT, false);
  setAilFlag(PersonAilState::BOND, false);
  setAilFlag(PersonAilState::BONDED, false);
  setAilFlag(PersonAilState::IS_BUBBY, false);
  setAilFlag(PersonAilState::IS_MODULATED, false);
  setBFlag(BState::IN_BATTLE, true);
  setBFlag(BState::ALIVE, true);
  setBFlag(BState::REVIVABLE, false);
  setBFlag(BState::IS_SELECTING, false);
  setBFlag(BState::SELECTED_ACTION, false);
  setBFlag(BState::SELECTED_2ND_ACTION, false);
  setBFlag(BState::SELECTED_3RD_ACTION, false);
  setBFlag(BState::CAN_CRIT, true);
  setBFlag(BState::CAN_BE_CRIT, true);
  setBFlag(BState::DEFENDING, false);
  setBFlag(BState::GUARDED, false);
  setBFlag(BState::GUARDING, false);
  setBFlag(BState::SHIELDED, false);
  setBFlag(BState::IS_ATTACKING, false);

  resetActionFlags();
}

/*
 * Description: Prepares the person for the beginning of a turn during a
 *              battle.
 *
 * Inputs: none
 * Output: none
 */
void Person::battleTurnPrep()
{
  setBFlag(BState::SELECTED_ACTION, false);
  setBFlag(BState::SELECTED_2ND_ACTION, false);
  setBFlag(BState::SELECTED_3RD_ACTION, false);
  //setBFlag(BState::TWO_SKILLS, true);
}

/*
 * Description: Clears the learned skills of the Person
 *
 * Inputs: none
 * Output: none
 */
void Person::clearLearnedSkills()
{
  learned_skills->clear();
  updateSkills();
}

/*
 * Description: Creates a new AIModule for the person given a difficulty and
 *              a primary and secodary personality type
 *
 * Inputs: diff - the difficulty the AI will have
 *         prim_personality - the primary personality for the AI
 *         secd_personality - the secondary personality for the AI
 * Output: bool - true if the old AIModule underwent annihilation
 */
bool Person::createAI(const AIDifficulty &diff,
                      const AIPersonality &prim_personality,
                      const AIPersonality &secd_personality)
{
  auto destroyed = false;

  if (ai_module != nullptr)
  {
    delete ai_module;
    ai_module = nullptr;
    destroyed = true;
  }

  ai_module = new AIModule (diff, prim_personality, secd_personality);
  ai_module->setParent(this);

  return destroyed;
}

/*
 * Description: Shorthand function for dealing damage to the Person. Returns
 *              true if the Person's HP is 0 after the damage takes place.
 *
 * Inputs: amount - amount of damage to deal
 * Output: bool - true if the Person is now dead
 */
bool Person::doDmg(const uint32_t &amount)
{
  curr_stats.alterStat(Attribute::VITA, -amount);

  return (curr_stats.getStat(Attribute::VITA) <= 0);
}

/*
 * Description: Determines whether the person is a power defender (whether they
 *              can persist in defending against multiple attacks)
 *
 * Inputs: none
 * Output: bool - true if the person is a power defender
 */
bool Person::isPowerDefender()
{
  /* If either battle class or race class allows the person to be a power
   * defender, the person will be said to be a power defender */
  if (battle_class->getFlag(CategoryState::POWER_DEFENDER) ||
      race_class->getFlag(CategoryState::POWER_DEFENDER))
  {
    return true;
  }

  return false;
}

/*
 * Description: Determines whether the person is a power guarder (whether they
 *              can persist in shielding their guardee from multiple attacks)
 *
 * Inputs: none
 * Output: bool - true if person is a power guarder
 */
bool Person::isPowerGuarder()
{
  if (battle_class->getFlag(CategoryState::POWER_GUARDER)  ||
      race_class->getFlag(CategoryState::POWER_GUARDER))
  {
    return true;
  }

  return false;
}

/*
 * Description: Method for printing out the data describing a person
 *
 * Inputs: simple - true for a simplified version of a person, false otherwise
 *         equips - true to print out the equipment of the person
 *         flags - true to print out each flag of the person
 *         skills - true to print out information for each skill
 * Output: none
 */
void Person::print(const bool &simple, const bool &equips,
                   const bool &flags, const bool &skills)
{
  std::cout << "==== Person: " << getName() << " ====\n";
  if (simple)
  {
    std::cout << "GID: " << game_id << " MID: " << my_id << " Name: " << name
              << " Level: " << static_cast<int>(level) << " Exp: " << total_exp
              << "\n";
  }
  else
  {
    std::cout << "Game ID: " << game_id << "\n";
    std::cout << "My ID: " << my_id << "\n";
    std::cout << "Base Person? " << (base_person == nullptr) << "\n";
    std::cout << "Battle Class: " << battle_class->getName() << "\n";
    std::cout << "Race? " << race_class->getName() << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "[void]Rank " << "\n";
    std::cout << "Primary: " << Helpers::elementToString(primary) << "\n";
    std::cout << "Secondary: " << Helpers::elementToString(secondary);
    std::cout << "\nPrim Curve: " << static_cast<int>(primary_curve);
    std::cout << "\nSecd Curve: " << static_cast<int>(secondary_curve);
    std::cout << "\nDmg Modifier: " << dmg_mod << "\n";
    std::cout << "Exp Modifier: " << exp_mod << "\n";
    std::cout << "Item Drops: " << item_drops.size() << "\n";
    std::cout << "Credit Drop: " << credit_drop << "\n";
    std::cout << "Exp Drop: " << exp_drop << "\n";
    std::cout << "Level: " << static_cast<int>(level) << "\n";
    std::cout << "Total Exp: " << total_exp << "\n";
    std::cout << "To Next Lvl: " << findExpPerPC() << "\n";
    std::cout << "First Person? " << (first_person != nullptr) << "\n";
    std::cout << "Third Person? " << (third_person != nullptr) << "\n";
    std::cout << "VITA Regn: " << Helpers::regenRateToStr(getVitaRegenRate());
    std::cout << "\nQTDR Regn: " << Helpers::regenRateToStr(getQDRegenRate());
    std::cout << "\n\n";

    std::cout << "Base Stats: ";
    base_stats.print(true);
    std::cout << "\n";
    std::cout << "Base Max Stats: ";
    base_max_stats.print(true);
    std::cout << "\n";
    std::cout << "Curr Stats: ";
    curr_stats.print(true);
    std::cout << "\n";
    std::cout << "Curr Max Stats: ";
    curr_max_stats.print(true);
    std::cout << "\n";
    std::cout << "Temp Max Stats: ";
    temp_max_stats.print(true);
    std::cout << "\n";

    if (skills)
    {
      std::cout << "Base Skill Set: ";

      if (base_skills != nullptr)
        base_skills->print();

      std::cout << "\nCurr Skill Set: ";

      if (curr_skills != nullptr)
        curr_skills->print();

      std::cout << "\nLearned Skills: ";

      if (learned_skills != nullptr)
        learned_skills->print();

      std::cout << "\n";
    }

    if (equips)
      for (auto equipment : equipments)
        if (equipment != nullptr)
          equipment->print();

    if (flags)
    {
      std::cout << "--- Battle State Flags ---\n";
      std::cout << "IN_BATTLE: " << getBFlag(BState::IN_BATTLE) << "\n";
      std::cout << "ALIVE: " << getBFlag(BState::ALIVE) << "\n";
      std::cout << "ATK_ENABLED: " << getBFlag(BState::ATK_ENABLED) << "\n";
      std::cout << "SKL_ENABLED: " << getBFlag(BState::SKL_ENABLED) << "\n";
      std::cout << "ITM_ENABLED: " << getBFlag(BState::ITM_ENABLED) << "\n";
      std::cout << "DEF_ENABLED: " << getBFlag(BState::DEF_ENABLED) << "\n";
      std::cout << "GRD_ENABLED: " << getBFlag(BState::GRD_ENABLED) << "\n";
      std::cout << "IMP_ENABLED: " << getBFlag(BState::IMP_ENABLED) << "\n";
      std::cout << "RUN_ENABLED: " << getBFlag(BState::RUN_ENABLED) << "\n";
      std::cout << "PAS_ENABLED: " << getBFlag(BState::PAS_ENABLED) << "\n";
      // std::cout << "SKIP_NEXT_TURN: " << getBFlag(BState::SKIP_NEXT_TURN);
      // std::cout << "\nMISS_NEXT_TARGET: " << getBFlag(BState::MISS_NEXT_TARGET);
      // std::cout << "\nNEXT_ATK_NO_EFFECT: "
      //           << getBFlag(BState::NEXT_ATK_NO_EFFECT);
      // std::cout << "\nIS_BUBBY: " << getBFlag(BState::IS_BUBBY) << "\n";
      // std::cout << "TWO_SKILLS: " << getBFlag(BState::TWO_SKILLS) << "\n";
      // std::cout << "THREE_SKILLS: " << getBFlag(BState::THREE_SKILLS) << "\n";
      // std::cout << "HALF_COST: " << getBFlag(BState::HALF_COST) << "\n";
      // std::cout << "REFLECT: " << getBFlag(BState::REFLECT) << "\n";
      // std::cout << "BOND: " << getBFlag(BState::BOND) << "\n";
      // std::cout << "BONDED: " << getBFlag(BState::BONDED) << "\n";
      // std::cout << "REVIVABLE: " << getBFlag(BState::REVIVABLE);
      std::cout << "\nIS_SELECTING: " << getBFlag(BState::IS_SELECTING);
      std::cout << "\nSELECTED_ACTION: " << getBFlag(BState::SELECTED_ACTION);
      std::cout << "\nSELECTED_2ND_ACTION: "
                << getBFlag(BState::SELECTED_2ND_ACTION);
      std::cout << "\nSELECTED_3RD_ACTION: "
                << getBFlag(BState::SELECTED_3RD_ACTION);
      std::cout << "\nCAN_CRIT: " << getBFlag(BState::CAN_CRIT);
      std::cout << "\nCAN_BE_CRIT: " << getBFlag(BState::CAN_BE_CRIT);
      std::cout << "\nDEFENDING: " << getBFlag(BState::DEFENDING);
      std::cout << "\nGUARDED: " << getBFlag(BState::GUARDED);
      std::cout << "\nGUARDING: " << getBFlag(BState::GUARDING);
      std::cout << "\nSHIELDED: " << getBFlag(BState::SHIELDED);
      std::cout << "\nIS_ATTACKING: " << getBFlag(BState::IS_ATTACKING);

      std::cout << "\n\n--- Person State Flags ---\n";
      std::cout << "SLEUTH: " << getPFlag(PState::SLEUTH) << "\n";
      std::cout << "BEARACKS: " << getPFlag(PState::BEARACKS) << "\n";
      std::cout << "MAIN: " << getPFlag(PState::MAIN) << "\n";
      std::cout << "FINAL: " << getPFlag(PState::FINAL) << "\n";
      std::cout << "BOSS: " << getPFlag(PState::BOSS) << "\n";
      std::cout << "MINI_BOSS: " << getPFlag(PState::MINI_BOSS) << "\n";
      std::cout << "CAN_GAIN_EXP: " << getPFlag(PState::CAN_GAIN_EXP) << "\n";
      std::cout << "CAN_LEVEL_UP: " << getPFlag(PState::CAN_LEVEL_UP) << "\n";
      std::cout << "CAN_LEARN_SKILLS: " << getPFlag(PState::CAN_LEARN_SKILLS)
                << "\n";
      std::cout << "CAN_CHANGE_EQUIP: " << getPFlag(PState::CAN_CHANGE_EQUIP)
                << "\n";
      std::cout << "MAX_LVL: " << getPFlag(PState::MAX_LVL) << "\n";
    }
  }
  std::cout << "--- // Person ---\n\n";
}

/*
 * Description: Attempts to remove equipment from a given equip slot. Removes
 *              true if an equipment at that slot was found and it was removed.
 *
 * Note [1]: This function will cause a memory leak unless the equipment is
 *           grabbed before the removal takes place!
 *
 * Inputs: equip_slot - enumerated equip slot to remove the equipment for
 * Output: bool - true if a piece of equipment was there and removed
 */
bool Person::removeEquip(const EquipSlots &equip_slot)
{
  auto removed = false;

  if (getEquip(equip_slot) == nullptr)
    return false;

  auto equip = getEquip(equip_slot);

  switch (equip_slot)
  {
    case (EquipSlots::LARM):
    case (EquipSlots::RARM):
      if (equip->getEquipFlag(EquipState::TWO_HANDED))
      {
        equipments.at(1) = nullptr;
        equipments.at(2) = nullptr;
      }
      else if (equip_slot == EquipSlots::LARM)
        equipments.at(1) = nullptr;
      else if (equip_slot == EquipSlots::RARM)
        equipments.at(2) = nullptr;

      removed = true;
      break;
    case (EquipSlots::HEAD):
      equipments.at(0) = nullptr;
    case (EquipSlots::BODY):
      equipments.at(3) = nullptr;
    case (EquipSlots::LEGS):
      equipments.at(4) = nullptr;
    default:
      std::cerr << "Warning: Attempting removal of invalid equip slot.\n";
  }

  return removed;
}

/*
 * Description: Resets the action flags (types of useable actions) to their
 *              standard states (defaults plus category defaults).
 *
 * Inputs: none
 * Output: none
 */
void Person::resetActionFlags()
{
  setBFlag(BState::ATK_ENABLED, true);
  setBFlag(BState::SKL_ENABLED, true);
  setBFlag(BState::ITM_ENABLED, false); //TODO: Temporarily disabled  [08-09-14]
  setBFlag(BState::INS_ENABLED, false);//TODO: What enables inspect? [08-09-14]
  setBFlag(BState::RUN_ENABLED, true);
  setBFlag(BState::PAS_ENABLED, true);

  setBFlag(BState::DEF_ENABLED,
           battle_class->getFlag(CategoryState::DEF_ENABLED));
  //TODO: Enable guard
  setBFlag(BState::GRD_ENABLED, false);
  // setBFlag(BState::GRD_ENABLED,
  //          battle_class->getFlag(CategoryState::GRD_ENABLED));
  setBFlag(BState::IMP_ENABLED,
           battle_class->getFlag(CategoryState::IMP_ENABLED));
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Person::resetAI()
{
  if (ai_module == nullptr)
    return false;

  ai_module->incrementTurns();
  ai_module->resetForNewTurn(this);

  return true;
}

/*
 * Description: Resets the Person pointer guard and GUARDED flag of this person
 *              and returns true if the guard status has properly changed.
 *
 * Inputs: none
 * Output: bool - true if the guard status was reset properly
 */
bool Person::resetGuard()
{
  bool good_reset = false;

  /* Proper reset if guard was assigned as well as GUARDED flag */
  good_reset &= (guard != nullptr && getBFlag(BState::GUARDED));

  /* Unassign the guard and turn off GUARDED flag */
  guard = nullptr;
  setBFlag(BState::GUARDED, false);

  return good_reset;
}

/*
 * Description: Resets the defending status of the person. Returns true if
 *              the defending status was removed from a defending state.
 *
 * Inputs: none
 * Output: bool - true if the person was defending and now no longer is
 */
bool Person::resetDefend()
{
  bool good_defend = false;

  good_defend &= getBFlag(BState::DEFENDING);

  setBFlag(BState::DEFENDING, false);

  return good_defend;
}

/*
 * Description: Resets the guarding status of this person. Returns true if
 *              this person was being a person and now no longer is.
 *
 * Inputs: none
 * Output: bool - true if guarding state was set and no longer is
 */
bool Person::resetGuardee()
{
  bool good_reset = false;

  good_reset &= (guardee != nullptr && getBFlag(BState::GUARDING));

  guardee = nullptr;
  setBFlag(BState::GUARDING, false);

  return good_reset;
}

/*
 * Description: Resets the state of the users current skill set for proper
 *              use in Battle. updateSkils() should be called prior.
 *
 * Inputs: none
 * Output: none
 */
void Person::resetSkills()
{
  auto elements = curr_skills->getElements(level);

  for (auto& element : elements)
    element.silenced = false;
}

Sprite* Person::getActionSprite()
{
  return action_sprite;
}

/*
 * Description: Returns the action frames for the person - for the rendering
 *              swipe-away.
 *
 * Inputs: none
 * Output: Sprite* - the sprite pointer
 */
Sprite* Person::getDialogSprite()
{
  return dialog_sprite;
}

/*
 * Description: Returns the rendering action X offset in the swipe away in the
 *              battle.
 *
 * Inputs: none
 * Output: int16_t - x offset
 */
int16_t Person::getActionX()
{
  return action_x;
}

/*
 * Description: Returns the rendering action Y offset in the swipe away in the
 *              battle.
 *
 * Inputs: none
 * Output: int16_t - y offset
 */
int16_t Person::getActionY()
{
  return action_y;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
AIModule* Person::getAI()
{
  return ai_module;
}

/*
 * Description: Returns the non-unique game ID of the Person
 *
 * Inputs: none
 * Output: uint32_t - game ID of the Person
 */
int32_t Person::getGameID() const
{
  return game_id;
}

/*
 * Description: Returns the unique my_id of the Person
 *
 * Inputs: none
 * Output: uint32_t - the unique ID of the Person
 */
int32_t Person::getMyID()
{
  return my_id;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool Person::getAilFlag(const PersonAilState &test_flag)
{
  return static_cast<bool>((ailment_flags & test_flag) == test_flag);
}

/*
 * Description: Evaluates and returns the state of a given BState flag
 *
 * Inputs: test_flag - BState flag to find the value for
 * Output: bool - the value of the given flag to check
 */
bool Person::getBFlag(const BState &test_flag)
{
  return static_cast<bool>((battle_flags & test_flag) == test_flag);
}

/*
 * Description: Evaluates and returns the state of a given PState flag
 *
 * Inputs: test_flag - PState flag to find the value for
 * Output: bool - the value of the given flag to check
 */
bool Person::getPFlag(const PState &test_flag)
{
  return static_cast<bool>((person_flags & test_flag) == test_flag);
}

/*
 * Description: Returns a pointer to the assigned base person
 *
 * Inputs: none
 * Output: Person* - pointer to the assigned base person
 */
Person* Person::getBasePerson()
{
  return base_person;
}

/*
 * Description: Returns the pointer to the person who is guarding this person
 *
 * Inputs: none
 * Output: Person* - person guarding this person
 */
Person* Person::getGuard()
{
  return guard;
}

/*
 * Description: Returns the pointer to the person being guarded by this person
 *
 * Inputs: none
 * Output: Person* - the person being guarded
 */
Person* Person::getGuardee()
{
  return guardee;
}

/*
 * Description: Returns a pointer to the battle class category
 *
 * Inputs: none
 * Output: Category* - pointer to the base class
 */
Category* Person::getClass()
{
  return battle_class;
}

/*
 * Description: Returns a pointer to the race class category
 *
 * Inputs: none
 * Output: Category* - pointer to the race class
 */
Category* Person::getRace()
{
  return race_class;
}

/*
 * Description: Returns the string name of the Person
 *
 * Inputs: none
 * Output: std::string - string name of the Person
 */
std::string Person::getName() const
{
  return name;
}

/*
 * Description: Returns the enumerated rank of the Person
 *
 * Inputs: none
 * Output: Ranks - enumerated rank of the Person
 */
Ranks Person::getRank()
{
  return rank;
}

/*
 * Description: Returns the enumerated primary affiliation of the Person
 *
 * Inputs: none
 * Output: Element - the enumerated primary affiliation of the Person
 */
Element Person::getPrimary()
{
  return primary;
}

/*
 * Description: Returns the enumerated secondary affiliation of the Person
 *
 * Inputs: none
 * Output: Element - the enumerated secondary affiliation of the Person
 */
Element Person::getSecondary()
{
  return secondary;
}

/*
 * Description: Return the enumerated curve of prim. elemental progression
 *
 * Inputs: none
 * Output: ElementCurve - the enumerated curve of prim. elemental progression
 */
ElementCurve Person::getPrimaryCurve()
{
  return primary_curve;
}

/*
 * Description: Return the enumerated curve of secd. elemental progression
 *
 * Inputs: none
 * Output: ElementCurve - the enumerated curve of secd. elemental progression
 */
ElementCurve Person::getSecondaryCurve()
{
  return secondary_curve;
}

/*
 * Description: Return a reference to the base stats of the Person
 *
 * Inputs: none
 * Output: AttributeSet& - ref. to the base stats of the Person
 */
AttributeSet& Person::getBase()
{
  return base_stats;
}

/*
 * Description: Return a reference to the base max stats of the Person
 *
 * Inputs: none
 * Output: AttributeSet& - ref. to the base max stats of the Person
 */
AttributeSet& Person::getBaseMax()
{
  return base_max_stats;
}

/*
 * Description: Returns a reference to the current stats of the Person
 *
 * Inputs: none
 * Output: AttributeSet& - ref. to the curr stats of the Person
 */
AttributeSet& Person::getCurr()
{
  return curr_stats;
}

/*
 * Description: Returns a reference to the current max stats of the Person
 *
 * Inputs: none
 * Output: AttributeSet& - ref. to the curr max stats of the Person
 */
AttributeSet& Person::getCurrMax()
{
  return curr_max_stats;
}

/*
 * Description: Return the temp stats of the Person
 *
 * Inputs: none
 * Output: AttributeSet& - ref. to the temp stats of the Person
 */
AttributeSet& Person::getTemp()
{
  return temp_max_stats;
}

/*
 * Description: Returns the pointer to the base skills of the Person
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the base skills of the Person
 */
SkillSet* Person::getBaseSkills()
{
  return base_skills;
}

/*
 * Description: Returns the current SkillSet of the Person.
 *
 * Inputs: none
 * Output: SkillSet* - pointer to the current SkillSet of the Person
 */
SkillSet* Person::getCurrSkills()
{
  return curr_skills;
}

/*
 * Description: Returns the damage modifier of the Person
 *
 * Inputs: none
 * Output: float - the damage modifier of the Person.
 */
float Person::getDmgMod()
{
  return dmg_mod;
}

/*
 * Description: Returns the experience modifier of the Person
 *
 * Inputs: none
 * Output: float - the experience modifier of the Person.
 */
float Person::getExpMod()
{
  return exp_mod;
}

/*
 * Description: Returns a pointer to the equipment a given slot
 *
 * Inputs: equip_slot - the equip slot to check the equipment for
 * Output: Equipment* - pointer to the equipment at the enumerated slot
 */
Equipment* Person::getEquip(const EquipSlots &equip_slot)
{
  auto index = getEquipIndex(equip_slot);

  if (index < equipments.size())
    return equipments.at(index);

  return nullptr;
}

/*
 * Description: Grabs the corresponding index of a given EquipSlots enumeration
 *
 * Inputs: none
 * Output: uint32_t - the mapped index of a given equip slot
 */
uint32_t Person::getEquipIndex(const EquipSlots &equip_slot)
{
  if (static_cast<uint8_t>(equip_slot) < equipments.size())
  {
    switch (equip_slot)
    {
      case (EquipSlots::HEAD):
        return 0;
      case (EquipSlots::LARM):
        return 1;
      case (EquipSlots::RARM):
        return 2;
      case (EquipSlots::BODY):
        return 3;
      case (EquipSlots::LEGS):
        return 4;
      default:
        std::cerr << "Error: Checking invalid EquipSlots enum";
        break;
    }
  }
  return equipments.size() + 1;
}

/*
 * Description: Returns the assigned credit drop awarded upon defeat of this
 *
 * Inputs: none
 * Output: uint32_t - the credit drop awarded
 */
uint32_t Person::getCreditDrop()
{
  return credit_drop;
}

/*
 * Description: Returns the assigned exp drop awarded upon defeat of this
 *
 * Inputs: none
 * Output: uint32_t - the experience drop awarded
 */
uint32_t Person::getExpDrop()
{
  return exp_drop;
}

/*
 * Description: Returns the level of the person
 *
 * Inputs: none
 * Output: uint8_t - the level of the Person
 */
uint16_t Person::getLevel()
{
  return level;
}

/*
 * Description: Returns the total experience of the Person
 *
 * Inputs: none
 * Output: uint32_t - the total experience of the Person
 */
uint32_t Person::getTotalExp()
{
  return total_exp;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float Person::getQDPercent()
{
  return calcQtdrPercentAtVal(getCurr().getStat(Attribute::QTDR));
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float Person::getVitaPercent()
{
  return calcVitaPercentAtVal(getCurr().getStat(Attribute::VITA));
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int16_t Person::getTrueCost(Skill* test_skill)
{
  if (temp_skills != nullptr && !getAilFlag(PersonAilState::IS_BUBBY))
  {
    auto elements = temp_skills->getElements(level);

    for (auto it = begin(elements); it != end(elements); ++it)
    {
      if ((*it).skill == test_skill)
      {
        if (getAilFlag(PersonAilState::HALF_COST))
          return (*it).skill->getCost() / 2;

        return (*it).skill->getCost();
      }
    }
  }
  else if (getAilFlag(PersonAilState::IS_BUBBY))
  {
    if (getAilFlag(PersonAilState::HALF_COST))
      return test_skill->getCost() / 2;

    return test_skill->getCost();
  }

  return AttributeSet::getMaxValue();
}

/*
 * Description: Grabs the currentassigned first person frame (based on the
 *              IS_BUBBY BState flag)
 *
 * Inputs: none
 * Output: Sprite* - pointer to the correct current first person sprite
 */
Sprite* Person::getFirstPerson()
{
  if(base_person != nullptr)
    return base_person->first_person;
  return first_person;
}

/*
 * Description: Grabs the current assigned third person frame (based on the
 *              IS_BUBBY BState flag)
 *
 * Inputs: none
 * Output: Sprite* - pointer to the correct current third person sprite
 */
Sprite* Person::getThirdPerson()
{
  if(base_person != nullptr)
    return base_person->third_person;
  return third_person;
}

/*
 * Description: Grabs the vector of item IDs the person can drop
 *
 * Inputs: none
 * Output: std::vector<uint32_t> - the vector of item drops.
 */
/* Grabs the vector of item IDs the person can drop */
std::vector<uint32_t> Person::getItemDrops()
{
  return item_drops;
}

/*
 * Description: Calculates the total QD regeneration rate from the battle class
 *              and race class bonuses and returns the enumerated result.
 *
 * Inputs: none
 * Output: RegenRate - enumerated regen rate for quantum drive / turn
 */
RegenRate Person::getQDRegenRate()
{
  auto total_rate = static_cast<int16_t>(battle_class->getQDRegenRate()) +
      static_cast<int16_t>(race_class->getQDRegenRate());

  if (total_rate <= static_cast<uint16_t>(RegenRate::GRAND))
    return static_cast<RegenRate>(total_rate);
  else
    return RegenRate::GRAND;

  return RegenRate::ZERO;
}

/*
 * Description: Calculates the total Vita regeneration rate from the battle
 *              class and race class bonuses and returns the enumerated result.
 *
 * Inputs: none
 * Output: RegenRate - enumerated regen rate for vitality / turn
 */
RegenRate Person::getVitaRegenRate()
{
  auto total_rate = static_cast<int16_t>(battle_class->getVitaRegenRate()) +
      static_cast<int16_t>(race_class->getVitaRegenRate());

  if (total_rate <= static_cast<uint16_t>(RegenRate::GRAND))
    return static_cast<RegenRate>(total_rate);
  else
    return RegenRate::GRAND;

  return RegenRate::ZERO;
}

/*
 * Description: Calculates a vector of Skill ptrs containing all Skills the
 *              Person has current access to use for the Battle based upon
 *              their attribute values and the current settings of flags.
 *
 * Inputs: none
 * Output: SkillSet* useable skills
 */
SkillSet* Person::getUseableSkills()
{
  /* Delete the old temp skills ptr if set */
  if (temp_skills != nullptr)
    delete temp_skills;

  /* Create a new temporary skills ptr */
  temp_skills = new SkillSet();

  /* Grab the SetElement useable from curr skills ptr at a given lvl */
  std::vector<SetElement> useable_skills;
  auto elements = curr_skills->getElements(level);

  for (auto it = begin(elements); it != end(elements); ++it)
  {
    auto add_skill = true;

    if ((*it).enabled && !(*it).silenced)
    {
      auto skill_cost = static_cast<int32_t>((*it).skill->getCost());

      /* Adjust for Half Cost of skills */
      if (getAilFlag(PersonAilState::HALF_COST))
        if (skill_cost != 1)
          skill_cost /= 2;

      /* If the adjusted QD cost is 0, assert that ATK is enabled */
      if (skill_cost == 0 || (*it).skill->isBerserkSkill())
        add_skill &= getBFlag(BState::ATK_ENABLED);
      /* If the adjusted QD cost is > 0, asert that SKL is enabled */
      else
        add_skill &= getBFlag(BState::SKL_ENABLED);

      add_skill &= (skill_cost <= temp_max_stats.getStat(Attribute::QTDR));

      /* If the skill can still be added, push it to the useable elements */
      if (add_skill)
        useable_skills.push_back(*it);
    }
  }

  if (temp_skills->addSkills(useable_skills))
    return temp_skills;

  return nullptr;
}

/*
 * Description: Calculates and prints the valid action types which this person
 *              may perform in a battle in their current state
 *
 * Inputs: none
 * Output: std::vector<ActionType> - vector of enuemrated action types possible
 */
std::vector<ActionType> Person::getValidActions()
{
  std::vector<ActionType> valid_action_types;

  if (getBFlag(BState::ATK_ENABLED) ||
      getBFlag(BState::SKL_ENABLED))
    valid_action_types.push_back(ActionType::SKILL);

  if (getBFlag(BState::ITM_ENABLED))
    valid_action_types.push_back(ActionType::ITEM);

  if (getBFlag(BState::DEF_ENABLED))
    valid_action_types.push_back(ActionType::DEFEND);

  if (getBFlag(BState::GRD_ENABLED))
    valid_action_types.push_back(ActionType::GUARD);

  if (getBFlag(BState::IMP_ENABLED))
    valid_action_types.push_back(ActionType::IMPLODE);

  if (getBFlag(BState::RUN_ENABLED))
    valid_action_types.push_back(ActionType::RUN);

  if (getBFlag(BState::PAS_ENABLED))
    valid_action_types.push_back(ActionType::PASS);

  return valid_action_types;
}

/*
 * Description: Sets the action x and y offset for the rendering person in the
 *              battle action frame slide-away.
 *
 * Inputs: uint8_t action_x - the x direction offset
 *         uint8_t action_y - the y direction offset
 * Output: none
 */
void Person::setActionXY(int16_t action_x, int16_t action_y)
{
  this->action_x = action_x;
  this->action_y = action_y;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Person::setAI(AIModule* const new_ai_module)
{
  ai_module = new_ai_module;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void Person::setAilFlag(const PersonAilState &flag, const bool &set_value)
{
  (set_value) ? (ailment_flags |= flag) : (ailment_flags &= ~flag);
}

/*
 * Description: Assigns a given BState flag a given set value
 *
 * Inputs: flag - the BState flag to be assigned a value
 *         set_value - the value to assign the flag to
 * Output: none
 */
void Person::setBFlag(const BState &flag, const bool &set_value)
{
  (set_value) ? (battle_flags |= flag) : (battle_flags &= ~flag);
}

/*
 * Description: Evaluates and returns a given PState flag
 *
 * Inputs: flag - the PState flag to be assigned a value
 *         set_value - the value to assign the flag to
 * Output: none
 */
void Person::setPFlag(const PState &flag, const bool &set_value)
{
  (set_value) ? (person_flags |= flag) : (person_flags &= ~flag);
}

/*
 * Description: Assigns a curve modifiers for the person (Change's level
 *              progression)
 *
 * Inputs: prim - enumerated element for primary affiliation
 *         prim_curve - enumerated curve progress rate for primary elmt
 *         secd - enumerated element for secondary affiliation
 *         secd_curve - enumerated curve progress rate for secondary elmt
 * Output: none
 */
void Person::setCurves(Element prim, ElementCurve prim_curve,
                       Element secd, ElementCurve secd_curve,
                       const bool &update_level)
{
  primary         = prim;
  secondary       = secd;
  primary_curve   = prim_curve;
  secondary_curve = secd_curve;

  if (update_level)
  {
    updateBaseStats();
    updateStats();
  }
}

/*
 * Description: Assigns a new current attribute set
 *
 * Inputs: new_curr_set - const ref. to an AttributeSet to be assigned
 * Output: bool - true if the assignment was successful
 */
bool Person::setCurr(const AttributeSet& new_curr_set)
{
  if (new_curr_set.getFlag(AttributeState::PERSONAL))
  {
    curr_stats = new_curr_set;

    return true;
  }

  return false;
}

/*
 * Description: Assign a new current max attribute set
 *
 * Inputs: new_curr_max_set - const ref. to an AttributeSet to be assigned
 * Output: bool - true if the assignment was successfully
 */
bool Person::setCurrMax(const AttributeSet& new_curr_max_set)
{
  if (curr_max_stats.getFlag(AttributeState::PERSONAL))
  {
    curr_max_stats = new_curr_max_set;

    return true;
  }

  return false;
}

/*
 * Description: Assign a new current temp attribute set
 *
 * Inputs: new_temp_set - const ref. to an AttributeSet to be assigned
 * Output: bool - true if the assignment was successfully
 */
bool Person::setTemp(const AttributeSet& new_temp_set)
{
  if (new_temp_set.getFlag(AttributeState::PERSONAL))
  {
    temp_max_stats = new_temp_set;

    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new damage modifier value.
 *
 * Inputs: new_dmg_mod - float value that is a new dmg modifier value
 * Output: bool - true if the new value was within range
 */
bool Person::setDmgMod(const float &new_dmg_mod)
{
  if (Helpers::isInRange(new_dmg_mod, kMIN_DMG_MODI, kMAX_DMG_MODI))
  {
    dmg_mod = new_dmg_mod;

    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new experience modifier value.
 *
 * Inputs: new_exp_mod - float value that is a new exp modifier value
 * Output: bool - true if the nw value was within range
 */
bool Person::setExpMod(const float &new_exp_mod)
{
  if (Helpers::isInRange(new_exp_mod, kMIN_EXP_MODI, kMAX_EXP_MODI))
  {
    exp_mod = new_exp_mod;

    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a given equipment slot a given equipment
 *              pointer.
 *
 * Inputs: slot - enumerated EquipSlots -- the slot to be equipped
 *         new_equip - pointer to the equipment to be equipped
 * Output: bool - true if the equipment was attached successfully
 */
bool Person::setEquip(const EquipSlots &slot, Equipment* new_equip)
{
  if (new_equip == nullptr || slot != new_equip->getEquipSlot())
    return false;

  if (new_equip->getEquipFlag(EquipState::EQUIPPED))
    return false;

  if (new_equip->getEquipFlag(EquipState::TWO_HANDED))
  {
    if (getEquip(EquipSlots::LARM) == nullptr &&
        getEquip(EquipSlots::RARM) == nullptr)
    {
      equipments[getEquipIndex(EquipSlots::LARM)] = new_equip;
      equipments[getEquipIndex(EquipSlots::RARM)] = new_equip;
    }
  }
  else
  {
    equipments[getEquipIndex(slot)] = new_equip;
  }

  return true;
}

/*
 * Description: Sets the game ID for the person.
 *
 * Inputs: int id - the game reference id
 * Output: none
 */
void Person::setGameID(int id)
{
  if(id < 0)
    game_id = kUNSET_ID;
  else
    game_id = id;
}

/*
 * Description: Assigns a new guard (person guarding this object)
 *
 * Inputs: Person* - the new guard for this person
 * Output: bool - true if the guard was assigned
 */
bool Person::setGuard(Person* const new_guard)
{
  /* If this person is guarding someone, the guard cannot be made, or if this
   * is trying to be as assigned to a guard to self */
  if (guardee == nullptr && new_guard != nullptr && this != new_guard)
  {
    setBFlag(BState::GUARDED, true);
    setBFlag(BState::GUARDING, false);
    guard = new_guard;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a new person to be guarded
 *
 * Inputs: Person* - the new person being guarded by this person
 * Output: bool - true if the guarding person was assigned
 */
bool Person::setGuardee(Person* const new_guardee)
{
  /* If this has a guard, they cannot guard someone else. They cannot be guarded
   * by themselves as well */
  if (guard == nullptr && new_guardee != nullptr && this != new_guardee)
  {
    setBFlag(BState::GUARDING, true);
    setBFlag(BState::GUARDED, false);
    guardee = new_guardee;

    return true;
  }

  return false;
}

/*
 * Description: Attempts to assign a new loot set for the Person
 *
 * Inputs: new_credit_drop - new value to be assigned as credit drop loot
 *         new_exp_drop - new value to be assigned for experience drop loot
 *         new_item_drops - new item drop IDs for drop loot
 * Output: bool - true if the loot assignment was successful
 */
bool Person::setLoot(const uint32_t &new_credit_drop,
                     const uint32_t &new_exp_drop,
                     const std::vector<uint32_t> &new_item_drops)
{
  auto all_set = true;

  if (new_credit_drop < kMAX_CREDIT_DROP)
    credit_drop = new_credit_drop;
  else
  {
    credit_drop = kMAX_CREDIT_DROP;
    all_set = false;
  }

  if (new_exp_drop < kMAX_EXP_DROP)
    credit_drop = new_exp_drop;
  else
  {
    exp_drop = kMAX_EXP_DROP;
    all_set = false;
  }

  if (new_item_drops.size() < kMAX_ITEM_DROPS)
    item_drops = new_item_drops;
  else
  {
    item_drops.clear();
    all_set = false;
  }

  return all_set;
}

/*
 * Description: Sets the name of the person.
 *
 * Inputs: std::string name - the new name of the person
 * Output: none
 */
void Person::setName(std::string name)
{
  this->name = name;
}

/*
 * Description: Assigns new sprite pointers for the Person
 *
 * Inputs: new_fp - pointer to a first person sprite
 *         new_tp - pointer to a third person sprite
 *         new_action - pointer to an action frame sprite
 * Output: none
 */
void Person::setSprites(Sprite* new_fp, Sprite* new_tp, Sprite* new_dialog,
    Sprite* new_action)
{
  /* Unset the current sprites */
  unsetSprites();

  /* Assign the new sprite pointers */
  action_sprite = new_action;
  first_person  = new_fp;
  third_person  = new_tp;
  dialog_sprite = new_dialog;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Grabs the experience required to reach a given level by
 *              accessing the experience table.
 *
 * Inputs: level - the level to be checked for
 * Output: uint32_t - the minimum experience required to reach that level
 */
uint32_t Person::getExpAt(const uint8_t &level)
{
  if (level != 0 && level <= kNUM_LEVELS)
    return exp_table.at(level - 1);

  return kMAX_EXP + 1;
}

/*
 * Description: Grabs the level corresponding to a given experience value
 *
 * Inputs: experience - the experience to find a level for
 * Output: uint8_t - the highest level reachable at the given experience
 */
uint16_t Person::getLevelAt(const uint32_t &experience)
{
  for (size_t i = 0; i < exp_table.size(); i++)
    if (exp_table.at(i) > experience)
      return i;

  return exp_table.size();
}

/*
 * Description: Grabs the number of total levels for Person progression
 *
 * Inputs: non
 * Output: size_t - kNUM_LEVELS - the highest level reachable
 */
size_t Person::getNumLevels()
{
  return kNUM_LEVELS;
}

/*
 * Description: Grabs the number of equipment slots a Person has
 *
 * Inputs: none
 * Output: size_t - kNUM_EQUIP_SLOTS - the number of equipment slots for Persons
 */
size_t Person::getNumEquipSlots()
{
  return kNUM_EQUIP_SLOTS;
}

/*
 * Description: Returns the minimum experience possible.
 *
 * Inputs: none
 * Output: uint32_t - kMIN_EXP - the lowest possible experience
 */
/* Returns the minimum experience possible */
uint32_t Person::getMinExp()
{
  return kMIN_EXP;
}

/*
 * Description: Returns the maximum experience possible
 *
 * Inputs: none
 * Output: uint32_t - kMAX_EXP - the maximum experience possible
 */
uint32_t Person::getMaxExp()
{
  return kMAX_EXP;
}

/*
 * Description: Returns the minimum level experience
 *
 * Inputs: none
 * Output: uint32_t - kMIN_LVL_EXP - experience required at the lowest level
 */
uint32_t Person::getMinLvlExp()
{
  return kMIN_LVL_EXP;
}

/*
 * Description: Returns the experience required at the final level
 *
 * Inputs: none
 * Output: uint32_t - kMAX_LVL_EXP - the experience req. at the highets level
 */
uint32_t Person::getMaxLvlExp()
{
  return kMAX_LVL_EXP;
}

/*
 * Description: Returns the maximum possible credit drop
 *
 * Inputs: none
 * Output: uint32_t - kMAX_CREDIT_DROP - the maximum possible credit drop
 */
uint32_t Person::getMaxCreditDrop()
{
  return kMAX_CREDIT_DROP;
}

/*
 * Description: Returns the maximum experience awarded by a single Person
 *
 * Inputs: none
 * Output: uint32_t - kMAX_EXP_DROP - the maximum experience awarded by a Person
 */
uint32_t Person::getMaxExpDrop()
{
  return kMAX_EXP_DROP;
}

/*
 * Description: Returns the maximum number of item drops a Person can reward
 *
 * Inputs: none
 * Output: uint32_t - kMAX_ITEM_DROPS - the maximum number of item drops
 */
uint32_t Person::getMaxItemDrops()
{
  return kMAX_ITEM_DROPS;
}

/*
 * Description: Returns the minimum damage modifier possible
 *
 * Inputs: none
 * Output: float - kMIN_DMG_MODI - the minimum damage modifier possible
 */
float Person::getMinDmgModi()
{
  return kMIN_DMG_MODI;
}

/*
 * Description: Returns the maximum damage modifier possible
 *
 * Inputs: none
 * Output: float - kMAX_DMG_MODI - the maximum damage modifier possible
 */
/* Returns the maximum damage modifier possible */
float Person::getMaxDmgModi()
{
  return kMAX_DMG_MODI;
}

/*
 * Description: Returns the minimum damage modifier possible
 *
 * Inputs: none
 * Output: float - kMIN_EXP_MODI - the minimum damage modifier possible
 */
float Person::getMinExpModi()
{
  return kMIN_EXP_MODI;
}

/*
 * Description: Returns the max experience modifier
 *
 * Inputs: none
 * Output: float - kMAX_EXP_MODI - the maximum damage modifier possible
 */
float Person::getMaxExpModi()
{
  return kMAX_EXP_MODI;
}
