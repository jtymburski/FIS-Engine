/******************************************************************************
* Class Name: Frame
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The Frame class, this represents an animation frame within the
*              Sprite class. It acts as a linked list node, in that it contains
*              a pointer to the next Frame in the sequence.
******************************************************************************/
#ifndef FRAME_H
#define FRAME_H

#include <QFile>
#include <QPixmap>

class Frame
{
public:
  /* Constructor function (path and next pointer initialization) */
  Frame(QString path, Frame* next = 0, Frame* previous = 0);

  /* Destructor function */
  ~Frame();

private:
  /* The image for this frame */
  QPixmap image;

  /* The previous element in the linked list */
  Frame* previous;

  /* The next element in the linked list */
  Frame* next;

  /* Asserts that the image has been set */
  bool image_set;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns if an image is set */
  bool isImageSet();

  /* Get stored image */
  QPixmap getImage();

  /* Get next frame */
  Frame* getNext();

  /* Get previous frame */
  Frame* getPrevious();

  /* Set stored image */
  bool setImage(QString path);

  /* Set next frame */
  bool setNext(Frame* next);

  /* Set previous frame */
  bool setPrevious(Frame* previous);
};

#endif // FRAME_H
