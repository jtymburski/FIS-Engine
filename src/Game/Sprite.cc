/******************************************************************************
* Class Name: Sprite
* Date Created: Dec 2 2012
* Inheritance: none
* Description: The Sprite class. This handles the linked list control that 
*              wraps the Frame. This will allow for a sequence of events, 
*              that emulate a GIF for animation or just store one image.
******************************************************************************/
#include "Game/Sprite.h"

/* 
 * Description: Constructor function - Set up no images
 *
 * Input: none
 */
Sprite::Sprite()
{
  head = 0;
  current = 0;
  size = 0;
  direction = FORWARD;
}

/* 
 * Description: Constructor function - Set up one image
 *
 * Input: none
 */
Sprite::Sprite(QString image_path)
{
  head = 0;
  current = 0;
  size = 0;
  direction = FORWARD;
  insertFirst(image_path);
}

/* 
 * Description: Constructor function - Set up sequence of images
 *
 * Input: none
 */
Sprite::Sprite(QString path_mask, int num_frames, QString file_type)
{
  head = 0;
  current = 0;
  size = 0;
  direction = FORWARD;
  insertSequence(path_mask, num_frames, file_type);  
}

/* 
 * Description: Destructor function 
 */
Sprite::~Sprite()
{
  while(size > 0)
    removeTail();
}

/* 
 * Description: Inserts the image into the sprite sequence at the given 
 *              position.
 *
 * Inputs: QString image_path - the path to the image to add
 *         int position - the location in the linked list sequence
 * Output: bool - status if insert is successful
 */
bool Sprite::insert(QString image_path, int position)
{
  Frame* next_frame;
  Frame* new_frame;
  Frame* previous_frame;

  /* Only add if the size is within the bounds of the sprite */
  if(size == 0)
  {
    return insertFirst(image_path);
  }
  else if(position <= size && position >= 0)
  {
    new_frame = new Frame(image_path);

    if(new_frame->isImageSet())
    {
      next_frame = head;

      /* Parse through to find where to insert */
      for(int i = 0; i < position; i++)
        next_frame = next_frame->getNext();
      previous_frame = next_frame->getPrevious();

      /* Reassign linked list pointers */
      previous_frame->setNext(new_frame);
      next_frame->setPrevious(new_frame);
      new_frame->setNext(next_frame);
      new_frame->setPrevious(previous_frame);

      /* If inserting to the front, reset the head pointer */
      if(position == 0)
        head = new_frame;

      size++;
      return TRUE;
    }
    delete new_frame;
  }

  return FALSE;
}

/* 
 * Description: Inserts the first image if the frame sequence is empty.
 * Note: This isn't for inserting the head, just the first one in an empty
 *       list.
 *
 * Inputs: QString image_path - the path to the image to insert
 * Output: bool - status if insertion was successful
 */
bool Sprite::insertFirst(QString image_path)
{
  if(size == 0)
  {
    head = new Frame(image_path);
    if(head->isImageSet())
    {
      head->setNext(head);
      head->setPrevious(head);
      current = head;
      size = 1;
      return TRUE;
    }
    delete head;
  }
  return FALSE;
}

/* 
 * Description: Inserts a sequence of images that are stored. This allows for 
 * quick insertion of stored frames.
 * For example: path_mask = ":/animation/image_"
 *              num_frames = 5
 *              file_type = "png"
 *   This will allow for image_0.png -> image_4.png to be added into
 *   a sequence 
 *
 * Inputs: See the above example.
 * Output: bool - status if insertion was succesful
 */
bool Sprite::insertSequence(QString path_mask, int num_frames, 
		                               QString file_type)
{
  bool status = TRUE;

  /* Test if there are sufficient frames */
  if(num_frames <= 0)
    status = FALSE;

  /* Store the initial condition */
  int old_size = size;

  /* Parse all the frames in the sequence */
  for(int i = 0; i < num_frames; i++)
  {
    if(i >= kDOUBLE_DIGITS)
      status = status & insertTail(path_mask + QString::number(i) + 
		                               "." + file_type);
    else
      status = status & insertTail(path_mask + "0" + QString::number(i) + 
		                                     "." + file_type);
  }

  /* If the sequence failed, delete the created pointers */
  if(!status)
  {
    while(size != old_size)
      removeTail();
  }

  return status;
}

/* 
 * Description: Inserts the image at the end of the sprite sequence 
 *
 * Inputs: QString image_path - the path to the image to insert
 * Output: bool - status if insertion was successful
 */
bool Sprite::insertTail(QString image_path)
{
  return insert(image_path, size);
}

/* 
 * Description: Checks if the linked list pointer is at the head of the list 
 *
 * Inputs: none
 * Output: bool - returns TRUE if the current pointer is at the head
 */
bool Sprite::isAtFirst()
{
  if(head == current)
    return TRUE;
  return FALSE;
}

/* 
 * Description: Removes the frame in the sequence at the given position 
 *
 * Inputs: int position - the position of the frame to remove in the linked
 *                        list.
 * Output: bool - status if removal was successful
 */
bool Sprite::remove(int position)
{
  Frame* old_frame;
  Frame* next_frame;
  Frame* previous_frame;

  /* Only remove if the position exists within the size boundaries */
  if(position < size && position >= 0)
  {
    old_frame = head;

    /* Find the location to delete */
    for(int i = 0; i < position; i++)
      old_frame = old_frame->getNext();
    next_frame = old_frame->getNext();
    previous_frame = old_frame->getPrevious();

    /* Reset linked list pointers */
    next_frame->setPrevious(previous_frame);
    previous_frame->setNext(next_frame);

    /* If at the front of the list, shift list backwards for new head */
    if(position == 0)
      head = next_frame;

    /* Delete frame and reduce the size after deletion */
    delete old_frame;
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

/* 
 * Description: Removes the last frame in the sequence 
 *
 * Inputs: none
 * Output: bool - status if tail removal was successful
 */
bool Sprite::removeTail()
{
  return remove(size-1);
}

/* 
 * Description: Shifts to the given position in the sequence 
 *
 * Inputs: int position - position to shift to in the linked list
 * Output: bool - status if moving in the linked list was successful
 */
bool Sprite::shift(int position)
{
  /* Only shift if the position is within the bounds of the sprite */
  if(position < size && position >= 0)
  {
    Frame* new_current_frame = head;

    for(int i = 0; i < position; i++)
      new_current_frame = new_current_frame->getNext();
    current = new_current_frame;

    return TRUE;
  }

  return FALSE;
}

/* 
 * Description: Shifts to the next frame in the sprite 
 *
 * Inputs: none
 * Output: bool - status if shift to next was successful
 */
bool Sprite::shiftNext()
{
  if(size > 0)
  {
    if(direction == FORWARD)
      current = current->getNext();
    else
      current = current->getPrevious();
    return TRUE;
  }
  return FALSE;
}

/* 
 * Description: Sets the direction that the linked list is navigated to
 *              opposite of what it was before. FORWARD -> REVERSE or
 *              REVERSE -> FORWARD.
 *
 * Inputs: none
 * Output: bool - status if direction set was successful
 */
bool Sprite::switchDirection()
{
  if(direction == FORWARD)
    return setDirectionReverse();
  return setDirectionForward();
}

/* 
 * Description: Gets the current frame 
 *
 * Inputs: none
 * Output: QPixmap - the current image in the list
 */
QPixmap Sprite::getCurrent()
{
  if(size > 0)
    return current->getImage();
  return NULL;
}

/* 
 * Description: Gets the current frame and then shifts to the next one 
 *
 * Inputs: none
 * Output: QPixmap - the current image in the list 
 */
QPixmap Sprite::getCurrentAndShift()
{
  if(size > 0)
  {
    QPixmap image = current->getImage();
    shiftNext();
    return image;
  }
  return NULL;
}

/* 
 * Description: Returns the position that the linked list is currently at
 *
 * Inputs: none
 * Output: int - the position in the linked list from the head
 */
int Sprite::getPosition()
{
  int location = 0;
  Frame* shifted = head;

  while(shifted != current)
  {
    shifted = shifted->getNext();
    location++;
  }
    
  return location;
}

/* 
 * Description: Returns the size of the sequence 
 *
 * Inputs: none
 * Output: int - the size of the sprite list
 */
int Sprite::getSize()
{
  return size;
}

/* 
 * Description: Sets the direction that the linked list is navigated to
 *              FORWARD. In other words, accessing the *next pointer when
 *              parsing it.
 *
 * Inputs: none
 * Output: bool - status if direction set was successful
 */
bool Sprite::setDirectionForward()
{
  direction = FORWARD;
  return TRUE;
}

/* 
 * Description: Sets the direction that the linked list is navigated to
 *              REVERSE. In other words, accessing the *previous pointer when
 *              parsing it.
 *
 * Inputs: none
 * Output: bool - status if direction set was successful
 */
bool Sprite::setDirectionReverse()
{
  direction = REVERSE;
  return TRUE;
}

/* 
 * Description: Set the linked list current pointer to the head of the list 
 *
 * Inputs: none
 * Output: bool - status if resetting the linked list to the first element
 *                was successful.
 */
bool Sprite::setAtFirst()
{
  current = head;
  return TRUE;
}
