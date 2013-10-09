/******************************************************************************
* Class Name: MapState
* Date Created: May 23, 2013
* Inheritance: QObject
* Description: This class becomes the middle interface for a sprite and map
*              actions to be tied together.
******************************************************************************/
#include "Game/Map/MapState.h"

/* Constant Implementation - see header file for descriptions */
const short MapState::kMAX_OPACITY = 100;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

MapState::MapState()
{
  animation = 0;
  opacity = kMAX_OPACITY;
  passable = false;
}

MapState::MapState(Sprite* animation, int opacity, bool passable)
{
  this->animation = 0;
  setSprite(animation);
  setOpacity(opacity);
  setPassable(passable);
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

/* Returns the painted opacity of the state */
int MapState::getOpacity()
{
  return opacity;
}
  
/* Returns the sprite stored in the state for control/usage */
Sprite* MapState::getSprite()
{
  return animation;
}

/* Returns if the state is passable */
bool MapState::isPassable()
{
  return passable;
}

/* Sets the opacity of the painted state (0 - 100) */
void MapState::setOpacity(int opacity)
{
  if(opacity < 0)
    this->opacity = 0;
  else if(opacity > kMAX_OPACITY)
    this->opacity = kMAX_OPACITY;
  else
    this->opacity = opacity;
}
  
/* Sets the passability and if this state restricts walking */
void MapState::setPassable(bool passable)
{
  this->passable = passable;
}

bool MapState::setSprite(Sprite* animation)
{
  if(animation != 0 && animation->getSize() > 0)
  {
    unsetSprite();
    this->animation = animation;
    //this->animation->initializeGl(); // TODO?
    return true;
  }
  return false;
}

void MapState::unsetSprite()
{
  delete animation;
  animation = 0;
}
