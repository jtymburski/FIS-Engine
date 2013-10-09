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
  MapState(Sprite* animation, int opacity = kMAX_OPACITY, 
                              bool passable = false);

  /* Destructor function */
  ~MapState();

private:
  /* The information that defines the animation image data for the sprite */
  Sprite* animation;
  short opacity;
  bool passable;
  
  /* -------------------------- Constants ------------------------- */
  const static short kMAX_OPACITY; /* The max opacity allowable (0-100) */
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clear out the state definition */
  void clear();

  /* Returns the painted opacity of the state */
  int getOpacity();
  
  /* Returns the sprite stored in the state for control/usage */
  Sprite* getSprite();

  /* Returns if the state is passable */
  bool isPassable();
  
  /* Sets the opacity of the painted state (0 - 100) */
  void setOpacity(int opacity);
  
  /* Sets the passability and if this state restricts walking */
  void setPassable(bool passable);
  
  /* Sets the sprite internally to the state */
  bool setSprite(Sprite* animation);

  /* Unsets the sprite internally to the state */
  void unsetSprite();
};

#endif // MAPSSTATE_H
