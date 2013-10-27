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

#include <cmath>
#include <QDebug>
//#include <QObject>

#include "EnumDb.h"
#include "Game/EventHandler.h"
#include "Game/Map/Tile.h"
#include "Game/Sprite.h"

class MapThing// : public QObject
{
//  Q_OBJECT

public:
  /* Constructor functions */
  MapThing();
  MapThing(Sprite* frames, int width, int height, QString name = "", 
           QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  virtual ~MapThing();

protected:
  /* The thing classification */
  QString description;
  short height;
  int id;
  QString name;
  float opacity;
  bool passable;
  Tile* tile_main;
  Tile* tile_previous;
  short tile_section;
  short width;
  float x;
  float y;

  /* The target for this thing. If set, it cannot be targetted by others */
  MapThing* target;
  
  /* The main sprite frame data */
  Sprite* frames;

  /* Movement information */
  short animation_buffer;
  short animation_time;
  EnumDb::Direction movement;
  bool movement_paused;
  short speed;
  
  /* The event handler information and corresponding interact event */
  EventHandler* event_handler;
  EventHandler::Event interact_event;
  
  /* Painting information */
  Frame dialog_image;

  /* -------------------------- Constants ------------------------- */
  const static short kDEFAULT_ANIMATION; /* The default animation speed */
  const static short kDEFAULT_SPEED; /* The default thing speed */
  const static float kMAX_OPACITY; /* The max opacity allowable (0-1.0) */
  const static short kMINIMUM_ID; /* The minimum ID, for a thing */
  const static short kPLAYER_ID; /* The player ID */
  const static short kUNSET_ID; /* The placeholder unset ID */

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
  virtual void tileMoveFinish();
  virtual bool tileMoveStart(Tile* next_tile);
 
/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  //void startConversation(Conversation conversation_data);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the class descriptor, useful for casting */
  virtual QString classDescriptor();

  /* Clears the entire class data */
  virtual void clear();
   
  /* Clear all movement from the stack. This is filled from add/remove
   * direction */
  virtual void clearAllMovement();

  /* Clears the target that the map thing is currently pointing at */
  void clearTarget();
  
  /* Gets the animation speed of the thing */
  virtual short getAnimationSpeed();
  
  /* Gets the things decription */
  QString getDescription();

  /* Returns the dialog image data, so that it may be painted */
  Frame* getDialogImage();

  /* Returns the map frames that's defined */
  Sprite* getFrames();
  
  /* Returns the height of the thing */
  int getHeight();

  /* Gets the things ID */
  int getID();
  
  /* Gets the interaction event for the thing */
  EventHandler::Event getInteraction();

  /* Returns the map section of the tile where this is painted */
  int getMapSection();
  
  /* Get the specific details of the movement information */
  EnumDb::Direction getMovement();
  bool getMovementPaused();
  virtual EnumDb::Direction getMoveRequest();

  /* Gets the things name */
  QString getName();

  /* Returns the opacity of the painted thing. */
  virtual float getOpacity();
  
  /* Returns the speed that the thing is moving at */
  short getSpeed();
  
  /* Returns the target that this thing is pointed at */
  MapThing* getTarget();
  
  /* Returns the central tile */
  Tile* getTile();
  
  /* Returns the width of the thing */
  int getWidth();

  /* Returns the location of the thing */
  float getX();
  float getY();

  /* Initializes the GL context with the frames / states stored internally */
  virtual bool initializeGl();
  
  /* Starts interaction (conversation, giving something, etc) */
  virtual bool interact(MapPerson* initiator);
  
  /* Returns if there is a move request for the given thing */
  virtual bool isMoveRequested();

  /* Returns if the thing is moving */
  bool isMoving();

  /* Is the thing centered on a tile */
  bool isOnTile();

  /* Is the thing passable - can you walk in it? */
  virtual bool isPassable();
  
  /* Paint call, that paints the main state of the thing */
  bool paintGl(float offset_x = 0.0, float offset_y = 0.0);

  /* Sets the animation time for each frame */
  virtual bool setAnimationSpeed(short frame_time);
 
  /* Sets the things description */
  void setDescription(QString new_description);

  /* Sets the dialog image data, for display during conversation */
  bool setDialogImage(QString path);
    
  /* Sets the event handler */
  void setEventHandler(EventHandler* event_handler);
  
  /* Sets the state of the thing */
  bool setFrames(Sprite* frames, bool unset_old = true);
  
  /* Sets the things height classification */
  bool setHeight(int new_height);

  /* Sets the things ID */
  bool setID(int new_id);
  void setIDPlayer();

  /* Sets the interaction event for the thing */
  bool setInteraction(EventHandler::Event interact_event);
  
  /* Sets if the movement is paused */
  void setMovementPaused(bool paused);
  
  /* Sets the things name */
  void setName(QString new_name);

  /* Sets the opacity of the painted state (0 - 1.0) */
  void setOpacity(float opacity);

  /* Sets the things speed */
  bool setSpeed(short speed);
    
  /* Set the tile to hook the map thing to */
  virtual bool setStartingTile(int section_id, Tile* new_tile, 
                                               bool no_events = false);

  /* Sets the target map thing, fails if there is already a target */
  bool setTarget(MapThing* target);
  
  /* Sets the things width classification */
  bool setWidth(int new_width);

  /* Updates the thing, called on the tick */
  virtual void updateThing(float cycle_time, Tile* next_tile);
  
  /* Unsets the thing frames, in the class */
  void unsetFrames(bool delete_frames = true);
  
  /* Unsets the starting tile */
  virtual void unsetStartingTile(bool no_events = false);
};

#endif // MAPTHING_H
