/********************************************************************************
* Class Name: BattleMenu [Declaration]
* Date Created: Sunday, October 28th, 2012
* Rewritten: Saturday, November 9th, 2013
* Inheritance: QWidget
* Description: The BattleMenu class is the menu displayed when a user is choosing
*              a skill or action during Battle.
*
* Notes: Layer Zero - Primary Battle Menu Options:
*                     SKILL, ITEM, DEFEND, RUN
*
*        Layer One  - Choice of secondary actions.
*                     SKILL -> a list of skills available for the cur. player
*                     ITEM  -> a list of items available for the cur. player
*                     DEFEND -> sends DEFEND action to battle
*                     RUN   -> sends RUN action to battle
*
*        Layer Two  - If SKILL or ITEM action chosen for layer zero,
*                     layer two will show a list of targets for the skill or
*                     item the person wishes to use (if the item or skill are
*                     single hit). If the item or skill are multi-hit, the user
*                     will have the choice between the two parties.
********************************************************************************/
#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

#include "Options.h"
#include "Sound.h"
#include "Game/Battle/SkillBuffer.h"
#include "Game/Battle/ItemBuffer.h"
#include "Game/Player/Party.h"

class BattleMenu : public QWidget
{
  Q_OBJECT

public: 
  /* Constructs a default Battle Menu object */
  BattleMenu(QWidget* parent = 0);

  /* Constructs a normal Battle Menu object */
  BattleMenu(Options running_config, QWidget* parent = 0);

  /* Destroys a BattleMenu object */
  ~BattleMenu();

  /* Enumerated display mode of the BattleMenu */
  enum MenuStatus
  {
    OFF,      /* OFF - not showing */
    SHOWING,  /* SHOWING - rising from the depths */
    ON,       /* ON - currently fully displayed */
    HIDING,   /* HIDING - menu is hiding from view */
    DEBUG     /* DEBUG - outputting in text mode (not rendering) */
  };

private:
  /* Currently active and inactive parties for the Battle Menu Bar */
  Party* active_party;
  Party* inactive_party;

  /* Index of the active person in the party */
  ushort active_person;

  /* Index of the active skill of the active person */
  ushort active_skill;

  /* Vector of valid items the player has available */
  QList<Item*> valid_items;

  /* Vector of valid skills the player has available */
  QList<Skill*> valid_skills;

  /* Vector of targets the user may hit */
  QList<Person*> valid_person_targets;

  /* Vector of targets currently chosen */
  QList<Person*> chosen_targets;

  /* Current layer index */
  ushort layer_index;

  /* Current layer zero element index */
  ushort cursor_index;

  /* Current status of the menu (display mode) */
  MenuStatus menu_status;

  /* Enumeration of the selected action */
  EnumDb::ActionType selected_action;

  /* Configuration for display of the Menu */
  Options system_options;

  /* ------------------ Constants ------------------ */
  static const ushort kNUM_MENU_ITEMS;  /* # of items on layer zero of menu */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *=============================================================================*/
private:
  /* Calculates and assigns the valid items */
  void calcValidItems();

  /* Calculates and assigns the valid skills */
  void calcValidSkills();

  /* Calculates adn assigns the valid targets */
  void calcValidTargets();

  /* Decrements to the previous menu layer */
  void decrementLayer();

  /* Decrements the selected menu item (of the layer index) */
  void decrementSelected();

  /* Highlight the selected index (of the layer index) */
  bool highlight(int index);

  /* Increments the selected menu item (of the layer index) */
  void incrementSelected();

  /* Increments to the next layer */
  void incrementLayer();

  /* Chooses the selected menu item */
  void setChosen();

  /* Assigns the selected menu item - fails if out of range */
  bool setSelected(int index);

  /* Remove highlight on the selected index */
  bool unhighlight(int index);

  /* Returns the scope of the chosen action */
  EnumDb::ActionScope getActionScope();

  /* List of valid items from the active person available */
  QList<Item*> getValidItems();

  /* List of skills from the active person available */
  QList<Skill*> getValidSkills();

  /* List of targets from the inactive party available to the active person */
  QList<Person*> getValidTargets();

  /* Assigns the cursor index */
  void setCursorIndex(int new_index);

  /* Assigns the running configuration from the options cass */
  void setConfiguration(Options running_config);

  /* Assigns a new layer index */
  void setLayerIndex(int new_index);

  /* Assigns the menu status */
  void setMenuStatus(MenuStatus new_menu_status);

/*=============================================================================
 * PROTECTED FUNCTIONS
 *=============================================================================*/
protected:
  /* Paint event for the class */
  void paintEvent(QPaintEvent* event);

  /* Key press event */
  void keyPressEvent(QKeyEvent* event);

/*=============================================================================
 * SIGNALS
 *=============================================================================*/
signals:
  /* Emitted when a Skill is chosen */
  void actionChosen(EnumDb::ActionType action_type);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Method for printing out information on the current state of Menu */
  void printInfo();

  /* Returns a pointer to the skill currently chosen by a person */
  Skill* getChosenSkill();

  /* Returns a pointer to the item currently chosen by an item */
  Item* getChosenItem();

  /* Returns a pointer to the user of the current chosen action */
  Person* getChosenUser();

  /* Returns a vector containing the targets chosen for the current action */
  QList<Person*> getChosenTargets();

  /* Assigns the active party of the menu */
  bool setActiveParty(Party* new_active_party);

  /* Assigns the inactive party of the menu */
  bool setInactiveParty(Party* new_inactive_party);

};

#endif // BATTLEMENU_H
