/******************************************************************************
 * Class Name: MapThing
 * Date Created: October 28, 2012
 * Inheritance: Layer
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapWalkOver, MapSolid, etc. Also handles the basic setup for 
 *              name, description, id, sprite. No interaction is handled in 
 *              this class since its a generic parent.
 *****************************************************************************/
#ifndef MAPTHING_H
#define MAPTHING_H

#include "Game/Map/Layer.h"

class MapThing
{
public:
  /* Constructor functions */
  MapThing();
  MapThing(Sprite* frames, QString name = "", 
           QString description = "", int id = kUNSET_ID);

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
  bool thing_set;

  /* -------------------------- Constants ------------------------- */
  const static int kMINIMUM_ID;
  const static int kUNSET_ID;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the things decription */
  QString getDescription();

  /* Gets the things ID */
  int getID();

  /* Gets the things name */
  QString getName();

  /* Gets the sprite of the thing */
  Sprite* getSprite();
  
  /* Starts inteaction (conversation, giving something, etc) */
  void interaction();

  /* Returns if the thing is set */
  bool isSet();

  /* Sets the things description */
  void setDescription(QString new_description);

  /* Sets the things ID */
  bool setID(int new_id);

  /* Sets the things name */
  void setName(QString new_name);

  /* Sets the sprite of the thing */
  bool setSprite(Sprite* new_frames);

  /* Unsets the sprite of the thing */
  bool unsetSprite();
};

#endif // MAPTHING_H
