/******************************************************************************
* Class Name: Sprite
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The Sprite class
******************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>
#include <QtGui/QWidget>

#include "Game/Frame.h"

class Sprite : public QWidget
{
public:
  /* Constructor function */
  Sprite(QWidget* parent = 0);

  /* Destructor function */
  ~Sprite();

private:
  /* The current frame */
  Frame* current;

  /* The first frame */
  Frame* head;

  /* The number of frames */
  int size;

protected:
  void paintEvent(QPaintEvent *);

public:
  /* Inserts the image into the sprite sequence at the given position */
  void insert(int pos, QImage img);

  /* Inserts the image at the end of the sprite sequence */
  void insertTail(QImage img);

  /* Removes the frame in the sequence at the given position */
  void remove(int pos);

  /* Removes the last frame in the sequence */
  void removeTail();

  /* Shifts to the given position in the sequence */
  void shift(int pos);

  /* Gets the current frame */
  QImage getCurrent();

  /* Gets the current frame and then shifts to the next one */
  QImage getCurrentAndShift();

  /* Returns the size of the sequence */
  int getSize();

};

#endif // SPRITE_H
