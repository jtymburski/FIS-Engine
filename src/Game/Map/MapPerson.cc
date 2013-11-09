/******************************************************************************
* Class Name: MapPerson
* Date Created: Dec 2 2012
* Inheritance: MapThing
* Description: The MapPerson class. An addition on top of MapThing that 
*              expands the Thing into possible states to allow for walking
*              in multiple directions and on multiple surfaces. At present, 
*              it allows for the 4 directions (N,S,E,W) all on one surface
*              (Ground). Future expansion is available for other surfaces
*              such as water, flying, etc.
******************************************************************************/
#include "Game/Map/MapPerson.h"

/* Constant Implementation - see header file for descriptions */
const char MapPerson::kDIR_EAST = 1;
const char MapPerson::kDIR_NORTH = 0;
const char MapPerson::kDIR_SOUTH = 2;
const char MapPerson::kDIR_UNKNOWN = -1;
const char MapPerson::kDIR_WEST = 3;
const char MapPerson::kTOTAL_DIRECTIONS = 4;
const char MapPerson::kTOTAL_SURFACES   = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Sets up an empty person with no
 *              data.
 *
 * Inputs: none
 */
MapPerson::MapPerson()
{
  initializeStates();
  clearTarget();
  
  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = EnumDb::NORTH;
}

// TODO: Need to call parent constructor
/* 
 * Description: Constructor for this class. Sets up a person with the
 *              appropriate height and width (for the tile) and other info
 *              that may be required by the person.
 *
 * Inputs: int width - the width of the tile for the person
 *         int height - the height of the tile for the person
 *         QString name - a string name for the person
 *         QString description - a description of the person
 *         int id - a unique numerical identifier, for the person
 */
MapPerson::MapPerson(int width, int height, QString name, 
                     QString description, int id)
{
  /* Class value setup */
  setAnimationSpeed(kDEFAULT_ANIMATION);
  setDescription(description);
  setHeight(height);
  setID(id);
  setName(name);
  setWidth(width);

  /* Initializes the class information */
  initializeStates();
  clearTarget();

  /* Set the default setup for what the player is standing on and facing */
  surface = GROUND;
  direction = EnumDb::NORTH;
}

/* 
 * Description: Destructor function 
 */
MapPerson::~MapPerson()
{
  clear();
  states.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Initializes the internal states for all possibilities, based
 *              on the constants. Only called once, by the constructor for 
 *              initialization.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::initializeStates()
{
  states.clear();

  for(int i = 0; i < kTOTAL_SURFACES; i++)
  {
    QList<Sprite*> row;

    for(int j = 0; j < kTOTAL_DIRECTIONS; j++)
      row.append(0);
    states.append(row);
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Adds the direction from the direction stack, if it's not
 *              currently on the stack. Used for movement of the person.
 *
 * Inputs: EnumDb::Direction direction - the direction to add
 * Output: none
 */
void MapPerson::addDirection(EnumDb::Direction direction)
{
  if(!movement_stack.contains(direction))
    movement_stack.append(direction);
}

/* 
 * Description: Integer converter from Direction enumerator to ensure that 
 *              the compiler type doesn't affect the operation. This info
 *              is used within internal array operation
 * 
 * Inputs: EnumDb::Direction dir - the direction enumerator
 * Output: int - the converted integer from the enumerator
 */
int MapPerson::dirToInt(EnumDb::Direction dir)
{
  if(dir == EnumDb::NORTH)
    return kDIR_NORTH;
  else if(dir == EnumDb::EAST)
    return kDIR_EAST;
  else if(dir == EnumDb::SOUTH)
    return kDIR_SOUTH;
  else if(dir == EnumDb::WEST)
    return kDIR_WEST;
  return kDIR_UNKNOWN;
}

/* 
 * Description: Direction enumerator converter from integer to ensure that 
 *              the compiler type doesn't affect the operation. This info
 *              is used within internal array operation
 * 
 * Inputs: int dir_index - the direction index, associated with the enum
 * Output: EnumDb::Direction - the direction enumerator, from the index
 */
EnumDb::Direction MapPerson::intToDir(int dir_index)
{
  if(dir_index == kDIR_NORTH)
    return EnumDb::NORTH;
  else if(dir_index == kDIR_EAST)
    return EnumDb::EAST;
  else if(dir_index == kDIR_SOUTH)
    return EnumDb::SOUTH;
  else if(dir_index == kDIR_WEST)
    return EnumDb::WEST;
  return EnumDb::DIRECTIONLESS;
}

/*
 * Description: Removes the direction from the direction stack, if it's
 *              currently on the stack. Used for movement of the person.
 *
 * Inputs: EnumDb::Direction direction - the direction to remove
 * Output: none
 */
void MapPerson::removeDirection(EnumDb::Direction direction)
{
  if(movement_stack.contains(direction))
    movement_stack.removeAt(movement_stack.indexOf(direction));
}

/* 
 * Description: Sets a new direction for the person on the map. It will update
 *              the parent frame so a new classifier is printed.
 * 
 * Inputs: MovementDirection direction - the new direction to set
 * Output: bool - indicates if the directional movement changed
 */
bool MapPerson::setDirection(EnumDb::Direction direction, bool set_movement)
{
  bool changed = (this->direction != direction);
  bool movement_changed = false;
  
  /* If moving, set the direction in map thing */
  if(set_movement)
    movement_changed = MapThing::setDirection(direction);
  else
    MapThing::setDirection(EnumDb::DIRECTIONLESS);

  /* If it's a movement direction, rotate the fellow */
  if(direction != EnumDb::DIRECTIONLESS)
  {
    if(changed && states[surface][dirToInt(direction)] != 0)
      MapThing::setFrames(states[surface][dirToInt(direction)], false);

    /* Finally set the in class direction */
    this->direction = direction;
  }

  /* Only update the animation if the direction has changed */
  if(movement_changed)
    updateAnimation();

  return movement_changed;
}

/* 
 * Description: The tile move finish call. To be called after a move and it's
 *              determined that the thing is on the main tile (for the first
 *              time). Essentially just cleans up the previous tile pointer.
 *              This is reimplemented from MapThing since it uses "Person"
 *              instead of "Thing" in tile.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::tileMoveFinish()
{
  if(tile_previous != 0)
    tile_previous->unsetPerson(getID() != kPLAYER_ID);
  tile_previous = 0;
}

// TODO
/* 
 * Description: The tile move initialization call. To be called after
 *              passability checks have passed and the thing can be moved to
 *              the next tile. Sets the new main pointer and moves the current
 *              to the old spot. This is reimplemented from MapThing since it
 *              uses "Person" instead of "Thing" in tile.
 * 
 * Inputs: Tile* next_tile - the tile to move to
 * Output: bool - if the tile start was successfully started
 */
bool MapPerson::tileMoveStart(Tile* next_tile)
{
  if(next_tile != 0 && !next_tile->isPersonSet())
  {
    tile_previous = tile_main;
    tile_main = next_tile;
    tile_main->setPerson(this, getID() != kPLAYER_ID);
    return true;
  }
  return false;
}

/*
 * Description: Updates the current animation state. This is called each time
 *              the direction has changed or if movement has been initiated.
 *              It is relevant for applicable animation speeds to be processed.
 *
 * Inputs: none
 * Output: none
 */
void MapPerson::updateAnimation()
{
  if((direction == EnumDb::WEST || direction == EnumDb::EAST || 
                                   direction == EnumDb::SOUTH) && 
     states[surface][dirToInt(direction)] != 0 && 
     states[surface][dirToInt(EnumDb::NORTH)] != 0)
  {
    int e_w_count = states[surface][dirToInt(direction)]->getSize();
    int n_s_count = states[surface][dirToInt(EnumDb::NORTH)]->getSize();
    MapThing::setAnimationSpeed(n_s_count * 1.0 * 
                                animation_vertical / e_w_count);
  }
  else
  {
    MapThing::setAnimationSpeed(animation_vertical);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds person information from the XML file. This will be
 *              virtually called by children where the child will deal with
 *              additional sets needed and then pass call to this. Works off
 *              reading the XML file in a strict manner. Passes call to parent
 *              after it is complete.
 *
 * Inputs: XmlData data - the read XML data
 *         int file_index - the index in the xml data where this detail begins
 *         int section_index - the map section index of the person
 * Output: bool - status if successful
 */
bool MapPerson::addThingInformation(XmlData data, int file_index, 
                                                  int section_index)
{
  QList<QString> elements = data.getTailElements(file_index + 1);
  bool success = true;
  
  /* Parse the identifier for setting the person information */
  if(elements.size() >= 3)
  {
    QStringList identifiers = elements[2].split("_");

    /*--------------------- FRAMES -----------------*/
    if(identifiers[0] == "frames")
    {
      Sprite* new_sprite = new Sprite(data.getDataString(&success));
      new_sprite->execImageAdjustments(identifiers.mid(1));
      
      /* Create the surface identifier */
      SurfaceClassifier surface = GROUND;
      if(elements[0] == "ground")
        surface = GROUND;
      
      /* Create the direction identifier */
      EnumDb::Direction direction = EnumDb::DIRECTIONLESS;
      if(elements[1] == "north")
        direction = EnumDb::NORTH;
      else if(elements[1] == "south")
        direction = EnumDb::SOUTH;
      else if(elements[1] == "east")
        direction = EnumDb::EAST;
      else if(elements[1] == "west")
        direction = EnumDb::WEST;

      /* Attempt to set the state */
      if(!setState(surface, direction, new_sprite))
      {
        delete new_sprite;
        success = false;
      }
    }
  }
  
  /* Proceed to parent */
  return (success && 
          MapThing::addThingInformation(data, file_index, section_index));
}

/*
 * Description: This is the class descriptor. Primarily used for encapsulation
 *              to determine which class to cast it to for specific parameters.
 *
 * Inputs: none
 * Output: QString - the string descriptor, it will be the same as the class
 *                   name. For example, "MapThing", "MapPerson", etc.
 */
QString MapPerson::classDescriptor()
{
  return "MapPerson";
}

/* 
 * Description: Clears out all person states that were initialized into this
 *              class. This includes the appropriate procedure of cleaning
 *              up the parent class and deleting all the pointers.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::clear()
{
  for(int i = 0; i < kTOTAL_SURFACES; i++)
  {
    unsetState( (SurfaceClassifier)i, EnumDb::NORTH );
    unsetState( (SurfaceClassifier)i, EnumDb::EAST );
    unsetState( (SurfaceClassifier)i, EnumDb::SOUTH );
    unsetState( (SurfaceClassifier)i, EnumDb::WEST );
  }
  
  /* Clear direction and movement information */
  direction = EnumDb::NORTH;
  movement = EnumDb::DIRECTIONLESS;
  clearAllMovement();
  surface = GROUND;
  setAnimationSpeed(kDEFAULT_ANIMATION);

  MapThing::clear();
}

/* 
 * Description: Clears all active movement pointers that are in the current
 *              movement stack. This allows to halt all movement once the
 *              person has reached the next tile.
 * 
 * Inputs: none
 * Output: none
 */
void MapPerson::clearAllMovement()
{
  movement_stack.clear();
}
   
/* Gets the animation speed of the person */
short MapPerson::getAnimationSpeed()
{
  return animation_vertical;
}

/* 
 * Description: Returns the direction that the MapPerson is currently set to. 
 * 
 * Inputs: none
 * Output: EnumDb::Direction - the direction enumerator for this class
 */
EnumDb::Direction MapPerson::getDirection()
{
  return direction;
}

/* 
 * Description: This is a reimplemented call from MapThing, gets the actual
 *              move request which will be the last key pressed by the
 *              keyboard, since it's utilized as a stack.
 * 
 * Inputs: none
 * Output: EnumDb::Direction - the direction movement.
 */
EnumDb::Direction MapPerson::getMoveRequest()
{
  if(isMoveRequested())
    return movement_stack.last();
  return EnumDb::DIRECTIONLESS;
}

/* 
 * Description: Returns the surface classifier to what the map person is 
 *              standing on.
 * 
 * Inputs: none
 * Output: SurfaceClassifier - the surface enumerator for this class
 */
MapPerson::SurfaceClassifier MapPerson::getSurface()
{
  return surface;
}

bool MapPerson::initializeGl()
{
  bool success = true;
  Sprite* state = 0;
  
  for(int i = 0; i < kTOTAL_SURFACES; i++)
  {
    /* Check the NORTH moving state */
    state = states[(SurfaceClassifier)i][dirToInt(EnumDb::NORTH)];
    if(state != 0)
      state->initializeGl();
      
    /* Check the EAST moving state */
    state = states[(SurfaceClassifier)i][dirToInt(EnumDb::EAST)];
    if(state != 0)
      state->initializeGl();
    
    /* Check the SOUTH moving state */
    state = states[(SurfaceClassifier)i][dirToInt(EnumDb::SOUTH)];
    if(state != 0)
      state->initializeGl();
      
    /* Check the WEST moving state */
    state = states[(SurfaceClassifier)i][dirToInt(EnumDb::WEST)];
    if(state != 0)
      state->initializeGl();
  }
  
  return success;
}

/* 
 * Description: Reimplemented is move request call from map thing. This 
 *              utilizes the key press stack to get movement options.
 * 
 * Inputs: none
 * Output: bool - returns if a move is requested.
 */
bool MapPerson::isMoveRequested()
{
  return !movement_stack.isEmpty();
}

void MapPerson::keyFlush()
{
  movement_stack.clear();
}

/*
 * Description: The key press event implementation. Handles all the movement
 *              for the map person, if redirected here.
 *
 * Inputs: QKeyEvent* event - the key event to analyze
 * Output: none
 */
void MapPerson::keyPress(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down)
    addDirection(EnumDb::SOUTH);
  else if(event->key() == Qt::Key_Up)
    addDirection(EnumDb::NORTH);
  else if(event->key() == Qt::Key_Right)
    addDirection(EnumDb::EAST);
  else if(event->key() == Qt::Key_Left)
    addDirection(EnumDb::WEST);
}

/*
 * Description: The key release event implementation. Handles all the 
 *              movement for the map person, if redirected here.
 *
 * Inputs: QKeyEvent* event - the key event to analyze
 * Output: none
 */
void MapPerson::keyRelease(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Down)
    removeDirection(EnumDb::SOUTH);
  else if(event->key() == Qt::Key_Up)
    removeDirection(EnumDb::NORTH);
  else if(event->key() == Qt::Key_Right)
    removeDirection(EnumDb::EAST);
  else if(event->key() == Qt::Key_Left)
    removeDirection(EnumDb::WEST);
}

/* Sets the animation time for each frame */
bool MapPerson::setAnimationSpeed(short frame_time)
{
  if(frame_time >= 0)
  {
    animation_vertical = frame_time;
    updateAnimation();

    return true;
  }
  return false;
}

/* 
 * Description: Sets the connected tile information for the map person. This is
 *              the initial starting point and where the person is initially
 *              placed. If this is unset, the person will not move or paint.
 *              This replaces the generic mapthing call to allow for map 
 *              person in tile to be modified.
 *
 * Inputs: int section_id - the map id that the tile is from
 *         Tile* new_tile - the tile to set the starting pointer to
 *         bool no_events - don't execute any events when set
 * Output: bool - status if the change was able to occur
 */
bool MapPerson::setStartingTile(int section_id, Tile* new_tile, bool no_events)
{
  if(section_id >= 0 && new_tile != 0 && !new_tile->isPersonSet())
  {
    /* Unsets the old tile information */
    unsetStartingTile(no_events);
  
    /* Set the new tile */
    tile_main = new_tile;
    this->x = tile_main->getPixelX();
    this->y = tile_main->getPixelY();
    tile_main->setPerson(this, no_events);
    tile_section = section_id;
    
    return true;
  }

  return false;
}

/* 
 * Description: Sets a state within the class, based on the double set of 
 *              enumerators, for surface and direction. This will automatically
 *              unset a state that is currently in its place, if one does
 *              exist. 
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         EnumDb::Direction direction - the direction for the state
 *         Sprite* frames - the frame data to set the state at
 * Output: bool - if the call was successful
 */
bool MapPerson::setState(SurfaceClassifier surface, 
                         EnumDb::Direction direction, Sprite* frames)
{
  /* Only proceed with insertion if the sprite and state data is valid */
  if(frames != 0 && frames->getSize() > 0)
  {
    unsetState(surface, direction);
    states[surface][dirToInt(direction)] = frames;
    
    /* If the updated state is the active one, automatically set the printable
     * sprite */
    if(this->surface == surface && this->direction == direction)
    {
      MapThing::setFrames(states[surface][dirToInt(direction)], false);
      updateAnimation();
    }

    //states[surface][dirToInt(direction)]->initializeGl();
    
    return true;
  }

  return false;
}

/* 
 * Description: Sets a new surface for the person on the map to sit on. It 
 *              will update the parent frame so the new classifier is printed.
 * 
 * Inputs: SurfaceClassifier surface - the new surface to set
 * Output: none
 */
void MapPerson::setSurface(SurfaceClassifier surface)
{
  this->surface = surface;

  if(states[surface][dirToInt(direction)] != 0)
    MapThing::setFrames(states[surface][dirToInt(direction)], false);
}

/*
 * Description: Updates the state of the person. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: float cycle_time - the time elapsed between updates
 *         Tile* next_tile - the next tile to be travelled onto
 * Output: none 
 */
void MapPerson::updateThing(float cycle_time, Tile* next_tile)
{
  bool can_move = isMoveAllowed(next_tile) && !getMovementPaused();
  bool reset = false;
  
  /* Once a tile end has reached, cycle the movement direction */
  if(isAlmostOnTile(cycle_time))
  {
    tileMoveFinish();
    
    /* If paused and there is a target, change direction to the target */
    if(getMovementPaused())
    {
      if(getTarget())
      {
        int delta_x = getTile()->getX() - getTarget()->getTile()->getX();
        int delta_y = getTile()->getY() - getTarget()->getTile()->getY();

        if(delta_x < 0)
          setDirection(EnumDb::EAST, can_move);
        else if(delta_x > 0)
          setDirection(EnumDb::WEST, can_move);
        else if(delta_y < 0)
          setDirection(EnumDb::SOUTH, can_move);
        else if(delta_y > 0)
          setDirection(EnumDb::NORTH, can_move);
      }
      else
      {
        setDirection(EnumDb::DIRECTIONLESS);
      }

      reset = true;
    }
    /* Update direction based on move request and if it should move */
    else if(isMoveRequested())
    {
      /* Set the new direction and if the direction is changed, or it's
       * not allowed to move, recenter the thing */
      reset = !can_move;
      if(setDirection(getMoveRequest(), can_move) || !can_move)
      {
        x = tile_main->getPixelX();
        y = tile_main->getPixelY();
        reset = true;
      }
      
      /* If it can move, initiate tile shifting */
      if(can_move)
        tileMoveStart(next_tile);
    }
    /* If there is no move request, stop movement */
    else
    {
      if(tile_main != 0)
      {
        x = tile_main->getPixelX();
        y = tile_main->getPixelY();
      }

      setDirection(EnumDb::DIRECTIONLESS);
      reset = true;
    }
  }

  /* Proceed to move the thing */
  moveThing(cycle_time);

  /* Only animate if the direction exists */
  animate(cycle_time, reset, getMovement() != EnumDb::DIRECTIONLESS);
}

/* 
 * Description: Unsets the state in this class based on the two classifiers.
 *              This includes the appropriate delete functionality for the 
 *              stored pointers. Also, unsets the parent classifier, if 
 *              the current state data is being used.
 * 
 * Inputs: SurfaceClassifier surface - the surface classifier for the state
 *         EnumDb::Direction direction - the direction for the state
 * Output: none
 */
void MapPerson::unsetState(SurfaceClassifier surface, 
                           EnumDb::Direction direction)
{
  delete states[surface][dirToInt(direction)];
  states[surface][dirToInt(direction)] = 0;

  /* Clear out the parent call if the direction or surface lines up */
  if(this->surface == surface && this->direction == direction)
    MapThing::unsetFrames(false);
}

/*
 * Description: Unsets the starting tile location that is stored within the
 *              thing.
 *
 * Inputs: bool no_events - fire no events when unsetting
 * Output: none
 */
void MapPerson::unsetStartingTile(bool no_events)
{ 
  /* Stop movement */
  setDirection(EnumDb::DIRECTIONLESS);
  
  /* Unset the previous tile */
  if(tile_previous != 0)
    tile_previous->unsetPerson(no_events);
  tile_previous = 0;
  
  /* Unset the main tile */
  if(tile_main != 0)
    tile_main->unsetPerson(no_events);
  tile_main = 0;
    
  /* Resets the coordinates */
  this->x = 0;
  this->y = 0;
}
