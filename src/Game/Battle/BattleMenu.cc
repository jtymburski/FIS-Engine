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
  selected_action = EnumDb::NULL_ACTION;
}

/*
 * Description: Constructs a BattleMenu object with an active party.
 *
 * Inputs: bool debug - enables debug output mode of the BattleMenu
 *         QWidget* parent - the parent of the object
 */
BattleMenu::BattleMenu(Options running_config, QWidget* parent)
    : QWidget(parent)
{
  setConfiguration(running_config);
  selected_action = EnumDb::NULL_ACTION;
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
 void BattleMenu::calcValidItems()
 {
   valid_items = getValidItems();
 }

 /*
 * Description:
 *
 * Inputs:
 * Output:
 */
 void BattleMenu::calcValidSkills()
 {
   valid_skills = getValidSkills();
 }

 /*
 * Description:
 *
 * Inputs:
 * Output:
 */
 void BattleMenu::calcValidTargets()
 {
   valid_person_targets = getValidTargets();
 }

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
    if (selected_action == EnumDb::SKILL)
    {
      if (cursor_index == 0)
        setSelected(valid_skills.size() - 1);
      else
        setSelected(cursor_index - 1);
    }

    if (selected_action == EnumDb::ITEM)
    {
      if (cursor_index == 0)
        setSelected(valid_items.size() - 1);
      else
        setSelected(cursor_index - 1);
    }
  }
  else if (layer_index == 2)
  {
    if (cursor_index == 0)
      setSelected(valid_person_targets.size() - 1);
    else
      setSelected(cursor_index - 1 );

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
  if (layer_index == 0)
  {
    setLayerIndex(0);
    setSelected(0);
  }
  if (layer_index == 1)
  {
    setLayerIndex(0);
    setSelected(0);
  }
  else if (layer_index == 2)
  {
    setLayerIndex(1);
    setSelected(0);
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::highlight(int index)
{
  index;//WARNING
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
    if (selected_action == EnumDb::SKILL)
    {
      if ((cursor_index + 1) == valid_skills.size())
        setSelected(0);
      else
        setSelected(cursor_index + 1);
    }

    if (selected_action == EnumDb::ITEM)
    {
      if ((cursor_index + 1) == valid_items.size())
        setSelected(0);
      else
        setSelected(0);
    }
  }
  else if (layer_index == 2)
  {
    if ((cursor_index + 1) == valid_person_targets.size())
    {

    }

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
  else if (layer_index == 1)
  {
    setChosen();
    setLayerIndex(2);
    setSelected(0);
  }
  else if (layer_index == 2)
  {
    setLayerIndex(0);
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setChosen()
{
  if (layer_index == 0)
  {
    if (cursor_index == 0)
    {
      selected_action = EnumDb::SKILL;

      if (menu_status == DEBUG)
        qDebug() << "Skill Action Chosen";
    }
    if (cursor_index == 1)
    {
      selected_action = EnumDb::ITEM;

      if (menu_status == DEBUG)
        qDebug() << "Item Action Chosen";
    }
    if (cursor_index == 2)
    {
      selected_action = EnumDb::DEFEND;
      emit actionChosen(selected_action);

      if (menu_status == DEBUG)
        qDebug() << "Defend Action Chosen";
    }
    if (cursor_index == 3)
    {
      selected_action = EnumDb::RUN;
      emit actionChosen(selected_action);

      if (menu_status == DEBUG)
        qDebug() << "Run Action Chosen";
    }
  }
  else if (layer_index == 2)
  {
    //TODO: All party vs. single person selctions? [11-10-13]
  }
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
  index;//WARNING
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
  else
    menu_status = HIDING;
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
EnumDb::ActionScope  BattleMenu::getActionScope()
{
  if (selected_action == EnumDb::SKILL)
    return valid_skills.at(active_skill)->getSkillScope();
  if (selected_action == EnumDb::ITEM)
    return valid_items.at(active_skill)->getItemScope();

  return EnumDb::NO_SCOPE;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QList<Item*> BattleMenu::getValidItems()
{
  return active_party->getInventory()->getBattleItems();
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QList<Skill*> BattleMenu::getValidSkills()
{
  active_party->getMember(active_person)->calcSkills();
  SkillSet* usable_skills = active_party->getMember(active_person)->getSkills();
  return usable_skills->getSkills().toList();
}

/*
 * Description: Build list of persons based on scope -- see EnumDB for
 *              documentation
 * Inputs: none
 * Output: QList<Person*> builds list of valid targets
 */
QList<Person*> BattleMenu::getValidTargets()
{
  EnumDb::ActionScope action_scope = getActionScope();
  QList<Person*> targets;

  if (action_scope == EnumDb::ONE_TARGET ||
      action_scope == EnumDb::ALL_TARGETS)
  {
    targets += active_party->getLivingMembers();
    targets += inactive_party->getLivingMembers();
  }
  else if (action_scope == EnumDb::ONE_ENEMY ||
           action_scope == EnumDb::TWO_ENEMIES ||
           action_scope == EnumDb::ALL_ENEMIES)
  {
    targets += inactive_party->getLivingMembers();
  }
  else if (action_scope == EnumDb::ONE_ALLY ||
           action_scope == EnumDb::TWO_ALLIES ||
           action_scope == EnumDb::ALL_ALLIES)
  {
    targets += active_party->getLivingMembers();
  }
  else if (action_scope == EnumDb::ONE_ALLY_KO ||
           action_scope == EnumDb::ALL_ALLIES_KO)
  {
    targets += active_party->getDeadMembers();
  }
  else if (action_scope == EnumDb::NOT_USER ||
           action_scope == EnumDb::ALL_NOT_USER)
  {
    targets += active_party->getLivingMembers();
    targets.removeAt(active_person);
    targets += inactive_party->getLivingMembers();
  }

  return targets;
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
  switch (event->key())
  {
    case Qt::Key_Escape:
      break;

    default:
      break;
  }
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *=============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::printInfo()
{
  qDebug() << "Active Person Index: " << active_person;
  qDebug() << "Active Skill Index: " << active_skill;
  qDebug() << "Chosen Targets Size: " << chosen_targets.size();
  qDebug() << "Layer Index: " << layer_index;
  qDebug() << "Cursor Index: " << cursor_index;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Skill* BattleMenu::getChosenSkill()
{
  return getValidSkills().at(active_skill);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Item* BattleMenu::getChosenItem()
{
  return getValidItems().at(active_skill);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Person* BattleMenu::getChosenUser()
{
  return active_party->getMember(active_person);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
QList<Person*> BattleMenu::getChosenTargets()
{
  return chosen_targets;
}
