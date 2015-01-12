/*******************************************************************************
* Class Name: Category [Declaration]
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
* TODO
* ----
* - Equip check for actual equipment classes once developed [01-24-14]
*******************************************************************************/
#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>
#include <vector>

#include "EnumDb.h"
#include "EnumFlags.h"
#include "StringDb.h"
#include "Game/Player/Equipment.h"

/* Category state flags - primarily for enabling battle features
 * and describing the types of equipment a member can equip 
 */
ENUM_FLAGS(CategoryState)
enum class CategoryState
{
  DEF_ENABLED    = 1 <<  0,
  GRD_ENABLED    = 1 <<  1,
  IMP_ENABLED    = 1 <<  2,
  POWER_DEFENDER = 1 <<  3,
  POWER_GUARDER  = 1 <<  4,
  E_STAFF        = 1 <<  5,
  E_SWORD        = 1 <<  6,
  E_CLAWS        = 1 <<  7,
  E_SMALL_ARMS   = 1 <<  8,
  E_LONG_ARMS    = 1 <<  9,
  E_BIG_ARMS     = 1 << 10,
  E_LIGHT_ARMOR  = 1 << 11,
  E_MID_ARMOR    = 1 << 12,
  E_HEAVY_ARMOR  = 1 << 13,

  //TODO: Develop classes of equipments
};

class Category
{
public:
  /* Blank constructor */
  Category();

  /* Creates an empty category with just a name */
  Category(const int32_t &my_id, const std::string &name);

  /* Constructs a normal category with stat values and a SkillSet */
  Category(const int32_t &my_id, const std::string &name, 
           const std::string &denonym, const AttributeSet &base_stats, 
           const AttributeSet &max_stats, SkillSet* const skills = nullptr);

private:
  /* Has the attribute min and max sets been built? */
  static bool attr_sets_built;

  /* The bonus stat sets for the min. and max. level */
  AttributeSet base_stats;
  AttributeSet top_stats;

  /* The categories minimum and maximum attributes */
  static AttributeSet max_stats;
  static AttributeSet min_stats;
 
  /* Flags set for the current category */
  CategoryState cat_flags;

  /* Description, name, and denonym (ex. Bears) */
  std::string description;
  std::string denonym;
	std::string name;

  /* Regen rate for the category (default: NONE) */
  RegenRate qtdr_regen_rate;
  RegenRate vita_regen_rate;

  /* Pointer to a SkillSet which the category unlocks */
  SkillSet* const skill_set;

  /* ID of the Category */
  int32_t id;

  /* Vector of inflictions which the clas is immune to */
	std::vector<Infliction> immunities;

  /* ------------ Constants --------------- */
  static const std::vector<int> kMIN_VALUES;
  static const std::vector<int> kMAX_VALUES;
  static const int32_t kUNSET_ID; /* The unset ID for the category ID */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Constructs the Category's absolute min. and max. attr sets */
  static void buildAttrSets();

  /* Construction setup */
  void classSetup();

  /* Clean up the base and top stats */
  void cleanUpStats();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Checks if a piece of equipment can be equipped by a person of this cat. */
  bool canEquip(Equipment* const check);

  /* Adds an enumerated infliction to the list of immunities */
  bool addImmunity(const Infliction &new_immunity);

  /* Checks if a person of this cat. is immune to a given infliction */
  bool isImmune(const Infliction &check_immunity);
 
  /* Removes a given infliction from the list of immunities (if it exists) */
  bool removeImmunity(const Infliction &rem_immunity);

  /* Prints out the state of the object */
  void print(const bool &simple = false, const bool &flags = false);
 
  /* Returns the string denonym */
  std::string getDenonym();

  /* Returns the string description */
  std::string getDescription();
  
  /* Evaluates and returns a given CategoryState flag */
  bool getFlag(const CategoryState &test_flag);

  /* Returns th ID of the Category */
  int32_t getID();

  /* Returns the string name */
  std::string getName();

  /* Returns a ref. to the base attr set */
  AttributeSet& getBaseSet();

  /* Return the enumerated RegenRate for quantum drive */
  RegenRate getQDRegenRate() const;

  /* Return the enumerated RegenRate for vitality */
  RegenRate getVitaRegenRate() const;

  /* Returns a ptr to the SkillSet granted to a member of this Category */
  SkillSet* getSkills();

  /* Returns a ref. to the top attr set */
  AttributeSet& getTopSet();

  /* Assigns a new denonym */
  bool setDenonym(const std::string &new_denonym);

  /* Assigns a new description */
  bool setDescription(const std::string &new_description);

  /* Assigns a given flag to a given value  */
  void setFlag(const CategoryState &flag, const bool &set_value = true);

  /* Assigns the id for the category */
  void setID(int32_t id);

  /* Assigns the name for the category */
  void setName(const std::string &name);

  /* Assign a new enumerated quantum drive regeneration rate */
  void setQDRegenRate(const RegenRate &new_regen_rate);
  
  /* Assign a new enumerated vitality regeneration rate */
  void setVitaRegenRate(const RegenRate &new_regen_rate);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns a ref. to the maximum allowable AttributeSet */
  static AttributeSet& getMaxSet();

  /* Returns a ref. to the minimum allowable AttributeSet */
  static AttributeSet& getMinSet();
};

#endif //CATEGORY_H
