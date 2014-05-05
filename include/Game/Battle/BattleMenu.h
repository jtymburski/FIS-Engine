/*******************************************************************************
* Class Name: BattleMenu [Declaration]
* Date Created: April 13th, 2018
* Inheritance: None
* Description: 
*
* Notes
* -----
*
* [1]: [Action Type Layer]  -- [Action Selection Layer] -- [Target Layer]
*      -------------------     ------------------------    --------------
*      <Skill>           
*      <Item>                  --- <Select items> ------   <Item dependable>
*      <Guard>                 -------------------------   <Non-self target>
*      <Run>                   -------------------------   -----------------
*
* TODO
* ----
*******************************************************************************/
#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include "EnumDb.h"
#include "Options.h"
#include "Game/Player/Item.h"
#include "Game/Player/SkillSet.h"

#include <vector>

class BattleMenu
{
public:
  /* BattleMenu */
  BattleMenu();

private:
  /* Valid action types for the current person index */
  std::vector<ActionType> valid_actions;

  /* The current selectable items on the menu */
  std::vector<std::pair<Item*, uint16_t>> menu_items;

  /* The current selectable skills on the menu */
  std::vector<Skill*> menu_skills;

  /* Remaining valid targets that can be chosen and already chosen targets */
  std::vector<int32_t> valid_targets;
  std::vector<int32_t> selected_targets;

  /* Boolean to represent an action selection completed */
  bool selection_complete;

  /* The type of the action that was chosen (if selection_completed) */ 
  ActionType action_type;

  /* The index along the corresponding action which was chosen (if selected) */
  int32_t action_index;

  /* The vector of targets which were chosen for the action */
  std::vector<int32_t> action_targets;

  /* Currently assigned options */
  Options* config;

  /* The window status of the BattleMenu */
  WindowStatus window_status;

  /* Menu indexes */
  int32_t person_index;
  int32_t layer_index;
  int32_t element_index;

  /* ------------ Constants --------------- */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Decrease and increment the menu layers */
  bool decrementLayer();
  bool incrementLayer();

  /* Decrease, increment, or assign the index of the selected layer */
  bool decrementIndex();
  bool incrementIndex();
  bool setIndex(const std::string subset);

  /* Adding and removing target selections */
  bool addTarget(const int32_t &new_target);
  bool removeLastTarget();

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the state of the current menu selection */
  bool isActionSelected();

  /* Resets the menu data to be used for a new Person */
  void reset(const uint32_t &new_person_index);

  /* Gets the user to select an action and targets for an action */
  bool selectAction();

  /* Layer 0 printing of actions to choose from */
  void printValidActions();

  /* Layer 1 printing of skills to choose from */
  void printSkills();

  /* Layer 2 printing of items to choose from */
  void printItems();

  /* Key press evnet for menu operation */
  bool keyDownEvent(SDL_KeyboardEvent event);

  /* Obtains the selected enumerated ActionType */
  ActionType getActionType();
  
  /* Obtains the index of  */
  int32_t getActionIndex();

  /* Obtains user selected targets for the action from the Menu */
  std::vector<int32_t> getActionTargets();

  /* Finds the maximum index for the current layer */
  int32_t getMaxIndex();

  /* Returns the window status of the BattleMenu */
  WindowStatus getWindowStatus();

  /* Assigns a new selectable list of actions */
  bool setSelectableActions(std::vector<ActionType> new_valid_actions);

  /* Assigns a new selectable list of items for the menu */
  bool setSelectableItems(std::vector<std::pair<Item*, uint16_t>> new_menu_items);

  /* Assigns new selectable list of skills for the menu */
  bool setSelectableSkills(std::vector<Skill*> new_menu_skills);
 
  /* Assigns the running configuration of the menu */
  bool setConfiguration(Options* new_config);

  /* Assigns a new person index to choose an action for */
  void setPersonIndex(const int32_t &new_person_index);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
};

#endif //BATTLEMENU_H