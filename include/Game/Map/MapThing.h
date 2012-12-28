/******************************************************************************
* Class Name: MapThing
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The MapThing class
******************************************************************************/
#ifndef MAPTHING_H
#define MAPTHING_H

#include "EnumDatabase.h"
#include "Game/Sprite.h"

class MapThing
{
public:
  /* Constructor function */
  MapThing();

  /* Destructor function */
  ~MapThing();

private:
  /* The things description */
  QString description;

  /* The id for the thing (Not currently used in the design, may need later) */
  int id;

  /* The things name */
  QString name;

  /* The facing sprites for the thing */
  Sprite* north,east,south,west;

public:
  /* Starts inteaction (conversation, giving something etc, reimplemented in
    subclasses polymorphically, called from Map class) */
  void interaction();

  /* Gets the things decription */
  QString getDescription();

  /* Gets the things name */
  QString getName();

  /* Gets the directional sprite of the thing */
  Sprite getSprite(Direction dir);
};

#endif // MAPTHING_H
