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
  : flags(static_cast<BattleMenuState>(0))
{
  selection_verified = false;
  config             = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Decrease and increment the menu layers */
bool BattleMenu::decrementLayer(const int32_t &new_layer_index)
{
  auto success = false;

  if (new_layer_index <= 3)
  {
    valid_targets.clear();
    selected_targets.clear();

    element_index = 0;
    layer_index   = 3;

    setMenuFlag(BattleMenuState::TARGETS_ASSIGNED, false);
    setMenuFlag(BattleMenuState::SCOPE_ASSIGNED, false);

    success = true;
  }

  if (new_layer_index <= 2)
  {
    action_index  = -1;
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

bool BattleMenu::incrementLayer(const int32_t &new_layer_index)
{
  if (layer_index != new_layer_index)
    element_index = 0;

  if (new_layer_index == 2)
  {
    std::cout << "Incrementing to layer 2!" << std::endl;
    layer_index = 2;

    return true;
  }

  else if (new_layer_index == 3)
  {
    layer_index = 3;
    
    return true;
  }

  else if (new_layer_index == 4)
  {
    layer_index = 4;

    return true;
  }

  return false;
}
  
bool BattleMenu::setIndex(const std::string &subset)
{
  (void)subset;//TODO: WARNING
  return false;
}

/* Adding and removing target selections */
bool BattleMenu::addTarget(const int32_t &new_target)
{
  auto it = std::find(begin(valid_targets), end(valid_targets), new_target);

  if (it != end(valid_targets))
  {
    std::cout << "found target!" << std::endl;
    selected_targets.push_back(*it);
    valid_targets.erase(it);


    return true;
  }

  return false;
}

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

/* Methods for containing code for each key action */
void BattleMenu::keyDownAlpha(const char &c)
{
  std::cout << "Key down alpha: " << c << std::endl;

  auto index = menu_skills->getIndexOfAlpha(c);

  if (index != -1)
    element_index = index;
}

void BattleMenu::keyDownCancel()
{
  auto decrement_to_layer = -1;

  if (layer_index == 2)
  {
    decrement_to_layer = 1;
  }
  else if (layer_index == 3)
  {
    /* Hitting cancel on a GUARD action will reset the action_type */
    if (action_type == ActionType::GUARD)
    {
      decrement_to_layer = 1;
    }
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
    else if (action_type == ActionType::SKILL ||
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
          std::cout << "Remove last target was true" << std::endl;
          decrement_to_layer = 2;
        }
        else
          std::cout << "Removing last target: false" << std::endl;
      }
    }
  }
  else if (layer_index == 4)
  {
    decrement_to_layer = 3;
  }
  
  if (decrement_to_layer != -1)
    decrementLayer(decrement_to_layer);
}

void BattleMenu::keyDownDecrement()
{
  /* Move to the last index if on the first index */
  if (element_index == 0)
    element_index = getMaxIndex();
  else
    element_index--;
}

void BattleMenu::keyDownIncrement()
{
  /* Increment the viewing index if it is less than the max index */
  if (element_index < getMaxIndex())
    element_index++;
  /* Otherwise, set to the top index */
  else
    element_index = 0;
}

void BattleMenu::keyDownSelect()
{
  auto layer_to_increment = -1;

  if (layer_index == 1)
  {
    if (static_cast<uint32_t>(element_index) < valid_actions.size())
      action_type = valid_actions.at(element_index);

    if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
    {
      std::cout << "Attempting selection of current action: " 
                << Helpers::actionTypeToStr(action_type)
                << std::endl;
    }

    /* Assert there are skills if the chosen action type is SKILL */
    if (action_type == ActionType::SKILL)
    {
      if (menu_skills->getSize() > 0)
        layer_to_increment = 2;

      if (layer_to_increment == -1)
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
      if (menu_items.size() > 0)
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
    else if (action_type == ActionType::DEFEND  || 
             action_type == ActionType::IMPLODE || 
             action_type == ActionType::RUN     ||
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
        if (static_cast<uint32_t>(element_index) < menu_skills->getSize())
        {
          //TODO: Conditions of increment?
          layer_to_increment = 3;

#ifdef UDEBUG
          std::cout << "Selecting Skill element index: " << element_index 
                    << std::endl;
#endif

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
        if (static_cast<uint32_t>(element_index) < menu_items.size())
        {
          //TODO: Conditions of increment?
          layer_to_increment = 3;

#ifdef UDEBUG
          std::cout << "Selecting Skill element index: " << element_index 
                    << std::endl;
#endif       
          action_index = element_index;

          if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
          {
            std::cout << "Selecting item element: " << element_index 
                      << std::endl;
          }
        } 
      }
  }
  else if (layer_index == 3)
  {
    if (action_type == ActionType::SKILL)
    {
      if (action_scope == ActionScope::ALL_ENEMIES   ||
          action_scope == ActionScope::ALL_ALLIES    ||
          action_scope == ActionScope::ALL_ALLIES_KO ||
          action_scope == ActionScope::ALL_TARGETS   ||
          action_scope == ActionScope::ALL_NOT_USER)
      {
        while (valid_targets.size() > 0)
          addTarget(valid_targets.at(valid_targets.size() - 1));

        layer_to_increment = 4;
      }
      else if (action_scope == ActionScope::TWO_ENEMIES ||
          action_scope == ActionScope::TWO_ALLIES)
      {
        addTarget(valid_targets.at(element_index));

        if (selected_targets.size() == 2)
          layer_to_increment = 4;
      }
      else
      {
        addTarget(valid_targets.at(element_index));
        layer_to_increment = 4;
      }
    }
    else if (action_type == ActionType::ITEM)
    {

    }
    else if (action_type == ActionType::GUARD)
    {
      addTarget(valid_targets.at(element_index));
      action_targets     = selected_targets;
      layer_to_increment = 4;
    }
  }

  if (layer_to_increment != -1)
    incrementLayer(layer_to_increment);
}


/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool BattleMenu::isActionTypeSelected()
{
  return (action_type != ActionType::NONE);
}

bool BattleMenu::isActionSelected()
{
  return selection_verified;
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
  menu_skills = nullptr;
  valid_targets.clear();
  selected_targets.clear();

  selection_verified = false;
  action_type   = ActionType::NONE;
  action_index  = -1;
  action_scope  = ActionScope::NO_SCOPE;
  action_targets.clear();

  setMenuFlag(BattleMenuState::TARGETS_ASSIGNED, false);
  setMenuFlag(BattleMenuState::SCOPE_ASSIGNED, false);

  layer_index   = 1;
  element_index = 0;
  person_index = new_person_index;
  person_level = 0;
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
  auto index = 0;
  auto elements = menu_skills->getElements(person_level);
  std::cout << "Printing skills!" << std::endl;

  for (auto it = begin(elements); it != end(elements); ++it, ++index)
  {
    if (index == element_index)
      std::cout << "[X]";
    else
      std::cout << "[ ]";

    std::cout << " --- " << (*it).skill->getName() << std::endl;
  }
}

void BattleMenu::printTargets(const bool &print_selected)
{
  auto index = 0;

  if (print_selected)
  {
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
    if (action_scope == ActionScope::ALL_ALLIES || 
        action_scope == ActionScope::ALL_ENEMIES ||
        action_scope == ActionScope::ALL_ALLIES_KO || 
        action_scope == ActionScope::ALL_TARGETS ||
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
    std::cout << "Selecting action (scope: " 
              << Helpers::actionScopeToStr(action_scope)
              << ") for person index: " << person_index << std::endl;
    printMenuState();

    if (selection_verified)
      std::cout << "Action has been chosen from menu!" << std::endl;
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
  else if (layer_index == 3)
    printTargets(true);
  else if (layer_index == 4)
  {
    std::cout << "Action Type: " << Helpers::actionTypeToStr(action_type)
              << " with action index name: ";

    if (action_type == ActionType::SKILL)
      std::cout << menu_skills->getElement(action_index).skill->getName();
    else if (action_type == ActionType::ITEM)
      std::cout << menu_items.at(action_index).first->getName();
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
    return menu_skills->getSize() - 1;
  else if (layer_index == 2 && action_type == ActionType::ITEM)
    return menu_items.size() - 1;
  else if (layer_index == 2 && action_type == ActionType::GUARD)
  {
    //TODO: Determine maximum targets for Guarding
  }
  else if (layer_index == 2)
    return 0;
  else if (layer_index == 3)
    return valid_targets.size() - 1;

  return 0;
}

bool BattleMenu::getMenuFlag(const BattleMenuState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

SkillSet* BattleMenu::getMenuSkills()
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

void BattleMenu::setActionScope(const ActionScope &new_action_scope)
{
  action_scope = new_action_scope;
}

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
bool BattleMenu::setSelectableSkills(SkillSet* new_menu_skills)
{
  menu_skills = new_menu_skills;

  if (menu_skills != nullptr)
    menu_skills->sort(SkillSorts::NAME);

  return (new_menu_skills != nullptr);
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

void BattleMenu::setPersonLevel(const int32_t &new_person_level)
{
  person_level = new_person_level;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/