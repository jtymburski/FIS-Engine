/******************************************************************************
 * Class Name: MapState
 * Date Created: May 23, 2013
 * Inheritance: none
 * Description: This class becomes the middle interface for a sprite and map
 *              actions to be tied together.
 *****************************************************************************/
#ifndef MAPSTATE_H
#define MAPSTATE_H

#include "Game/EventHandler.h"
#include "Game/Sprite.h"

class MapState
{
public:
  /* Constructor functions */
  MapState();
  MapState(Sprite* animation, EventHandler* event_handler = 0,
           float opacity = kMAX_OPACITY);

  /* Destructor function */
  ~MapState();

  /* NOINTERACTION - no interaction to transfer states
   * WALKON - walk on by person to initiate interaction
   * WALKOFF - walk off by person to initiate interaction
   * USE - use key to initiate interaction */
  enum InteractionState{NOINTERACTION, WALKON, WALKOFF, USE};

private:
  /* The information that defines the animation image data for the sprite */
  Sprite* animation;
  float opacity;

  /* Stores the interaction state for changing state */
  InteractionState interaction;

  /* Events necessary for the state */
  EventHandler* event_handler;
  EventHandler::Event enter_event;
  EventHandler::Event exit_event;
  EventHandler::Event use_event;
  EventHandler::Event walkover_event;

  /* -------------------------- Constants ------------------------- */
  const static float kMAX_OPACITY; /* The max opacity allowable (0-1.0) */
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clear out the state definition and data from the class */
  void clear();
  bool clearEvents();

  /* Returns what interaction would initiate this */
  InteractionState getInteraction();

  /* Returns the painted opacity of the state */
  float getOpacity();
  
  /* Returns the sprite stored in the state for control/usage */
  Sprite* getSprite();
  
  /* Initializes GL in all the frames stored within this state */
  bool initializeGl();
  
  /* Sets the enter event */
  bool setEnterEvent(EventHandler::Event enter_event);

  /* Sets the event handler - this call clears out all existing events */
  void setEventHandler(EventHandler* event_handler);

  /* Sets the exit event */
  bool setExitEvent(EventHandler::Event exit_event);

  /* Sets how this state gets interacted with */
  void setInteraction(InteractionState interaction);

  /* Sets the opacity of the painted state (0 - 1.0) */
  void setOpacity(float opacity);
  
  /* Sets the sprite internally to the state */
  bool setSprite(Sprite* animation);

  /* Sets the use event */
  bool setUseEvent(EventHandler::Event use_event);

  /* Sets the walkover event */
  bool setWalkoverEvent(EventHandler::Event walkover_event);

  /* The event triggers. Fire to start the event */
  bool triggerEnterEvent(MapPerson* initiator);
  bool triggerExitEvent(MapPerson* initiator);
  bool triggerUseEvent(MapPerson* initiator);
  bool triggerWalkoverEvent(MapPerson* initiator);

  /* Unsets the sprite internally to the state */
  void unsetSprite();
};

struct StateNode
{
  MapState* state;
  Sprite* transition;
  bool passable;

  StateNode* previous;
  StateNode* next;
};

#endif // MAPSSTATE_H
