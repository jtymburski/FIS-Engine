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
 * Description: BattleMenu general constructor. Constructs a new BattleMenu
 *              object with default settings given a pointer to the current
 *              options configuration.
 *
 * Inputs:
 */
BattleMenu::BattleMenu(Options* running_config)
  : qtdr_cost_paid{0}
  , action_type{ActionType::NONE}
  , action_scope{ActionScope::NO_SCOPE}
  , config{running_config}
  , selected_skill{nullptr}
  , selected_item{nullptr}
  , flags{static_cast<BattleMenuState>(0)}
  , window_status{WindowStatus::OFF}
  , current_user{nullptr}
  , person_index{0}
  , layer_index{0}
  , element_index{-1}
{}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
/* Decrease and increment the menu layers */
bool BattleMenu::decrementLayer(const int32_t &new_layer_index)
{
  auto success = false;

  if (new_layer_index <= 3)
  {
    valid_targets.clear();
    selected_targets.clear();

    element_index = 0;
    //TODO: element index must be valid [08-03-14]

    layer_index   = 3;

    setMenuFlag(BattleMenuState::TARGETS_ASSIGNED, false);
    setMenuFlag(BattleMenuState::SCOPE_ASSIGNED, false);

    success = true;
  }

  if (new_layer_index <= 2)
  {
    if (qtdr_cost_paid != 0)
    {
      if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
      {
        std::cout << "Replacing " << qtdr_cost_paid << " QD paid for action." 
                  << std::endl;
      }
      
      current_user->getCurr().alterStat("QTDR", qtdr_cost_paid);
      qtdr_cost_paid = 0;
    }

    setMenuFlag(BattleMenuState::ACTION_SELECTED, false);

    action_scope  = ActionScope::NO_SCOPE;
    layer_index = 2;
          
    success = true;
  }

  if (new_layer_index <= 1)
  {
    action_type   = ActionType::NONE;
    layer_index   = 1;

    success = true;
  }

  return success;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::incrementLayer(const int32_t &new_layer_index)
{
  if (layer_index != new_layer_index)
    element_index = 0;

  if (new_layer_index == 2)
  {
    layer_index = 2;

    return true;
  }

  else if (new_layer_index == 3)
  {
    setMenuFlag(BattleMenuState::ACTION_SELECTED);
    layer_index = 3;
    
    return true;
  }

  else if (new_layer_index == 4)
  {
    setMenuFlag(BattleMenuState::ACTION_SELECTED);
    layer_index = 4;

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
/* Adding and removing target selections */
bool BattleMenu::addTarget(const int32_t &new_target)
{
  auto it = std::find(begin(valid_targets), end(valid_targets), new_target);

  if (it != end(valid_targets))
  {
    selected_targets.push_back(*it);
    valid_targets.erase(it);

    if (valid_targets.size() > 0)
      element_index = 0;
    else
      element_index = -1;

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
/* Adds all the targets of the same party given a party index */
bool BattleMenu::addPartyTargets(const int32_t &party_index)
{
  auto success = false;
  auto original = valid_targets;
  
  for (auto it = begin(original); it != end(original); ++it)
  {
    if ((*it < 0 && party_index < 0) || (*it > 0 && party_index > 0))
    {
      auto jt = std::find(begin(valid_targets), end(valid_targets), *it);
     
      if (jt != end(valid_targets))
        valid_targets.pop_back();

      selected_targets.push_back(*it);

      success = true;
    }
  }

  return success;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::removeLastTarget(const bool &clear_all)
{
  if (clear_all && selected_targets.size() == 0)
  {
    return true;
  }
  else if (clear_all)
  {
    valid_targets.clear();
    selected_targets.clear();

    /* Make sure Battle update will assign new targets */
    setMenuFlag(BattleMenuState::TARGETS_ASSIGNED, false);

    return false;
  }

  if (selected_targets.size() > 0)
  {
    valid_targets.push_back(*(end(selected_targets) - 1));
    selected_targets.pop_back();

    return false;
  }

  return true;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
/* Methods for containing code for each key action */
void BattleMenu::keyDownAlpha(const char &c)
{
  //auto index = menu_skills->getIndexOfAlpha(c);
  (void)c;
  auto index = -1; //TODO: Redo menu alpha skill selection.

  if (index != -1)
    element_index = index;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::keyDownCancel()
{
  auto decrement_to_layer = -1;

  if (layer_index == 2)
  {
    decrement_to_layer = 1;
  }
  else if (layer_index == 3)
  {
    /* Back on a SKILL or ITEM action will remove the last added target,
     * which depends on the scope of the action:
     *
     *    TWO_ENEMIES   - previous single target cleared
     *    TWO_ALLIES    - previous single target cleared
     *
     *    NO_SCOPE      - invalid
     *
     *    all others    - clear vector
     */
    if (action_type == ActionType::SKILL ||
        action_type == ActionType::ITEM)
    {
      if (action_scope == ActionScope::NO_SCOPE)
      {
        decrement_to_layer = 2;
      }
      else if (action_scope == ActionScope::TWO_ENEMIES ||
               action_scope == ActionScope::TWO_ALLIES)
      {
        /* If the size is greater than 0, pop the last selected target off,
           or go back to the skill selection menu */
        if (selected_targets.size() > 0)
          removeLastTarget();
        else
          decrement_to_layer = 2;
      }
      else
      {
        if (removeLastTarget(true))
        {
          decrement_to_layer = 2;
        }
      }
    }
    /* Hitting cancel on a GUARD action will reset the action_type */
    else if (action_type == ActionType::GUARD ||
             action_type == ActionType::DEFEND)
    {
      decrement_to_layer = 1;
    }
  }
  else if (layer_index == 4)
  {
    decrement_to_layer = 3;
  }
  
  if (decrement_to_layer != -1)
    decrementLayer(decrement_to_layer);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::keyDownDecrement()
{
  /* Move to the last index if on the first index */
  if (element_index == 0)
  {
    element_index = getMaxIndex();

    if (action_type != ActionType::NONE && valid_targets.empty())
    {
      for (; !indexHasTargets(); )
        element_index--;
    }
  }
  else
  {
    element_index--;

    if (action_type != ActionType::NONE && valid_targets.empty())
    {
      for (; !indexHasTargets(); )
        element_index--;  
    }
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::keyDownIncrement()
{
  /* Increment the viewing index if it is less than the max index */
  if (element_index < getMaxIndex())
  {
    element_index++;

    if (action_type != ActionType::NONE && valid_targets.empty())
    {
      for (; !indexHasTargets(); )
        element_index++;;
    }
  }
  /* Otherwise, set to the top index */
  else
  {
    element_index = 0;

    if (action_type != ActionType::NONE && valid_targets.empty())
    {
      for (; !indexHasTargets(); )
        element_index++;
    }
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::keyDownSelect()
{
  auto layer_to_increment = -1;

  if (layer_index == 1)
  {
    if (static_cast<uint32_t>(element_index) < valid_actions.size())
      action_type = valid_actions.at(element_index);

    if (config->getBattleMode() == BattleMode::TEXT)
    {
      std::cout << "Attempting selection of current action type: " 
                << Helpers::actionTypeToStr(action_type)
                << std::endl;
    }

    /* Assert there are skills if the chosen action type is SKILL */
    if (action_type == ActionType::SKILL)
    {
      if (menu_skills.size() > 0 && someIndexHasTargets())
        layer_to_increment = 2;

      if (layer_to_increment == -1)
      {
        action_type = ActionType::NONE;

        if (config->getBattleMode() == BattleMode::TEXT) 
        {
          std::cout << "No valid skills available, please select another action" 
                    << std::endl;            
        }         
      }
    }

    /* Else, assert there are items if the chosen action type is ITEM */
    else if (action_type == ActionType::ITEM)
    {
      if (menu_items.size() > 0 && someIndexHasTargets())
        layer_to_increment = 2;

      if (layer_to_increment == -1)
      {
        action_type = ActionType::NONE;

        if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
        {
          std::cout << "No items available please select another action!"
                      << std::endl;
        }
      }
    }

    /* If the selected action is GUARD, increment directly to target layer */
    else if (action_type == ActionType::GUARD)
    {
      layer_to_increment = 3;
    }
    
    /* DEFEND, IMPLODE, RUN, PASS actions require no other menus -> done */
    else if (action_type == ActionType::DEFEND)
    {
      layer_to_increment = 3;
    } 

    else if (action_type == ActionType::RUN ||
             action_type == ActionType::PASS)
    {
      layer_to_increment = 4;
    }
  }
  
  else if (layer_index == 2)
  {
    /* Selection of skill index -> move to target menu */
    if (action_type == ActionType::SKILL)
    {
      if (static_cast<uint32_t>(element_index) < menu_skills.size())
      {
        layer_to_increment = 3;

        /* Grab the selected skill */
        selected_skill = menu_skills.at(element_index).skill;

        /* Decrease the current user's QD by the cost required */
        auto true_cost = current_user->getTrueCost(selected_skill);

        if (true_cost <= current_user->getCurr().getStat("QTDR"))
        {
          if (config != nullptr && 
              config->getBattleMode() == BattleMode::TEXT)
          {
            std::cout << "Decreasing " << current_user->getName() << "s QTDR"
                      << " by " << true_cost << "." << std::endl;
          }

          qtdr_cost_paid = true_cost;
          current_user->getCurr().alterStat("QTDR", -true_cost);
        }
      }
    }
    else if (action_type == ActionType::ITEM)
    {
      if (static_cast<uint32_t>(element_index) < menu_items.size())
      {
        layer_to_increment = 3;

        selected_item = menu_items.at(element_index).item;
      } 
    }
  }
  else if (layer_index == 3)
  {
    if (action_type == ActionType::SKILL ||
        action_type == ActionType::ITEM)
    {
      if (action_scope == ActionScope::ONE_PARTY)
      {
        addPartyTargets(valid_targets.at(element_index));

        if (selected_targets.size() > 0)
          layer_to_increment = 4;
      }
      else if (action_scope == ActionScope::ALL_ENEMIES   ||
               action_scope == ActionScope::ALL_ALLIES    ||
               action_scope == ActionScope::ALL_ALLIES_KO ||
               action_scope == ActionScope::ALL_TARGETS   ||
               action_scope == ActionScope::ALL_NOT_USER)
      {
        while (valid_targets.size() > 0 && element_index != -1)
          addTarget(valid_targets.at(valid_targets.size() - 1));

        if (selected_targets.size() > 0)
          layer_to_increment = 4;
      }
      else if (action_scope == ActionScope::TWO_ENEMIES ||
          action_scope == ActionScope::TWO_ALLIES)
      {
        if (valid_targets.size() > 0 && element_index != -1)
          addTarget(valid_targets.at(element_index));

        if (selected_targets.size() == 2)
          layer_to_increment = 4;
      }
      else
      {
        if (valid_targets.size() > 0 && element_index != -1)
        {
          addTarget(valid_targets.at(element_index));
          layer_to_increment = 4;
        }
      } 
    }
    else if (action_type == ActionType::DEFEND ||
             action_type == ActionType::GUARD  ||
             action_type == ActionType::IMPLODE)
    {
      if (valid_targets.size() > 0 && element_index != -1)
      {
        addTarget(valid_targets.at(element_index));
        layer_to_increment = 4;
      }
    }
    else if (action_type == ActionType::RUN ||
             action_type == ActionType::PASS)
    {
      layer_to_increment = 4;
    }
  }
  else if (layer_index == 4)
  {
    setMenuFlag(BattleMenuState::SELECTION_VERIFIED, true);
  }

  if (layer_to_increment != -1)
    incrementLayer(layer_to_increment);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::unsetAll()
{
  valid_actions.clear();
  menu_items.clear();
  valid_targets.clear();
  selected_targets.clear();
  menu_skills.clear();

  selected_skill = nullptr;
  selected_item  = nullptr;
  current_user   = nullptr;

  action_type = ActionType::NONE;
  action_scope = ActionScope::NO_SCOPE;
  flags = static_cast<BattleMenuState>(0);

  qtdr_cost_paid = 0;
  person_index   = 0;
  layer_index    = 1;
  element_index  = 0;

  //TODO: Window/rendering status on unset?
  window_status = WindowStatus::ON;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::isActionTypeSelected()
{
  return (action_type != ActionType::NONE);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::someIndexHasTargets()
{
  auto has_targets     = false;
  auto old_index       = element_index;
  auto old_layer_index = layer_index;

  if (element_index != -1)
  {
    if ((action_type == ActionType::SKILL && !menu_skills.empty()) ||
        (action_type == ActionType::ITEM && !menu_items.empty()))
    {
      layer_index = 2;

      for (element_index = 0; element_index <= getMaxIndex(); element_index++)
        has_targets |= indexHasTargets();
    }
  }

  element_index = old_index;
  layer_index   = old_layer_index;

  return has_targets;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::indexHasTargets()
{
  auto has_targets = false;

  if (element_index != -1 && layer_index == 2)
  {
    if (action_type == ActionType::SKILL)
      has_targets |= !menu_skills.at(element_index).all_targets.empty();
    else if (action_type == ActionType::ITEM)
      has_targets |= !menu_items.at(element_index).all_targets.empty();
  }
  else if (element_index != -1 && layer_index == 3)
  {
    if (action_type == ActionType::SKILL)
    {
      if (action_scope == ActionScope::TWO_ALLIES)
        has_targets |= menu_skills.at(element_index).ally_targets.size() >= 2;
      else if (action_scope == ActionScope::TWO_ENEMIES)
        has_targets |= menu_skills.at(element_index).foe_targets.size() >= 2;
    }
    else if (action_type == ActionType::ITEM)
    {
      if (action_scope == ActionScope::TWO_ALLIES)
        has_targets |= menu_items.at(element_index).ally_targets.size() >= 2;
      else if (action_scope == ActionScope::TWO_ENEMIES)
        has_targets |= menu_items.at(element_index).foe_targets.size() >= 2; 
    }
  }

  return has_targets;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::reset(Person* const new_user, const uint32_t &new_person_index)
{
  unsetAll();

  current_user = new_user;
  person_index = new_person_index;

  if (current_user != nullptr)
  {
    //menu_skills   = current_user->getUseableSkills();
    valid_actions = current_user->getValidActions();
  }
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

    std::cout << " -- " << Helpers::actionTypeToStr(*it) << std::endl;
  }

  std::cout << std::endl;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::printSkills()
{
  if (current_user != nullptr)
  {
    auto index = 0;
    //auto elements = menu_skills->getElements(current_user->getLevel());

    for (auto it = begin(menu_skills); it != end(menu_skills); ++it, ++index)
    {
      if (index == element_index)
        std::cout << "[X]";
      else
        std::cout << "[ ]";
  
      std::cout << " -- [ " << (*it).skill->getCost() << " QD ] -- "
                << (*it).skill->getName() << " -- [";

      for (auto jt = begin((*it).all_targets); jt != end((*it).all_targets); ++jt)
      {
        if (jt == begin((*it).all_targets))
          std::cout << (*jt)->getName();
        else
          std::cout << ", " << (*jt)->getName();
      }

      std::cout << "]" << std::endl;

    }
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::printTargets(const bool &print_selected)
{
  auto index = 0;

  if (print_selected)
  {
    if (selected_targets.size() > 0)
      std::cout << "Selected Targets: " << std::endl;

    for (auto it = begin(selected_targets); it != end(selected_targets); ++it)
      std::cout << *it << " ";

    std::cout << std::endl;
  }

  std::cout << "Valid Targets Remaining: " << std::endl;
  for (auto it = begin(valid_targets); it != end(valid_targets); ++it, ++index)
  {
    /* If the index matches the element index or if the action scope is always
     * highlighting, display an 'X' on it
     * The following action scopes will always choose all selectable targets:
     * ALL_ENEMIES, ALL_ALLIES, ALL_ALLIES_KO, ALL_TARGETS, ALL_NOT_USER */
    if (action_scope == ActionScope::ALL_ALLIES    || 
        action_scope == ActionScope::ALL_ENEMIES   ||
        action_scope == ActionScope::ALL_ALLIES_KO || 
        action_scope == ActionScope::ALL_TARGETS   ||
        action_scope == ActionScope::ALL_NOT_USER  ||
        index == element_index)
    {
      std::cout << "[X]";
    }
    else
    {
      std::cout << "[ ]";
    }

    std::cout << " --- " << (*it) << std::endl;
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

    std::cout << " --- " << (*it).item->getName() << " -- [ " 
              << (*it).amount << " ] -- [";

    for (auto jt = begin((*it).all_targets); 
         jt != end((*it).all_targets); 
         ++ jt)
    {
      std::cout << (*jt)->getName() << ", ";
    }
    
    std::cout << "]" << std::endl;
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
  auto change_index = false;

  if (event.keysym.sym == SDLK_UP || event.keysym.sym == SDLK_DOWN)
  {
    change_index = true;

    if (action_type == ActionType::SKILL)
    {
      if (action_scope == ActionScope::ONE_PARTY)
      {
        change_index = false;

        if (element_index == 0)
          element_index = getMaxIndex();
        else if (element_index == getMaxIndex())
          element_index = 0;
      }
    }
  }

  if (change_index)
  {
    if (event.keysym.sym == SDLK_UP)
      keyDownDecrement();
    else if (event.keysym.sym == SDLK_DOWN)
      keyDownIncrement();
  }
  else if (event.keysym.sym == SDLK_RETURN)
    keyDownSelect();
  else if (event.keysym.sym == SDLK_BACKSPACE)
    keyDownCancel();
  else if (static_cast<int>(event.keysym.sym) >= 'a' &&
           static_cast<int>(event.keysym.sym) <= 'z')
  {
    keyDownAlpha(static_cast<int>(event.keysym.sym));
  }

  if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
  {
    if (current_user != nullptr)
    {
      std::cout << "Selecting action for person index: " << person_index 
                << " named: " << current_user->getName()  << std::endl;
    }

    printMenuState();

    if (getMenuFlag(BattleMenuState::SELECTION_VERIFIED))
      std::cout << "Selection has been verified!" << std::endl;
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::printMenuState()
{
  if (layer_index == 1)
    printValidActions();
  else if (layer_index == 2 && action_type == ActionType::SKILL)
    printSkills();
  else if (layer_index == 2 && action_type == ActionType::ITEM)
    printItems();
  else if (layer_index == 3)
    printTargets(true);
  else if (layer_index == 4)
  {
    std::cout << "Action Type: " << Helpers::actionTypeToStr(action_type)
              << " with name: ";

    if (action_type == ActionType::SKILL)
      std::cout << selected_skill->getName();
    else if (action_type == ActionType::ITEM)
      std::cout << selected_item->getName();
    else
      std::cout << "NONE";

    std::cout << std::endl << "Targeting person indexes: ";

    for (auto it = begin(selected_targets); it != end(selected_targets); ++it)
      std::cout << (*it) << ", ";

    std::cout << std::endl << "Select this action?" << std::endl;
  }
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
  return element_index;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::vector<int32_t> BattleMenu::getActionTargets()
{
  return selected_targets;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int32_t BattleMenu::getMaxIndex()
{
  if (layer_index == 1)
    return valid_actions.size() - 1;
  else if (layer_index == 2 && action_type == ActionType::SKILL)
    return menu_skills.size() - 1;
  else if (layer_index == 2 && action_type == ActionType::ITEM)
    return menu_items.size() - 1;
  else if (layer_index == 2)
    return 0;
  else if (layer_index == 3)
    return valid_targets.size() - 1;

  return 0;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::getMenuFlag(const BattleMenuState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::vector<BattleSkill> BattleMenu::getMenuSkills()
{
  return menu_skills;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Skill* BattleMenu::getSelectedSkill()
{
  return selected_skill;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
Item* BattleMenu::getSelectedItem()
{
  return selected_item;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::vector<BattleItem> BattleMenu::getMenuItems()
{
  return menu_items;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
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
void BattleMenu::setActionScope(const ActionScope &new_action_scope)
{
  action_scope = new_action_scope;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void BattleMenu::setMenuFlag(BattleMenuState flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setSelectableSkills(std::vector<BattleSkill> new_menu_skills)
{
  menu_skills = new_menu_skills;

  return !menu_skills.empty();
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setSelectableItems(std::vector<BattleItem> new_menu_items)
{
  menu_items = new_menu_items;

  return !menu_items.empty();
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool BattleMenu::setSelectableTargets(std::vector<int32_t> new_menu_targets)
{
  std::cout << "Assigning selectable targets size: " << new_menu_targets.size() << std::endl;
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