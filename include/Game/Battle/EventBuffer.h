/*******************************************************************************
* Class Name: EventBuffer [Declaration]
* Date Created: January 11, 2014
* Inheritance: None
* Description: The EventBuffer is a Buffer structure for Battle which creates,
*              stores, and clears events for both rendering and performing
*              during the processing action phase of battle.
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#ifndef EVENTBUFFER_H
#define EVENTBUFFER_H

#include <algorithm>

#include "EnumDb.h"
#include "Game/Player/Ailment.h"
#include "Game/Player/Item.h"
#include "Game/Player/Skill.h"
#include "Game/Player/Person.h"

/* A BattleEvent - an event during a battle which requires rendering and
 * performing */
struct BattleEvent
{
  EventType type;
  Action* action_use;
  Ailment* ailment_use;
  Bubby* bubby_use;
  Item*  item_use;
  Skill* skill_use;
  Person* user;
  std::vector<Person*> targets;
  int32_t amount;

  bool allies;
  bool happens;
  bool rendered;
  bool performed;
};

class EventBuffer
{
public:
  /* Generic EventBuffer instantiation function */
  EventBuffer();

  /* Annihilates an EventBuffer */
  ~EventBuffer();

private:
  /* Vector of pointers to BattleEvent objects */
  std::vector<BattleEvent*> events;
  
  /* Is the EventBuffer active? */
  bool active;

  /* Current processing index */
  int32_t curr_index;

  /* ------------ Constants --------------- */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Is a given index null? */
  bool isNullptr(BattleEvent* check_event);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Clears and frees all BattleEvents */
  void clearAll();
  void clearCurrent();
  void clearRendered();

  /* Creates an action event */
  BattleEvent* createActionEvent(EventType action_event_type,
      Action* action_use, Skill* skill_use, Person* user, Person* target,
      bool happens);

  /* Creates an ailment event (ex. Infliction or Relieving) */
  BattleEvent* createAilmentEvent(EventType event_type, Person* user,
      Person* target, Action* curr_action, Ailment* ailment_use);

  /* Creates an alteration event and returns it */
  BattleEvent* createAlterEvent(Action* alter_action, Person* target, 
      int32_t amount);

  /* Creates an assign action event */
  BattleEvent* createAssignEvent(Action* assign_action, Person* target,
      int32_t amount);

  /* Skill cooldown event with the skill pointer and the # turns remaining */
  BattleEvent* createCooldownEvent(Person* user, Skill* skill_cooldown, 
      int32_t cooldown);

  /* Creates a new blank event and returns the constructed event */
  BattleEvent* createDamageEvent(EventType damage_type, Person* target,
      int32_t amount);

  /* Creates a death event and returns the constructd object */
  BattleEvent* createDeathEvent(EventType death_type, Person* target, 
      bool allies);

  /* Creates an experience event */
  BattleEvent* createExperienceEvent(EventType exp_type, Person* target,
    int32_t amount);

  /* Overridden experience function for Bubbies */
  BattleEvent* createExperienceEvent(EventType exp_type, Bubby* target,
      int32_t amount);

  /* Creates a defend (begin/persist/break) event and returns a pointer */
  BattleEvent* createDefendEvent(EventType defend_type, Person* user);

  /* Creates a guard event and returns a pointer to the event */
  BattleEvent* createGuardEvent(EventType guard_type, Person* user,
      Person* target);
  
  /* Creates an item event */
  BattleEvent* createItemEvent(Item* item_use, Person* user, Person* target);

  /* Creates a miss event and returns a pointer */
  BattleEvent* createMissEvent(EventType miss_type, Person* user,
      std::vector<Person*> targets);

  /* Creates and returns an empty BattleEvent */
  BattleEvent* createNewEvent();

  /* Creates and returns a pass event */
  BattleEvent* createPassEvent(Person* user);

  /* Creates a revive event and returns it */
  BattleEvent* createReviveEvent(Person* target, int32_t amount);

  /* Creates a run event */
  BattleEvent* createRunEvent(EventType run_type, Person* user, bool allies);

  /* Creaes a Skill Use event with given information */
  BattleEvent* createSkillEvent(Skill* skill_use, Person* user,
      std::vector<Person*> targets, bool happens);

  /* Creates a skip type event */
  BattleEvent* createSkipEvent(EventType skip_type, Person* user,
      Skill* skill_cooldown);

  /* Creates a Skill Use Fizzle event */
  BattleEvent* createFizzleEvent(EventType fizzle_type, Person* user,
      std::vector<Person*> targets);

  /* Creates a Fizzle event and returns it */
  BattleEvent* createFizzleEvent(EventType fizzle_type, Person* user,
      Person* target);
 
  /* Annihilates the last battle event straight into the void */
  bool destroyLastEvent();

  /* Checks and returns whether a give Person will die in the current evnets */
  bool hasPersonDeathEvent(Person* target);

  /* Returns whether the EventBuffer is active */
  bool isActive();

  /* Prints out all the current events on the Buffer */
  void print(bool only_current);

  /* Prints an event at a given index */
  bool printEvent(uint32_t index);

  /* Returns a pointer to the most current battle event or nullptr */
  BattleEvent* getCurrentEvent();

  /* Returns the current index (first non-performed index) */
  int32_t getCurrentIndex();

  /* Returns the size of the event buffer */
  uint32_t getCurrentSize();

  /* Returns a pointer to the event at a given index */
  BattleEvent* getEvent(int32_t index);

  /* Returns the assigned index value */
  int32_t getIndex();

  /* Returns a pointer to the last BattleEvent of all time */
  BattleEvent* getLastEvent();

  /* Sets the EventBuffer to active */
  void setActive();

  /* Sets the index to the oldest non-performed index */
  bool setCurrentIndex();

  /* Sets the EventBuffer to inactive */
  void setInactive();

  /* Assigns a new index for event performing/rendering */
  bool setIndex(int32_t new_index);

  /* Attempts to set the next index, returns true if successful */
  bool setNextIndex();

  /* Sets the given index to performed */
  bool setPerformed(int32_t index);

  /* Sets the index to the oldest non-rendered index */
  bool setRenderIndex();

  /* Sets the given index to rendered */
  bool setRendered(int32_t index);
};

#endif //EVENTBUFFER_H
