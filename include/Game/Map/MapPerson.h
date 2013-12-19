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

#include "Game/Map/MapThing.h"

class MapPerson : public MapThing
{
public:
  /* Constructor functions */
  MapPerson();
  MapPerson(uint16_t width, uint16_t height, std::string name = "", 
            std::string description = "", int id = kUNSET_ID);

  /* Destructor function */
  virtual ~MapPerson();

  /* The surface classification, first layer of states 2D vector
   *  GROUND - is on a dirt like substance (where walking is possible) */
  enum SurfaceClassifier{GROUND = 0};

private:
  /* The direction of the player and the current movement direction */
  Direction direction;
  std::vector<Direction> movement_stack;

  /* The initial tile location to start movement on */
  uint16_t starting_section;
  Tile* starting_tile;

  /* Set of all states for person. 1st layer is surface (water, ground, etc)
   * and then 2nd layer is direction facing */
  std::vector< std::vector<Sprite*> > states;

  /* A counter of steps made on the map */
  uint32_t steps;
  
  /* The surface that the person is walking on */
  SurfaceClassifier surface;
  
  /* -------------------------- Constants ------------------------- */
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
  /* Initializes the states stack to an empty set */
  void initializeStates();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Add movement direction to the stack */
  void addDirection(Direction direction);

  /* Direction enumerator to/from integer converters */
  int dirToInt(Direction dir);
  Direction intToDir(int dir_index);

  /* Remove movement direction from the stack, if it exists */
  void removeDirection(Direction direction);

  /* Sets the direction that the person is travelling in */
  bool setDirection(Direction direction, bool set_movement = true);
  
  /* Starts and stops tile move. Relies on underlying logic for occurance */
  void tileMoveFinish();
  bool tileMoveStart(Tile* next_tile);

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
  virtual std::string classDescriptor();

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
  
  /* Returns the number of tiles walked on for the person */
  uint32_t getStepCount();
  
  /* Returns the predicted move request in the class */
  virtual Direction getPredictedMoveRequest();
  
  /* Returns the state at the defined surface and direction */
  Sprite* getState(SurfaceClassifier surface, Direction direction);
  
  /* Returns the surface that this person resides on */
  SurfaceClassifier getSurface();
  
  /* Determines if there is an active move request (virtual reimplemented) */
  bool isMoveRequested();
  
  /* Key Down/Flush/Up events handled */
  void keyDownEvent(SDL_KeyboardEvent event);
  void keyFlush();
  void keyUpEvent(SDL_KeyboardEvent event);
  
  /* Resets the tile position */
  bool resetPosition();
  
  /* Set the tile to hook the map person to */
  bool setStartingTile(uint16_t section_id, Tile* new_tile, 
                                            bool no_events = false);

  /* Sets a new state to add into the states list */
  bool setState(SurfaceClassifier surface, Direction direction, Sprite* frames);

  /* Sets the surface that the person travels on */
  void setSurface(SurfaceClassifier surface);
 
  /* Sets the white mask texture, for tuning brightness */
  virtual bool setWhiteMask(SDL_Texture* texture);
  
  /* Updates the thing, based on the tick */
  virtual void update(int cycle_time, Tile* next_tile);

  /* Unsets a state, if it exists, to remove from the stack of states */
  void unsetState(SurfaceClassifier surface, Direction direction);
  
  /* Unsets the starting tile */
  void unsetStartingTile(bool no_events = false);
};

#endif // MAPPERSON_H
