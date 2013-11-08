/******************************************************************************
 * Class Name: MapState
 * Date Created: May 23, 2013
 * Inheritance: none
 * Description: This class becomes the middle interface for a sprite and map
 *              actions to be tied together.
 *****************************************************************************/
#include "Game/Map/MapState.h"

/* Constant Implementation - see header file for descriptions */
const float MapState::kMAX_OPACITY = 1.0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapState::MapState()
{
  animation = 0;
  event_handler = 0;
  setEventHandler(0);
  interaction = NOINTERACTION;
  opacity = kMAX_OPACITY;
}

MapState::MapState(Sprite* animation, EventHandler* event_handler, 
                   float opacity)
{
  this->animation = 0;
  this->event_handler = 0;
  interaction = NOINTERACTION;

  /* Set the relevant data */
  setSprite(animation);
  setEventHandler(event_handler);
  setOpacity(opacity);
}

MapState::~MapState()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Returns the interaction, based on the string. Returns NOINTERACTION if
 * nothing found */
MapState::InteractionState MapState::getInteraction(QString interaction)
{
  if(interaction.toLower() == "use")
    return USE;
  else if(interaction.toLower() == "walkoff")
    return WALKOFF;
  else if(interaction.toLower() == "walkon")
    return WALKON;
  else
    return NOINTERACTION;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Clear out the state definition */
void MapState::clear()
{
  clearEvents();
  unsetSprite();
}

/*
 * Description: Clears all events stored within the class. Call only works if 
 *              the event handler has already been set up and is available.
 *              Note that if there is no event handler, no events fire.
 *
 * Inputs: none
 * Output: bool - status if the events were cleared.
 */
bool MapState::clearEvents()
{
  if(event_handler != 0)
  {
    enter_event = event_handler->deleteEvent(enter_event);
    exit_event = event_handler->deleteEvent(exit_event);
    use_event = event_handler->deleteEvent(use_event);
    walkover_event = event_handler->deleteEvent(walkover_event);

    return true;
  }
  
  return false;
}
  
/* Returns what interaction would initiate this */
MapState::InteractionState MapState::getInteraction()
{
  return interaction;
}

/* Returns the painted opacity of the state */
float MapState::getOpacity()
{
  return opacity;
}
  
/* Returns the sprite stored in the state for control/usage */
Sprite* MapState::getSprite()
{
  return animation;
}

bool MapState::initializeGl()
{
  bool status = true;

  if(animation != 0)
    status &= animation->initializeGl();

  return status;
}

/*
 * Description: Sets the enter event. This goes off when the interactive object
 *              operation enters onto this state.
 *
 * Inputs: Event enter_event - the event to be executed
 * Output: bool - if the setting was able to occur
 */
bool MapState::setEnterEvent(Event enter_event)
{
  if(event_handler != 0)
  {
    /* Delete the existing event and set the new event */
    this->enter_event = event_handler->deleteEvent(this->enter_event);
    this->enter_event = enter_event;
    return true;
  }

  return false;
}

/*
 * Description: Sets the event handler to create and manage all existing events
 *              that get fired throughout interaction with the class. This is 
 *              necessary to ensure that any events work.
 *
 * Inputs: EventHandler* event_handler - the new handler pointer (must not be 0)
 * Output: none
 */
void MapState::setEventHandler(EventHandler* event_handler)
{
  /* Clean up the existing event handler */
  clearEvents();

  /* Clear out the conversation data */
  enter_event.convo = 0;
  exit_event.convo = 0;
  use_event.convo = 0;
  walkover_event.convo = 0;

  /* Set the new event handler and clean up the interact event */
  this->event_handler = event_handler;
  if(event_handler != 0)
    clearEvents();
}

/*
 * Description: Sets the exit event. This goes off when the interactive object
 *              operation exits this state.
 *
 * Inputs: Event exit_event - the event to be executed
 * Output: bool - status if the event could be set
 */
bool MapState::setExitEvent(Event exit_event)
{
  if(event_handler != 0)
  {
    /* Delet the existing event and set the new event */
    this->exit_event = event_handler->deleteEvent(this->exit_event);
    this->exit_event = exit_event;
    return true;
  }

  return false;
}
  
/* Sets how this state gets interacted with */
bool MapState::setInteraction(InteractionState interaction)
{
  this->interaction = interaction;
  return true;
}

/* Sets how this state is interacted with from string */
bool MapState::setInteraction(QString interaction)
{
  InteractionState new_interaction = getInteraction(interaction);
  if(!interaction.isEmpty())
  {
    this->interaction = new_interaction;
    return true;
  }

  return false;
}

/* Sets the opacity of the painted state (0 - 1.0) */
void MapState::setOpacity(float opacity)
{
  if(opacity < 0)
    this->opacity = 0.0;
  else if(opacity > kMAX_OPACITY)
    this->opacity = kMAX_OPACITY;
  else
    this->opacity = opacity;
}
  
bool MapState::setSprite(Sprite* animation)
{
  if(animation != 0 && animation->getSize() > 0)
  {
    unsetSprite();
    this->animation = animation;
    //this->animation->initializeGl(); // TODO?
    return true;
  }
  return false;
}

/*
 * Description: Sets the use event. This goes off when the interactive 
 *              object is interacted with on this state. This will only work
 *              if this state is the last one in the sequence and doesn't 
 *              have a corresponding next state/transition frames.
 *
 * Inputs: Event use_event - the event to be executed
 * Output: bool - if the setting was able to occur
 */
bool MapState::setUseEvent(Event use_event)
{
  if(event_handler != 0)
  {
    /* Delete the existing event and set the new event */
    this->use_event = event_handler->deleteEvent(this->use_event);
    this->use_event = use_event;
    return true;
  }

  return false;
}

/*
 * Description: Sets the walkover event. This goes off when the interactive 
 *              object is on this state and the person walks on it.
 *
 * Inputs: Event walkover_event - the event to be executed
 * Output: bool - if the setting was able to occur
 */
bool MapState::setWalkoverEvent(Event walkover_event)
{
  if(event_handler != 0)
  {
    /* Delete the existing event and set the new event */
    this->walkover_event = event_handler->deleteEvent(this->walkover_event);
    this->walkover_event = walkover_event;
    return true;
  }

  return false;
}
  
bool MapState::triggerEnterEvent(MapPerson* initiator)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != 0 && enter_event.classification != EnumDb::NOEVENT)
  {
    event_handler->executeEvent(enter_event, initiator);
    return true;
  }

  return false;
}

bool MapState::triggerExitEvent(MapPerson* initiator)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != 0 && exit_event.classification != EnumDb::NOEVENT)
  {
    event_handler->executeEvent(exit_event, initiator);
    return true;
  }

  return false;
}

bool MapState::triggerUseEvent(MapPerson* initiator)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != 0 && use_event.classification != EnumDb::NOEVENT)
  {
    event_handler->executeEvent(use_event, initiator);
    return true;
  }

  return false;
}

bool MapState::triggerWalkoverEvent(MapPerson* initiator)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != 0 && 
     walkover_event.classification != EnumDb::NOEVENT)
  {
    event_handler->executeEvent(walkover_event, initiator);
    return true;
  }

  return false;
}

void MapState::unsetSprite()
{
  delete animation;
  animation = 0;
}
