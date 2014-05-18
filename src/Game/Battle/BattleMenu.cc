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

bool BattleMenu::isActionTypeSelected()
{
  return (action_type != ActionType::NONE);
}

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

  layer_index   = 1;
  element_index = 0;
  person_index = new_person_index;
  window_status = WindowStatus::ON;

  if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
    printMenuState();
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
    std::cout << "Select action for index:" << person_index << std::endl;

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

    std::cout << " --- " << Helpers::actionTypeToStr(*it) << std::endl;
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
  std::cout << "Printing skills" << std::endl;
  std::cout << "Menu skills size: " << menu_skills.size() << std::endl;
  auto index = 0;

  for (auto it = begin(menu_skills); it != end(menu_skills); ++it, ++index)
  {
    if (index == element_index)
      std::cout << "[X]";
    else
      std::cout << "[ ]";

    std::cout << " --- " << (*it)->getName() << std::endl;
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

    std::cout << " --- " << (*it).first->getName() << std::endl;
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
    /* Move to the last index if on the first index */
    if (element_index == 0)
      element_index = getMaxIndex();
    else
      element_index--;
  }

  else if (event.keysym.sym == SDLK_DOWN)
  {
    /* Increment the viewing index if it is less than the max index */
    if (element_index < getMaxIndex())
      element_index++;
    /* Otherwise, set to the top index */
    else
      element_index = 0;
  }

  else if (event.keysym.sym == SDLK_a)
  {
    auto increment_layer = false;

    if (static_cast<uint32_t>(element_index) < valid_actions.size())
      action_type = valid_actions.at(element_index);

    if (layer_index == 1)
    {
      if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "Attempting selection of current action: " 
                  << Helpers::actionTypeToStr(action_type)
                  << std::endl;
      }

      /* Assert there are skills if the chosen action type is SKILL */
      if (action_type == ActionType::SKILL)
      {
        increment_layer |= (menu_skills.size() > 0);

        if (!increment_layer)
        {
          action_type = ActionType::NONE;

          if (config != nullptr && config->getBattleMode() == BattleMode::TEXT) 
          {
            std::cout << "No skills available please select another action" 
                      << std::endl;            
          }         
        }
      }

      /* Else, assert there are items if the chosen action type is ITEM */
      else if (action_type == ActionType::ITEM)
      {
        increment_layer |= (menu_items.size() > 0);

        if (!increment_layer)
        {
          action_type = ActionType::NONE;

          if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
          {
            std::cout << "No items available please select another action!"
                      << std::endl;
          }
        }
      }
      
      /* If the layer is set to increment from SKILL or ITEM */
      if ((action_type == ActionType::SKILL || 
           action_type == ActionType::ITEM)
                && increment_layer)
      {
        std::cout << "!!! incrementing layer !!!" << std::endl;
        action_index   = element_index;
        layer_index   = 2;
        element_index = 0;
      }

      /* If the selected action is GUARD, increment directly to target layer */
      else if (action_type == ActionType::GUARD)
      {
        action_index  = element_index;
        layer_index   = 3;
        element_index = 0;
      }

      /* DEFEND, IMPLODE, RUN, PASS actions require no other menus -> done */
      else if (action_type == ActionType::DEFEND  || 
               action_type == ActionType::IMPLODE || 
               action_type == ActionType::RUN     ||
               action_type == ActionType::PASS)
      {
        selection_complete = true;
      }
    }
    
    else if (layer_index == 1)
    {
      /* Selection of skill index -> move to target menu */
      if (action_type == ActionType::SKILL)
      {
        if (static_cast<uint32_t>(element_index) < menu_skills.size())
        {
          action_index = element_index;

          if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
          {
            std::cout << "Selecting skill element: " << element_index 
                      << std::endl;
          }
        }
      }
      else if (action_type == ActionType::ITEM)
      {
        if (static_cast<uint32_t>(element_index < menu_items.size()))
        {
          action_index = element_index;

          if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
          {
            std::cout << "Selecting item element: " << element_index 
                      << std::endl;
          }
        } 
      }

      /* Selection of item index -> move to target menu */
      else if (action_type == ActionType::ITEM)
      {
        if (element_index < menu_items.size())
          action_index = element_index;

      }
    }
    else if (layer_index == 2)
    {

    }
    else if (layer_index == 3)
    {
    
      if (action_type == ActionType::GUARD)
      {
        selected_targets.push_back(action_index);
        action_targets     = selected_targets;
        selection_complete = true;
      }
    }
  }
  else if (event.keysym.sym == SDLK_b)
  {
    if (layer_index == 1)
    {
      action_type = ActionType::NONE;
      layer_index = 0;
      element_index = 0;
    }
    else if (layer_index == 2)
    {

    }
    else if (layer_index == 3)
    {
      //TODO: Pop off the last added target selection depending on ActionScope
    }

  }

  if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
  {
    printMenuState();

    if (selection_complete)
      std::cout << "Selection complete!" << std::endl;
  }

  return false;
}

void BattleMenu::printMenuState()
{
  if (layer_index == 1)
    printValidActions();
  else if (layer_index == 2 && action_type == ActionType::SKILL)
    printSkills();
  else if (layer_index == 2 && action_type == ActionType::ITEM)
    printItems();
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
  if (layer_index == 1)
    return valid_actions.size() - 1;
  else if (layer_index == 2 && action_type == ActionType::SKILL)
    return menu_skills.size() - 1;
  else if (layer_index == 2 && action_type == ActionType::ITEM)
    return menu_items.size() - 1;
  else if (layer_index == 2 && action_type == ActionType::GUARD)
  {
    //TODO: Determine maximum targets for Guarding
  }
  else if (layer_index == 2)
    return 0;
  else if (layer_index == 3)
  {
    //TODO: How to determine maximum targets size? Depends on ActionScope
  }

  return 0;
}

std::vector<Skill*> BattleMenu::getMenuSkills()
{
  return menu_skills;
}

std::vector<std::pair<Item*, uint16_t>> BattleMenu::getMenuItems()
{
  return menu_items;
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
bool BattleMenu::setSelectableItems(std::vector<std::pair<Item*, uint16_t>> 
                                                                 new_menu_items)
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

bool BattleMenu::setSelectableTargets(std::vector<int32_t> new_menu_targets)
{
  valid_targets = new_menu_targets;

  return (!valid_targets.empty());
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