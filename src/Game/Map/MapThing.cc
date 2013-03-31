/******************************************************************************
* Class Name: MapThing
* Date Created: Dec 2 2012
* Inheritance: none
* Description: This class handles the generic MapThing. It contains things on
*              the map that don't fall under general scenary. It acts as the
*              parent class to a sequence of others, for example, MapPerson,
*              MapWalkOver, MapInteractiveObject, etc. Also handles the 
*              basic setup for name, description, id, sprite. No interaction
*              is handled in this class since its a generic parent.
******************************************************************************/
#include "Game/Map/MapThing.h"

/* 
 * Description: Constructor for this class. Takes data to create the thing.
 *
 * Inputs: Sprite* frames - the sprite data to encapsalate by the thing
 *         QString name - the name of the thing, default to ""
 *         QString description - the description of the thing, default to ""
 *         int id - the ID of the thing, default to -1
 */
MapThing::MapThing(Sprite* frames, QString name, QString description, int id)
{
  thing_set = false;

  setSprite(frames);
  setName(name);
  setDescription(description);
  setID(id);
}

/* 
 * Description: Constructor for this class. Sets up an empty thing with no
 *              data.
 *
 * Inputs: none
 */
MapThing::MapThing()
{
  thing_set = false;

  setSprite(NULL);
  setName("");
  setDescription("");
  setID(kUNSET_ID); 
}

/* 
 * Description: Destructor function 
 */
MapThing::~MapThing()
{
  if(thing != NULL)
    delete thing;
  thing = 0;
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
 * Description: Gets the sprite data of the thing. If sprite isn't set, returns
 *              NULL.
 *
 * Inputs: none
 * Output: Sprite* - the pointer to the data for the thing, as a Sprite
 */
Sprite* MapThing::getSprite()
{
  return thing;
}

/*
 * Description: Returns if the thing is set.
 *
 * Inputs: none
 * Output: bool - returns if the thing is set
 */
bool MapThing::isSet()
{
  return thing_set;
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
 * Description: Sets the sprite frame data that defines the thing.
 *
 * Inputs: Sprite* new_frames - the new frame data to use for representing
 *                              the thing.
 * Output: bool - returns if the thing was set successfuly
 */
bool MapThing::setSprite(Sprite* new_frames)
{
  unsetSprite();

  /* Check if the frames are valid */
  if(new_frames != NULL && new_frames->getSize() > 0)
  {
    thing = new_frames;
    thing_set = true;
  }

  return thing_set;
}

/*
 * Description: Unsets the sprite that's embedded in this as the Map Thing
 *
 * Inputs: none
 * Output: bool - returns true if the Map Thing was set before this was called 
 */
bool MapThing::unsetSprite()
{
  bool was_thing_set = thing_set;

  if(thing_set)
    delete thing;

  this->thing = NULL;
  thing_set = false;

  return was_thing_set;
}
