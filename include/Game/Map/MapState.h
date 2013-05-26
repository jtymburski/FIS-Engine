/******************************************************************************
* Class Name: MapState
* Date Created: May 23, 2013
* Inheritance: QObject
* Description: This class becomes the middle interface for a sprite and map
*              actions to be tied together.
******************************************************************************/
#ifndef MAPSTATE_H
#define MAPSTATE_H

#include <QObject>

#include "Game/Sprite.h"

class MapState : public QObject
{
public:
  /* Constructor functions */
  MapState();
  MapState(Sprite* animation);

  /* Destructor function */
  ~MapState();

private:
  /* The information that defines the animation image data for the sprite */
  Sprite* animation;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clear out the state definition */
  void clear();

  /* Returns the sprite stored in the state for control/usage */
  Sprite* getSprite();

  /* Sets the sprite internally to the state */
  bool setSprite(Sprite* animation);

  /* Unsets the sprite internally to the state */
  void unsetSprite();
};

#endif // MAPSSTATE_H
