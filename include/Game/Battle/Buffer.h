/*******************************************************************************
* Class Name: Buffer [Declaration]
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
  BufferAction()
      : cooldown{0},
        initial_turn{0},
        started{false},
        processed{false},
        user{nullptr},
        used_skill{nullptr},
        targets{},
        owned_item{nullptr},
        type{ActionType::NONE}
  {
  }

  /* The number of turns needed after cooldown */
  uint32_t cooldown;

  /* The turn # the buffer element was added */
  uint32_t initial_turn;

  /* Has this element started processing */
  bool started;

  /* Bool will be set once an element is processed */
  bool processed;

  /* Pointer to the user of the action */
  BattleActor* user;

  /* The Skill OR Item which the action will perform - only oe will be set */
  BattleSkill* used_skill;

  /* The vector of targets the action will target */
  std::vector<BattleActor*> targets;

  /* Owned item */
  Item* owned_item;

  /* Enumerated type of buffer (SKILL or ITEM or NONE) */
  ActionType type;
};

class Buffer
{
public:
  /* Constructs a Buffer object */
  Buffer();

private:
  /* The current index of the buffer */
  uint32_t index;

  /* Has the buffer been sorted? */
  bool sorted;

  /* The buffer of actions to take place */
  std::vector<BufferAction> action_buffer;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Returns a ref. to a BufferAction at a given index */
  BufferAction& getIndex(const uint32_t& index);

  /* Sorts a vector of Buffer actions by a given buffer sort */
  std::vector<BufferAction> sort(std::vector<BufferAction> actions,
                                 BufferSorts buffer_sorts);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Adds a defend element to the buffer */
  void addDefend(BattleActor* user);

  /* Adds a guard element to the buffer */
  void addGuard(BattleActor* user, BattleActor* target);

  /* Add an imploding element to the buffer */
  void addImplode(BattleActor* user);

  /* Adds an Item element to the buffer */
  void addItem(BattleActor* user, BattleItem* used_item,
               std::vector<BattleActor*> targets);

  /* Adds a Pass event to the Battle */
  void addPass(BattleActor* user, int32_t initialturn);

  /* Adds a run element to the buffer */
  void addRun(BattleActor* user);

  /* Adds a skill use to the buffer */
  void addSkill(BattleActor* user, BattleSkill* used_skill,
                std::vector<BattleActor*> targets, uint32_t cooldown = 0,
                uint32_t initial_turn = 0);

  /* Clears all elements from the Buffer */
  void clear();

  /* Clear the currently owned item */
  void clearItem();

  /* Clears all BufferAction elements from the Buffer */
  void clearForTurn(uint32_t turn_number);

  /* Checks if an element containing a Skill cooldown for given person exists */
  bool hasCoolingSkill(BattleActor* check_person);

  /* Is the current element processed? */
  bool isIndexProcessed();

  /* Has the index started processing */
  bool isIndexStarted();

  /* Has the buffer been sorted this turn? */
  bool isSorted();

  /* Prints out the information of the Buffer */
  void print(bool simple = false);

  /* Removes all BufferActions which are of the given user type */
  void removeAllByUser(BattleActor* user);

  /* Updates the cooldown of each BufferAction element */
  void updateCooldowns();

  /* Returns the action type for the current index */
  ActionType getActionType();

  /* Returns the value for the cooldown at the current element index */
  int32_t getCooldown();

  /* Obtains the Person ptr for the current element index */
  BattleActor* getUser();

  /* Obtains the Skill ptr for the current element index */
  BattleSkill* getSkill();

  /* Obtains the Item ptr for the current element index */
  Item* getItem();

  /* Returns the initial turn assigned to the current element index */
  int32_t getInitialTurn();

  /* Obtains the target vector for current element index */
  std::vector<BattleActor*> getTargets();

  /* Sets the next index of the Buffer if available, returns true if done */
  bool setNext();

  /* Set the currrent index as processed */
  void setProcessed();

  /* Set the current index as started */
  void setStarted();

  /* Reorders the Buffer based off given primary and secondary sorts */
  void reorder();
};

#endif // BUFFER_H
