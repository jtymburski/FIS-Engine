/******************************************************************************
* Class Name: MapInteractiveObject
* Date Created: Oct 28 2012
* Inheritance: MapThing
* Description: This is the object is a specialization of the stock map thing
*              that allows interaction from key presses by players in 
*              nearby slots. Pressing the key will toggle the object and allow
*              it to change states. These objects are unmovable and are of the
*              typical interactive objects such as doors, chests, etc.
******************************************************************************/
#ifndef MAPINTERACTIVEOBJECT_H
#define MAPINTERACTIVEOBJECT_H

#include "Game/Map/MapThing.h"

class MapInteractiveObject : public MapThing
{
public:
  /* Constructor function */
  MapInteractiveObject();

  /* Destructor function */
  ~MapInteractiveObject();
};

#endif // MAPINTERACTIVEOBJECT_H
