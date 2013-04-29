/******************************************************************************
* Class Name: Item
* Date Created: Dec 2 2012
* Inheritance: InventoryUnit
* Description: The Item Header that defines all elements within Inventory.
*              This includes Equipment and Bubby.
******************************************************************************/
#include "Game/Player/Person.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for Item class
 *
 * Inputs: QWidget
 */
Item::Item()
{
}

/*
 * Description: Annihilates an Item class object
 */
Item::~Item() {}

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the category of an item
 *
 * Inputs: ItemState flag to be toggled
 * Output: none
 */
QString Item::getCategory()
{
    return item_category;
}

/*
 * Description: Returns the description of an item
 *
 * Inputs: none
 * Output: QString - description of the item
 */
QString Item::getDescription()
{
    return description;
}

/*
 * Description: Returns the name of the item
 *
 * Inputs: QString - name of the item
 * Output: none
 */
QString Item::getName()
{
    return name;
}

/*
 * Description: Gets the image thumbnail of the item
 *
 * Inputs: none
 * Output: QImage - image thumbnail of the item
 */
Sprite* Item::getThumb()
{
  return thumb;
}

/*
 * Description: Returns the duration of an item (turns that it can be used)
 *
 * Inputs: none
 * Output: uint - # of turns it can be used in battle
 */
uint Item::getDuration()
{
    return duration;
}

/*
 * Description: Gets the weight of the item
 *
 * Inputs: none
 * Output: double - the weight of the item
 */
double Item::getWeight()
{
  return weight;
}

/*
 * Description: Evaluates a given ItemState flag
 *
 * Inputs: ITemState flag to be evaluated
 * Output: boolean value of the flag
 */
bool Item::getItemFlag(ItemState flag)
{
  return (iflag_set.testFlag(flag));
}

/*
 * Description: Sets the sprite of an item
 *
 * Inputs: Sprite* - pointer to a sprite object
 * Output: none
 */
void Item::setThumb(Sprite* new_thumb)
{
  thumb = new_thumb;
}

/*
 * Description: Given an ItemState object and a bool, sets the flags
 *              contained in the object to the boolean value.
 *
 * Inputs: ItemSTate flag to be set
 *         set_value - boolean value to set flags to
 * Output: none
 */
void Item::setItemFlag(ItemState flag, bool set_value)
{
  (set_value) ? (iflag_set |= flag) : (iflag_set ^= flag);
}

/*
 * Description: Sets the value of duration (time the item lasts in battle)
 *
 * Input: int - value to set the duration of the item too
 * Output: none
 */
void Item::setDuration(uint value)
{
  duration = value;
}

/*
 * Description: Sets the weight value of the item.
 *
 * Inputs: double - new value to set weight to
 * Output: none
 */
void Item::setWeight(double new_value)
{
  weight = new_value;
}
