/******************************************************************************
* Class Name: MapItem
* Date Created: Oct 28 2012
* Inheritance: MapInteractiveObject
* Description: The MapItem class, right now this is just for categorization
******************************************************************************/
#ifndef MAPITEM_H
#define MAPITEM_H

#include "Game/Map/MapInteractiveObject.h"

class MapItem : public MapInteractiveObject
{
public:
  /* Constructor function */
  MapItem();

  /* Destructor function */
  ~MapItem();
};

#endif // MAPITEM_H
