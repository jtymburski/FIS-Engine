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

/* Enum declaration for direction of linked list */
enum { REVERSE, FORWARD };

class Sprite
{
public:
  /* Constructor: Set up no images */
  Sprite();

  /* Constructor: Set up one image */
  Sprite(QString image_path);

  /* Constructor: Set up sequence of images */
  Sprite(QString path_mask, int num_frames, QString file_type);

  /* Destructor function */
  ~Sprite();

private:
  /* The current frame */
  Frame* current;

  /* The first frame */
  Frame* head;

  /* The number of frames */
  int size;

  /* Direction */
  bool direction;

public:
  /* Inserts the image into the sprite sequence at the given position */
  bool insert(QString image_path, int position);

  /* Inserts the first image if the frame sequence is empty
   * Note: This isn't for inserting the head, just the first one */
  bool insertFirst(QString image_path);

  /* Inserts a sequence of images that are stored. This allows for 
   * quick insertion of stored frames
   * For example: path_mask = ":/animation/image_"
   *              num_frames = 5
   *              file_type = "png"
   *   This will allow for image_0.png -> image_4.png to be added into
   *   a sequence */
  bool insertSequence(QString path_mask, int num_frames, QString file_type);

  /* Inserts the image at the end of the sprite sequence */
  bool insertTail(QString image_path);

  /* Returns if the linked list pointer is currently at the head */
  bool isAtFirst();

  /* Removes the frame in the sequence at the given position */
  bool remove(int position);

  /* Removes the last frame in the sequence */
  bool removeTail();

  /* Shifts to the given position in the sequence */
  bool shift(int position);

  /* Shifts to the next frame in the sprite */
  bool shiftNext();

  /* Switches the direction that the linked list is parsed in */
  bool switchDirection();

  /* Gets the current frame */
  QPixmap getCurrent();

  /* Gets the current frame and then shifts to the next one */
  QPixmap getCurrentAndShift();

  /* Returns the position that the linked list pointer is at */
  int getPosition();

  /* Returns the size of the sequence */
  int getSize();

  /* Asserts the direction is forward for when accessing the linked list */
  bool setDirectionForward();

  /* Asserts the direction is reverse for when accessing the linked list */
  bool setDirectionReverse();

  /* Asserts that the current pointer in the linked list is at the head */
  bool setAtFirst();
};

#endif // SPRITE_H
