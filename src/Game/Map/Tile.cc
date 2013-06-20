/******************************************************************************
* Class Name: Tile
* Date Created: December 2, 2012
* Inheritance: QObject
* Description: This class handles the basic tile that is set up on the map.
*              It is the overall structure. The tile class creates a base, 
*              enhancer, lower, upper, passable, and impassable to define
*              all the possibilities on the tile. This also handles printing
*              its own tile data and ensuring that movement isn't entered 
*              through the tile. For additional information, read the comments
*              below for each function.
******************************************************************************/
#include "Game/Map/Tile.h"

/* Constant Implementation - see header file for descriptions */
const int Tile::kLOWER_DEPTH = 0;
const int Tile::kUPPER_DEPTH = 3;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Set up the tile class to an empty 
 *              null state. In this state, you will have to manually set
 *              the width, height, x, and y coordinates of the tile and then
 *              enable the tile using setStatus()
 *
 * Input: none
 */
Tile::Tile()
{
  /* Clear out the logic for parts of the tile */
  impassable_set = UNSET;
  lower = new Layer();
  passable_set = false;
  upper = new Layer();

  /* Tile definitions */
  height = 0;
  width = 0;
  x = 0;
  y = 0;

  /* Set the initial passibility as completely open but OFF */
  setPassibility(true);
}

/* 
 * Description: Constructor for this class. Takes information on the tile 
 *              such as size, location and the parent and sets up the given
 *              tile. This will be framed by an upper class that handles 
 *              overall control (Map).
 *
 * Inputs: int width - the width of the tile in pixels
 *         int height - the height of the tile in pixels
 *         int x - the x location respective to the parent (in pixels)
 *         int y - the y location respective to the parent (in pixels)
 *         QObject* parent - the parent object to be set in the tile
 */
Tile::Tile(int width, int height, int x, int y)
{
  /* Clear out the logic for parts of the tile */
  impassable_set = UNSET;
  lower = new Layer(width, height, x, y, kLOWER_DEPTH);
  passable_set = false;
  upper = new Layer(width, height, x, y, kUPPER_DEPTH);

  /* Tile definitions */
  this->height = height;
  this->width = width;
  this->x = x;
  this->y = y;

  /* Set the initial passibility as completely open and ACTIVE */
  setPassibility(true);
}

/* 
 * Description: Destructor function
 */
Tile::~Tile()
{
  unsetBase();
  unsetEnhancer();
  unsetLower();
  unsetUpper();

  /* Delete the layers from memory */
  delete lower;
  lower = 0;
  delete upper;
  upper = 0;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Animates all sprite sets on the tile. This allows for the fine 
 *              control over when the timer hits and how it's updated.
 *
 * Inputs: none
 * Output: none
 */
void Tile::animate()
{
  /* Animate the two layers stored within this class */
  lower->animate();
  upper->animate();
}

/* 
 * Description: Appends the new Sprite frame data onto the created lower
 *              layer for this tile. Passes implementation into Layer.
 *
 * Inputs: Sprite* lower - the lower tile data, as a Sprite set
 * Output: bool - if the appending call succeeded and the data was inserted
 */
bool Tile::appendLower(Sprite* lower)
{
  return this->lower->appendLower(lower);
}

/* 
 * Description: Appends the new Sprite frame data onto the created upper
 *              layer for this tile. Passes implementation into Layer.
 *
 * Inputs: Sprite* upper - the upper tile data, as a Sprite set
 * Output: bool - if the appending call succeeded and the data was inserted
 */
bool Tile::appendUpper(Sprite* upper)
{
  return this->upper->appendUpper(upper);
}

/* 
 * Description: Gets the base sprite sequence and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the base sprite pointer
 */
Sprite* Tile::getBase()
{
  return lower->getBase();
}

/* 
 * Description: Gets the enhancer sprite sequence and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the enhancer sprite pointer
 */
Sprite* Tile::getEnhancer()
{
  return lower->getEnhancer();
}

/* 
 * Description: Gets the height stored within the tile (and all the layers).
 *
 * Inputs: none
 * Output: int - the height in pixels
 */
int Tile::getHeight()
{
  return height;
}

/* TODO: [2013-06-19]
 * Description: Gets the impassable object sprite that is stored, if set
 *
 * Inputs: none
 * Output: MapThing* - the MapThing pointer, could be a person or object
 */
MapThing* Tile::getImpassableObject()
{
  if(impassable_set == DECOR)
    return impassable_object;
  else if(impassable_set == PERSON)
    return person;
  return NULL;
}

/* 
 * Description: Gets the lower sprite(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: QList<Sprite*> - the lower sprites, in a QList
 */
QList<Sprite*> Tile::getLower()
{
  return lower->getLower();
}

/* TODO: [2013-06-19] 
 * Description: Gets the passable object sprite and returns it, if set.
 *
 * Inputs: none
 * Output: MapThing* - the passable object MapThing pointer
 */
MapThing* Tile::getPassableObject()
{
  if(passable_set)
    return passable_object;
  return NULL;
}

/* TODO: [2013-06-19]
 * Description: Gets if the tile is passable from the East
 *
 * Inputs: none
 * Output: bool - status if the tile is accessable from the east
 */
bool Tile::getPassibilityEast()
{
  return east_passibility && !impassable_set;
}

/* TODO: [2013-06-19]
 * Description: Gets if the tile is passable from the North
 *
 * Inputs: none
 * Output: bool - status if the tile is accessable from the north
 */
bool Tile::getPassibilityNorth()
{
  return north_passibility;
}

/* TODO: [2013-06-19]
 * Description: Gets if the tile is passable from the South
 *
 * Inputs: none
 * Output: bool - status if the tile is accessable from the south
 */
bool Tile::getPassibilitySouth()
{
  return south_passibility;
}

/* TODO: [2013-06-19]
 * Description: Gets if the tile is passable from the West
 *
 * Inputs: none
 * Output: bool - status if the tile is accessable from the west
 */
bool Tile::getPassibilityWest()
{
  return west_passibility;
}

/* 
 * Description: Returns the status the tile is currently classified in. Uses
 *              the enum from Layer. See the corresponding setStatus()
 *
 * Inputs: none
 * Output: Status - public enum from layer identifying state
 */
Layer::Status Tile::getStatus()
{
  return lower->getStatus();
}

/* 
 * Description: Gets the upper sprite(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: QList<Sprite*> - the upper sprites, in a QList
 */
QList<Sprite*> Tile::getUpper()
{
  return upper->getUpper();
}

/* 
 * Description: Gets the width stored within the tile (and all the layers).
 *
 * Inputs: none
 * Output: int - the width in pixels
 */
int Tile::getWidth()
{
  return width;
}

/* 
 * Description: Gets the X coordinate stored within the tile (and all the 
 *              layers).
 *
 * Inputs: none
 * Output: int - the x coordinate in pixels
 */
int Tile::getX()
{
  return x;
}

/* 
 * Description: Gets the Y coordinate stored within the tile (and all the 
 *              layers).
 *
 * Inputs: none
 * Output: int - the y coordinate in pixels
 */
int Tile::getY()
{
  return y;
}

/* 
 * Description: Inserts the inner layers (QGraphicsItems) into the scene
 *              that have been created by the class.
 *
 * Inputs: QGraphicsScene* scene - the scene to insert the items
 * Output: bool - returns if the insertion was successful and executed.
 *                Only fails if something went wrong internally in the class
 *                and the appropriate pointers aren't set.
 */
bool Tile::insertIntoScene(QGraphicsScene* scene)
{
  if(lower != 0 && upper != 0)
  {
    scene->addItem(lower);
    scene->addItem(upper);
    return true;
  }
  return false;
}

/* 
 * Description: Returns if the Base Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the base sprite is set
 */
bool Tile::isBaseSet()
{
  return (lower->getBase() != 0);
}

/* 
 * Description: Returns if the Enhancer Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the enhancer sprite is set
 */
bool Tile::isEnhancerSet()
{
  return (lower->getEnhancer() != 0);
}

/* TODO: [2013-06-19]
 * Description: Returns if the Impassable Sprite (object or person) is set 
 *
 * Inputs: none
 * Output: ImpassableObjectState - Enumerator that handles the impassable 
 *                                 object state: UNSET, OBJECT, PERSON.
 */
Tile::ImpassableObjectState Tile::isImpassableObjectSet()
{
  return impassable_set;
}

/* 
 * Description: Returns if the Lower Sprite(s) is(are) set 
 *
 * Inputs: none
 * Output: bool - status if there is at least one lower sprite
 */
bool Tile::isLowerSet()
{
  return (lower->getLower().size() > 0);
}

/* TODO: [2013-06-19]
 * Description: Returns if the Passable Sprite(s) is(are) set 
 *
 * Inputs: none
 * Output: bool - status if the passable sprite(s) is(are) set
 */
bool Tile::isPassableObjectSet()
{
  return passable_set;
}

/* 
 * Description: Returns if the Upper Sprite(s) is(are) set 
 *
 * Inputs: none
 * Output: bool - status if there is at least one upper sprite
 */
bool Tile::isUpperSet()
{
  return (upper->getUpper().size() > 0);
}

/* 
 * Description: Removes the layers in this class from the scene.
 *              Typically called before deconstruction.
 *
 * Inputs: QGraphicsScene* scene - the scene to remove the items from
 * Output: none
 */
void Tile::removeFromScene(QGraphicsScene* scene)
{
  if(lower != 0)
    scene->removeItem(lower);

  if(upper != 0)
    scene->removeItem(upper);
}

/*
 * Description: Sets the base part of the layer, in the lower half. This class 
 *              only takes the pointer for usage and does not manage deleting.
 *              Once the class is finished, deletion of the pointer is still
 *              required.
 *
 * Inputs: Sprite* base - the sprite that defines the base part
 * Output: bool - returns true if the base was set successfully.
 */
bool Tile::setBase(Sprite* base)
{
  return lower->setBase(base);
}

/*
 * Description: Sets the enhancer part of the layer, in the lower half. This 
 *              class only takes the pointer for usage and does not manage 
 *              deleting. Once the class is finished, deletion of the pointer 
 *              is still required.
 *
 * Inputs: Sprite* enhancer - the sprite that defines the enhancer part
 * Output: bool - returns true if the enhancer was set successfully.
 */
bool Tile::setEnhancer(Sprite* enhancer)
{
  return lower->setEnhancer(enhancer);
}

/*
 * Description: Sets the height of the tile. This will call each layer that has
 *              already been created and update the height in them as well.
 *
 * Inputs: int height - the tile height, in pixels
 * Output: bool - returns if the height set was successful (and the height was
 *                a positive number)
 */
bool Tile::setHeight(int height)
{
  if(height > 0)
  {
    this->height = height;

    /* Set it on all the layers */
    lower->setHeight(height);
    upper->setHeight(height);

    return true;
  }
  return false;
}

/* TODO: [2013-06-19]
 * Description: Sets the impassable object sprite in the tile using a path to 
 *              the sprite image file.
 *
 * Inputs: QString path - the path to the image to load in
 * Output: bool - returns true if the impassable object sprite was successfuly 
 *                set.
 */
bool Tile::setImpassableObject(QString path, ImpassableObjectState type)
{
  // TODO
    (void)path;//warning
    (void)type; //warning
  return true;
}

/*
 * Description: Sets the lower part of the layer, in the lower half. This 
 *              class only takes the pointer for usage and does not manage 
 *              deleting. Once the class is finished, deletion of the pointer 
 *              is still required.
 *
 * Inputs: Sprite* lower - the sprite that defines the lower part
 * Output: bool - returns true if the lower was set successfully.
 */
bool Tile::setLower(Sprite* lower)
{
  return this->lower->setLower(lower);
}

/* TODO: [2013-06-19]
 * Description: Sets the passable object sprite in the tile using a path to 
 *              the sprite image file.
 *
 * Inputs: QString path - the path to the image to load in
 * Output: bool - returns true if the passable object sprite was successfuly set
 */
bool Tile::setPassableObject(QString path)
{
  // TODO
  (void)path;//warning
  return true;
}

/* TODO: [2013-06-19]
 * Description: Sets into the tile from all directions
 *
 * Inputs: bool is_passable - is the tile passable from all directions
 * Output: none
 */
void Tile::setPassibility(bool is_passable)
{
  north_passibility = is_passable;
  east_passibility = is_passable;
  south_passibility = is_passable;
  west_passibility = is_passable;
}

/* TODO: [2013-06-19]
 * Description: Sets the passibility into the tile from all directions.
 * 
 * Inputs: bool north_is_passable - is the tile passable from the north
 *         bool east_is_passable - is the tile passable from the east
 *         bool south_is_passable - is the tile passable from the south
 *         bool west_is_passable - is the tile passable from the west
 * Output: none
 */
void Tile::setPassibility(bool north_is_passable, bool east_is_passable,
                          bool south_is_passable, bool west_is_passable)
{
  north_passibility = north_is_passable;
  east_passibility = east_is_passable;
  south_passibility = south_is_passable;
  west_passibility = west_is_passable;
}

/* TODO: [2013-06-19]
 * Description: Sets if the tile is passable from the East
 * 
 * Inputs: bool is_passable - is the tile passable
 * Output: none
 */
void Tile::setPassibilityEast(bool is_passable)
{
  east_passibility = is_passable;
}

/* TODO: [2013-06-19]
 * Description: Sets if the tile is passable from the North
 * 
 * Inputs: bool is_passable - is the tile passable
 * Output: none
 */
void Tile::setPassibilityNorth(bool is_passable)
{
  north_passibility = is_passable;
}

/* TODO: [2013-06-19]
 * Description: Sets if the tile is passable from the South
 * 
 * Inputs: bool is_passable - is the tile passable
 * Output: none
 */
void Tile::setPassibilitySouth(bool is_passable)
{
  south_passibility = is_passable;
}

/* TODO: [2013-06-19]
 * Description: Sets if the tile is passable from the West
 * 
 * Inputs: bool is_passable - is the tile passable
 * Output: none
 */
void Tile::setPassibilityWest(bool is_passable)
{
  west_passibility = is_passable;
}

/* 
 * Description: Sets the tile status, which then sends this off to all the
 *              applicable layers in the Tile utilizing the enum from Layer.
 *              This allows of 3 possible states that the tile can be in. This 
 *              affects the visibility and painting of the tile.
 * 
 * Inputs: Status updated_status - the new status to update the tile to
 * Output: none
 */
void Tile::setStatus(Layer::Status updated_status)
{
  lower->setStatus(updated_status);
  upper->setStatus(updated_status);
}

/*
 * Description: Sets the upper part of the layer, in the upper half. This 
 *              class only takes the pointer for usage and does not manage 
 *              deleting. Once the class is finished, deletion of the pointer 
 *              is still required.
 *
 * Inputs: Sprite* upper - the sprite that defines the upper part
 * Output: bool - returns true if the upper was set successfully.
 */
bool Tile::setUpper(Sprite* upper)
{
  return this->upper->setUpper(upper);
}

/*
 * Description: Sets the width of the tile. This will call each layer that has
 *              already been created and update the width in them as well.
 *
 * Inputs: int width - the tile width, in pixels
 * Output: bool - returns if the width set was successful (and the width was
 *                a positive number)
 */
bool Tile::setWidth(int width)
{
  if(width > 0)
  {
    this->width = width;

    /* Set it on all the layers */
    lower->setWidth(width);
    upper->setWidth(width);

    return true;
  }
  return false;
}

/*
 * Description: Sets the X coordinate of the tile. This will call each layer 
 *              that has already been created and update the X coordinate in 
 *              them as well.
 *
 * Inputs: int x - the tile x coordinate, in pixels
 * Output: none
 */
void Tile::setX(int x)
{
  this->x = x;

  /* Set it on all the layers */
  lower->setX(x);
  upper->setX(x);
}

/*
 * Description: Sets the Y coordinate of the tile. This will call each layer 
 *              that has already been created and update the Y coordinate in 
 *              them as well.
 *
 * Inputs: int y - the tile y coordinate, in pixels
 * Output: none
 */
void Tile::setY(int y)
{
  this->y = y;

  /* Set it on all the layers */
  lower->setY(y);
  upper->setY(y);
}

/* 
 * Description: Unsets the base sprite in the tile.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetBase()
{
  lower->unsetBase();
}

/* 
 * Description: Unsets the enhancer sprite in the tile. 
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetEnhancer()
{
  lower->unsetEnhancer();
}

/* TODO: [2013-06-19]
 * Description: Unsets the impassable object sprite in the tile. Deletes the 
 *              pointer, if applicable, and sets the internal variable to 
 *              notify the class so the impassable object isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the impassable object was set before being
 *                unset.
 */
bool Tile::unsetImpassableObject()
{
  // TODO
  return false;
}

/* 
 * Description: Unsets the lower sprite(s) in the tile. 
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetLower()
{
  lower->unsetLower();
}

/* TODO: [2013-06-19]
 * Description: Unsets the passable object sprite in the tile. Deletes the
 *              pointer, if applicable, and sets the internal variable to
 *              notify the class so the passable object isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the passable object was set before being unset
 */
bool Tile::unsetPassableObject()
{
  // TODO
  return false;
}

/* 
 * Description: Unsets the upper sprite(s) in the tile. 
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetUpper()
{
  upper->unsetUpper();
}
