/*******************************************************************************
* Class Name: BattleMenu [Declaration]
* Date Created: April 13th, 2013
* Date Redesigned: September 14th, 2015
*
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include <SDL2/SDL.h>

#include "Game/Battle/BattleActor.h"
#include "Game/Battle/BattleDisplayData.h"

enum BattleMenuLayer
{
  ZEROTH_LAYER = 0,
  TYPE_SELECTION = 1,
  ACTION_SELECTION = 2,
  TARGET_SELECTION = 3
};

ENUM_FLAGS(BattleMenuState)
enum class BattleMenuState
{
  SELECTION_COMPLETE = 1 << 0
};

class BattleMenu
{
public:
  /* BattleMenu Default Initialization Function */
  BattleMenu();

private:
  /* The BattleActor which is using the Menu */
  BattleActor* actor;

  /* The display data for the Battle */
  BattleDisplayData* battle_display_data;

  /* Configuration pointer for the BattleMenu */
  Options* config;

  /* Vector of frames for skill infos */
  std::vector<Frame*> frames_skill_info;

  /* The frames for skill names */
  std::vector<Frame*> frames_skill_name;

  /* The QD frame */
  Frame frame_qd;

  /* The flags for the class */
  BattleMenuState flags;

  /* The current menu layer */
  BattleMenuLayer menu_layer;

  /* The assigned Renderer */
  SDL_Renderer* renderer;

  /* The selected action_scope */
  ActionScope selected_action_scope;

  /* The selected action type */
  ActionType selected_action_type;

  /* The selected battle skill */
  BattleSkill* selected_battle_skill;

  /* The selected battle item */
  BattleSkill* selected_battle_item;

  /* The window status of the GUI */
  WindowStatus status_window;

  /* The vector of action types available for choice on the menu */
  std::vector<ActionType> valid_action_types;

  /* Selectable battle items */
  std::vector<BattleItem*> valid_battle_items;

  /* Selectable battle */
  std::vector<BattleSkill*> valid_battle_skills;

  /* The current element index for the menu */
  int32_t element_index;

  /* ------------ Constants --------------- */
  const static uint16_t kBIGBAR_OFFSET;   /* Offset of bar off bottom */
  const static uint16_t kBIGBAR_CHOOSE;   /* Additional offset for choice */
  const static uint16_t kBIGBAR_R_OFFSET; /* Offset off end for right section */

  const static uint8_t kMENU_SEPARATOR_B; /* Separator gap off bottom */
  const static uint8_t kMENU_SEPARATOR_T; /* Separator gap off top */

  const static uint8_t kSCROLL_R; /* Radius on scroll renders */

  const static uint8_t kSKILL_BORDER; /* Border around edge and elements */
  const static uint8_t kSKILL_BORDER_WIDTH; /* Width of border around element */
  const static uint8_t kSKILL_DESC_GAP;   /* Gap between name and description */
  const static uint8_t kSKILL_DESC_LINES; /* Max number of description lines */
  const static uint8_t kSKILL_DESC_SEP;   /* Gap between lines in description */
  const static uint8_t kSKILL_FRAME_S;    /* Small frame size on skill info */
  const static uint8_t kSKILL_FRAME_L;    /* Large frame size on skill info */
  const static uint8_t kSKILL_QD_GAP;     /* Gap between top edge and QD icon */
  const static uint8_t kSKILL_SEP;        /* Separator between image and text */
  const static uint8_t kSKILL_SUCCESS;    /* Gap between success and cooldown */
  const static uint8_t kSKILL_TIME_GAP;   /* Gap between cooldown and bottom */

  const static uint8_t kTYPE_MARGIN; /* Margin around text options in type */
  const static uint8_t kTYPE_MAX;    /* Max number of action types to render */
  const static uint8_t kTYPE_SELECT; /* Margin to spread select around type */

  /*=============================================================================
   * PRIVATE FUNCTIONS - OPERATION
   *============================================================================*/
private:
  /*=============================================================================
   * PUBLIC FUNCTIONS - RENDERING
   *============================================================================*/
private:
  /* Clears the Skill Frames */
  void clearSkillFrames();

  /* Creates a Frame for a given BattleSkill */
  SDL_Texture* createSkillFrame(BattleSkill* battle_skill, uint32_t width,
                                uint32_t height);

  /* Constructs all SkillFrames for the current BattleSkills */
  bool createSkillFrames(uint32_t width_left, uint32_t width_right);

  /* Rendering functions */
  bool renderActionTypes(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
  bool renderSkills(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

  /*=============================================================================
   * PUBLIC FUNCTIONS - OPERATION
   *============================================================================*/
public:
  /* Clears the information in the menu - for a new person/turn */
  void clear();

  /* Constructs other battle menu data (like the QD frame) */
  bool buildData();

  /* Return the value of a given BattleMenuState flag */
  bool getFlag(const BattleMenuState& test_flag);

  /* Assigns the Renderer */
  bool setConfig(Options* config);

  /* Assigns the DisplayData object which holds various constructed frames */
  bool setDisplayData(BattleDisplayData* battle_display_data);

  /* Assigns a BattleMenuState flag a given value */
  void setFlag(BattleMenuState flags, const bool& set_value = true);

  /* Assigns the Renderer of BattleMenu elements */
  void setRenderer(SDL_Renderer* renderer);

  /*=============================================================================
   * PUBLIC FUNCTIONS - RENDERING
   *============================================================================*/
public:
};

#endif // BATTLEMENU_H

//   /* Decrement the current menu layer */
//   bool decrementLayer(const int32_t &new_layer_index);

//   /* Increment the current menu layer */
//   bool incrementLayer(const int32_t &new_layer_index);

//   /* Adding and removing target selections */
//   bool addTarget(const int32_t &new_target);
//   bool addPartyTargets(const int32_t &party_index);
//   bool removeLastTarget(const bool &clear_all = false);

//   /* Methods for containing code for each key addition */
//   void keyDownAlpha(const char &c);
//   void keyDownCancel();
//   void keyDownDecrement();
//   void keyDownIncrement();
//   void keyDownSelect();

//   /* Returns the first target of a desired party (if exists) */
//   int32_t getPartyTargetIndex(bool opposite);

// /*=============================================================================
//  * PRIVATE FUNCTIONS - DISPLAY
//  *============================================================================*/

// /*=============================================================================
//  * PUBLIC FUNCTIONS - OPERATION
//  *============================================================================*/
// public:
//   /* Unset all BattleMenu information (for end of selection, etc) */
//   void unsetAll(const bool &window_off = false);

//   /* Returns the state of a chosen type of action */
//   bool isActionTypeSelected();

//   /* Returns the state of the current menu selection */
//   bool isActionSelected();

//   /* Returns whether the current action index has valid targets */
//   bool indexHasTargets();

//   /* Key press evnet for menu operation */
//   bool keyDownEvent(SDL_KeyboardEvent event);

//   /* Sets to the next left index */
//   void nextLeftIndex();

//   /* Sets to the next right index (if exists) */
//   void nextRightIndex();

//   /* Is the given index a valid index? */
//   bool isValidIndex(int32_t check_index);

//   /* Sets to the most left index */
//   void mostLeftIndex();

//   /* Sets to the most right index */
//   void mostRightIndex();

//   /* Swaps the party selection to the proper index */
//   void swapPartyIndex();

//   /* Layer 2 printing of items to choose from */
//   void printItems();

//   /* Prints out the state of the menu */
//   void printMenuState();

//   /* Layer 1 printing of skills to choose from */
//   void printSkills();

//   /* Resets the menu data to be used for a new Person */
//   void reset(Person* const new_user, const uint32_t &new_person_index);

//   /* Selects a random action (Skill) with a random target, or PASS if failed
//   */
//   void selectRandomAction();

//   /* Returns whether some index of the current selected type has targets */
//   bool someIndexHasTargets();

//   /* Prints out the list of (selected and) valid targets to choose from */
//   void printTargets(const bool &print_selected = false);

//   /* Layer 0 printing of actions to choose from */
//   void printValidActions();

//   /* Obtains the selected enumerated ActionType */
//   ActionType getActionType();

//   /* Obtains user selected targets for the action from the Menu */
//   // std::vector<int32_t> getActionTargets();

//   /* Get a pointer to the current person */
//   // Person* getCurrentUser();

//   /* Get the element index (ex. index of skill selection */
//   int32_t getElementIndex();

//   /* Get targets hovered over during the selection process */
//   std::vector<int32_t> getHoverTargets();

//   /* Obtain the layer index of the menu */
//   int32_t getLayerIndex();

//   /* Finds the maximum index for the current layer */
//   int32_t getMaxIndex();

//   /* Returns the vector of targets for the party */
//   std::vector<int32_t> getPartyTargets(int32_t party_index);

//   /* Return the index of the person assigned to the menu */
//   int32_t getPersonIndex();

//   /* Returns a vector of random targets */
//   std::vector<int32_t> getRandomTargets();

//   /* Obtain the true QD cost paid by a user (to return the right amount) */
//   int32_t getQtdrCostPaid();

//   /* The currently selected Skill (if set) */
//   BattleSkill* getSelectedSkill();

//   /* The currently selected Item (if set) */
//   Item* getSelectedItem();

//   /* Returns a vector of action types the user may use */
//   std::vector<ActionType> getValidActionTypes();

//   /* Returns the window status of the BattleMenu */
//   WindowStatus getWindowStatus();

//   /* Assigns the scope of the skill when a skill has been chosen */
//   // void setActionScope(const ActionScope &new_action_scope);

//   /* Assigns the number of allies the curr person has (1 == self) */
//   void setNumAllies(uint16_t num_allies);

//   /* Assigns valid targets for the menu */
//   bool setSelectableTargets(std::vector<int32_t> valid_targets);

//   /* Set the window status for the Battle menu */
//   void setWindowStatus(WindowStatus new_window_status);
