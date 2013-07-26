/******************************************************************************
 * Class Name: MapThing
 * Date Created: Dec 2 2012
 * Inheritance: QObject
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapWalkOver, MapSolid, etc. Also handles the basic setup for 
 *              name, description, id, sprite. No interaction is handled in 
 *              this class since its a generic parent.
 *****************************************************************************/
#include "Game/Map/MapThing.h"

/* Constant Implementation - see header file for descriptions */
const short MapThing::kDEFAULT_ANIMATION = 250;
const short MapThing::kDEFAULT_SPEED = 150;
const short MapThing::kMINIMUM_ID =  0;
const short MapThing::kUNSET_ID   = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Sets up an empty thing with no
 *              data.
 *
 * Inputs: none
 */
MapThing::MapThing()
{
  state = 0;
  tile_main = 0;
  tile_previous = 0;
  clear();
}

/* 
 * Description: Constructor for this class. Takes data to create the thing.
 *
 * Inputs: MapState* state - the state data to encapsalate by the thing
 *         QString name - the name of the thing, default to ""
 *         QString description - the description of the thing, default to ""
 *         int id - the ID of the thing, default to -1
 */
MapThing::MapThing(MapState* state, int width, int height, 
                   QString name, QString description, int id)
{
  this->state = 0;
  tile_main = 0;
  tile_previous = 0;
  clear();
  
  /* The class definitions */
  setDescription(description);
  setHeight(height);
  setID(id);
  setName(name);
  setState(state);
  setWidth(width);
}

/* 
 * Description: Destructor function 
 */
MapThing::~MapThing()
{
  clear();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Animates the frames of the thing, based on the animate offset
 *              of the thing as well as calls to the sprite holder
 * 
 * Inputs: short cycle_time - the msec time between the last animate call
 *         bool reset - Resets the animation back to head. Used for either 
 *                      restarting animation or stopping it.
 *         bool skip_head - Skip the head of the list of frames
 * Output: bool - a status on the animate, only fails if it tries to animate
 *                the sprite and there are no frames in it.
 */
bool MapThing::animate(short cycle_time, bool reset, bool skip_head)
{
  bool status = true;
  
  /* Check if an animation can occur */
  if(state != 0 && state->getSprite() != 0)
  {
    /* Increment the animation time */
    animation_buffer += cycle_time;
    
    if(reset || animation_time == 0)
    {
      status = state->getSprite()->setAtFirst();
      animation_buffer = 0;
    }
    else if(animation_buffer >= animation_time)
    {
      status = state->getSprite()->shiftNext(skip_head);
      animation_buffer -= animation_time;
    }
  }
  
  return status;
}

/* Is the thing almost centered on a tile (less than 1 pulse away) */
bool MapThing::isAlmostOnTile(float cycle_time)
{
  float x_diff = tile_main->getPixelX();
  float y_diff = tile_main->getPixelY();
  
  if(tile_main != 0)
  {
    /* X differential calculations to ensure positive number */
    if(x_diff > x)
      x_diff = x_diff - x;
    else
      x_diff = x - x_diff;
      
    /* Y differential calculations to ensure positive number */
    if(y_diff > y)
      y_diff = y_diff - y;
    else
      y_diff = y - y_diff;
    
    //float tile_diff = sqrt(x_diff*x_diff + y_diff*y_diff);
    //return (moveAmount(cycle_time) > tile_diff);
    
    return (moveAmount(cycle_time) > (x_diff + y_diff));
  }
  
  return false;
}

bool MapThing::isMoveAllowed(Tile* next_tile)
{
  EnumDb::Direction move_request = getMoveRequest();
  
  if(tile_main != 0 && next_tile != 0 && isMoveRequested())
  {
    return tile_main->getPassabilityExiting(move_request) && 
           next_tile->getPassabilityEntering(move_request);
  }
  return false;
}

float MapThing::moveAmount(float cycle_time)
{
  float move_amount = ((speed * cycle_time) / 1000.0);
  if(move_amount > width)
    move_amount = width;
  return move_amount;
}

void MapThing::moveThing(float cycle_time)
{
  float move_amount = moveAmount(cycle_time);
  
  if(movement == EnumDb::EAST)
    x += move_amount;
  else if(movement == EnumDb::WEST)
    x -= move_amount;
  else if(movement == EnumDb::SOUTH)
    y += move_amount;
  else if(movement == EnumDb::NORTH)
    y -= move_amount;
}

/* Sets the new direction that the class is moving in */
bool MapThing::setDirection(EnumDb::Direction new_direction)
{
  bool changed = false;

  /* Check if the direction is changed */
  if(movement != new_direction)
    changed = true;

  /* Update the direction */
  movement = new_direction;

  /* Only shift the location and animation if the direction changed */
  if(changed)
    animation_buffer = animation_time;
  
  return changed;
}

void MapThing::tileMoveFinish()
{
  if(tile_previous != 0)
    tile_previous->unsetImpassableThing();
  tile_previous = 0;
}

bool MapThing::tileMoveStart(Tile* next_tile, Tile::ThingState classification)
{
  if(next_tile != 0)
  {
    tile_previous = tile_main;
    tile_main = next_tile;
    tile_main->setImpassableThing(this, classification);
    return true;
  }
  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Clears out all information stored in the class
 * 
 * Inputs: none
 * Output: none
 */
void MapThing::clear()
{
  Tile* null_tile = 0;
  setStartingTile(null_tile);
  
  /* Resets the class parameters */
  setAnimationSpeed(kDEFAULT_ANIMATION);
  setDescription("");
  setID(kUNSET_ID);
  setName("");
  setSpeed(kDEFAULT_SPEED);
  
  height = 0;
  width = 0;

  unsetState();
}

short MapThing::getAnimationSpeed()
{
  return animation_time;
}

/* 
 * Description: Gets the things description.
 * 
 * Inputs: none
 * Output: QString - the description for the thing, as a QString
 */
QString MapThing::getDescription()
{
  return description;
}

/* 
 * Description: Gets the height of the internal tile.
 *
 * Inputs: none
 * Output: int - the height, in pixels
 */
int MapThing::getHeight()
{
  return height;
}

/* 
 * Description: Gets the things ID. If the ID is not set, it will be an
 *              out of range number (-1)
 *
 * Inputs: none
 * Output: int - the ID for the thing, as an integer
 */
int MapThing::getID()
{
  return id;
}

EnumDb::Direction MapThing::getMovement()
{
  return movement;
}

EnumDb::Direction MapThing::getMoveRequest()
{
  return EnumDb::DIRECTIONLESS;
}

/* 
 * Description: Gets the things name.
 *
 * Inputs: none
 * Output: QString - the name for the thing, as a QString
 */
QString MapThing::getName()
{
  return name;
}

short MapThing::getSpeed()
{
  return speed;
}

/* 
 * Description: Gets the state data of the thing. If state isn't set, returns
 *              0.
 *
 * Inputs: none
 * Output: MapState* - the pointer to the data for the thing, as a MapState
 */
MapState* MapThing::getState()
{
  return state;
}

/* Returns the central tile */
Tile* MapThing::getTile()
{
  return tile_main;
}

/* 
 * Description: Gets the width of the internal tile.
 *
 * Inputs: none
 * Output: int - the width, in pixels
 */
int MapThing::getWidth()
{
  return width;
}

float MapThing::getX()
{
  return x;
}

float MapThing::getY()
{
  return y;
}

/* 
 * Description: Starts interaction. In map thing, this isn't of use and is
 *              only used by its children classes.
 *
 * Inputs: none
 * Output: none
 */
void MapThing::interaction()
{
}

bool MapThing::isMoveRequested()
{
  return false;
}

bool MapThing::isMoving()
{
  return (movement != EnumDb::DIRECTIONLESS);
}

bool MapThing::isOnTile()
{
  return (((int)x % getWidth() == 0) && ((int)y % getHeight() == 0));
}

bool MapThing::paintGl(float offset_x, float offset_y, float opacity)
{
  if(state != 0 && state->getSprite() != 0 && tile_main != 0)
  {
    state->getSprite()->paintGl(x - offset_x, y - offset_y, 
                                width, height, opacity);
    return true;
  }
  return false;
}

bool MapThing::setAnimationSpeed(short frame_time)
{
  if(frame_time >= 0)
  {
    animation_buffer = 0;
    animation_time = frame_time;
    return true;
  }
  return false;
}

/* 
 * Description: Sets the description that defines the thing.
 *
 * Inputs: QString new_description - the data for the new description
 * Output: none
 */
void MapThing::setDescription(QString new_description)
{
  description = new_description;
}

/*
 * Description: Sets the new tile height for the thing. This number must
 *              be greater than 0 and if it's not, the call will fail.
 *
 * Inputs: int new_height - the new height of the thing, in pixels
 * Output: bool - status if the height change was successful
 */
bool MapThing::setHeight(int new_height)
{
  if(new_height > 0)
  {
    height = new_height;
    return true;
  }

  return false;
}

/*
 * Description: Sets the ID for the thing. If out of the allowable range, 
 *              the value is set to an minimum storage value. (Out of range)
 *
 * Inputs: int new_id - the new integer ID to define the thing
 * Output: bool - status if the new_id is within the allowable range.
 */
bool MapThing::setID(int new_id)
{
  /* If the ID is out of range */
  if(new_id < kMINIMUM_ID)
  {
    id = kUNSET_ID;
    return false;
  }

  /* Otherwise, the ID is good */
  id = new_id;
  return true;
}

/*
 * Description: Sets the name for the thing.
 *
 * Inputs: QString new_name - the new name for the thing
 * Output: none
 */
void MapThing::setName(QString new_name)
{
  name = new_name;
}

bool MapThing::setSpeed(short speed)
{
  if(speed >= 0)
  {
    this->speed = speed;
    return true;
  }
  
  return false;
}

/* 
 * Description: Sets the connected tile information for the map thing. This is
 *              the initial starting point and where the thing is initially
 *              placed. If this is unset, the thing will not move or paint.
 *
 * Inputs: Tile* new_tile
 * Output: none
 */
void MapThing::setStartingTile(Tile* new_tile)
{
  /* Stop movement */
  setDirection(EnumDb::DIRECTIONLESS);
  
  /* Unset the previous tile */
  if(tile_previous != 0)
    tile_previous->unsetImpassableThing();
  tile_previous = 0;
  
  /* Unset the main tile */
  if(tile_main != 0)
    tile_main->unsetImpassableThing();
  tile_main = 0;
  
  /* Set the new tile */
  tile_main = new_tile;
  if(tile_main != 0)
  {
    this->x = tile_main->getPixelX();
    this->y = tile_main->getPixelY();
    tile_main->setImpassableThing(this, Tile::PERSON);
  }
  else
  {
    this->x = 0;
    this->y = 0;
  }
}

/*
 * Description: Sets the state data that defines the thing.
 *
 * Inputs: MapState* state - the new state to define to insert into the Map
 *           Thing. Must actually have a sprite set in order to work.
 *         bool unset_old - delete the old state from memory?
 * Output: bool - returns if the thing state was set successfuly
 */
bool MapThing::setState(MapState* state, bool unset_old)
{
  /* Check if the state is valid */
  if(state != 0 && state->getSprite() != 0)
  {
    unsetState(unset_old);
    this->state = state;
    return true;
  }

  return false;
}

/*
 * Description: Sets the new tile width for the thing. This number must
 *              be greater than 0 and if it's not, the call will fail.
 *
 * Inputs: int new_width - the new width of the thing, in pixels
 * Output: bool - status if the height change was successful
 */
bool MapThing::setWidth(int new_width)
{
  if(new_width > 0)
  {
    width = new_width;
    return true;
  }

  return false;
}

/*
 * Description: Updates the state of the thing. This can include animation
 *              sequencing or movement and such. Called on the tick.
 *
 * Inputs: none
 * Output: none 
 */
void MapThing::updateThing(float cycle_time, Tile* next_tile)
{
  (void)next_tile;
  
  if(tile_main != 0)
  {
    /* Move the thing */
    moveThing(cycle_time);

    /* Animate the thing */
    animate(cycle_time);
  }
}

/*
 * Description: Unsets the state that's embedded in this as the Map Thing
 *
 * Inputs: bool delete_state - should the old state be deleted?
 * Output: none 
 */
void MapThing::unsetState(bool delete_state)
{
  if(delete_state)
    delete state;
  state = 0;
}
