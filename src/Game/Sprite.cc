/******************************************************************************
* Class Name: Sprite
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Sprite class
******************************************************************************/
#include "Game/Player/Sprite.h"

/* Constructor function */
Sprite::Sprite(QWidget *parent = 0)
{
}

/* Destructor function */
Sprite::~Sprite()
{
}

/* Painting event */
void Sprite::paintEvent(QPaintEvent* event)
{
}

/* Inserts the image into the sprite sequence at the given position */
void Sprite::insert(int pos, QImage img)
{
}

/* Inserts the image at the end of the sprite sequence */
void Sprite::insertTail(QImage img)
{
}

/* Removes the frame in the sequence at the given position */
void Sprite::remove(int pos)
{
}

/* Removes the last frame in the sequence */
void Sprite::removeTail()
{
}

/* Shifts to the given position in the sequence */
void Sprite::shift(int pos)
{
}

/* Gets the current frame */
QImage Sprite::getCurrent()
{
    return;
}

/* Gets the current frame and then shifts to the next one */
QImage Sprite::getCurrentAndShift()
{
    return;
}

/* Returns the size of the sequence */
int Sprite::getSize()
{
    return size;
}
