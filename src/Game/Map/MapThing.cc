/******************************************************************************
* Class Name: MapThing
* Date Created: Dec 2 2012
* Inheritance: none
* Description: The MapThing class
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
  this->thing = frames;
  this->name = name;
  this->description = description;
  this->id = id;
}

/* 
 * Description: Constructor for this class. Sets up an empty thing with no
 *              data.
 *
 * Inputs: none
 */
MapThing::MapThing()
{
  this->thing = NULL;
  this->name = "";
  this->description = "";
  this->id = -1;
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
 * Description: Starts interaction (conversation, giving something, etc) 
 *              TODO?
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
 * Description: Gets the things ID.
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
 * Description: Gets the sprite data of the thing.
 *
 * Inputs: none
 * Output: Sprite* - the pointer to the data for the thing, as a Sprite
 */
Sprite* MapThing::getSprite()
{
  return thing;
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
 * Description: Sets the ID for the thing.
 *
 * Inputs: int new_id - the new integer ID to define the thing
 * Output: none
 */
void MapThing::setID(int new_id)
{
  id = new_id;
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
 * Output: bool - returns if the thing was set before
 */
bool MapThing::setSprite(Sprite* new_frames)
{
  bool sprite_set = FALSE;
  if(thing != NULL)
    sprite_set = TRUE;

  thing = new_frames;
  return sprite_set;
}
