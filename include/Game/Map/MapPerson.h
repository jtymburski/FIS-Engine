/*******************************************************************************
 * Class Name: MapPerson
 * Date Created: Oct 28 2012
 * Inheritance: MapThing
 * Description: The MapPerson class. An addition on top of MapThing that
 *              expands the Thing into possible states to allow for walking
 *              in multiple directions and on multiple surfaces. At present,
 *              it allows for the 4 directions (N,S,E,W) all on one surface
 *              (Ground). Future expansion is available for other surfaces
 *              such as water, flying, etc.
 ******************************************************************************/
#ifndef MAPPERSON_H
#define MAPPERSON_H

#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapThing.h"

class MapPerson : public MapThing
{
public:
  /* Constructor functions */
  MapPerson();
  MapPerson(int id, std::string name = "", std::string description = "");

  /* Destructor function */
  virtual ~MapPerson();

  /* The surface classification, first layer of states 2D vector
   *  GROUND - is on a dirt like substance (where walking is possible) */
  enum SurfaceClassifier{GROUND = 0};

protected:
/* The direction of the player */
  Direction direction;

  /* The surface that the person is walking on */
  SurfaceClassifier surface;

private:
  /* Movement freeze */
  bool move_freeze;

  /* The current movement direction */
  std::vector<Direction> movement_stack;

  /* Is the person running? */
  bool running;

  /* Sound delay */
  int sound_delay;

  /* The initial starting direction */
  Direction starting_dir;

  /* Set of all states for person. 1st index is surface (water, ground, etc)
   * and then 2nd index is direction facing */
  std::vector<std::vector<SpriteMatrix*>> states;

  /* A counter of steps made on the map */
  uint32_t steps;

  /* -------------------------- Constants ------------------------- */
  const static int kDELAY_MAX; /* The maximum delay (in milliseconds) */
  const static int kDELAY_MIN; /* The minimum delay (in milliseconds) */
  const static int8_t kDIR_EAST;        /* The EAST direction for moving */
  const static int8_t kDIR_NORTH;       /* The NORTH direction for moving */
  const static int8_t kDIR_SOUTH;       /* The SOUTH direction for moving */
  const static int8_t kDIR_UNKNOWN;     /* The unknown direction value */
  const static int8_t kDIR_WEST;        /* The WEST direction for moving */
  const static uint8_t kTOTAL_DIRECTIONS; /* The max # of directions to move */
  const static uint8_t kTOTAL_SURFACES; /* The max # of surfaces to walk on */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Deletes the states */
  void deleteStates();

  /* Get top most sound trigger ID */
  int32_t getSoundTrigger();

  /* Initializes the states stack to an empty set */
  void initializeStates();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Add movement direction to the stack */
  void addDirection(Direction direction);

  /* Check if the tile can be set with the person */
  virtual bool canSetTile(Tile* tile, TileSprite* frames,
                          bool avoid_player = false);

  /* Is move allowed, based on main tile and the next tile */
  virtual bool isTileMoveAllowed(Tile* previous, Tile* next, uint8_t
                                 render_depth, Direction move_request);

  /* Determine the move amount of the person */
  float moveAmount(uint16_t cycle_time);

  /* Remove movement direction from the stack, if it exists */
  void removeDirection(Direction direction);

  /* Sets the direction that the person is travelling in */
  virtual bool setDirection(Direction direction, bool set_movement = true);

  /* Sets the tile of the selected with the corresponding frames */
  virtual bool setTile(Tile* tile, TileSprite* frames,
                       bool no_events = true);
  virtual void setTileFinish(Tile* old_tile, Tile* new_tile,
                             uint8_t render_depth, bool reverse_last = false,
                             bool no_events = false);
  virtual bool setTileStart(Tile* old_tile, Tile* new_tile,
                            uint8_t render_depth, bool no_events = false);

  /* Starts tile move. Relies on underlying logic for occurance */
  bool tileMoveStart(std::vector<std::vector<Tile*>> tile_set,
                     bool no_events = false);

  /* This unsets the tile, at the given frame coordinate */
  virtual void unsetTile(uint32_t x, uint32_t y, bool no_events);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds person information from the XML file. Will be virtually re-called
   * by all children for proper operation */
  virtual bool addThingInformation(XmlData data, int file_index,
                                   int section_index, SDL_Renderer* renderer,
                                   std::string base_path = "");

  /* Returns the class descriptor, useful for casting */
  virtual ThingBase classDescriptor();

  /* Shrink the frame matrix to the valid size and removes all null and void
   * pointers.  */
  virtual bool cleanMatrix(bool first_call = true);

  /* Clears all information from the class (including deleting necessary
   * pointers) */
  virtual void clear();

  /* Clear all movement from the stack. This is filled from add/remove
   * direction */
  void clearAllMovement();

  /* Returns the direction that this person is travelling in */
  Direction getDirection();

  /* Returns the move request in the class (virtual reimplemented) */
  Direction getMoveRequest();

  /* Returns the stored player. Blank in Person, used in NPC child */
  virtual MapPerson* getPlayer();

  /* Returns the starting facing direction of the person */
  Direction getStartingDirection() const;

  /* Returns the number of tiles walked on for the person */
  uint32_t getStepCount();

  /* Returns the predicted move request in the class */
  virtual Direction getPredictedMoveRequest();

  /* Returns the state at the defined surface and direction */
  SpriteMatrix* getState(SurfaceClassifier surface, Direction direction,
                         bool include_base = true);

  /* Returns the surface that this person resides on */
  SurfaceClassifier getSurface();

  /* Returns if the NPC will force interaction - always false for player */
  virtual bool isForcedInteraction(bool false_if_active = true);

  /* Determines if the persons movement is frozen */
  bool isMoveFrozen();

  /* Determines if there is an active move request (virtual reimplemented) */
  bool isMoveRequested();

  /* Determines if the person is running */
  bool isRunning();

  /* Key Down/Flush/Up events handled */
  void keyDownEvent(SDL_KeyboardEvent event);
  void keyFlush();
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Resets the tile position */
  virtual bool resetPosition();

  /* Resets the step count */
  void resetStepCount();

  /* Sets the base class */
  virtual bool setBase(MapThing* base);

  /* Sets if the person movement is frozen at the current tile */
  void setMoveFreeze(bool freeze);

  /* Sets if the person is running */
  void setRunning(bool running);

  /* Sets the starting direction of the person */
  bool setStartingDirection(Direction starting);

  /* Sets a new state to add into the states list */
  bool setState(TileSprite* frame, SurfaceClassifier surface,
                Direction direction, uint32_t x, uint32_t y,
                bool delete_old = true);
  bool setStates(std::vector<std::vector<TileSprite*>> frames,
                 SurfaceClassifier surface, Direction direction,
                 bool delete_old = true);

  /* Sets the surface that the person travels on */
  void setSurface(SurfaceClassifier surface);

  /* Updates the thing, based on the tick */
  virtual Floatinate update(int cycle_time,
                            std::vector<std::vector<Tile*>> tile_set);

  /* Unsets a state, if it exists, to remove from the stack of states */
  void unsetState(SurfaceClassifier surface, Direction direction, uint32_t x,
                  uint32_t y, bool delete_frames = true);
  void unsetStates(SurfaceClassifier surface, Direction direction,
                   bool delete_frames = true);
  void unsetStates(bool delete_frames = true);

  /* Unset the rendering tiles in the class */
  virtual void unsetTiles(bool no_events = false);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Direction enumerator to/from integer converters */
  static int dirToInt(Direction dir);
  static Direction intToDir(int dir_index);
};

#endif // MAPPERSON_H
