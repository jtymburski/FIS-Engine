/*******************************************************************************
 * Class Name: Bubby [Declaration]
 * Date Created: December 12th, 2013
 * Inheritance: Public from Item
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
 ******************************************************************************/
#ifndef BUBBY_H
#define BUBBY_H

#include <iostream>

#include "Helpers.h"
#include "Game/Player/Flavour.h"
#include "Game/Player/Item.h"

class Bubby : public Item
{
public:
  /* Blank constructor */
  Bubby();

  /* Constructs a Bubby object with a flavour at a certain tier */
  Bubby(Flavour* parent, const uint32_t &tier = 0);

  /* Default Bubby annihilation */
  ~Bubby() = default;

private:

  /* Experience values for levels of Bubbiesby */
  static std::vector<uint32_t> exp_table;

  /* The level of the Bubby */
  uint32_t level;

  /* The tier [size on signature] of the Bubby */
  uint32_t tier;

  /* Total experience the Bubby has earned in Battle */
  uint32_t total_exp;

  /* Ptr to the type of Bubby */
  Flavour* type;

  /* ------------ Constants --------------- */
  static const uint32_t kMIN_EXP;         /* Starting experience value */
  static const uint32_t kMAX_LEVEL_EXP;   /* Experience required for last lvl */
  static const uint32_t kMAX_EXP;         /* Abs. maximum experience value */

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Determines whether experience can be added */
  bool canAddExperience();

  /* Builds the experience table for the Bubby levels */
  static void buildExpTable();

  /* Increments the Bubby's level */
  bool levelUp();

  /* Increments the Bubby's tier */
  void tierUp();

  /* Updates the thumbnail to the one stored in the Flavour for the curr tier */
  bool updateThumb();

  /* Updates the experience to the minimum req. for the curr level */
  bool updateExp();

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Attempts to add experience to the Bubby and returns if successful */
  bool addExperience(const uint32_t &amount);

  /* Prints out the information for the Bubby */
  void print(const bool& print_table = false, const bool& item_info = false);

  /* This method MUST be called to upgrade the Bubby from Tier 0 to Tier 1 */
  bool upgrade();

  /* Returns the level */
  uint32_t getLevel();

  /* Returns the minimum level required for the Bubby to reach the next tier */
  uint32_t getLevelNext();

  /* Returns the mass at the current tier (determined from Flavour */
  uint32_t getMass();

  /* Calculates and returns the stats the Bubby will provide on a Signature */
  AttributeSet getStats();

  /* Returns the ptr to the thumbnail */
  Frame* getThumb();

  /* Returns the tier */
  uint32_t getTier();

  /* Return the Flavour [type] of the Bubby by ref */
  Flavour* getType();

  /* Returns the total exp */
  uint32_t getExp();

  /* Returns the exp required to reach the next level */
  uint32_t getExpNext();

  /* Returns the value [currency] worth */
  uint32_t getValue();
};

#endif //BUBBY_H
