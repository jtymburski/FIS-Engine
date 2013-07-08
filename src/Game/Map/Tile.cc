/******************************************************************************
* Class Name: Tile
* Date Created: December 2, 2012
* Inheritance: none
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
const char Tile::kLOWER_COUNT_MAX = 5;
const char Tile::kUPPER_COUNT_MAX = 5;

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
  clear();
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
 */
Tile::Tile(int width, int height, int x, int y)
{
  clear();

  /* Finally reset the parameters based on this alternate constructor */
  setHeight(height);
  setWidth(width);
  setX(x);
  setY(y);
}

/* 
 * Description: Destructor function
 */
Tile::~Tile()
{
  clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

bool Tile::addPassability(QString data, QString classifier, QString index)
{
  bool success = true;
  EnumDb::Direction new_direction = EnumDb::DIRECTIONLESS;
  QStringList data_list = data.split(",");
  
  /* Loop through each value of the data list to add */
  for(int i = 0; i < data_list.size(); i++)
  {
    /* Determine the data identifier first */
    if(data_list[i].toLower().trimmed() == "n")
      new_direction = EnumDb::NORTH;
    else if(data_list[i].toLower().trimmed() == "e")
      new_direction = EnumDb::EAST;
    else if(data_list[i].toLower().trimmed() == "s")
      new_direction = EnumDb::SOUTH;
    else if(data_list[i].toLower().trimmed() == "w")
      new_direction = EnumDb::WEST;

    /* Add the layer passability to the specific classifier */
    if(classifier.toLower().trimmed() == "base")
      success &= setBasePassability(new_direction, true);
    else if(classifier.toLower().trimmed() == "lower")
      success &= setLowerPassability(index.toInt(), new_direction, true);
  }

  return success;
}

bool Tile::addSprite(Sprite* frames, QString classifier, QString index)
{
  /* Add the layer data to the specific classifier */
  if(classifier.toLower().trimmed() == "base")
    return setBase(frames);
  else if(classifier.toLower().trimmed() == "enhancer")
    return setEnhancer(frames);
  else if(classifier.toLower().trimmed() == "lower")
    return insertLower(frames, index.toInt());
  else if(classifier.toLower().trimmed() == "upper")
    return insertUpper(frames, index.toInt());
  return false;
}

/*
 * Description: Animates all sprite layer sets on the tile. This allows for 
 *              the fine control over when the timer hits and how it's updated.
 *
 * Inputs: none
 * Output: none
 */
void Tile::animate()
{
  /* Animate the base first */
  if(base != 0)
    base->shiftNext();

  /* Then the enhancer sprite */
  if(enhancer != 0)
    enhancer->shiftNext();

  /* Then animate the set of lower layers */
  for(int i = 0; i < lower.size(); i++)
    if(lower[i] != 0)
      lower[i]->shiftNext();

  /* Finish by animating the upper set, if set */
  for(int i = 0; i < upper.size(); i++)
    if(upper[i] != 0)
      upper[i]->shiftNext();
}

/*
 * Description: Clears the data within the tile. Usually used for cleaning
 *              up after the tile operation is complete. Has the functionality
 *              just to clean out the sprites and not the width/height and
 *              coordinates of the tile.
 *
 * Inputs: bool just_sprites - Only clean up the sprites or all of the class.
 * Output: none
 */
void Tile::clear(bool just_sprites)
{
  /* Clear sprite layer data */
  unsetBase();
  unsetEnhancer();
  unsetLower();
  unsetUpper();

  /* Clear player/thing data */
  impassable_set = UNSET;
  passable_set = false;

  if(!just_sprites)
  {
    /* Reset tile parameters */
    setWidth(1);
    setHeight(1);
    setStatus(OFF);

    /* Reset coordinates */
    setX(0);
    setY(0);
  }
}

/* 
 * Description: Gets the base sprite sequence and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the base sprite pointer
 */
Sprite* Tile::getBase()
{
  return base;
}

/* 
 * Description: Gets the passability for the base portion of the tile. This 
 *              only responds true passability for set sprites. Otherwise,
 *              it returns true.
 *
 * Inputs: EnumDb::Direction dir - the direction to get
 * Output: bool - the base passability directional status
 */
bool Tile::getBasePassability(EnumDb::Direction dir)
{
  if(base != 0)
  {
    if(dir == EnumDb::DIRECTIONLESS)
      return (base_passability == EnumDb::DIRECTIONLESS);
    return ((base_passability & dir) > 0);
  }

  return true;
}

/* 
 * Description: Gets the enhancer sprite sequence and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the enhancer sprite pointer
 */
Sprite* Tile::getEnhancer()
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
  return lower;
}

/* 
 * Description: Gets the passability for the entire lower portion of the tile.
 *              Only returns for set lower sprites. Otherwise, it returns 
 *              true.
 *
 * Inputs: EnumDb::Direction dir - the direction to get
 * Output: bool - the lower passability directional status
 */
bool Tile::getLowerPassability(EnumDb::Direction dir)
{
  bool passability = true;

  if(dir == EnumDb::DIRECTIONLESS)
  {
    passability = false;

    for(int i = 0; i < lower_passability.size(); i++)
      if(lower[i] != 0)
        passability |= getLowerPassability(i, dir);
  }
  else
  {
    for(int i = 0; i < lower_passability.size(); i++)
      if(lower[i] != 0)
        passability &= getLowerPassability(i, dir);
  }

  return passability;
}

/* 
 * Description: Gets the passability for the lower specific index of the stack.
 *              Only returns an actual value for set sprites. If it's
 *              not set or if it's out of range, it returns true.
 *
 * Inputs: int index - the lower sprite stack index
 *         EnumDb::Direction dir - the direction to get
 * Output: bool - the lower passability directional status
 */
bool Tile::getLowerPassability(int index, EnumDb::Direction dir)
{
  if(index >= 0 && index < lower_passability.size() && lower[index] != 0)
  {
    if(dir == EnumDb::DIRECTIONLESS)
      return (lower_passability[index] == EnumDb::DIRECTIONLESS);
    return ((lower_passability[index] & dir) > 0);
  }
  return true;
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

/* TODO: Add in thing [2013-06-26]
 * Description: Gets if the tile is passable from the given direction
 *
 * Inputs: EnumDb::Direction dir - the direction enumerated for passability
 * Output: bool - status if the tile is accessible into the tile from the
 *                given direction.
 */
bool Tile::getPassability(EnumDb::Direction dir)
{
  if(dir == EnumDb::DIRECTIONLESS)
    return (getBasePassability(dir) || getLowerPassability(dir));
  return (getBasePassability(dir) && getLowerPassability(dir));
}

/* 
 * Description: Returns the status the tile is currently classified in. Uses
 *              the enum from Layer. See the corresponding setStatus()
 *
 * Inputs: none
 * Output: Status - public enum from layer identifying state
 */
Tile::TileStatus Tile::getStatus()
{
  return status;
}

/* 
 * Description: Gets the upper sprite(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: QList<Sprite*> - the upper sprites, in a QList
 */
QList<Sprite*> Tile::getUpper()
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
 * Description: Initialize the GL functionality in all the stored sprites
 *              within the tile. Only runs if the sprite is set and it hasn't
 *              already been initialized.
 *
 * Inputs: none
 * Output: int - if the initialization procedure was successful. Only fails
 *               if one of the sprites are set but with no legitimate frames.
 */
bool Tile::initializeGl()
{
  bool success = true;

  /* Initialize the base first */
  if(base != 0)
    success &= base->initializeGl();

  /* Then the enhancer sprite */
  if(enhancer != 0)
    success &= enhancer->initializeGl();

  /* Then initialize the set of lower layers */
  for(int i = 0; i < lower.size(); i++)
    if(lower[i] != 0)
      success &= lower[i]->initializeGl();

  /* Finish by initialize the upper set, if set */
  for(int i = 0; i < upper.size(); i++)
    if(upper[i] != 0)
      success &= upper[i]->initializeGl();

  return success;
}

/* 
 * Description: Inserts the new Sprite frame data onto the created lower
 *              portion for this tile.
 *
 * Inputs: Sprite* lower - the lower tile data, as a Sprite set
 *         int index - the index to where the lower sprite is inserted
 * Output: bool - if the insert call succeeded and the data was used
 */
bool Tile::insertLower(Sprite* lower, int index)
{
  if(lower != 0 && lower->getSize() > 0 && 
     index >= 0 && index < kLOWER_COUNT_MAX)
  {
    /* Increase the size of lower if it isn't big enough */
    Sprite* null_sprite = 0;
    if(index >= this->lower.size())
    {
      for(int i = this->lower.size(); i <= index; i++)
      {
        this->lower.append(null_sprite);
        lower_passability.append(EnumDb::DIRECTIONLESS);
      }
    }

    /* Actually set the lower layer now */
    this->lower[index] = lower;
    lower_passability[index] = EnumDb::DIRECTIONLESS;

    return true;
  }

  return false;
}

/* 
 * Description: Inserts the new Sprite frame data onto the created upper
 *              portion for this tile. 
 *
 * Inputs: Sprite* upper - the upper tile data, as a Sprite set
 *         int index - the index to where the upper sprite is inserted
 * Output: bool - if the insert call succeeded and the data was used
 */
bool Tile::insertUpper(Sprite* upper, int index)
{
  if(upper != 0 && upper->getSize() > 0 && 
     index >= 0 && index < kUPPER_COUNT_MAX)
  {
    /* Increase the size of upper if it isn't big enough */
    Sprite* null_sprite = 0;
    if(index >= this->upper.size())
      for(int i = this->upper.size(); i <= index; i++)
        this->upper.append(null_sprite);

    /* Actually set the upper layer now */
    this->upper[index] = upper;

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
  return (base != 0);
}

/* 
 * Description: Returns if the Enhancer Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the enhancer sprite is set
 */
bool Tile::isEnhancerSet()
{
  return (enhancer != 0);
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
  return (lower.size() > 0);
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
  return (upper.size() > 0);
}
  
/* 
 * Description: Paints the lower sprites in the tile using native GL calls. The 
 *              context for GL must have been called for this and the sprite
 *              GL initialization must have occurred before any painting.
 *
 * Inputs: int x - the x offset in the plane (left-right)
 *         int y - the y offset in the plane (up-down)
 *         int width - the width to render the sprite to
 *         int height - the height to render the sprite to
 *         float opacity - the transparency of the paint object (0-1)
 * Output: bool - status if the sprite was painted. If failed, make sure there
 *         is an image in the sprite and make sure initializeGl() was called.
 */
bool Tile::paintLower(int x, int y, int width, int height, float opacity)
{
  bool success = true;

  /* Paint the base first */
  if(base != 0)
    success &= base->paintGl(x, y, width, height, opacity);

  /* Then the enhancer sprite */
  if(enhancer != 0)
    success &= enhancer->paintGl(x, y, width, height, opacity);

  /* Then Paint the set of lower layers */
  for(int i = 0; i < lower.size(); i++)
    if(lower[i] != 0)
      success &= lower[i]->paintGl(x, y, width, height, opacity);

  return success;
}

/* 
 * Description: Paints the upper sprites in the tile using native GL calls. The 
 *              context for GL must have been called for this and the sprite
 *              GL initialization must have occurred before any painting.
 *
 * Inputs: int x - the x offset in the plane (left-right)
 *         int y - the y offset in the plane (up-down)
 *         int width - the width to render the sprite to
 *         int height - the height to render the sprite to
 *         float opacity - the transparency of the paint object (0-1)
 * Output: bool - status if the sprite was painted. If failed, make sure there
 *         is an image in the sprite and make sure initializeGl() was called.
 */
bool Tile::paintUpper(int x, int y, int width, int height, float opacity)
{
  bool success = true;

  /* Paint the upper set, if set */
  for(int i = 0; i < upper.size(); i++)
    if(upper[i] != 0)
      success &= upper[i]->paintGl(x, y, width, height, opacity);

  return success;
}

/* 
 * Description: Sets the base sprite stored within the tile. Only sets it 
 *              if the pointer is valid and the number of frames is greater 
 *              than 0. The old base is set if the new pointer is valid.
 *
 * Inputs: Sprite* base - the new base sprite to attempt to set
 * Output: bool - status if the set procedure succeeded
 */
bool Tile::setBase(Sprite* base)
{
  if(base != 0 && base->getSize() > 0)
  {
    unsetBase();
    this->base = base;
    base_passability = EnumDb::DIRECTIONLESS;

    return true;
  }
  return false;
}

/* 
 * Description: Sets the base passability using a given direction and the 
 *              set boolean value. Only fails if the base internal to the
 *              tile is not set.
 *
 * Inputs: EnumDb::Direction dir - the direction to set
 *         bool set_value - the value to set it to.
 * Output: bool - the status if the set was successful.
 */
bool Tile::setBasePassability(EnumDb::Direction dir, bool set_value)
{
  /* Only set if the base is set */
  if(base != 0)
  {
    if(dir == EnumDb::DIRECTIONLESS && set_value)
      base_passability = EnumDb::DIRECTIONLESS;
    else
      (set_value) ? (base_passability |= dir) : (base_passability &= ~dir);

    return true;
  }
  return false;
}

/* 
 * Description: Sets the enhancer sprite stored within the tile. Only sets it 
 *              if the pointer is valid and the number of frames is greater 
 *              than 0. The old enhancer is set if the new pointer is valid.
 *
 * Inputs: Sprite* enhancer - the new enhancer sprite to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Tile::setEnhancer(Sprite* enhancer)
{
  if(enhancer != 0 && enhancer->getSize() > 0)
  {
    unsetEnhancer();
    this->enhancer = enhancer;

    return true;
  }
  return false;
}

/*
 * Description: Sets the height of the tile.
 *
 * Inputs: int height - the tile height, in pixels
 * Output: bool - returns if the height set was successful (and the height was
 *                a positive number and greater than 0)
 */
bool Tile::setHeight(int height)
{
  if(height > 0)
  {
    this->height = height;

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
 * Description: Sets the lower sprite stored within the tile. Only sets it if 
 *              the pointer is valid and the number of frames is greater than 
 *              0. Since lower is a stack, it unsets the stack if the sprite
 *              is valid and puts this new lower at the front of the list.
 *
 * Inputs: Sprite* lower - the new lower sprite to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Tile::setLower(Sprite* lower)
{
  if(lower != 0 && lower->getSize() > 0)
  {
    Sprite* null_sprite = 0;

    unsetLower();
    this->lower.append(null_sprite);
    this->lower[0] = lower;
    lower_passability[0] = EnumDb::DIRECTIONLESS;

    return true;
  }
  return false;
}

/*
 * Description: Sets the lower passability for a sprite based on an index, 
 *              a direction enumerator, and the value it should be. Only 
 *              fails if the index is out of range of the allowable bounds
 *              or the lower isn't set at that index.
 *
 * Inputs: int index - the lower layer index
 *         EnumDb::Direction dir - the direction passability to set
 *         bool set_value - the value to set that direction to
 * Output: bool - if the set was successful
 */
bool Tile::setLowerPassability(int index, EnumDb::Direction dir, 
                                          bool set_value)
{
  /* Only set if the lower layer is valid and in the right range */
  if(index >= 0 && index < kLOWER_COUNT_MAX && lower[index] != 0)
  {
    if(dir == EnumDb::DIRECTIONLESS && set_value)
      lower_passability[index] = EnumDb::DIRECTIONLESS;
    else
      (set_value) ? (lower_passability[index] |= dir) 
                  : (lower_passability[index] &= ~dir);

    return true;
  }
  return false;
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

/* 
 * Description: Sets the tile status. This allows of 3 possible states that 
 *              the tile can be in. This affects the visibility and painting \
 *              of the tile.
 * 
 * Inputs: TileStatus status - the new status to update the tile to
 * Output: none
 */
void Tile::setStatus(TileStatus status)
{
  this->status = status;
}

/* 
 * Description: Sets the upper sprite stored within the tile. Only sets it if 
 *              the pointer is valid and the number of frames is greater than 
 *              0. Since upper is a stack, it unsets the stack if the sprite
 *              is valid and puts this new upper at the front of the list.
 *
 * Inputs: Sprite* upper - the new upper layer to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Tile::setUpper(Sprite* upper)
{
  if(upper != 0 && upper->getSize() > 0)
  {
    unsetUpper();
    this->upper[0] = upper;

    return true;
  }
  return false;
}

/*
 * Description: Sets the width of the tile.
 *
 * Inputs: int width - the tile width, in pixels
 * Output: bool - returns if the width set was successful (and the width was
 *                a positive number greater than 0)
 */
bool Tile::setWidth(int width)
{
  if(width > 0)
  {
    this->width = width;

    return true;
  }
  return false;
}

/*
 * Description: Sets the X coordinate of the tile. 
 *
 * Inputs: int x - the tile x coordinate, in pixels
 * Output: none
 */
void Tile::setX(int x)
{
  this->x = x;
}

/*
 * Description: Sets the Y coordinate of the tile.
 *
 * Inputs: int y - the tile y coordinate, in pixels
 * Output: none
 */
void Tile::setY(int y)
{
  this->y = y;
}

/* 
 * Description: Unsets the base sprite in the tile.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetBase()
{
  base = 0;
  base_passability = EnumDb::DIRECTIONLESS;
}

/* 
 * Description: Unsets the enhancer sprite in the tile.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetEnhancer()
{
  enhancer = 0;
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
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetLower()
{
  /* Disconnect all references */
  for(int i = 0; i < lower.size(); i++)
  {
    lower[i] = 0;
    lower_passability[i] = EnumDb::DIRECTIONLESS;
  }

  /* Clean out the array */
  lower.clear();
  lower_passability.clear();
}

/* 
 * Description: Unsets the lower sprite in the tile based on the index, only
 *              if it exists.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: int index - the index of the lower to unset
 * Output: bool - status if the unset occurred
 */
bool Tile::unsetLower(int index)
{
  if(index < lower.size() && lower[index] != 0)
  {
    lower[index] = 0;
    lower_passability[index] = EnumDb::DIRECTIONLESS;
    return true;
  }
  return false;
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
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: none
 * Output: none
 */
void Tile::unsetUpper()
{
  /* Disconnect all references */
  for(int i = 0; i < upper.size(); i++)
    upper[i] = 0;

  /* Clear out the array */
  upper.clear();
}

/* 
 * Description: Unsets the upper sprite in the tile based on the index, only
 *              if it exists.
 * Note: this class does NOT delete the pointer, only releases it.
 *
 * Inputs: int index - the index of the upper to unset
 * Output: bool - status if the unset occurred
 */
bool Tile::unsetUpper(int index)
{
  if(index < upper.size() && upper[index] != 0)
  {
    upper[index] = 0;
    return true;
  }
  return false;
}
