/******************************************************************************
* Class Name: Item
* Date Created: Dec 2 2012
* Inheritance: InventoryUnit
* Description: The Item Header that defines all elements within Inventory.
*              This includes Equipment and Bubby.
******************************************************************************/
#include "Game/Player/Person.h"

/*
 * Description: Constructor for Item class
 *
 * Inputs: QWidget
 */
Item::Item(QWidget* parent)
{
}

/*
 * Description: Annihilates an Item class object
 */
Item::~Item()
{
}

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
QImage Item::getThumbnail()
{
    return thumbnail;
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
 * Description: Toggles a given ItemState flag
 *
 * Inputs: ItemState flag to be toggled
 * Output: none
 */
void Item::toggleItemFlag(ItemState flag)
{
    setItemFlag(flag, !getItemFlag(flag));
}

/*
 * Description: Evaluates a given ItemState flag
 *
 * Inputs: ITemState flag to be evaluated
 * Output: boolean value of the flag
 */
const bool Item::getItemFlag(ItemState flag)
{
    return (iflag_set.testFlag(flag));
}

/*
 * Description: Given an ItemState object and a bool, sets the flags
 *              contained in the object to the boolean value.
 *
 * Inputs: ItemSTate flag to be set
 *         set_value - boolean value to set flags to
 * Output: none
 */
void Item::setItemFlag(ItemState flag, const bool set_value)
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
