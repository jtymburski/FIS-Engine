/******************************************************************************
* Class Name: MapMenu
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The MapMenu class, this represents the menu that is opened when
*              the player presses the ESC key.
******************************************************************************/
#include "Game/Map/MapMenu.h"

/* Constructor function */
MapMenu::MapMenu()
{
}

/* Destructor function */
MapMenu::~MapMenu()
{
}

/* Painting function */
void MapMenu::paintEvent(QPaintEvent *event)
{
}

/* Deselects a submenu, esc key calls this */
void MapMenu::deselectSubMenu()
{
}

/* Initializes the menu with the given party data */
void MapMenu::initMenu (Party* party)
{
}

/* Evaluates INSUBMENU flag */
bool MapMenu::isInSubMenu()
{
    return INSUBMENU;
}

/* Goes to the next menu based on current_menu */
void MapMenu::nextMenu()
{
}

/* Goes to the previous menu based on current_menu */
void MapMenu::prevMenu()
{
}

/* Selects a submenu, enter key calls this */
void MapMenu::selectSubMenu()
{
}

/* Shifts the current sub menu down (down key) */
void MapMenu::shiftSubMenuDown()
{
}

/* Shifts the current sub menu up (up key) */
void MapMenu::shiftSubMenuUp()
{
}

/* Shifts the current sub menu left (left key) */
void MapMenu::shiftSubMenuLeft()
{
}

/* Shifts the current sub menu right (right key) */
void MapMenu::shiftSubMenuRight()
{
}
