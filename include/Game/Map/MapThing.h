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
#include "Game/Map/SpriteMatrix.h"
#include "Game/Map/Tile.h"
#include "Helpers.h"
#include "XmlData.h"

class MapThing
{
public:
  /* Constructor functions */
  MapThing();
  MapThing(uint16_t width, uint16_t height, std::string name = "", 
           std::string description = "", int id = kUNSET_ID);
  MapThing(std::vector<std::vector<TileSprite*>> frames, uint16_t width, 
           uint16_t height, std::string name = "", 
           std::string description = "", int id = kUNSET_ID);

  /* Destructor function */
  virtual ~MapThing();

protected:
  /* The thing classification */
  std::string description;
  uint16_t height;
  int id;
  std::string name;
  bool passable;
  bool visible;
  uint16_t width;
  int x;
  int x_raw;
  int y;
  int y_raw;

  /* Painting information */
  Frame dialog_image;

  /* The event handler information and corresponding interact event */
  EventHandler* event_handler;
  Event interact_event;
  
  /* The main sprite frame data - contains passability and render depth */
  SpriteMatrix* sprite_set;

  /* Movement information */
  Direction movement;
  bool movement_paused;
  uint16_t speed;

  /* The target for this thing. If set, it cannot be targetted by others */
  MapThing* target;
  
  /* Tile information */
  uint16_t tile_section;
  bool tiles_set;
  uint16_t tile_x;
  uint16_t tile_y;

  /* -------------------------- Constants ------------------------- */
  const static uint16_t kDEFAULT_SPEED; /* The default thing speed */
  const static int kPLAYER_ID; /* The player ID */
  const static uint8_t kRAW_MULTIPLIER; /* The coordinate raw multiplier */
  const static int kUNSET_ID; /* The placeholder unset ID */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* This unsets the tile, at the given frame coordinate */
  virtual void unsetTile(uint32_t x, uint32_t y, bool no_events);
  
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Animates the thing, if it has multiple frames */
  virtual bool animate(int cycle_time, bool reset = false, 
                                       bool skip_head = false);

  /* Returns the rendering matrix */
  SpriteMatrix* getMatrix();

  /* Is the thing almost centered on a tile (less than 1 pulse away) */
  bool isAlmostOnTile(int cycle_time);
  
  /* Is move allowed, based on main tile and the next tile */
  bool isMoveAllowed(std::vector<std::vector<Tile*>> tile_set);
  
  /* Move the thing, based on the internal direction */
  int moveAmount(int cycle_time);
  void moveThing(int cycle_time);

  /* Sets the new direction that the class is moving in */
  bool setDirection(Direction new_direction);

  /* Sets the matrix in the class - this is used by children to utilize the
   * printing of the thing */
  bool setMatrix(SpriteMatrix* matrix);

  /* Starts and stops tile move. Relies on underlying logic for occurance */
  virtual void tileMoveFinish();
  virtual bool tileMoveStart(std::vector<std::vector<Tile*>> tile_set);

  /* Unsets the matrix in the class - no deletion occurs */
  void unsetMatrix();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds thing information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index, 
                                   int section_index, SDL_Renderer* renderer, 
                                   std::string base_path = "");
  
  /* Returns the class descriptor, useful for casting */
  virtual std::string classDescriptor();

  /* Clears the entire class data */
  virtual void clear();
   
  /* Clear all movement from the stack. This is filled from add/remove
   * direction */
  virtual void clearAllMovement();

  /* Clears the target that the map thing is currently pointing at */
  void clearTarget();

  /* Shrink the frame matrix to the valid size and removes all null and void
   * pointers.  */
  void cleanMatrix();

  /* Returns the bounding box of the thing */
  SDL_Rect getBoundingBox();
  SDL_Rect getBoundingPixels();
  
  /* Gets the things decription */
  std::string getDescription();

  /* Returns the dialog image data, so that it may be painted */
  Frame* getDialogImage();

  /* Returns the map frames that's defined */
  TileSprite* getFrame(uint32_t x, uint32_t y);
  std::vector<std::vector<TileSprite*>> getFrames();
  
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
  
  /* Returns the tile based coordinates for the top left of the thing */
  uint16_t getTileX();
  uint16_t getTileY();
  
  /* Returns the width of the thing */
  uint16_t getWidth();

  /* Returns the location of the thing */
  int getX();
  int getY();
  
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

  /* Is the rendering tiles set, for the frames */
  bool isTilesSet();

  /* Returns if the thing is visible / rendered on the screen */
  virtual bool isVisible();
  
  /* Renders the Map Thing */
  bool render(SDL_Renderer* renderer, int offset_x, int offset_y);
  bool render(SDL_Renderer* renderer, Tile* tile, int offset_x, int offset_y);
  
  /* Resets the location back to default (0,0,0), relative to the map */
  virtual void resetLocation();

  /* Sets the things description */
  void setDescription(std::string new_description);

  /* Sets the dialog image data, for display during conversation */
  bool setDialogImage(std::string path, SDL_Renderer* renderer);
    
  /* Sets the event handler */
  void setEventHandler(EventHandler* event_handler);
  
  /* Sets the state frames of the thing */
  bool setFrame(TileSprite* frame, uint32_t x, uint32_t y, 
                bool delete_old = true);
  void setFrames(std::vector<std::vector<TileSprite*>> frames, 
                 bool delete_old = false);
  
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
  
  /* Sets the starting x and y coordinate */
  void setStartingLocation(uint16_t section_id, uint16_t x, uint16_t y);

  /* Sets the set of tiles that the thing will be placed on. Needed after
   * defining a starting point.*/
  virtual bool setStartingTiles(std::vector<std::vector<Tile*>> tile_set, 
                                bool no_events = false);
  
  /* Sets the target map thing, fails if there is already a target */
  bool setTarget(MapThing* target);
  
  /* Sets the visibility of the rendering thing */
  void setVisibility(bool visible);
  
  /* Sets the things width classification */
  bool setWidth(uint16_t new_width);

  /* Updates the thing, called on the tick */
  virtual void update(int cycle_time, std::vector<std::vector<Tile*>> tile_set);
  
  /* Unsets the thing frames, in the class */
  void unsetFrame(uint32_t x, uint32_t y, bool delete_frames = true);
  void unsetFrames(bool delete_frames = true);
  
  /* Unset the rendering tiles in the class */
  virtual void unsetTiles(bool no_events = false); 
};

#endif // MAPTHING_H
