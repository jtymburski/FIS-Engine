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

#include "Game/Player/Equipment.h"
#include "EnumDb.h"
#include "EnumFlags.h"
#include "StringDb.h"

/* Category state flags - primarily for enabling battle features
 * and describing the types of equipment a member can equip 
 */
ENUM_FLAGS(CategoryState)
enum class CategoryState
{
  DEF_ENABLED = 1 << 0,
  GRD_ENABLED = 1 << 1,
  IMP_ENABLED = 1 << 2,
  E_STAFF     = 1 << 3,
  E_SWORD     = 1 << 4
  //TODO: Develop classes of equipments
};

class Category
{
public:
  /* Creates an empty category with just a name */
  Category(const std::string &name);

  /* Constructs a normal category with stat values and a SkillSet */
  Category(const std::string &name, const std::string &denonym, 
  	       const AttributeSet &base_stats, const AttributeSet &max_stats, 
  	       SkillSet* const skills = nullptr);

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
	const std::string name;

  /* Pointer to a SkillSet which the category unlocks */
  SkillSet* const skill_set;

  /* Vector of inflictions which the clas is immune to */
	std::vector<Infliction> immunities;

  /* ------------ Constants --------------- */
  static const std::vector<int> kMIN_VALUES;
  static const std::vector<int> kMAX_VALUES;

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

  /* Returns the string name */
  std::string getName();

  /* Returns a ref. to the base attr set */
  AttributeSet& getBaseSet();

  /* Returns a ptr to the SkillSet granted to a member of this Category */
  SkillSet* getSkills();

  /* Returns a ref. to the top attr set */
  AttributeSet& getTopSet();

  /* Assigns a new description */
  bool setDescription(const std::string &new_description);

  /* Assigns a new denonym */
  bool setDenonym(const std::string &new_denonym);

  /* Assigns a given flag to a given value  */
  void setFlag(const CategoryState &flag, const bool &set_value = true);

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
