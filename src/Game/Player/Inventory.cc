/*******************************************************************************
* Class Name: x [Implementation]
* Date Created: 
* Inheritance: 
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Inventory.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

uint32_t Inventory::id = 0;

const double Inventory::kMIN_CARRY_MASS = 100.00;
const double Inventory::kMAX_CARRY_MASS = 5000.00;
const uint32_t Inventory::kMIN_ITEM = 40;
const uint32_t Inventory::kMAX_ITEM = 25000;
const uint8_t Inventory::kMIN_EACH_ITEM = 10;
const uint8_t Inventory::kMAX_EACH_ITEM = 99;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
Inventory::Inventory(const uint32 game_id, const std::string name, 
	                   Frame* thumb)
  : bubby_limit(0)
  , equip_limit(0)
  , item_limit(0)
  , item_each_limit(0)
  , mass_limit(0)
  , game_id(game_id)
  , my_id(++id)
  , name(name)
  , description("")
  , backdrop(nullptr)
  , thumbnail(thumb)
{
  setLimits(kMIN_ITEM, kMIN_ITEM, kMIN_ITEM, kITEM_EACH_LIMIT, kMIN_CARRY_MASS);
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/


/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Inventory::setImages(Frame* const new_backdrop, Frame* const new_thumbnail)
{
  backdrop  = new_backdrop;
  thumbnail = new_thumbnail;

  return (new_backdrop != nullptr && new_thumbnail != nullptr)
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
 void Inventory::setLimits(const uint32_t bubby_lim, const uint32_t equip_lim,
 	                         const uint32_t item_lim, const uint32_t item_ea,
 	                         const double mass_lim)
{
  bubby_limit = Helpers::setWithinRange(bubby_lim, kMIN_ITEM, kMAX_ITEM);
  equip_limit = Helpers::setWithinRange(equip_lim, kMIN_ITEM, kMAX_ITEM);
  item_limit = Helpers::setWithinRange(item_lim, kMIN_ITEM, kMAX_ITEM);
  item_ea = Helpers::setWithinRange(item_ea, kMIN_EACH_ITEM, kMAX_EACH_ITEM);
  mass_limit = Helpers::setWithinRange(mass_lim, kMIN_MASS, kMAX_MASS);
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/