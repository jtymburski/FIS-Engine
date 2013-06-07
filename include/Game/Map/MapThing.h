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

#include <QDebug>

#include "Game/Map/Layer.h"
#include "Game/Map/MapState.h"

class MapThing : public Layer
{
public:
  /* Constructor functions */
  MapThing();
  MapThing(MapState* state, int width, int height, QString name = "", 
           QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  ~MapThing();

private:
  /* The animation delay, stored locally */
  int animation_delay;

  /* The things description */
  QString description;

  /* The id for the thing (Not currently used in the design, may need later) */
  int id;

  /* The things name */
  QString name;

  /* The main state, the main one */
  MapState* state;

  /* -------------------------- Constants ------------------------- */
protected:
  const static int kANIMATION_OFFSET; /* The number of animate calls before
                                         the frame sequence is updated */
  const static int kMINIMUM_ID;       /* The minimum ID, for a thing */
  const static int kUNSET_ID;         /* The placeholder unset ID */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Animates the thing. Skips the head of the list, if requested */
  bool animate(bool skip_head = false, bool just_started = false);

  /* Clears the entire class data */
  void clear();

  /* Gets the things decription */
  QString getDescription();

  /* Gets the things ID */
  int getID();

  /* Gets the things name */
  QString getName();

  /* Returns the map state that's defined */
  MapState* getState();
  
  /* Starts inteaction (conversation, giving something, etc) */
  void interaction();

  /* Resets the animation of the thing, back to the starting frame */ 
  bool resetAnimation();

  /* Set the coordinates for the data in the map thing (for the parent) */
  void setCoordinates(int x, int y, int z = 0);

  /* Sets the things description */
  void setDescription(QString new_description);

  /* Sets the things ID */
  bool setID(int new_id);

  /* Sets the things name */
  void setName(QString new_name);

  /* Sets the state of the thing */
  bool setState(MapState* state, bool unset_old = true);

  /* Unsets the state, in the class */
  void unsetState(bool delete_state = true);
};

#endif // MAPTHING_H
