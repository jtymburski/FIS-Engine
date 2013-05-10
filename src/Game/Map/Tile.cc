/******************************************************************************
* Class Name: Tile
* Date Created: Dec 2 2012
* Inheritance: QWidget
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
const int Tile::kENHANCER_TOTAL = 4;
const int Tile::kNE_ENHANCER     = 1;
const int Tile::kNW_ENHANCER     = 0;
const int Tile::kSE_ENHANCER     = 3;
const int Tile::kSW_ENHANCER     = 2;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

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
 *         QWidget* parent - the parent widget to be set in the tile
 */
Tile::Tile(int width, int height, int x, int y, QWidget* parent)
{
  //setParent(parent);
  base_set = false;
  enhancer_set = false;
  lower_set = false;
  impassable_set = UNSET; 
  upper_set = false;
  passable_set = false;

  setPassibility(true);
  tileselector = new GridShifter(this);
  hover = false;
  connect(tileselector->getBase(),SIGNAL(textChanged(QString)),this,SLOT(setBase(QString)));
  connect(tileselector->getLower(),SIGNAL(textChanged(QString)),this,SLOT(setLower(QString)));
  connect(tileselector->getUpper(),SIGNAL(textChanged(QString)),this,SLOT(setUpper(QString)));
  setGeometry(x, y, width, height);

  //show();
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
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Paint event for the tile that gets called when the QWidget
 *              gets updated. Handles all the printing of sprites and 
 *              assembling the tile into one. Note: this is an override
 *              function.
 *
 * Inputs: QPaintEvent* event - the event that results in the call
 * Output: none
 */
void Tile::paintEvent(QPaintEvent* event)
{
  (void)event;// TODO: removing causes warning
  QPainter painter(this);

  /* Print the base, if it exists */
  if(base_set)
    painter.drawPixmap(0, 0, base->getCurrentAndShift());

  /* Print the enhancer, if it exists */
  //if(enhancer_set)
  //    painter.drawPixmap(0,0,enhancer->getCurrentAndShift());

  /* Print the lower sprite, if it exists */
  //if(lower_set)
  //  painter.drawPixmap(0, 0, width(), height(), lower->getCurrentAndShift());

  /* Print the upper sprite, if it exists */
  //if(upper_set)
  //  painter.drawPixmap(0, 0, upper->getCurrentAndShift());

  /* Bounding box if the mouse is hovered over the tile */
  //if(hover)
  //{
  //  painter.setPen(QColor(Qt::red));
  //  painter.drawRect(1,1,width()-2,height()-2);
  //}
}

/*
 * Description: Mouse Press event that handles setting map tiles on the fly for
 *              easy testing of textures in different places, this will be
 *              removed once either Map starts working, or the Map Editor is
 *              implemented, this is just a temporary method
 * Inputs: QMouseEvent*
 * Output: none
 */
void Tile::mousePressEvent(QMouseEvent *)
{
    tileselector->show();
}
void Tile::enterEvent(QEvent *)
{
    hover = true;
    update();
}
void Tile::leaveEvent(QEvent *)
{
    hover = false;
    update();
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

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Animates all sprites on tile. This allows for the fine control
 *              of the QWidget update and if any special conditions need to be
 *              done for animation.
 *
 * Inputs: none
 * Output: none
 */
void Tile::animate()
{
  // TODO
  //update();
}

/* 
 * Description: Gets the base sprite and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the base sprite pointer
 */
Sprite* Tile::getBase()
{
  if(base_set)
    return base;
  return NULL;
}

/* 
 * Description: Gets the enhancer sprites and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the enhancer sprite pointer
 */
Sprite* Tile::getEnhancer()
{
  if(enhancer_set)
    return enhancer;
  return NULL;
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
 * Description: Gets the lower sprite and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the lower sprite pointer
 */
Sprite* Tile::getLower()             
{
  if(lower_set)
    return lower;
  return NULL;
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
 * Description: Gets the upper sprite and returns it, if set.
 *
 * Inputs: none
 * Output: Sprite* - the upper sprite pointer
 */
Sprite* Tile::getUpper()
{
  if(upper_set)
    return upper;
  return NULL;
}

/* 
 * Description: Returns if the Base Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the base sprite is set
 */
bool Tile::isBaseSet()
{
  return base_set;
}

/* 
 * Description: Returns if the Enhancer Sprite(s) is set 
 *
 * Inputs: none
 * Output: bool - status if the enhancer sprite is set
 */
bool Tile::isEnhancerSet()
{
  return enhancer_set;
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
 * Description: Returns if the Lower Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the lower sprite is set
 */
bool Tile::isLowerSet()
{
  return lower_set;
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
 * Description: Returns if the Upper Sprite is set 
 *
 * Inputs: none
 * Output: bool - status if the upper sprite is set
 */
bool Tile::isUpperSet()
{
  return upper_set;
}

/* 
 * Description: Sets the base sprite in the tile using a path to the sprite 
 *              image file.
 *
 * Inputs: QString path - the path to the image to load in
 *         RotateAngle angle - the angle classification to rotate the tile
 * Output: bool - returns true if the base sprite was set
 */
bool Tile::setBase(QString path, RotatedAngle angle)
{
  /* Deletes the old base and sets the new base */
  unsetBase();
  base = new Sprite(path, getAngle(angle));

  /* Determine if the base was set successfully */
  if(base->getSize() == 0)
  {
    delete base;
    base_set = false;
  }
  else
  {
    base_set = true;  
  }

  return base_set;
}

/*
 * Description: Sets the enhancer tile using a path to a single sprite image
 *              file to cover the tile.
 * Warning: This will unset the enhancer no matter what, even if the call
 *          fails to set the enhancer with the new sprites. 
 *
 * Inputs: QString path - the path to the image to load in
 *         RotateAngle angle - the angle classification to rotate the tile 
 * Output: bool - returns true if the enhancer was set successfully.
 */
bool Tile::setEnhancer(QString path, RotatedAngle angle)
{
  /* Deletes the old enhancer and sets the new enhancer */
  unsetEnhancer();
  enhancer = new Sprite(path, getAngle(angle));

  /* Determine if the enhancer was set successfully */
  if(enhancer->getSize() == 0)
  {
    delete enhancer;
    enhancer_set = false;
  }
  else
  {
    enhancer_set = true;  
  }

  return enhancer_set;
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
 * Description: Sets the lower sprite in the tile using a path to the sprite 
 *              image file.
 *
 * Inputs: QString path - the path to the image to load in
 *         RotateAngle angle - the angle classification to rotate the tile 
 * Output: bool - returns true if the lower sprite was successfuly set
 */
bool Tile::setLower(QString path, RotatedAngle angle)
{
  /* Unset the sprite if it exists and try and set the new one */
  unsetLower();
  lower = new Sprite(path, getAngle(angle));

  /* Determine if the lower was set successfully */
  if(lower->getSize() == 0)
  {
    delete lower;
    lower_set = false;
  }
  else
  {
    lower_set = true;  
  }

  return lower_set;
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
 * Description: Sets the upper sprite in the tile using a path to the sprite 
 *              image file.
 *
 * Inputs: QString path - the path to the image to load in
 *         RotateAngle angle - the angle classification to rotate the tile 
 * Output: bool - returns true if the upper sprite was set before
 */
bool Tile::setUpper(QString path, RotatedAngle angle)
{
  /* Unset the sprite if it exists and try and set the new one */
  unsetUpper();
  upper = new Sprite(path, getAngle(angle));

  /* Determine if the upper was set successfully */
  if(upper->getSize() == 0)
  {
    delete upper;
    upper_set = false;
  }
  else
  {
    upper_set = true;
  }

  return upper_set;
}

/* 
 * Description: Unsets the base in the tile. Deletes the pointer, if applicable
 *              and sets the internal variable to notify the class so the base
 *              isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the base was set before being unset
 */
bool Tile::unsetBase()
{
  if(base_set)
  {
    delete base;
    base_set = false;
    return true;
  }
  return false;
}

/* 
 * Description: Unsets the enhancer in the tile. Deletes the pointer, 
 *              and sets the internal variable to notify the class so the 
 *              enhancer isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the enhancer was set before being unset
 */
bool Tile::unsetEnhancer()
{
  if(enhancer_set)
  {
    delete enhancer;
    enhancer_set = false;
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
 * Description: Unsets the lower sprite in the tile. Deletes the pointer, if 
 *              applicable, and sets the internal variable to notify the class 
 *              so the lower isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the lower was set before being unset
 */
bool Tile::unsetLower()
{
  if(lower_set)
  {
    delete lower;
    lower_set = false;
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
 * Description: Unsets the upper sprite in the tile. Deletes the pointer, if 
 *              applicable, and sets the internal variable to notify the class 
 *              so the upper isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the upper was set before being unset
 */
bool Tile::unsetUpper()
{
  if(upper_set)
  {
    delete upper;
    upper_set = false;
    return true;
  }
  return false;
}
