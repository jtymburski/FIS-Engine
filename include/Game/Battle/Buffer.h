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

#include "Game/Player/Item.h"
#include "Game/Player/Skill.h"
#include "Game/Player/Person.h"

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

  /* Pointer to the user of the action */
  Person* user;

  /* The Skill OR Item which the action will perform - only oe will be set */
  Skill* skill_used;
  Item*  item_used;

  /* The vector of targets for the action */
  std::vector<Person*> targets;

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
  bool sort(BufferSorts buffer_sorts);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Creates and adds a new Skill BufferAction element given params */
  bool add(Person* const user, Skill* const skill_used, 
           std::vector<Person*> targets, const uint32_t &cooldown = 0);

  /* Creates and adds a new Item BufferAction element given params */
  bool add(Person* const user, Item* const item_used, 
           std::vector<Person*> targets, const uint32_t &cooldown = 0);

  bool add(Person* const user, ActionType const &buffer_type,
           std::vector<Person*> targets, const uint32_t &cooldown = 0);

  /* Adds a given BufferAction element to the vector */
  bool add(BufferAction &action);

  /* Clears all BufferAction elements from the Buffer */
  void clearAll();

  /* Clears all invalid buffer elements */
  void clearInvalid();

  /* Returns true if the next BufferAction is valid */
  bool isNextValid();

  /* Prints out the information of the Buffer */
  void print(const bool &simple = false);

  /* Attempts to remove a BufferAction at a given index */
  bool remove(const uint32_t &index);

  /* Removes all BufferActions which are of the given user type */
  bool removeAllByUser(Person* user);

  /* Updates the cooldown of each BufferAction element */
  void update(const bool &clear = false);

  /* Obtains the Person ptr for the current element index */
  Person* getUser();

  /* Obtains the Skill ptr for the current element index */
  Skill* getSkill();

  /* Obtains the target vector for current element index */
  std::vector<Person*> getTargets();

  /* Sets the next index of the Buffer if available */
  bool setNext();

  /* Reorders the Buffer based off given primary and secondary sorts */
  bool reorder(BufferSorts primary, BufferSorts secondary = BufferSorts::NONE);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the maximum possible size of the Buffer */
  static uint16_t getMaxSize();

};

#endif //BUFFER_H