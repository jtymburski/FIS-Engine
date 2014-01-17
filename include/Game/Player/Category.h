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
#include "StringDB.h"

class Category
{
public:
  Category(const std::string &name);

  Category(const std::string &name, const std::string &denonym, 
  	       const AttributeSet &base_stats, const AttributeSet &max_stats, 
  	       SkillSet* const skills = nullptr);

private:

  static bool attr_sets_built;

  AttributeSet base_stats;
  AttributeSet top_stats;

  static AttributeSet max_stats;
  static AttributeSet min_stats;

  std::string description;
  std::string denonym;
	const std::string name;

  SkillSet* const skill_set;

	std::vector<Infliction> immunities;

  /* ------------ Constants --------------- */
  static const std::vector<int> kMIN_VALUES;
  static const std::vector<int> kMAX_VALUES;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  void buildAttrSets();

  void cleanUpStats();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  bool addImmunity(const Infliction &new_immunity);

  bool isImmune(const Infliction &check_immunity);

  bool removeImmunity(const Infliction &rem_immunity);

  void print(const bool &simple = false);

  std::string getDescription();

  std::string getDenonym();

  std::string getName();

  AttributeSet& getBaseSet();

  SkillSet* getSkills();

  AttributeSet& getTopSet();

  bool setDescription(const std::string &new_description);

  bool setDenonym(const std::string &new_denonym);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  static AttributeSet& getMaxSet();

  static AttributeSet& getMinSet();
};

#endif //CATEGORY_H
