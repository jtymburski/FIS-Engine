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
#include <QImage>

class Frame
{
public:
  /* Constructor function (path and next pointer initialization) */
  Frame(QString path, Frame* next = 0);

  /* Destructor function */
  ~Frame();

/*--------------------------------------------------------
 * Variables
 *------------------------------------------------------*/
private:
  /* The image for this frame */
  QImage image;

  /* The next element in the linked list */
  Frame* next;

/*--------------------------------------------------------
 * Methods
 *------------------------------------------------------*/
public:
  /* Get stored image */
  QImage getImage();

  /* Get next frame */
  Frame* getNext();

  /* Set stored image */
  bool setImage(QString path);

  /* Set next frame */
  bool setNext(Frame* next);
};

#endif // FRAME_H
