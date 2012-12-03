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

  Q_OBJECT // TODO: What is this for? [12-02-12]

/* Creates a BattleMenu object */
BattleMenu::BattleMenu(QWidget *pointer = 0)
{
}

/* Annihilates a BattleMenu object */
BattleMenu::~BattleMenu()
{
}

/* Selects the target PARTY for the action. Also adds the action to the action buffer. */
void BattleMenu::selectedTarget (Party *)
{
}

/* Selects the target PERSON for the action. Also adds the action to the action buffer. */
void BattleMenu::selectedTarget (Person *)
{
}

/* Returns TRUE if menu is active */
bool BattleMenu::getActive()
{
  return active;
}

/* Sets menu activity */
void BattleMenu::setActive(bool new_active)
{
  active = new_active;
}