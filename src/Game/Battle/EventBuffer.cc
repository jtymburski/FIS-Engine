/*******************************************************************************
* Class Name: EventBuffer [Implementation]
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
* See .h file for TODOs
******************************************************************************/
#include "Game/Battle/EventBuffer.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Instantiates a new event buffer - generic constructor
 *
 * Inputs: none
 */
EventBuffer::EventBuffer()
  : active{false}
  , curr_index{-1}
{}

/*
 * Description: Annihilates the event buffer.
 */
EventBuffer::~EventBuffer()
{
  clearAll();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Determines whether a given BattleEvent pointer is nullptr
 *
 * Inputs: BattleEvent* check_event - check event to check for nullptr
 * Output: bool - true if the given index is nullptr
 */
bool EventBuffer::isNullptr(BattleEvent* check_event)
{
  return (check_event == nullptr);
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Clears all events in the buffer.
 *
 * Inputs: none
 * Output: none
 */
void EventBuffer::clearAll()
{
  for (auto event : events)
  {
    if (event != nullptr)
      delete event;
    event = nullptr;
  }

  events.clear();
}

/*
 * Description: Clears and removes all non-rendered events from the buffer.
 *
 * Inputs: none
 * Output: none
 */
void EventBuffer::clearCurrent()
{
  for (auto event : events)
  {
    if (event != nullptr && !event->rendered)
      delete event;
    event = nullptr;
  }

  // events.erase(std::remove_if(std::begin(events), std::end(events), 
  //     isNullptr), std::end(events));
}

/*
 * Description: Clears and removes all rendered events from the buffer.
 *
 * Inputs: none
 * Output: none
 */
void EventBuffer::clearRendered()
{
  for (auto event : events)
  {
    if (event != nullptr && event->rendered)
      delete event;
    event = nullptr;
  }

  // events.erase(std::remove_if(std::begin(events), std::end(events), 
  //     isNullptr), std::end(events));
}

/*
 * Description: Creates and appends an action use start event to list of events.
 *
 * Inputs: Action* action_use - pointer to the action being used
 *         Skill* skill_use - the Skill being used.
 *         User* user - the user of the skill
 *         std::vector<Person*> targets - all targets of the skill
 *         bool happens - true if the action is to miss, false if to miss
 * Output: BattleEvent* - pointer to the recently created event
 */
BattleEvent* EventBuffer::createActionEvent(Action* action_use, 
    Skill* skill_use, Person* user, Person* target, bool happens)
{
  auto new_event = createNewEvent();
  new_event->type = EventType::ACTION_BEGIN;
  new_event->action_use = action_use;
  new_event->skill_use  = skill_use;
  new_event->user       = user;
  std::vector<Person*> target_vec{target};
  new_event->targets = target_vec;
  new_event->happens = happens;
  events.push_back(new_event);
  
  return new_event;
}

/*
 * Description: Creates a new blank event 
 *
 * Inputs: EventType damage_type - the type of damage (standard/poison/etc.)
 *         Person* target - the target of the damage
 *         int32_t amount - the amount of the damage 
 * Output: BattleEvent* - pointer to the recently created event
 */
BattleEvent* EventBuffer::createDamageEvent(EventType damage_type, 
    Person* target, int32_t amount)
{
  auto new_event = createNewEvent();
  new_event->type = damage_type;
  std::vector<Person*> target_vec{target};
  new_event->targets = target_vec;
  new_event->amount  = amount;
  new_event->happens = true;
  events.push_back(new_event);

  return new_event;
}

/*
 * Description:
 *
 * Inputs:
 * Output: 
 */
BattleEvent* EventBuffer::createDeathEvent(EventType death_type, Person* target,
     bool allies)
{
  auto new_event = createNewEvent();
  new_event->type = death_type;
  std::vector<Person*> target_vec{target};
  new_event->targets = target_vec;
  new_event->allies = allies;
  events.push_back(new_event);

  return new_event;
}

/*
 * Description: Creates a new defend event and appends it to the list of events.
 *
 * Inputs: EventType defend_type - the type of defending event
 *         Person* user - the user of the defend event
 * Output: BattleEvent* - pointer to the recently created defend event
 */
BattleEvent* EventBuffer::createDefendEvent(EventType defend_type, Person* user)
{
  auto new_event = createNewEvent();
  new_event->type = defend_type;
  new_event->user = user;
  events.push_back(new_event);

  return new_event;
}

/*
 * Description: Creates a new guard event and appends it to the list of events.
 *
 * Inputs: EventType guard_type - the type of guarding event
 *         Person* user - the user of the guard event
 *         Person* target - the target of the guard event
 * Output: BattleEvent* - pointer to the recently created defend event
 */
BattleEvent* EventBuffer::createGuardEvent(EventType guard_type, Person* user,
    Person* target)
{
  auto new_event = createNewEvent();
  new_event->type = guard_type;
  new_event->user = user;
  std::vector<Person*> target_vec{target};
  new_event->targets = target_vec;

  return new_event;
}

/*
 * Description: Creates and appends a skill miss event to the list of events.
 *
 * Inputs: Person* user - user of the Skill which misses
 *         std::vector<Person*> targets - targets of the Skill
 * Output: BattleEvent* - pointer to the recently created miss event
 */
BattleEvent* EventBuffer::createMissEvent(EventType miss_type, Person* user, 
    std::vector<Person*> targets)
{
  auto new_event = createNewEvent();
  new_event->type = miss_type;
  new_event->user = user;
  new_event->targets = targets;
  events.push_back(new_event);

  return new_event;
}

/*
 * Description: Creates a new blank event 
 *
 * Inputs: none
 * Output: BattleEvent* - pointer to the recently created empty event
 */
BattleEvent* EventBuffer::createNewEvent()
{
  auto new_event = new BattleEvent();
  new_event->type       = EventType::NONE;
  new_event->action_use = nullptr;
  new_event->item_use   = nullptr;
  new_event->skill_use  = nullptr;
  new_event->user       = nullptr;
  new_event->amount     = -1;
  new_event->happens    = false;
  new_event->rendered   = false;
  new_event->performed  = false;

  return new_event;
}

/*
 * Description: Creates a pass event
 *
 * Inputs: none
 * Output: BattleEvent* - pointer to the recently created pass event.
 */
BattleEvent* EventBuffer::createPassEvent(Person* user)
{
  auto new_event = new BattleEvent();
  new_event->type = EventType::PASS;
  new_event->user = user;

  return new_event;
}

/*
 * Description: Creates a run event with a type (ATTMEPT/SUCCEED/FAIL) and
 *              a bool whether the running action is taking place by an ally
 *              or an enemy.
 *
 * Inputs: EventType run_type - whether the run is an ATTEMPT, SUCCEED, or FAIL
 *         Person* user - pointer to the user of the running action
 * Output: BattleEvent* - pointer to the recently created event.
 */
BattleEvent* EventBuffer::createRunEvent(EventType run_type, Person* user, 
    bool allies)
{
  auto new_event = createNewEvent();
  new_event->type = run_type;
  new_event->user = user;
  new_event->allies = allies;
  events.push_back(new_event);

  return new_event;
}

/*
 * Description: Creates and appends a skill use start event to list of events.
 *
 * Inputs: Skill* skill_use - the Skill being used.
 *         User* user - the user of the skill
 *         std::vector<Person*> targets - all targets of the skill
 *         bool happens - true if the skill is to miss, false if to miss
 * Output: BattleEvent* - pointer to the recently created event
 */
BattleEvent* EventBuffer::createSkillEvent(Skill* skill_use, Person* user, 
    std::vector<Person*> targets, bool happens)
{
  auto new_event = createNewEvent();
  new_event->type       = EventType::SKILL_USE;
  new_event->skill_use  = skill_use;
  new_event->user       = user;
  new_event->targets    = targets;
  new_event->happens    = happens;
  events.push_back(new_event);

  return new_event;
}

/*
 * Description: Creates and appens a skill fizzle event to the list of events.
 *
 * Inputs: Person* user - the user of the Skill which fizzled.
 *         std::vector<Person*> targets - the targets which fizzled
 * Output: BattleEvent* - pointer to the recently created event
 */
BattleEvent* EventBuffer::createFizzleEvent(EventType fizzle_type, Person* user,
    std::vector<Person*> targets)
{
  auto new_event = createNewEvent();
  new_event->type = fizzle_type;
  new_event->user = user;
  new_event->targets = targets; 
  events.push_back(new_event);

  return new_event;
}

/*
 * Description: Destroys the last event, if it exists. If this action was
 *              able to take place, this function will return true.
 *
 * Inputs: none
 * Output: bool - true if the last event was able to be destroyed
 */
bool EventBuffer::destroyLastEvent()
{
  auto last_event = getLastEvent();

  if (last_event != nullptr)
  {
    delete last_event;
    last_event = nullptr;
    events.pop_back();

    return true;
  }

  return false;
}

/*
 * Description: Returns whether the EventBuffer is active
 *
 * Inputs: none
 * Output: bool - the active-ness of the EventBuffer
 */
bool EventBuffer::isActive()
{
  return active;
}

/*
 * Description: Prints only curr
 *
 * Inputs: bool only_current - whether to print only non-rendered events
 * Output: none
 */
void EventBuffer::print(bool only_current)
{
  std::cout << "==== Event Buffer ====\n";
  std::cout << "Size: " << events.size() << " Index: " << curr_index <<  "\n";

  for (uint32_t i = 0; i < events.size(); i++)
  {
    if (only_current && !events.at(i)->rendered)
      printEvent(i);
    else if (!only_current)
      printEvent(i);
  }
}

/*
 * Description: Print an event of the given index
 *
 * Inputs: none
 * Output: bool - true if the index was valid
 */
bool EventBuffer::printEvent(uint32_t index)
{
  if (index < events.size())
  {
    std::cout << "EventType: " << static_cast<int>(events.at(index)->type) <<
                 " " << Helpers::eventToStr(events.at(index)->type);

    if (events.at(index)->action_use != nullptr)
      std::cout << "\nAction: " << events.at(index)->action_use;
    if (events.at(index)->skill_use != nullptr)
      std::cout << "\nSkill: " << events.at(index)->skill_use->getName();
    if (events.at(index)->user != nullptr)
      std::cout << "\nUser: " << events.at(index)->user->getName();

    for (const auto& target: events.at(index)->targets)
      if (target != nullptr)
        std::cout << "\nTarget: " << target->getName();
    
    std::cout << "\nAmount: " << events.at(index)->amount;
    std::cout << "\nAllies?  " << events.at(index)->allies << std::endl;
    std::cout << "Happens? " << events.at(index)->happens << std::endl;
    std::cout << "Rendered?  " << events.at(index)->rendered << std::endl;
    std::cout << "Performed? " << events.at(index)->performed << std::endl
              << std::endl;
  
    return true;
  }

  return false;
}

/*
 * Description: The "current" size of the buffer, the distance between the
 *              current index and the end of the buffer.
 *
 * Inputs: none
 * Output: uint32_t - the "current" size of the Buffer.
 */
uint32_t EventBuffer::getCurrentSize()
{
  if (getCurrentIndex() != -1)
    return events.size() - getCurrentIndex();

  return 0;
}

/*
 * Description: Returns a pointer to the BattleEvent at a given index.
 *
 * Inputs: int32_t index - the index to grab the event for
 * Output: BattleEvent* - pointer to an event at a given index
 */
BattleEvent* EventBuffer::getEvent(int32_t index)
{
  if (static_cast<uint32_t>(index) < events.size())
    return events.at(index);

  return nullptr;
}

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
int32_t EventBuffer::getIndex()
{
  return curr_index;
}

/*
 * Description: Returns a pointer to the last event on the event buffer.
 *
 * Inputs: none
 * Output: BattleEvent* - pointer to the last event
 */
BattleEvent* EventBuffer::getLastEvent()
{
  if (events.size() > 0)
    return events[events.size() - 1];

  return nullptr;
}

/*
 * Description: Returns a pointer to the BattleEvent at the curr index, if
 *              it's valid else returns a nullptr
 *
 * Inputs: none
 * Output: BattleEvent* - pointer to the event at the current index.
 */
BattleEvent* EventBuffer::getCurrentEvent()
{
  if (curr_index >= 0 && static_cast<uint32_t>(curr_index) < events.size())
    return events.at(curr_index);

  return nullptr;
}

/*
 * Description: Returns the current index of the event buffer (the first index
 *              that has not been performed).
 *
 * Inputs: none
 * Output: int32_t - the current index of the event buffer, or -1 if not found
 */
int32_t EventBuffer::getCurrentIndex()
{
  for (uint32_t i = 0; i < events.size(); i++)
    if (events.at(i)->performed == false)
      return i;

  return -1;
}

/*
 * Description: Sets the activeness of the EventBuffer to true
 *
 * Inputs: none
 * Output: none
 */
void EventBuffer::setActive()
{
  active = true;
}

/*
 * Description: Assigns the current index of the buffer to the oldest
 *              non-performed index.
 *
 * Inputs: none
 * Output: bool - true if a non-performed index was found and the index was set
 */
bool EventBuffer::setCurrentIndex()
{
  if (getCurrentIndex() != -1)
  {
    curr_index = getCurrentIndex();

    return true;
  }

  return false;
}

/*
 * Description: Sets the activeness of the EventBuffer to  false
 *
 * Inputs: none
 * Output: none
 */
void EventBuffer::setInactive()
{
  active = false;
}

/*
 * Description: Assigns a new index for rendering/performing events on the 
 *              buffer.
 *
 * Inputs: int32_t new_index - assigns the current index of the buffer.
 * Output: bool 
 */
bool EventBuffer::setIndex(int32_t new_index)
{
  if (static_cast<uint32_t>(new_index) < events.size())
  {
    curr_index = new_index;

    return true;
  }

  return false;
}

/*
 * Description: Assigns the next index of the Buffer, returning true if
 *              the next index was in range and assignment successful.
 *
 * Inputs: none
 * Output: bool - true if the next index existed and was assigned
 */
bool EventBuffer::setNextIndex()
{
  if (static_cast<uint32_t>(curr_index) + 1 < events.size())
  {
    curr_index++;

    return true;
  }

  return false;
}

/*
 * Description: Assigns the current index's performed flag to true
 *
 * Inputs: none
 * Output: bool - true if the index was valid and the index was set
 */
bool EventBuffer::setPerformed(int32_t index)
{
  if (static_cast<uint32_t>(index) < events.size())
  {
    events.at(index)->performed = true;

    return true;
  }

  return false;
}

/*
 * Description: Assigns the current index of the buffer to to the oldest 
 *              non-rendered index.
 *
 * Inputs: none
 * Output: bool - true if a non-rendered index was found and the index was set
 */
bool EventBuffer::setRenderIndex()
{
  for (uint32_t i = 0; i < events.size(); i++)
  {
    if (events.at(i)->rendered == false)
    {
      curr_index = i;

      return true;
    }
  }

  return false;
}

/*
 * Description: Assigns the current index's rendered flag to true
 *
 * Inputs: none
 * Output: bool - true if the index was valid and the index was set
 */
bool EventBuffer::setRendered(int32_t index)
{
  if (static_cast<uint32_t>(index) < events.size())
  {
    events.at(index)->rendered = true;

    return true;
  }

  return false;
}