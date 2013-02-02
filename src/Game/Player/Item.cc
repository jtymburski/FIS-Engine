/******************************************************************************
* Class Name: Item
* Date Created: Dec 2 2012
* Inheritance: InventoryUnit
* Description: The Item Header that defines all elements within Inventory.
*              This includes Equipment and Bubby.
******************************************************************************/
#include "Game/Player/Person.h"

/*Constructor function*/
Item::Item(QWidget* parent)
{
}

/*Destructor function*/
Item::~Item()
{
}

/*Evaulates BATTLEREADY flag*/
bool Item::isBattleReady()
{
    return BATTLEREADY;
}

/*Evaluates STACKABLE flag*/
bool Item::isStackable()
{
    return STACKABLE;
}

/*Evaluates INDEFINITE flag*/
bool Item::isIndefinite()
{
    return INDEFINITE;
}

/*Gets category of unit*/
QString Item::getCategory()
{
    return  "";
}

/*Gets unit count*/
int Item::getCount()
{
    return count;
}

/*Gets description of unit*/
QString Item::getDescription()
{
    return description;
}

/*Gets name of unit*/
QString Item::getName()
{
    return name;
}

/*Gets thumbnail of unit*/
QImage Item::getThumbnail()
{
    return thumbnail;
}

/*Gets the turn count*/
int Item::getTurnCount()
{
    return turn_count;
}

/*Sets unit count*/
void Item::setCount(int count)
{
    turn_count = count;
}
