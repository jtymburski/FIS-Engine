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
  setParent(parent);
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
  show();
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
  (void)event;//warning
  QPainter painter(this);

  /* Print the base, if it exists */
  if(base_set)
    painter.drawPixmap(0, 0, base->getCurrentAndShift());

  /* Print the enhancer, if it exists */
  if(enhancer_set)
  {
    if(enhancer.size() == 1)
    {
      painter.drawPixmap(0,0,enhancer[0]->getCurrentAndShift());
    }
    else
    {
      if(enhancer[0] != NULL)
        painter.drawPixmap(0,0,enhancer[0]->getCurrentAndShift());
      if(enhancer[1] != NULL)
        painter.drawPixmap(32,0,enhancer[1]->getCurrentAndShift());
      if(enhancer[2] != NULL)
        painter.drawPixmap(0,32,enhancer[2]->getCurrentAndShift());
      if(enhancer[3] != NULL)
        painter.drawPixmap(32,32,enhancer[3]->getCurrentAndShift());
    }
  }

  /* Print the lower sprite, if it exists */
  if(lower_set)
    painter.drawPixmap(0, 0, width(), height(), lower->getCurrentAndShift());

  /* Print the upper sprite, if it exists */
  if(upper_set)
    painter.drawPixmap(0, 0, upper->getCurrentAndShift());

  /* Bounding box if the mouse is hovered over the tile */
  if(hover)
  {
    painter.setPen(QColor(Qt::red));
    painter.drawRect(1,1,width()-2,height()-2);
  }

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
 * Description: Gets the enhancer sprite(s) and returns it(them), if set.
 *
 * Inputs: none
 * Output: QVector<Sprite*> - the enhancer sprite QVector pointer
 */
QVector<Sprite*> Tile::getEnhancer()
{
  if(enhancer_set)
    return enhancer;

  QVector<Sprite*> temp_vector;
  return temp_vector;
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
 * Output: bool - returns true if the base sprite was set
 */
bool Tile::setBase(QString path)
{
  /* Deletes the old base and sets the new base */
  unsetBase();
  base = new Sprite(path);

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
 * Output: bool - returns true if the enhancer was set successfully.
 */
bool Tile::setEnhancer(QString path)
{
  unsetEnhancer();
  Sprite* new_enhancer = new Sprite(path);

  if(new_enhancer->getSize() != 0)
  {
    /* If the image size is half of the tile or less, call the function
     * that puts 4 in one instead of a just single one. Allows for 4 32x32
     * identical tiles to be assembled by just sending one path */
    if(new_enhancer->getCurrent().width() <= this->width() / 2)
      setEnhancer(path, path, path, path);
    else
      enhancer.append(new_enhancer);

    enhancer_set = true;
  }
  else
  {
    delete new_enhancer;
    enhancer_set = false;
  }

  return enhancer_set;
}

/*
 * Description: Sets the enhancer tile using a path to 4 sprites that cover
 *              the 4 corners of the standard tile size. The corners are: 
 *              NW NE
 *              SW SE
 *              Set the path to "" if you don't want to use that corner of
 *              the Enhancer set in the Tile.
 * Warning: This will unset the enhancer no matter what, even if the call
 *          fails to set the enhancer with the new sprites.
 *
 * Inputs: QString nw_path - the NW corner path for the sprite
 *         QString ne_path - the NE corner path for the sprite
 *         QString sw_path - the SW corner path for the sprite
 *         QString se_path - the SE corner path for the sprite
 * Output: bool - returns true if the enhancer was successfuly set.
 */
bool Tile::setEnhancer(QString nw_path, QString ne_path, 
                       QString sw_path, QString se_path)
{
  unsetEnhancer();
  enhancer_set = true;
  Sprite test_sprite;

  /* Sets the new enhancer tile with 4 1/4 portions of a tile */
  if(!nw_path.isEmpty())
  {
    if(test_sprite.insertTail(nw_path))
      enhancer.append(new Sprite(nw_path));
    else
      enhancer_set = false;
  }
  else
  {
    enhancer.append(NULL);
  }

  if(!ne_path.isEmpty() && enhancer_set)
  {
    if(test_sprite.insertTail(ne_path))
      enhancer.append(new Sprite(ne_path));
    else
      enhancer_set = false;
  }
  else
  {
    enhancer.append(NULL);
  }

  if(!sw_path.isEmpty() && enhancer_set)
  {
    if(test_sprite.insertTail(sw_path))
      enhancer.append(new Sprite(sw_path));
    else
      enhancer_set = false;
  }
  else
  {
    enhancer.append(NULL);
  }

  if(!se_path.isEmpty() && enhancer_set)
  {
    if(test_sprite.insertTail(se_path))
      enhancer.append(new Sprite(se_path));
    else
      enhancer_set = false;
  }
  else
  {
    enhancer.append(NULL);
  }

  /* Unset if it failed during the process */
  if(!enhancer_set)
    unsetEnhancer();

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
 * Output: bool - returns true if the lower sprite was successfuly set
 */
bool Tile::setLower(QString path)
{
  /* Unset the sprite if it exists and try and set the new one */
  unsetLower();
  lower = new Sprite(path);

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
 * Output: bool - returns true if the upper sprite was set before
 */
bool Tile::setUpper(QString path)
{
  /* Unset the sprite if it exists and try and set the new one */
  unsetUpper();
  upper = new Sprite(path);

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
 * Description: Unsets the enhancer in the tile. Deletes the pointer(s), 
 *              clears out the QVector if applicable, and sets the internal
 *              variable to notify the class so the enhancer isn't repainted.
 *
 * Inputs: none
 * Output: bool - returns true if the enhancer was set before being unset
 */
bool Tile::unsetEnhancer()
{
  if(enhancer_set)
  {
    for(int i = 0; i < enhancer.size(); i++)
      delete enhancer[i];

    while(enhancer.size() > 0)
      enhancer.remove(0);
    
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
