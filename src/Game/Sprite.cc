/******************************************************************************
* Class Name: Sprite
* Date Created: Dec 2 2012
* Inheritance: none
* Description: The Sprite class. This handles the linked list control that 
*              wraps the Frame. This will allow for a sequence of events, 
*              that emulate a GIF for animation or just store one image. This
*              class also has the functionality for direction GL painting 
*              through the native API.
*
* TODO:
*   1. Add reverse function: 1(head)->2->3->4  -->  4(head)->3->2->1
******************************************************************************/
#include "Game/Sprite.h"

/* Constant Implementation - see header file for descriptions */
const short Sprite::kDEFAULT_ANIMATE_TIME = 250;
const float Sprite::kDEFAULT_BRIGHTNESS = 1.0;
const short Sprite::kDOUBLE_DIGITS = 10;
const short Sprite::kUNSET_ANIMATE_TIME = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* 
 * Description: Constructor function - Set up no images
 *
 * Input: none
 */
Sprite::Sprite()
{
  animation_time = kUNSET_ANIMATE_TIME;
  brightness = kDEFAULT_BRIGHTNESS;
  current = 0;
  elapsed_time = 0;
  head = 0;
  size = 0;
  sequence = FORWARD;
}

/* 
 * Description: Constructor function - Set up one image, using the string path
 *              with an integer rotated angle.
 *
 * Input: QString image_path - image path to set as one sprite
 *        int rotate_angle - the degree angle to rotate the image at the path
 */
Sprite::Sprite(QString image_path, int rotate_angle)
{
  animation_time = kUNSET_ANIMATE_TIME;
  brightness = kDEFAULT_BRIGHTNESS;
  current = 0;
  elapsed_time = 0;
  head = 0;
  size = 0;
  sequence = FORWARD;
  insertFirst(image_path, rotate_angle);
}

/* 
 * Description: Constructor function - Set up sequence of images with an
 *              integer rotated angle.
 *
 * Input: QString head_path - the start part of the path
 *        int num_frames - the number of frames in this path sequence
 *        QString tail_path - the end of the path, after the count index
 *        int rotate_angle - the degree to rotate all the images at
 */
Sprite::Sprite(QString head_path, int num_frames, 
               QString tail_path, int rotate_angle)
{
  animation_time = kDEFAULT_ANIMATE_TIME;
  brightness = kDEFAULT_BRIGHTNESS;
  current = 0;
  elapsed_time = 0;
  head = 0; 
  size = 0;
  sequence = FORWARD;
  insertSequence(head_path, num_frames, tail_path, rotate_angle);  
}

/* 
 * Description: Destructor function 
 */
Sprite::~Sprite()
{
  removeAll();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* 
 * Description: Flips all the frames inside the sprite either along the 
 *              horizontal X axis or the vertical Y axis or both. 
 *              InitializeGl() required after this call since the image data
 *              has been changed.
 *
 * Inputs: bool horizontal - flip the frames along the horizontal axis
 *         bool vertical - flip the frames along the vertical axis
 * Output: bool - status if all the flips worked.
 */
bool Sprite::flipAll(bool horizontal, bool vertical)
{
  bool success = true;
  Frame* temp_frame = head;

  /* Only proceed if there are frames to rotate */
  if(size > 0)
  {
    for(int i = 0; i < size; i++)
    {
      success &= temp_frame->flipImage(horizontal, vertical);
      temp_frame = temp_frame->getNext();
    }

    return success;
  }

  return false;
}

/*
 * Description: Returns the animation time between frame changes on the sprite
 *              sequence.
 *
 * Inputs: none
 * Output: short - the animation time in milliseconds
 */
short Sprite::getAnimationTime()
{
  return animation_time;
}

/* 
 * Description: Gets the brightness value that the sprite sequence is being
 *              rendered at.
 *
 * Inputs: none
 * Output: float - the brightness indicator 
 *                (<1: darker, 1: default, >1: lighter)
 */
float Sprite::getBrightness()
{
  return brightness;
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

  QPixmap null_image;
  return null_image;
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

  QPixmap null_image;
  return null_image;
}

/*
 * Description: Returns the frame that marks the head of the sprite stack.
 *              Do not delete since this causes erradic results.
 *
 * Inputs: none
 * Output: Frame* - the frame pointer
 */
Frame* Sprite::getFirstFrame()
{
  return head;
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

/* Initializes GL in all the frames stored within this sprite */
/* 
 * Description: Iniatilizes all the GL painting in all the frames. Call this
 *              before starting the animation paint sequence. This will take
 *              care of all the necessary prep setup of a single sprite.
 *
 * Inputs: none
 * Output: bool - status if initialization was successful. If failed, there
 *                is no image data to initialize.
 */
bool Sprite::initializeGl()
{
  bool status = true;
  Frame* temp = head;

  /* Loop through all frames and initialize them all */
  for(int i = 0; i < size; i++)
  {
    status &= temp->initializeGl();
    temp = temp->getNext();
  }

  return status;
}

/* 
 * Description: Inserts the image into the sprite sequence at the given 
 *              position based on the given string path.
 *
 * Inputs: QString image_path - the path to the image to add
 *         int position - the location in the linked list sequence
 * Output: bool - status if insert is successful
 */
bool Sprite::insert(QString image_path, int position, int rotate_angle)
{
  Frame* next_frame;
  Frame* new_frame;
  Frame* previous_frame;

  /* Only add if the size is within the bounds of the sprite */
  if(size == 0)
  {
    return insertFirst(image_path, rotate_angle);
  }
  else if(position <= size && position >= 0)
  {
    new_frame = new Frame(image_path, rotate_angle);

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
      return true;
    }
    delete new_frame;
  }

  return false;
}

/* 
 * Description: Inserts the first image, based on the image path, if the frame 
 *              sequence is empty.
 * Note: This isn't for inserting the head, just the first one in an empty
 *       list.
 *
 * Inputs: QString image_path - the path to the image to insert
 * Output: bool - status if insertion was successful
 */
bool Sprite::insertFirst(QString image_path, int rotate_angle)
{
  if(size == 0)
  {
    head = new Frame(image_path, rotate_angle);
    if(head->isImageSet())
    {
      head->setNext(head);
      head->setPrevious(head);
      current = head;
      size = 1;
      return true;
    }

    delete head;
  }
  return false;
}

/* 
 * Description: Inserts a sequence of images that are stored. This allows for 
 * quick insertion of stored frames.
 * For example: head_path = ":/animation/image_"
 *              num_frames = 5
 *              tail_path = ".png"
 *   This will allow for image_00.png -> image_04.png to be added into
 *   a sequence 
 *
 * Inputs: See the above example.
 * Output: bool - status if insertion was succesful
 */
bool Sprite::insertSequence(QString head_path, int num_frames, 
		                        QString tail_path, int rotate_angle)
{
  bool status = true;

  /* Test if there are sufficient frames */
  if(num_frames <= 0)
    status = false;

  /* Store the initial condition */
  int old_size = size;

  /* Parse all the frames in the sequence */
  for(int i = 0; i < num_frames; i++)
  {
    if(i >= kDOUBLE_DIGITS)
      status = status & insertTail(head_path + QString::number(i) + tail_path,
                                   rotate_angle);
    else
      status = status & insertTail(head_path + "0" + QString::number(i) + 
		                               tail_path, rotate_angle);
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
 * Description: Inserts the image, based on the path, at the end of the sprite 
 *              sequence 
 *
 * Inputs: QString image_path - the path to the image to insert
 * Output: bool - status if insertion was successful
 */
bool Sprite::insertTail(QString image_path, int rotate_angle)
{
  return insert(image_path, size, rotate_angle);
}

/* 
 * Description: Checks if the linked list pointer is at the head of the list 
 *
 * Inputs: none
 * Output: bool - returns true if the current pointer is at the head
 */
bool Sprite::isAtFirst()
{
  if(head == current)
    return true;
  return false;
}

/*
 * Description: Checks if the linked list pointer is at the tail of the list
 *
 * Inputs: none
 * Output: bool - returns true if the current pointer is at the tail
 */
bool Sprite::isAtEnd()
{
  if(getSize() <= 1)
    return isAtFirst();
  return (head->getPrevious() == current);
}

/*
 * Description: Returns if the parsing direction through the frame sequence
 *              is forward. Otherwise, it is in reverse.
 *
 * Inputs: none
 * Output: bool - status if the direction is forward. False if reverse.
 */
bool Sprite::isDirectionForward()
{
  return (sequence == FORWARD);
}

/* 
 * Description: Paints the active frame using native GL calls. The context
 *              for GL must have been called for this and the sprite
 *              GL initialization must have occurred before any painting.
 *
 * Inputs: int x - the x offset in the plane (left-right)
 *         int y - the y offset in the plane (up-down)
 *         int width - the width to render the frame to
 *         int height - the height to render the frame to
 *         float opacity - the transparency of the paint object (0-1)
 * Output: bool - status if the frame was painted. If failed, make sure there
 *         is an image in the sprite and make sure initializeGl() was called.
 */
bool Sprite::paintGl(float x, float y, int width, int height, float opacity)
{
  if(current != 0)
    return current->paintGl(x, y, width, height, brightness, opacity);
  return false;
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

    return true;
  }
  
  return false;
}

/* 
 * Description: Removes all the frames in the sequence. Empties the sprite.
 *
 * Inputs: none
 * Output: bool - status if process was successful. If any faults, will return
 *                false. Also, if the sprite is already empty, it will return
 *                false.
 */
bool Sprite::removeAll()
{
  bool status = true;

  if(size == 0)
    status = false;

  while(size > 0)
    status = status & removeTail();

  return status;
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
 * Description: Rotates all the frames within the sprite to the given angle.
 *              Returns false if it fails or there are no frames.
 *
 * Inputs: int angle - the angle to rotate the frames to
 * Output: bool - the status of the rotation
 */
bool Sprite::rotateAll(int angle)
{
  Frame* temp_frame = head;

  /* Only proceed if there are frames to rotate */
  if(size > 0)
  {
    /* Only rotate the image if the angle is not 0 */
    if(angle != 0)
    {
      for(int i = 0; i < size; i++)
      {
        temp_frame->rotateImage(angle);
        temp_frame = temp_frame->getNext();
      }
    }

    return true;
  }

  return false;
}

/*
 * Description: Sets the animation time between frame changes. Gets called from
 *              the update call below for updating the frames in the sequence.
 *
 * Inputs: short time - the update time in milliseconds
 * Output: none
 */
void Sprite::setAnimationTime(short time)
{
  /* Set the new animation time */
  if(time < 0)
    animation_time = kUNSET_ANIMATE_TIME;
  animation_time = time;
  
  /* Reset the elapsed time */
  elapsed_time = 0;
}
  
/*
 * Description: Sets the brightness that the entire sprite sequence will be
 *              rendered at. It's range is 0-0.99: darker than default, 1.0: 
 *              default brightness, 1.0+: brighter.
 *
 * Inputs: float brightness - the brightness value (0+, 1.0 default)
 * Output: bool - if the set succeeded (brightness in proper range)
 */
bool Sprite::setBrightness(float brightness)
{
  /* Only allow the change if the brightness is a positive number */
  if(brightness >= 0.0)
  {
    this->brightness = brightness;
    return true;
  }
  
  return false;
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
  sequence = FORWARD;
  return true;
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
  sequence = REVERSE;
  return true;
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
  return true;
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

    return true;
  }

  return false;
}

/* 
 * Description: Shifts to the next frame in the sprite. 
 * Note: this function allows you to skip the head of the list while shifting.
 *       However, if the size of the sprite is only one frame, the head will
 *       still be selected.
 *
 * Inputs: bool skipHead - allows for the head of the list to be skipped
 *                         while shifting.
 * Output: bool - status if shift to next was successful
 */
bool Sprite::shiftNext(bool skipHead)
{
  if(size > 0)
  {
    if(sequence == FORWARD)
    {
      if(!skipHead || current->getNext() != head)
        current = current->getNext();
      else
        current = current->getNext()->getNext();
    }
    else
    {
      if(!skipHead || current->getPrevious() != head)
        current = current->getPrevious();
      else
        current = current->getPrevious()->getPrevious();
    }
    return true;
  }
  return false;
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
  if(sequence == FORWARD)
    return setDirectionReverse();
  return setDirectionForward();
}

/*
 * Description: Updates the sprite times for animation. Necessary for automated
 *              animation.
 *
 * Inputs: int cycle_time - the update time that has elapsed, in milliseconds
 * Output: none
 */
void Sprite::updateSprite(int cycle_time)
{
  if(size > 1 && cycle_time > 0 && animation_time > 0)
  {
    elapsed_time += cycle_time;
    if(elapsed_time > animation_time)
    {
      elapsed_time -= animation_time;
      shiftNext();
    }
    else if(elapsed_time < 0)
    {
      elapsed_time = 0;
    }
  }
}
  
/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the degree interpretation of a string representation
 *              for the 3 classifications. 
 *              90: Clockwise or CW
 *              180: Flip or F
 *              270: Counterclockwise or CCW
 *
 * Inputs: QString identifier - the angle string identity
 * Output: int - angle in degrees.
 */
int Sprite::getAngle(QString identifier)
{
  identifier = identifier.toLower().trimmed();

  if(identifier == "cw" || identifier == "clockwise")
    return getAngle(CLOCKWISE);
  else if(identifier == "ccw" || identifier == "counterclockwise")
    return getAngle(COUNTERCLOCKWISE);
  else if(identifier == "f" || identifier == "flip")
    return getAngle(HALFCIRCLE);
  return 0;
}

/*
 * Description: Returns the degree interpretation of the rotated angle enum
 *              for the 4 classifications. Can be used above for translating
 *              integer representation from the enumerator.
 *
 * Inputs: RotatedAngle angle - the angle enumerator to classify
 * Output: int - angle in degrees.
 */
int Sprite::getAngle(RotatedAngle angle)
{
  if(angle == CLOCKWISE)
    return 90;
  else if(angle == COUNTERCLOCKWISE)
    return -90;
  else if(angle == HALFCIRCLE)
    return 180;
  return 0;
}
