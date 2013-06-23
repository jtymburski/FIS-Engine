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
  /* Constructor function - empty initialization */
  Frame();

  /* Constructor function - image and next pointer initialization */
  Frame(QPixmap image, int rotate_angle = 0,
        Frame* next = 0, Frame* previous = 0);

  /* Constructor function - path and next pointer initialization */
  Frame(QString path, int rotate_angle = 0, 
        Frame* next = 0, Frame* previous = 0);

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

  /* Rotate the image an integer number of degrees */
  bool rotateImage(int angle);

  /* Sets the stored image, from QPixmap */
  bool setImage(QPixmap image);

  /* Sets the stored image, from QString */
  bool setImage(QString path);

  /* Set next frame */
  bool setNext(Frame* next);

  /* Set previous frame */
  bool setPrevious(Frame* previous);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Opens an image and returns the qpixmap, if it exists */
  static QPixmap openImage(QString path);
};

#endif // FRAME_H
