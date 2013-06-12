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

#include <QDebug>

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
  /* Initialize the item to a blank slate */
  item = 0;
  width = 0;
  height = 0;
  visible = true;

  /* TEMP */
  paint_count = 0;
}

/* 
 * Description: Constructor function - Sets up the layer by defining the
 *              sprite, width and height. Optional variables include the X and 
 *              Y value of the widget as well as the layer in the scene (Z).
 *
 * Inputs: Sprite* item - the new sprite to create the layer around
 *         int width - the pixel width of the tile layer
 *         int height - the pixel height of the tile layer
 *         int x - the x location in the scene for the tile layer
 *         int y - the y location in the scene for the tile layer
 *         int z - the layer depth for the QGraphicsScene
 */
Layer::Layer(Sprite* item, int width, int height, int x, int y, int z)
{
  /* Pointers nullified */
  this->item = 0;

  /* Initialize the item */
  setItem(item);
  visible = true;
  setEnabled(false);
  setHeight(height);
  setWidth(width);

  /* Set the coordinates */
  setX(x);
  setY(y);
  setZValue(z);

  /* TEMP */
  paint_count = 0;
}

/* 
 * Description: Destructor function 
 */
Layer::~Layer()
{
  clear();
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
  unsetItem();
  width = 0;
  height = 0;
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
 * Description: Gets the sprite item encapsulated by the layer
 *
 * Inputs: none
 * Output: Sprite* - the sprite housed within this layer
 */
Sprite* Layer::getItem()
{
  return item;
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
 * Description: Returns if the layer is set to be visible
 *
 * Inputs: none
 * Output: bool - the layer visibility status
 */
bool Layer::isVisible()
{
  return visible;
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
  //painter->setBrush(QBrush(Qt::black));
  //painter->setPen(Qt::NoPen);
  //painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

  /* Only paint if enabled */
  if(isEnabled())
  {
    if(isVisible())
    {
      /* Only paint if sprite exists */
      if(item != 0)
        painter->drawPixmap(0, 0, width, height, item->getCurrent());
    }
    else
    {
      painter->drawRect(QRectF(0, 0, width, height));
    }
  }
}

/* 
 * Description: Sets the height of the stored sprite
 *
 * Inputs: int height - the height, in pixels
 * Output: bool - the status, returns false if the height is <= 0
 */
bool Layer::setHeight(int height)
{
  if(height > 0)
  {
    this->height = height;
    return true;
  }

  this->height = 0;
  return false;
}

/* 
 * Description: Sets the item stored within the layer. Only sets it if the
 *              pointer is valid and the number of frames is greater than 0.
 *
 * Inputs: Sprite* item - the new item pointer to attempt to insert in
 *         bool unset_old - delete the old item from memory?
 * Output: bool - status if the insertion succeeded
 */
bool Layer::setItem(Sprite* item, bool unset_old)
{

  if(item != 0 && item->getSize() > 0)
  {
    if(unset_old)
      unsetItem();
    this->item = item;

    return true;
  }
  return false;
}

/* 
 * Description: Sets if the sprite layer is visible
 *
 * Inputs: bool status - the visibility status
 * Output: none
 */
void Layer::setVisible(bool status)
{
  visible = status;
}

/* 
 * Description: Sets the width of the stored sprite
 *
 * Inputs: int width - the width, in pixels
 * Output: bool - the status, returns false if the width is <= 0
 */
bool Layer::setWidth(int width)
{
  if(width > 0)
  {
    this->width = width;
    return true;
  }

  this->width = 0;
  return false;
}

/* 
 * Description: Unsets the item stored within the class, by deleting the 
 *              pointer and nullifying it.
 *
 * Inputs: none
 * Output: none
 */
void Layer::unsetItem(bool deleteItem)
{
  if(deleteItem)
    delete item;
  item = 0;
}
