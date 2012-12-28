/******************************************************************************
* Class Name: MapThing
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The MapThing class
******************************************************************************/
#include "Game/Map/MapThing.h"

/* Constructor function */
MapThing::MapThing()
{
}

/* Destructor function */
MapThing::~MapThing()
{
}

/* Starts inteaction (conversation, giving something etc, reimplemented in
  subclasses polymorphically, called from Map class) */
void MapThing::interaction()
{
}

/* Gets the things decription */
QString MapThing::getDescription()
{
    return description;
}

/* Gets the things name */
QString MapThing::getName()
{
    return name;
}

/* Gets the directional sprite of the thing */
Sprite MapThing::getSprite(Direction dir)
{
    //Sprite* sprite1 = new Sprite(0);

    //return *sprite1;
}
