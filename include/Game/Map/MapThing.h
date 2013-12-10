/******************************************************************************
 * Class Name: MapThing
 * Date Created: October 28, 2012
 * Inheritance: none
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

#include "EnumDb.h"
#include "Game/EventHandler.h"
#include "Game/Map/Tile.h"
#include "Helpers.h"
#include "Sprite.h"
#include "XmlData.h"

class MapThing
{
public:
  /* Constructor functions */
  MapThing();
  MapThing(Sprite* frames, uint16_t width, uint16_t height, 
           std::string name = "", std::string description = "", 
           int id = kUNSET_ID);

  /* Destructor function */
  virtual ~MapThing();

protected:
  /* The thing classification */
  std::string description;
  uint16_t height;
  int id;
  std::string name;
  bool passable;
  Tile* tile_main;
  Tile* tile_previous;
  uint16_t tile_section;
  uint16_t width;
  float x;
  float y;

  /* The target for this thing. If set, it cannot be targetted by others */
  MapThing* target;
  
  /* The main sprite frame data */
  Sprite* frames;

  /* Movement information */
  Direction movement;
  bool movement_paused;
  uint16_t speed;
  
  /* The event handler information and corresponding interact event */
  EventHandler* event_handler;
  Event interact_event;
  
  /* Painting information */
  Frame dialog_image;

  /* -------------------------- Constants ------------------------- */
  const static uint16_t kDEFAULT_SPEED; /* The default thing speed */
  const static int kPLAYER_ID; /* The player ID */
  const static int kUNSET_ID; /* The placeholder unset ID */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Animates the thing, if it has multiple frames */
  bool animate(int cycle_time, bool reset = false, bool skip_head = false);
 
  /* Is the thing almost centered on a tile (less than 1 pulse away) */
  bool isAlmostOnTile(int cycle_time);
  
  /* Is move allowed, based on main tile and the next tile */
  bool isMoveAllowed(Tile* next_tile);
  
  /* Move the thing, based on the internal direction */
  float moveAmount(int cycle_time);
  void moveThing(int cycle_time);

  /* Sets the new direction that the class is moving in */
  bool setDirection(Direction new_direction);

  /* Starts and stops tile move. Relies on underlying logic for occurance */
  virtual void tileMoveFinish();
  virtual bool tileMoveStart(Tile* next_tile);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds thing information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index, 
                                   int section_index, SDL_Renderer* renderer);
  
  /* Returns the class descriptor, useful for casting */
  virtual std::string classDescriptor();

  /* Clears the entire class data */
  virtual void clear();
   
  /* Clear all movement from the stack. This is filled from add/remove
   * direction */
  virtual void clearAllMovement();

  /* Clears the target that the map thing is currently pointing at */
  void clearTarget();
  
  /* Gets the things decription */
  std::string getDescription();

  /* Returns the dialog image data, so that it may be painted */
  Frame* getDialogImage();

  /* Returns the map frames that's defined */
  Sprite* getFrames();
  
  /* Returns the height of the thing */
  uint16_t getHeight();

  /* Gets the things ID */
  int getID();
  
  /* Gets the interaction event for the thing */
  Event getInteraction();

  /* Returns the map section of the tile where this is painted */
  uint16_t getMapSection();
  
  /* Get the specific details of the movement information */
  Direction getMovement();
  bool getMovementPaused();
  virtual Direction getMoveRequest();

  /* Gets the things name */
  std::string getName();
  
  /* Returns the speed that the thing is moving at */
  uint16_t getSpeed();
  
  /* Returns the target that this thing is pointed at */
  MapThing* getTarget();
  
  /* Returns the central tile */
  Tile* getTile();
  
  /* Returns the width of the thing */
  uint16_t getWidth();

  /* Returns the location of the thing */
  float getX();
  float getY();
  
  /* Starts interaction (conversation, giving something, etc) */
  virtual bool interact();//MapPerson* initiator); // TODO: Add
  
  /* Returns if there is a move request for the given thing */
  virtual bool isMoveRequested();

  /* Returns if the thing is moving */
  bool isMoving();

  /* Is the thing centered on a tile */
  bool isOnTile();

  /* Is the thing passable - can you walk in it? */
  virtual bool isPassable();

  /* Renders the Map Thing */
  bool render(SDL_Renderer* renderer, int offset_x, int offset_y);
 
  /* Sets the things description */
  void setDescription(std::string new_description);

  /* Sets the dialog image data, for display during conversation */
  bool setDialogImage(std::string path, SDL_Renderer* renderer);
    
  /* Sets the event handler */
  void setEventHandler(EventHandler* event_handler);
  
  /* Sets the state of the thing */
  bool setFrames(Sprite* frames, bool unset_old = true);
  
  /* Sets the things height classification */
  bool setHeight(uint16_t new_height);

  /* Sets the things ID */
  bool setID(int new_id);
  void setIDPlayer();

  /* Sets the interaction event for the thing */
  bool setInteraction(Event interact_event);
  
  /* Sets if the movement is paused */
  void setMovementPaused(bool paused);
  
  /* Sets the things name */
  void setName(std::string new_name);

  /* Sets the things speed */
  void setSpeed(uint16_t speed);
    
  /* Set the tile to hook the map thing to */
  virtual bool setStartingTile(uint16_t section_id, Tile* new_tile, 
                                                    bool no_events = false);

  /* Sets the target map thing, fails if there is already a target */
  bool setTarget(MapThing* target);
  
  /* Sets the things width classification */
  bool setWidth(uint16_t new_width);

  /* Updates the thing, called on the tick */
  virtual void update(int cycle_time, Tile* next_tile);
  
  /* Unsets the thing frames, in the class */
  void unsetFrames(bool delete_frames = true);
  
  /* Unsets the starting tile */
  virtual void unsetStartingTile(bool no_events = false);
};

#endif // MAPTHING_H
