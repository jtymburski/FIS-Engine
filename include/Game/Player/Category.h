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
*******************************************************************************/

#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>
#include <vector>

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "EnumDb.h"
#include "EnumFlags.h"
#include "StringDb.h"

ENUM_FLAGS(CategoryState)
enum class CategoryState
{
  DEF_ENABLED = 1 << 0,
  GRD_ENABLED = 1 << 1,
  IMP_ENABLED = 1 << 2
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

  static bool attr_sets_built;

  AttributeSet base_stats;
  AttributeSet top_stats;

  static AttributeSet max_stats;
  static AttributeSet min_stats;

  CategoryState flags;

  std::string description;
  std::string denonym;
	const std::string name;

  SkillSet* const skill_set;

  /* Vector of equipment types which the category can equip */
  std::vector<EquipClass> equippables;

  /* Vector of inflictions which the clas is immune to */
	std::vector<Infliction> immunities;

  /* ------------ Constants --------------- */
  static const std::vector<int> kMIN_VALUES;
  static const std::vector<int> kMAX_VALUES;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  void buildAttrSets();

  void classSetup();

  void cleanUpStats();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  bool addEquippable(const EquipClass &new_equip_class);

  bool addImmunity(const Infliction &new_immunity);

  bool isEquippable(const EquipClass &check_equip_class);

  bool isImmune(const Infliction &check_immunity);

  bool removeEquippable(const EquipClass &rem_equip_class);

  bool removeImmunity(const Infliction &rem_immunity);

  void print(const bool &simple = false);

  std::string getDescription();

  std::string getDenonym();

  bool getFlag(const CategoryState &test_flag);

  std::string getName();

  AttributeSet& getBaseSet();

  SkillSet* getSkills();

  AttributeSet& getTopSet();

  bool setDescription(const std::string &new_description);

  bool setDenonym(const std::string &new_denonym);

  void setFlag(const CategoryState &flag, const bool &set_value = true);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  static AttributeSet& getMaxSet();

  static AttributeSet& getMinSet();
};

#endif //CATEGORY_H
