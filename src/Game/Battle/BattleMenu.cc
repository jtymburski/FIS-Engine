/******************************************************************************
* Class Name: BattleMenu
* Date Created: Sunday, October 28th, 2012
* Inheritance: Parent class: Battle <-- Game <-- Application
* Description: BattleMenu displays an interactive menu for the player to 
*        utilize commands while in a Battle.
*
*  TODO: CONSTRUCTORS TO BE FINISHED
*
*  NOTES:
*
* On construction of a BattleMenu, the person pointer is accessed to get all
* the possible categories (from action class) and that is assembled into a 
* local Qvector<QString> for categories.  This is stored in layer 2 of the
* battle menu. Once a category is selected, a method accesses all actions'
* from the person that match the category selected and assemble them into
* a new Qvector<Action *> actions. This is then rendered in layer 3 to be 
* ready for selection. 
******************************************************************************/

#include "Game/Battle/BattleMenu.h"

/*
 * Description: Constructs a BattleMenu object
 *
 * Inputs:
 */
BattleMenu::BattleMenu(QWidget* parent) : QWidget(parent)
{

}

/*
 * Description: Destroys a BattleMenu object
 */
BattleMenu::~BattleMenu()
{
\
}

/*=============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Paint event for the BattleMenu class
 *
 * Inputs: QPaintEVent*
 * Output: none
 */
void BattleMenu::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::selectedTarget(Party *)
{
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::selectedTarget(Person *)
{
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::getActive()
{
  return active;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setActive(bool new_active)
{
  active = new_active;
}
