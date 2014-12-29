/*******************************************************************************
* Class Name: Person [Declaration]
* Date Created: December 21st, 2013
* Inheritance: None
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
* base_max_stats - the level max stats of the Person calc by battle_class + race
* 
* Out of Battle
* -------------
* curr_sats      - unused
* curr_max_stats - normal stats between base and base max at the person's level
* temp_max_stats - curr_max_stats + equipmemt
*
* In Battle
* ---------
* curr_stats     - displayed value of stats after Battle modifiers
* curr_max_stats - possible reachable stat values after Battle modifiers
* temp_max_stats - the normal value of stats before Batle modifiers
*
* TODO
* ----
* - PersonRecord [12-21-13]
*******************************************************************************/
#ifndef PERSON_H
#define PERSON_H

class AIModule;

#include "Game/Battle/AIModule.h"
#include "Game/Player/Category.h"
#include "Game/Player/Equipment.h"

/* Structure representing a skill and its possible targets for targ. selec. */
struct BattleSkill
{
  Skill* skill;
  bool selectable;

  std::vector<Person*> all_targets;
  std::vector<Person*> ally_targets;
  std::vector<Person*> foe_targets;
};

/* Structure representing an item and its amt and possible targets */
struct BattleItem
{
  Item* item;
  Skill* item_skill;
  uint32_t amount;
  bool selectable;

  std::vector<Person*> all_targets;
  std::vector<Person*> ally_targets;
  std::vector<Person*> foe_targets;
};

/* BState Flags - Flags which describe only battle-related flags */
ENUM_FLAGS(BState)
enum class BState
{
  IN_BATTLE          = 1 << 0, /* Is the person in a Battle? */
  ALIVE              = 1 << 1, /* Is the person alive? */
  ATK_ENABLED        = 1 << 2, /* Can the person use 0-QD skills? */
  SKL_ENABLED        = 1 << 3, /* Can the person use >0-QD skills? */
  ITM_ENABLED        = 1 << 4, /* Can the person use item in battle? */
  DEF_ENABLED        = 1 << 5, /* Can the person use 'Defend'? */
  GRD_ENABLED        = 1 << 6, /* Can the person use 'Guard?' */
  IMP_ENABLED        = 1 << 7, /* Can the person use 'Implode'? */
  INS_ENABLED        = 1 << 8, /* Can the person use 'Inspect' ? */
  RUN_ENABLED        = 1 << 9, /* Can the person use 'Run'? */
  PAS_ENABLED        = 1 << 10, /* Can the person use 'Pass'? */
  RANDOM_SELECTION   = 1 << 11, /* Does the person select randomly? */
  SKIP_NEXT_TURN     = 1 << 12, /* Will the person skip their next turn? */
  MISS_NEXT_TARGET   = 1 << 13, /* Will the person's next action miss? */
  NEXT_ATK_NO_EFFECT = 1 << 14, /* Will the person's next atk have no effect? */
  IS_BUBBY           = 1 << 15, /* Is the person 'Bubbified'? */
  TWO_SKILLS         = 1 << 16, /* Can the person use two skills per turn? */
  THREE_SKILLS       = 1 << 17, /* Can the person use three skills per turn? */
  HALF_COST          = 1 << 18, /* Is person's skill cost halved? */
  REFLECT            = 1 << 19, /* Will the person reflect elemental skills? */
  BOND               = 1 << 20, /* Is the person under 'Bond'? */
  BONDED             = 1 << 21, /* Person bonded to someone under 'Bond' ?*/
  REVIVABLE          = 1 << 22, /* Can this person be revived if they are KO? */
  SELECTED_ACTION    = 1 << 23, /* Has this person selected an action? */
  SELECTED_2ND_ACTION = 1 << 24, /* Has this person selected a 2nd action? */
  SELECTED_3RD_ACTION = 1 << 25, /* Has this person selected a 3rd action? */
  CAN_CRIT            = 1 << 26, /* Can this person crit against a target? */
  CAN_BE_CRIT         = 1 << 27, /* Can this person have crit's against them? */
  DEFENDING           = 1 << 28, /* Is this person defending currently? */
  GUARDED             = 1 << 29, /* Is this person being guarded? */
  GUARDING            = 1 << 30, /* Is this person guarding? */
  SHIELDED            = 1 << 31, /* Is this person shielded from damage? */
};

/* PState Flags - flags which have impacts outside of Battle */
ENUM_FLAGS(PState)
enum class PState 
{
  SLEUTH             = 1 << 0, /* Is this person in the main Sleuth? */
  BEARACKS           = 1 << 1, /* Is this person in the main Bearacks? */
  MAIN               = 1 << 2, /* Is this person in main character? */
  FINAL              = 1 << 3, /* Is this person a final boss? */
  BOSS               = 1 << 4, /* Is this person a boss? */
  MINI_BOSS          = 1 << 5, /* Is this person a mini boss? */
  CAN_GAIN_EXP       = 1 << 6, /* Can this person gain exp? */
  CAN_LEVEL_UP       = 1 << 7, /* Can this person level up? */
  CAN_LEARN_SKILLS   = 1 << 8, /* Can this person change skills? */
  CAN_CHANGE_EQUIP   = 1 << 9,  /* Can this person modify equipment? */
  MAX_LVL            = 1 << 10 /* Person reached max level? */
};

class Person
{
  friend class Ailment;
  
public:
  /* Base Person object constructor */
  Person(const int32_t &game_id, const std::string &name, 
         Category* const battle_class, Category* const race_class);

  /* Construct a non-base Person */
  Person(Person* const source);

  /* Annihilates a Person object */
  ~Person();

private:
  /* Action x and y render location */
  uint8_t action_x;
  uint8_t action_y;

  /* AI Module */
  AIModule* ai_module;

	/* Person IDs */
	int32_t game_id;
	int32_t my_id;
	
	/* Pointer to the base person */
  Person* base_person;
  Person* guardee;
  Person* guard;

  /* Flags for the curent Battle State and Person State */
  BState battle_flags;
  PState person_flags;

  /* Record of the Person */ //TODO [12-21-13]
  //Record person_record;

  /* Categories of battle class and person */
  Category* battle_class;
  Category* race_class;

  /* Person's string name */
  std::string name;
  
  /* Rank of the Person */
  Ranks rank;

  /* Elemental affiliations and curves of stat development */
  Element primary;
  Element secondary;
  ElementCurve primary_curve;
  ElementCurve secondary_curve;

  /* Attribute set containers for storing stat values
   * See Note[1] at top of file for descriptions
   */
  AttributeSet base_stats;
  AttributeSet base_max_stats;
  AttributeSet curr_stats;
  AttributeSet curr_max_stats;
  AttributeSet temp_max_stats;


  /* Skill sets fro the Person */
  SkillSet* base_skills;
  SkillSet* curr_skills;
  SkillSet* learned_skills;
  SkillSet* temp_skills;

  /* Current modifier to damage for action outcomes (default = 1.00) */
  float dmg_mod;

  /* Current modifier for experience gain */
  float exp_mod;

  /* Current equipment attached to the Person */
  std::vector<Equipment*> equipments;

  /* Loot of Person when defeated */
  std::vector<uint32_t> item_drops;
  uint32_t credit_drop;
  uint32_t exp_drop;

  /* Experience and level of the Person */
  uint16_t level;
  uint32_t total_exp;

  /* Frames for a Person */
  Sprite* first_person;
  Sprite* third_person;
  //Frame* first_person;
  //Frame* third_person;

  /* ------------ Static Private Members --------------- */
  static int id; /* Person unique ID counter */
  static std::vector<uint32_t> exp_table; /* Table of exp. values */

  /* ------------ Constants --------------- */
  static const uint8_t  kACTION_X;        /* Action render X point */
  static const uint8_t  kACTION_Y;        /* Action render Y point */
  static const size_t   kNUM_LEVELS;      /* Number of Levels for Persons */
  static const size_t   kNUM_EQUIP_SLOTS; /* Number of Equip Slots */
  static const uint32_t kMAX_CREDIT_DROP; /* Maximum credit award */
  static const uint32_t kMAX_EXP;         /* Max. Exp possible */
  static const uint32_t kMAX_EXP_DROP;    /* Maximum award for one Exp */
  static const size_t   kMAX_ITEM_DROPS;  /* Maximum # Item drops */
  static const uint32_t kMAX_LVL_EXP;     /* Exp to reach final level */
  static const uint32_t kMIN_EXP;         /* Minimum Exp possible */
  static const uint32_t kMIN_LVL_EXP;     /* Starting exp. */
  static const float kMIN_DMG_MODI;       /* Min. Dmg modifier */
  static const float kMAX_DMG_MODI;       /* Max. Dmg modifier */
  static const float kMIN_EXP_MODI;       /* Min. experience modifier */
  static const float kMAX_EXP_MODI;       /* Max. experience modifier */
 
  static const std::vector<float> kPRIM_MODS; /* Primary elm curv modifiers */
  static const std::vector<float> kSECD_MODS; /* Secondary elm curv mods */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Loads the default values for the Person */
  void loadDefaults();

  /* Sets up the class, based on whether base_person is assigned or not */
  void setupClass();

  /* Unsets all members of the class, bools to clear memory */
  void unsetAll(const bool &clear = false);

  /* Recalculates the Person's base and base_max stats based on categories */
  void updateBaseStats();

  /* Recalculates the Base skills based on categories */
  void updateBaseSkills();

  /* Recalculates the stat bonuses for equipment */
  void updateEquipStats();

  /* Updates the level of the person based on their current total experience */
  void updateLevel();

  /* Updates the current stats of the Person based on their current level */
  void updateStats();

  /* Recalculates Curr skills based on flags */
  void updateSkills();

  /* Updates the rank of the Person based on their Person record */
  void updateRank();

  /* Constructs the table of experience values given the current constants */
  static void buildExpTable();

  /* Returns the curve modifier given a curve value and whether to check prim */
  static float getCurveModifier(const ElementCurve &curve, 
                                const bool primary = true);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds an amount of experience and may update the level */
  bool addExp(const uint32_t &amount, const bool &update = true);

  /* Return the (floored) % of exp. reached towards the next level */
  uint16_t findExpPercent();

  /* Find the amount of experience per percentage of this level */
  int32_t findExpPerPC();

  /* Determine the amount of experience earned this level */
  uint16_t findExpThisLevel();

  /* Lose an amount of experience */
  bool loseExp(const uint32_t &amount, const bool &update = true);

  /* Lose a percentage of experience towards the next level */
  bool loseExpPercent(const uint16_t &percent);

  /* Prepares the person for entering Battle (flags, Attributes etc.) */
  void battlePrep();
  void battleTurnPrep();

  /* Clears the skills the player has learned */
  void clearLearnedSkills();

  /* Creates an AI module given a difficulty and personalities */
  bool createAI(const AIDifficulty &diff, const AIPersonality &prim_personality,
                const AIPersonality &secd_personality);

  /* Shorthand function for dealing damage, returns true if the Person KO's */
  bool doDmg(const uint32_t &amount, DamageType damage_type);

  /* Determines if the person is a power defender */
  bool isPowerDefender();

  /* Determines if the person is a power guarder */
  bool isPowerGuarder();

  /* Method for printing out the data of the class */
  void print(const bool &simple = true, const bool &equips = false,
             const bool &flags = false, const bool &skills = false);

  /* Removes the equipment from a given slot */
  bool removeEquip(const EquipSlots &equip_slot);

  // TODO
  void resetActionFlags();

  /* Resets the AI module for a new turn */
  bool resetAI();

  /* Clear the guard status of this person */
  bool resetGuard();

  /* Clear the defend status of this person */
  bool resetDefend();

  /* Clear the guarding status of this person */
  bool resetGuardee();

  /* The action X and Y render location in battle */
  uint8_t getActionX();
  uint8_t getActionY();

  /* Returns a pointer to the AI module */
  AIModule* getAI();

  /* Returns the real % QD value of the maximum QD */
  float getQDPercent();

  /* Returns the real % VITA value of the maximum VITA */
  float getVitaPercent();

  /* Find the true cost for a Skill to the Person's QD */
  int16_t getTrueCost(Skill* test_skill);

  /* Returns the game_id (not unique) of the Person */
  int32_t getGameID();

  /* Returns the my_id (unique) of the Person */
  int32_t getMyID();

  /* Evaluates and returns the state of a given BState flag */
  bool getBFlag(const BState &test_flag);

  /* Evaluates and returns the state of a given PState flag */
  bool getPFlag(const PState &test_flag);

  /* Returns a pointer to the assigned base person */
  Person* getBasePerson();

  /* Return the person guarding this person */
  Person* getGuard();

  /* Return the person guarded by this person */
  Person* getGuardee();

  /* Returns a pointer to the battle class category */
  Category* getClass();

  /* Returns a pointer to the race category */
  Category* getRace();

  /* Returns the string name */
  std::string getName();

  /* Returns the enumerated rank of the Person */
  Ranks getRank();

  /* Returns the primary elemental affiliation */
  Element getPrimary();

  /* Return the secondary elemental affiliation */
  Element getSecondary();

  /* Return the enumerated curve of prim. elemental progression */
  ElementCurve getPrimaryCurve();

  /* Return the enumerated curve of secd. elemental progression */
  ElementCurve getSecondaryCurve();

  /* Returns a reference to the base stats */
  AttributeSet& getBase();

  /* Returns a ref to the base max stats */
  AttributeSet& getBaseMax();

  /* Returns a ref to the curr stats */
  AttributeSet& getCurr();

  /* Returns a ref to the curr max stats */
  AttributeSet& getCurrMax();

  /* Returns the temp stats */
  AttributeSet& getTemp();

  /* Returns the base skills of the Person */
  SkillSet* getBaseSkills();

  /* Returns the assigned current skills of the Person */
  SkillSet* getCurrSkills();

  /* Returns the damage modifier value */
  float getDmgMod();

  /* Returns the experience modifier value */
  float getExpMod();

  /* Returns a pointer to the equipment a given slot */
  Equipment* getEquip(const EquipSlots &equip_slot);

  /* Returns the corresponding index slot of a given enumerated EquipSlots */
  uint32_t getEquipIndex(const EquipSlots &equip_slot);

  /* Returns the credit drop reward */
  uint32_t getCreditDrop();

  /* Returns the exp drop reward */
  uint32_t getExpDrop();

  /* Returns the current level */
  uint16_t getLevel();

  /* Returns the total experience earned */
  uint32_t getTotalExp();

  /* Grabs the curr first person frame (based on BUBBIFIED flags) */
  Sprite* getFirstPerson();

  /* Grabs the curr third person frame (based on BUBBIFIED flags) */
  Sprite* getThirdPerson();

  /* Grabs the vector of item IDs the person can drop */
  std::vector<uint32_t> getItemDrops();

  /* Calculate and return the /turn RegenRates for QD & VITA */
  RegenRate getQDRegenRate();
  RegenRate getVitaRegenRate();

  /* Calculates and determines current useable skills for Battle */
  SkillSet* getUseableSkills();

  /* Returns a vector of valid enumerated action types */
  std::vector<ActionType> getValidActions();

  /* Sets the action x and y offset */
  void setActionXY(uint8_t action_x, uint8_t action_y);

  /* Assigns a new AI Module for the person */
  void setAI(AIModule* const new_ai_module);

  /* Evaluates and returns a given battle state flag */
  void setBFlag(const BState &flag, const bool &set_value = true);

  /* Evaluates and returns a given person state flag */
  void setPFlag(const PState &flag, const bool &set_value = true);

  /* Assigns curve modifiers for the person (change's level progression) */
  void setCurves(Element prim, ElementCurve prim_curve,
                 Element secd, ElementCurve secd_curve,
                 const bool &update_level = false);

  /* Assigns a new curr Attr set */
  bool setCurr(const AttributeSet& new_curr_set);

  /* Assigns a new curr max Attr set */
  bool setCurrMax(const AttributeSet& new_curr_max_set);

  /* Assigns a new temp attr set */
  bool setTemp(const AttributeSet& new_temp_set);

  /* Assigns a new damage modifier value */
  bool setDmgMod(const float &new_dmg_mod);

  /* Assigns a new experience modifier value */
  bool setExpMod(const float &new_exp_mod);

  /* Attempts to assign a given equipment slot a given equipment pointer */
  bool setEquip(const EquipSlots &equip_slot, Equipment* new_equipment);

  /* Assigns a new guard (person guarding this object) */
  bool setGuard(Person* const new_guard);

  /* Assign a new person this person is guarding */
  bool setGuardee(Person* const new_guarding_person);

  /* Attempts to assign a new loop set for the person */
  bool setLoot(const uint32_t &new_credit_drop, const uint32_t &new_exp_drop, 
               const std::vector<uint32_t> &new_item_drops);

  /* Assigns the sprite pointers for the person */
  void setSprites(Sprite* new_fp = nullptr, Sprite* new_tp = nullptr);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Grabs the experience required to reach a given level */
  static uint32_t getExpAt(const uint8_t &level);

  /* Grabs the highest level at a certain experience value */
  static uint16_t getLevelAt(const uint32_t &experience);

  /* Grabs the number the total number of levels for Person progression */
  static size_t getNumLevels();

  /* Grabs the number of equipment slots a Person has */
  static size_t getNumEquipSlots();

  /* Returns the minimum experience possible */
  static uint32_t getMinExp();

  /* Returns the maximum experience possible */
  static uint32_t getMaxExp();

  /* Returns the minimum level experience */
  static uint32_t getMinLvlExp();

  /* Returns the  experience at the final level */
  static uint32_t getMaxLvlExp();

  /* Returns the maximum possible credit drop */
  static uint32_t getMaxCreditDrop();

  /* Returns the maximum experience a Person can reward */
  static uint32_t getMaxExpDrop();

  /* Returns the maximmum # of item drops a Person can reward */
  static uint32_t getMaxItemDrops();

  /* Returns the minimum damage modifier possible */
  static float getMinDmgModi();

  /* Returns the maximum damage modifier possible */
  static float getMaxDmgModi();

  /* Returns the minimum experience modifier possible */
  static float getMinExpModi();

  /* Returns the maximum experience modifier possible */
  static float getMaxExpModi();
};

#endif //PERSON_H
