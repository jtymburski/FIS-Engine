/******************************************************************************
* Class Name: MapState
* Date Created: May 23, 2013
* Inheritance: QObject
* Description: This class becomes the middle interface for a sprite and map
*              actions to be tied together.
******************************************************************************/
#include "Game/Map/MapState.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapState::MapState()
{
  animation = 0;
}

MapState::MapState(Sprite* animation)
{
  this->animation = 0;
  setSprite(animation);
}

MapState::~MapState()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Clear out the state definition */
void MapState::clear()
{
  unsetSprite();
}

/* Returns the sprite stored in the state for control/usage */
Sprite* MapState::getSprite()
{
  return animation;
}

bool MapState::setSprite(Sprite* animation)
{
  if(animation != 0 && animation->getSize() > 0)
  {
    unsetSprite();
    this->animation = animation;
    return true;
  }
  return false;
}

void MapState::unsetSprite()
{
  delete animation;
  animation = 0;
}
