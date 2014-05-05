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
  selection_complete = false;
  config             = nullptr;
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

bool BattleMenu::isActionSelected()
{
  return selection_complete;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::reset(const uint32_t &new_person_index)
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
  person_index = new_person_index;
  window_status = WindowStatus::ON;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::selectAction()
{
  if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
    std::cout << "Select action for index:" << person_index << "\n";

  return true;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::printValidActions()
{
  auto index = 0;

  for (auto it = begin(valid_actions); it != end(valid_actions); ++it, ++index)
  {
    if (index == element_index)
      std::cout << "[X]";
    else
      std::cout << "[ ]";

    std::cout << " --- " << Helpers::actionTypeToStr(*it) << "\n";
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::printSkills()
{
  auto index = 0;

  for (auto it = begin(menu_skills); it != end(menu_skills); ++it, ++index)
  {
    if (index == element_index)
      std::cout << "[X]";
    else
      std::cout << "[ ]";

    std::cout << " --- " << (*it)->getName() << "\n";
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::printItems()
{
  auto index = 0;

  for (auto it = begin(menu_items); it != end(menu_items); ++it, ++index)
  {
    if (index == element_index)
      std::cout << "[X]";
    else
      std::cout << "[ ]";

    std::cout << " --- " << (*it).first->getName() << "\n";
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::keyDownEvent(SDL_KeyboardEvent event)
{
  if (event.keysym.sym == SDLK_UP)
  {
    if (element_index == 0)
      element_index = getMaxIndex();
  }
  // else if (event.keysym.sym == SDLK_DOWN)
  // {

  // }
  // else if (event.keysym.sym == SDLK_a)
  // {
  //   action_type = valid_actions.at(element_index);

  //   if (layer_index == 0)
  //   {
  //     if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
  //     {
  //       std::cout << "Selecting current action: " 
  //                 << Helpers::actionTypeToStr(action_type)
  //                 << "\n";
  //     }

  //     layer_index = 1;

  //     if (action_type == ActionType::DEFEND  || 
  //         action_type == ActionType::IMPLODE || 
  //         action_type == ActionType::PASS)
  //     {
  //       selection_complete = true;
  //     }
  //   }
  //   else if (layer_index == 1)
  //   {
  //     if (action_type == ActionType::SKILL || action_type == ActionType::ITEM)
  //     {
  //       action_index = element_index;
  //       layer_index = 2;
  //       element_index = 0;
  //     }
  //     else if (action_type == ActionType::GUARD)
  //       action_targets.push_back(action_index);
  //   }
  //   else if (layer_index == 3)
  //   {
  //     action_targets = selected_targets;
  //     selection_complete = true;
  //   }

  // }
  // else if (event.keysym.sym == SDLK_b)
  // {
  //   if (layer_index == 1)
  //   {
  //     action_type = ActionType::NONE;
  //     layer_index = 0;
  //     element_index = 0;
  //   }
  //   else if (layer_index == 2)
  //   {

  //   }
  //   else if (layer_index == 3)
  //   {
  //     //TODO: Pop off the last added target selection depending on ActionScope
  //   }

  // }

  if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
  {
    if (layer_index == 0)
      printValidActions();
    else if (layer_index == 1 && action_type == ActionType::SKILL)
      printSkills();
    else if (layer_index == 1 && action_type == ActionType::ITEM)
      printItems();

    if (selection_complete)
      std::cout << "Selection complete! \n";
  }
  else if (config == nullptr)
    std::cout << "Config if nullptr" << std::endl;

  return false;
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

int32_t BattleMenu::getMaxIndex()
{
  if (layer_index == 0)
    return valid_actions.size();
  else if (layer_index == 1 && action_type == ActionType::SKILL)
    return menu_skills.size();
  else if (layer_index == 1 && action_type == ActionType::ITEM)
    return menu_items.size();
  else if (layer_index == 1 && action_type == ActionType::GUARD)
  {
    //TODO: Determine maximum targets for Guarding
  }
  else if (layer_index == 1)
    return 0;
  else if (layer_index == 2)
  {
    //TODO: How to determine maximum targets size? Depends on ActionScope
  }

  return 0;
}

WindowStatus BattleMenu::getWindowStatus()
{
  return window_status;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setSelectableActions(std::vector<ActionType> new_valid_actions)
{
  valid_actions = new_valid_actions;

  return (!valid_actions.empty());
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

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setConfiguration(Options* new_config)
{
  config = new_config;

  return (config != nullptr);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setPersonIndex(const int32_t &new_person_index)
{
  person_index = new_person_index; 
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/