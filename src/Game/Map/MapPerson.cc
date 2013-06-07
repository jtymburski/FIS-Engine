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
const int MapPerson::kTOTAL_DIRECTIONS = 4;
const int MapPerson::kTOTAL_SURFACES   = 1;

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
  direction = NORTH;
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
MapPerson::MapPerson(int width, int height, 
                     QString name, QString description, int id)
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
  direction = NORTH;
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
    {
      row.append(0);
    }

    states.append(row);
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
    for(int j = 0; j < kTOTAL_DIRECTIONS; j++)
      unsetState( (SurfaceClassifier)i, (MovementDirection)j );
}

/* 
 * Description: Returns the direction that the MapPerson is currently set to. 
 * 
 * Inputs: none
 * Output: MovementDirection - the direction enumerator for this class
 */
MapPerson::MovementDirection MapPerson::getDirection()
{
  return direction;
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
 * Description: Sets a new direction for the person on the map. It will update
 *              the parent frame so a new classifier is printed.
 * 
 * Inputs: MovementDirection direction - the new direction to set
 * Output: none
 */
void MapPerson::setDirection(MovementDirection direction)
{
  this->direction = direction;

  if(states[surface][direction] != 0)
    MapThing::setState(states[surface][direction], false);
}

/* 
 * Description: Sets a state within the class, based on the double set of 
 *              enumerators, for surface and direction. This will automatically
 *              unset a state that is currently in its place, if one does
 *              exist. 
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         MovementDirection direction - the direction for the state
 * Output: bool - if the call was successful
 */
bool MapPerson::setState(SurfaceClassifier surface, MovementDirection direction, MapState* state)
{
  /* Only proceed with insertion if the sprite and state data is valid */
  if(state != 0 && state->getSprite() != 0)
  {
    unsetState(surface, direction);
    states[surface][direction] = state;

    /* If the updated state is the active one, automatically set the printable
     * sprite */
    if(this->surface == surface && this->direction == direction)
      MapThing::setState(states[surface][direction], false);

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

  if(states[surface][direction] != 0)
    MapThing::setState(states[surface][direction], false);
}

/* 
 * Description: Unsets the state in this class based on the two classifiers.
 *              This includes the appropriate delete functionality for the 
 *              stored pointers. Also, unsets the parent classifier, if 
 *              the current state data is being used.
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         MovementDirection direction - the direction for the state
 * Output: none
 */
void MapPerson::unsetState(SurfaceClassifier surface, 
                           MovementDirection direction)
{
  delete states[surface][direction];
  states[surface][direction] = 0;

  /* Clear out the parent call if the direction or surface lines up */
  if(this->surface == surface && this->direction == direction)
    MapThing::unsetState(false);
}
