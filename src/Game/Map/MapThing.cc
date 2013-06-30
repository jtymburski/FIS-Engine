/******************************************************************************
 * Class Name: MapThing
 * Date Created: Dec 2 2012
 * Inheritance: QGraphicsObject
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapWalkOver, MapSolid, etc. Also handles the basic setup for 
 *              name, description, id, sprite. No interaction is handled in 
 *              this class since its a generic parent.
 *****************************************************************************/
#include "Game/Map/MapThing.h"

/* Constant Implementation - see header file for descriptions */
const int MapThing::kANIMATION_OFFSET = 12;
const int MapThing::kMINIMUM_ID =  0;
const int MapThing::kTHING_INCREMENT = 4;
const int MapThing::kUNSET_ID   = -1;

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
  animation_delay = 0;
  state = 0;
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
  animation_delay = 0;
  this->state = 0;

  /* The parent class definitions */
  setHeight(height);
  setWidth(width);

  /* The class definitions */
  setDescription(description);
  setID(id);
  setName(name);
  setState(state);
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
 * Description: Animates the frames of the thing, based on the update offset
 *              of the thing as well as calls to the sprite holder
 * 
 * Inputs: bool skip_head - Skip the head of the list of frames
 * Output: bool - a status on the animate, only fails if it tries to animate
 *                the sprite and there are no frames in it.
 */
bool MapThing::animate(bool skip_head)
{
  if(state != 0 && state->getSprite() != 0 && 
     animation_delay == kANIMATION_OFFSET)
  {
    bool status = state->getSprite()->shiftNext(skip_head);
    animation_delay = 0;
    return status;
  }

  animation_delay++;
  return true;
}

void MapThing::moveThing()
{
  if(movement == EnumDb::EAST)
    setX(x() + kTHING_INCREMENT);
  else if(movement == EnumDb::WEST)
    setX(x() - kTHING_INCREMENT);
  else if(movement == EnumDb::SOUTH)
    setY(y() + kTHING_INCREMENT);
  else if(movement == EnumDb::NORTH)
    setY(y() - kTHING_INCREMENT);
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

  /* Only shift the animation if the direction changed */
  if(changed)
    animation_delay = kANIMATION_OFFSET;

  return changed;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Implemented virtual function. Returns the bounding rectangle
 *              around the image that is being added into the scene. 
 *
 * Inputs: none
 * Output: QRectF - the rectangle around the internal image
 */
QRectF MapThing::boundingRect() const
{
  QRectF rect(0, 0, width, height);

  return rect;
}

/* 
 * Description: Clears out all information stored in the class
 * 
 * Inputs: none
 * Output: none
 */
void MapThing::clear()
{
  setDescription("");
  setID(kUNSET_ID);
  setName("");

  height = 0;
  width = 0;

  unsetState();
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
  return ((int)x() % getWidth() == 0 && (int)y() % getHeight() == 0);
}

/* 
 * Description: Reimplemented virtual function. Handles the painting of the 
 *              image stored within the layer. Runs based on the same data as 
 *              the above QRectF for the bounding box. Only paints if the tile 
 *              is set.
 *
 * Inputs: QPainter* painter - the painter to send all the image data to
 *         QStyleOptionGraphicsItem* option - any painting options.
 *         QWidget* widget - the related widget, if applicable
 * Output: none
 */
void MapThing::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
                  QWidget* widget)
{
  /* Remove unused parameter warnings */
  (void)option;
  (void)widget;

  /* Set painter information */
  //painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

  /* Only paint if the sprite exists */
  if(state != 0 && state->getSprite() != 0)
    painter->drawPixmap(0, 0, width, height, state->getSprite()->getCurrent());
}

/* 
 * Description: Resets the sequence of animation to the head of the list.
 *              The head is used as the inanimate place holder in most cases.
 *
 * Inputs: none
 * Output: bool - status if the reset was successful. Only fails if there are
 *                no frames in the sprite.
 */
bool MapThing::resetAnimation()
{
  if(state != 0 && state->getSprite() != 0)
  {
    if(!state->getSprite()->isAtFirst())
      return state->getSprite()->setAtFirst();
    return true;
  }
  return false;
}

/* 
 * Description: Sets the coordinate information for the base item (inherited).
 *              X and Y are with respect to the scene plane. Z is the vertical
 *              plane for how items are layered.
 *
 * Inputs: int x - the x pixel coordinate on the scene
 *         int y - the y pixel coordinate on the scene
 *         int z - the z rating on the screen (default to 0).
 * Output: none
 */
void MapThing::setCoordinates(int x, int y, int z)
{
  setX(x);
  setY(y);
  setZValue(z);
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
    prepareGeometryChange();
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
    prepareGeometryChange();
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
void MapThing::updateThing(bool can_move)
{
  if(can_move)
    moveThing();

  if(movement != EnumDb::DIRECTIONLESS)
    animate();
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
