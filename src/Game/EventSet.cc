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
const uint8_t EventSet::kGIVE_ITEM_COUNT = 1;
const uint8_t EventSet::kGIVE_ITEM_ID = 0;
const uint8_t EventSet::kMAP_ID = 0;
const uint8_t EventSet::kTELEPORT_ID = 0;
const uint8_t EventSet::kTELEPORT_SECTION = 3;
const uint8_t EventSet::kTELEPORT_X = 1;
const uint8_t EventSet::kTELEPORT_Y = 2;
/* ---- */
const uint8_t EventSet::kHAVE_ITEM_CONSUME = 0;
const uint8_t EventSet::kHAVE_ITEM_COUNT = 1;
const uint8_t EventSet::kHAVE_ITEM_ID = 0;
/* ---- */
const int32_t EventSet::kUNSET_ID = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventSet::EventSet()
{
  event_locked = createBlankEvent();
  events_unlocked.clear();

  get_index = -1;
  locked_status = createBlankLocked();
  unlocked_state = UnlockedState::NONE;
}

/* Destructor function */
EventSet::~EventSet()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Gets an event reference from the unlocked status. Will create if not
 * there, if the variable has been set */
Event* EventSet::getUnlockedRef(int index, bool create)
{
  Event* found_event = nullptr;

  if(index >= 0)
  {
    /* Create if doesn't exist */
    if(create)
    {
      while(events_unlocked.size() < (uint32_t)index)
        events_unlocked.push_back(createBlankEvent());
    }

    /* Find the reference */
    if((uint32_t)index < events_unlocked.size())
      found_event = &events_unlocked[index];
  }

  return found_event;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Add calls, for lists */
void EventSet::addEventUnlocked(Event new_event)
{
  events_unlocked.push_back(new_event);
}

/* Clears all event data within the class */
void EventSet::clear()
{
  get_index = -1;
  unlocked_state = UnlockedState::NONE;

  unsetEventLocked();
  unsetEventUnlocked();
  unsetLocked();
}

/* Returns the event when accessed - depending on locked unlocked status */
Event EventSet::getEvent(bool trigger)
{
  Event* found_event = nullptr;

  /* If locked, use locked event */
  if(isLocked())
  {
    if(!event_locked.one_shot ||
       (event_locked.one_shot && !event_locked.has_exec))
    {
      found_event = &event_locked;
    }
  }
  /* Otherwise, unlocked */
  else if(unlocked_state != UnlockedState::NONE && events_unlocked.size() > 0)
  {
    /* Determine valid events */
    std::vector<Event*> valid;
    for(uint32_t i = 0; i < events_unlocked.size(); i++)
    {
      if(!events_unlocked[i].one_shot ||
         (events_unlocked[i].one_shot && !events_unlocked[i].has_exec))
      {
        valid.push_back(&events_unlocked[i]);
      }
    }

    /* Determine event */
    if(valid.size() > 0)
    {
      /* -- Ordered Selection -- */
      if(unlocked_state == UnlockedState::ORDERED)
      {
        bool found = false;

        /* Initial index setting */
        uint32_t ref_index = 0;
        uint32_t start_index = 0;
        if(get_index >= 0)
          start_index = get_index + 1;
        if(start_index >= events_unlocked.size())
          start_index = 0;

        /* Loop through all available and try to find an event */
        for(uint32_t i = 0; !found && i < events_unlocked.size(); i++)
        {
          /* Reference index */
          ref_index = start_index + i;
          if(ref_index >= events_unlocked.size())
            ref_index -= events_unlocked.size();

          /* Check event */
          if(!events_unlocked[ref_index].one_shot ||
             (events_unlocked[ref_index].one_shot &&
              !events_unlocked[ref_index].has_exec))
          {
            found = true;
            found_event = &events_unlocked[ref_index];
          }
        }

        /* Take the start index and save it to the get index */
        if(found)
          get_index = ref_index;
      }
      /* -- Random Selection -- */
      else if(unlocked_state == UnlockedState::RANDOM)
      {
        int index = Helpers::randInt(valid.size() - 1);
        found_event = valid[index];
      }
    }

    /* If valid event found and unlocked, re-lock if lock is not permanent */
    if(found_event != nullptr && trigger)
      locked_status = unlockUsed(locked_status);
  }

  /* Trigger and return */
  if(found_event != nullptr)
  {
    if(trigger)
      found_event->has_exec = true;
    return *found_event;
  }
  return createBlankEvent();
}

/* Individual getters for events */
Event EventSet::getEventLocked()
{
  return event_locked;
}

/* Individual getters for events */
std::vector<Event> EventSet::getEventUnlocked()
{
  return events_unlocked;
}

/* Individual getters for events */
Event EventSet::getEventUnlocked(int index)
{
  if(index >= 0 && (uint32_t)index < events_unlocked.size())
    return events_unlocked[index];
  return createBlankEvent();
}

/* Returns the locked state struct */
Locked EventSet::getLockedState()
{
  return locked_status;
}

/* Returns the unlocked state enum */
UnlockedState EventSet::getUnlockedState()
{
  return unlocked_state;
}

/* Returns if the class is currently locked */
bool EventSet::isLocked()
{
  return locked_status.is_locked;
}

/* Load data from file */
bool EventSet::loadData(XmlData data, int file_index, int section_index)
{
  std::string category = data.getElement(file_index);

  /* Parse */
  /* -- Lock Information -- */
  if(category == "lock")
  {
    locked_status = updateLocked(locked_status, data, file_index + 1);
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

  return true;
}

/* Individual setters for events */
void EventSet::setEventLocked(Event new_event)
{
  unsetEventLocked();
  event_locked = new_event;
}

/* Individual setters for events */
bool EventSet::setEventUnlocked(int index, Event new_event, bool replace)
{
  bool success = false;

  if(index >= 0)
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

/* Sets the locked status */
void EventSet::setLocked(Locked new_locked)
{
  locked_status = new_locked;
}

/* Sets the unlocked state */
void EventSet::setUnlockedState(UnlockedState state)
{
  unlocked_state = state;
}

/* Unset calls */
void EventSet::unsetEventLocked()
{
  event_locked = deleteEvent(event_locked);
}

/* Unset calls */
void EventSet::unsetEventUnlocked(int index)
{
  if(index >= 0 && (uint32_t)index < events_unlocked.size())
  {
    Event delete_event = events_unlocked[index];
    events_unlocked.erase(events_unlocked.begin() + index);
    deleteEvent(delete_event);
  }
}

/* Unset calls */
void EventSet::unsetEventUnlocked()
{
  for(uint16_t i = 0; i < events_unlocked.size(); i++)
    deleteEvent(events_unlocked[i]);
  events_unlocked.clear();
}

/* Unset calls */
void EventSet::unsetLocked()
{
  locked_status = createBlankLocked();
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/* Copies a passed in event */
Event EventSet::copyEvent(Event source)
{
  /* Copy the event */
  Event event;
  event.classification = source.classification;
  event.convo = nullptr;
  event.ints = source.ints;
  event.sound_id = source.sound_id;
  event.strings = source.strings;

  /* If convo, do the proper copy */
  if(event.classification == EventClassifier::STARTCONVO &&
     source.convo != nullptr)
  {
    event.convo = new Conversation;
    event.convo->action_event = source.convo->action_event;
    event.convo->category = source.convo->category;
    event.convo->next = source.convo->next;
    event.convo->text = source.convo->text;
    event.convo->thing_id = source.convo->thing_id;
  }

  return event;
}

/* Creates a blank conversation */
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

/* Create blank event */
Event EventSet::createBlankEvent()
{
  Event blank_event;

  blank_event.classification = EventClassifier::NOEVENT;
  blank_event.convo = nullptr;
  blank_event.ints.clear();
  blank_event.sound_id = kUNSET_ID;
  blank_event.strings.clear();

  blank_event.one_shot = false;
  blank_event.has_exec = false;

  return blank_event;
}

/* Creates a blank locked struct */
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

/* Creates the conversation initiation event */
Event EventSet::createEventConversation(Conversation* new_conversation,
                                        int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::STARTCONVO;
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

/* Creates a give item event, with the appropriate parameters */
Event EventSet::createEventGiveItem(int id, int count, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::GIVEITEM;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(id);
  new_event.ints.push_back(count);

  return new_event;
}

/* Creates a notification event, that can fire and result in visible text */
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

/* Creates a sound event */
Event EventSet::createEventSound(int sound_id)
{
  /* Create the new event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::JUSTSOUND;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  return new_event;
}

/* Creates a start battle event */
Event EventSet::createEventStartBattle(int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::RUNBATTLE;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  return new_event;
}

/* Create a start map event */
Event EventSet::createEventStartMap(int id, int sound_id)
{
  /* Create the event and identify */
  Event new_event = createBlankEvent();
  new_event.classification = EventClassifier::RUNMAP;
  if(sound_id >= 0)
    new_event.sound_id = sound_id;

  /* Fill in the event specific information */
  new_event.ints.push_back(id);

  return new_event;
}

/* Creates a teleport event */
Event EventSet::createEventTeleport(int thing_id, uint16_t tile_x,
                                    uint16_t tile_y, uint16_t section_id,
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

/* Creates a have item check based lock */
Locked EventSet::createLockHaveItem(int id, int count, bool consume,
                                    bool permanent)
{
  /* Create the new lock struct and identify */
  Locked new_locked = createBlankLocked();
  new_locked.state = LockedState::ITEM;
  new_locked.permanent = permanent;

  /* Fill in the locked state specific information */
  new_locked.bools.push_back(consume);
  new_locked.ints.push_back(id);
  new_locked.ints.push_back(count);

  return new_locked;
}

/* Created a trigger based lock */
Locked EventSet::createLockTriggered(bool permanent)
{
  /* Create the new lock struct and identify */
  Locked new_locked = createBlankLocked();
  new_locked.state = LockedState::TRIGGER;
  new_locked.permanent = permanent;

  return new_locked;
}

/* Deletes the given event. Just clears the relevant memory */
Event EventSet::deleteEvent(Event event)
{
  /* Delet the existing event, if relevant */
  if(event.convo != nullptr)
    delete event.convo;
  event.convo = nullptr;

  return createBlankEvent();
}

/* Returns the conversation at the given index */
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

/* Unlocks the locked state, if the conditions are met */
Locked EventSet::unlockItem(Locked locked_state, int id, int count,
                            bool &consume)
{
  /* Starting values */
  consume = false;

  if(locked_state.state == LockedState::ITEM &&
     locked_state.ints.size() > kHAVE_ITEM_COUNT &&
     locked_state.bools.size() > kHAVE_ITEM_CONSUME &&
     locked_state.is_locked)
  {
    if(locked_state.ints[kHAVE_ITEM_ID] == id &&
       locked_state.ints[kHAVE_ITEM_COUNT] == count)
    {
      locked_state.is_locked = false;
      consume = locked_state.bools[kHAVE_ITEM_CONSUME];
    }
  }

  return locked_state;
}

/* Unlocks the locked state, if the conditions are met */
Locked EventSet::unlockTrigger(Locked locked_state)
{
  if(locked_state.state == LockedState::TRIGGER)
  {
    locked_state.is_locked = false;
  }

  return locked_state;
}

/* Unlock state used of locked state - will re-lock if not permanent */
Locked EventSet::unlockUsed(Locked locked_state)
{
  if(locked_state.state != LockedState::NONE && !locked_state.permanent)
    locked_state.is_locked = true;
  return locked_state;
}

/* Updates the conversation values of the pointed object, based on the XML
 * file data */
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

/* Updates the event from the data in the file */
Event EventSet::updateEvent(Event event, XmlData data, int file_index,
                            int section_index)
{
  EventClassifier category = EventClassifier::NOEVENT;
  std::string category_str = data.getElement(file_index);
  bool read_success;

  /* Determine the category of the event that is being updated */
  if(category_str == "conversation")
    category = EventClassifier::STARTCONVO;
  else if(category_str == "giveitem")
    category = EventClassifier::GIVEITEM;
  else if(category_str == "justsound")
    category = EventClassifier::JUSTSOUND;
  else if(category_str == "notification")
    category = EventClassifier::NOTIFICATION;
  else if(category_str == "startbattle")
    category = EventClassifier::RUNBATTLE;
  else if(category_str == "startmap")
    category = EventClassifier::RUNMAP;
  else if(category_str == "teleportthing")
    category = EventClassifier::TELEPORTTHING;

  /* If the category doesn't match, create a new event */
  if(category != event.classification)
  {
    event = deleteEvent(event);

    if(category == EventClassifier::GIVEITEM)
      event = createEventGiveItem();
    else if(category == EventClassifier::JUSTSOUND)
      event = createEventSound();
    else if(category == EventClassifier::NOTIFICATION)
      event = createEventNotification();
    else if(category == EventClassifier::RUNBATTLE)
      event = createEventStartBattle();
    else if(category == EventClassifier::RUNMAP)
      event = createEventStartMap();
    else if(category == EventClassifier::STARTCONVO)
      event = createEventConversation();
    else if(category == EventClassifier::TELEPORTTHING)
      event = createEventTeleport();
  }

  /* Proceed to set up the event with the marked changes */
  if(data.getElement(file_index + 1) == "one_shot")
  {
    bool one_shot = data.getDataBool(&read_success);
    if(read_success)
      event.one_shot = one_shot;
  }
  /* -- SOUND -- */
  else if(data.getElement(file_index + 1) == "sound_id" ||
          category == EventClassifier::JUSTSOUND)
  {
    int32_t sound_id = data.getDataInteger(&read_success);
    if(read_success && sound_id >= 0)
      event.sound_id = sound_id;
  }
  /* -- GIVE ITEM -- */
  else if(category == EventClassifier::GIVEITEM)
  {
    std::string give_item_element = data.getElement(file_index + 1);
    if(give_item_element == "id")
      event.ints.at(kGIVE_ITEM_ID) = data.getDataInteger();
    else if(give_item_element == "count")
      event.ints.at(kGIVE_ITEM_COUNT) = data.getDataInteger();
  }
  /* -- NOTIFICATION -- */
  else if(category == EventClassifier::NOTIFICATION)
  {
    if(data.getTailElements(file_index).size() == 1 ||
       data.getElement(file_index + 1) == "text")
    {
      event.strings.at(0) = data.getDataString();
    }
  }
  /* -- START BATTLE -- */
  else if(category == EventClassifier::RUNBATTLE)
  {
  }
  /* -- CHANGE MAPS -- */
  else if(category == EventClassifier::RUNMAP)
  {
    std::string element = data.getElement(file_index + 1);
    if(element == "id")
      event.ints.at(kMAP_ID) = data.getDataInteger();
  }
  /* -- START CONVERSATION -- */
  else if(category == EventClassifier::STARTCONVO)
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
  /* -- TELEPORT THING -- */
  else if(category == EventClassifier::TELEPORTTHING)
  {
    std::string teleport_element = data.getElement(file_index + 1);
    if(teleport_element == "id")
      event.ints.at(kTELEPORT_ID) = data.getDataInteger();
    if(teleport_element == "x")
      event.ints.at(kTELEPORT_X) = data.getDataInteger();
    else if(teleport_element == "y")
      event.ints.at(kTELEPORT_Y) = data.getDataInteger();
    else if(teleport_element == "section")
      event.ints.at(kTELEPORT_SECTION) = data.getDataInteger();
  }

  return event;
}

/* Updates the event with one shot information */
Event EventSet::updateEventOneShot(Event event, bool one_shot)
{
  event.one_shot = one_shot;
  return event;
}

/* Update the locked struct from the data in the file */
Locked EventSet::updateLocked(Locked locked_curr, XmlData data, int file_index)
{
  LockedState state = LockedState::NONE;
  std::string state_str = data.getElement(file_index);
  bool read_success;

  /* Determine the state of the lock that is being updated */
  if(state_str == "item")
    state = LockedState::ITEM;
  else if(state_str == "trigger")
    state = LockedState::TRIGGER;

  /* If the state doesn't match, create a new locked state - with fresh data */
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
  {
  }

  return locked_curr;
}

