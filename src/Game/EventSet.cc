/******************************************************************************
 * Class Name: EventSet
 * Date Created: October 20, 2015
 * Inheritance: none
 * Description: The event set handles the set of events that can be stored at
 *              a given trigger location. The event set can be locked, unlocked,
 *              includes multiple events, and handles all associated data.
 *****************************************************************************/
#include "Game/EventSet.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t EventSet::kBATTLE_EVENT_WIN = 0;
const uint8_t EventSet::kBATTLE_EVENT_LOSE = 1;
const uint8_t EventSet::kBATTLE_FLAGS = 0;
const uint8_t EventSet::kGIVE_ITEM_CHANCE = 3;
const uint8_t EventSet::kGIVE_ITEM_COUNT = 1;
const uint8_t EventSet::kGIVE_ITEM_FLAGS = 2;
const uint8_t EventSet::kGIVE_ITEM_ID = 0;
const uint8_t EventSet::kMAP_ID = 0;
const uint8_t EventSet::kPROP_BOOLS = 3;
const uint8_t EventSet::kPROP_ID = 1;
const uint8_t EventSet::kPROP_INACTIVE = 7;
const uint8_t EventSet::kPROP_MODS = 2;
const uint8_t EventSet::kPROP_RESPAWN = 4;
const uint8_t EventSet::kPROP_SPEED = 5;
const uint8_t EventSet::kPROP_TRACK = 6;
const uint8_t EventSet::kPROP_TYPE = 0;
const uint8_t EventSet::kTAKE_ITEM_COUNT = 1;
const uint8_t EventSet::kTAKE_ITEM_ID = 0;
const uint8_t EventSet::kTELEPORT_ID = 0;
const uint8_t EventSet::kTELEPORT_SECTION = 3;
const uint8_t EventSet::kTELEPORT_X = 1;
const uint8_t EventSet::kTELEPORT_Y = 2;
const uint8_t EventSet::kUNIO_ID = 0;
const uint8_t EventSet::kUNIO_MODE = 1;
const uint8_t EventSet::kUNIO_MODE_EVENT = 3;
const uint8_t EventSet::kUNIO_STATE = 2;
const uint8_t EventSet::kUNIO_VIEW_MODE = 4;
const uint8_t EventSet::kUNIO_VIEW_TIME = 5;
const uint8_t EventSet::kUNTHING_ID = 0;
const uint8_t EventSet::kUNTHING_VIEW_MODE = 1;
const uint8_t EventSet::kUNTHING_VIEW_TIME = 2;
const uint8_t EventSet::kUNTILE_MODE = 3;
const uint8_t EventSet::kUNTILE_SECTION = 0;
const uint8_t EventSet::kUNTILE_X = 1;
const uint8_t EventSet::kUNTILE_Y = 2;
const uint8_t EventSet::kUNTILE_VIEW_MODE = 4;
const uint8_t EventSet::kUNTILE_VIEW_TIME = 5;
/* ---- */
const uint8_t EventSet::kHAVE_ITEM_CONSUME = 0;
const uint8_t EventSet::kHAVE_ITEM_COUNT = 1;
const uint8_t EventSet::kHAVE_ITEM_ID = 0;
/* ---- */
const int32_t EventSet::kVIEW_TIME = 1000;
const int32_t EventSet::kUNSET_ID = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor for event set. Sets up the event with nothing
 *              set and no lock settings.
 *
 * Inputs: none
 */
EventSet::EventSet()
{
  base = nullptr;
  event_locked = createBlankEvent();
  events_unlocked.clear();

  get_index = -1;
  locked_status = createBlankLocked();
  unlocked_state = UnlockedState::ORDERED;
}

/*
 * Description: Copy constructor. Copies data from EventSet source
 *
 * Inputs: EventSet &source - the source to copy the data from
 */
EventSet::EventSet(const EventSet& source) : EventSet()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EventSet::~EventSet()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
/*
 * Description: Copies all data from source event set to this event set.
 *
 * Inputs: EventSet& source - the source to copy from
 * Output: none
 */
void EventSet::copySelf(const EventSet& source)
{
  /* Clear data first */
  clear();

  /* Copy data in */
  base = source.base;
  event_locked = copyEvent(source.event_locked);
  for(uint32_t i = 0; i < source.events_unlocked.size(); i++)
    events_unlocked.push_back(copyEvent(source.events_unlocked[i]));
  locked_status = source.locked_status;
  unlocked_state = source.unlocked_state;
}

/*
 * Description: Returns the pair association of the base and instance event set
 *              in a pair vector. If no base is set, the pointer pair is
 *              identical. Used within the getEvent() main call.
 *
 * Inputs: none
 * Output: std::vector<std::pair<Event*,Event*>> - the returned set of pairs
 */
std::vector<std::pair<Event*,Event*>> EventSet::getUnlockedPair()
{
  std::vector<std::pair<Event*,Event*>> pair_set;

  /* Get the events */
  std::vector<Event*> base_set = getEventUnlockedRef();
  std::vector<Event*> instance_set = getEventUnlockedRef(true);

  /* Combine */
  if(base_set.size() == instance_set.size())
  {
    for(uint32_t i = 0; i < base_set.size(); i++)
    {
      pair_set.push_back(
                   std::pair<Event*,Event*>(base_set[i], instance_set[i]));
    }
  }

  return pair_set;
}

/*
 * Description: Returns the unlocked event reference based on the index. If
 *              create is true, it will create the unlocked and all missing in
 *              the stack before it. If create is false, and the index is out
 *              of the existing range, it will return null.
 *
 * Inputs: int index - the index of the event to find (0+ valid)
 *         bool create - true to create events, if the index is out of range.
 *                       Default true.
 * Output: Event* - the found event pointer
 */
Event* EventSet::getUnlockedRef(int index, bool create)
{
  Event* found_event = nullptr;

  if(index >= 0)
  {
    /* Create if doesn't exist */
    if(create)
    {
      while(events_unlocked.size() <= (uint32_t)index)
        events_unlocked.push_back(createBlankEvent());
    }

    /* Find the reference */
    if((uint32_t)index < events_unlocked.size())
      found_event = &events_unlocked[index];
  }

  return found_event;
}

/*
 * Description: Sets up the required data ffor the base. Called when a base is
 *              set and not equal to null
 *
 * Inputs: none
 * Output: none
 */
void EventSet::setupForBase()
{
  /* Locked State - just copies base data */
  locked_status = base->getLockedState();

  /* Locked Event - ensures empty event */
  event_locked = copyEvent(base->event_locked, true);

  /* Unlocked Event - ensures empty event with the correct stack count */
  int event_count = base->getEventUnlocked().size();
  for(int i = 0; i < event_count; i++)
    events_unlocked.push_back(copyEvent(base->events_unlocked[i], true));

  /* Reset get index */
  get_index = -1;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds an event to the unlocked stack of events that can be used.
 *
 * Inputs: Event new_event - the new event to add to the unlocked stack
 * Output: bool - true if event was added
 */
bool EventSet::addEventUnlocked(Event new_event)
{
  if(!isBaseSet())
  {
    events_unlocked.push_back(new_event);
    return true;
  }
  return false;
}

/*
 * Description: Clears all the event data from the class and unlocks and removes
 *              all lock information.
 *
 * Inputs: none
 * Output: none
 */
void EventSet::clear()
{
  base = nullptr;
  get_index = -1;
  unlocked_state = UnlockedState::ORDERED;

  unsetEventLocked();
  unsetEventUnlocked();
  unsetLocked();
}

/*
 * Description: Returns the base set reference. If null, there is no base event
 *              set.
 *
 * Inputs: none
 * Output: EventSet* - the base reference
 */
EventSet* EventSet::getBase()
{
  return base;
}

/*
 * Description: Returns the event when accessed which depends on the locked or
 *              unlocked status, and if unlocked depends on the UnlockedState.
 *              If trigger is set to true, it will identify this get event
 *              will be triggering the returned event.
 *
 * Inputs: bool trigger - true if the returned event will be passed to the
 *         EventHandler. Default to true.
 * Output: Event - the returned event at the forefront of the get access
 */
Event EventSet::getEvent(bool trigger)
{
  Event* event_ref = getEventRef(trigger);
  if(event_ref != nullptr)
    return *event_ref;
  return createBlankEvent();
}

/*
 * Description: Returns the event reference when accessed which depends on the
 *              locked or unlocked status, and if unlocked depends on the
 *              UnlockedState. If trigger is set to true, it will identify this
 *              get event will be triggering the returned event.
 *
 * Inputs: bool trigger - true if the returned event ref will be passed to the
 *         EventHandler. Default to true.
 * Output: Event* - the returned event ref at the forefront of the get access
 */
EventPair EventSet::getEventPair(bool trigger)
{
  Event* found_base = nullptr;
  Event* found_inst = nullptr;
  EventPair pair{nullptr, nullptr};

  /* -- If locked, use locked event -- */
  if(isLocked())
  {
    Event* lock_base = getEventLockedRef();
    Event* lock_inst = getEventLockedRef(true);

    if(!lock_base->one_shot ||
       (lock_base->one_shot && !lock_inst->has_exec))
    {
      found_base = lock_base;
      found_inst = lock_inst;
    }
  }
  /* -- Otherwise, unlocked -- */
  else if(getUnlockedState() != UnlockedState::NONE &&
          events_unlocked.size() > 0)
  {
    std::vector<std::pair<Event*,Event*>> unlock_set = getUnlockedPair();

    /* Determine valid events */
    std::vector<std::pair<Event*,Event*>> valid_set;
    for(uint32_t i = 0; i < unlock_set.size(); i++)
    {
      if(!unlock_set[i].first->one_shot ||
         (unlock_set[i].first->one_shot && !unlock_set[i].second->has_exec))
      {
        valid_set.push_back(unlock_set[i]);
      }
    }

    /* Determine event */
    if(valid_set.size() > 0)
    {
      /* -- Ordered Selection -- */
      if(getUnlockedState() == UnlockedState::ORDERED)
      {
        bool found = false;

        /* Initial index setting */
        uint32_t ref_index = 0;
        uint32_t start_index = 0;
        if(get_index >= 0)
          start_index = get_index + 1;
        if(start_index >= unlock_set.size())
          start_index = 0;

        /* Loop through all available and try to find an event */
        for(uint32_t i = 0; !found && i < unlock_set.size(); i++)
        {
          /* Reference index */
          ref_index = start_index + i;
          if(ref_index >= unlock_set.size())
            ref_index -= unlock_set.size();

          /* Check event */
          if(!unlock_set[ref_index].first->one_shot ||
             (unlock_set[ref_index].first->one_shot &&
              !unlock_set[ref_index].second->has_exec))
          {
            found = true;
            found_base = unlock_set[ref_index].first;
            found_inst = unlock_set[ref_index].second;
          }
        }

        /* Take the start index and save it to the get index */
        if(found && trigger)
          get_index = ref_index;
      }
      /* -- Random Selection -- */
      else if(getUnlockedState() == UnlockedState::RANDOM)
      {
        int index = Helpers::randInt(valid_set.size() - 1);
        if(get_index < 0)
          index = 0;

        /* Get found event */
        found_base = valid_set[index].first;
        found_inst = valid_set[index].second;

        /* If this is first call, set get_index */
        if(get_index < 0 && trigger)
          get_index = 0;
      }
    }
  }

  /* Execution complete - trigger unlock used (which locks if unlocked and
   * the lock is not set to permanent) */
  if(trigger)
  {
    unlockUsed(locked_status);
  }

  /* Trigger and return */
  if(found_base != nullptr && found_inst != nullptr)
  {
    if(trigger)
      found_inst->has_exec = true;

    pair.base = found_base;
    pair.inst = found_inst;
  }
  return pair;
}

/*
 * Description: Returns the event reference when accessed which depends on the
 *              locked or unlocked status, and if unlocked depends on the
 *              UnlockedState. If trigger is set to true, it will identify this
 *              get event will be triggering the returned event.
 *
 * Inputs: bool trigger - true if the returned event ref will be passed to the
 *         EventHandler. Default to true.
 * Output: Event* - the returned event ref at the forefront of the get access
 */
Event* EventSet::getEventRef(bool trigger)
{
  EventPair pair = getEventPair(trigger);
  return pair.base;
}

/*
 * Description: Returns the event if the set is locked.
 *
 * Inputs: none
 * Output: Event - the event associated with the locked status
 */
Event EventSet::getEventLocked()
{
  /* Has Base */
  if(base != nullptr)
  {
    return base->getEventLocked();
  }
  /* Is a Base */
  else
  {
    return event_locked;
  }
}

/*
 * Description: Returns the event if the set is locked, but the ptr reference.
 *
 * Inputs: bool force_instance - choose instance even if base set
 * Output: Event* - the locked event reference pointer
 */
Event* EventSet::getEventLockedRef(bool force_instance)
{
  /* Has Base */
  if(base != nullptr && !force_instance)
  {
    return base->getEventLockedRef();
  }
  /* Is a Base */
  else
  {
    return &event_locked;
  }
}

/*
 * Description: Returns the stack of events if the set is unlocked.
 *
 * Inputs: none
 * Output: std::vector<Event> - the event(s) associated with the unlocked status
 */
std::vector<Event> EventSet::getEventUnlocked()
{
  /* Has Base */
  if(base != nullptr)
  {
    return base->getEventUnlocked();
  }
  /* Is a Base */
  else
  {
    return events_unlocked;
  }
}

/*
 * Description: Returns the event at the given index in the unlocked stack.
 *
 * Inputs: int index - the index in the unlocked stack
 * Output: Event - the event associated with the index in the unlocked stack.
 *                 It will return a blank event if the index is out of range
 */
Event EventSet::getEventUnlocked(int index)
{
  /* Has Base */
  if(base != nullptr)
  {
    return base->getEventUnlocked(index);
  }
  /* Is a Base */
  else
  {
    if(index >= 0 && (uint32_t)index < events_unlocked.size())
      return events_unlocked[index];
    return createBlankEvent();
  }
}

/*
 * Description: Returns the stack of events if the set is unlocked, but the
 *              reference pointers.
 *
 * Inputs: bool force_instance - choose instance even if base set
 * Output: std::vector<Event*> - the event(s) associated with the unlocked
 */
std::vector<Event*> EventSet::getEventUnlockedRef(bool force_instance)
{
  std::vector<Event*> set;

  /* Has Base */
  if(base != nullptr && !force_instance)
  {
    return base->getEventUnlockedRef();
  }
  /* Is a Base */
  else
  {
    for(uint32_t i = 0; i < events_unlocked.size(); i++)
      set.push_back(&events_unlocked[i]);
  }

  return set;
}

/*
 * Desciption: Returns the locked state struct. This defines the locked
 *             properties of the event set.
 *
 * Inputs: none
 * Output: Locked - the locked state struct. See 'EventSet.h' for description
 */
Locked EventSet::getLockedState()
{
  return locked_status;
}

/*
 * Description: Returns the unlocked state of the set. This defines how the
 *              unlocked stack of events is accessed and handled during a get
 *              call.
 *
 * Inputs: none
 * Output: UnlockedState - the unlocked state enum of the set.
 */
UnlockedState EventSet::getUnlockedState()
{
  if(base != nullptr)
    return base->getUnlockedState();
  return unlocked_state;
}

/*
 * Description: Returns if there is a base event set that this event set is
 *              using.
 *
 * Inputs: none
 * Output: bool - true if this event set has a base reference set
 */
bool EventSet::isBaseSet()
{
  return (base != nullptr);
}

/*
 * Description: Checks to see if there is any data to save. If so, return true
 *
 * Inputs: none
 * Output: bool - true if there is data to save by saveData(*)
 */
bool EventSet::isDataToSave()
{
  /* Locked status */
  if(isDataToSave(locked_status))
    return true;

  /* Lock event */
  if(locked_status.state != LockedState::NONE && isDataToSave(event_locked))
    return true;

  /* Unlock event(s) */
  for(uint32_t i = 0; i < events_unlocked.size(); i++)
    if(isDataToSave(events_unlocked[i]))
      return true;

  /* Otherwise, return false */
  return false;
}

/*
 * Description: Returns if the event set has any events or data set. If true,
 *              there are no events. It will always return true after a clear()
 *              call.
 *
 * Inputs: none
 * Output: bool - true if the event configuration is empty
 */
bool EventSet::isEmpty()
{
  /* If base, returns the base call */
  if(base != nullptr)
    return base->isEmpty();

  /* Otherwise, the instance version */
  return (event_locked.classification == EventClassifier::NOEVENT &&
          events_unlocked.size() == 0 &&
          locked_status.state == LockedState::NONE);
}

/*
 * Description: Returns if the current state of the set is locked. If true,
 *              during a get, the locked event will be returned. Otherwise,
 *              the unlocked stack will be accessed.
 *
 * Inputs: none
 * Output: bool - true if the set is currently locked.
 */
bool EventSet::isLocked()
{
  return isLocked(locked_status);
}

/*
 * Description: Returns if there is no interaction if triggering the event set
 *              within the handler. This takes into account if one shots have
 *              been used as opposed to isEmpty() which just checks if data
 *              exists
 *
 * Inputs: none
 * Output: bool - true if no interaction within
 */
bool EventSet::isNoInteraction()
{
  bool no_interact = isEmpty();

  /* First check to ensure it is not empty */
  if(!no_interact)
  {
    bool valid_event = false;

    /* Locked Events */
    Event* lock_base = getEventLockedRef();
    Event* lock_inst = getEventLockedRef(true);
    valid_event |= (lock_base->classification != EventClassifier::NOEVENT &&
                    !(lock_base->one_shot && lock_inst->has_exec));

    /* Unlocked Events */
    std::vector<Event*> unlock_base = getEventUnlockedRef();
    std::vector<Event*> unlock_inst = getEventUnlockedRef(true);
    for(uint32_t i = 0; i < unlock_base.size(); i++)
      valid_event |=
               (unlock_base[i]->classification != EventClassifier::NOEVENT &&
                !(unlock_base[i]->one_shot && unlock_inst[i]->has_exec));

    /* Finally certify */
    no_interact = !valid_event;
  }
  return no_interact;
}

/*
 * Description: Loads the data from file associated with the event set.
 *
 * Inputs: XmlData data - the xml data structure
 *         int file_index - the element reference index
 *         int section_index - the map section where the event is defined
 * Output: bool - true if load was successful
 */
bool EventSet::loadData(XmlData data, int file_index, int section_index)
{
  std::string category = data.getElement(file_index);
  std::string back_element = data.getAllElements().back();

  /* If base is set, unset */
  if(isBaseSet() && back_element != "executed" && back_element != "locked")
    setBase(nullptr);

  /* Parse */
  /* -- Lock Information -- */
  if(category == "lock")
  {
    locked_status = updateLocked(locked_status, data, file_index + 1);
  }
  /* -- Locked status -- */
  else if(category == "locked")
  {
    locked_status = updateLocked(locked_status, data, file_index);
  }
  /* -- Lock Event -- */
  else if(category == "lockevent")
  {
    event_locked = updateEvent(event_locked, data, file_index + 1,
                               section_index);
  }
  /* -- Unlock Event(s) -- */
  else if(category == "unlockevent" && !data.getKeyValue(file_index).empty())
  {
    int index = std::stoi(data.getKeyValue(file_index));
    Event* found = getUnlockedRef(index);
    if(found != nullptr)
    {
      *found = updateEvent(*found, data, file_index + 1, section_index);
    }
  }
  /* -- Unlock Index Property -- */
  else if(category == "unlockindex")
  {
    bool success;
    int int_data = data.getDataInteger(&success);
    if(success && int_data >= 0 && int_data < (int)events_unlocked.size())
      get_index = int_data;
  }
  /* -- Unlock Parse Property -- */
  else if(category == "unlockparse")
  {
    std::string def = data.getDataString();

    if(def == "none")
      unlocked_state = UnlockedState::NONE;
    else if(def == "ordered")
      unlocked_state = UnlockedState::ORDERED;
    else if(def == "random")
      unlocked_state = UnlockedState::RANDOM;
  }
  /* -- COMBATIBILITY: For old system -- */
  else if(category == "event" || category == "tileevent" ||
          category == "useevent" || category == "enterevent" ||
          category == "exitevent" || category == "walkoverevent" ||
          category == "x" || category == "y")
  {
    int index = 0; /* First event always acts as old system reference */
    Event* found = getUnlockedRef(index);
    if(found != nullptr)
    {
      *found = updateEvent(*found, data, file_index + 1, section_index);
    }
  }

  return true;
}

/*
 * Description: Saves the data of this event set to the file handler pointer.
 *              This includes a wrapper. If blank, no wrap will be added
 *
 * Inputs: FileHandler* fh - the saving file handling pointer
 *         std::string wrapper - the wrapper xml text for the data
 * Output: bool - true if successful and there was data to save
 */
bool EventSet::saveData(FileHandler* fh, std::string wrapper)
{
  if(fh != nullptr)
  {
    /* Accumulate save status' */
    bool lock_valid = isDataToSave(locked_status);
    bool lock_event_valid = (locked_status.state != LockedState::NONE &&
                            isDataToSave(event_locked));
    bool unlock_event_valid = false;
    std::vector<bool> unlock_events_valid;
    for(uint32_t i = 0; i < events_unlocked.size(); i++)
    {
      if(isDataToSave(events_unlocked[i]))
      {
        unlock_event_valid = true;
        unlock_events_valid.push_back(true);
      }
      else
      {
        unlock_events_valid.push_back(false);
      }
    }

    /* Check if save will occur */
    if(lock_valid || lock_event_valid || unlock_event_valid)
    {
      /* Wrapper */
      if(!wrapper.empty())
        fh->writeXmlElement(wrapper);

      /* Locked status */
      if(lock_valid)
        saveLocked(fh, locked_status);

      /* Lock event */
      if(lock_event_valid)
        saveEvent(fh, event_locked, "lockevent", false);

      /* Unlock event index */
      if(get_index >= 0 && events_unlocked.size() > 1)
        fh->writeXmlData("unlockindex", get_index);

      /* Unlock event(s) */
      for(uint32_t i = 0; i < events_unlocked.size(); i++)
      {
        if(unlock_events_valid[i])
        {
          fh->writeXmlElement("unlockevent", "id", i);
          saveEvent(fh, events_unlocked[i], "", false);
          fh->writeXmlElementEnd();
        }
      }

      /* End Wrapper */
      if(!wrapper.empty())
        fh->writeXmlElementEnd();

      return true;
    }
  }
  return false;
}

/*
 * Description: Sets the base set reference. Any change here clears out all
 *              properties stored within the class. If set to null, this set
 *              becomes a base event set
 *
 * Inputs: EventSet* new_base - the new base event set reference
 * Output: none
 */
void EventSet::setBase(EventSet* new_base)
{
  if(base != new_base)
  {
    /* Clear data: also unsets base within clear call */
    clear();

    /* Set the base */
    base = new_base;
    if(base != nullptr)
      setupForBase();
  }
}

/*
 * Description: Sets the event to be used while the set is locked. Once set,
 *              the class takes control of any memory management.
 *
 * Inputs: Event new_event - the new event to use in a locked condition
 * Output: bool - true if the locked event was set
 */
bool EventSet::setEventLocked(Event new_event)
{
  if(!isBaseSet())
  {
    unsetEventLocked();
    event_locked = new_event;
    return true;
  }
  return false;
}

/*
 * Description: Sets the event to be used within the unlocked stack set at the
 *              given index. If replace is true, will delete the event, at the
 *              index if it exists. Otherwise, it will push index+ back one and
 *              insert the new event. If the index is greater than the size of
 *              the array, it will push the event to the back of the stack. Once
 *              set, this class takes control of any memory management.
 *
 * Inputs: int index - the index in the unlocked stack to set the event
 *         Event new_event - the new event to insert or set
 *         bool replace - true to replace at index. false to insert. Default
 *                        false
 * Output: bool - true if the insert or set was successful
 */
bool EventSet::setEventUnlocked(int index, Event new_event, bool replace)
{
  bool success = false;

  if(!isBaseSet() && index >= 0)
  {
    uint32_t uindex = (uint32_t)index;

    /* -- Replace existing -- */
    if(replace)
    {
      /* If in range, find and replace */
      if(uindex < events_unlocked.size())
      {
        deleteEvent(events_unlocked[uindex]);
        events_unlocked[uindex] = new_event;
      }
      /* Otherwise, append */
      else
      {
        events_unlocked.push_back(new_event);
      }
    }
    /* -- Insert only -- */
    else
    {

      /* If in range, insert */
      if(uindex < events_unlocked.size())
      {
        events_unlocked.insert(events_unlocked.begin() + uindex, new_event);
      }
      /* Otherwise, append */
      else
      {
        events_unlocked.push_back(new_event);
      }
    }

    success = true;
  }

  return success;
}

/*
 * Description: Defines the locked information to use within the set. This
 *              replaces any existing locked information with no saving of the
 *              current running conditions (if set while the class is in
 *              operation).
 *
 * Inputs: Locked new_locked - the new locked struct with lock information
 * Output: bool - true if the locked struct was set
 */
bool EventSet::setLocked(Locked new_locked)
{
//  if(!isBaseSet())
//  {
  locked_status = new_locked;
  return true;
//  }
//  return false;
}

/*
 * Description: Defines the unlocked enum which defines how the class accesses
 *              and utilizes the unlocked stack of events.
 *
 * Inputs: UnlockedState state - the state enumerator to use
 * Output: bool - true if the unlocked state was set
 */
bool EventSet::setUnlockedState(UnlockedState state)
{
  if(!isBaseSet())
  {
    unlocked_state = state;
    return true;
  }
  return false;
}

/*
 * Description: Attempts an unlock on the locked state stored within the set
 *              with a trigger event.
 *
 * Inputs: none
 * Output: bool - true if was locked and now unlocked
 */
bool EventSet::unlockTrigger()
{
  if(isLocked())
  {
    unlockTrigger(locked_status);
    return !locked_status.is_locked;
  }
  return false;
}

/*
 * Description: Unsets the locked event and replaces it with a blank one.
 *
 * Inputs: none
 * Output: bool - true if the locked event was unset. Only fails if base set
 */
bool EventSet::unsetEventLocked()
{
  //if(!isBaseSet())
  //{
  event_locked = deleteEvent(event_locked);
  return true;
  //}
  //return false;
}

/*
 * Description: Unsets the unlocked event at the given index. Once the index is
 *              removed, its position is removed from the stack.
 *
 * Inputs: int index - the index in the unlocked stack to remove. If out of
 *                     range, nothing occurs
 * Output: bool - true if the unlocked event was unset. Fails if out of range or
 *                base is set
 */
bool EventSet::unsetEventUnlocked(int index)
{
  //if(!isBaseSet() && index >= 0 && (uint32_t)index < events_unlocked.size())
  if(index >= 0 && (uint32_t)index < events_unlocked.size())
  {
    Event delete_event = events_unlocked[index];
    events_unlocked.erase(events_unlocked.begin() + index);
    deleteEvent(delete_event);
    return true;
  }
  return false;
}

/*
 * Description: Unsets all unlocked events from the stack and clears out the
 *              stack.
 *
 * Inputs: none
 * Output: bool - true if the unlocked events were unset. Only fails if base set
 */
bool EventSet::unsetEventUnlocked()
{
  //if(!isBaseSet())
  //{
  for(uint16_t i = 0; i < events_unlocked.size(); i++)
    deleteEvent(events_unlocked[i]);
  events_unlocked.clear();
  return true;
  //}
  //return false;
}

/*
 * Description: Unsets the locked struct information and replaces it with a
 *              blank permanently unlocked version.
 *
 * Inputs: none
 * Output: bool - true if the locked struct was unset. Only fails if base set
 */
bool EventSet::unsetLocked()
{
  if(!isBaseSet())
  {
    locked_status = createBlankLocked();
    return true;
  }
  return false;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EventSet.
 *
 * Inputs: const EventSet& source - the source class constructor
 * Output: EventSet& - pointer to the copied class
 */
EventSet& EventSet::operator=(const EventSet& source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the enum form a given EventClassifier string
 *
 * Inputs: std::string classifier - the string version of the classifier
 * Output: EventClassifier - enum of classifier corresponding to string
 */
EventClassifier EventSet::classifierFromStr(const std::string& classifier)
{
  std::string class_up = classifier;
  std::transform(class_up.begin(), class_up.end(), class_up.begin(), toupper);

  /* Parse */
  if(class_up == "BATTLE START")
    return EventClassifier::BATTLESTART;
  else if(class_up == "CONVERSATION")
    return EventClassifier::CONVERSATION;
  else if(class_up == "ITEM: GIVE")
    return EventClassifier::ITEMGIVE;
  else if(class_up == "ITEM: TAKE")
    return EventClassifier::ITEMTAKE;
  else if(class_up == "MAP SWITCH")
    return EventClassifier::MAPSWITCH;
  else if(class_up == "MULTIPLE")
    return EventClassifier::MULTIPLE;
  else if(class_up == "NOTIFICATION")
    return EventClassifier::NOTIFICATION;
  else if(class_up == "PROPERTY MOD")
    return EventClassifier::PROPERTY;
  else if(class_up == "SOUND ONLY")
    return EventClassifier::SOUNDONLY;
  else if(class_up == "TELEPORT THING")
    return EventClassifier::TELEPORTTHING;
  else if(class_up == "UNLOCK: IO")
    return EventClassifier::UNLOCKIO;
  else if(class_up == "UNLOCK: THING")
    return EventClassifier::UNLOCKTHING;
  else if(class_up == "UNLOCK: TILE")
    return EventClassifier::UNLOCKTILE;
  return EventClassifier::NOEVENT;
}

/*
 * Description: Returns the string form a given EventClassifier enum
 *
 * Inputs: EventClassifier classifier - enum of the event classification
 * Output: std::string - string corresponding to the EventClassifier
 */
std::string EventSet::classifierToStr(const EventClassifier& classifier)
{
  if(classifier == EventClassifier::BATTLESTART)
    return "Battle Start";
  else if(classifier == EventClassifier::CONVERSATION)
    return "Conversation";
  else if(classifier == EventClassifier::ITEMGIVE)
    return "Item: Give";
  else if(classifier == EventClassifier::ITEMTAKE)
    return "Item: Take";
  else if(classifier == EventClassifier::MAPSWITCH)
    return "Map Switch";
  else if(classifier == EventClassifier::MULTIPLE)
    return "Multiple";
  else if(classifier == EventClassifier::NOTIFICATION)
    return "Notification";
  else if(classifier == EventClassifier::PROPERTY)
    return "Property Mod";
  else if(classifier == EventClassifier::SOUNDONLY)
    return "Sound Only";
  else if(classifier == EventClassifier::TELEPORTTHING)
    return "Teleport Thing";
  else if(classifier == EventClassifier::UNLOCKIO)
    return "Unlock: IO";
  else if(classifier == EventClassifier::UNLOCKTHING)
    return "Unlock: Thing";
  else if(classifier == EventClassifier::UNLOCKTILE)
    return "Unlock: Tile";
  return "None";
}

/* Copies a passed in conversation struct */
// TODO: Comment
Conversation EventSet::copyConversation(Conversation source, bool skeleton)
{
  /* Data */
  Conversation convo = createBlankConversation();
  convo.text = source.text;
  convo.thing_id = source.thing_id;
  convo.category = source.category;
  convo.action_event = copyEvent(source.action_event, skeleton);

  /* Go through all other conversations */
  for(uint32_t i = 0; i < source.next.size(); i++)
    convo.next.push_back(copyConversation(source.next[i]));

  return convo;
}

/*
 * Description: Public static. Copies a past in event and returns the copied
 *              version as source. Function that calls it is in charge of
 *              deleting conversation, if that's been generated.
 *
 * Inputs: Event source - the event struct to copy
 *         bool skeleton - true to only create a copy of source event as a
 *                         skeleton. Used for instance based triggers
 * Output: Event - the copied event
 */
Event EventSet::copyEvent(Event source, bool skeleton)
{
  /* Copy the event */
  Event event = createBlankEvent();
  event.classification = source.classification;
  event.convo = nullptr;
  event.one_shot = source.one_shot;
  event.has_exec = false;
  //if(!skeleton) // TODO: FUTURE
  //{
  event.ints = source.ints;
  event.sound_id = source.sound_id;
  event.strings = source.strings;
  //}

  /* If events, do the proper copy */
  for(uint32_t i = 0; i < source.events.size(); i++)
  {
    event.events.push_back(copyEvent(source.events[i], skeleton));
  }

  /* If convo, do the proper copy */
  if(event.classification == EventClassifier::CONVERSATION &&
     source.convo != nullptr)
  {
    event.convo = new Conversation;
    event.convo->action_event = copyEvent(source.convo->action_event,
                                          skeleton);
    event.convo->category = source.convo->category;
    event.convo->text = source.convo->text;
    event.convo->thing_id = source.convo->thing_id;

    for(uint32_t i = 0; i < source.convo->next.size(); i++)
      event.convo->next.push_back(copyConversation(source.convo->next[i],
                                                   skeleton));

    /* If skeleton mode, purge conversation */
    // TODO: FUTURE
    //if(skeleton)
    //  std::cout << "TODO: PURGE" << std::endl;
  }

  return event;
}

/*
 * Description: Public static. Creates a blank conversation entry.
 *
 * Inputs: none
 * Output: Conversation - the blank conversation struct to insert
 */
Conversation EventSet::createBlankConversation()
{
  Conversation convo;
  convo.text = "";
  convo.thing_id = kUNSET_ID;
  convo.category = DialogCategory::TEXT;
  convo.action_event = createBlankEvent();
  convo.next.clear();

  return convo;
}

/*
 * Description: Public static. Creates a blank event with no defined event.
 *
 * Inputs: none
 * Output: Event - the blank event struct to utilize
 */
Event EventSet::createBlankEvent()
{
  Event blank_event;

  blank_event.classification = EventClassifier::NOEVENT;
  blank_event.convo = nullptr;
  blank_event.events.clear();
  blank_event.ints.clear();
  blank_event.sound_id = kUNSET_ID;
  blank_event.strings.clear();

  blank_event.one_shot = false;
  blank_event.has_exec = false;

  return blank_event;
}

/*
 * Description: Public static. Creates a blank locked struct with no lock and
 *              permanently maintains that state.
 *
 * Inputs: none
 * Output: Locked - the blank locked struct to utilize
 */
Locked EventSet::createBlankLocked()
{
  Locked blank_locked;

  blank_locked.state = LockedState::NONE;
  blank_locked.is_locked = false;
  blank_locked.permanent = true;
  blank_locked.bools.clear();
  blank_locked.ints.clear();

  return blank_locked;
}

/*
 * Description: Public static. Creates the BattleFlags enum using the 4
 *              boolean options.
 *
 * Inputs: bool win_disappear - true if on win, the initiating thing disappears
 *         bool lose_gg - true if on lose, the game is over
 *         bool restore_health - true if on battle end, health is restored
 *         bool restore_qd - true if on battle end, qd is restored
 * Output: BattleFlags - the battle flags enum from the input booleans
 */
BattleFlags EventSet::createEnumBattleFlags(bool win_disappear, bool lose_gg,
                                            bool restore_health,
                                            bool restore_qd)
{
  int flags_enum = 0;

  /* Parse bools */
  if(win_disappear)
    flags_enum |= static_cast<int>(BattleFlags::ONWINDISAPPEAR);
  if(lose_gg)
    flags_enum |= static_cast<int>(BattleFlags::ONLOSEENDGAME);
  if(restore_health)
    flags_enum |= static_cast<int>(BattleFlags::RESTOREHEALTH);
  if(restore_qd)
    flags_enum |= static_cast<int>(BattleFlags::RESTOREQD);

  /* Return the created enum */
  return static_cast<BattleFlags>(flags_enum);
}

/*
 * Description: Public static. Creates the GiveItemFlags enum using the 1
 *              boolean option.
 *
 * Inputs: bool auto_drop - true if the give should automatically drop
 * Output: GiveItemFlags - the enum, as per the inputs
 */
GiveItemFlags EventSet::createEnumGiveFlags(bool auto_drop)
{
  int flags_enum = 0;

  /* Parse bools */
  if(auto_drop)
    flags_enum |= static_cast<int>(GiveItemFlags::AUTODROP);

  /* Return the created enum */
  return static_cast<GiveItemFlags>(flags_enum);
}

/*
 * Description: Public static. Creates the UnlockIOEvent enum using the 4
 *              boolean options.
 *
 * Inputs: bool enter - true if the enter event should be unlocked
 *         bool exit - true if the exit event should be unlocked
 *         bool use - true if the use event should be unlocked
 *         bool walkover - true if the walkover event should be unlocked
 * Output: UnlockIOEvent - the enum, as per the inputs
 */
UnlockIOEvent EventSet::createEnumIOEvent(bool enter, bool exit,
                                          bool use, bool walkover)
{
  int io_event_enum = 0;

  /* Parse bools */
  if(enter)
    io_event_enum |= static_cast<int>(UnlockIOEvent::ENTER);
  if(exit)
    io_event_enum |= static_cast<int>(UnlockIOEvent::EXIT);
  if(use)
    io_event_enum |= static_cast<int>(UnlockIOEvent::USE);
  if(walkover)
    io_event_enum |= static_cast<int>(UnlockIOEvent::WALKOVER);

  /* Return the created enum */
  return static_cast<UnlockIOEvent>(io_event_enum);
}

/*
 * Description: Public static. Creates the UnlockIOMode enum using the 2
 *              boolean options.
 *
 * Inputs: bool lock - true if the main state lock should be unlocked in IO
 *         bool events - true if the state related events should be unlocked
 * Output: UnlockIOMode - the enum, as per the inputs
 */
UnlockIOMode EventSet::createEnumIOMode(bool lock, bool events)
{
  int io_mode_enum = 0;

  /* Parse bools */
  if(lock)
    io_mode_enum |= static_cast<int>(UnlockIOMode::LOCK);
  if(events)
    io_mode_enum |= static_cast<int>(UnlockIOMode::EVENTS);

  /* Return the created enum */
  return static_cast<UnlockIOMode>(io_mode_enum);
}

/*
 * Description: Public static. Creates the ThingProperty enum using the
 *              numerous boolean options.
 *
 * Inputs: bool active - modify the thing active bit (thing+)
 *         bool forced - modify the thing if interaction is forced (npc)
 *         bool inactive - modify the thing inactive time (io)
 *         bool move - modify the thing if it is able to move (person+)
 *         bool reset - modify the thing back to its original location (person+)
 *         bool respawn - modify the thing respawn time (thing+)
 *         bool speed - modify the thing speed (person+)
 *         bool track - modify the thing tracking (npc)
 *         bool visible - modify if the thing is visible (thing+)
 * Output: ThingProperty - the resulting enum from the combination of bits
 */
ThingProperty EventSet::createEnumProperties(bool active, bool forced,
                           bool inactive, bool move, bool reset, bool respawn,
                           bool speed, bool track, bool visible)
{
  int thing_prop_enum = 0;

  /* Parse bools */
  if(active)
    thing_prop_enum |= static_cast<int>(ThingProperty::ACTIVE);
  if(forced)
    thing_prop_enum |= static_cast<int>(ThingProperty::FORCED_INTERACT);
  if(inactive)
    thing_prop_enum |= static_cast<int>(ThingProperty::INACTIVE_TIME);
  if(move)
    thing_prop_enum |= static_cast<int>(ThingProperty::MOVE_DISABLE);
  if(reset)
    thing_prop_enum |= static_cast<int>(ThingProperty::RESET_LOCATION);
  if(respawn)
    thing_prop_enum |= static_cast<int>(ThingProperty::RESPAWN_TIME);
  if(speed)
    thing_prop_enum |= static_cast<int>(ThingProperty::SPEED);
  if(track)
    thing_prop_enum |= static_cast<int>(ThingProperty::TRACKING);
  if(visible)
    thing_prop_enum |= static_cast<int>(ThingProperty::VISIBLE);

  /* Return the created enum */
  return static_cast<ThingProperty>(thing_prop_enum);
}

/*
 * Description: Public static. Creates the UnlockTileMode enum using the 2
 *              boolean options.
 *
 * Inputs: bool enter - true if the enter event should be unlocked
 *         bool exit - true if the exit event should be unlocked
 * Output: UnlockTileMode - the enum, as per the inputs
 */
UnlockTileMode EventSet::createEnumTileEvent(bool enter, bool exit)
{
  int tile_event_enum = 0;

  /* Parse bools */
  if(enter)
    tile_event_enum |= static_cast<int>(UnlockTileMode::ENTER);
  if(exit)
    tile_event_enum |= static_cast<int>(UnlockTileMode::EXIT);

  /* Return the created enum */
  return static_cast<UnlockTileMode>(tile_event_enum);
}

/*
 * Description: Public static. Creates the UnlockView enum using the 2 boolean
 *              options.
 *
 * Inputs: bool view - true if the unlock point should be viewed
 *         bool scroll - true if the view should scroll to the location
 * Output: UnlockView - the enum, as per the inputs
 */
UnlockView EventSet::createEnumView(bool view, bool scroll)
{
  int view_enum = 0;

  /* Parse bools */
  if(view)
    view_enum |= static_cast<int>(UnlockView::GOTO);
  if(scroll)
    view_enum |= static_cast<int>(UnlockView::SCROLL);

  /* Return the created enum */
  return static_cast<UnlockView>(view_enum);
}

/*
 * Description: Public static. Creates a new conversation event with the passed
 *              in conversation starting pointer (if null, one is created). The
 *              control of memory management for conversation is handled by the
 *              caller.
 *
 * Inputs: Conversation* new_conversation - the new conversation reference
 *         int sound_id - the sound reference to connect to the event. Default
 *                        to invalid
 * Output: Event - the conversation event to utilize
 */
Event EventSet::createEventConversation(Conversation* new_conversation,
                                        int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::CONVERSATION;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Use the existing conversation if it exists. Otherwise create new one */
  if(new_conversation != nullptr)
    new_event.convo = new_conversation;
  else
  {
    new_event.convo = new Conversation;
    new_event.convo->text = "";
    new_event.convo->thing_id = kUNSET_ID;
    new_event.convo->category = DialogCategory::TEXT;
    new_event.convo->action_event = createBlankEvent();
    new_event.convo->next.clear();
  }

  return new_event;
}

/*
 * Description: Public static. Creates a new give item event with the passed
 *              ID, item count, flags, and chance, with an optional connected
 *              sound ID.
 *
 * Inputs: int id - the give item reference ID
 *         int count - the give item reference count
 *         GiveItemFlags flags - the flags for give item control
 *         int chance - the percent chance (0-100) for the give item event
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the give item event to utilize
 */
Event EventSet::createEventGiveItem(int id, int count, GiveItemFlags flags,
                                    int chance, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::ITEMGIVE;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(id);
  new_event.ints.push_back(count);
  new_event.ints.push_back(static_cast<int>(flags));
  new_event.ints.push_back(chance);

  return new_event;
}

/*
 * Description: Public static. Creates a new multi event with the passed in set
 *              of sub events, and an optional connected sound ID.
 *
 * Inputs: std::vector<Event> event - set of events to trigger within the
 *                                    multiple. Default to blank
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the multi event to utilize
 */
Event EventSet::createEventMultiple(std::vector<Event> events, int sound_id)
{
  /* Create the new event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::MULTIPLE;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.events = events;

  return new_event;
}

/*
 * Description: Public static. Creates a new notification event with the passed
 *              notification string, and an optional connected sound ID.
 *
 * Inputs: std::string notification - the notification string for the event
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the notification event to utilize
 */
Event EventSet::createEventNotification(std::string notification,
                                        int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::NOTIFICATION;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Set up the rest of the event */
  new_event.strings.push_back(notification);

  return new_event;
}

/*
 * Description: Public static. Creates a new property modifier event with the
 *              passed in various properties (as defined by inputs) and an
 *              optional sound ID.
 *
 * Inputs: ThingBase type - the type of thing modified by the event
 *         int id - the id of the thing
 *         ThingProperty props - which properties are being modified by the call
 *         ThingProperty bools - the bool values of the properties being modded
 *         int respawn - the modified respawn time (thing+)
 *         int speed - the modified speed (person+)
 *         TrackingState track - the modification of what tracking (npc)
 *         int inactive - modify the time before going inactive (io)
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the notification event to utilize
 */
Event EventSet::createEventPropMod(ThingBase type, int id, ThingProperty props,
                              ThingProperty bools, int respawn, int speed,
                              TrackingState track, int inactive, int sound_id)
{
  /* Create the event and check validity */
  Event new_event = createBlankEvent();
  if(type != ThingBase::ISBASE)
  {
    /* Identify the event */
    new_event.classification = EventClassifier::PROPERTY;
    if(sound_id >= 0)
      new_event.sound_id = sound_id;

    /* Set up the rest of the event */
    new_event.ints.push_back(static_cast<int>(type));
    new_event.ints.push_back(id);
    new_event.ints.push_back(static_cast<int>(props));
    new_event.ints.push_back(static_cast<int>(bools));
    new_event.ints.push_back(respawn);
    new_event.ints.push_back(speed);
    new_event.ints.push_back(static_cast<int>(track));
    new_event.ints.push_back(inactive);
  }
  return new_event;
}

/*
 * Description: Public static. Creates a new sound only event with the passed
 *              sound ID.
 *
 * Inputs: int sound_id - the sound reference ID
 * Output: Event - the sound only event to utilize
 */
Event EventSet::createEventSound(int sound_id)
{
  /* Create the new event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::SOUNDONLY;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  return new_event;
}

/*
 * Description: Public static. Creates a new start battle event with an optional
 *              connected sound ID.
 *
 * Inputs: int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the start battle event to utilize
 */
Event EventSet::createEventStartBattle(int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::BATTLESTART;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(static_cast<int>(BattleFlags::NONE));
  new_event.events.push_back(createBlankEvent());
  new_event.events.push_back(createBlankEvent());

  return new_event;
}

/*
 * Description: Public static. Creates a new start battle event with an optional
 *              connected sound ID.
 *
 * Inputs: BattleFlags flags - the flags to define battle handling by game
 *         Event event_win - the event if won.
 *         Event event_lose - the event if lost.
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the start battle event to utilize
 */
Event EventSet::createEventStartBattle(BattleFlags flags, Event event_win,
                                       Event event_lose, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createEventStartBattle(sound_id);

  /* Fill in the event specific information */
  new_event.ints[kBATTLE_FLAGS] = static_cast<int>(flags);
  new_event.events[kBATTLE_EVENT_WIN] =
                            deleteEvent(new_event.events[kBATTLE_EVENT_WIN]);
  new_event.events[kBATTLE_EVENT_WIN] = copyEvent(event_win);
  new_event.events[kBATTLE_EVENT_LOSE] =
                            deleteEvent(new_event.events[kBATTLE_EVENT_LOSE]);
  new_event.events[kBATTLE_EVENT_LOSE] = copyEvent(event_lose);

  return new_event;
}

/*
 * Description: Public static. Creates a new map switch event with the map ID
 *              and an optional connected sound ID.
 *
 * Inputs: int id - the main map reference ID
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the switch map event to utilize
 */
Event EventSet::createEventStartMap(int id, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::MAPSWITCH;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(id);

  return new_event;
}

/*
 * Description: Public static. Creates the take item event with the item ID and
 *              itemc count, with an optional connected sound ID.
 *
 * Inputs: int id - the item ID
 *         int count - the item count
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the take item event to utilize
 */
Event EventSet::createEventTakeItem(int id, int count, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::ITEMTAKE;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(id);
  new_event.ints.push_back(count);

  return new_event;
}

/*
 * Description: Public static. Creates a new teleport thing event with the
 *              connected thing ID to teleport, the tile X and Y within the map
 *              as well as the sub-map ID to teleport, and an optional connected
 *              sound ID.
 *
 * Inputs: int thing_id - the thing instance ID to teleport
 *         uint16_t tile_x - the X tile location (horizontal)
 *         uint16_t tile_y - the Y tile location (vertical)
 *         int section_id - the section ID to teleport within the map
 *         uint16_t sound_id - the sound reference ID. Default to invalid
 * Output: Event - the teleport thing event to utilize
 */
Event EventSet::createEventTeleport(int thing_id, uint16_t tile_x,
                                    uint16_t tile_y, int section_id,
                                    int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::TELEPORTTHING;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(thing_id);
  new_event.ints.push_back(tile_x);
  new_event.ints.push_back(tile_y);
  new_event.ints.push_back(section_id);

  return new_event;
}

/*
 * Description: Public static. Creates a new unlock IO event with the connected
 *              IO ID to modify and various properties (as defined by inputs)
 *              and an optional sound ID.
 *
 * Inputs: int io_id - the IO instance ID to unlock
 *         UnlockIOMode mode - what part of the IO to unlock, events or main
 *         int state_num - the state within the IO to modify (events mode only)
 *         UnlockIOEvent events - what events to attempt unlock in the state
 *         UnlockView view_mode - the view mode for the unlocked location
 *         int view_time - the ms to view the tile for (depends on mode)
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the unlock IO event to utilize
 */
Event EventSet::createEventUnlockIO(int io_id, UnlockIOMode mode, int state_num,
                                    UnlockIOEvent events, UnlockView view_mode,
                                    int view_time, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::UNLOCKIO;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(io_id);
  new_event.ints.push_back(static_cast<int>(mode));
  new_event.ints.push_back(state_num);
  new_event.ints.push_back(static_cast<int>(events));
  new_event.ints.push_back(static_cast<int>(view_mode));
  new_event.ints.push_back(view_time);

  return new_event;
}

/*
 * Description: Public static. Creates a new unlock thing event with the
 *              connected thing ID, view mode, and an optional sound ID.
 *
 * Inputs: int thing_id - the thing instance ID to unlock
 *         UnlockView view_mode - the view mode for the unlocked location
 *         int view_time - the ms to view the tile for (depends on mode)
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the unlock thing event to utilize
 */
Event EventSet::createEventUnlockThing(int thing_id, UnlockView view_mode,
                                       int view_time, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::UNLOCKTHING;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(thing_id);
  new_event.ints.push_back(static_cast<int>(view_mode));
  new_event.ints.push_back(view_time);

  return new_event;
}

/*
 * Description: Public static. Creates a new unlock tile event with the
 *              connected tile location, mode, view mode and an optional
 *              sound ID.
 *
 * Inputs: int section_id - the map sub-section ID
 *         uint16_t tile_x - the x tile location
 *         uint16_t tile_y - the y tile location
 *         UnlockTileMode mode - which events within the tile to unlock
 *         UnlockView view_mode - the view mode for the unlocked location
 *         int view_time - the ms to view the tile for (depends on mode)
 *         int sound_id - the sound reference ID. Default to invalid
 * Output: Event - the unlock tile event to utilize
 */
Event EventSet::createEventUnlockTile(int section_id, uint16_t tile_x,
                                      uint16_t tile_y, UnlockTileMode mode,
                                      UnlockView view_mode, int view_time,
                                      int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::UNLOCKTILE;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(section_id);
  new_event.ints.push_back(tile_x);
  new_event.ints.push_back(tile_y);
  new_event.ints.push_back(static_cast<int>(mode));
  new_event.ints.push_back(static_cast<int>(view_mode));
  new_event.ints.push_back(view_time);

  return new_event;
}

/*
 * Description: Public static. Creates a lock structure which can be unlocked
 *              with an base item ID and count, and true if it is consumed. It
 *              also includes a permanent flag: if true, once unlocked, it will
 *              stay. Otherwise, after one get call, it re-locks.
 *
 * Inputs: int id - the item ID to check the count on
 *         int count - the number of said item IDs that must exist to unlock
 *         bool consume - true if it consumes the item on unlock. Default true
 *         bool permanent - true if the unlock is permanent. Default true
 * Output: Locked - structure with the lock properties. To be used in lock
 *                  situations, such as EventSet
 */
Locked EventSet::createLockHaveItem(int id, int count, bool consume,
                                    bool permanent)
{
  /* Create the new lock struct and identify */
  Locked new_locked = createBlankLocked();
  new_locked.state = LockedState::ITEM;
  new_locked.permanent = permanent;
  new_locked.is_locked = true;

  /* Fill in the locked state specific information */
  new_locked.bools.push_back(consume);
  new_locked.ints.push_back(id);
  new_locked.ints.push_back(count);

  return new_locked;
}

/*
 * Description: Public static. Creates a lock structure which can be unlocked
 *              only by a trigger event. It also includes a permanent flag: if
 *              true, once unlocked, it will stay. Otherwise, after one get
 *              call, it re-locks.
 *
 * Inputs: bool permanent - true if the unlock is permanent. Default true
 * Output: Locked - structure with the lock properties. To be used in lock
 *                  situations, such as EventSet
 */
Locked EventSet::createLockTriggered(bool permanent)
{
  /* Create the new lock struct and identify */
  Locked new_locked = createBlankLocked();
  new_locked.state = LockedState::TRIGGER;
  new_locked.permanent = permanent;
  new_locked.is_locked = true;

  return new_locked;
}

/*
 * Description: Public static. Extracts data from the BattleFlags enum, as
 *              defined by the inputs.
 *
 * Inputs: BattleFlags flags - the enum to extract bitwise data from
 *         bool win_disappear - the if win, thing should disappear flag
 *         bool lose_gg - the if lose, the game is over flag
 *         bool restore_health - the if battle over, restore health to full
 *         bool restore_qd - the if battle over, restore qd to full
 * Output: none
 */
void EventSet::dataEnumBattleFlags(BattleFlags flags, bool& win_disappear,
                                   bool& lose_gg, bool& restore_health,
                                   bool& restore_qd)
{
  int enum_int = static_cast<int>(flags);

  /* Extract data */
  win_disappear =
              ((enum_int & static_cast<int>(BattleFlags::ONWINDISAPPEAR)) > 0);
  lose_gg = ((enum_int & static_cast<int>(BattleFlags::ONLOSEENDGAME)) > 0);
  restore_health =
               ((enum_int & static_cast<int>(BattleFlags::RESTOREHEALTH)) > 0);
  restore_qd = ((enum_int & static_cast<int>(BattleFlags::RESTOREQD)) > 0);
}

/*
 * Description: Extracts data from the GiveItemFlags enum, as defined by the
 *              inputs.
 *
 * Inputs: GiveItemFlags flags - the enum to extract bitwise data from
 *         bool auto_drop - the item should be automatically dropped on map
 * Output: none
 */
void EventSet::dataEnumGiveFlags(GiveItemFlags flags, bool& auto_drop)
{
  int enum_int = static_cast<int>(flags);

  /* Extract data */
  auto_drop = ((enum_int & static_cast<int>(GiveItemFlags::AUTODROP)) > 0);
}

/*
 * Description: Extracts data from the UnlockIOEvent enum, as defined by the
 *              inputs.
 *
 * Inputs: UnlockIOEvent io_enum - the enum to extract bitwise data from
 *         bool enter - the enter IO event should be modified
 *         bool exit - the exit IO event should be modified
 *         bool use - the use IO event should be modified
 *         bool walkover - the walkover IO event should be modified
 * Output: none
 */
void EventSet::dataEnumIOEvent(UnlockIOEvent io_enum, bool& enter, bool& exit,
                               bool& use, bool& walkover)
{
  int enum_int = static_cast<int>(io_enum);

  /* Extract data */
  enter = ((enum_int & static_cast<int>(UnlockIOEvent::ENTER)) > 0);
  exit = ((enum_int & static_cast<int>(UnlockIOEvent::EXIT)) > 0);
  use = ((enum_int & static_cast<int>(UnlockIOEvent::USE)) > 0);
  walkover = ((enum_int & static_cast<int>(UnlockIOEvent::WALKOVER)) > 0);
}

/*
 * Description: Extracts data from the UnlockIOMode enum, as defined by the
 *              inputs.
 *
 * Inputs: UnlockIOEvent io_enum - the enum to extract bitwise data from
 *         bool lock - the main state IO lock should be modified
 *         bool events - the events within the state IO should be modified
 * Output: none
 */
void EventSet::dataEnumIOMode(UnlockIOMode io_enum, bool& lock, bool& events)
{
  int enum_int = static_cast<int>(io_enum);

  /* Extract data */
  lock = ((enum_int & static_cast<int>(UnlockIOMode::LOCK)) > 0);
  events = ((enum_int & static_cast<int>(UnlockIOMode::EVENTS)) > 0);
}

/*
 * Description: Extracts data from the ThingProperty enum, as defined by the
 *              inputs.
 *
 * Inputs: ThingProperty props - the enum to extract bitwise data from
 *         bool& active - modify the thing active bit (thing+)
 *         bool& forced - modify the thing if interaction is forced (npc)
 *         bool& inactive - modify the thing inactive time (io)
 *         bool& move - modify the thing if it is able to move (person+)
 *         bool& reset - modify the thing back to its start location (person+)
 *         bool& respawn - modify the thing respawn time (thing+)
 *         bool& speed - modify the thing speed (person+)
 *         bool& track - modify the thing tracking (npc)
 *         bool& visible - modify if the thing is visible (thing+)
 * Output: none
 */
void EventSet::dataEnumProperties(ThingProperty props, bool& active,
                       bool& forced, bool& inactive, bool& move, bool& reset,
                       bool& respawn, bool& speed, bool& track, bool& visible)
{
  int enum_int = static_cast<int>(props);

  /* Extract data */
  active = ((enum_int & static_cast<int>(ThingProperty::ACTIVE)) > 0);
  forced = ((enum_int & static_cast<int>(ThingProperty::FORCED_INTERACT)) > 0);
  inactive = ((enum_int & static_cast<int>(ThingProperty::INACTIVE_TIME)) > 0);
  move = ((enum_int & static_cast<int>(ThingProperty::MOVE_DISABLE)) > 0);
  reset = ((enum_int & static_cast<int>(ThingProperty::RESET_LOCATION)) > 0);
  respawn = ((enum_int & static_cast<int>(ThingProperty::RESPAWN_TIME)) > 0);
  speed = ((enum_int & static_cast<int>(ThingProperty::SPEED)) > 0);
  track = ((enum_int & static_cast<int>(ThingProperty::TRACKING)) > 0);
  visible = ((enum_int & static_cast<int>(ThingProperty::VISIBLE)) > 0);
}

/*
 * Description: Extracts data from the UnlockTileMode enum, as defined by the
 *              inputs.
 *
 * Inputs: UnlockTileMode tile_enum - the enum to extract bitwise data from
 *         bool enter - the enter tile event should be modified
 *         bool exit - the exit tile event should be modified
 * Output: none
 */
void EventSet::dataEnumTileEvent(UnlockTileMode tile_enum,
                                 bool& enter, bool& exit)
{
  int enum_int = static_cast<int>(tile_enum);

  /* Extract data */
  enter = ((enum_int & static_cast<int>(UnlockTileMode::ENTER)) > 0);
  exit = ((enum_int & static_cast<int>(UnlockTileMode::EXIT)) > 0);
}

/*
 * Description: Extracts data from the UnlockView enum, as defined by the
 *              inputs.
 *
 * Inputs: UnlockView view_enum - the enum to extract bitwise data from
 *         bool view - if the unlocked location should be viewed
 *         bool scroll - if the unlocked location should be scrolled to
 * Output: none
 */
void EventSet::dataEnumView(UnlockView view_enum, bool& view, bool& scroll)
{
  int enum_int = static_cast<int>(view_enum);

  /* Extract data */
  view = ((enum_int & static_cast<int>(UnlockView::GOTO)) > 0);
  scroll = ((enum_int & static_cast<int>(UnlockView::SCROLL)) > 0);
}

/*
 * Description: Extracts data from the passed in event if its a give item event.
 *
 * Inputs: Event event - the event to extract the data from
 *         int& item_id - the give item ID reference
 *         int& count - the give item count reference
 *         GiveItemFlags& flags - the give item control flags
 *         int& chance - the percent chance (0-100) of the give occurring
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventGiveItem(Event event, int& item_id, int& count,
                                 GiveItemFlags& flags, int& chance)
{
  if(event.classification == EventClassifier::ITEMGIVE &&
     event.ints.size() > kGIVE_ITEM_CHANCE)
  {
    item_id = event.ints[kGIVE_ITEM_ID];
    count = event.ints[kGIVE_ITEM_COUNT];
    flags = static_cast<GiveItemFlags>(event.ints[kGIVE_ITEM_FLAGS]);
    chance = event.ints[kGIVE_ITEM_CHANCE];

    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its a multiple event.
 *              This only pulls the data as is by value; not a reference.
 *
 * Inputs: Event event - the event val to extract the data from
 *         std::vector<Event>& event_list - set of event list structs
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventMultiple(Event event, std::vector<Event>& event_list)
{
  if(event.classification == EventClassifier::MULTIPLE)
  {
    event_list = event.events;
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its a multiple event.
 *              This only pulls the data as is by reference; not a value.
 *
 * Inputs: Event* event - the event ref to extract the data from
 *         std::vector<Event*>& event_list - set of event list references
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventMultiple(Event* event,
                                 std::vector<Event*>& event_list)
{
  if(event != nullptr && event->classification == EventClassifier::MULTIPLE)
  {
    /* First clear in passed array in case events were in it prior */
    event_list.clear();

    /* Parse through events and add reference to stack */
    for(uint32_t i = 0; i < event->events.size(); i++)
      event_list.push_back(&(event->events[i]));

    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its a notification
 *              event.
 *
 * Inputs: Event event - the event to extract the data from
 *         std::string& notification - the notification string reference
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventNotification(Event event, std::string& notification)
{
  if(event.classification == EventClassifier::NOTIFICATION &&
     event.strings.size() > 0)
  {
    notification = event.strings.front();
    return true;
  }
  return false;
}

/*
 * Description: Public static. Extracts data from the passed in property
 *              modifier event.
 *
 * Inputs: Event event - the event to extract the data from
 *         ThingBase type - the type of thing modified by the event
 *         int id - the id of the thing
 *         ThingProperty props - which properties are being modified by the call
 *         ThingProperty bools - the bool values of the properties being modded
 *         int respawn - the modified respawn time (thing+)
 *         int speed - the modified speed (person+)
 *         TrackingState track - the modification of what tracking (npc)
 *         int inactive - modify the time before going inactive (io)
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventPropMod(Event event, ThingBase& type, int& id,
                                ThingProperty& props, ThingProperty& bools,
                                int& respawn, int& speed, TrackingState& track,
                                int& inactive)
{
  if(event.classification == EventClassifier::PROPERTY &&
     event.ints.size() > kPROP_INACTIVE)
  {
    type = static_cast<ThingBase>(event.ints[kPROP_TYPE]);
    id = event.ints[kPROP_ID];
    props = static_cast<ThingProperty>(event.ints[kPROP_MODS]);
    bools = static_cast<ThingProperty>(event.ints[kPROP_BOOLS]);
    respawn = event.ints[kPROP_RESPAWN];
    speed = event.ints[kPROP_SPEED];
    track = static_cast<TrackingState>(event.ints[kPROP_TRACK]);
    inactive = event.ints[kPROP_INACTIVE];

    return true;
  }
  return false;
}

/*
 * Description: Public static. Extracts data from the passed in event if it's
 *              a start battle event.
 *
 * Inputs: Event* event - the event to extract the data from
 *         BattleFlags& flags - the battle flag properties
 *         Event*& event_win - the event triggered if battle is won
 *         Event*& event_lose = the event triggered if the battle is lost
 * Output: bool - true if the extracted data is valid
 */
bool EventSet::dataEventStartBattle(Event* event, BattleFlags& flags,
                                    Event*& event_win, Event*& event_lose)
{
  if(event != nullptr &&
     event->classification == EventClassifier::BATTLESTART &&
     event->ints.size() > 0 && event->events.size() > kBATTLE_EVENT_LOSE)
  {
    flags = static_cast<BattleFlags>(event->ints[kBATTLE_FLAGS]);
    event_win = &(event->events[kBATTLE_EVENT_WIN]);
    event_lose = &(event->events[kBATTLE_EVENT_LOSE]);
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its a start map event.
 *
 * Inputs: Event event - the event to extract the data from
 *         int& map_id - the start map ID reference
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventStartMap(Event event, int& map_id)
{
  if(event.classification == EventClassifier::MAPSWITCH &&
     event.ints.size() > kMAP_ID)
  {
    map_id = event.ints[kMAP_ID];
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its a take item event
 *
 * Inputs: Event event - the event to extract the data from
 *         int& item_id - the take item ID reference
 *         int& count - the take item count reference
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventTakeItem(Event event, int& item_id, int& count)
{
  if(event.classification == EventClassifier::ITEMTAKE &&
     event.ints.size() > kTAKE_ITEM_COUNT)
  {
    item_id = event.ints[kTAKE_ITEM_ID];
    count = event.ints[kTAKE_ITEM_COUNT];
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its a teleport thing
 *              event.
 *
 * Inputs: Event event - the event to extract the data from
 *         int& thing_id - the teleport thing ID reference
 *         int& x - the teleport thing tile X reference
 *         int& y - the teleport thing tile Y reference
 *         int& section_id - the teleport thing map sub ID reference
 * Output: bool - true if the data was extracted. Fails if the event is the
 *                wrong category
 */
bool EventSet::dataEventTeleport(Event event, int& thing_id, int& x, int& y,
                                int& section_id)
{
  if(event.classification == EventClassifier::TELEPORTTHING &&
     event.ints.size() > kTELEPORT_SECTION)
  {
    thing_id = event.ints[kTELEPORT_ID];
    x = event.ints[kTELEPORT_X];
    y = event.ints[kTELEPORT_Y];
    section_id = event.ints[kTELEPORT_SECTION];
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its an unlock IO
 *              event.
 *
 * Inputs: Event event - the event to extract the data from
 *         int& io_id - the IO id to unlock
 *         UnlockIOMode& mode - the mode enum for how the IO is unlocked
 *         int& state_num - the state to parse for unlock attempts
 *         UnlockIOEvent& mode_events - the type of events to unlock in state
 *         UnlockView& mode_view - how the unlocked IO is viewed
 *         int& view_time - the length of time the unlocked is viewed
 * Output: bool - true if the data was extracted. Fails if event is the wrong
 *                category
 */
bool EventSet::dataEventUnlockIO(Event event, int& io_id, UnlockIOMode& mode,
                                 int& state_num, UnlockIOEvent& mode_events,
                                 UnlockView& mode_view, int& view_time)
{
  if(event.classification == EventClassifier::UNLOCKIO &&
     event.ints.size() > kUNIO_VIEW_TIME)
  {
    io_id = event.ints[kUNIO_ID];
    mode = static_cast<UnlockIOMode>(event.ints[kUNIO_MODE]);
    state_num = event.ints[kUNIO_STATE];
    mode_events = static_cast<UnlockIOEvent>(event.ints[kUNIO_MODE_EVENT]);
    mode_view = static_cast<UnlockView>(event.ints[kUNIO_VIEW_MODE]);
    view_time = event.ints[kUNIO_VIEW_TIME];
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its an unlock thing
 *              event.
 *
 * Inputs: Event event - the event to extract the data from
 *         int& thing_id - the thing ID to unlock
 *         UnlockView& mode_view - how the unlocked is viewed
 *         int& view_time - the length of time the unlocked is viewed
 * Output: bool - true if the data was extracted. Fails if event is the wrong
 *                category
 */
bool EventSet::dataEventUnlockThing(Event event, int& thing_id,
                                    UnlockView& mode_view, int& view_time)
{
  if(event.classification == EventClassifier::UNLOCKTHING &&
     event.ints.size() > kUNTHING_VIEW_TIME)
  {
    thing_id = event.ints[kUNTHING_ID];
    mode_view = static_cast<UnlockView>(event.ints[kUNTHING_VIEW_MODE]);
    view_time = event.ints[kUNTHING_VIEW_TIME];
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the passed in event if its an unlock tile
 *              event.
 *
 * Inputs: Event event - the event to extract the data from
 *         int& section_id - the sub-map section ID to access the tile
 *         int& tile_x - the tile X location within the sub-map
 *         int& tile_y - the tile Y location within the sub-map
 *         UnlockTileMode& mode - the events within the tile to access
 *         UnlockView& mode_view - how the unlocked is viewed
 *         int& view_time - the length of time the unlocked is viewed
 * Output: bool - true if the data was extracted. Fails if event is the wrong
 *                category
 */
bool EventSet::dataEventUnlockTile(Event event, int& section_id, int& tile_x,
                                   int& tile_y, UnlockTileMode& mode,
                                   UnlockView& mode_view, int& view_time)
{
  if(event.classification == EventClassifier::UNLOCKTILE &&
     event.ints.size() > kUNTILE_VIEW_TIME)
  {
    section_id = event.ints[kUNTILE_SECTION];
    tile_x = event.ints[kUNTILE_X];
    tile_y = event.ints[kUNTILE_Y];
    mode = static_cast<UnlockTileMode>(event.ints[kUNTILE_MODE]);
    mode_view = static_cast<UnlockView>(event.ints[kUNTILE_VIEW_MODE]);
    view_time = event.ints[kUNTILE_VIEW_TIME];
    return true;
  }
  return false;
}

/*
 * Description: Extracts data from the locked struct regarding the have item
 *              unlock trigger.
 *
 * Inputs: Locked lock - the locked data struct
 *         int& id - the item ID reference
 *         int& count - the item count reference
 *         bool& consume - if the item is consumed bool reference
 * Output: bool - true if the data from the retrieval is valid
 */
bool EventSet::dataLockedItem(Locked lock, int& id, int& count, bool& consume)
{
  if(lock.state == LockedState::ITEM && lock.is_locked &&
     lock.ints.size() > kHAVE_ITEM_COUNT &&
     lock.bools.size() > kHAVE_ITEM_CONSUME)
  {
    id = lock.ints[kHAVE_ITEM_ID];
    count = lock.ints[kHAVE_ITEM_COUNT];
    consume = lock.bools[kHAVE_ITEM_CONSUME];
    return true;
  }
  return false;
}

/*
 * Description: Public static. Deletes the passed in event of all dynamic
 *              memory. Returns a blank event, as replacement.
 *
 * Inputs: Event event - the event struct to delete all dynamic memory
 * Output: Event - blank event, to replace deleted event
 */
Event EventSet::deleteEvent(Event event)
{
  /* Delete the existing conversation within the event, if relevant */
  if(event.convo != nullptr)
    delete event.convo;
  event.convo = nullptr;

  /* Delete the existing events within the event, if relevant */
  for(uint32_t i = 0; i < event.events.size(); i++)
  {
    deleteEvent(event.events[i]);
  }
  event.events.clear();

  return createBlankEvent();
}

/*
 * Description: Public static. Takes the base conversation pointer with a list
 *              of indexes, such as 1,2,3,1,1,1 to indicate depth. It then
 *              returns a reference based on the depth path above. If the depth
 *              path doesn't exist, its created with blank conversation events.
 *
 * Inputs: Conversation* reference - the reference conversation to manipulate
 *                                   and get the conversation element
 *         std::vector<std::string> index_list - the list of indexes to parse
 * Output: Conversation* - the found conversation instance reference
 */
Conversation* EventSet::getConversation(Conversation* reference,
                                        std::vector<std::string> index_list)
{
  Conversation* convo = reference;

  if(convo != nullptr)
  {
    for(uint16_t i = 1; i < index_list.size(); i++)
    {
      uint16_t convo_index = std::stoi(index_list[i]);

      while(convo->next.size() < convo_index)
        convo->next.push_back(createBlankConversation());

      convo = &convo->next[convo_index-1];
    }
  }

  return convo;
}

/*
 * Description: Checks if there is data to save within the conversation ref and
 *              recursively within its children.
 *
 * Inputs: Conversation* ref - the reference conversation to check
 * Output: bool - true if there is data to save within the conversation
 */
bool EventSet::isDataToSave(Conversation* ref)
{
  if(ref != nullptr)
  {
    /* Check event */
    if(isDataToSave(ref->action_event))
      return true;

    /* Check remaining conversation points */
    for(uint32_t i = 0; i < ref->next.size(); i++)
      if(isDataToSave(&ref->next[i]))
        return true;
  }

  return false;
}

/*
 * Description: Checks if there is data to save within the event reference and
 *              within any objects stored within the event (including
 *              conversations).
 *
 * Inputs: Event event_ref - the event reference to check
 * Output: bool - true if there is data to save within the event
 */
bool EventSet::isDataToSave(const Event& event_ref)
{
  if(event_ref.classification != EventClassifier::NOEVENT)
  {
    /* First check the event itself */
    if(event_ref.has_exec)
      return true;

    /* Then check the events stored within */
    if(event_ref.classification == EventClassifier::BATTLESTART ||
       event_ref.classification == EventClassifier::MULTIPLE)
    {
      for(uint32_t i = 0; i < event_ref.events.size(); i++)
        if(isDataToSave(event_ref.events[i]))
          return true;
    }

    /* Then check the conversation */
    if(event_ref.classification == EventClassifier::CONVERSATION)
    {
      if(isDataToSave(event_ref.convo))
        return true;
    }
  }

  /* Otherwise, nothing to save */
  return false;
}

/*
 * Description: Checks if there is data to save within the lock reference
 *
 * Inputs: Locked lock_ref - the locked reference to check
 * Output: bool - true if there is data to save within the lock
 */
bool EventSet::isDataToSave(const Locked& lock_ref)
{
  return (lock_ref.state != LockedState::NONE && !lock_ref.is_locked);
}

/*
 * Description: Determines if the locked struct is locked and with a valid
 *              unlock mode possibility.
 *
 * Inputs: Locked lock_struct - the lock struct data
 * Output: bool - true if the struct is locked
 */

bool EventSet::isLocked(const Locked& lock_struct)
{
  return (lock_struct.state != LockedState::NONE &&
          lock_struct.is_locked);
}

/*
 * Description: Recursive save of the conversation. Takes a conversation ref
 *              and starting index with file handling pointer and parses
 *              all embedded conversations from there.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         Conversation* convo - the starting convo
 *         std::string index - the index of the starting convo, default "1"
 * Output: bool - true if the save was successful
 */
bool EventSet::saveConversation(FileHandler* fh, Conversation* convo,
                                std::string index)
{
  if(fh != nullptr && convo != nullptr)
  {
    /* Check if this index needs to be saved */
    if(isDataToSave(convo->action_event))
    {
      fh->writeXmlElement("conversation", "id", index);
      saveEvent(fh, convo->action_event, "event", false);
      fh->writeXmlElementEnd();
    }

    /* Recursively parse through remaining entries */
    for(uint32_t i = 0; i < convo->next.size(); i++)
      saveConversation(fh, &convo->next[i], index + "." + std::to_string(i+1));

    return true;
  }
  return false;
}

/*
 * Description: Save the passed in event to the file reference pointer. If the
 *              wrapper is set to blank, it will not wrap in XML
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         Event event_ref - the event reference to save delta data
 *         std::string wrapper - the wrapping xml. Default is "event"
 *         bool check - check if a save is needed prior to call. default true
 * Output: bool - true if the save was successful and there was data to save
 */
bool EventSet::saveEvent(FileHandler* fh, const Event& event_ref,
                         std::string wrapper, bool check)
{
  if(fh != nullptr)
  {
    /* Check to see if the save is to proceed */
    if(!check || isDataToSave(event_ref))
    {
      /* Wrapper */
      if(!wrapper.empty())
        fh->writeXmlElement(wrapper);

      /* Executed */
      if(event_ref.has_exec)
      {
        fh->writeXmlData("executed", event_ref.has_exec);

        /* Specialty events with additional data embedded */
        /* -- BATTLE START -- */
        if(event_ref.classification == EventClassifier::BATTLESTART)
        {
          if(event_ref.events.size() > kBATTLE_EVENT_LOSE)
          {
            bool win_save = isDataToSave(event_ref.events[kBATTLE_EVENT_WIN]);
            bool lose_save = isDataToSave(event_ref.events[kBATTLE_EVENT_LOSE]);

            if(win_save || lose_save)
            {
              fh->writeXmlElement("startbattle");
              if(win_save)
                saveEvent(fh, event_ref.events[kBATTLE_EVENT_WIN],
                          "eventwin", false);
              if(lose_save)
                saveEvent(fh, event_ref.events[kBATTLE_EVENT_LOSE],
                          "eventlose", false);
              fh->writeXmlElementEnd();
            }
          }
        }
        /* -- CONVERSATION -- */
        else if(event_ref.classification == EventClassifier::CONVERSATION)
        {
          saveConversation(fh, event_ref.convo);
        }
        /* -- MULTIPLE -- */
        else if(event_ref.classification == EventClassifier::MULTIPLE)
        {
          bool wrapped = false;
          for(uint32_t i = 0; i < event_ref.events.size(); i++)
          {
            if(isDataToSave(event_ref.events[i]))
            {
              /* Check wrapping */
              if(!wrapped)
              {
                fh->writeXmlElement("multiple");
                wrapped = true;
              }

              /* Write event delta */
              fh->writeXmlElement("event", "id", i);
              saveEvent(fh, event_ref.events[i], "", false);
              fh->writeXmlElementEnd();
            }
          }
          if(wrapped)
            fh->writeXmlElementEnd();
        }
      }

      /* End Wrapper */
      if(!wrapper.empty())
        fh->writeXmlElementEnd();

      return true;
    }
  }
  return false;
}

/*
 * Description: Save the passed in locked struct to the file reference pointer.
 *              This only will save if locked (one item).
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         Locked lock_ref - the lock structure reference to save
 * Output: bool - true if the save was successful
 */
bool EventSet::saveLocked(FileHandler* fh, const Locked& lock_ref)
{
  if(fh != nullptr)
  {
    if(isDataToSave(lock_ref))
      fh->writeXmlData("locked", lock_ref.is_locked);
    return true;
  }
  return false;
}

/*
 * Description: Public static. Attempts an unlock on the locked state (pass by
 *              ref) with the given item ID and count total from the calling
 *              parent. If valid, it updates the locked state and returns if the
 *              items are consumed or not.
 *
 * Inputs: Locked& locked_state - the locked state to manipulate
 *         int id - the item ID that the player owns
 *         int count - the item ID count in the player inventory
 * Output: bool - true if the passed in item ID and count should be consumed
 */
bool EventSet::unlockItem(Locked& locked_state, int id, int count)
{
  /* Starting values */
  bool consume = false;

  if(locked_state.state == LockedState::ITEM &&
     locked_state.ints.size() > kHAVE_ITEM_COUNT &&
     locked_state.bools.size() > kHAVE_ITEM_CONSUME &&
     locked_state.is_locked)
  {
    if(locked_state.ints[kHAVE_ITEM_ID] == id &&
       locked_state.ints[kHAVE_ITEM_COUNT] <= count)
    {
      locked_state.is_locked = false;
      consume = locked_state.bools[kHAVE_ITEM_CONSUME];
    }
  }

  return consume;
}

/*
 * Description: Public static. Attempts an unlock on the locked state (pass by
 *              ref) with a trigger event.
 *
 * Inputs: Locked& locked_state - the locked state to manipulate
 * Output: none
 */
void EventSet::unlockTrigger(Locked& locked_state)
{
  if(locked_state.state == LockedState::TRIGGER)
  {
    locked_state.is_locked = false;
  }
}

/*
 * Description: Public static. Called on a locked state once an event was
 *              triggered and used, or if the object the state was locking was
 *              utilized. If the lock is not permanent, triggers a re-lock.
 *
 * Inputs: Locked& locked_state - the locked state to manipulate. Pass by ref
 * Output: none
 */
void EventSet::unlockUsed(Locked& locked_state)
{
  if(locked_state.state != LockedState::NONE && !locked_state.permanent)
    locked_state.is_locked = true;
}

/*
 * Description: Updates the reference conversation with the XML load data.
 *
 * Inputs: Conversation* reference - the reference conversation element to
 *                                   update with the data
 *         XmlData data - the xml data structure
 *         int index - the element reference index
 *         int section_index - the map section where the event is defined
 * Output: none
 */
void EventSet::updateConversation(Conversation* reference, XmlData data,
                                  int index, int section_index)
{
  /* Only proceed if the reference convo is not NULL */
  if(reference != nullptr)
  {
    std::string element = data.getElement(index);

    /* Determine what part of the conversation to edit */
    if(element == "text")
      reference->text = data.getDataString();
    else if(element == "id")
      reference->thing_id = data.getDataInteger();
    else if(element == "event")
      reference->action_event = updateEvent(reference->action_event, data,
                                            index + 1, section_index);
  }
}

/*
 * Description: Updates the event with the XML load data.
 *
 * Inputs: Event event - the event to update with the data
 *         XmlData data - the xml data structure
 *         int index - the element reference index
 *         int section_index - the map section where the event is defined
 * Output: Event - the returned event after the load
 */
Event EventSet::updateEvent(Event event, XmlData data, int file_index,
                            int section_index)
{
  EventClassifier category = EventClassifier::NOEVENT;
  bool read_success;

  /* Category and Element */
  std::string category_str = data.getElement(file_index);
  std::string element = data.getElement(file_index + 1);

  /* Determine the category of the event that is being updated */
  if(category_str == "startbattle")
    category = EventClassifier::BATTLESTART;
  else if(category_str == "conversation")
    category = EventClassifier::CONVERSATION;
  else if(category_str == "giveitem")
    category = EventClassifier::ITEMGIVE;
  else if(category_str == "takeitem")
    category = EventClassifier::ITEMTAKE;
  else if(category_str == "startmap")
    category = EventClassifier::MAPSWITCH;
  else if(category_str == "multiple")
    category = EventClassifier::MULTIPLE;
  else if(category_str == "notification")
    category = EventClassifier::NOTIFICATION;
  else if(category_str == "propertymod")
    category = EventClassifier::PROPERTY;
  else if(category_str == "justsound")
    category = EventClassifier::SOUNDONLY;
  else if(category_str == "teleportthing")
    category = EventClassifier::TELEPORTTHING;
  else if(category_str == "unlockio")
    category = EventClassifier::UNLOCKIO;
  else if(category_str == "unlockthing")
    category = EventClassifier::UNLOCKTHING;
  else if(category_str == "unlocktile")
    category = EventClassifier::UNLOCKTILE;

  /* If the category doesn't match, create a new event */
  if(category_str != "executed" && category != event.classification)
  {
    bool old_exec = event.has_exec;
    event = deleteEvent(event);

    if(category == EventClassifier::BATTLESTART)
      event = createEventStartBattle();
    else if(category == EventClassifier::CONVERSATION)
      event = createEventConversation();
    if(category == EventClassifier::ITEMGIVE)
      event = createEventGiveItem();
    else if(category == EventClassifier::ITEMTAKE)
      event = createEventTakeItem();
    else if(category == EventClassifier::MAPSWITCH)
      event = createEventStartMap();
    else if(category == EventClassifier::MULTIPLE)
      event = createEventMultiple();
    else if(category == EventClassifier::NOTIFICATION)
      event = createEventNotification();
    else if(category == EventClassifier::PROPERTY)
      event = createEventPropMod();
    else if(category == EventClassifier::SOUNDONLY)
      event = createEventSound();
    else if(category == EventClassifier::TELEPORTTHING)
      event = createEventTeleport();
    else if(category == EventClassifier::UNLOCKIO)
      event = createEventUnlockIO();
    else if(category == EventClassifier::UNLOCKTHING)
      event = createEventUnlockThing();
    else if(category == EventClassifier::UNLOCKTILE)
      event = createEventUnlockTile();

    event.has_exec = old_exec;
  }

  /* Proceed to set up the event with the marked changes */
  if(category_str == "executed")// && category != EventClassifier::NOEVENT)
  {
    bool executed = data.getDataBool(&read_success);
    if(read_success)
      event.has_exec = executed;
  }
  else if(element == "one_shot")
  {
    bool one_shot = data.getDataBool(&read_success);
    if(read_success)
      event.one_shot = one_shot;
  }
  /* -- SOUND ONLY -- */
  else if(element == "sound_id" || category == EventClassifier::SOUNDONLY)
  {
    int32_t sound_id = data.getDataInteger(&read_success);
    if(read_success && sound_id >= 0)
      event.sound_id = sound_id;
  }
  /* -- BATTLE START -- */
  else if(category == EventClassifier::BATTLESTART)
  {
    /* Get the existing data */
    Event *event_win, *event_lose;
    BattleFlags flags;
    if(dataEventStartBattle(&event, flags, event_win, event_lose))
    {
      /* Parse new data */
      if(element == "windisappear" || element == "losegg" ||
         element == "restorehealth" || element == "restoreqd")
      {
        /* Get existing enum data */
        bool win_disappear, lose_gg, restore_health, restore_qd;
        dataEnumBattleFlags(flags, win_disappear, lose_gg,
                            restore_health, restore_qd);

        /* Parse new enum data */
        if(element == "windisappear")
          win_disappear = data.getDataBool();
        else if(element == "losegg")
          lose_gg = data.getDataBool();
        else if(element == "restorehealth")
          restore_health = data.getDataBool();
        else if(element == "restoreqd")
          restore_qd = data.getDataBool();

        /* Set new enum data */
        flags = createEnumBattleFlags(win_disappear, lose_gg,
                                      restore_health, restore_qd);
        event.ints[kBATTLE_FLAGS] = static_cast<int>(flags);
      }
      else if(element == "eventwin")
      {
        *event_win = updateEvent(*event_win, data, file_index + 2,
                                 section_index);
      }
      else if(element == "eventlose")
      {
        *event_lose = updateEvent(*event_lose, data, file_index + 2,
                                  section_index);
      }
    }
  }
  /* -- CONVERSATION -- */
  else if(category == EventClassifier::CONVERSATION)
  {
    /* Split the key value for conversation (Eg. index="1.2.1.1") */
    std::vector<std::string> index_list =
                              Helpers::split(data.getKeyValue(file_index), '.');

    /* If size is only one, the conversation is the first item */
    if(index_list.size() == 1)
    {
      updateConversation(event.convo, data, file_index + 1, section_index);
    }
    /* Otherwise, it's embedded in the conversation stack */
    else if(index_list.size() > 1)
    {
      Conversation* edited_convo = getConversation(event.convo, index_list);
      updateConversation(edited_convo, data, file_index + 1, section_index);
    }
  }
  /* -- ITEM GIVE -- */
  else if(category == EventClassifier::ITEMGIVE)
  {
    if(element == "id")
      event.ints.at(kGIVE_ITEM_ID) = data.getDataInteger();
    else if(element == "chance")
      event.ints.at(kGIVE_ITEM_CHANCE) = data.getDataInteger();
    else if(element == "count")
      event.ints.at(kGIVE_ITEM_COUNT) = data.getDataInteger();
    else if(element == "autodrop")
    {
      /* Get the current data */
      GiveItemFlags item_flags =
                   static_cast<GiveItemFlags>(event.ints.at(kGIVE_ITEM_FLAGS));
      bool auto_drop;
      dataEnumGiveFlags(item_flags, auto_drop);

      /* Set the data */
      auto_drop = data.getDataBool();

      /* Set new enum data */
      item_flags = createEnumGiveFlags(auto_drop);
      event.ints.at(kGIVE_ITEM_FLAGS) = static_cast<int>(item_flags);
    }
  }
  /* -- ITEM TAKE -- */
  else if(category == EventClassifier::ITEMTAKE)
  {
    if(element == "id")
      event.ints.at(kTAKE_ITEM_ID) = data.getDataInteger();
    else if(element == "count")
      event.ints.at(kTAKE_ITEM_COUNT) = data.getDataInteger();
  }
  /* -- MAP SWITCH -- */
  else if(category == EventClassifier::MAPSWITCH)
  {
    if(element == "id")
      event.ints.at(kMAP_ID) = data.getDataInteger();
  }
  /* -- MULTIPLE -- */
  else if(category == EventClassifier::MULTIPLE)
  {
    if(element == "event")
    {
      int id = std::stoi(data.getKeyValue(file_index + 1));

      /* Increase size to accomodate new ID index */
      while(static_cast<int>(event.events.size()) <= id)
        event.events.push_back(createBlankEvent());

      /* Load the information into the new index event */
      event.events[id] = updateEvent(event.events[id], data, file_index + 2,
                                     section_index);
    }
  }
  /* -- NOTIFICATION -- */
  else if(category == EventClassifier::NOTIFICATION)
  {
    if(data.getTailElements(file_index).size() == 1 ||
       element == "text")
    {
      event.strings.at(0) = data.getDataString();
    }
  }
  /* -- PROPERTY MODIFIER -- */
  else if(category == EventClassifier::PROPERTY)
  {
    /* Baseline modifier */
    ThingProperty mods_curr =
                        static_cast<ThingProperty>(event.ints.at(kPROP_MODS));
    bool active, forced, inactive, move, reset,
         respawn, speed, track, visible;
    dataEnumProperties(mods_curr, active, forced, inactive, move, reset,
                       respawn, speed, track, visible);

    /* Parse the element */
    if(element == "class")
    {
      ThingBase class_def = Helpers::typeFromStr(data.getDataString());
      event.ints.at(kPROP_TYPE) = static_cast<int>(class_def);
    }
    else if(element == "id")
    {
      event.ints.at(kPROP_ID) = data.getDataInteger();
    }
    else if(element == "inactive")
    {
      event.ints.at(kPROP_INACTIVE) = data.getDataInteger();
      inactive = true;
    }
    else if(element == "respawn")
    {
      event.ints.at(kPROP_RESPAWN) = data.getDataInteger();
      respawn = true;
    }
    else if(element == "speed")
    {
      event.ints.at(kPROP_SPEED) = data.getDataInteger();
      speed = true;
    }
    else if(element == "tracking")
    {
      TrackingState track_def = TrackingState::NOTRACK;
      std::string track_str = data.getDataString();
      if(track_str == "toplayer")
        track_def = TrackingState::TOPLAYER;
      else if(track_str == "avoidplayer")
        track_def = TrackingState::AVOIDPLAYER;

      event.ints.at(kPROP_TRACK) = static_cast<int>(track_def);
      track = true;
    }
    else /* Bools */
    {
      /* Get start point */
      ThingProperty prop_curr =
                        static_cast<ThingProperty>(event.ints.at(kPROP_BOOLS));
      bool active_b, forced_b, inactive_b, move_b, reset_b,
           respawn_b, speed_b, track_b, visible_b;
      dataEnumProperties(prop_curr, active_b, forced_b, inactive_b, move_b,
                         reset_b, respawn_b, speed_b, track_b, visible_b);

      /* Parse new data */
      if(element == "active")
      {
        active_b = data.getDataBool();
        active = true;
      }
      else if(element == "forceinteract")
      {
        forced_b = data.getDataBool();
        forced = true;
      }
      else if(element == "movedisable")
      {
        move_b = data.getDataBool();
        move = true;
      }
      else if(element == "resetlocation")
      {
        reset_b = data.getDataBool();
        reset = true;
      }
      else if(element == "visible")
      {
        visible_b = data.getDataBool();
        visible = true;
      }

      /* Set new data */
      ThingProperty prop_new = createEnumProperties(active_b, forced_b,
          inactive_b, move_b, reset_b, respawn_b, speed_b, track_b, visible_b);
      event.ints.at(kPROP_BOOLS) = static_cast<int>(prop_new);
    }

    /* Finalize modifier flags */
    ThingProperty mods_new = createEnumProperties(active, forced, inactive,
                                  move, reset, respawn, speed, track, visible);
    event.ints.at(kPROP_MODS) = static_cast<int>(mods_new);
  }
  /* -- TELEPORT THING -- */
  else if(category == EventClassifier::TELEPORTTHING)
  {
    if(element == "id")
      event.ints.at(kTELEPORT_ID) = data.getDataInteger();
    if(element == "x")
      event.ints.at(kTELEPORT_X) = data.getDataInteger();
    else if(element == "y")
      event.ints.at(kTELEPORT_Y) = data.getDataInteger();
    else if(element == "section")
      event.ints.at(kTELEPORT_SECTION) = data.getDataInteger();
  }
  /* -- UNLOCK IO -- */
  else if(category == EventClassifier::UNLOCKIO)
  {
    /* Get existing data */
    int io_id, state_num, view_time;
    UnlockIOMode mode;
    UnlockIOEvent mode_events;
    UnlockView mode_view;
    if(dataEventUnlockIO(event, io_id, mode, state_num, mode_events,
                         mode_view, view_time))
    {
      /* Parse new data */
      if(element == "evententer" || element == "eventexit" ||
         element == "eventuse" || element == "eventwalkover" ||
         element == "eventall")
      {
        /* Get existing enum data */
        bool enter,exit,use,walkover;
        dataEnumIOEvent(mode_events, enter, exit, use, walkover);

        /* Parse new enum data */
        if(element == "evententer" || element == "eventall")
          enter = data.getDataBool();
        if(element == "eventexit" || element == "eventall")
          exit = data.getDataBool();
        if(element == "eventuse" || element == "eventall")
          use = data.getDataBool();
        if(element == "eventwalkover" || element == "eventall")
          walkover = data.getDataBool();

        /* Set new enum data */
        mode_events = createEnumIOEvent(enter, exit, use, walkover);
      }
      else if(element == "id")
      {
        io_id = data.getDataInteger();
      }
      else if(element == "modelock" || element == "modeevents")
      {
        /* Get existing enum data */
        bool lock, events;
        dataEnumIOMode(mode, lock, events);

        /* Parse new enum data */
        if(element == "modelock")
          lock = data.getDataBool();
        else if(element == "modeevents")
          events = data.getDataBool();

        /* Set new enum data */
        mode = createEnumIOMode(lock, events);
      }
      else if(element == "state")
      {
        state_num = data.getDataInteger();
      }
      else if(element == "view" || element == "viewscroll")
      {
        /* Get existing enum data */
        bool view, scroll;
        dataEnumView(mode_view, view, scroll);

        /* Parse new enum data */
        if(element == "view")
          view = data.getDataBool();
        else if(element == "viewscroll")
          scroll = data.getDataBool();

        /* Set new enum data */
        mode_view = createEnumView(view, scroll);
      }
      else if(element == "viewtime")
      {
        view_time = data.getDataInteger();
      }

      /* Update event with new data */
      event = createEventUnlockIO(io_id, mode, state_num, mode_events,
                                  mode_view, view_time, event.sound_id);
    }
  }
  /* -- UNLOCK THING -- */
  else if(category == EventClassifier::UNLOCKTHING)
  {
    /* Get existing data */
    int thing_id, view_time;
    UnlockView mode_view;
    if(dataEventUnlockThing(event, thing_id, mode_view, view_time))
    {
      /* Parse new data */
      if(element == "id")
      {
        thing_id = data.getDataInteger();
      }
      else if(element == "view" || element == "viewscroll")
      {
        /* Get existing enum data */
        bool view, scroll;
        dataEnumView(mode_view, view, scroll);

        /* Parse new enum data */
        if(element == "view")
          view = data.getDataBool();
        else if(element == "viewscroll")
          scroll = data.getDataBool();

        /* Set new enum data */
        mode_view = createEnumView(view, scroll);
      }
      else if(element == "viewtime")
      {
        view_time = data.getDataInteger();
      }

      /* Update event with new data */
      event = createEventUnlockThing(thing_id, mode_view, view_time,
                                     event.sound_id);
    }
  }
  /* -- UNLOCK TILE -- */
  else if(category == EventClassifier::UNLOCKTILE)
  {
    /* Get existing data */
    int section_id, tile_x, tile_y, view_time;
    UnlockTileMode mode;
    UnlockView mode_view;
    if(dataEventUnlockTile(event, section_id, tile_x, tile_y, mode,
                           mode_view, view_time))
    {
      /* Parse new data */
      if(element == "evententer" || element == "eventexit" ||
         element == "eventall")
      {
        /* Get existing enum data */
        bool enter,exit;
        dataEnumTileEvent(mode, enter, exit);

        /* Parse new enum data */
        if(element == "evententer" || element == "eventall")
          enter = data.getDataBool();
        if(element == "eventexit" || element == "eventall")
          exit = data.getDataBool();

        /* Set new enum data */
        mode = createEnumTileEvent(enter, exit);
      }
      else if(element == "sectionid")
      {
        section_id = data.getDataInteger();
      }
      else if(element == "view" || element == "viewscroll")
      {
        /* Get existing enum data */
        bool view, scroll;
        dataEnumView(mode_view, view, scroll);

        /* Parse new enum data */
        if(element == "view")
          view = data.getDataBool();
        else if(element == "viewscroll")
          scroll = data.getDataBool();

        /* Set new enum data */
        mode_view = createEnumView(view, scroll);
      }
      else if(element == "viewtime")
      {
        view_time = data.getDataInteger();
      }
      else if(element == "x")
      {
        tile_x = data.getDataInteger();
      }
      else if(element == "y")
      {
        tile_y = data.getDataInteger();
      }

      /* Update event with new data */
      if(tile_x >= 0 && tile_y >= 0)
        event = createEventUnlockTile(section_id, tile_x, tile_y, mode,
                                      mode_view, view_time, event.sound_id);
    }
  }

  return event;
}

/*
 * Description: Updates the event with the one shot property (true or false)
 *
 * Inputs: Event event - the event to update
 *         bool one_shot - set if the event is a one shot trigger only
 * Output: Event - the updated event with the one shot property
 */
Event EventSet::updateEventOneShot(Event event, bool one_shot)
{
  event.one_shot = one_shot;
  return event;
}

/*
 * Description: Updates the locked struct with the XML load data.
 *
 * Inputs: Locked locked_curr - the locked struct to update
 *         XmlData data - the xml data structure
 *         int file_index - the element reference index
 * Output: Locked - the updated locked struct
 */
Locked EventSet::updateLocked(Locked locked_curr, XmlData data, int file_index)
{
  LockedState state = LockedState::NONE;
  std::string state_str = data.getElement(file_index);
  bool read_success;

  if(state_str == "locked")
  {
    bool bool_data = data.getDataBool(&read_success);
    if(read_success)
      locked_curr.is_locked = bool_data;
  }
  else
  {
    /* Determine the state of the lock that is being updated */
    if(state_str == "item")
      state = LockedState::ITEM;
    else if(state_str == "trigger")
      state = LockedState::TRIGGER;

    /* If the state doesn't match, create a new locked state with fresh data */
    if(state != locked_curr.state)
    {
      locked_curr = createBlankLocked();

      if(state == LockedState::ITEM)
        locked_curr = createLockHaveItem();
      else if(state == LockedState::TRIGGER)
        locked_curr = createLockTriggered();
    }

    /* Proceed to set up the event with the marked changes */
    if(data.getElement(file_index + 1) == "permanent")
    {
      bool permanent = data.getDataBool(&read_success);
      if(read_success)
        locked_curr.permanent = permanent;
    }
    /* -- HAVE ITEM -- */
    else if(state == LockedState::ITEM)
    {
      std::string item_element = data.getElement(file_index + 1);
      if(item_element == "consume")
        locked_curr.bools.at(kHAVE_ITEM_CONSUME) = data.getDataBool();
      else if(item_element == "count")
        locked_curr.ints.at(kHAVE_ITEM_COUNT) = data.getDataInteger();
      else if(item_element == "id")
        locked_curr.ints.at(kHAVE_ITEM_ID) = data.getDataInteger();
    }
    /* -- TRIGGER -- */
    else if(state == LockedState::TRIGGER)
    {}
  }

  return locked_curr;
}
