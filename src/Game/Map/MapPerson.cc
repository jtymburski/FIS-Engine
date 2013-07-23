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
MapPerson::MapPerson(int width, int height, QObject* parent,
                     QString name, QString description, int id) : MapThing(parent)
{
  /* Class value setup */
  setDescription(description);
  setHeight(height);
  setID(id);
  setName(name);
  setWidth(width);

  /* Initializes the class information */
  initializeStates();

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
 * Description: Sets a new direction for the person on the map. It will update
 *              the parent frame so a new classifier is printed.
 * 
 * Inputs: MovementDirection direction - the new direction to set
 * Output: none
 */
void MapPerson::setDirection(EnumDb::Direction direction, bool set_movement)
{
  bool changed = (this->direction != direction);
 
  /* If moving, set the direction in map thing */
  if(set_movement)
    MapThing::setDirection(direction);
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

/*
 * Description: The key press event implementation. Handles all the movement
 *              for the map person, if redirected here.
 *
 * Inputs: QKeyEvent* event - the key event to analyze
 * Output: none
 */
void MapPerson::keyPress(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down && 
     !movement_stack.contains(EnumDb::SOUTH))
    movement_stack.append(EnumDb::SOUTH);
  else if(event->key() == Qt::Key_Up && 
          !movement_stack.contains(EnumDb::NORTH))
    movement_stack.append(EnumDb::NORTH);
  else if(event->key() == Qt::Key_Right && 
          !movement_stack.contains(EnumDb::EAST))
    movement_stack.append(EnumDb::EAST);
  else if(event->key() == Qt::Key_Left && 
          !movement_stack.contains(EnumDb::WEST))
    movement_stack.append(EnumDb::WEST);
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
  if(event->key() == Qt::Key_Down  && 
     movement_stack.contains(EnumDb::SOUTH))
    movement_stack.removeAt(movement_stack.indexOf(EnumDb::SOUTH));
  else if(event->key() == Qt::Key_Up && 
          movement_stack.contains(EnumDb::NORTH))
    movement_stack.removeAt(movement_stack.indexOf(EnumDb::NORTH));
  else if(event->key() == Qt::Key_Left && 
          movement_stack.contains(EnumDb::WEST))
    movement_stack.removeAt(movement_stack.indexOf(EnumDb::WEST));
  else if(event->key() == Qt::Key_Right && 
          movement_stack.contains(EnumDb::EAST))
    movement_stack.removeAt(movement_stack.indexOf(EnumDb::EAST));
}

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

/*
 * Description: Updates the state of the thing. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: none
 * Output: none 
 */
void MapPerson::updateThing(float cycle_time, bool can_move)
{
  /* Once a tile end has reached, cycle the movement direction */
  if(isOnTile())
  {
    /* Only update direction if a move is requested */
    if(isMoveRequested())
    {
      setDirection(getMoveRequest(), can_move);

      /* If not allowed to move, reset the animation */
      if(!can_move)
        resetAnimation();
    }
    else
    {
      setDirection(EnumDb::DIRECTIONLESS);
      resetAnimation();
    }
  }

  moveThing(cycle_time);

  /* Only animate if the direction exists */
  if(getMovement() != EnumDb::DIRECTIONLESS)
    animate(true);
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
