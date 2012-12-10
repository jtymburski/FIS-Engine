/******************************************************************************
* Class Name: MapEquipment
* Date Created: Oct 28 2012
* Inheritance: MapInteractiveObject
* Description: The MapEquipment class, right now this is just for 
*              categorization.
******************************************************************************/
#ifndef MAPEQUIPMENT_H
#define MAPEQUIPMENT_H

#include "Game/Map/MapInteractiveObject.h"

class MapEquipment : public MapInteractiveObject
{
public:
  /* Constructor function */
  MapEquipment();

  /* Destructor function */
  ~MapEquipment();
};

#endif // MAPEQUIPMENT_H
