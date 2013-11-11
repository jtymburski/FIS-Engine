/********************************************************************************
* Class Name: BattleMenu [Implementation]
* Date Created: Sunday, October 28th, 2012
* Rewritten: Saturday, November 13th, 2013
* Inheritance: QWidget
* Description: The BattleMenu class is the menu displayed when a user is choosing
*              a skill or action during Battle.
*
* See Header for TODOs
********************************************************************************/

#include "Game/Battle/BattleMenu.h"

/*=============================================================================
 * CONSTANTS
 *=============================================================================*/

const QString BattleMenu::kMENU_ITEMS[] = { "SKILL", /* Skills available */
                                            "ITEM",  /* Items available */
                                            "DEFEND",/* Defends [Pass turn] */
                                            "RUN"};  /* Attempts to flee */


const ushort BattleMenu::kNUM_MENU_ITEMS = 4; /* Number of layer zero elements */


/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *=============================================================================*/

/*
 * Description: Constructs a default BattleMenu object with no active party.
 *
 * Inputs: QWidget* parent - the parent of the object
 */
BattleMenu::BattleMenu(QWidget* parent)
    : QWidget(parent)
{

}

/*
 * Description: Constructs a BattleMenu object with an active party.
 *
 * Inputs: Party* active_party - party the menu is currently being displayed for
 *         Party* inactive_party - target party for the active party
 *         bool debug - enables debug output mode of the BattleMenu
 *         QWidget* parent - the parent of the object
 */
BattleMenu::BattleMenu(Options running_config, QWidget* parent)
    : QWidget(parent)
{
  setConfiguration(running_config);
}

/*
 * Description: Destroys a BattleMenu object
 */
BattleMenu::~BattleMenu()
{

}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *=============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::decrementSelected()
{
  if (layer_index == 0)
  {
    if (cursor_index == 0)
      setSelected(kNUM_MENU_ITEMS - 1);
    else
      setSelected(cursor_index - 1);
  }
  else if (layer_index == 1)
  {

  }
  else if (layer_index == 2)
  {

  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::decrementLayer()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::highlight(int index)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::incrementSelected()
{
  if (layer_index == 0)
  {
    if ((cursor_index + 1) == kNUM_MENU_ITEMS)
      setSelected(0);
    else
      setSelected(cursor_index + 1);
  }
  else if (layer_index == 1)
  {

  }
  else if (layer_index == 2)
  {

  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::incrementLayer()
{
  if (layer_index == 0)
  {
    setLayerIndex(1);
    setSelected(0);
  }
  else if (layer_index == 3)
    setLayerIndex(0);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setChosen()
{
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setSelected(int index)
{
  if (index >= 0)
  {
    if (layer_index == 0 && index < kNUM_MENU_ITEMS)
    {
      unhighlight(cursor_index);
      setCursorIndex(index);
      highlight(cursor_index);

      return true;
    }
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::unhighlight(int index)
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setActiveParty(Party* new_active_party)
{
  if (new_active_party != 0)
  {
    active_party = new_active_party;
    return true;
  }
  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setCursorIndex(int new_index)
{
  cursor_index = new_index;

  if (menu_status == DEBUG)
    qDebug() << "Set Cursor Index: " << cursor_index;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setInactiveParty(Party *new_inactive_party)
{
  if (new_inactive_party != 0)
  {
    inactive_party = new_inactive_party;
    return true;
  }
  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setConfiguration(Options running_config)
{
  system_options = running_config;
  setFixedSize(system_options.getScreenWidth(),
               system_options.getScreenHeight());

  if (running_config.getBattleMode() == Options::DEBUG)
    menu_status = DEBUG;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setLayerIndex(int new_layer_index)
{
  layer_index = new_layer_index;

  if (menu_status == DEBUG)
    qDebug() << "Set Layer Index: " << layer_index;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QList<Skill*> BattleMenu::getValidSkills()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QList<Person*> BattleMenu::getValidTargets()
{

}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setMenuStatus(MenuStatus new_menu_status)
{
  menu_status = new_menu_status;
}

/*=============================================================================
 * PROTECTED FUNCTIONS
 *=============================================================================*/

/*
 * Description: Paint event for the BattleMenu class
 *
 * Inputs: QPaintEVent*
 * Output: none
 */
void BattleMenu::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::keyPressEvent(QKeyEvent* event)
{

}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *=============================================================================*/

