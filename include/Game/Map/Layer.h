/******************************************************************************
* Class Name: Layer
* Date Created: May 10, 2013
* Inheritance: QGraphicsItem
* Description: This class becomes the middle interface in between a sprite 
*              and converting it into the data needed to add it to the
*              QGraphicsScene. Essentially it just offers the re-implemented
*              functions needed to define a sprite.
******************************************************************************/
#ifndef LAYER_H
#define LAYER_H

//#include <QDebug>
#include <QGraphicsItem>
#include <QPainter>

#include "Game/Sprite.h"

class Layer : public QGraphicsItem
{
public:
  /* Constructor functions */
  Layer();
  Layer(int width, int height, int x = 0, int y = 0, int z = 0);

  /* Destructor function */
  ~Layer();

  /* OFF - Not rendered at all
   * BLANKED - Blacked out
   * ACTIVE - Rendered */
  enum Status{OFF, BLANKED, ACTIVE};

private:
  /* The information that defines the layer */
  int width;
  int height;
  bool blanked;

  /* The base information */
  Sprite* base;
  char base_passability;

  /* The enhancer information */
  Sprite* enhancer;

  /* The lower information */
  QList<Sprite*> lower;
  QList<char> lower_passability;

  /* The upper information */
  QList<Sprite*> upper;

  /* Temporary, DELETE when done */
  int paint_count;

  /*------------------- Constants -----------------------*/
  const static int kLOWER_COUNT_MAX; /* The maximum number of lower layers */
  const static int kUPPER_COUNT_MAX; /* The max number of upper layers */
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Animates the layer, and the frames available within it */
  void animate();

  /* Virtual bounding rectangle - The rectangle that encapsulates the item */
  QRectF boundingRect() const;

  /* Clear out the layer definition */
  void clear();

  /* Returns the base layer sprite and passability */
  Sprite* getBase();
  bool getBasePassability(EnumDb::Direction dir);

  /* Returns the enhancer layer sprite */
  Sprite* getEnhancer();

  /* Returns the height of the layer */
  int getHeight();

  /* Returns the list of lower layer sprites */
  QList<Sprite*> getLower();
  bool getLowerPassability(EnumDb::Direction dir);
  bool getLowerPassability(int index, EnumDb::Direction dir);

  /* Returns the number of time the layer has painted */
  int getPaintCount();

  /* Returns the status corresponding to the particular layer */
  Status getStatus();

  /* Returns the list of upper layer sprites */
  QList<Sprite*> getUpper();

  /* Returns the width of the layer */
  int getWidth();

  /* Inserts to the existing lower and upper layers. This allows for multiple
   * uppers and lowers to exist */
  bool insertLower(Sprite* new_lower, int index);
  bool insertUpper(Sprite* new_upper, int index);

  /* Virtual painter reimplementation - for painting the item */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
             QWidget* widget);

  /* Sets the base sprite and the passability */
  bool setBase(Sprite* new_base);
  bool setBasePassability(EnumDb::Direction dir, bool set_value);

  /* Sets the enhancer sprite, if it's a legitimate sprite */
  bool setEnhancer(Sprite* new_enhancer);

  /* Sets the height, stored in the layer */
  bool setHeight(int height);

  /* Sets the lower sprite. This removes all other lower sprites and sets
   * this as the only one, if it's legitimate */
  bool setLower(Sprite* new_lower);
  bool setLowerPassability(int index, EnumDb::Direction dir, bool set_value);
  
  /* Sets the status of the layer */
  void setStatus(Status new_status);

  /* Sets the upper sprite. This removes all other upper sprites and sets
   * this as the only one, if ti's legitimate */
  bool setUpper(Sprite* new_upper);

  /* Sets the width, stored in the layer */
  bool setWidth(int width);

  /* The unset calls, to clean up all the pointers in the class. 
   * NOTE: This does not delete pointers */
  void unsetBase();
  void unsetEnhancer();
  void unsetLower();
  void unsetUpper();
};

#endif // LAYER_H
