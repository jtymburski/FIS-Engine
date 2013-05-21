/******************************************************************************
* Class Name: Sprite
* Date Created: Oct 28 2012
* Inheritance: none
* Description: The Sprite class. This handles the linked list control that 
*              wraps the Frame. This will allow for a sequence of events, 
*              that emulate a GIF for animation or just store one image.
******************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>

#include "Game/Frame.h"

class Sprite
{
public:
  /* Constructor: Set up no images */
  Sprite();

  /* Constructor: Set up one image */
  Sprite(QPixmap image, int rotate_angle = 0);
  Sprite(QString image_path, int rotate_angle = 0);

  /* Constructor: Set up sequence of images */
  Sprite(QString head_path, int num_frames, 
         QString tail_path, int rotate_angle = 0);

  /* Destructor function */
  ~Sprite();

  /* Public enumerators */
  enum { REVERSE, FORWARD };

private:
  /* The current frame */
  Frame* current;

  /* The first frame */
  Frame* head;

  /* The number of frames */
  int size;

  /* Direction */
  bool direction;
  
  /*------------------- Constants -----------------------*/
  const static int kDOUBLE_DIGITS;   /* the borderline to double digits */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the current frame */
  QPixmap getCurrent();

  /* Gets the current frame and then shifts to the next one */
  QPixmap getCurrentAndShift();

  /* Returns the position that the linked list pointer is at */
  int getPosition();

  /* Returns the size of the sequence */
  int getSize();

  /* Inserts the image into the sprite sequence at the given position */
  bool insert(QPixmap image, int position, int rotate_angle = 0);
  bool insert(QString image_path, int position, int rotate_angle = 0);

  /* Inserts the first image if the frame sequence is empty
   * Note: This isn't for inserting the head, just the first one */
  bool insertFirst(QPixmap image, int rotate_angle = 0);
  bool insertFirst(QString image_path, int rotate_angle = 0);

  /* Inserts a sequence of images that are stored. This allows for 
   * quick insertion of stored frames
   * For example: head_path = ":/animation/image_"
   *              num_frames = 5
   *              file_type = ".png"
   *   This will allow for image_00.png -> image_04.png to be added into
   *   a sequence */
  bool insertSequence(QString head_path, int num_frames, 
                      QString tail_path, int rotate_angle = 0);

  /* Inserts the image at the end of the sprite sequence */
  bool insertTail(QPixmap image, int rotate_angle = 0);
  bool insertTail(QString image_path, int rotate_angle = 0);

  /* Returns if the linked list pointer is currently at the head */
  bool isAtFirst();

  /* Removes the frame in the sequence at the given position */
  bool remove(int position);

  /* Removes all the frames in the sequence */
  bool removeAll();

  /* Removes the last frame in the sequence */
  bool removeTail();

  /* Rotates all the frames within this sprite a specific angle */
  bool rotateAll(int angle);

  /* Asserts the direction is forward for when accessing the linked list */
  bool setDirectionForward();

  /* Asserts the direction is reverse for when accessing the linked list */
  bool setDirectionReverse();

  /* Asserts that the current pointer in the linked list is at the head */
  bool setAtFirst();

  /* Shifts to the given position in the sequence */
  bool shift(int position);

  /* Shifts to the next frame in the sprite */
  bool shiftNext();

  /* Switches the direction that the linked list is parsed in */
  bool switchDirection();
};

#endif // SPRITE_H
