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

class MapPerson;

#include <cmath>
#include <QDebug>
#include <QObject>

#include "EnumDb.h"
#include "Game/Map/MapState.h"
#include "Game/Map/Tile.h"

class MapThing : public QObject
{
  Q_OBJECT

public:
  /* Constructor functions */
  MapThing();
  MapThing(MapState* state, int width, int height, QString name = "", 
           QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  ~MapThing();

protected:
  /* The thing classification */
  QString description;
  short height;
  int id;
  QString name;
  Tile* tile_main;
  Tile* tile_previous;
  short width;
  float x;
  float y;

  /* The main state */
  MapState* state;

  /* Conversation information */
  Conversation conversation_info;

  /* Movement information */
  short animation_buffer;
  short animation_time;
  EnumDb::Direction movement;
  short speed;
  
  /* Painting information */
  Frame dialog_image;

  /* -------------------------- Constants ------------------------- */
  const static short kDEFAULT_ANIMATION; /* The default animation speed */
  const static short kDEFAULT_SPEED;     /* The default thing speed */
  const static short kMINIMUM_ID;        /* The minimum ID, for a thing */
  const static short kUNSET_ID;          /* The placeholder unset ID */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Animates the thing, if it has multiple frames */
  bool animate(short cycle_time, bool reset = false, bool skip_head = false);
 
  /* Is the thing almost centered on a tile (less than 1 pulse away) */
  bool isAlmostOnTile(float cycle_time);
  
  /* Is move allowed, based on main tile and the next tile */
  bool isMoveAllowed(Tile* next_tile);
  
  /* Move the thing, based on the internal direction */
  float moveAmount(float cycle_time);
  void moveThing(float cycle_time);

  /* Sets the new direction that the class is moving in */
  bool setDirection(EnumDb::Direction new_direction);

  /* Starts and stops tile move. Relies on underlying logic for occurance */
  void tileMoveFinish();
  bool tileMoveStart(Tile* next_tile, Tile::ThingState classification);
 
/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void startConversation(Conversation conversation_data);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears the entire class data */
  virtual void clear();

  /* Gets the animation speed of the thing */
  short getAnimationSpeed();
  
  /* Gets the conversation data for the thing */
  Conversation getConversation();

  /* Gets the things decription */
  QString getDescription();

  /* Returns the dialog image data, so that it may be painted */
  Frame* getDialogImage();

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
  
  /* Returns the central tile */
  Tile* getTile();
  
  /* Returns the width of the thing */
  int getWidth();

  /* Returns the location of the thing */
  float getX();
  float getY();

  /* Initiates a conversation to occur with the map */
  void initiateConversation(EnumDb::Direction person_dir);

  /* Starts interaction (conversation, giving something, etc) */
  virtual bool interaction(MapPerson* person);
  
  /* Returns if there is a move request for the given thing */
  virtual bool isMoveRequested();

  /* Returns if the thing is moving */
  bool isMoving();

  /* Is the thing centered on a tile */
  bool isOnTile();

  /* Paint call, that paints the main state of the thing */
  bool paintGl(float offset_x = 0.0, float offset_y = 0.0, float opacity = 1.0);

  /* Sets the animation time for each frame */
  bool setAnimationSpeed(short frame_time);
 
  /* Sets the conversation data for the thing */
  void setConversation(Conversation conversation_info);

  /* Sets the things description */
  void setDescription(QString new_description);

  /* Sets the dialog image data, for display during conversation */
  bool setDialogImage(QString path);

  /* Sets the things height classification */
  bool setHeight(int new_height);

  /* Sets the things ID */
  bool setID(int new_id);

  /* Sets the things name */
  void setName(QString new_name);

  /* Sets the things speed */
  bool setSpeed(short speed);
    
  /* Set the tile to hook the map thing to */
  void setStartingTile(Tile* new_tile);
  
  /* Sets the state of the thing */
  bool setState(MapState* state, bool unset_old = true);

  /* Sets the things width classification */
  bool setWidth(int new_width);

  /* Updates the thing, called on the tick */
  virtual void updateThing(float cycle_time, Tile* next_tile);
  
  /* Unsets the state, in the class */
  void unsetState(bool delete_state = true);
};

#endif // MAPTHING_H
