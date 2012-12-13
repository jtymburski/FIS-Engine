/******************************************************************************
* Class Name: Sprite
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Sprite class
******************************************************************************/
#include "Game/Sprite.h"

/* Constructor function */
Sprite::Sprite()
{
    size = 0;
}

/* Destructor function */
Sprite::~Sprite()
{
}

/* Inserts the image into the sprite sequence at the given position */
bool Sprite::insert(int pos, QImage img)
{
}

/* Inserts the image at the end of the sprite sequence */
bool Sprite::insertTail(QImage img)
{
}

/* Removes the frame in the sequence at the given position */
bool Sprite::remove(int pos)
{
}

/* Removes the last frame in the sequence */
bool Sprite::removeTail()
{
}

/* Shifts to the given position in the sequence */
bool Sprite::shift(int pos)
{
}

/* Gets the current frame */
QImage Sprite::getCurrent()
{
    QImage* image = new QImage();

    return *image;
}

/* Gets the current frame and then shifts to the next one */
QImage Sprite::getCurrentAndShift()
{
    QImage* image = new QImage();

    return *image;
}

/* Returns the size of the sequence */
int Sprite::getSize()
{
    return size;
}
