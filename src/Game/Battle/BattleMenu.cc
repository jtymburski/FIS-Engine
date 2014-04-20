/*******************************************************************************
* Class Name: Battle [Implementation]
* Date Created: February 23rd, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Battle/BattleMenu.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
BattleMenu::BattleMenu()
{
  config = nullptr;
}

/*
 * Description:
 */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::reset()
{
  menu_items.clear();
  menu_skills.clear();
  valid_targets.clear();
  selected_targets.clear();

  selection_complete = false;
  action_type   = ActionType::NONE;
  action_index  = -1;
  action_targets.clear();

  layer_index   = -1;
  element_index = -1;
}

bool BattleMenu::selectAction()
{
  auto text     = false;
  auto selected = false;

  if (config != nullptr)
    if (config->getBattleMode() == BattleMode::TEXT)
      text = true;

  while (!selected)
  {
    if (text)
    {
      std::cout << "Select User Action" << std::endl;      
    }

  }

  return true;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
ActionType BattleMenu::getActionType()
{
  return action_type;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t BattleMenu::getActionIndex()
{
  return action_index;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::vector<int32_t> BattleMenu::getActionTargets()
{
  return action_targets;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setSelectableItems(std::vector<std::pair<Item*, uint16_t>> new_menu_items)
{
  menu_items = new_menu_items;

  return (!menu_items.empty());
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setSelectableSkills(std::vector<Skill*> new_menu_skills)
{
  menu_skills = new_menu_skills;

  return (!menu_skills.empty());
}

bool BattleMenu::setConfiguration(Options* new_config)
{
  config = new_config;

  return (config != nullptr);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/