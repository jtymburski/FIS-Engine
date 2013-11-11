/********************************************************************************
* Class Name: BattleMenu [Declaration]
* Date Created: Sunday, October 28th, 2012
* Rewritten: Saturday, November 9th, 2013
* Inheritance: QWidget
* Description: The BattleMenu class is the menu displayed when a user is choosing
*              a skill or action during Battle.
*
* //TODO: Everything [11-09-13]
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

  /* Index of the active item of the active party */
  ushort active_item;

  /* Index of the active person in the party */
  ushort active_person;

  /* Index of the active skill of the active person */
  ushort active_skill;

  /* Current layer index */
  ushort layer_index;

  /* Current layer zero element index */
  ushort cursor_index;

  /* Current status of the menu (display mode) */
  MenuStatus menu_status;

  /* Configuration for display of the Menu */
  Options system_options;

  /* ------------------ Constants ------------------ */
  static const QString kMENU_ITEMS[];  /* The stored menu items */
  static const ushort kNUM_MENU_ITEMS;  /* # of items on layer zero of menu */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *=============================================================================*/
private:
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
  bool setChosen();

  /* Assigns the selected menu item - fails if out of range */
  bool setSelected(int index);

  /* Remove highlight on the selected index */
  bool unhighlight(int index);

  /* List of skills from the active person available */
  QList<Skill*> getValidSkills();

  /* List of targets from the inactive party available to the active person */
  QList<Person*> getValidTargets();

  /* Assigns the active party of the menu */
  bool setActiveParty(Party* new_active_party);

  /* Assigns the cursor index */
  void setCursorIndex(int new_index);

  /* Assigns the inactive party of the menu */
  bool setInactiveParty(Party* new_inactive_party);

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
  void skillChosen(SkillUseAction skill_chosen);

  void itemChosen(ItemUseAction item_chosen);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:

};

#endif // BATTLEMENU_H
