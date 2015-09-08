/*******************************************************************************
* Class Name: Battle [Declaration]
* Date Created: February 23rd, 2014
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
*******************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#include "Game/Battle/BattleActor.h"

using std::begin;
using std::end;

/* Enumerated method of sorting for the Buffer objects */
enum class BufferSorts
{
  ITEM_FIRST,
  SKILL_FIRST,
  MOMENTUM,
  NONE
};

/* A buffer element */
struct BufferAction
{
  /* The number of turns to remain in the buffer */
  uint32_t cooldown;

  /* The turn # the buffer element was added */
  uint32_t initial_turn;

  /* Pointer to the user of the action */
  BattleActor* user;

  /* The Skill OR Item which the action will perform - only oe will be set */
  Skill* skill_used;
  Item*  item_used;

  /* The vector of targets for the action */
  std::vector<BattleActor*> targets;

  /* Enumerated type of buffer (SKILL or ITEM or NONE) */
  ActionType type;

  /* The validity of the element */
  bool valid;
};

class Buffer
{
public:
  /* Constructs a Buffer object */
  Buffer();

private:
  /* The current index of the buffer */
  uint32_t index;

  /* The buffer of actions to take place */
  std::vector<BufferAction> action_buffer;

  /* ------------ Constants --------------- */
  static const uint16_t kMAXIMUM_ELEMENTS;
  static const uint16_t kMAXIMUM_TARGETS;
  static const uint16_t kMAXIMUM_COOLDOWN;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *==========================================================================*/
private:
  /* Checks the validity of a given BufferAction element */
  bool checkValid(BufferAction& elm);

  /* Decrements the cooldown of a BufferAction at a given index */
  bool decrementCooldown(const uint32_t &index);

  /* Returns a ref. to a BufferAction at a given index */
  BufferAction& getIndex(const uint32_t &index);

  /* Sorts a vector of Buffer actions by a given buffer sort */
  std::vector<BufferAction> sort(std::vector<BufferAction> actions,
      BufferSorts buffer_sorts);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Creates and adds a new Skill BufferAction element given params */
  bool add(BattleActor* const user, Skill* const skill_used,
      std::vector<BattleActor*> targets, const uint32_t &cooldown = 0,
      const uint32_t &initial_turn = 0);

  /* Creates and adds a new Item BufferAction element given params */
  bool add(BattleActor* const user, Item* const item_used,
      std::vector<BattleActor*> targets, const uint32_t &cooldown = 0,
      const uint32_t &initial_turn = 0);

  bool add(BattleActor* const user, ActionType const &buffer_type,
      std::vector<BattleActor*> targets, const uint32_t &cooldown = 0,
      const uint32_t &initial_turn = 0);

  /* Adds a given BufferAction element to the vector */
  bool add(BufferAction &action);

  /* Clears all BufferAction elements from the Buffer */
  void clearAll(int32_t this_turn_only = -1);

  /* Clears all invalid buffer elements */
  void clearInvalid();

  /* Checks if an element containing a Skill cooldown for given person exists */
  Skill* hasCoolingSkill(BattleActor* check_person);

  /* Returns true if the next BufferAction is valid */
  bool isNextValid();

  /* For every action which targets a guarded person -> swap in the guard */
  // void injectGuardTargets(Person* guard, Person* guardee);

  /* Prints out the information of the Buffer */
  void print(const bool &simple = false);

  /* Attempts to remove a BufferAction at a given index */
  bool remove(const uint32_t &index);

  /* Remove a guard's targets from buffer actions */
  // void rejectGuardTargets(Person* guard);

  /* Removes all BufferActions which are of the given user type */
  void removeAllByUser(BattleActor* user);

  /* Updates the cooldown of each BufferAction element */
  void update(const bool &clear = false);

  /* Returns the action type for the current index */
  ActionType getActionType();

  /* Returns the value for the cooldown at the current element index */
  int32_t getCooldown();

  /* Obtains the Person ptr for the current element index */
  Person* getUser();

  /* Obtains the Skill ptr for the current element index */
  Skill* getSkill();

  /* Obtains the Item ptr for the current element index */
  Item* getItem();

  /* Returns the initial turn assigned to the current element index */
  int32_t getInitialTurn();

  /* Obtains the target vector for current element index */
  std::vector<BattleActor*> getTargets();

  /* Returns the corresponding damage types for target indexes */
  // std::vector<DamageType> getDamageTypes();

  /* Sets the next index of the Buffer if available */
  bool setNext();

  /* Reorders the Buffer based off given primary and secondary sorts */
  bool reorder();

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the maximum possible size of the Buffer */
  static uint16_t getMaxSize();

};

#endif //BUFFER_H
