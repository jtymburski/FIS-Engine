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
  MapState(Sprite* animation, EventHandler* event_handler = 0);

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

  /* Stores the interaction state for changing state */
  InteractionState interaction;

  /* Events necessary for the state */
  EventHandler* event_handler;
  Event enter_event;
  Event exit_event;
  Event use_event;
  Event walkover_event;

  /* -------------------------- Constants ------------------------- */
  //const static float kMAX_OPACITY; /* The max opacity allowable (0-1.0) */
 
/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Returns the interaction, based on the string. Returns NOINTERACTION if
   * nothing found */
  InteractionState getInteraction(QString interaction); 

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clear out the state definition and data from the class */
  void clear();
  bool clearEvents();

  /* Returns the enter and exit events */
  Event getEnterEvent();
  Event getExitEvent();
  
  /* Returns what interaction would initiate this */
  InteractionState getInteraction();
  
  /* Returns the sprite stored in the state for control/usage */
  Sprite* getSprite();
  
  /* Returns the use and walkover events */
  Event getUseEvent();
  Event getWalkoverEvent();
  
  /* Initializes GL in all the frames stored within this state */
  bool initializeGl();
  
  /* Sets the enter event */
  bool setEnterEvent(Event enter_event);

  /* Sets the event handler - this call clears out all existing events */
  void setEventHandler(EventHandler* event_handler);

  /* Sets the exit event */
  bool setExitEvent(Event exit_event);

  /* Sets how this state gets interacted with */
  bool setInteraction(InteractionState interaction);
  bool setInteraction(QString interaction);
  
  /* Sets the sprite internally to the state */
  bool setSprite(Sprite* animation);

  /* Sets the use event */
  bool setUseEvent(Event use_event);

  /* Sets the walkover event */
  bool setWalkoverEvent(Event walkover_event);

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
  float opacity;
  bool passable;

  StateNode* previous;
  StateNode* next;
};

#endif // MAPSSTATE_H
