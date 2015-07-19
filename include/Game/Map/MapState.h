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
#include "Game/Map/SpriteMatrix.h"

class MapState
{
public:
  /* Constructor functions */
  MapState(EventHandler* event_handler = NULL);

  /* Destructor function */
  ~MapState();

  /* NOINTERACTION - no interaction to transfer states
   * WALKON - walk on by person to initiate interaction
   * WALKOFF - walk off by person to initiate interaction
   * USE - use key to initiate interaction */
  enum InteractionState{NOINTERACTION = 0, WALKON = 1, WALKOFF = 2, USE = 3};

private:
  /* The information that defines the animation image data for the sprite */
  SpriteMatrix* sprite_set;

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
  InteractionState getInteraction(std::string interaction); 

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds the matrix information from the XML data classifier from file */
  bool addFileInformation(XmlData data, int file_index, int section_index, 
                          SDL_Renderer* renderer, std::string base_path = "");

  /* Clear out the state definition and data from the class */
  void clear();
  bool clearEvents();

  /* Returns the enter and exit events */
  Event getEnterEvent();
  Event getExitEvent();
  
  /* Returns what interaction would initiate this */
  InteractionState getInteraction();
 
  /* Returns the matrix stored in the state for control/usage */
  SpriteMatrix* getMatrix();

  /* Returns the sprite stored in the state for control/usage */
  TileSprite* getSprite(uint16_t x, uint16_t y);

  /* Returns the use and walkover events */
  Event getUseEvent();
  Event getWalkoverEvent();
 
  /* Check if events or interactions are set */
  bool isEnterEventSet();
  bool isExitEventSet();
  bool isUseEventSet();
  bool isWalkInteraction();
  bool isWalkoverEventSet();

  /* Sets the enter event */
  bool setEnterEvent(Event enter_event);

  /* Sets the event handler - this call clears out all existing events */
  void setEventHandler(EventHandler* event_handler);

  /* Sets the exit event */
  bool setExitEvent(Event exit_event);

  /* Sets how this state gets interacted with */
  bool setInteraction(InteractionState interaction);
  bool setInteraction(std::string interaction);

  /* Sets the matrix stored in the state */
  bool setMatrix(SpriteMatrix* matrix);

  /* Sets the sprite internally to the state */
  bool setSprite(TileSprite* frames, uint16_t x, uint16_t y, 
                 bool delete_old = true);

  /* Sets the use event */
  bool setUseEvent(Event use_event);

  /* Sets the walkover event */
  bool setWalkoverEvent(Event walkover_event);

  /* The event triggers. Fire to start the event */
  bool triggerEnterEvent(MapPerson* initiator);
  bool triggerExitEvent(MapPerson* initiator);
  bool triggerUseEvent(MapPerson* initiator);
  bool triggerWalkoverEvent(MapPerson* initiator);

  /* Unsets the matrix internally from the state */
  void unsetMatrix();
};

/*
 * Structure which handles the node based selection for states that are used
 * within the interactive object
 */
struct StateNode
{
  MapState* state;
  SpriteMatrix* transition;

  StateNode* previous;
  StateNode* next;
};

#endif // MAPSSTATE_H
