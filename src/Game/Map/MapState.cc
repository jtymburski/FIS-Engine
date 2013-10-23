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
  interaction = NOINTERACTION;
  opacity = kMAX_OPACITY;
  passable = false;
}

MapState::MapState(Sprite* animation, EventHandler* event_handler, 
                   float opacity, bool passable)
{
  this->animation = 0;
  this->event_handler = 0;
  interaction = NOINTERACTION;

  setSprite(animation);
  setEventHandler(event_handler);
  setOpacity(opacity);
  setPassable(passable);
}

MapState::~MapState()
{
  clear();
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
    setEnterEvent(event_handler->createBlankEvent());
    setExitEvent(event_handler->createBlankEvent());
    setUseEvent(event_handler->createBlankEvent());
    setWalkoverEvent(event_handler->createBlankEvent());

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

/* Returns if the state is passable */
bool MapState::isPassable()
{
  return passable;
}

/*
 * Description: Sets the enter event. This goes off when the interactive object
 *              operation enters onto this state.
 *
 * Inputs: Event enter_event - the event to be executed
 * Output: bool - if the setting was able to occur
 */
bool MapState::setEnterEvent(EventHandler::Event enter_event)
{
  if(event_handler != 0)
  {
    /* Delete the existing event, if relevant */
    if(this->enter_event.convo != 0)
      delete this->enter_event.convo;
    this->enter_event.convo = 0;

    /* Set the new event */
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
  if(this->event_handler != 0)
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
bool MapState::setExitEvent(EventHandler::Event exit_event)
{
  if(event_handler != 0)
  {
    /* Delet the existing event, if relevant */
    if(this->exit_event.convo != 0)
      delete this->exit_event.convo;
    this->exit_event.convo = 0;

    /* Set the new event */
    this->exit_event = exit_event;
    return true;
  }

  return false;
}
  
/* Sets how this state gets interacted with */
void MapState::setInteraction(InteractionState interaction)
{
  this->interaction = interaction;
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
  
/* Sets the passability and if this state restricts walking */
void MapState::setPassable(bool passable)
{
  this->passable = passable;
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
bool MapState::setUseEvent(EventHandler::Event use_event)
{
  if(event_handler != 0)
  {
    /* Delete the existing event, if relevant */
    if(this->use_event.convo != 0)
      delete this->use_event.convo;
    this->use_event.convo = 0;

    /* Set the new event */
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
bool MapState::setWalkoverEvent(EventHandler::Event walkover_event)
{
  if(event_handler != 0)
  {
    /* Delete the existing event, if relevant */
    if(this->walkover_event.convo != 0)
      delete this->walkover_event.convo;
    this->walkover_event.convo = 0;

    /* Set the new event */
    this->walkover_event = walkover_event;
    return true;
  }

  return false;
}

void MapState::unsetSprite()
{
  delete animation;
  animation = 0;
}
