/******************************************************************************
* Class Name: MapPerson
* Date Created: Dec 2 2012
* Inheritance: MapThing
* Description: The MapPerson class. An addition on top of MapThing that 
*              expands the Thing into possible states to allow for walking
*              in multiple directions and on multiple surfaces. At present, 
*              it allows for the 4 directions (N,S,E,W) all on one surface
*              (Ground). Future expansion is available for other surfaces
*              such as water, flying, etc.
******************************************************************************/
#include "Game/Map/MapPerson.h"

/* Constant Implementation - see header file for descriptions */
const char MapPerson::kDIR_EAST = 1;
const char MapPerson::kDIR_NORTH = 0;
const char MapPerson::kDIR_SOUTH = 2;
const char MapPerson::kDIR_UNKNOWN = -1;
const char MapPerson::kDIR_WEST = 3;
const char MapPerson::kTOTAL_DIRECTIONS = 4;
const char MapPerson::kTOTAL_SURFACES   = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Sets up an empty person with no
 *              data.
 *
 * Inputs: none
 */
MapPerson::MapPerson()
{
  initializeStates();
  clearTarget();
  
  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = EnumDb::NORTH;
}

/* 
 * Description: Constructor for this class. Sets up a person with the
 *              appropriate height and width (for the tile) and other info
 *              that may be required by the person.
 *
 * Inputs: int width - the width of the tile for the person
 *         int height - the height of the tile for the person
 *         QString name - a string name for the person
 *         QString description - a description of the person
 *         int id - a unique numerical identifier, for the person
 */
MapPerson::MapPerson(int width, int height, QString name, 
                     QString description, int id)
{
  /* Class value setup */
  setDescription(description);
  setHeight(height);
  setID(id);
  setName(name);
  setWidth(width);

  /* Initializes the class information */
  initializeStates();
  clearTarget();

  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = EnumDb::NORTH;
}

/* 
 * Description: Destructor function 
 */
MapPerson::~MapPerson()
{
  clear();
  states.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Initializes the internal states for all possibilities, based
 *              on the constants. Only called once, by the constructor for 
 *              initialization.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::initializeStates()
{
  states.clear();

  for(int i = 0; i < kTOTAL_SURFACES; i++)
  {
    QVector<MapState*> row;

    for(int j = 0; j < kTOTAL_DIRECTIONS; j++)
      row.append(0);
    states.append(row);
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Adds the direction from the direction stack, if it's not
 *              currently on the stack. Used for movement of the person.
 *
 * Inputs: EnumDb::Direction direction - the direction to add
 * Output: none
 */
void MapPerson::addDirection(EnumDb::Direction direction)
{
  if(!movement_stack.contains(direction))
    movement_stack.append(direction);
}

/* 
 * Description: Integer converter from Direction enumerator to ensure that 
 *              the compiler type doesn't affect the operation. This info
 *              is used within internal array operation
 * 
 * Inputs: EnumDb::Direction dir - the direction enumerator
 * Output: int - the converted integer from the enumerator
 */
int MapPerson::dirToInt(EnumDb::Direction dir)
{
  if(dir == EnumDb::NORTH)
    return kDIR_NORTH;
  else if(dir == EnumDb::EAST)
    return kDIR_EAST;
  else if(dir == EnumDb::SOUTH)
    return kDIR_SOUTH;
  else if(dir == EnumDb::WEST)
    return kDIR_WEST;
  return kDIR_UNKNOWN;
}

/* 
 * Description: Direction enumerator converter from integer to ensure that 
 *              the compiler type doesn't affect the operation. This info
 *              is used within internal array operation
 * 
 * Inputs: int dir_index - the direction index, associated with the enum
 * Output: EnumDb::Direction - the direction enumerator, from the index
 */
EnumDb::Direction MapPerson::intToDir(int dir_index)
{
  if(dir_index == kDIR_NORTH)
    return EnumDb::NORTH;
  else if(dir_index == kDIR_EAST)
    return EnumDb::EAST;
  else if(dir_index == kDIR_SOUTH)
    return EnumDb::SOUTH;
  else if(dir_index == kDIR_WEST)
    return EnumDb::WEST;
  return EnumDb::DIRECTIONLESS;
}

/*
 * Description: Removes the direction from the direction stack, if it's
 *              currently on the stack. Used for movement of the person.
 *
 * Inputs: EnumDb::Direction direction - the direction to remove
 * Output: none
 */
void MapPerson::removeDirection(EnumDb::Direction direction)
{
  if(movement_stack.contains(direction))
    movement_stack.removeAt(movement_stack.indexOf(direction));
}

/* 
 * Description: Sets a new direction for the person on the map. It will update
 *              the parent frame so a new classifier is printed.
 * 
 * Inputs: MovementDirection direction - the new direction to set
 * Output: bool - indicates if the directional movement changed
 */
bool MapPerson::setDirection(EnumDb::Direction direction, bool set_movement)
{
  bool changed = (this->direction != direction);
  bool movement_changed = false;
  
  /* If moving, set the direction in map thing */
  if(set_movement)
    movement_changed = MapThing::setDirection(direction);
  else
    MapThing::setDirection(EnumDb::DIRECTIONLESS);

  /* If it's a movement direction, rotate the fellow */
  if(direction != EnumDb::DIRECTIONLESS)
  {
    if(changed && states[surface][dirToInt(direction)] != 0)
      MapThing::setState(states[surface][dirToInt(direction)], false);

    /* Finally set the in class direction */
    this->direction = direction;
  }
  
  return movement_changed;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Clears out all person states that were initialized into this
 *              class. This includes the appropriate procedure of cleaning
 *              up the parent class and deleting all the pointers.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::clear()
{
  for(int i = 0; i < kTOTAL_SURFACES; i++)
  {
    unsetState( (SurfaceClassifier)i, EnumDb::NORTH );
    unsetState( (SurfaceClassifier)i, EnumDb::EAST );
    unsetState( (SurfaceClassifier)i, EnumDb::SOUTH );
    unsetState( (SurfaceClassifier)i, EnumDb::WEST );
  }
  
  /* Clear direction and movement information */
  direction = EnumDb::NORTH;
  movement = EnumDb::DIRECTIONLESS;
  clearAllMovement();
  clearTarget();
  surface = GROUND;

  MapThing::clear();
}

/* 
 * Description: Clears all active movement pointers that are in the current
 *              movement stack. This allows to halt all movement once the
 *              person has reached the next tile.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::clearAllMovement()
{
  movement_stack.clear();
}

/* Clears the target that the map person is currently pointing at */
void MapPerson::clearTarget()
{
  target = 0;
}
  
/* 
 * Description: Returns the direction that the MapPerson is currently set to. 
 * 
 * Inputs: none
 * Output: EnumDb::Direction - the direction enumerator for this class
 */
EnumDb::Direction MapPerson::getDirection()
{
  return direction;
}

/* 
 * Description: This is a reimplemented call from MapThing, gets the actual
 *              move request which will be the last key pressed by the
 *              keyboard, since it's utilized as a stack.
 * 
 * Inputs: none
 * Output: EnumDb::Direction - the direction movement.
 */
EnumDb::Direction MapPerson::getMoveRequest()
{
  if(isMoveRequested())
    return movement_stack.last();
  return EnumDb::DIRECTIONLESS;
}

/* 
 * Description: Returns the surface classifier to what the map person is 
 *              standing on.
 * 
 * Inputs: none
 * Output: SurfaceClassifier - the surface enumerator for this class
 */
MapPerson::SurfaceClassifier MapPerson::getSurface()
{
  return surface;
}

/* Returns the target that this person is pointed at */
MapThing* MapPerson::getTarget()
{
  return target;
}
 
void MapPerson::keyFlush()
{
  movement_stack.clear();
}

/*
 * Description: The key press event implementation. Handles all the movement
 *              for the map person, if redirected here.
 *
 * Inputs: QKeyEvent* event - the key event to analyze
 * Output: none
 */
void MapPerson::keyPress(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down)
    addDirection(EnumDb::SOUTH);
  else if(event->key() == Qt::Key_Up)
    addDirection(EnumDb::NORTH);
  else if(event->key() == Qt::Key_Right)
    addDirection(EnumDb::EAST);
  else if(event->key() == Qt::Key_Left)
    addDirection(EnumDb::WEST);
  else if(event->key() == Qt::Key_E && !event->isAutoRepeat())
  {
    /* Initiate the thing and get the conversation, if applicable */
    emit initiateThingUse(this);

    /* Start the conversation, if one exists */
    if(target != 0)
    {
      Conversation thing_convo = target->getConversation();

      /* Check if the conversation is relevant and use if so */
      if(!thing_convo.text.isEmpty() || thing_convo.next.size() > 0)
      {
        target->initiateConversation(getDirection());
        keyFlush();
      }
    }
  }
}

/*
 * Description: The key release event implementation. Handles all the 
 *              movement for the map person, if redirected here.
 *
 * Inputs: QKeyEvent* event - the key event to analyze
 * Output: none
 */
void MapPerson::keyRelease(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down)
    removeDirection(EnumDb::SOUTH);
  else if(event->key() == Qt::Key_Up)
    removeDirection(EnumDb::NORTH);
  else if(event->key() == Qt::Key_Right)
    removeDirection(EnumDb::EAST);
  else if(event->key() == Qt::Key_Left)
    removeDirection(EnumDb::WEST);
}

/* 
 * Description: Reimplemented is move request call from map thing. This 
 *              utilizes the key press stack to get movement options.
 * 
 * Inputs: none
 * Output: bool - returns if a move is requested.
 */
bool MapPerson::isMoveRequested()
{
  return !movement_stack.isEmpty();
}

/* 
 * Description: Sets a state within the class, based on the double set of 
 *              enumerators, for surface and direction. This will automatically
 *              unset a state that is currently in its place, if one does
 *              exist. 
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         EnumDb::Direction direction - the direction for the state
 * Output: bool - if the call was successful
 */
bool MapPerson::setState(SurfaceClassifier surface, 
                         EnumDb::Direction direction, MapState* state)
{
  /* Only proceed with insertion if the sprite and state data is valid */
  if(state != 0 && state->getSprite() != 0)
  {
    unsetState(surface, direction);
    states[surface][dirToInt(direction)] = state;

    /* If the updated state is the active one, automatically set the printable
     * sprite */
    if(this->surface == surface && this->direction == direction)
      MapThing::setState(states[surface][dirToInt(direction)], false);

    return true;
  }

  return false;
}

/* 
 * Description: Sets a new surface for the person on the map to sit on. It 
 *              will update the parent frame so the new classifier is printed.
 * 
 * Inputs: SurfaceClassifier surface - the new surface to set
 * Output: none
 */
void MapPerson::setSurface(SurfaceClassifier surface)
{
  this->surface = surface;

  if(states[surface][dirToInt(direction)] != 0)
    MapThing::setState(states[surface][dirToInt(direction)], false);
}

/* Sets the target map person, fails if there is already a target */
void MapPerson::setTarget(MapThing* target)
{
  this->target = target;
}

/*
 * Description: Updates the state of the thing. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: none
 * Output: none 
 */
void MapPerson::updateThing(float cycle_time, Tile* next_tile)
{
  bool can_move = isMoveAllowed(next_tile);
  bool reset = false;
  
  /* Once a tile end has reached, cycle the movement direction */
  if(isAlmostOnTile(cycle_time))
  {
    tileMoveFinish();
    
    /* Only update direction if a move is requested */
    if(isMoveRequested())
    {
      /* Set the new direction and if the direction is changed, or it's
       * not allowed to move, recenter the thing */
      if(setDirection(getMoveRequest(), can_move) || !can_move)
      {
        x = tile_main->getPixelX();
        y = tile_main->getPixelY();
      }
      
      /* If it can move, initiate tile shifting */
      if(can_move)
        tileMoveStart(next_tile, Tile::PERSON);

      reset = !can_move;
    }
    /* If there is no move request, stop movement */
    else
    {
      if(tile_main != 0)
      {
        x = tile_main->getPixelX();
        y = tile_main->getPixelY();
      }

      setDirection(EnumDb::DIRECTIONLESS);
      reset = true;
    }
  }

  /* Proceed to move the thing */
  moveThing(cycle_time);

  /* Only animate if the direction exists */
  animate(cycle_time, reset, true);
}

/* 
 * Description: Unsets the state in this class based on the two classifiers.
 *              This includes the appropriate delete functionality for the 
 *              stored pointers. Also, unsets the parent classifier, if 
 *              the current state data is being used.
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         EnumDb::Direction direction - the direction for the state
 * Output: none
 */
void MapPerson::unsetState(SurfaceClassifier surface, 
                           EnumDb::Direction direction)
{
  delete states[surface][dirToInt(direction)];
  states[surface][dirToInt(direction)] = 0;

  /* Clear out the parent call if the direction or surface lines up */
  if(this->surface == surface && this->direction == direction)
    MapThing::unsetState(false);
}
