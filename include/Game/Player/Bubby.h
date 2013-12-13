/*******************************************************************************
* Class Name: Bubby [Declaration]
* Date Created: December 12th, 2013
* Inheritance: Subclass of Item
* Description: Bubby subclass defines details to define experience, level
*              flavour, and tier of a Bubby Item.
*
* Notes
* -----
*
* [1]: All Bubbies have a "Flavour" (type) and a Bubby of a certain flavour is
*      identical to another Bubby of the same flavour except for: tier, level, 
*      and experience.
*  
* [2]: This class reimplements virtual functions from Item: getMass(), 
*      getValue(), getThumb(), getStats()
*
* TODO
* ----
* Testing [12-12-13]
* Conventions [12-12-13]
*******************************************************************************/

#ifndef BUBBY_H
#define BUBBY_H

#include <iostream>

#include "Game/Player/Flavour.h"
#include "Game/Player/Item.h"
#include "Helpers.h"

class Bubby : public Item
{
public:
  /* Default constructor: constructs a default Bubby */
  Bubby();

  /* Constructs a Bubby object with a flavour at a certain tier */
  Bubby(Flavour* parent, const uint32_t &tier = 0);

private:

  /* Experience values for levels of Bubby */
  static std::vector<uint32_t> exp_table;

  /* The level of the Bubby */
  uint32_t level;

  /* The tier [size on signature] of the Bubby */
  uint32_t tier;

  /* Total experience the Bubby has earned in Battle */
  uint32_t total_exp;

  /* Ptr to the type of Bubby */
  Flavour* const type;

  /* ------------ Constants --------------- */
  static const uint32_t kMIN_EXP;
  static const uint32_t kMAX_LEVEL_EXP;
  static const uint32_t kMAX_EXP;
  
/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Builds the */
static bool buildExpTable();

bool levelUp();

void tierUp();

bool updateThumb();

bool updateExp();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool addExperience(const uint32_t &amount);

void print(const bool& print_table = false, const bool& item_info = false);

bool upgrade();

uint32_t getLevel();

uint32_t getLevelNext();

double getMass();

AttributeSet getStats();

uint32_t getTier();

Flavour* getType();

uint32_t getExp();

uint32_t getExpNext();

uint32_t getValue();

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

};

#endif //BUBBY_H
