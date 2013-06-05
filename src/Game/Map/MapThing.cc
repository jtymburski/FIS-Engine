/******************************************************************************
 * Class Name: MapThing
 * Date Created: Dec 2 2012
 * Inheritance: none
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapWalkOver, MapSolid, etc. Also handles the basic setup for 
 *              name, description, id, sprite. No interaction is handled in 
 *              this class since its a generic parent.
 *****************************************************************************/
#include "Game/Map/MapThing.h"

/* Constant Implementation - see header file for descriptions */
const int MapThing::kMINIMUM_ID =  0;
const int MapThing::kUNSET_ID   = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Sets up an empty thing with no
 *              data.
 *
 * Inputs: none
 */
MapThing::MapThing()
{
  state = 0;
  clear();
}

/* 
 * Description: Constructor for this class. Takes data to create the thing.
 *
 * Inputs: MapState* state - the state data to encapsalate by the thing
 *         QString name - the name of the thing, default to ""
 *         QString description - the description of the thing, default to ""
 *         int id - the ID of the thing, default to -1
 */
MapThing::MapThing(MapState* state, int width, int height, 
                   QString name, QString description, int id)
{
  this->state = 0;

  /* The parent class definitions */
  setHeight(height);
  setWidth(width);

  /* The class definitions */
  setDescription(description);
  setID(id);
  setName(name);
  setState(state);
}

/* 
 * Description: Destructor function 
 */
MapThing::~MapThing()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Clears out all information stored in the class
 * 
 * Inputs: none
 * Output: none
 */
void MapThing::clear()
{
  setDescription("");
  setID(kUNSET_ID);
  setName("");
  unsetState();
}

/* 
 * Description: Gets the things description.
 * 
 * Inputs: none
 * Output: QString - the description for the thing, as a QString
 */
QString MapThing::getDescription()
{
  return description;
}

/* 
 * Description: Gets the things ID. If the ID is not set, it will be an
 *              out of range number (-1)
 *
 * Inputs: none
 * Output: int - the ID for the thing, as an integer
 */
int MapThing::getID()
{
  return id;
}

/* 
 * Description: Gets the things name.
 *
 * Inputs: none
 * Output: QString - the name for the thing, as a QString
 */
QString MapThing::getName()
{
  return name;
}

/* 
 * Description: Gets the state data of the thing. If state isn't set, returns
 *              0.
 *
 * Inputs: none
 * Output: MapState* - the pointer to the data for the thing, as a MapState
 */
MapState* MapThing::getState()
{
  return state;
}

/* 
 * Description: Starts interaction. In map thing, this isn't of use and is
 *              only used by its children classes.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::interaction()
{
}

/* 
 * Description: Sets the coordinate information for the base item (inherited).
 *              X and Y are with respect to the scene plane. Z is the vertical
 *              plane for how items are layered.
 *
 * Inputs: int x - the x pixel coordinate on the scene
 *         int y - the y pixel coordinate on the scene
 *         int z - the z rating on the screen (default to 0).
 * Output: none
 */
void MapThing::setCoordinates(int x, int y, int z)
{
  setX(x);
  setY(y);
  setZValue(z);
}

/* 
 * Description: Sets the description that defines the thing.
 *
 * Inputs: QString new_description - the data for the new description
 * Output: none
 */
void MapThing::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the ID for the thing. If out of the allowable range, 
 *              the value is set to an minimum storage value. (Out of range)
 *
 * Inputs: int new_id - the new integer ID to define the thing
 * Output: bool - status if the new_id is within the allowable range.
 */
bool MapThing::setID(int new_id)
{
  /* If the ID is out of range */
  if(new_id < kMINIMUM_ID)
  {
    id = kUNSET_ID;
    return false;
  }

  /* Otherwise, the ID is good */
  id = new_id;
  return true;
}

/*
 * Description: Sets the name for the thing.
 *
 * Inputs: QString new_name - the new name for the thing
 * Output: none
 */
void MapThing::setName(QString new_name)
{
  name = new_name;
}

/*
 * Description: Sets the state data that defines the thing.
 *
 * Inputs: MapState* state - the new state to define to insert into the Map
 *           Thing. Must actually have a sprite set in order to work.
 *         bool unset_old - delete the old state from memory?
 * Output: bool - returns if the thing state was set successfuly
 */
bool MapThing::setState(MapState* state, bool unset_old)
{
  /* Check if the state is valid */
  if(state != 0 && state->getSprite() != 0)
  {
    if(unset_old)
      unsetState();

    this->state = state;
    setItem(state->getSprite(), unset_old);

    return true;
  }

  return false;
}

/*
 * Description: Unsets the state that's embedded in this as the Map Thing
 *
 * Inputs: bool delete_state - should the old state be deleted?
 * Output: none 
 */
void MapThing::unsetState(bool delete_state)
{
  unsetItem(false);
  if(delete_state)
    delete state;
  state = 0;
}
