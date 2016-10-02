/*******************************************************************************
 * Class Name: Bubby [Implementation]
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
#include "Game/Player/Bubby.h"

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/
/* Static ID Counter */
int32_t Bubby::id{0};

const uint32_t Bubby::kMIN_EXP       = 75;
const uint32_t Bubby::kMAX_LEVEL_EXP = 858585;
const uint32_t Bubby::kMAX_EXP       = 1500000;
const int32_t Bubby::kUNSET_ID{-1};

/* The exp table level up values */
std::vector<uint32_t> Bubby::exp_table{};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: The blank constructor.
 *
 * Inputs: none
 */
Bubby::Bubby()
               : my_id{++id}
               , game_id{kUNSET_ID}
               , level{0}
               , tier{0}
               , total_exp{kMIN_EXP}
               , type{nullptr}
{ 
}

/*
 * Description: General constructor: constructs a Bubby object with a given 
 *              Flavour type, and uses the info in the Flavour to 
 *              construct the Item portion of the Bubby.
 *
 * Inputs: parent - pointer to the Flavour of the Bubby to create
 *         tier - tier-level to create the Bubby at
 */
Bubby::Bubby(Flavour* const parent, const uint32_t &tier)
  : level{0}
  , tier{tier}
  , total_exp{kMIN_EXP}
  , type{parent}
{
  buildExpTable();

  /* Ensure the tier is within range */
  if (tier >= Flavour::getTiers())
    this->tier = Flavour::getTiers() - 1;

  if (tier != 0)
  {
    /* Assign the Bubby to the minimum level req. at the given tier */
    level = Flavour::getLevels(Flavour::getTiers() - 1);

    /* Update the exp. to reflect the current level's min. value */
    updateExp();
  }
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Constructs the experience table if it is not empty.
 *
 * Inputs: none
 * Output: none
 */
void Bubby::buildExpTable()
{
  if (exp_table.empty())
  {
    uint32_t levels = Flavour::getMaxLevel();

    if (levels > 0)
      exp_table = Helpers::buildExpTable(kMIN_EXP, kMAX_LEVEL_EXP, levels + 1);
  }
}

bool Bubby::canAddExperience()
{
  /* Max Tier can always add experience */
  if (getTier() == Flavour::getTiers() - 1)
    return true;

  if (level >= Flavour::getLevels(getTier() + 1) - 1)
    return false;

  return true;
}

/*
 * Description: Determines whether to level up based on the current experience
 *              and returns true if the level changed values
 *
 * Inputs: none
 * Output: bool - true if the level changed values
 */
bool Bubby::levelUp()
{
  if (level >= Flavour::getMaxLevel())
    return false;

  auto temp_level = level;

  while (total_exp >= exp_table.at(level))
    level++;

  if (level < Flavour::getMaxLevel() && level == Flavour::getLevels(getTier() + 1))
    updateExp();

  return (temp_level == level);
}

/*
 * Description: Increments the tier counter and calls function to update the 
 *              thumbnail.
 *
 * Inputs: none
 * Output: none
 */
void Bubby::tierUp()
{
  tier++;
}

/*
 * Description: Updates the experience of the Bubby to the minimum value
 *              at the Bubby's current level.
 *
 * Inputs: none
 * Output: bool - true if the level was within range and exp updated
 */
bool Bubby::updateExp()
{
  if ((level - 1) < exp_table.size())
  {
    total_exp = exp_table.at(level - 1);

    return true;
  }

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Attempts to add experience to the Bubby
 *
 * Inputs: amount - amount to add to the Bubby's experience
 * Output: bool - true if the experience was added
 */
bool Bubby::addExperience(const uint32_t &amount)
{
  if (tier != 0)
  {
    if (canAddExperience() && total_exp + amount <= kMAX_EXP)
    {
      std::cout << "adding experience" << "\n";
      total_exp += amount;
      levelUp();
    }
    else if (total_exp + amount >= kMAX_EXP)
      total_exp = kMAX_EXP;

    return true;
  }

  return false;
}

/*
 * Description: Prints out the information of the Bubby
 *
 * Inputs: print_tale - boolean whether to print the exp table values
 *         item_info - boolean whether to print out the item info portion
 * Output: none
 */
// void Bubby::print(const bool &print_table, const bool &item_info)
// {
//   /* Print out the Bubby-only portion data in a simple format */
//   std::cout << "--- Bubby --- \n";
//   std::cout << "T: " << tier << " L: " << level << " E: " << total_exp << "\n";
//   std::cout << "Type: " << type->getName() << "\n";
//   std::cout << "Value: " << this->getValue() << "\n";

//   /* Print out the table of exp values */
//   if (print_table)
//     for (uint32_t value : exp_table)
//       std::cout << value << "\n";
  
//   std::cout << "--- /Bubby --- \n";
// }

/*
 * Description: Upgrades only a tier 0 bubby into a tier 1 Bubby, which can
 *              finally be equipped to a signature for use in Battle
 *
 * Inputs: none
 * Output: bool - true if the upgrade took place
 */
bool Bubby::upgrade()
{
  if (tier < Flavour::getTiers() - 1)
  {
    /* Tier up */
    tierUp();

    /* Find the level at which Tier 1 starts */
    level = Flavour::getLevels(getTier());
    updateExp();

    return true;
  }

  return false;
}

/*
 * Description: Returns the level
 *
 * Inputs: none
 * Output: uint32_t - the current level
 */
uint32_t Bubby::getLevel()
{
  return level;
}

/*
 * Description: Finds and returns the next level at which a tier-up can be
 *              reached.
 *
 * Inputs: none
 * Output: uint32_t - level at which next tier can be reached
 */
uint32_t Bubby::getLevelNext()
{
 if (tier < Flavour::getTiers())
   return Flavour::getLevels(tier + 1);

  return 0;
}

/*
 * Description: Calculates and returns the mass at the current tier
 *
 * Inputs: none
 * Output: double - mass at the current tier
 */
uint32_t Bubby::getMass()
{
  if (type != nullptr)
    return type->getMass(tier);

  return 0.0;
}

int32_t Bubby::getID()
{
  return my_id;
}
  
int32_t Bubby::getGameID()
{
  return game_id; 
}

/*
 * Description: Returns the Bubby's current stat boost by ref.
 *
 * Inputs: none
 * Output: AttributeSet& - ref. to the Bubby's stat boost
 */
AttributeSet Bubby::getStats()
{
  if (type != nullptr)
    return type->getStats(tier);

  /* If the Bubby is a default object, return a default set */
  return AttributeSet();
}

/*
 * Description: Returns the tier thumbnail, for displaying on the screen.
 *              Virtual function to take control away from the parent item.
 *
 * Inputs: none
 * Output: Frame* - pointer to frame display data
 */
Frame* Bubby::getThumb()
{
  if(type != nullptr)
    return type->getThumb(tier);
  return nullptr;
}

/*
 * Description: Returns the tier-level
 *
 * Inputs: none
 * Output: uint32_t - the tier-level
 */
uint32_t Bubby::getTier()
{
  return tier;
}

/*
 * Description: Returns the Flavour of the Bubby by reference.
 *
 * Inputs: none
 * Output: Flavour& - ref. to the Bubby's Flavour [type]
 */
Flavour* Bubby::getType()
{
  return type;
}

/*
 * Description: Returns the total experience earned by the Bubby
 *
 * Inputs: none
 * Output: uint32_t - the total experience
 */
uint32_t Bubby::getExp()
{
  return total_exp;
}

/*
 * Description: Returns the experience required for the Bubby to reach the next
 *              level. Returns 0 if the Bubby is at the maximum level.
 *
 * Inputs: none
 * Output: uint32_t - the exp. required to reach the next level or 0
 */
uint32_t Bubby::getExpNext()
{
  if (level < Flavour::getMaxLevel())
    return exp_table.at(level);

  return 0;
}

/*
 * Description: Returns the value of the Bubby, implemented by calculating the
 *              Flavour's base value plus the modifiers of the current tier.
 *
 * Inputs: none
 * Output: uint32_t - the value of the Bubby of its flavour at current tier
 */
uint32_t Bubby::getValue()
{
  if (type != nullptr)
    return type->getValue(tier);

  return 0;
}
