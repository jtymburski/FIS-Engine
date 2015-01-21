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
  MapThing(int id, std::string name = "", std::string description = "");
  MapThing(std::vector<std::vector<TileSprite*>> frames, int id = kUNSET_ID,  
           std::string name = "", std::string description = "");

  /* Destructor function */
  virtual ~MapThing();

protected:
  /* The base class */
  MapThing* base;
  ThingBase base_category;

  /* The thing classification */
  std::string description;
  int id;
  std::string name;
  bool visible;
  float x;
  float y;

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

  /* Startinc coordinate information */
  uint16_t starting_section;
  uint16_t starting_x;
  uint16_t starting_y;

  /* The target for this thing. If set, it cannot be targetted by others */
  MapThing* target;
  
  /* Tile information */
  std::vector<std::vector<Tile*>> tile_main;
  std::vector<std::vector<Tile*>> tile_prev;
  uint16_t tile_section;

  /* -------------------------- Constants ------------------------- */
  const static uint16_t kDEFAULT_SPEED; /* The default thing speed */
  const static float kMOVE_FACTOR; /* Move factor for pixel movement */
  const static int kPLAYER_ID; /* The player ID */
  const static uint8_t kRAW_MULTIPLIER; /* The coordinate raw multiplier */
  const static int kUNSET_ID; /* The placeholder unset ID */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Animates the thing, if it has multiple frames */
  virtual bool animate(int cycle_time, bool reset = false, 
                                       bool skip_head = false);
  /* Returns the tile index as a decimal to indicate what percentage of the
   * next tile it resides in */
  float getFloatTileX();
  float getFloatTileY();

  /* Returns the rendering matrix */
  SpriteMatrix* getMatrix();

  /* Returns the appropriate tile. NULL if unset or beyond range */
  Tile* getTileMain(uint32_t x, uint32_t y);
  Tile* getTilePrevious(uint32_t x, uint32_t y);
  
  /* Is the thing almost centered on a tile (less than 1 pulse away) */
  bool isAlmostOnTile(int cycle_time);
  
  /* Is move allowed, based on main tile and the next tile */
  bool isMoveAllowed(std::vector<std::vector<Tile*>> tile_set, 
                     Direction move_request);
  virtual bool isTileMoveAllowed(Tile* previous, Tile* next, 
                                 uint8_t render_depth, Direction move_request);

  /* Move the thing, based on the internal direction */
  virtual float moveAmount(uint16_t cycle_time);
  void moveThing(int cycle_time);

  /* Sets the new direction that the class is moving in */
  bool setDirection(Direction new_direction);

  /* Sets the matrix in the class - this is used by children to utilize the
   * printing of the thing */
  bool setMatrix(SpriteMatrix* matrix);

  /* Sets the tile of the selected with the corresponding frames */
  virtual bool setTile(Tile* tile, TileSprite* frames, 
                       bool no_events = true);
  virtual void setTileFinish(Tile* old_tile, Tile* new_tile, 
                             uint8_t render_depth, bool reverse_last = false, 
                             bool no_events = false);
  virtual bool setTileStart(Tile* old_tile, Tile* new_tile, 
                            uint8_t render_depth, bool no_events = false);

  /* Starts and stops tile move. Relies on underlying logic for occurance */
  virtual void tileMoveFinish(bool no_events = true);
  virtual bool tileMoveStart(std::vector<std::vector<Tile*>> tile_set,
                             bool no_events = true);

  /* Unsets the matrix in the class - no deletion occurs */
  void unsetMatrix();

  /* This unsets the tile, at the given frame coordinate */
  virtual void unsetTile(uint32_t x, uint32_t y, bool no_events);

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

  /* Shrink the frame matrix to the valid size and removes all null and void
   * pointers.  */
  virtual bool cleanMatrix();

  /* Clears the entire class data */
  virtual void clear();
   
  /* Clear all movement from the stack. This is filled from add/remove
   * direction */
  virtual void clearAllMovement();

  /* Clears the target that the map thing is currently pointing at */
  void clearTarget();

  /* Returns the base class */
  MapThing* getBase();
  
  /* Returns the bounding box of the thing */
  SDL_Rect getBoundingBox();
  SDL_Rect getBoundingPixels();
 
  /* Returns center pixel of thing */
  uint32_t getCenterX();
  uint32_t getCenterY();

  /* Gets the things decription */
  std::string getDescription();

  /* Returns the dialog image data, so that it may be painted */
  Frame* getDialogImage();

  /* Returns the map frames that's defined */
  TileSprite* getFrame(uint32_t x, uint32_t y);
  TileSprite* getFrameMain(Tile* tile);
  TileSprite* getFramePrevious(Tile* tile);
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

  /* Returns the move X/Y, as a float 0 to 1 with respect to width/height */
  float getMoveX();
  float getMoveY();

  /* Gets the things name */
  std::string getName();
   
  /* Returns the passability of the tile based on direction */
  bool getPassabilityEntering(Tile* frame_tile, Direction dir);
  bool getPassabilityExiting(Tile* frame_tile, Direction dir); 
  
  /* Returns the speed that the thing is moving at */
  uint16_t getSpeed();
 
  /* Returns the starting coordinates - as set by setStartingLocation() */
  uint16_t getStartingSection();
  uint16_t getStartingX();
  uint16_t getStartingY();

  /* Returns the target that this thing is pointed at */
  MapThing* getTarget();
  
  /* Returns the tile height */
  uint16_t getTileHeight();

  /* Returns array of set tiles, with only the render depth valid (all others
   * are null) */
  std::vector<std::vector<Tile*>> getTileRender(uint8_t render_depth);

  /* Returns tile width */
  uint16_t getTileWidth();

  /* Returns x/y of the top left, in tile units */
  uint16_t getTileX();
  uint16_t getTileY();
  
  /* Returns the width of the thing */
  uint16_t getWidth();

  /* Returns the location of the thing */
  uint32_t getX();
  uint32_t getY();
  
  /* Starts interaction (conversation, giving something, etc) */
  virtual bool interact(MapPerson* initiator);
  
  /* Returns if there is a move request for the given thing */
  virtual bool isMoveRequested();

  /* Returns if the thing is moving */
  bool isMoving();

  /* Is the thing centered on a tile */
  bool isOnTile();

  /* Is the rendering tiles set, for the frames */
  bool isTilesSet();

  /* Returns if the thing is visible / rendered on the screen */
  virtual bool isVisible();
  
  /* Renders the Map Thing */
  bool render(SDL_Renderer* renderer, int offset_x, int offset_y);
  bool renderMain(SDL_Renderer* renderer, Tile* tile, uint8_t render_depth,
                  int offset_x, int offset_y);
  bool renderPrevious(SDL_Renderer* renderer, Tile* tile, uint8_t render_depth,
                      int offset_x, int offset_y);
 
  /* Resets the location back to default (0,0,0), relative to the map */
  virtual void resetLocation();

  /* Sets the base class */
  bool setBase(MapThing* base);

  /* Sets the things description */
  void setDescription(std::string new_description);

  /* Sets the dialog image data, for display during conversation */
  bool setDialogImage(std::string path, SDL_Renderer* renderer);
    
  /* Sets the event handler */
  void setEventHandler(EventHandler* event_handler);
  
  /* Sets the state frames of the thing */
  virtual bool setFrame(TileSprite* frame, uint32_t x, uint32_t y, 
                        bool delete_old = true);
  virtual void setFrames(std::vector<std::vector<TileSprite*>> frames, 
                         bool delete_old = false);
  
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
                                uint16_t section, bool no_events = true);
  
  /* Sets the target map thing, fails if there is already a target */
  bool setTarget(MapThing* target);
  
  /* Sets the visibility of the rendering thing */
  void setVisibility(bool visible);
  
  /* Triggers walk on / walk off events on the thing */
  virtual void triggerWalkOff(MapPerson* trigger);
  virtual void triggerWalkOn(MapPerson* trigger);
  
  /* Updates the thing, called on the tick */
  virtual void update(int cycle_time, std::vector<std::vector<Tile*>> tile_set);
  
  /* Unsets the thing frames, in the class */
  void unsetFrame(uint32_t x, uint32_t y, bool delete_frames = true);
  virtual void unsetFrames(bool delete_frames = true);
  
  /* Unset the rendering tiles in the class */
  virtual void unsetTiles(bool no_events = false); 
};

#endif // MAPTHING_H
