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

enum class BufferType
{
  SKILL,
  ITEM,
  NONE
};

enum class BufferSorts
{
  ITEM_FIRST,
  SKILL_FIRST,
  MOMENTUM
};


struct BufferAction
{
  uint32_t cooldown;

  Person* user;

  Skill* skill_used;
  Item*  item_used;

  std::vector<Person*> targets;

  BufferType type;

  bool valid;
};

class Buffer
{
public:
  /* */
  Buffer();

private:
  
  uint32_t index;

  std::vector<BufferAction> action_buffer;

  /* ------------ Constants --------------- */
  static const uint16_t kMAXIMUM_ELEMENTS;
  static const uint16_t kMAXIMUM_TARGETS;
  static const uint16_t kMAXIMUM_COOLDOWN;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

  bool checkValid(BufferAction& elm);

  bool decrementCooldown(const uint32_t &index);

  BufferAction& getIndex(const uint32_t &index);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  bool add(Person* const user, Skill* const skill_used, 
           std::vector<Person*> targets, const uint32_t &cooldown = 0);

  bool add(Person* const user, Item* const item_used, 
           std::vector<Person*> targets, const uint32_t &cooldown = 0);

  bool add(BufferAction &action);

  void clearAll();

  void clearInvalid();

  bool isNextValid();

  void print(const bool &simple = false);

  bool remove(const uint32_t &index);

  bool removeAllByUser(Person* user);

  void update(const bool &clear = false);

  Person* getUser();

  Skill* getSkill();

  std::vector<Person*> getTargets();

  bool setNext();

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* */
  static uint16_t getMaxSize();

  /*  */
  static bool sort(std::vector<BufferAction> actions, BufferSorts buffer_sorts);


};

#endif //BUFFER_H
