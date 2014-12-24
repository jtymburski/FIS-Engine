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
 * Inputs: Options* running_config - pointer to a running config assignment.
 */
BattleMenu::BattleMenu(Options* running_config)
    : qtdr_cost_paid{0}
    , action_type{ActionType::NONE}
    , action_scope{ActionScope::NO_SCOPE}
    , config{running_config}
    , selected_item{nullptr}
    , flags{static_cast<MenuState>(0)}
    , window_status{WindowStatus::OFF}
    , current_user{nullptr}
    , num_allies{0}
    , person_index{0}
    , layer_index{0}
    , element_index{-1}
{}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Decrement a menu layer to a given layer index. Performs all
 *              necessary updating and reworking to do this.
 *
 * Inputs: int32_t new_layer_index - index of new layer to go to.
 * Output: bool - true if the decrement was successful.
 */
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

    setMenuFlag(MenuState::TARGETS_ASSIGNED, false);
    setMenuFlag(MenuState::SCOPE_ASSIGNED, false);

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

    setMenuFlag(MenuState::ACTION_SELECTED, false);

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
 * Description: Increments the BattleMenu to a given layer index. Performs
 *              all necessary updating to make this happen,
 *
 * Inputs: int32_t - new layer index for the Battle menu
 * Output: bool - true if the incrementing occurs correctly
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
    setMenuFlag(MenuState::ACTION_SELECTED);
    layer_index = 3;
    
    return true;
  }

  else if (new_layer_index == 4)
  {
    setMenuFlag(MenuState::ACTION_SELECTED);
    layer_index = 4;

    return true;
  }

  return false;
}

/*
 * Description: Adds a new target by inded value to the array of selected 
 *              targets and removes this new target from the vector of remaining
 *              valid targets.
 *
 * Inputs: int32_t - index [can be +/- depending on party] of target to add
 * Output: bool - true if the target was added succesfully.
 */
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
 * Description: Add all the targets of the same party of a given Index (based
 *              on whether the given index is positive or negative) to the
 *              vector of selected targets and remove them all from valid 
 *              targets (although the selection should be complete)
 *
 * Inputs: int32_t party_index - given index of member to be added to targets
 * Output: bool - true if all party targets were added.
 */
bool BattleMenu::addPartyTargets(const int32_t &party_index)
{
  selected_targets = getPartyTargets(party_index);

  return true;
}

/*
 * Description:
 *
 * Inputs:
 * Output: 
 */
std::vector<int32_t> BattleMenu::getPartyTargets(int32_t party_index)
{
  std::vector<int32_t> highlight_targets;

  for (auto it = begin(valid_targets); it != end(valid_targets); ++it)
    if ((*it < 0 && party_index < 0) || (*it > 0 && party_index > 0))
      highlight_targets.push_back(*it);

  return highlight_targets;
}

/*
 * Description: Attempts to remove the last added target.
 *
 * Inputs: bool clear_all - whether to clear all the target assignments.
 * Output: bool - true if ???
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
    setMenuFlag(MenuState::TARGETS_ASSIGNED, false);

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
 * Description: Jump to element index based on alha numeric keys.
 *
 * Inputs:
 * Output:
 */
/* Methods for containing code for each key action */
void BattleMenu::keyDownAlpha(const char &c)
{
  //auto index = menu_skills->getIndexOfAlpha(c);
  (void)c;
  auto index = -1; //TODO: [08-24-14] Redo menu alpha skill selection.

  if (index != -1)
    element_index = index;
}

/*
 * Description: Method for holding what happens when the player hits the
 *              cancel Key.
 *
 * Inputs: none
 * Output: none
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
             action_type == ActionType::DEFEND ||
             action_type == ActionType::PASS)
    {
      decrement_to_layer = 1;
    }
  }
  else if (layer_index == 4)
  {
    if (action_type == ActionType::DEFEND || action_type == ActionType::RUN ||
        action_type == ActionType::PASS)
      decrement_to_layer = 1;
    else 
      decrement_to_layer = 3;
  }
  
  if (decrement_to_layer != -1)
    decrementLayer(decrement_to_layer);
}

/*
 * Description: Method for what happens when the player hits the decrement key.
 *
 * Inputs: none
 * Output: none
 */
void BattleMenu::keyDownDecrement()
{
  /* Move to the last index if on the first index */
  if (element_index == 0)
  {
    if(getLayerIndex() > 2)
    {
      element_index = getMaxIndex();

      if (action_type != ActionType::NONE && valid_targets.empty())
        for (; !indexHasTargets() && element_index > 0; )
          element_index--;
    }
  }
  else
  {
    element_index--;

    if (action_type != ActionType::NONE && valid_targets.empty())
    {
      for (; !indexHasTargets() && element_index > 0; )
        element_index--;  
    }
  }
}

/*
 * Description: Method for what happens when the player hits the increment key.
 *
 * Inputs: none
 * Output: none
 */
void BattleMenu::keyDownIncrement()
{
  /* Increment the viewing index if it is less than the max index */
  if (element_index < getMaxIndex())
  {
    element_index++;

    if (action_type != ActionType::NONE && valid_targets.empty())
    {
      for (; !indexHasTargets() && element_index < getMaxIndex(); )
        element_index++;

      /* If there are no valid targets at the end of the list, search
       * through again starting at the top of the vector until one has
       * been found. This next step ASSUMES there exists at least one
       * valid skill (which should have been checked already).
       */
      if (element_index == getMaxIndex() && !indexHasTargets())
      {
        element_index = 0;

        for (; !indexHasTargets() && element_index < getMaxIndex();)
          element_index++;
      }
    }
  }
  /* Otherwise, set to the top index */
  else if(getLayerIndex() > 2)
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
 * Description: Method for what happens when the player hits the select key.
 *
 * Inputs: none
 * Output: none
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
      if (num_allies > 1)
        layer_to_increment = 3;
      else
      {
        action_type = ActionType::NONE;

        if (config->getBattleMode() == BattleMode::TEXT)
          std::cout << "No allies to guard!" << std::endl;
      }
    }
    
    /* DEFEND, RUN, PASS actions require no other menus -> done */
    else if (action_type == ActionType::DEFEND ||
             action_type == ActionType::RUN ||
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
        if (indexHasTargets())
        {
          /* Grab the selected skill */
          selected_skill = menu_skills.at(element_index);

          /* Decrease the current user's QD by the cost required */
          auto true_cost = current_user->getTrueCost(selected_skill.skill);

          std::cout << "True Cost: " << true_cost << std::endl;

          if (true_cost <= current_user->getCurr().getStat("QTDR"))
          {
            if (config != nullptr && 
                config->getBattleMode() == BattleMode::TEXT)
            {
              std::cout << "Decreasing " << current_user->getName() << "s QTDR"
                        << " by " << true_cost << "." << std::endl;
            }
 
            layer_to_increment = 3;
            qtdr_cost_paid = true_cost;
            current_user->getCurr().alterStat(Attribute::QTDR, -true_cost);
          }
          else
          {
            if (config->getBattleMode() == BattleMode::TEXT)
              std::cout << "Cannot afford that skill!" << std::endl;
          }
        }
        else if (config->getBattleMode() == BattleMode::TEXT)
        {
          std::cout << "Selected skill index has invalid targets" << std::endl;
        }
        else if (config->getBattleMode() == BattleMode::GUI)
        {
          //TODO: Error selection of invalid index. 
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
    setMenuFlag(MenuState::SELECTION_VERIFIED, true);
  }

  if (layer_to_increment != -1)
    incrementLayer(layer_to_increment);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Unset all the battle menu parameters to default, thereby getting
 *              it ready for a new person/new turn.
 *
 * Inputs: none
 * Output: none
 */
void BattleMenu::unsetAll(const bool &window_off)
{
  valid_actions.clear();
  menu_items.clear();
  valid_targets.clear();
  selected_targets.clear();
  menu_skills.clear();

  selected_item  = nullptr;
  current_user   = nullptr;

  action_type = ActionType::NONE;
  action_scope = ActionScope::NO_SCOPE;
  flags = static_cast<MenuState>(0);

  qtdr_cost_paid = 0;
  person_index   = 0;
  layer_index    = 1;
  element_index  = 0;

  //TODO: Window/rendering status on unset?
  if (window_off)
    window_status = WindowStatus::OFF;
  else
    window_status = WindowStatus::ON;
}

/*
 * Description: Determines whether an ActionType has been chosen for current
 *              menu selection occurence.
 *
 * Inputs: none
 * Output: bool - true if an action type has been selected for das Menu.
 */
bool BattleMenu::isActionTypeSelected()
{
  return (action_type != ActionType::NONE);
}

/*
 * Description: Determines whether there is some index of the current element
 *              which has a valid target selection possible.
 *
 * Inputs: none
 * Output: bool - true if there exists valid target selections for curr. elm.
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
 * Description: Determines whether the current selected index (of a Skill or
 *              Item selection) has valid targets to be chosen. If not,
 *              the selection should not be able to be be chosen.
 *
 * Inputs: checking_all - if this function is called from someIndexHasTargets
 * Output: bool - true if the index is valid
 */
bool BattleMenu::indexHasTargets()
{
  auto has_targets = false;

  if (element_index != -1 && layer_index == 2)
  {
    if (action_type == ActionType::SKILL)
    {
      action_scope = menu_skills.at(element_index).skill->getScope();

      if (action_scope == ActionScope::TWO_ALLIES)
        has_targets |= menu_skills.at(element_index).ally_targets.size() >= 2;
      else if (action_scope == ActionScope::TWO_ENEMIES)
        has_targets |= menu_skills.at(element_index).foe_targets.size() >= 2;
      else
        has_targets |= menu_skills.at(element_index).all_targets.size() > 0;
    }
    else if (action_type == ActionType::ITEM)
    {
      action_scope = menu_items.at(element_index).item_skill->getScope();

      if (action_scope == ActionScope::TWO_ALLIES)
        has_targets |= menu_items.at(element_index).ally_targets.size() >= 2;
      else if (action_scope == ActionScope::TWO_ENEMIES)
        has_targets |= menu_items.at(element_index).foe_targets.size() >= 2;
      else
        has_targets |= menu_items.at(element_index).all_targets.size() > 0;
    }
  }

  return has_targets;
}

/*
 * Description: Resets the BattleMenu for a new user for a new turn, given
 *              a pointer to the user and the index they correspond to.
 *
 * Inputs: Person* new_user - pointer to the new person
 *         uint32_t new_person_index - corresponding index of the person
 * Output: none
 */
void BattleMenu::reset(Person* const new_user, const uint32_t &new_person_index)
{
  unsetAll();

  num_allies = 0;
  current_user = new_user;
  person_index = new_person_index;

  if (current_user != nullptr)
  {
    valid_actions = current_user->getValidActions();
  }
}

/*
 * Description: Selects a random action among the list of available menu.
 *              This function is used in lieu of the menu for ailments
 *              such as 
 *
 * Inputs: none
 * Output: none
 */
void BattleMenu::selectRandomAction()
{
  if (menu_skills.size() > 0)
  {
    auto rand_elm = Helpers::randU(0, menu_skills.size() - 1);
    selected_skill = menu_skills.at(rand_elm);

    action_type = ActionType::SKILL;
    setMenuFlag(MenuState::ACTION_SELECTED, true);
  }
}

/*
 * Description: Prints out the valid actions types for the non-GUI battle.
 *
 * Inputs: none
 * Output: none
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
 * Description: Prints out the list of available skills for the current menu 
 *              setup.
 *
 * Inputs: none
 * Output: none
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

      for (auto jt = begin((*it).all_targets); 
           jt != end((*it).all_targets); 
           ++jt)
      {
        if (jt == begin((*it).all_targets))
          std::cout << (*jt)->getName();
        else
          std::cout << ", " << (*jt)->getName();
      }

      std::cout << "] - [" << Helpers::actionScopeToStr((*it).skill->getScope()) 
                << "]"<< std::endl;

    }
  }
}

/*
 * Description: Prints out the target selection for current selectable targets.
 *
 * Inputs: none
 * Output: bool - whether to print selected targets or not.
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

bool BattleMenu::isValidIndex(int32_t check_index)
{
  auto it = std::find(begin(valid_targets), end(valid_targets), check_index);
  return it != end(valid_targets);
}

/*
 * Description: 
 *
 * Inputs: none
 * Output: none
 */
void BattleMenu::mostLeftIndex()
{
  if (person_index < 0)
  {
    if (isValidIndex(-5))
      person_index = -5;
    if (isValidIndex(-4))
      person_index = -4;
    if (isValidIndex(-3))
      person_index = -3;
    if (isValidIndex(-1))
      person_index = -1;
    if (isValidIndex(-2))
      person_index = -2;
  }
  else
  {
    if (isValidIndex(2))
     person_index = 2;
    if (isValidIndex(1))
      person_index = 1;
    if (isValidIndex(3))
      person_index = 3;
    if (isValidIndex(4))
      person_index = 4;
    if (isValidIndex(5))
      person_index = 5;
  }

  person_index = 0;
}

/*
 * Description: 
 *
 * Inputs: none
 * Output: none
 */
void BattleMenu::mostRightIndex()
{
  if (person_index < 0)
  {
    if (isValidIndex(-2))
      person_index = -2;
    else if (isValidIndex(-1))
      person_index = -1;
    else if (isValidIndex(-3))
      person_index = -3;
    else if (isValidIndex(-4))
      person_index = -4;
    else if (isValidIndex(-5))
      person_index = -5;
  }
  else
  {
    if (isValidIndex(5))
      person_index = 5;
    else if (isValidIndex(4))
      person_index = 4;
    else if (isValidIndex(3))
      person_index = 3;
    else if (isValidIndex(1))
      person_index = 1;
    else if (isValidIndex(2))
      person_index = 2;
  }
}

/*
 * Description: 
 *
 * Inputs: none
 * Output: none
 */
void BattleMenu::swapPartyIndex()
{
  auto temp_index = person_index;

  if (person_index < 0)
  {
    person_index = 1;
    mostLeftIndex();
  }
  else if (person_index > 0)
  {
    person_index = -1;
    mostRightIndex();
  }

  if (person_index == 0)
    person_index = temp_index;

  element_index = person_index;
}

/*
 * Description: Prints out the available items for the BattleMenu.
 *
 * Inputs: none
 * Output: none
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
 * Description: Method for processing keydown events.
 *
 * Inputs: SDL_KeyboardEvent event - the key down event.
 * Output: bool - true if ???
 */
bool BattleMenu::keyDownEvent(SDL_KeyboardEvent event)
{
  auto change_index = false;

  if (event.keysym.sym == SDLK_UP || event.keysym.sym == SDLK_DOWN ||
      event.keysym.sym == SDLK_LEFT || event.keysym.sym == SDLK_RIGHT)
  {
    change_index = true;

    if (action_type == ActionType::SKILL)
    {
      if (layer_index == 3 && action_scope == ActionScope::ONE_PARTY)
      {
        change_index = false;

        if (element_index == 0)
          element_index = getMaxIndex();
        else if (element_index == getMaxIndex())
          element_index = 0;
      }
    }

    if (layer_index == 4)
      change_index = false;
  }
  
  if (change_index)
  {
    if (event.keysym.sym == SDLK_UP || event.keysym.sym == SDLK_RIGHT)
    {
      if (layer_index == 3)
        keyDownIncrement();
      else
        keyDownDecrement();
    }
    else if (event.keysym.sym == SDLK_DOWN || event.keysym.sym == SDLK_LEFT)
    {
      if (layer_index == 3)
        keyDownDecrement();
      else
        keyDownIncrement();
    }
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
  else
  {
    std::cout << "CANNOT CHANGE INDEX" << std::endl;
  }

  if (config != nullptr && config->getBattleMode() == BattleMode::TEXT)
  {
    if (current_user != nullptr)
    {
      std::cout << "Selecting action for person index: " << person_index 
                << " named: " << current_user->getName()  << std::endl;
    }

    printMenuState();

    if (getMenuFlag(MenuState::SELECTION_VERIFIED))
      std::cout << "Selection has been verified!\n" << std::endl;
  }

  return false;
}

/*
 * Description: Prints out the state of the menu for a non-gui battle based 
 *              on the current layering index.
 *
 * Inputs: none
 * Output: none
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
      std::cout << selected_skill.skill->getName();
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
 * Description: Returns enumerated action type for the selected action. This
 *              will be ActionType::NONE if no action type is currently
 *              selected.
 *
 * Inputs:
 * Output:
 */
ActionType BattleMenu::getActionType()
{
  return action_type;
}

/*
 * Description: Returns the selected index of current action (ex. the index
 *              along the vector of items/skills which is selected).
 *
 * Inputs: none
 * Output: int32_t - the index of the selected acion
 */
int32_t BattleMenu::getActionIndex()
{
  return element_index;
}

/*
 * Description: Returns the list of action target indexes which were selected
 *              by the user of the action.
 *
 * Inputs: none
 * Output: std::vector<int32_t> - vector of indexes of selected targets
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
Person* BattleMenu::getCurrentUser()
{
  return current_user;
}

/*
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
int32_t BattleMenu::getElementIndex()
{
  return element_index;
}
  
/* Get targets hovered over during the selection process */
// TODO: Comment
std::vector<int32_t> BattleMenu::getHoverTargets()
{
  std::vector<int32_t> hover_targets;

  for(uint16_t i = 0; i < valid_targets.size(); i++)
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
        (i == element_index && action_scope != ActionScope::ONE_PARTY))
    {
      hover_targets.push_back(valid_targets[i]);
    }
  }

  if (action_scope == ActionScope::ONE_PARTY)
  {
    hover_targets = getPartyTargets(valid_targets.at(element_index));
  }

  return hover_targets;
}

/*
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
int32_t BattleMenu::getLayerIndex()
{
  return layer_index;
}

/*
 * Description: Returns the current value for maximum selection index based
 *              on the layering of hte menu.
 *
 * Inputs: none
 * Output: int32_t - the maximum selectable index
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
 * Description: Returns the value of a given MenuState flag
 *
 * Inputs: test_flag - enumerated flag to test the value for
 * Output: bool - the boolean value of the flag
 */
bool BattleMenu::getMenuFlag(const MenuState &test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Returns a vector of BattleSkill objects.
 *
 * Inputs: none
 * Output: std::vector<BattleSkill> - the vector of BattleSkill objects.
 */
std::vector<BattleSkill> BattleMenu::getMenuSkills()
{
  return menu_skills;
}

/*
 * Description: Returns the pointer to the currently selected Skill object.
 *
 * Inputs: none
 * Output: Skill* - pointer to the currently selected Skill object.
 */
BattleSkill BattleMenu::getSelectedSkill()
{
  return selected_skill;
}

/*
 * Description: Returns the pointer to the currently selected item objet.
 *
 * Inputs: none
 * Output: Item* - pointer to the Item object selected.
 */
Item* BattleMenu::getSelectedItem()
{
  return selected_item;
}

/*
 * Description: Returns the vector of BattleMenu objects.
 *
 * Inputs: none
 * Output: std::vector<BattleItem> - the vector of Battle item objects.
 */
std::vector<BattleItem> BattleMenu::getMenuItems()
{
  return menu_items;
}

/*
 * Description: Returns the vector of BattleMenu objects.
 *
 * Inputs: none
 * Output: std::vector<BattleItem> - the vector of Battle item objects.
 */
int32_t BattleMenu::getPersonIndex()
{
  return person_index;
}

/*
 * Description: Compiles randomized targets for the selected skill. (ex., for
 *              attacking while confused)
 *
 * Inputs: none
 * Output: std::vector<int32_t> - vector of targets
 */
std::vector<int32_t> BattleMenu::getRandomTargets()
{
  std::vector<int32_t> random_targets;

  if (action_scope == ActionScope::USER ||
      action_scope == ActionScope::ALL_ENEMIES ||
      action_scope == ActionScope::ALL_ALLIES ||
      action_scope == ActionScope::ALL_ALLIES_KO ||
      action_scope == ActionScope::ALL_TARGETS ||
      action_scope == ActionScope::ALL_NOT_USER)
  {
    random_targets = valid_targets;
  }
  else if (action_scope == ActionScope::ONE_TARGET ||
           action_scope == ActionScope::ONE_ENEMY ||
           action_scope == ActionScope::ONE_ALLY ||
           action_scope == ActionScope::ONE_ALLY_NOT_USER ||
           action_scope == ActionScope::ONE_ALLY_KO ||
           action_scope == ActionScope::NOT_USER)
  {
    random_targets = Helpers::getRandElements(valid_targets, 1);
  }
  else if (action_scope == ActionScope::TWO_ENEMIES ||
           action_scope == ActionScope::TWO_ALLIES)
  {
    random_targets = Helpers::getRandElements(valid_targets, 2);
  }
  else if (action_scope == ActionScope::ONE_PARTY)
  {
    if (Helpers::flipCoin())
    {
      for (const auto& target : valid_targets)
        if (target > 0)
          random_targets.push_back(target);
    }
    else
    {
      for (const auto& target : valid_targets)
        if (target < 0)
          random_targets.push_back(target);
    }
  }

  return random_targets;
}

/*
 * Description:
 *
 * Inputs:
 * Output: 
 */
int32_t BattleMenu::getQtdrCostPaid()
{
  return qtdr_cost_paid;
}

std::vector<ActionType> BattleMenu::getValidActionTypes()
{
  return current_user->getValidActions();
}

/*
 * Description: Returns the enumerated WindowSatus of the BattleMenu.
 *
 * Inputs: none
 * Output: WindowStatus - enumerated window status.
 */
WindowStatus BattleMenu::getWindowStatus()
{
  return window_status;
}

/*
 * Description: Assigns an ActionScope, depending on conditions, of the
 *              currently selected action.
 *
 * Inputs: ActionScope new_action_scope - new enumerated scope for the action.
 * Output: none
 */
void BattleMenu::setActionScope(const ActionScope &new_action_scope)
{
  action_scope = new_action_scope;
}

/*
 * Description: Assigns a number of allies the current person will have for
 *              selection.
 *
 * Inputs: uint16_t - a new number of allies to assign
 * Output: none
 */
void BattleMenu::setNumAllies(uint16_t new_num_allies)
{
  num_allies = new_num_allies;
}

/*
 * Description: Assigns a MenuState flag a given set value.
 *
 * Inputs: MenuState flag - flag to be assigned a value.
 *         bool set_value - value to assign the flag to.
 * Output: none
 */
void BattleMenu::setMenuFlag(MenuState flag, const bool &set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description: Assigns the selectable vector of BattleSkills to the Menu.
 *
 * Inputs: std::vector<BattleSkill> new_menu_skills - new menu skill vector
 * Output: bool - true if the new Battle skills were non-empty.
 */
bool BattleMenu::setSelectableSkills(std::vector<BattleSkill> new_menu_skills)
{
  menu_skills = new_menu_skills;

  return !menu_skills.empty();
}

/*
 * Description: Assigns a vector of selectable Battle Items to the Menu.
 *
 * Inputs: std::vector<BattleItem> - vector of BattleItems.
 * Output: bool - true if the vector was non-empty.
 */
bool BattleMenu::setSelectableItems(std::vector<BattleItem> new_menu_items)
{
  menu_items = new_menu_items;

  return !menu_items.empty();
}

/*
 * Description: Assigns a vector of selectable targets (index values in vec).
 *
 * Inputs: std::vector<int32_t> new_menu_targets - the taget indexes
 * Output: bool - true if the targets are not empty.
 */
bool BattleMenu::setSelectableTargets(std::vector<int32_t> new_menu_targets)
{
  valid_targets = new_menu_targets;

  if (isValidIndex(1) && isValidIndex(2))
    std::swap(valid_targets[0], valid_targets[1]);

  if (isValidIndex(-1) && isValidIndex(-2))
  {
    auto neg_one = std::find(begin(valid_targets), end(valid_targets), -1);
    auto neg_two = std::find(begin(valid_targets), end(valid_targets), -2);
    std::iter_swap(neg_one, neg_two);
  }

  return (!valid_targets.empty());
}

/*
 * Description: Assigns a configuration to the BattleMenu.
 *
 * Inputs: Options* new_config - configuration for the BattleMenu
 * Output: bool - true if the config was not nullptr
 */
bool BattleMenu::setConfiguration(Options* new_config)
{
  config = new_config;

  return (config != nullptr);
}
