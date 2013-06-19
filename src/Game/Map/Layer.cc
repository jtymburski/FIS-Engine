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
  clear();
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
  /* Clear variables and layer sprite information */
  base = 0;
  enhancer = 0;
  lower.clear();
  upper.clear();
  paint_count = 0;

  /* Set layer parameters */
  this->width = width;
  this->height = height;
  setStatus(ACTIVE);

  /* Set coordinates */
  setX(x);
  setY(y);
  setZValue(z);
}

/* 
 * Description: Destructor function 
 */
Layer::~Layer()
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
 * Description: Appends a new lower sprite stored within the layer. Only sets 
 *              it if the pointer is valid and the number of frames is greater
 *              than 0 and if there is room in the stack (limit of 5). 
 *
 * Inputs: Sprite* new_lower - the new lower layer to attempt to append
 * Output: bool - status if the insertion succeeded
 */
bool Layer::appendLower(Sprite* new_lower)
{
  if(new_lower != 0 && new_lower->getSize() > 0
                    && lower.size() < kLOWER_COUNT_MAX)
  {
    lower.append(new_lower);
    return true;
  }
  return false;
}

/* 
 * Description: Appends a new upper sprite stored within the layer. Only sets 
 *              it if the pointer is valid and the number of frames is greater
 *              than 0 and if there is room in the stack (limit of 5). 
 *
 * Inputs: Sprite* new_upper - the new upper layer to attempt to append
 * Output: bool - status if the insertion succeeded
 */
bool Layer::appendUpper(Sprite* new_upper)
{
  if(new_upper != 0 && new_upper->getSize() > 0 
                    && upper.size() < kUPPER_COUNT_MAX)
  {
    upper.append(new_upper);
    return true;
  }
  return false;
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
      painter->drawPixmap(0, 0, width, height, lower[i]->getCurrent());

    /* Finish by printing the upper set, if set */
    for(int i = 0; i < upper.size(); i++)
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
    lower.append(new_lower);

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
    upper.append(new_upper);

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
  lower.clear();
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
  upper.clear();
}
