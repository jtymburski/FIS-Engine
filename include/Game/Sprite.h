/******************************************************************************
* Class Name: Sprite
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The Sprite class
******************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>

#include "Game/Frame.h"

class Sprite
{
public:
  /* Constructor function */
  Sprite();

  /* Destructor function */
  ~Sprite();

private:
  /* The current frame */
  Frame* current;

  /* The first frame */
  Frame* head;

  /* The number of frames */
  int size;

public:
  /* Inserts the image into the sprite sequence at the given position */
  bool insert(int pos, QImage img);

  /* Inserts the image at the end of the sprite sequence */
  bool insertTail(QImage img);

  /* Removes the frame in the sequence at the given position */
  bool remove(int pos);

  /* Removes the last frame in the sequence */
  bool removeTail();

  /* Shifts to the given position in the sequence */
  bool shift(int pos);

  /* Gets the current frame */
  QImage getCurrent();

  /* Gets the current frame and then shifts to the next one */
  QImage getCurrentAndShift();

  /* Returns the size of the sequence */
  int getSize();
};

#endif // SPRITE_H
