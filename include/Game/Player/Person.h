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
* In & Out of Battlefmna
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

#include "FileHandler.h"
#include "Game/Battle/AIModule.h"
#include "Game/Player/Category.h"

/* PState Flags - flags which have impacts outside of Battle */
ENUM_FLAGS(PState)
enum class PState
{
  SLEUTH = 1 << 0,           /* Is this person in the main Sleuth? */
  BEARACKS = 1 << 1,         /* Is this person in the main Bearacks? */
  MAIN = 1 << 2,             /* Is this person the main character? */
  FINAL = 1 << 3,            /* Is this person a final boss? */
  BOSS = 1 << 4,             /* Is this person a boss? */
  MINI_BOSS = 1 << 5,        /* Is this person a mini boss? */
  CAN_GAIN_EXP = 1 << 6,     /* Can this person gain exp? */
  CAN_LEVEL_UP = 1 << 7,     /* Can this person level up? */
  CAN_LEARN_SKILLS = 1 << 8, /* Can this person change skills? */
  CAN_CHANGE_SIG = 1 << 9, /* Can this person modify signature? */
  MAX_LVL = 1 << 10          /* Person reached max level? */
};

class Person
{
public:
  /* Empty constructor */
  Person();

  /* Base Person object constructor */
  Person(const int32_t& game_id, const std::string& name,
         Category* const battle_class, Category* const race_class);

  /* Construct a non-base Person */
  Person(Person* const source);

  /* Annihilates a Person object */
  ~Person();

private:
  /* Action x and y render location */
  int16_t action_x;
  int16_t action_y;

  /* AI Module */
  AIModule* ai_module;

  /* Person IDs */
  int32_t game_id;
  int32_t my_id;

  /* Pointer to the base person */
  Person* base_person;

  /* Flags for the curent Battle State and Person State */
  PState person_flags;

  /* Record of the Person */ // TODO [12-21-13]
  // Record person_record;

  /* Categories of battle class and person */
  Category* battle_class;
  Category* race_class;

  /* Person's string name */
  std::string name;
  std::string name_display;

  /* Rank of the Person */
  Rank rank;

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

  /* Skill sets for the Person */
  SkillSet* base_skills;
  SkillSet* curr_skills;
  SkillSet* learned_skills;

  /* Current modifier to damage for action outcomes (default = 1.00) */
  float dmg_mod;

  /* Current modifier for experience gain */
  float exp_mod;

  /* Loot of Person when defeated */
  std::vector<uint32_t> item_drops;
  uint32_t credit_drop;
  uint32_t exp_drop;

  /* Experience and level of the Person */
  uint16_t level;
  uint32_t total_exp;

  /* Person related sprites (non-build copies) */
  Sprite* sprite_ally;
  Sprite* sprite_ally_defensive;
  Sprite* sprite_ally_offensive;
  Sprite* sprite_dialog;
  Sprite* sprite_face;
  Sprite* sprite_foe;
  Sprite* sprite_foe_defensive;
  Sprite* sprite_foe_offensive;

  /* ------------ Static Private Members --------------- */
  static int id;                          /* Person unique ID counter */
  static std::vector<uint32_t> exp_table; /* Table of exp. values */

public:
  /* ------------ Constants --------------- */
  static const uint8_t kACTION_X; /* Action render X point */
  static const uint8_t kACTION_Y; /* Action render Y point */
  static const uint32_t kID_PLAYER;
  static const size_t kNUM_LEVELS;        /* Number of Levels for Persons */
  static const uint32_t kMAX_CREDIT_DROP; /* Maximum credit award */
  static const uint32_t kMAX_EXP;         /* Max. Exp possible */
  static const uint32_t kMAX_EXP_DROP;    /* Maximum award for one Exp */
  static const size_t kMAX_ITEM_DROPS;    /* Maximum # Item drops */
  static const uint32_t kMAX_LVL_EXP;     /* Exp to reach final level */
  static const uint32_t kMIN_EXP;         /* Minimum Exp possible */
  static const uint32_t kMIN_LVL_EXP;     /* Starting exp. */
  static const uint32_t kMIN_LVL_IMPLODE; /* The minimum lvl to implode at */
  static const float kMIN_DMG_MODI;       /* Min. Dmg modifier */
  static const float kMAX_DMG_MODI;       /* Max. Dmg modifier */
  static const float kMIN_EXP_MODI;       /* Min. experience modifier */
  static const float kMAX_EXP_MODI;       /* Max. experience modifier */
  static const int32_t kUNSET_ID;         /* The unset id of the person */

  static const std::vector<float> kPRIM_MODS; /* Primary elm curv modifiers */
  static const std::vector<float> kSECD_MODS; /* Secondary elm curv mods */

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const Person& source);

  /* Loads the default values for the Person */
  void loadDefaults();

  /* Sets up the class, based on whether base_person is assigned or not */
  void setupClass();

  /* Unsets all members of the class, bools to clear memory */
  void unsetAll(const bool& clear = false);

  /* Unsets the sprites for the person */
  void unsetSprites();

  /* Updates the level of the person based on their current total experience */
  void updateLevel(const bool& ignore_flags = false);

  /* Updates the rank of the Person based on their Person record */
  void updateRank();

  /*===================== PRIVATE STATIC FUNCTIONS ===========================*/
private:
  /* Constructs the table of experience values given the current constants */
  static void buildExpTable();

  /* Returns the curve modifier given a curve value and whether to check prim */
  static float getCurveModifier(const ElementCurve& curve,
                                const bool primary = true);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Adds an amount of experience and may update the level */
  bool addExp(const uint32_t& amount, const bool& update = true,
              const bool& ignore_flags = false);

  /* Calculates the % of vitality/qtdr which a target value would be */
  float calcVitaPercentAtVal(uint32_t target_value);
  float calcQtdrPercentAtVal(uint32_t target_value);

  /* Clears the skills the player has learned */
  void clearLearnedSkills();

  /* Shorthand function for dealing damage, returns true if the Person KO's */
  bool doDmg(const uint32_t& amount);

  /* Return the (floored) % of exp. reached towards the next level */
  uint16_t findExpPercent();

  /* Find the amount of experience per percentage of this level */
  int32_t findExpPerPC();

  /* Determine the amount of experience earned this level */
  uint16_t findExpThisLevel();

  /* Determines if the person is a power defender */
  bool isPowerDefender();

  /* Determines if the person is a power guarder */
  bool isPowerGuarder();

  /* Load data from file */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer,
                std::string base_path);

  /* Lose an amount of experience */
  bool loseExp(const uint32_t& amount, const bool& update = true,
               const bool& ignore_flags = false);

  /* Lose a percentage of experience towards the next level */
  bool loseExpPercent(const uint16_t& percent);

  /* Method for printing out the data of the class */
  void print(const bool& simple = true, const bool& equips = false,
             const bool& flags = false, const bool& skills = false);

  /* Restoring functions */
  void restoreHealth();
  void restoreQtdr();

  /* Save data to file */
  bool saveData(FileHandler* fh);

  /* Recalculates the Base skills based on categories */
  void updateBaseSkills();

  /* Recalculates the Person's base and base_max stats based on categories */
  void updateBaseStats();

  /* Recalculates Curr skills based on flags */
  void updateSkills();

  /* Updates the current stats of the Person based on their current level */
  void updateStats();

  /* Returns the action frames */
  Sprite* getSpriteAlly();
  Sprite* getSpriteAllyDefensive();
  Sprite* getSpriteAllyOffensive();
  Sprite* getSpriteDialog();
  Sprite* getSpriteFace();
  Sprite* getSpriteFoe();
  Sprite* getSpriteFoeDefensive();
  Sprite* getSpriteFoeOffensive();

  /* The action X and Y render location in battle */
  int16_t getActionX();
  int16_t getActionY();

  /* Returns the person AI module handler */
  AIModule* getAI();

  /* Returns the game_id (not unique) of the Person */
  int32_t getGameID() const;

  /* Returns the learned skill set - and creates if flagged */
  SkillSet* getLearnedSet(const bool& create = false);

  /* Returns the my_id (unique) of the Person */
  int32_t getMyID();

  /* Evaluates and returns the state of a given PState flag */
  bool getPFlag(const PState& test_flag);

  /* Returns a pointer to the assigned base person */
  Person* getBasePerson();

  /* Returns a pointer to the battle class category */
  Category* getClass();

  /* Returns a pointer to the race category */
  Category* getRace();

  /* Returns the string name */
  std::string getName() const;
  std::string getNameDisplay() const;

  /* Returns the enumerated rank of the Person */
  Rank getRank();

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

  /* Gets temp stats */
  AttributeSet& getTemp();

  /* Returns the base skills of the Person */
  SkillSet* getBaseSkills();

  /* Returns the assigned current skills of the Person */
  SkillSet* getCurrSkills();

  /* Returns the damage modifier value */
  float getDmgMod();

  /* Returns the experience modifier value */
  float getExpMod();

  /* Returns the credit drop reward */
  uint32_t getCreditDrop();

  /* Returns the exp drop reward */
  uint32_t getExpDrop();

  /* Returns the current level */
  uint16_t getLevel();

  /* Returns the total experience earned */
  uint32_t getTotalExp();

  /* Pointer to the unbuilt first person sprite */

  /* Grabs the vector of item IDs the person can drop */
  std::vector<uint32_t> getItemDrops();

  /* Returns the real % QD value of the maximum QD */
  float getQDPercent();

  /* Returns the real % VITA value of the maximum VITA */
  float getVitaPercent();

  /* Calculate and return the /turn RegenRates for QD & VITA */
  RegenRate getQDRegenRate();
  RegenRate getVitaRegenRate();

  /* Returns a vector of valid enumerated action types */
  std::vector<ActionType> getValidActions();

  /* Sets the action x and y offset */
  void setActionXY(int16_t action_x, int16_t action_y);

  /* Assigns a new AI Module for the person */
  void setAI(AIModule* const new_ai_module);

  /* Assigns a value to a given PState flag */
  void setPFlag(const PState& flag, const bool& set_value = true);
  void setPFlag(const PartyType& type);

  /* Assigns a new battle class */
  void setClass(Category* const category);

  /* Assigns curve modifiers for the person (change's level progression) */
  void setCurves(Element prim, ElementCurve prim_curve, Element secd,
                 ElementCurve secd_curve, const bool& update_level = false);

  /* Assigns a new curr Attr set */
  bool setCurr(const AttributeSet& new_curr_set);

  /* Assigns a new curr max Attr set */
  bool setCurrMax(const AttributeSet& new_curr_max_set);

  /* Assigns a new temp attr set */
  bool setTemp(const AttributeSet& new_temp_set);

  /* Assigns a new damage modifier value */
  bool setDmgMod(const float& new_dmg_mod);

  /* Assigns a new experience modifier value */
  bool setExpMod(const float& new_exp_mod);

  /* Assigns the game ID for the person */
  void setGameID(int id);

  /* Attempts to assign a new loop set for the person */
  bool setLoot(const uint32_t& new_credit_drop, const uint32_t& new_exp_drop,
               const std::vector<uint32_t>& new_item_drops);

  /* Assigns the name of the person */
  void setName(std::string name);
  void setNameDisplay(std::string name_display);

  /* Assigns a new race class */
  void setRace(Category* const category);

  /*===================== PUBLIC STATIC FUNCTIONS ============================*/
public:
  /* Grabs the experience required to reach a given level */
  static uint32_t getExpAt(const uint8_t& level);

  /* Grabs the highest level at a certain experience value */
  static uint16_t getLevelAt(const uint32_t& experience);

  /* Grabs the number the total number of levels for Person progression */
  static size_t getNumLevels();

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

  /*======================= OPERATOR FUNCTIONS ===============================*/
public:
  /* The copy operator */
  Person& operator=(const Person& source);
};

#endif // PERSON_H
