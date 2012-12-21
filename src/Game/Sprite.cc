/******************************************************************************
* Class Name: Sprite
* Date Created: Dec 2 2012
* Inheritance: none
* Description: The Sprite class. This handles the linked list control that 
*              wraps the Frame. This will allow for a sequence of events, 
*              that emulate a GIF for animation or just store one image.
******************************************************************************/
#include "Game/Sprite.h"

/* Constructor function */
Sprite::Sprite()
{
  head = 0;
  current = 0;
  size = 0;
}

/* Destructor function */
Sprite::~Sprite()
{
  while(size > 0)
    removeTail();
}

/* Inserts the image into the sprite sequence at the given position */
bool Sprite::insert(QString image_path, int position)
{
  Frame* new_frame;
  Frame* temp_frame;

  /* Only add if the size is within the bounds of the sprite */
  if(size == 0)
  {
    return insertFirst(image_path);
  }
  else if(position <= size)
  {
    new_frame = new Frame(image_path);
    temp_frame = head;

    /* If position is 0, redefining the head
     * Otherwise, just inserting in the middle or end */
    if(position == 0)
    {
      for(int i = 1; i < size; i++)
        temp_frame = temp_frame->getNext();
      new_frame->setNext(temp_frame->getNext());
      temp_frame->setNext(new_frame);
      head = new_frame;
    }
    else
    {
      for(int i = 1; i < position; i++)
        temp_frame = temp_frame->getNext();
      new_frame->setNext(temp_frame->getNext());
      temp_frame->setNext(new_frame);
    }

    size++;
    return TRUE;
  }

  return FALSE;
}
/* Inserts the first image if the frame sequence is empty
 * Note: This isn't for inserting the head, just the first one */
bool Sprite::insertFirst(QString image_path)
{
  head = new Frame(image_path);
  current = head;
  size = 1;
  return TRUE;
}

/* Inserts a sequence of images that are stored. This allows for 
 * quick insertion of stored frames
 * For example: path_mask = ":/animation/image_"
 *              num_frames = 5
 *              file_type = ".png"
 *   This will allow for image_0.png -> image_4.png to be added into
 *   a sequence */
bool Sprite::insertSequence(QString path_mask, int num_frames, 
		                               QString file_type)
{
  bool status = TRUE;

  for(int i = 0; i < num_frames; i++)
  {
    if(num_frames < 10 || (i >= 10))
      status = status & insertTail(path_mask + i + file_type);
    else
      status = status & insertTail(path_mask + "0" + i + file_type);
  }

  return status;
}

/* Inserts the image at the end of the sprite sequence */
bool Sprite::insertTail(QString image_path)
{
  return insert(image_path, size);
}

/* Removes the frame in the sequence at the given position */
bool Sprite::remove(int position)
{
  Frame* old_frame;
  Frame* temp_frame;

  /* Only remove if the position exists within the size boundaries */
  if(position < size)
  {
    temp_frame = head;

    /* If position is 0, redefining the head
     * Otherwise, just inserting in the middle or end */
    if(position == 0)
    {
      for(int i = 1; i < size; i++)
        temp_frame = temp_frame->getNext();
      old_frame = temp_frame->getNext();
      head = old_frame->getNext();
      temp_frame->setNext(head);
      delete old_frame;
    }
    else
    {
      for(int i = 1; i < position; i++)
        temp_frame = temp_frame->getNext();
      old_frame = temp_frame->getNext();
      temp_frame->setNext(old_frame->getNext());
      delete old_frame;
    }

    /* Reduce the size after deletion */
    size--;

    /* Reset the current frame based on deletion of frame */
    if(size == 0)
    {
      head = 0;
      current = 0;
    }
    else
    {
      current = head;
    }

    return TRUE;
  }
  
  return FALSE;
}

/* Removes the last frame in the sequence */
bool Sprite::removeTail()
{
  return remove(size-1);
}

/* Shifts to the given position in the sequence */
bool Sprite::shift(int position)
{
  /* Only shift if the position is within the bounds of the sprite */
  if(position < size)
  {
    Frame* new_current_frame = head;

    for(int i = 0; i < position; i++)
      new_current_frame = new_current_frame->getNext();
    current = new_current_frame;

    return TRUE;
  }
  else
  {
    return FALSE;
  }  
}

/* Shifts to the next frame in the sprite */
bool Sprite::shiftNext()
{
  current = current->getNext();
  return TRUE;
}

/* Gets the current frame */
QImage Sprite::getCurrent()
{
  return current->getImage();
}

/* Gets the current frame and then shifts to the next one */
QImage Sprite::getCurrentAndShift()
{
  QImage image = current->getImage();
  current = current->getNext();
  return image;
}

/* Returns the size of the sequence */
int Sprite::getSize()
{
    return size;
}
