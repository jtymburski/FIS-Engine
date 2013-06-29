/******************************************************************************
* Class Name: Layer
* Date Created: May 10, 2013
* Inheritance: QGraphicsItem
* Description: This class becomes the middle interface in between a sprite 
*              and converting it into the data needed to add it to the
*              QGraphicsScene. Essentially it just offers the re-implemented
*              functions needed to define a sprite.
******************************************************************************/
#include "Game/Map/Layer.h"

/* Constant Implementation - see header file for descriptions */
const int Layer::kLOWER_COUNT_MAX = 5;
const int Layer::kUPPER_COUNT_MAX = 5;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor function - Sets up a blank layer
 *
 * Input: none
 */
Layer::Layer()
{
  /* Sets the initial size of the lower and upper, as per the class limits */
  Sprite* null_sprite = 0;
  for(int i = 0; i < kLOWER_COUNT_MAX; i++)
  {
    lower.append(null_sprite);
    lower_passability.append(EnumDb::DIRECTIONLESS);
  }
  for(int i = 0; i < kUPPER_COUNT_MAX; i++)
    upper.append(null_sprite);

  /* Clears out the class storage and sets to unset default */
  clear();

  /* Set some QGraphicsItem specific classifications for performance */
  setAcceptedMouseButtons(0);
  //setCacheMode(DeviceCoordinateCache);
}

/* 
 * Description: Constructor function - Sets up the layer by defining the
 *              width and height. Optional variables include the X and 
 *              Y value of the widget as well as the layer in the scene (Z).
 *
 * Inputs: int width - the pixel width of the tile layer
 *         int height - the pixel height of the tile layer
 *         int x - the x location in the scene for the tile layer
 *         int y - the y location in the scene for the tile layer
 *         int z - the layer depth for the QGraphicsScene
 */
Layer::Layer(int width, int height, int x, int y, int z)
{
  /* Sets the initial size of the lower and upper, as per the class limits */
  Sprite* null_sprite = 0;
  for(int i = 0; i < kLOWER_COUNT_MAX; i++)
  {
    lower.append(null_sprite);
    lower_passability.append(EnumDb::DIRECTIONLESS);
  }
  for(int i = 0; i < kUPPER_COUNT_MAX; i++)
    upper.append(null_sprite);

  /* Clear variables and layer sprite information */
  unsetBase();
  unsetEnhancer();
  unsetLower();
  unsetUpper();
  paint_count = 0;

  /* Set layer parameters */
  this->width = width;
  this->height = height;
  setStatus(ACTIVE);

  /* Set coordinates */
  setX(x);
  setY(y);
  setZValue(z);

  /* Set some QGraphicsItem specific classifications for performance */
  setAcceptedMouseButtons(0);
  //setCacheMode(DeviceCoordinateCache);
}

/* 
 * Description: Destructor function 
 */
Layer::~Layer()
{
  /* Unset all the sprite pointers stored (does not delete) */
  unsetBase();
  unsetEnhancer();
  unsetLower();
  unsetUpper();

  /* Clear out the lower and upper lists */
  lower.clear();
  upper.clear();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Animates the layer. This just shifts all the sprites to the
 *              next frame in the sequence. If there is only one frame,
 *              it will just stay on that one. You probably don't want to call
 *              this on every tick since that would animate quite quickly.
 *
 * Inputs: none
 * Output: none
 */
void Layer::animate()
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
 * Description: Implemented virtual function. Returns the bounding rectangle
 *              around the image that is being added into the scene. 
 *
 * Inputs: none
 * Output: QRectF - the rectangle around the internal image
 */
QRectF Layer::boundingRect() const
{
  QRectF rect(0, 0, width, height);

  return rect;
}

/* 
 * Description: Clears out the class by deleting the necessary sprite pointer
 *              (if set) and then clears all the internal variables.
 *
 * Inputs: none
 * Output: none
 */
void Layer::clear()
{
  /* Clear variables and layer sprite information */
  unsetBase();
  unsetEnhancer();
  unsetLower();
  unsetUpper();
  paint_count = 0;

  /* Reset layer parameters */
  width = 0;
  height = 0;
  setStatus(OFF);

  /* Reset coordinates */
  setX(0);
  setY(0);
  setZValue(0);
}

/* 
 * Description: Gets the base portion of the layer. Is 0 if it's not set
 *
 * Inputs: none
 * Output: Sprite* - a sprite pointer for the base part of the layer
 */
Sprite* Layer::getBase()
{
  return base;
}

/* 
 * Description: Gets the passability for the base portion of the layer. This 
 *              only responds true passability for set lower layers. Otherwise,
 *              it returns true.
 *
 * Inputs: EnumDb::Direction dir - the direction to get
 * Output: bool - the base passability directional status
 */
bool Layer::getBasePassability(EnumDb::Direction dir)
{
  if(dir == EnumDb::DIRECTIONLESS)
    return (base_passability == EnumDb::DIRECTIONLESS);
  return ((base_passability & dir) > 0);
}

/* 
 * Description: Gets the enhancer portion of the layer. Is 0 if it's not set
 *
 * Inputs: none
 * Output: Sprite* - a sprite pointer for the enhancer part of the layer
 */
Sprite* Layer::getEnhancer()
{
  return enhancer;
}

/* 
 * Description: Gets the height of the stored sprite
 *
 * Inputs: none
 * Output: int - the height, in pixels
 */
int Layer::getHeight()
{
  return height;
}

/* 
 * Description: Gets the lower portion of the layer. Is an empty list if it's
 *              not set.
 *
 * Inputs: none
 * Output: QList<Sprite*> - a list of sprite pointers for the lower part of 
 *                          the layer
 */
QList<Sprite*> Layer::getLower()
{
  return lower;
}

/* 
 * Description: Gets the passability for the entire lower portion of the layer.
 *              Only returns for set lower layers. Otherwise, it returns true.
 *
 * Inputs: EnumDb::Direction dir - the direction to get
 * Output: bool - the lower passability directional status
 */
bool Layer::getLowerPassability(EnumDb::Direction dir)
{
  bool passability = true;

  for(int i = 0; i < kLOWER_COUNT_MAX; i++)
    passability &= getLowerPassability(i, dir);

  return passability;
}
#include <QDebug>
/* 
 * Description: Gets the passability for the lower specific index of the stack.
 *              Only returns an actual value for set lower layers. If it's
 *              not set, it returns true.
 *
 * Inputs: int index - the lower layer stack index
 *         EnumDb::Direction dir - the direction to get
 * Output: bool - the lower passability directional status
 */
bool Layer::getLowerPassability(int index, EnumDb::Direction dir)
{
  if(index >= 0 && index < kLOWER_COUNT_MAX)
  {
    if(dir == EnumDb::DIRECTIONLESS)
      return (lower_passability[index] == EnumDb::DIRECTIONLESS);
    return ((lower_passability[index] & dir) > 0);
  }
  return false;
}

/* 
 * Description: Returns the number time the paint protocol has occurred
 *
 * Inputs: none
 * Output: int - the number of paints that have occurred since conception
 */
int Layer::getPaintCount()
{
  return paint_count;
}

/* 
 * Description: Returns the status the layer is currently classified in. 
 *              See the corresponding setStatus()
 *
 * Inputs: none
 * Output: Status - public enum identifying the status
 */
Layer::Status Layer::getStatus()
{
  if(!blanked && !isVisible())
    return OFF;
  else if(!blanked && isVisible())
    return ACTIVE;
  return BLANKED;
}

/* 
 * Description: Gets the upper portion of the layer. Is an empty list if it's
 *              not set.
 *
 * Inputs: none
 * Output: QList<Sprite*> - a list of sprite pointers for the upper part of 
 *                          the layer
 */
QList<Sprite*> Layer::getUpper()
{
  return upper;
}

/* 
 * Description: Gets the width of the stored sprite
 *
 * Inputs: none
 * Output: int - the width, in pixels
 */
int Layer::getWidth()
{
  return width;
}

/* 
 * Description: Inserts a new lower sprite stored within the layer. Only sets 
 *              it if the pointer is valid and the number of frames is greater
 *              than 0 and if the index is in the stack (index of 0 - 4). 
 *
 * Inputs: Sprite* new_lower - the new lower layer to attempt to insert
 *         int index - the index of the lower layer to insert
 * Output: bool - status if the insertion succeeded
 */
bool Layer::insertLower(Sprite* new_lower, int index)
{
  if(new_lower != 0 && new_lower->getSize() > 0 && 
     index >= 0 && index < kLOWER_COUNT_MAX)
  {
    lower[index] = new_lower;
    lower_passability[index] = EnumDb::DIRECTIONLESS;

    return true;
  }
  return false;
}

/* 
 * Description: Inserts a new upper sprite stored within the layer. Only sets 
 *              it if the pointer is valid and the number of frames is greater
 *              than 0 and if the index is in the stack (index of 0 - 4). 
 *
 * Inputs: Sprite* new_upper - the new upper layer to attempt to append
 *         int index - the index of the upper layer to insert
 * Output: bool - status if the insertion succeeded
 */
bool Layer::insertUpper(Sprite* new_upper, int index)
{
  if(new_upper != 0 && new_upper->getSize() > 0 && 
     index >= 0 && index < kUPPER_COUNT_MAX)
  {
    upper[index] = new_upper;
    return true;
  }
  return false;
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
void Layer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
                  QWidget* widget)
{
  /* Increment the paint count */
  paint_count++;

  /* Remove unused parameter warnings */
  (void)option;
  (void)widget;

  /* Set painter information */
  painter->setBrush(QBrush(Qt::black));
  painter->setPen(Qt::NoPen);
  //painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

  /* Only paint if the tile isn't blanked */
  if(!blanked)
  {
    /* Paint the base first */
    if(base != 0)
      painter->drawPixmap(0, 0, width, height, base->getCurrent());

    /* Then the enhancer sprite */
    if(enhancer != 0)
      painter->drawPixmap(0, 0, width, height, enhancer->getCurrent());

    /* Then paint the set of lower layers */
    for(int i = 0; i < lower.size(); i++)
      if(lower[i] != 0)
        painter->drawPixmap(0, 0, width, height, lower[i]->getCurrent());

    /* Finish by printing the upper set, if set */
    for(int i = 0; i < upper.size(); i++)
      if(upper[i] != 0)
        painter->drawPixmap(0, 0, width, height, upper[i]->getCurrent());
  }
  else
  {
    /* If the tile is blanked, just paint a black square */
    painter->drawRect(QRectF(0, 0, width, height));
  }
}

/* 
 * Description: Sets the base sprite stored within the layer. Only sets it 
 *              if the pointer is valid and the number of frames is greater 
 *              than 0. The old base is set if the new pointer is valid.
 *
 * Inputs: Sprite* new_base - the new base layer to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Layer::setBase(Sprite* new_base)
{
  if(new_base != 0 && new_base->getSize() > 0)
  {
    unsetBase();
    base = new_base;
    base_passability = EnumDb::DIRECTIONLESS;

    return true;
  }
  return false;
}

/* 
 * Description: Sets the base passability using a given direction and the 
 *              set boolean value. Only fails if the base internal to the
 *              layer is not set.
 *
 * Inputs: EnumDb::Direction dir - the direction to set
 *         bool set_value - the value to set it to.
 * Output: bool - the status if the set was successful.
 */
bool Layer::setBasePassability(EnumDb::Direction dir, bool set_value)
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
 * Description: Sets the enhancer sprite stored within the layer. Only sets it 
 *              if the pointer is valid and the number of frames is greater 
 *              than 0. The old enhancer is set if the new pointer is valid.
 *
 * Inputs: Sprite* new_enhancer - the new enhancer layer to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Layer::setEnhancer(Sprite* new_enhancer)
{
  if(new_enhancer != 0 && new_enhancer->getSize() > 0)
  {
    unsetEnhancer();
    enhancer = new_enhancer;

    return true;
  }
  return false;
}

/* 
 * Description: Sets the height of the stored sprite. Fails if the new height
 *              is not positive and greater than 0.
 *
 * Inputs: int height - the height, in pixels
 * Output: bool - the status, if the height call was successful
 */
bool Layer::setHeight(int height)
{
  if(height > 0)
  {
    prepareGeometryChange();
    this->height = height;
    return true;
  }

  return false;
}

/* 
 * Description: Sets the lower sprite stored within the layer. Only sets it if 
 *              the pointer is valid and the number of frames is greater than 
 *              0. Since lower is a stack, it unsets the stack if the sprite
 *              is valid and puts this new lower at the front of the list.
 *
 * Inputs: Sprite* new_lower - the new lower layer to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Layer::setLower(Sprite* new_lower)
{
  if(new_lower != 0 && new_lower->getSize() > 0)
  {
    unsetLower();
    lower[0] = new_lower;
    lower_passability[0] = EnumDb::DIRECTIONLESS;

    return true;
  }
  return false;
}
 
/* 
 * Description: Sets the lower passability using a given direction and the 
 *              set boolean value with the selected index to indicate the 
 *              lower layer.
 *
 * Inputs: int index - the index indicating the lower layer
 *         EnumDb::Direction dir - the direction to set
 *         bool set_value - the value to set it to.
 * Output: bool - if the set was successful
 */ 
bool Layer::setLowerPassability(int index, EnumDb::Direction dir, 
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

/* 
 * Description: Sets the status that the layer must conform to, based on the
 *              public enum in the class. This classification decides if the 
 *              layer is painted or if it is blacked out. 
 *
 * Inputs: Status new_status - the new status for this class, enumerated
 * Output: none
 */
void Layer::setStatus(Status new_status)
{
  /* Disables all events for the items (might need to enable) */
  setEnabled(false);
 
  /* Set the other settings, based on the status given */
  if(new_status == OFF)
  {
    setVisible(false);
    blanked = false;
  }
  else if(new_status == BLANKED)
  {
    setVisible(true);
    blanked = true;
  }
  else if(new_status == ACTIVE)
  {
    setVisible(true);
    blanked = false;
  }
}

/* 
 * Description: Sets the upper sprite stored within the layer. Only sets it if 
 *              the pointer is valid and the number of frames is greater than 
 *              0. Since upper is a stack, it unsets the stack if the sprite
 *              is valid and puts this new upper at the front of the list.
 *
 * Inputs: Sprite* new_upper - the new upper layer to attempt to set
 * Output: bool - status if the insertion succeeded
 */
bool Layer::setUpper(Sprite* new_upper)
{
  if(new_upper != 0 && new_upper->getSize() > 0)
  {
    unsetUpper();
    upper[0] = new_upper;

    return true;
  }
  return false;
}

/* 
 * Description: Sets the width of the stored sprite. The call will fail if the
 *              width is not positive and greater than 0.
 *
 * Inputs: int width - the width, in pixels
 * Output: bool - the status, if the width change was successful
 */
bool Layer::setWidth(int width)
{
  if(width > 0)
  {
    prepareGeometryChange();
    this->width = width;
    return true;
  }

  return false;
}

/* 
 * Description: Unsets the base sprite stored within the class.
 * Note: this class does NOT delete the pointer, only releases it
 *
 * Inputs: none
 * Output: none
 */
void Layer::unsetBase()
{
  base = 0;
  base_passability = EnumDb::NORTH | EnumDb::EAST |
                     EnumDb::SOUTH | EnumDb::WEST;
}

/* 
 * Description: Unsets the enhancer sprite stored within the class.
 * Note: this class does NOT delete the pointer, only releases it
 *
 * Inputs: none
 * Output: none
 */
void Layer::unsetEnhancer()
{
  enhancer = 0;
}

/* 
 * Description: Unsets all the lower sprites stored within the class.
 * Note: this class does NOT delete the pointer, only releases it
 *
 * Inputs: none
 * Output: none
 */
void Layer::unsetLower()
{
  for(int i = 0; i < lower.size(); i++)
  {
    lower[i] = 0;
    lower_passability[i] = EnumDb::NORTH | EnumDb::EAST | 
                           EnumDb::SOUTH | EnumDb::WEST;
  }
}

/* 
 * Description: Unsets all the upper sprites stored within the class.
 * Note: this class does NOT delete the pointer, only releases it
 *
 * Inputs: none
 * Output: none
 */
void Layer::unsetUpper()
{
  for(int i = 0; i < upper.size(); i++)
    upper[i] = 0;
}
