/********************************************************************************
* Class Name: MapMenu
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The MapMenu class, this represents the menu that is opened when
*              the player presses the ESC key.
********************************************************************************/
#ifndef MAPMENU_H
#define MAPMENU_H
#include <QtGui/QWidget>
#include "Party.h"

/*The menu items*/
enum MenuItems { SLEUTH, EQUIPMENT, ITEM, STATS, SAVE, OPTIONS, EXIT};

class MapMenu : public QWidget
{
public:
  /*Constructor function*/
  MapMenu();

  /*Destructor function*/
  ~MapMenu();

private:
  /*The current menu selected*/
  MenuItems current_menu;

  /*A pointer that gets access of your party to build the menu data*/
  Party* datapacket;

  /*Selections in submenus, handled in paint event*/
  unsigned int primary_selection, secondary_seleciton;

  /*Flag for if the menu is active*/
  bool ACTIVE;

  /*Flag for if the menu is in a submenu*/
  bool INSUBMENU;

protected:
  void paintEvent(QPaintEvent *);

public:

  /*Deselects a submenu, esc key calls this*/
  void deselectSubMenu();

  /*Initializes the menu with the given party data*/
  void initMenu (Party* p);

  /*Evaluates INSUBMENU flag*/
  bool isInSubMenu();

  /*Goes to the next menu based on current_menu*/
  void nextMenu();

  /*Goes to the previous menu based on current_menu*/
  void prevMenu();

  /*Selects a submenu, enter key calls this*/
  void selectSubMenu();

  /*Shifts the current sub menu down (down key)*/
  void shiftSubMenuDown();

  /*Shifts the current sub menu up (up key)*/
  void shiftSubMenuUp();

  /*Shifts the current sub menu left (left key)*/
  void shiftSubMenuLeft();

  /*Shifts the current sub menu right (right key)*/
  void shiftSubMenuRight();
};


#endif // MAPMENU_H
