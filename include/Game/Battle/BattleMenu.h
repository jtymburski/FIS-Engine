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
#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

#include "Game/Battle/BattleInfoBar.h"
#include "Game/Player/Action.h"
#include "Game/Player/Inventory.h"
#include "Game/Player/Party.h"
#include "Game/Player/Person.h"

class BattleMenu : public QWidget
{
  Q_OBJECT // TODO: What is this for? [12-02-12]

public: 
  BattleMenu(QWidget *pointer = 0);
  ~BattleMenu();

private:
  	/* Current action that is selected */
  Action* current_action;

  /* Menu active? */
  bool active;
  
  /* Pointer to Battle Info Bar (top bar describing battle events) */
  BattleInfoBar * bar;

  /* Inventory that the sleuth has access to */
  Inventory* inven;

  /* Allied party */
  Party* friends;
  
  /* Enemy party */
  Party* foes;

  /* Person currently selecting action */
  	Person* active_person;

  /* Actions currently available for active person (compiled from 
  equipment, bubbies, race, class etc.) */
  QVector<Action*> actions;

  /* Action buffer in battle */
  QVector<Action*>* action_buffer;

  /* The buffer of inventory items from battle */
  QVector<Item*>* inventory_buffer;

  signals:
  /* Emitted when the action is selected and verified as correct,
     connected to processAction() in battle */
  void actionSelected();

protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent*);

public:
  /* Selects the target PARTY for the action. Also adds the action to the action buffer. */
  	void selectedTarget (Party *);

  	/* Selects the target PERSON for the action. Also adds the action to the action buffer. */
  void selectedTarget (Person *); 

  /* Returns TRUE if menu is active */
  bool getActive();

  /* Sets menu activity */
  void setActive(bool active);
};

#endif // BATTLEMENU_H
