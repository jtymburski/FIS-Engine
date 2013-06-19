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
const int Tile::kBASE_COUNT_MAX = 5;
const int Tile::kBASE_DEPTH = 1;
const int Tile::kDATA_ELEMENT = 0;
const int Tile::kDATA_ROTATION = 1;
const int Tile::kENHANCER_DEPTH = 6;
const int Tile::kLOWER_DEPTH = 7;
const int Tile::kMAP_INTERACTIVE_DEPTH = 8;
const int Tile::kMAP_PERSON_DEPTH = 9;
const int Tile::kNE_ENHANCER     = 1;
const int Tile::kNW_ENHANCER     = 0;
const int Tile::kSE_ENHANCER     = 3;
const int Tile::kSW_ENHANCER     = 2;
const int Tile::kUPPER_COUNT_MAX = 5;
const int Tile::kUPPER_DEPTH = 10;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor for this class. Set up the tile class to an empty 
 *              null state.
 *
 * Input: none
 */
Tile::Tile()
{
  /* Clear out the logic for parts of the tile */
  enhancer = 0;
  lower = 0;
  impassable_set = UNSET; 
  passable_set = false;

  /* Tile definitions */
  setHeight(0);
  setWidth(0);
  setX(0);
  setY(0);

  /* Set the initial passibility as completely open but OFF */
  setPassibility(true);
  setStatus(OFF);
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
Tile::Tile(int width, int height, int x, int y, QObject* parent)
{
  setParent(parent);
  
  /* Clear out the logic for parts of the tile */
  enhancer = 0;
  lower = 0;
  impassable_set = UNSET; 
  passable_set = false;

  /* Tile definitions */
  setHeight(height);
  setWidth(width);
  setX(x);
  setY(y);

  /* Set the initial passibility as completely open and ACTIVE */
  setPassibility(true);
  setStatus(ACTIVE);
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
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Returns the angle that connects the RotatedAngle enumerator
 *              to the associated degrees to allow the rotation to occur
 * 
 * Inputs: RotatedAngle angle - the angle to determine the degree association
 * Output: int - the degree integer count
 */
int Tile::getAngle(RotatedAngle angle)
{
  if(angle == CLOCKWISE)
    return 90;
  else if(angle == COUNTERCLOCKWISE)
    return -90;
  else if(angle == FLIP)
    return 180;

  return 0;
}

/* 
 * Description: Sets if the layers are enabled, across them all.
 * 
 * Inputs: bool enabled - new enabled status of the layers
 * Output: void
 */
void Tile::setEnabled(bool enabled)
{
  for(int i = 0; i < base.size(); i++)
    base[i]->setEnabled(enabled);

  if(isEnhancerSet())
    enhancer->setEnabled(enabled);

  if(isLowerSet())
    lower->setEnabled(enabled);

  for(int i = 0; i < upper.size(); i++)
    upper[i]->setEnabled(enabled);
}

/* 
 * Description: Sets if the layers are visible, across them all.
 * 
 * Inputs: bool visible - new visible status of the layers
 * Output: void
 */
void Tile::setVisible(bool visible)
{
  for(int i = 0; i < base.size(); i++)
    base[i]->setVisible(visible);

  if(isEnhancerSet())
    enhancer->setVisible(visible);

  if(isLowerSet())
    lower->setVisible(visible);

  for(int i = 0; i < upper.size(); i++)
    upper[i]->setVisible(visible);

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Adds a new base layer to the stack. Only fails if the stack
 *              already has 5 layers (max) or the sprite size is 0.
 *
 * Inputs: Sprite* base_sprite - the new base sprite to attempt to load in
 *         RotateAngle angle - the angle classification to rotate the tile
 * Output: Layer* - returns the layer that was set for the base
 *                  If pointer is 0, call failed.
 */
Layer* Tile::addBase(Sprite* base_sprite, RotatedAngle angle)
{
  /* Determine if the base can be added to the stack */
//  if(base_sprite != 0 && base_sprite->getSize() > 0 
//                      && base.size() <= kBASE_COUNT_MAX)
//  {
    /* Add the base to the stack. Offset the Z component based on the
     * constant plus the array location so each base is on a different layer */
//    base_sprite->rotateAll(getAngle(angle));
//    base.append(new Layer(base_sprite, width, height, 
//                          x, y, kBASE_DEPTH + base.size()));

//    setStatus(tile_status);
//    emit addLayer(base[base.size() - 1]);
//    return base[base.size() - 1];
//  }

  return 0;
}

/* 
 * Description: Adds data to this tile, from a file data pointer plus
 *              applicable other data, as defined below. The added data is
 *              inserted into this tile only if it's legitimate data.
 *
 * Inputs: XmlData data - the data that is read from the file
 *         QPixmap image - image data, to speed up adding the info to the tile
 *         bool append - indication if the data should be appended onto 
 *                       existing sprite data or create new (default false)
 * Output: bool - returns if the addition was successful and completed
 */
bool Tile::addData(XmlData data, QPixmap image, bool append)
{
  QStringList path = data.getElement(data.getNumElements()-1).split("_");
  RotatedAngle angle = NONE;

  /* Determine the angle to be rotated, only if the rotation element exists */
  if(path.size() > kDATA_ROTATION)
  {
    if(path[kDATA_ROTATION] == "CW")
      angle = CLOCKWISE;
    else if(path[kDATA_ROTATION] == "CCW")
      angle = COUNTERCLOCKWISE;
    else if(path[kDATA_ROTATION] == "F")
      angle = FLIP;
  }

  /* Determine if this is a new set or an append call */
  if(!append)
  {
    /* Determines the type of tile, to be set to new */
    if(path[kDATA_ELEMENT] == "base")
      return (addBase(new Sprite(image), angle) != 0);
    else if(path[kDATA_ELEMENT] == "enhancer")
      return (setEnhancer(new Sprite(image), angle) != 0);
    else if(path[kDATA_ELEMENT] == "lower")
      return (setLower(new Sprite(image), angle) != 0);
    else if(path[kDATA_ELEMENT] == "upper")
      return (addUpper(new Sprite(image), angle) != 0);
    return false;
  }
  else
  {
    /* Determines the type of tile, to append */
    if(path[kDATA_ELEMENT] == "base")
      return appendBase(image, angle);
    else if(path[kDATA_ELEMENT] == "enhancer")
      return appendEnhancer(image, angle);
    else if(path[kDATA_ELEMENT] == "lower")
      return appendLower(image, angle);
    else if(path[kDATA_ELEMENT] == "upper")
      return appendUpper(image, angle);
    return false;
  }
}

/* 
 * Description: Adds a new upper layer to the stack. Only fails if the stack
 *              already has 5 layers (max) or the sprite size is 0.
 *
 * Inputs: Sprite* upper_sprite - the new upper sprite to attempt to load in
 *         RotateAngle angle - the angle classification to rotate the tile
 * Output: Layer* - returns the layer that was set for the upper
 *                  If pointer is 0, call failed.
 */
Layer* Tile::addUpper(Sprite* upper_sprite, RotatedAngle angle)
{
  /* Determine if the upper can be added to the stack */
//  if(upper_sprite != 0 && upper_sprite->getSize() > 0 
//                       && upper.size() <= kUPPER_COUNT_MAX)
//  {
    /* Add the upper to the stack. Offset the Z component based on the
     * constant plus the array location so each base is on a different layer */
//    upper_sprite->rotateAll(getAngle(angle));
//    upper.append(new Layer(upper_sprite, width, height, 
//                          x, y, kUPPER_DEPTH + upper.size()));
  
//    setStatus(tile_status);
//    emit addLayer(upper[upper.size() - 1]);
//    return upper[upper.size() - 1];
//  }

  return 0;
}

/* 
 * Description: Animates all sprite layers on tile. This allows for the fine 
 *              control over when the timer hits and how it's updated.
 *
 * Inputs: none
 * Output: none
 */
void Tile::animate()
{
  /* Shift the base layer(s) */
//  for(int i = 0; i < base.size(); i++)
//  {
//    base[i]->getItem()->shiftNext();
//    base[i]->update();
//  }

  /* Shift the enhancer layer */
//  if(isEnhancerSet())
//  {
//    enhancer->getItem()->shiftNext();
//    enhancer->update();
//  }

  /* Shift the lower layer */
//  if(isLowerSet())
//  {
//    lower->getItem()->shiftNext();
//    lower->update();
//  }

  /* Shift the upper layer(s) */
//  for(int i = 0; i < upper.size(); i++)
//  {
//    upper[i]->getItem()->shiftNext();
//    upper[i]->update();
//  }
}

/* 
 * Description: Appends the new QPixmap frame data onto the last created base
 *              layer for this tile. If there is no base to append to, it 
 *              creates a new base layer
 *
 * Inputs: QPixmap frame - the new image data to insert
 *         RotatedAngle angle - the angle to rotate the pixmap data
 * Output: bool - if the appending call succeeded and the data was inserted
 */
bool Tile::appendBase(QPixmap frame, RotatedAngle angle)
{
//  if(isBaseSet())
//  {
//    return base.last()->getItem()->insertTail(frame, getAngle(angle));
//  }

//  if(addBase(new Sprite(frame), angle) != 0)
//    return true;
  return false;
}

/* 
 * Description: Appends the new QPixmap frame data onto the created enhancer
 *              layer for this tile. If there is no enhancer to append to, it
 *              create a new enhancer layer.
 *
 * Inputs: QPixmap frame - the new image data to insert
 *         RotatedAngle angle - the angle to rotate the pixmap data
 * Output: bool - if the appending call succeeded and the data was inserted
 */
bool Tile::appendEnhancer(QPixmap frame, RotatedAngle angle)
{
//  if(isEnhancerSet())
//  {
//    return enhancer->getItem()->insertTail(frame, getAngle(angle));
//  }

//  if(setEnhancer(new Sprite(frame), angle) != 0)
//    return true;
  return false;
}

/* 
 * Description: Appends the new QPixmap frame data onto the created lower
 *              layer for this tile. If there is no lower to append to, it
 *              create a new lower layer.
 *
 * Inputs: QPixmap frame - the new image data to insert
 *         RotatedAngle angle - the angle to rotate the pixmap data
 * Output: bool - if the appending call succeeded and the data was inserted
 */
bool Tile::appendLower(QPixmap frame, RotatedAngle angle)
{
//  if(isLowerSet())
//  {
//    return lower->getItem()->insertTail(frame, getAngle(angle));
//  }

//  if(setLower(new Sprite(frame), angle) != 0)
//    return true;
  return false;
}

/* 
 * Description: Appends the new QPixmap frame data onto the last created upper
 *              layer for this tile. If there is no upper to append to, it 
 *              creates a new upper layer
 *
 * Inputs: QPixmap frame - the new image data to insert
 *         RotatedAngle angle - the angle to rotate the pixmap data
 * Output: bool - if the appending call succeeded and the data was inserted
 */
bool Tile::appendUpper(QPixmap frame, RotatedAngle angle)
{
//  if(isUpperSet())
//  {
//    return upper.last()->getItem()->insertTail(frame, getAngle(angle));
//  }

//  if(addUpper(new Sprite(frame), angle) != 0)
//    return true;
  return false;
}

/* 
 * Description: Gets the base layer(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: QVector<Layer*> - the QVector of up to 5 base layers allowed
 */
QVector<Layer*> Tile::getBase()
{
  return base;
}

/* 
 * Description: Gets the enhancer layer and returns it, if set.
 *
 * Inputs: none
 * Output: Layer* - the enhancer layer pointer
 */
Layer* Tile::getEnhancer()
{
  return enhancer;
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

/* 
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
 * Description: Gets the lower layer and returns it, if set.
 *
 * Inputs: none
 * Output: Layer* - the lower layer pointer
 */
Layer* Tile::getLower()             
{
  return lower;
}

/* 
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

/* 
 * Description: Gets if the tile is passable from the East
 *
 * Inputs: none
 * Output: bool - status if the tile is accessable from the east
 */
bool Tile::getPassibilityEast()
{
  return east_passibility && !impassable_set;
}

/* 
 * Description: Gets if the tile is passable from the North
 *
 * Inputs: none
 * Output: bool - status if the tile is accessable from the north
 */
bool Tile::getPassibilityNorth()
{
  return north_passibility;
}

/* 
 * Description: Gets if the tile is passable from the South
 *
 * Inputs: none
 * Output: bool - status if the tile is accessable from the south
 */
bool Tile::getPassibilitySouth()
{
  return south_passibility;
}

/* 
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
 * Description: Gets the upper layer(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: QVector<Layer*> - the upper layer(s) pointer in a QVector
 */
QVector<Layer*> Tile::getUpper()
{
  return upper;
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
 * Description: Returns if the Base Layer(s) is(are) set 
 *
 * Inputs: none
 * Output: bool - status if the base layer(s) is(are) set
 */
bool Tile::isBaseSet()
{
  return (base.size() > 0);
}

/* 
 * Description: Returns if the Enhancer Layer is set 
 *
 * Inputs: none
 * Output: bool - status if the enhancer layer is set
 */
bool Tile::isEnhancerSet()
{
  return (enhancer != 0);
}

/* 
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
 * Description: Returns if the Lower Layer is set 
 *
 * Inputs: none
 * Output: bool - status if the lower layer is set
 */
bool Tile::isLowerSet()
{
  return (lower != 0);
}

/* 
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
 * Description: Returns if the Upper Layer(s) is(are) set 
 *
 * Inputs: none
 * Output: bool - status if there is at least one upper layer
 */
bool Tile::isUpperSet()
{
  return (upper.size() > 0);
}



/*
 * Description: Sets the enhancer layer using a single sprite pointer
 *              to cover the tile.
 * Note: This will unset the enhancer automatically. 
 *
 * Inputs: Sprite* enhancer - the sprite to attempt to load in
 *         RotatedAngle angle - the angle classification to rotate the layer 
 * Output: bool - returns true if the enhancer was set successfully.
 */
Layer* Tile::setEnhancer(Sprite* enhancer_sprite, RotatedAngle angle)
{
  /* Set the enhancer, if the sprite is valid and has a size greater than 0 */
//  if(enhancer_sprite != 0 && enhancer_sprite->getSize() > 0)
//  {
//    enhancer_sprite->rotateAll(getAngle(angle));
//    unsetEnhancer();
//    enhancer = new Layer(enhancer_sprite, width, height, 
//                         x, y, kENHANCER_DEPTH);

//    setStatus(tile_status);
//    emit addLayer(enhancer);
//    return enhancer;
//  }

  return 0;
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
  if(height >= 0)
  {
    this->height = height;

    /* Set it on all the layers */
    for(int i = 0; i < base.size(); i++)
      base[i]->setHeight(height);
    if(isEnhancerSet())
      enhancer->setHeight(height); 
    if(isLowerSet())
      lower->setHeight(height);
    for(int i = 0; i < upper.size(); i++)
      upper[i]->setHeight(height);

    return true;
  }
  return false;
}

/* 
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
 * Description: Sets the lower layer in the tile using the sprite 
 *              image data.
 *
 * Inputs: Sprite* lower_sprite - the lower sprite to attempt to load in
 *         RotatedAngle angle - the angle classification to rotate the layer
 * Output: bool - returns true if the lower layer was successfuly set
 */
Layer* Tile::setLower(Sprite* lower_sprite, RotatedAngle angle)
{
  /* Set the lower layer, if the sprite is valid and has a size greater 
   * than 0 */
//  if(lower_sprite != 0 && lower_sprite->getSize() > 0)
//  {
//    lower_sprite->rotateAll(getAngle(angle));
//    unsetLower();
//    lower = new Layer(lower_sprite, width, height, 
//                      x, y, kLOWER_DEPTH);

//    setStatus(tile_status);
//    emit addLayer(lower);
//    return lower;
//  }

  return 0;
}

/* 
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

/* 
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

/* 
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

/* 
 * Description: Sets if the tile is passable from the East
 * 
 * Inputs: bool is_passable - is the tile passable
 * Output: none
 */
void Tile::setPassibilityEast(bool is_passable)
{
  east_passibility = is_passable;
}

/* 
 * Description: Sets if the tile is passable from the North
 * 
 * Inputs: bool is_passable - is the tile passable
 * Output: none
 */
void Tile::setPassibilityNorth(bool is_passable)
{
  north_passibility = is_passable;
}

/* 
 * Description: Sets if the tile is passable from the South
 * 
 * Inputs: bool is_passable - is the tile passable
 * Output: none
 */
void Tile::setPassibilitySouth(bool is_passable)
{
  south_passibility = is_passable;
}

/* 
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
 * Description: Sets the tile status, which allows of 3 possible states that
 *              the tile can be in. This affects the visibility and painting
 *              of the tile.
 * 
 * Inputs: Status updated_status - the new status to update the tile to
 * Output: none
 */
void Tile::setStatus(Status updated_status)
{
  /* The various cases for the different statuses */
  if(updated_status == OFF)
  {
    setEnabled(false);
  }
  else if(updated_status == ACTIVE)
  {
    setEnabled(true);
    setVisible(true);
  }
  else if(updated_status == INACTIVE)
  {
    setEnabled(true);
    setVisible(false);
  }

  tile_status = updated_status;
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
  if(width >= 0)
  {
    this->width = width;

    /* Set it on all the layers */
    for(int i = 0; i < base.size(); i++)
      base[i]->setWidth(width);
    if(isEnhancerSet())
      enhancer->setWidth(width); 
    if(isLowerSet())
      lower->setWidth(width);
    for(int i = 0; i < upper.size(); i++)
      upper[i]->setWidth(width);

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
  for(int i = 0; i < base.size(); i++)
    base[i]->setX(x);
  if(isEnhancerSet())
    enhancer->setX(x); 
  if(isLowerSet())
    lower->setX(x);
  for(int i = 0; i < upper.size(); i++)
    upper[i]->setX(x);
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
  for(int i = 0; i < base.size(); i++)
    base[i]->setY(y);
  if(isEnhancerSet())
    enhancer->setY(y); 
  if(isLowerSet())
    lower->setY(y);
  for(int i = 0; i < upper.size(); i++)
    upper[i]->setY(y);
}

/* 
 * Description: Unsets the base in the tile. Deletes the pointers, if 
 *              applicable and sets the internal variable to notify the class 
 *              so the base isn't reused.
 *
 * Inputs: none
 * Output: bool - returns true if the base was set before being unset
 */
bool Tile::unsetBase()
{
  if(isBaseSet())
  {
    for(int i = 0; i < base.size(); i++)
    {
      emit deleteLayer(base[i]);
      delete base[i];
      base[i] = 0;
    }

    base.clear();
    return true;
  }
  return false;
}

/* 
 * Description: Unsets the enhancer layer in the tile. Deletes the pointer, 
 *              and sets the internal variable to notify the class so the 
 *              enhancer isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the enhancer was set before being unset
 */
bool Tile::unsetEnhancer()
{
  if(isEnhancerSet())
  {
    emit deleteLayer(enhancer);
    delete enhancer;
    enhancer = 0;
    return true;
  }
  return false;
}

/*
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
 * Description: Unsets the lower layer in the tile. Deletes the pointer, if 
 *              applicable, and sets the internal variable to notify the class 
 *              so the lower isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the lower was set before being unset
 */
bool Tile::unsetLower()
{
  if(isLowerSet())
  {
    emit deleteLayer(lower);
    delete lower;
    lower = 0;
    return true;
  }
  return false;
}

/*
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
 * Description: Unsets the upper layer(s) in the tile. Deletes the pointers, if 
 *              applicable, and sets the internal variable to notify the class 
 *              so the upper isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the upper was set before being unset
 */
bool Tile::unsetUpper()
{
  if(isUpperSet())
  {
    for(int i = 0; i < upper.size(); i++)
    {
      emit deleteLayer(upper[i]);
      delete upper[i];
      upper[i] = 0;
    }

    upper.clear();
    return true;
  }
  return false;
}
