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
const uint8_t Person::kACTION_X = 60;
const uint8_t Person::kACTION_Y = 128;
const uint32_t Person::kID_PLAYER{0};
const size_t Person::kNUM_LEVELS{15};
const size_t Person::kNUM_EQUIP_SLOTS{5};
const uint32_t Person::kMAX_CREDIT_DROP{1000000}; /* 1 million */
const uint32_t Person::kMAX_EXP{1000000000};      /* 1 billion */
const uint32_t Person::kMAX_EXP_DROP{1000000};    /* 1 million */
const size_t Person::kMAX_ITEM_DROPS{25};
const uint32_t Person::kMAX_LVL_EXP{5000}; /* 100 million */
const uint32_t Person::kMIN_EXP{30};
const uint32_t Person::kMIN_LVL_EXP{50};
const uint32_t Person::kMIN_LVL_IMPLODE{7}; /* Min lvl for enemies to 'plode */
const float Person::kMIN_DMG_MODI{0.01};
const float Person::kMAX_DMG_MODI{10.00};
const float Person::kMIN_EXP_MODI{0.10};
const float Person::kMAX_EXP_MODI{10.00};
const int32_t Person::kUNSET_ID = -1;

/* Static ID counter */
int Person::id{0};

std::vector<uint32_t> Person::exp_table{};

/* XS, S, A, B, C, D */
const std::vector<float> Person::kPRIM_MODS = {1.425, 1.400, 1.375,
                                               1.350, 1.325, 1.300};

const std::vector<float> Person::kSECD_MODS = {1.375, 1.300, 1.250,
                                               1.225, 1.200, 1.175};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs an empty person object.
 *
 * Inputs: none
 */
Person::Person()
    : my_id{++id},
      base_person{nullptr},
      battle_class{nullptr},
      race_class{nullptr}
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
Person::Person(const int32_t& game_id, const std::string& name,
               Category* const battle_class, Category* const race_class)
    : game_id{game_id},
      my_id{++id},
      base_person{nullptr},
      battle_class{battle_class},
      race_class{race_class},
      name{name}
{
  setupClass();
}

/*
 * Description: Creates a copy of a base person from a pointer to a base person
 *
 * Inputs: Person* - pointer to the base person
 */
Person::Person(Person* const source)
    : game_id{source->game_id}, my_id{++id}, base_person{source}
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
 * Description: Copies all data from source object to this object.
 *
 * Inputs: Person &source - the source to copy from
 * Output: none
 */
// TODO: NOT FULLY COMPLETE. Relevant??
void Person::copySelf(const Person& source)
{
  action_x = source.action_x;
  action_y = source.action_y;

  game_id = source.game_id;

  base_person = source.base_person;
  person_flags = source.person_flags;

  battle_class = source.battle_class;
  race_class = source.race_class;

  name = source.name;
  rank = source.rank;

  primary = source.primary;
  secondary = source.secondary;
  primary_curve = source.primary_curve;
  secondary_curve = source.secondary_curve;

  equipments = source.equipments;

  item_drops = source.item_drops;
  credit_drop = source.credit_drop;
  exp_drop = source.exp_drop;

  level = source.level;
  total_exp = source.total_exp;

  sprite_ally = source.sprite_ally;
  sprite_ally_defensive = source.sprite_ally_defensive;
  sprite_ally_offensive = source.sprite_ally_offensive;
  sprite_dialog = source.sprite_dialog;
  sprite_face = source.sprite_face;
  sprite_foe = source.sprite_foe;
  sprite_foe_defensive = source.sprite_foe_defensive;
  sprite_foe_offensive = source.sprite_foe_offensive;
}

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
  person_flags = static_cast<PState>(0);

  setPFlag(PState::CAN_GAIN_EXP, true);
  setPFlag(PState::CAN_LEVEL_UP, true);
  setPFlag(PState::CAN_LEARN_SKILLS, true);
  setPFlag(PState::CAN_CHANGE_EQUIP, true);

  // person_record{};

  updateRank();

  primary = Element::NONE;
  secondary = Element::NONE;
  primary_curve = ElementCurve::D;
  secondary_curve = ElementCurve::D;

  total_exp = kMIN_LVL_EXP;

  updateLevel();

  updateBaseStats();

  curr_stats = base_stats;
  curr_max_stats = base_stats;
  temp_max_stats = base_stats;

  if(base_skills != nullptr)
    std::cerr << "[Warning]: Missing deletion of base skills\n";
  if(curr_skills != nullptr)
    std::cerr << "[Warning]: Missing deletion of curr skills\n";
  if(learned_skills != nullptr)
    std::cerr << "[Warning]: Missing deletion of temp skills\n";

  base_skills = nullptr;
  curr_skills = nullptr;
  learned_skills = nullptr;

  updateBaseSkills();

  dmg_mod = 1.000;
  exp_mod = 1.000;

  for(size_t a = 0; a < kNUM_EQUIP_SLOTS; a++)
    equipments.push_back(nullptr);

  credit_drop = 0;
  exp_drop = 0;

  sprite_ally = nullptr;
  sprite_ally_defensive = nullptr;
  sprite_ally_offensive = nullptr;
  sprite_dialog = nullptr;
  sprite_face = nullptr;
  sprite_foe = nullptr;
  sprite_foe_defensive = nullptr;
  sprite_foe_offensive = nullptr;
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
  ai_module = nullptr;
  base_skills = nullptr;
  curr_skills = nullptr;
  learned_skills = nullptr;

  if(exp_table.empty())
    buildExpTable();

  /* Setup the class for a Base Person */
  if(base_person == nullptr)
  {
    loadDefaults();
  }

  /* Setup the class as a copy of the Base Person */
  else
  {
    action_x = base_person->action_x;
    action_y = base_person->action_y;
    person_flags = base_person->person_flags;

    // TODO: Dynamic initialization of record? [10-25-14]
    // person_record = base-person->person_record;

    battle_class = base_person->battle_class;
    race_class = base_person->race_class;

    assert(battle_class && race_class);

    name = base_person->name;
    rank = base_person->rank;
    primary = base_person->primary;
    secondary = base_person->secondary;
    primary_curve = base_person->primary_curve;
    secondary_curve = base_person->secondary_curve;
    base_skills = base_person->base_skills;
    curr_skills = nullptr;
    learned_skills = nullptr;
    dmg_mod = base_person->dmg_mod;
    exp_mod = base_person->exp_mod;

    /* Equipments are deep-copied */
    for(auto equipment : base_person->equipments)
    {
      if(equipment != nullptr)
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
    sprite_ally = base_person->sprite_ally;
    sprite_ally_defensive = base_person->sprite_ally_defensive;
    sprite_ally_offensive = base_person->sprite_ally_offensive;
    sprite_dialog = base_person->sprite_dialog;
    sprite_face = base_person->sprite_face;
    sprite_foe = base_person->sprite_foe;
    sprite_foe_defensive = base_person->sprite_foe_defensive;
    sprite_foe_offensive = base_person->sprite_foe_offensive;

    updateBaseStats();
    curr_stats = base_stats;
    curr_max_stats = base_stats;
    temp_max_stats = base_stats;

    updateBaseSkills();
  }
}

/*
 * Description: Unsets all the parameters of a person.
 *
 * Inputs: bool - clears the memory of anything dynamically allocated by Person
 * Output: none
 */
void Person::unsetAll(const bool& clear)
{
  if(clear)
  {
    /* Delete the equipments contained within the person */
    for(auto equipment_index : equipments)
    {
      if(equipment_index != nullptr)
      {
        if(!(equipment_index->getEquipFlag(EquipState::TWO_HANDED) &&
             equipment_index == getEquip(EquipSlots::RARM)))
          delete equipment_index;
      }

      equipment_index = nullptr;
    }

    /* Delete the skills sets */
    if(base_person == nullptr && base_skills != nullptr)
      delete base_skills;
    if(curr_skills != nullptr)
      delete curr_skills;
    if(learned_skills != nullptr)
      delete learned_skills;
  }

  if(base_person == nullptr)
    unsetSprites();

  ai_module = nullptr;
  base_skills = nullptr;
  curr_skills = nullptr;
  learned_skills = nullptr;
}

/*
 * Description: Unsets the sprites stored within the person.
 *
 * Inputs: none
 * Output: none
 */
void Person::unsetSprites()
{
  if(sprite_ally &&
     (base_person == nullptr || base_person->sprite_ally != sprite_ally))
  {
    delete sprite_ally;
  }

  if(sprite_ally_defensive &&
     (base_person == nullptr ||
      base_person->sprite_ally_defensive != sprite_ally_defensive))
  {
    delete sprite_ally_defensive;
  }

  if(sprite_ally_offensive &&
     (base_person == nullptr ||
      base_person->sprite_ally_offensive != sprite_ally_offensive))
  {
    delete sprite_ally_offensive;
  }

  if(sprite_dialog &&
     (base_person == nullptr || base_person->sprite_dialog != sprite_dialog))
  {
    delete sprite_dialog;
  }

  if(sprite_face &&
     (base_person == nullptr || base_person->sprite_face != sprite_face))
  {
    delete sprite_face;
  }

  if(sprite_foe &&
     (base_person == nullptr || base_person->sprite_foe != sprite_foe))
  {
    delete sprite_foe;
  }

  if(sprite_foe_defensive &&
     (base_person == nullptr ||
      base_person->sprite_foe_defensive != sprite_foe_defensive))
  {
    delete sprite_ally;
  }

  if(sprite_foe_offensive &&
     (base_person == nullptr ||
      base_person->sprite_foe_offensive != sprite_foe_offensive))
  {
    delete sprite_foe_offensive;
  }

  sprite_ally = nullptr;
  sprite_ally_defensive = nullptr;
  sprite_ally_offensive = nullptr;
  sprite_face = nullptr;
  sprite_foe = nullptr;
  sprite_foe_defensive = nullptr;
  sprite_foe_offensive = nullptr;
}

/*
 * Description: Updates he level of the Person based on their curren total
 *              experience.
 *
 * Inputs: bool ignore_flags - true to ignore CAN_LEVEL_UP flag. default false
 * Output: none
 */
void Person::updateLevel(const bool& ignore_flags)
{
  if(getPFlag(PState::CAN_LEVEL_UP) || ignore_flags)
  {
    auto before = level;

    level = getLevelAt(total_exp);

    if(level == kNUM_LEVELS)
      setPFlag(PState::MAX_LVL);

    if(level != before)
      updateStats();
  }
}

/*
 * Description: Updates the rank of the Person based on their person record.
 *
 * Inputs: none
 * Output: nones
 */
void Person::updateRank()
{
  rank = Rank::NUBEAR;
  // rank = person_record.getRank();
}

/*=============================================================================
 * PRIVATE STATIC FUNCTIONS
 *============================================================================*/

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
float Person::getCurveModifier(const ElementCurve& curve, const bool primary)
{
  auto index = static_cast<uint8_t>(curve);

  if(primary && index < kPRIM_MODS.size())
    return kPRIM_MODS.at(index);

  if(index < kSECD_MODS.size())
    return kSECD_MODS.at(index);

  return 0;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Adds an unsigned amount of experience to the Person and a
 *levels
 *              them up (if necessary) if the update bool is assigned.
 *
 * Inputs: amount - the amount of experience to be added
 *         update - a boolean value whether to be updated
 *         ignore_flags - true to ignore CAN_GAIN_EXP flag. default false
 * Output: bool - true if the experience was added successfully
 */
bool Person::addExp(const uint32_t& amount, const bool& update,
                    const bool& ignore_flags)
{
  auto can_add = false;

  if(getPFlag(PState::CAN_GAIN_EXP) || ignore_flags)
  {
    auto mod_amount = std::floor(amount * exp_mod);

    if(total_exp + mod_amount < kMAX_EXP)
    {
      total_exp += mod_amount;
      can_add = true;
    }
    else if(total_exp != kMAX_EXP)
    {
      total_exp = kMAX_EXP;
      can_add = true;
    }

    for(auto equipment : equipments)
      if(equipment != nullptr)
        equipment->getSignature()->addExp(amount);
  }

  if(can_add && update)
    updateLevel(ignore_flags);

  return can_add;
}

/*
 * Description: Updates the temp_max_stats attribute set to reflect the
 *              curr_max_stat attribute plus the buff sets for each of the
 *              equipped equipment.
 *
 * Inputs: none
 * Output: none
 */
AttributeSet Person::calcEquipStats()
{
  // TODO: Fix this function
  // auto equip_bonus = AttributeSet();

  // for(auto it = begin(equipments); it != end(equipments); ++it)
  //   if((*it) != nullptr)
  //     equip_bonus += (*it)->getStats();

  // return (curr_max_stats + equip_bonus);

  return curr_max_stats;
}

/*
 * Description: Calculates the vitality percent at the given value v. max.
 *
 * Inputs: uint32_t target_value - the target VITA value
 * Output: float - the percent vitality of max
 */
float Person::calcVitaPercentAtVal(uint32_t target_value)
{
  auto target_vita = static_cast<int32_t>(target_value);
  auto max_vita = getTemp().getStat(Attribute::VITA);

  return (max_vita != 0) ? ((float)target_vita / (float)max_vita) : (0);
}

/*
 * Description: Calculates the QTDR percent at the given value v. max
 *
 * Inputs: uint32_t target_value - the target QTDR value
 * Output: float - the percent QTDR of max
 */
float Person::calcQtdrPercentAtVal(uint32_t target_value)
{
  auto target_qtdr = static_cast<int32_t>(target_value);
  auto max_qtdr = getTemp().getStat(Attribute::QTDR);

  return (max_qtdr != 0) ? ((float)target_qtdr / (float)max_qtdr) : (0);
}

/*
 * Description: Clears the learned skills of the Person
 *
 * Inputs: none
 * Output: none
 */
void Person::clearLearnedSkills()
{
  if(learned_skills != nullptr)
  {
    learned_skills->clear();
    updateSkills();
  }
}

/*
 * Description: Shorthand function for dealing damage to the Person. Returns
 *              true if the Person's HP is 0 after the damage takes place.
 *
 * Inputs: amount - amount of damage to deal
 * Output: bool - true if the Person is now dead
 */
bool Person::doDmg(const uint32_t& amount)
{
  curr_stats.alterStat(Attribute::VITA, -amount);

  return (curr_stats.getStat(Attribute::VITA) <= 0);
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

  if(level < kNUM_LEVELS)
  {
    auto curr_exp = findExpThisLevel();
    auto need_exp = Person::getExpAt(level + 1) - Person::getExpAt(level);

    pc_exp = static_cast<uint16_t>(floor((curr_exp * 100) / need_exp));
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
  if(level < kNUM_LEVELS)
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

  if(level < kNUM_LEVELS)
  {
    auto level_below_exp = Person::getExpAt(level);
    curr_exp = total_exp - level_below_exp;
  }

  return curr_exp;
}

/*
 * Description: Determines whether the person is a power defender (whether
 *they
 *              can persist in defending against multiple attacks)
 *
 * Inputs: none
 * Output: bool - true if the person is a power defender
 */
bool Person::isPowerDefender()
{
  /* If either battle class or race class allows the person to be a power
   * defender, the person will be said to be a power defender */
  if(battle_class->getFlag(CategoryState::POWER_DEFENDER) ||
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
  if(battle_class->getFlag(CategoryState::POWER_GUARDER) ||
     race_class->getFlag(CategoryState::POWER_GUARDER))
  {
    return true;
  }

  return false;
}

/*
 * Description: Loads the data from file associated with the person.
 *
 * Inputs: XmlData data - the xml data structure
 *         int index - the element reference index
 *         SDL_Renderer* renderer - the rendering engine
 *         std::string base_path - the base path for file handling
 * Output: bool - true if load was successful
 */
bool Person::loadData(XmlData data, int index, SDL_Renderer* renderer,
                      std::string base_path)
{
  bool success = true;

  /* ---- DAMAGE MODIFIER ---- */
  if(data.getElement(index) == "dmgmod")
  {
    float val = data.getDataFloat(&success);
    if(success)
      success &= setDmgMod(val);
  }
  /* ---- ELEMENT PRIMARY ---- */
  else if(data.getElement(index) == "elem_pri")
  {
    std::string elem_str = data.getDataString(&success);
    if(success)
    {
      std::vector<std::string> elem_set = Helpers::split(elem_str, ',');
      if(elem_set.size() == 2)
      {
        Element ele = Helpers::elementFromString(elem_set.front());
        ElementCurve curve = Helpers::curveFromString(elem_set.back());

        setCurves(ele, curve, secondary, secondary_curve);
      }
    }
  }
  /* ---- ELEMENT SECONDARY ---- */
  else if(data.getElement(index) == "elem_sec")
  {
    std::string elem_str = data.getDataString(&success);
    if(success)
    {
      std::vector<std::string> elem_set = Helpers::split(elem_str, ',');
      if(elem_set.size() == 2)
      {
        Element ele = Helpers::elementFromString(elem_set.front());
        ElementCurve curve = Helpers::curveFromString(elem_set.back());

        setCurves(primary, primary_curve, ele, curve);
      }
    }
  }
  /* ---- EXPERIENCE ---- */
  else if(data.getElement(index) == "exp")
  {
    int exp = data.getDataInteger(&success);
    if(success)
    {
      int diff_exp = (exp - getTotalExp());
      if(diff_exp > 0)
      {
        success &= addExp(diff_exp, true, true);
      }
      else if(diff_exp < 0)
      {
        success &= loseExp(-diff_exp, true, true);
      }
    }
  }
  /* ---- EXPERIENCE MODIFIER ---- */
  else if(data.getElement(index) == "expmod")
  {
    float val = data.getDataFloat(&success);
    if(success)
      success &= setExpMod(val);
  }
  /* ---- FLAGS ---- */
  else if(data.getElement(index) == "flags")
  {
    bool state = data.getDataBool(&success);

    if(success)
    {
      if(data.getElement(index + 1) == "can_gain_exp")
        setPFlag(PState::CAN_GAIN_EXP, state);
      else if(data.getElement(index + 1) == "can_level_up")
        setPFlag(PState::CAN_LEVEL_UP, state);
      else if(data.getElement(index + 1) == "can_learn_skills")
        setPFlag(PState::CAN_LEARN_SKILLS, state);
      else if(data.getElement(index + 1) == "can_change_equip")
        setPFlag(PState::CAN_CHANGE_EQUIP, state);
    }
  }
  /* ---- LOOT ---- */
  else if(data.getElement(index) == "loot")
  {
    std::vector<uint32_t> items = item_drops;
    uint32_t credits = credit_drop;
    uint32_t exps = exp_drop;

    /* -- CREDIT -- */
    if(data.getElement(index + 1) == "credit")
      credits = data.getDataInteger(&success);
    /* -- EXPERIENCE -- */
    else if(data.getElement(index + 1) == "exp")
      exps = data.getDataInteger(&success);
    /* -- ITEMS -- */
    else if(data.getElement(index + 1) == "item")
      items.push_back(data.getDataInteger(&success));

    /* Set Loot */
    success &= setLoot(credits, exps, items);
  }
  /* ---- NAME ---- */
  else if(data.getElement(index) == "name")
  {
    setName(data.getDataString(&success));
  }
  /* ---- QUANTUM DRIVE CURRENT VALUE ---- */
  else if(data.getElement(index) == "qtdr")
  {
    int val = data.getDataInteger(&success);
    if(success && val >= 0)
    {
      restoreQtdr();
      if(val < curr_max_stats.getStat(Attribute::QTDR))
        curr_stats.setStat(Attribute::QTDR, val);
    }
  }
  else if(data.getElement(index) == "sprite_face")
  {
    if(sprite_face == nullptr)
      sprite_face = new Sprite();

    success &= sprite_face->addFileInformation(data, index + 1, renderer,
                                               base_path, false, false);
  }
  /* ---- SPRITE ALLY ---- */
  else if(data.getElement(index) == "sprite_ally")
  {
    if(sprite_ally == nullptr)
      sprite_ally = new Sprite();

    success &= sprite_ally->addFileInformation(data, index + 1, renderer,
                                               base_path, false, false);
  }
  /* ---- SPRITE ALLY OFFENSIVE---- */
  else if(data.getElement(index) == "sprite_ally_off")
  {
    if(sprite_ally_offensive == nullptr)
      sprite_ally_offensive = new Sprite();

    success &= sprite_ally_offensive->addFileInformation(
        data, index + 1, renderer, base_path, false, false);
  }
  /* ---- SPRITE ALLY DEFENSIVE ---- */
  else if(data.getElement(index) == "sprite_ally_def")
  {
    if(sprite_ally_defensive == nullptr)
      sprite_ally_defensive = new Sprite();

    success &= sprite_ally_defensive->addFileInformation(
        data, index + 1, renderer, base_path, false, false);
  }
   /* ---- SPRITE FOE ---- */
  else if(data.getElement(index) == "sprite_foe")
  {
    if(sprite_foe == nullptr)
      sprite_foe = new Sprite();

    success &= sprite_foe->addFileInformation(data, index + 1, renderer,
                                               base_path, false, false);
  }
  /* ---- SPRITE FOE OFFENSIVE---- */
  else if(data.getElement(index) == "sprite_foe_off")
  {
    if(sprite_foe_offensive == nullptr)
      sprite_foe_offensive = new Sprite();

    success &= sprite_foe_offensive->addFileInformation(
        data, index + 1, renderer, base_path, false, false);
  }
  /* ---- SPRITE FOE DEFENSIVE ---- */
  else if(data.getElement(index) == "sprite_foe_def")
  {
    if(sprite_foe_defensive == nullptr)
      sprite_foe_defensive = new Sprite();

    success &= sprite_foe_defensive->addFileInformation(
        data, index + 1, renderer, base_path, false, false);
  }
  /* ---- SPRITE ACTION X ---- */
  else if(data.getElement(index) == "sprite_action_x")
  {
    setActionXY(data.getDataInteger(&success), action_y);
  }
  /* ---- SPRITE ACTION Y ---- */
  else if(data.getElement(index) == "sprite_action_y")
  {
    setActionXY(action_x, data.getDataInteger(&success));
  }
  /* ---- SPRITE DIALOG ---- */
  else if(data.getElement(index) == "sprite_dialog")
  {
    if(sprite_dialog == nullptr)
      sprite_dialog = new Sprite();

    success &= sprite_dialog->addFileInformation(data, index + 1, renderer,
                                                 base_path, false, false);
  }
  /* ---- VITALITY CURRENT VALUE ---- */
  else if(data.getElement(index) == "vita")
  {
    int val = data.getDataInteger(&success);
    if(success && val >= 0)
    {
      restoreHealth();
      if(val < curr_max_stats.getStat(Attribute::VITA))
        curr_stats.setStat(Attribute::VITA, val);
    }
  }

  return success;
}

/*
 * Description: Removes an amount of experience from the person but not from
 *              their equipment.
 *
 * Inputs: uint32_t amount - the amount to take away
 *         bool update - whether to update the level
 *         bool ignore_flags - should flags be ignored on changes
 * Output: bool - whether the loss of experience was possible
 */
bool Person::loseExp(const uint32_t& amount, const bool& update,
                     const bool& ignore_flags)
{
  if(amount >= total_exp)
    total_exp -= amount;

  if(update)
    updateLevel(ignore_flags);

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
bool Person::loseExpPercent(const uint16_t& percent)
{
  auto lost_exp = false;
  auto exp_pc = findExpPercent();

  /* If the % to lose is greater than % gained, remove all exp this level */
  if(percent > exp_pc)
  {
    auto exp_gained = findExpThisLevel();
    total_exp -= exp_gained;
    lost_exp = true;
  }
  else if(percent > 0)
  {
    total_exp -= findExpPerPC() * percent;
    lost_exp = true;
  }

  return lost_exp;
}

/*
 * Description: Attempts to remove equipment from a given equip slot.
 *              Returns true if an equipment at that slot was found and it was
 *              removed.
 *
 * Note [1]: This function will cause a memory leak unless the equipment is
 *           grabbed before the removal takes place!
 *
 * Inputs: equip_slot - enumerated equip slot to remove the equipment for
 * Output: bool - true if a piece of equipment was there and removed
 */
bool Person::removeEquip(const EquipSlots& equip_slot)
{
  auto removed = false;

  if(getEquip(equip_slot) == nullptr)
    return false;

  auto equip = getEquip(equip_slot);

  switch(equip_slot)
  {
  case(EquipSlots::LARM):
  case(EquipSlots::RARM):
    if(equip->getEquipFlag(EquipState::TWO_HANDED))
    {
      equipments.at(1) = nullptr;
      equipments.at(2) = nullptr;
    }
    else if(equip_slot == EquipSlots::LARM)
      equipments.at(1) = nullptr;
    else if(equip_slot == EquipSlots::RARM)
      equipments.at(2) = nullptr;

    removed = true;
    break;
  case(EquipSlots::HEAD):
    equipments.at(0) = nullptr;
  case(EquipSlots::BODY):
    equipments.at(3) = nullptr;
  case(EquipSlots::LEGS):
    equipments.at(4) = nullptr;
  default:
    std::cerr << "Warning: Attempting removal of invalid equip slot.\n";
  }

  return removed;
}

/*
 * Description: Restores the person health (VITA) to max (full).
 *
 * Inputs: none
 * Output: none
 */
void Person::restoreHealth()
{
  curr_stats.setStat(Attribute::VITA, curr_max_stats.getStat(Attribute::VITA));
}

/*
 * Description: Restores the person QTDR to max (full).
 *
 * Inputs: none
 * Output: none
 */
void Person::restoreQtdr()
{
  curr_stats.setStat(Attribute::QTDR, curr_max_stats.getStat(Attribute::QTDR));
}

/*
 * Description: Saves the data of this person to the file handler pointer.
 *This
 *              assumes the person wrapper xml is written before the call.
 *
 * Inputs: FileHandler* fh - the saving file handler
 * Output: bool - true if successful
 */
bool Person::saveData(FileHandler* fh)
{
  if(fh != nullptr)
  {
    Person default_person;

    /* Base reference and corresponding level */
    if(base_person != nullptr)
      fh->writeXmlData("base", base_person->getGameID());

    /* Experience and Level */
    fh->writeXmlData("exp", total_exp);
    fh->writeXmlData("level", level);

    /* Damage and experience mods */
    if(getDmgMod() != default_person.getDmgMod())
      fh->writeXmlData("dmgmod", getDmgMod());
    if(getExpMod() != default_person.getExpMod())
      fh->writeXmlData("expmod", getExpMod());

    /* Learned skills */
    if(learned_skills != nullptr)
      learned_skills->saveData(fh, "learned");

    /* Current vitality */
    if(curr_stats.getStat(Attribute::VITA) !=
       curr_max_stats.getStat(Attribute::VITA))
    {
      fh->writeXmlData("vita", curr_stats.getStat(Attribute::VITA));
    }

    /* Current quantum drive */
    if(curr_stats.getStat(Attribute::QTDR) !=
       curr_max_stats.getStat(Attribute::QTDR))
    {
      fh->writeXmlData("qtdr", curr_stats.getStat(Attribute::QTDR));
    }

    return true;
  }
  return false;
}

/*
 * Description: Updates the base skills of the Person based on the battle
 *class
 *              and the race class.
 *
 * Inputs: none
 * Output: none
 */
void Person::updateBaseSkills()
{
  if(base_skills != nullptr)
    base_skills->clear();
  else
    base_skills = new SkillSet();

  if(battle_class != nullptr && battle_class->getSkills() != nullptr)
    *base_skills = *base_skills + *(battle_class->getSkills());

  if(race_class != nullptr && race_class->getSkills() != nullptr)
    *base_skills = *base_skills + *(race_class->getSkills());

  updateSkills();
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
  AttributeSet temp;
  AttributeSet temp_max;

  if(battle_class != nullptr)
  {
    temp = battle_class->getBaseSet();
    temp_max = battle_class->getTopSet();
  }
  if(race_class != nullptr)
  {
    temp += race_class->getBaseSet();
    temp_max += race_class->getTopSet();
  }

  std::vector<int32_t> prim_indexes;
  std::vector<int32_t> secd_indexes;

  if(primary != Element::NONE)
  {
    prim_indexes.push_back(AttributeSet::getOffensiveIndex(primary));
    prim_indexes.push_back(AttributeSet::getDefensiveIndex(primary));

    auto p_mod = getCurveModifier(primary_curve, true);

    for(auto index : prim_indexes)
    {
      if(index != -1)
      {
        auto a = std::floor(static_cast<float>(temp.getStat(index)) * p_mod);
        auto b =
            std::floor(static_cast<float>(temp_max.getStat(index)) * p_mod);

        temp.setStat(index, a);
        temp_max.setStat(index, b);
      }
    }
  }

  if(secondary != Element::NONE)
  {
    secd_indexes.push_back(AttributeSet::getOffensiveIndex(secondary));
    secd_indexes.push_back(AttributeSet::getDefensiveIndex(secondary));

    auto s_mod = getCurveModifier(secondary_curve, false);

    for(auto index : secd_indexes)
    {
      if(index != -1)
      {
        auto a = std::floor(static_cast<float>(temp.getStat(index)) * s_mod);
        auto b =
            std::floor(static_cast<float>(temp_max.getStat(index)) * s_mod);

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
 * Description: Updates the current total SkillSet of the person. This means
 *              adding the skills granted by equipment and the learned skills
 *              of the Person to the base skills.
 *
 * Inputs: none
 * Output: none
 */
void Person::updateSkills()
{

  if(curr_skills != nullptr)
    curr_skills->clear();
  else
    curr_skills = new SkillSet();

  if(learned_skills != nullptr)
    *curr_skills += *learned_skills;

  if(base_skills != nullptr)
    *curr_skills += *base_skills;

  for(auto equipment : equipments)
    if(equipment != nullptr)
      *curr_skills += equipment->getSkills();
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
  if(level == 1)
  {
    curr_stats = base_stats;
    curr_max_stats = base_stats;
  }
  else if(level == kNUM_LEVELS)
  {
    curr_stats = base_max_stats;
    curr_max_stats = base_max_stats;
  }
  else if(level > 1)
  {
    curr_stats = AttributeSet(0, true);
    curr_max_stats = AttributeSet(0, true);

    for(size_t i = 0; i < AttributeSet::getSize(); i++)
    {
      std::vector<uint32_t> stat_values{};
      stat_values = Helpers::buildExpTable(
          base_stats.getStat(i), base_max_stats.getStat(i), kNUM_LEVELS);
      curr_stats.setStat(i, stat_values.at(level - 1));
    }

    curr_max_stats = curr_stats;
  }

  temp_max_stats = curr_max_stats;

  for(auto equipment : equipments)
    if(equipment != nullptr)
      temp_max_stats += equipment->getStats();

  curr_stats.cleanUp();
  curr_max_stats.cleanUp();
  temp_max_stats.cleanUp();
}

/*
 * Description: Returns the rendering action X offset in the swipe away in
 *the
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
 * Description: Returns the rendering action Y offset in the swipe away in
 *the
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
 * Description: Returns the learned skill set. If create flagged, it will
 *              generate the learned skills skillset if it is null
 *
 * Inputs: const bool& create - true to create it if its null
 * Output: SkillSet* - the learned set stored within the class
 */
SkillSet* Person::getLearnedSet(const bool& create)
{
  if(create && learned_skills == nullptr)
    learned_skills = new SkillSet();
  return learned_skills;
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
 * Description: Evaluates and returns the state of a given PState flag
 *
 * Inputs: test_flag - PState flag to find the value for
 * Output: bool - the value of the given flag to check
 */
bool Person::getPFlag(const PState& test_flag)
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
Rank Person::getRank()
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
 * Output: ElementCurve - the enumerated curve of prim. elemental
 *progression
 */
ElementCurve Person::getPrimaryCurve()
{
  return primary_curve;
}

/*
 * Description: Return the enumerated curve of secd. elemental progression
 *
 * Inputs: none
 * Output: ElementCurve - the enumerated curve of secd. elemental
 *progression
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
Equipment* Person::getEquip(const EquipSlots& equip_slot)
{
  auto index = getEquipIndex(equip_slot);

  if(index < equipments.size())
    return equipments.at(index);

  return nullptr;
}

/*
 * Description: Grabs the corresponding index of a given EquipSlots
 *enumeration
 *
 * Inputs: none
 * Output: uint32_t - the mapped index of a given equip slot
 */
uint32_t Person::getEquipIndex(const EquipSlots& equip_slot)
{
  if(static_cast<uint8_t>(equip_slot) < equipments.size())
  {
    switch(equip_slot)
    {
    case(EquipSlots::HEAD):
      return 0;
    case(EquipSlots::LARM):
      return 1;
    case(EquipSlots::RARM):
      return 2;
    case(EquipSlots::BODY):
      return 3;
    case(EquipSlots::LEGS):
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

Sprite* Person::getSpriteAlly()
{
  return sprite_ally;
}

Sprite* Person::getSpriteAllyDefensive()
{
  return sprite_ally_defensive;
}

Sprite* Person::getSpriteAllyOffensive()
{
  return sprite_ally_offensive;
}

Sprite* Person::getSpriteDialog()
{
  return sprite_dialog;
}

Sprite* Person::getSpriteFace()
{
  return sprite_face;
}

Sprite* Person::getSpriteFoe()
{
  return sprite_foe;
}

Sprite* Person::getSpriteFoeDefensive()
{
  return sprite_foe_defensive;
}

Sprite* Person::getSpriteFoeOffensive()
{
  return sprite_foe_offensive;
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
 * Description: Calculates the total QD regeneration rate from the battle
 *class
 *              and race class bonuses and returns the enumerated result.
 *
 * Inputs: none
 * Output: RegenRate - enumerated regen rate for quantum drive / turn
 */
RegenRate Person::getQDRegenRate()
{
  if(battle_class != nullptr && race_class != nullptr)
  {
    auto total_rate = static_cast<int16_t>(battle_class->getQDRegenRate()) +
                      static_cast<int16_t>(race_class->getQDRegenRate());

    if(total_rate <= static_cast<uint16_t>(RegenRate::GRAND))
      return static_cast<RegenRate>(total_rate);
    else
      return RegenRate::GRAND;
  }

  return RegenRate::ZERO;
}

/*
 * Description: Calculates the total Vita regeneration rate from the battle
 *              class and race class bonuses and returns the enumerated
 *result.
 *
 * Inputs: none
 * Output: RegenRate - enumerated regen rate for vitality / turn
 */
RegenRate Person::getVitaRegenRate()
{
  if(battle_class != nullptr && race_class != nullptr)
  {
    auto total_rate = static_cast<int16_t>(battle_class->getVitaRegenRate()) +
                      static_cast<int16_t>(race_class->getVitaRegenRate());

    if(total_rate <= static_cast<uint16_t>(RegenRate::GRAND))
      return static_cast<RegenRate>(total_rate);
    else
      return RegenRate::GRAND;
  }

  return RegenRate::ZERO;
}

/*
 * Description: Sets the action x and y offset for the rendering person in
 *the
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
 * Description: Evaluates and returns a given PState flag
 *
 * Inputs: flag - the PState flag to be assigned a value
 *         set_value - the value to assign the flag to
 * Output: none
 */
void Person::setPFlag(const PState& flag, const bool& set_value)
{
  (set_value) ? (person_flags |= flag) : (person_flags &= ~flag);
}

/*
 * Description: Evaluates and returns a given PState flag but based on the
 *              PartyType flag which is the type of party.
 *
 * Inputs: PartyType type - the type to correlate the PState flag to
 * Output: none
 */
void Person::setPFlag(const PartyType& type)
{
  PState flag = PState::SLEUTH;

  /* Clear existing state */
  setPFlag(PState::SLEUTH | PState::BEARACKS | PState::MAIN | PState::FINAL |
               PState::BOSS | PState::MINI_BOSS,
           false);

  /* Parse the party type and correlate */
  if(type == PartyType::SLEUTH)
    flag = PState::SLEUTH;
  else if(type == PartyType::BEARACKS)
    flag = PState::BEARACKS;
  else if(type == PartyType::REGULAR_FOE)
    flag = PState::MAIN;
  else if(type == PartyType::MINI_BOSS)
    flag = PState::MINI_BOSS;
  else if(type == PartyType::BOSS)
    flag = PState::BOSS;
  else if(type == PartyType::FINAL_BOSS)
    flag = PState::FINAL;

  /* Set the flag */
  setPFlag(flag, true);
}

/*
 * Description: Assigns a new battle class to the person. This re-generates
 *              both the base stats and base skills upon setting.
 *
 * Inputs: Category* category - the new class category
 * Output: none
 */
void Person::setClass(Category* const category)
{
  battle_class = category;

  updateBaseStats();
  updateBaseSkills();
}

/*
 * Description: Assigns a curve modifiers for the person (Change's level
 *              Fgression)
 *
 * Inputs: prim - enumerated element for primary affiliation
 *         prim_curve - enumerated curve progress rate for primary elmt
 *         secd - enumerated element for secondary affiliation
 *         secd_curve - enumerated curve progress rate for secondary elmt
 * Output: none
 */
void Person::setCurves(Element prim, ElementCurve prim_curve, Element secd,
                       ElementCurve secd_curve, const bool& update_level)
{
  primary = prim;
  secondary = secd;
  primary_curve = prim_curve;
  secondary_curve = secd_curve;

  if(update_level)
    updateBaseStats();
}

/*
 * Description: Assigns a new current attribute set
 *
 * Inputs: new_curr_set - const ref. to an AttributeSet to be assigned
 * Output: bool - true if the assignment was successful
 */
bool Person::setCurr(const AttributeSet& new_curr_set)
{
  if(new_curr_set.getFlag(AttributeState::PERSONAL))
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
  if(curr_max_stats.getFlag(AttributeState::PERSONAL))
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
  if(new_temp_set.getFlag(AttributeState::PERSONAL))
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
bool Person::setDmgMod(const float& new_dmg_mod)
{
  if(Helpers::isInRange(new_dmg_mod, kMIN_DMG_MODI, kMAX_DMG_MODI))
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
bool Person::setExpMod(const float& new_exp_mod)
{
  if(Helpers::isInRange(new_exp_mod, kMIN_EXP_MODI, kMAX_EXP_MODI))
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
bool Person::setEquip(const EquipSlots& slot, Equipment* new_equip)
{
  if(new_equip == nullptr || slot != new_equip->getEquipSlot())
    return false;

  if(new_equip->getEquipFlag(EquipState::EQUIPPED))
    return false;

  if(new_equip->getEquipFlag(EquipState::TWO_HANDED))
  {
    if(getEquip(EquipSlots::LARM) == nullptr &&
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
 * Description: Attempts to assign a new loot set for the Person
 *
 * Inputs: new_credit_drop - new value to be assigned as credit drop loot
 *         new_exp_drop - new value to be assigned for experience drop loot
 *         new_item_drops - new item drop IDs for drop loot
 * Output: bool - true if the loot assignment was successful
 */
bool Person::setLoot(const uint32_t& new_credit_drop,
                     const uint32_t& new_exp_drop,
                     const std::vector<uint32_t>& new_item_drops)
{
  auto all_set = true;

  if(new_credit_drop < kMAX_CREDIT_DROP)
    credit_drop = new_credit_drop;
  else
  {
    credit_drop = kMAX_CREDIT_DROP;
    all_set = false;
  }

  if(new_exp_drop < kMAX_EXP_DROP)
    exp_drop = new_exp_drop;
  else
  {
    exp_drop = kMAX_EXP_DROP;
    all_set = false;
  }

  if(new_item_drops.size() < kMAX_ITEM_DROPS)
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
 * Description: Assigns a new battle race to the person. This re-generates
 *              both the base stats and base skills upon setting.
 *
 * Inputs: Category* category - the new race class category
 * Output: none
 */
void Person::setRace(Category* const category)
{
  race_class = category;

  // updateBaseStats();
  // updateBaseSkills();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              Person.
 *
 * Inputs: const Person &source - the source class constructor
 * Output: Person& - pointer to the copied class
 */
Person& Person::operator=(const Person& source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
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
uint32_t Person::getExpAt(const uint8_t& level)
{
  if(level != 0 && level <= kNUM_LEVELS)
    return exp_table.at(level - 1);

  return 0;
}

/*
 * Description: Grabs the level corresponding to a given experience value
 *
 * Inputs: experience - the experience to find a level for
 * Output: uint8_t - the highest level reachable at the given experience
 */
uint16_t Person::getLevelAt(const uint32_t& experience)
{
  for(size_t i = 0; i < exp_table.size(); i++)
    if(exp_table.at(i) > experience)
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
 * Output: size_t - kNUM_EQUIP_SLOTS - the number of equipment slots for
 *Persons
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
 * Output: uint32_t - kMAX_LVL_EXP - the experience req. at the highets
 *level
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
 * Output: uint32_t - kMAX_EXP_DROP - the maximum experience awarded by a
 *Person
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
