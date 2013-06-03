/******************************************************************************
* Class Name: MapPerson
* Date Created: Dec 2 2012
* Inheritance: MapThing
* Description: The MapPerson class, this covers all Persons in the map
******************************************************************************/
#include "Game/Map/MapPerson.h"

/* Constant Implementation - see header file for descriptions */
const int MapPerson::kTOTAL_DIRECTIONS = 4;
const int MapPerson::kTOTAL_SURFACES   = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapPerson::MapPerson()
{
  initializeStates();
}

/* Another constructor function */
MapPerson::MapPerson(int width, int height, 
                     QString name, QString description, int id)
{
  MapThing(0, width, height, name, description, id);
  initializeStates();
}

/* Destructor function */
MapPerson::~MapPerson()
{
}

void MapPerson::initializeStates()
{
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

MapPerson::MovementDirection MapPerson::getDirection()
{
  return direction;
}

MapPerson::SurfaceClassifier MapPerson::getSurface()
{
  return surface;
}

void MapPerson::setDirection(MovementDirection direction)
{
  this->direction = direction;
  MapThing::setState(states[surface][direction], false);
}

bool MapPerson::setState(SurfaceClassifier surface, MovementDirection direction, MapState* state)
{
  /* Only proceed with insertion if the sprite and state data is valid */
  if(state != 0 && state->getSprite() != 0)
  {
    unsetState(surface, direction);
    states[surface][direction] = state;

    /* If the updated state is the active one, automatically set the printable
     * sprite */
    if(this->surface == surface || this->direction == direction)
      MapThing::setState(states[surface][direction], false);

    return true;
  }

  return false;
}

void MapPerson::setSurface(SurfaceClassifier surface)
{
  this->surface = surface;
  MapThing::setState(states[surface][direction], false);
}

void MapPerson::unsetState(SurfaceClassifier surface, 
                           MovementDirection direction)
{
  delete states[surface][direction];
  states[surface][direction] = 0;

  /* Clear out the parent call if the direction or surface lines up */
  if(this->surface == surface || this->direction == direction)
    MapThing::unsetState(false);
}
