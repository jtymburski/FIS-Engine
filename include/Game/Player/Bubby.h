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
* [1]:
*
* TODO
* ----
* X - [MM-DD-YY]
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

  Bubby(Flavour* parent, const uint32_t &tier = 0);

private:

  static std::vector<uint32_t> exp_table;

  uint32_t level;

  uint32_t tier;

  uint32_t total_exp;

  Flavour* type;

  /* ------------ Constants --------------- */
  static const uint32_t kMIN_EXP;
  static const uint32_t kMAX_LEVEL_EXP;
  static const uint32_t kMAX_EXP;
  
/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

bool buildExpTable();

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
