/******************************************************************************
 * Class Name: MapState
 * Date Created: May 23, 2013
 * Inheritance: none
 * Description: This class becomes the middle interface for a sprite and map
 *              actions to be tied together.
 *****************************************************************************/
#include "Game/Map/MapState.h"

/* Constant Implementation - see header file for descriptions */
//const float MapState::kMAX_OPACITY = 1.0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: The only constructor for the state. Defualts the handler to
 *              NULL as the blank state. Sets up all the initial variables to
 *              blank states.
 *
 * Inputs: EventHandler* event_handler - the event coordinator, for all triggers
 */
MapState::MapState(EventHandler* event_handler)
{
  this->event_handler = NULL;
  interaction = NOINTERACTION;
  sprite_set = new SpriteMatrix();

  /* Set the relevant data */
  setEventHandler(event_handler);
}

/*
 * Description: Destructor function
 */
MapState::~MapState()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the interaction, based on the passed in string. Returns
 *              NOINTERACTION if the string doesn't match any.
 *
 * Inputs: std::string interaction - the interacting string from file
 * Output: InteractionState - the corresponding enum for interaction
 */
MapState::InteractionState MapState::getInteraction(std::string interaction)
{
  if(interaction == "use")
    return USE;
  else if(interaction == "walkoff")
    return WALKOFF;
  else if(interaction == "walkon")
    return WALKON;
  else
    return NOINTERACTION;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Add the file data information, based on the xml data pointer
 *              retrieved from the file handler during an XML read. The internal
 *              elements get offset based on the given index.
 *
 * Inputs: XmlData data - the xml data storage class
 *         int file_index - the element offset index to the sprite data
 *         int section_index - the map section index of the thing
 *         SDL_Renderer* renderer - the rendering engine to add the info
 *         std::string base_path - the base path for resources
 * Output: bool - true if the add was successful
 */
bool MapState::addFileInformation(XmlData data, int file_index,
                                  int section_index, SDL_Renderer* renderer,
                                  std::string base_path)
{
  bool success = true;
  std::vector<std::string> elements = data.getTailElements(file_index);
  std::string identifier = data.getElement(file_index);

  /*--------------------- SPRITE INFO -----------------*/
  if(identifier == "sprites")
  {
    if(sprite_set == NULL)
      sprite_set = new SpriteMatrix();

    success &= sprite_set->addFileInformation(data, file_index + 1,
                                              renderer, base_path);
  }
  /*--------------------- INTERACTION -----------------*/
  else if(identifier == "interaction")
  {
    success &= setInteraction(data.getDataString(&success));
  }
  /*------------------ ENTER EVENT -----------------*/
  else if(identifier == "enterevent")
  {
    event_enter.loadData(data, file_index, section_index);
  }
  /*------------------ EXIT EVENT -----------------*/
  else if(identifier == "exitevent")
  {
    event_exit.loadData(data, file_index, section_index);
  }
  /*------------------ USE EVENT -----------------*/
  else if(identifier == "useevent")
  {
    event_use.loadData(data, file_index, section_index);
  }
  /*---------------- WALKOVER EVENT -----------------*/
  else if(identifier == "walkoverevent")
  {
    event_walkover.loadData(data, file_index, section_index);
  }

  return success;
}

/*
 * Description: Clears all variables in the state class definition and unsets
 *              the sprite data.
 *
 * Inputs: none
 * Output: none
 */
void MapState::clear()
{
  clearEvents();
  unsetMatrix();
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
  event_enter.clear();
  event_exit.clear();
  event_use.clear();
  event_walkover.clear();
  return true;
}

/*
 * Description: Returns the enter event (when this state becomes prime) set
 *              for the class.
 *
 * Inputs: none
 * Output: EventSet* - event reference, corresponding to the enter event
 */
EventSet* MapState::getEnterEvent()
{
  return &event_enter;
}

/*
 * Description: Returns the exit event (when this state stops becoming prime)
 *              set for the class.
 *
 * Inputs: none
 * Output: EventSet* - event reference, corresponding to the exit event
 */
EventSet* MapState::getExitEvent()
{
  return &event_exit;
}

/*
 * Description: Returns the interaction that will initiate a state shift.
 *
 * Inputs: none
 * Output: InteractionState - state enum, stored in the class
 */
MapState::InteractionState MapState::getInteraction()
{
  return interaction;
}

/*
 * Description: Returns the sprite matrix that corresponds to this state.
 *
 * Inputs: none
 * Output: SpriteMatrix* - matrix pointer. NULL if unset. DO NOT DELETE.
 */
SpriteMatrix* MapState::getMatrix()
{
  return sprite_set;
}

/*
 * Description: Returns the sprite at the corresponding x and y coordinates
 *              in the matrix. NULL if no sprite is located there.
 *
 * Inputs: uint16_t x - the x location. top left of matrix is 0,0
 * Output: TileSprite* - the sprite pointer reference. DO NOT DELETE.
 */
TileSprite* MapState::getSprite(uint16_t x, uint16_t y)
{
  TileSprite* null_sprite = NULL;

  if(sprite_set != NULL)
    return sprite_set->at(x, y);
  return null_sprite;
}

/*
 * Description: Returns the use event (when is prime and thing triggers it)
 *              set for the class.
 *
 * Inputs: none
 * Output: EventSet* - event reference, corresponding to the use event
 */
EventSet* MapState::getUseEvent()
{
  return &event_use;
}

/*
 * Description: Returns the walkover event (when is prime and thing walks on it)
 *              set for the class.
 *
 * Inputs: none
 * Output: EventSet* - event reference, corresponding to the walkover event
 */
EventSet* MapState::getWalkoverEvent()
{
  return &event_walkover;
}

/*
 * Description: Returns if the enter event for the state is set.
 *
 * Inputs: none
 * Output: bool - true if the enter event is set
 */
bool MapState::isEnterEventSet()
{
  return !event_enter.isEmpty();
}

/*
 * Description: Returns if the exit event for the state is set.
 *
 * Inputs: none
 * Output: bool - true if the exit event is set
 */
bool MapState::isExitEventSet()
{
  return !event_exit.isEmpty();
}

/*
 * Description: Returns if the use event for the state is set.
 *
 * Inputs: none
 * Output: bool - true if the use event is set
 */
bool MapState::isUseEventSet()
{
  return !event_use.isEmpty();
}

/*
 * Description: Returns if there is a walk interaction (either event or trigger)
 *
 * Inputs: none
 * Output: bool - true if there is a walk event or trigger set
 */
bool MapState::isWalkInteraction()
{
  if(interaction == WALKON || interaction == WALKOFF || isWalkoverEventSet())
    return true;
  return false;
}

/*
 * Description: Returns if the walkover event for the state is set.
 *
 * Inputs: none
 * Output: bool - true if the walkover event is set
 */
bool MapState::isWalkoverEventSet()
{
  return !event_walkover.isEmpty();
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
  this->event_handler = event_handler;
}

/*
 * Description: Sets the interaction trigger for the state (what shifts the
 *              states).
 *
 * Inputs: InteractionState interaction - the interaction corresponding enum
 * Output: bool - true if the interaction was set
 */
bool MapState::setInteraction(InteractionState interaction)
{
  this->interaction = interaction;
  return true;
}

/*
 * Description: Sets the interaction trigger for the state (what shifts the
 *              states) but based on a string from a file.
 *
 * Inputs: std::string interaction - the interaction corresponding string
 * Output: bool - true if the interaction was set
 */
bool MapState::setInteraction(std::string interaction)
{
  InteractionState new_interaction = getInteraction(interaction);
  if(!interaction.empty())
  {
    this->interaction = new_interaction;
    return true;
  }

  return false;
}

/*
 * Description: Sets the rendering sprite matrix, corresponding to the state.
 *              Fails if the matrix is NULL.
 *
 * Inputs: SpriteMatrix* matrix - sprite matrix pointer
 * Output: bool - true if the state matrix is set
 */
bool MapState::setMatrix(SpriteMatrix* matrix)
{
  if(matrix != NULL)
  {
    unsetMatrix();
    sprite_set = matrix;
    return true;
  }

  return false;
}

/*
 * Description: Sets a rendering sprite in the matrix, at the x and y location.
 *
 * Inputs: TileSprite* frames - a sprite pointer to set in the matrix
 *         uint16_t x - the x location in matrix (horizontal)
 *         uint16_t y - the y location in matrix (vertical)
 *         bool delete_old - delete old frame in its place?
 * Output: bool - true if sprite is set
 */
bool MapState::setSprite(TileSprite* frames, uint16_t x, uint16_t y,
                         bool delete_old)
{
  /* Make the matrix, if it doesn't exist */
  if(sprite_set == NULL)
    sprite_set = new SpriteMatrix();

  /* Add the frame */
  return sprite_set->setSprite(frames, x, y, delete_old);
}

/*
 * Description: Triggers enter event for the state by initiating person pointer.
 *
 * Inputs: MapPerson* initiator - the initiating person for the event
 *         MapThing* source - the source thing of the event. Default to NULL
 * Output: bool - true if the event is triggered
 */
bool MapState::triggerEnterEvent(MapPerson* initiator, MapThing* source)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != NULL && !event_enter.isEmpty())
  {
    event_handler->executeEventSet(getEnterEvent(), initiator, source);
    return true;
  }

  return false;
}

/*
 * Description: Triggers exit event for the state by initiating person pointer.
 *
 * Inputs: MapPerson* initiator - the initiating person for the event
 *         MapThing* source - the source thing of the event. Default to NULL
 * Output: bool - true if the event is triggered
 */
bool MapState::triggerExitEvent(MapPerson* initiator, MapThing* source)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != NULL && !event_exit.isEmpty())
  {
    event_handler->executeEventSet(getExitEvent(), initiator, source);
    return true;
  }

  return false;
}

/*
 * Description: Triggers use event for the state by initiating person pointer.
 *
 * Inputs: MapPerson* initiator - the initiating person for the event
 *         MapThing* source - the source thing of the event. Default to NULL
 * Output: bool - true if the event is triggered
 */
bool MapState::triggerUseEvent(MapPerson* initiator, MapThing* source)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != NULL && !event_use.isEmpty())
  {
    event_handler->executeEventSet(getUseEvent(), initiator, source);
    return true;
  }

  return false;
}

/*
 * Description: Triggers walk over event for the state by initiating person
 *              pointer.
 *
 * Inputs: MapPerson* initiator - the initiating person for the event
 *         MapThing* source - the source thing of the event. Default to NULL
 * Output: bool - true if the event is triggered
 */
bool MapState::triggerWalkoverEvent(MapPerson* initiator, MapThing* source)
{
  /* Only proceed with event if it's a valid event */
  if(event_handler != NULL && !event_walkover.isEmpty())
  {
    event_handler->executeEventSet(getWalkoverEvent(), initiator, source);
    return true;
  }

  return false;
}

/*
 * Description: Unsets the sprite matrix corresponding to the state.
 *
 * Inputs: none
 * Output: none
 */
void MapState::unsetMatrix()
{
  if(sprite_set != NULL)
    delete sprite_set;
  sprite_set = NULL;
}
