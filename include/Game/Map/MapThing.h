/******************************************************************************
* Class Name: MapThing
* Date Created: Oct 28 2012
* Inheritance: none
* Description: The MapThing class
******************************************************************************/
#ifndef MAPTHING_H
#define MAPTHING_H

#include "Game/Sprite.h"

class MapThing
{
public:
  /* Constructor functions */
  MapThing(Sprite* frames, QString name = "", 
           QString description = "", int id = -1);
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

  /* The sprite object for the thing */
  Sprite* thing;

public:
  /* Starts inteaction (conversation, giving something, etc) */
  void interaction();

  /* Gets the things decription */
  QString getDescription();

  /* Gets the things ID */
  int getID();

  /* Gets the things name */
  QString getName();

  /* Gets the sprite of the thing */
  Sprite* getSprite();

  /* Sets the things description */
  void setDescription(QString new_description);

  /* Sets the things ID */
  void setID(int new_id);

  /* Sets the things name */
  void setName(QString new_name);

  /* Sets the sprite of the thing */
  bool setSprite(Sprite* new_frames);
};

#endif // MAPTHING_H
