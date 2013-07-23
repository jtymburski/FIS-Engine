/******************************************************************************
 * Class Name: MapThing
 * Date Created: October 28, 2012
 * Inheritance: QObject
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapWalkOver, MapSolid, etc. Also handles the basic setup for 
 *              name, description, id, sprite. No interaction is handled in 
 *              this class since its a generic parent.
 *****************************************************************************/
#ifndef MAPTHING_H
#define MAPTHING_H

//#include <QDebug>
//#include <QGraphicsObject>
#include <QObject>
//#include <QPainter>

#include "Game/Map/MapState.h"

class MapThing : public QObject
{
public:
  /* Constructor functions */
  MapThing(QObject* parent = 0);
  MapThing(MapState* state, int width, int height, QString name = "", 
           QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  ~MapThing();

private:
  /* The animation delay, stored locally */
  int animation_delay;

  /* The thing classification */
  QString description;
  short height;
  int id;
  QString name;
  short width;
  float x;
  float y;

  /* The main state */
  MapState* state;

  /* Movement information */
  EnumDb::Direction movement;
  short speed;
  
  /* -------------------------- Constants ------------------------- */
protected:
  const static short kANIMATION_OFFSET; /* The number of animate calls before
                                           the frame sequence is updated */
  const static short kDEFAULT_SPEED;    /* The default thing speed (1 pixel every 10ms) */
  const static short kMINIMUM_ID;       /* The minimum ID, for a thing */
  const static short kUNSET_ID;         /* The placeholder unset ID */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Animates the thing, if it has multiple frames */
  bool animate(bool skip_head = false);

  /* Move the thing, based on the internal direction */
  float moveAmount(float cycle_time);
  void moveThing(float cycle_time);

  /* Sets the new direction that the class is moving in */
  bool setDirection(EnumDb::Direction new_direction);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears the entire class data */
  void clear();

  /* Gets the things decription */
  QString getDescription();

  /* Returns the height of the thing */
  int getHeight();

  /* Gets the things ID */
  int getID();

  /* Get the specific details of the movement information */
  EnumDb::Direction getMovement();
  virtual EnumDb::Direction getMoveRequest();

  /* Gets the things name */
  QString getName();

  /* Returns the speed that the thing is moving at */
  short getSpeed();
  
  /* Returns the map state that's defined */
  MapState* getState();
  
  /* Returns the width of the thing */
  int getWidth();

  /* Returns the location of the thing */
  float getX();
  float getY();

  /* Starts inteaction (conversation, giving something, etc) */
  virtual void interaction();
 
  /* Is the thing almost centered on a tile (less than 1 pulse away) */
  bool isAlmostOnTile(short cycle_time);
  
  /* Returns if there is a move request for the given thing */
  virtual bool isMoveRequested();

  /* Returns if the thing is moving */
  bool isMoving();

  /* Is the thing centered on a tile */
  bool isOnTile();

  /* Paint call, that paints the main state of the thing */
  bool paintGl(float offset_x = 0.0, float offset_y = 0.0, float opacity = 1.0);
  
  /* Resets the animation of the thing, back to the starting frame */ 
  bool resetAnimation();

  /* Set the coordinates for the data in the map thing (for the parent) */
  void setCoordinates(int x, int y);

  /* Sets the things description */
  void setDescription(QString new_description);

  /* Sets the things height classification */
  bool setHeight(int new_height);

  /* Sets the things ID */
  bool setID(int new_id);

  /* Sets the things name */
  void setName(QString new_name);

  /* Sets the things speed */
  bool setSpeed(short speed);
  
  /* Sets the state of the thing */
  bool setState(MapState* state, bool unset_old = true);

  /* Sets the things width classification */
  bool setWidth(int new_width);

  /* Updates the thing, called on the tick */
  virtual void updateThing(float cycle_time, bool can_move = true);
  
  /* Unsets the state, in the class */
  void unsetState(bool delete_state = true);
};

#endif // MAPTHING_H
