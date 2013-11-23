/*******************************************************************************
 * Class Name: Sprite
 * Date Created: Oct 28, 2012
 * Inheritance: none
 * Description: The Sprite class. This handles the linked list control that 
 *              wraps the Frame. This will allow for a sequence of events, 
 *              that emulate a GIF for animation or just store one image. This
 *              class also has the functionality for modded rendering through
 *              the SDL engine. The list of these mods are brightening or
 *              darkening, opacity, and color tuning.
 *
 * TODO:
 *   1. Add reverse function: 1(head)->2->3->4  -->  4(head)->3->2->1
*******************************************************************************/
#include "Sprite.h"

/* Constant Implementation - see header file for descriptions */
const short Sprite::kDEFAULT_ANIMATE_TIME = 250;
const float Sprite::kDEFAULT_BRIGHTNESS = 1.0;
const uint8_t Sprite::kDEFAULT_COLOR = 255;
const uint8_t Sprite::kDEFAULT_OPACITY = 255;
const short Sprite::kDOUBLE_DIGITS = 10;
const short Sprite::kUNSET_ANIMATE_TIME = -1;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* 
 * Description: Constructor function - Set up no frames
 *
 * Input: none
 */
Sprite::Sprite()
{
  /* Reset the class parameters */
  animation_time = kDEFAULT_ANIMATE_TIME;
  brightness = kDEFAULT_BRIGHTNESS;
  color_red = kDEFAULT_COLOR;
  color_green = kDEFAULT_COLOR;
  color_blue = kDEFAULT_COLOR;
  current = NULL;
  elapsed_time = 0;
  head = NULL;
  flip = SDL_FLIP_NONE;
  opacity = kDEFAULT_OPACITY;
  rotation_angle = 0;
  size = 0;
  sequence = FORWARD;
  texture = NULL;
  texture_update = false;
  white_mask = NULL;
}

/* 
 * Description: Constructor function - Set up one image, using the string path
 *              with an integer rotated angle.
 *
 * Input: QString image_path - image path to set as one sprite
 *        int rotate_angle - the degree angle to rotate the image at the path
 */
Sprite::Sprite(std::string path, SDL_Renderer* renderer) : Sprite()
{
  /* See if the tile needs a split sequence - do it if relevant */
  std::vector<std::string> split_path;
  Helpers::split(path, '|', split_path);
  if(split_path.size() == 3)
  {
    insertSequence(split_path[0], std::stoi(split_path[1]), 
                   split_path[2], renderer);
  }
  else
  {
    insertFirst(path, renderer);
  }
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
Sprite::Sprite(std::string head_path, int num_frames,
               std::string tail_path, SDL_Renderer* renderer) : Sprite()
{
  insertSequence(head_path, num_frames, tail_path, renderer);
}

/* 
 * Description: Destructor function 
 */
Sprite::~Sprite()
{
  removeAll();
  
  SDL_DestroyTexture(texture);
  texture = NULL;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Sets the color modification with the texture */
void Sprite::setColorMod()
{
  if(brightness < kDEFAULT_BRIGHTNESS)
  {
    SDL_SetTextureColorMod(texture, brightness * color_red, 
                           brightness * color_green, brightness * color_blue);
  }
  else
  {
    SDL_SetTextureColorMod(texture, color_red, color_green, color_blue);
  }
}
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Executes an image adjustment based on string data that is stored
 *              within the file. Usually tied to the path of the sprite to 
 *              indicate any extra adjustments.
 *
 * Inputs: std::string adjustment - the adjustment string (90, 180, 270, VF, HF)
 *                                - 90, 180, 270: angle of rotation
 *                                - VF, HF: horizontal or vertical flip
 * Output: bool - status if successful
 */
bool Sprite::execImageAdjustment(std::string adjustment)
{
  bool success = true;
  
  /* Parse the adjustment and do the appropriate activity */
  if(adjustment == "90" || adjustment == "180" || adjustment == "270")
  {
    setRotation(getAngle(adjustment));
  }
  else if(adjustment == "VF" || adjustment == "vf")
  {
    flipVertical();
  }
  else if(adjustment == "HF" || adjustment == "hf")
  {
    flipHorizontal();
  }
  else
  {
    success = false;
  }
  
  return success;
}

/*
 * Description: Executes a set of image adjustments using a list of strings.
 *              See execImageAdjustment() for more details
 *
 * Inputs: vector<string> adjustments - a stack of all adjustments
 * Output: bool - status if successful
 */
bool Sprite::execImageAdjustments(std::vector<std::string> adjustments)
{
  bool success = true;
  
  /* Run through all the adjustments and execute them */
  for(size_t i = 0; i < adjustments.size(); i++)
    success &= execImageAdjustment(adjustments[i]);
    
  return success;
}

/* Flips the sprite SDL image - either horizontal or vertical */
void Sprite::flipHorizontal(bool flip)
{
  /* Enables / Disables the horizontal flip */
  if(flip)
    this->flip = 
               static_cast<SDL_RendererFlip>(this->flip |  SDL_FLIP_HORIZONTAL);
  else
    this->flip = 
               static_cast<SDL_RendererFlip>(this->flip & ~SDL_FLIP_HORIZONTAL);
}

/* Flips the sprite SDL image - either horizontal or vertical */
void Sprite::flipVertical(bool flip)
{
  /* Enables / Disables the vertical flip */
  if(flip)
    this->flip = static_cast<SDL_RendererFlip>(this->flip | SDL_FLIP_VERTICAL);
  else
    this->flip = 
                 static_cast<SDL_RendererFlip>(this->flip & ~SDL_FLIP_VERTICAL);
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
 * Description: Gets the current frame that is active.
 *
 * Inputs: none
 * Output: Frame* - the current frame where the sprite is at
 */
Frame* Sprite::getCurrent()
{
  return current;
}

/* 
 * Description: Gets the current frame and then shifts to the next one 
 *
 * Inputs: none
 * Output: Frame* - the frame before the shift occurred 
 */
Frame* Sprite::getCurrentAndShift()
{
  Frame* previous = current;
  shiftNext();

  return previous;
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

/* 
 * Description: Inserts the image into the sprite sequence at the given 
 *              position based on the given string path.
 *
 * Inputs: std::string path - the path to the image to add
 *         SDL_Renderer* renderer - the rendering engine pointer
 *         int position - the location in the linked list sequence
 * Output: bool - status if insert is successful
 */
bool Sprite::insert(std::string path, SDL_Renderer* renderer, int position)
{
  Frame* next_frame;
  Frame* new_frame;
  Frame* previous_frame;

  /* Only add if the size is within the bounds of the sprite */
  if(size == 0)
  {
    return insertFirst(path, renderer);
  }
  else if(position <= size && position >= 0)
  {
    new_frame = new Frame(path, renderer);

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
 * Inputs: std::string path - the path to the image to add
 *         SDL_Renderer* renderer - the rendering engine pointer
 * Output: bool - status if insertion was successful
 */
bool Sprite::insertFirst(std::string path, SDL_Renderer* renderer)
{
  if(size == 0)
  {
    head = new Frame(path, renderer);
    if(head->isImageSet())
    {
      /* First set the rendering texture, if unset */
      if(texture == NULL)
      {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                    SDL_TEXTUREACCESS_TARGET, head->getWidth(), 
                                    head->getHeight());
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        setColorMod();
        setOpacity(opacity);
      }
      
      /* Only proceed with finishing if the rendering texture could be set */
      if(texture != NULL)
      {
        head->setNext(head);
        head->setPrevious(head);
        current = head;
        size = 1;
        texture_update = true;
      
        return true;
      }
    }

    delete head;
    head = NULL;
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
 * Inputs: std::string head_path - see above for explanation
 *         int num_frames - see above for explanation
 *         std::string tail_path - see above for explanation
 *         SDL_Renderer* renderer - the rendering engine pointer
 * Output: bool - status if insertion was succesful
 */
bool Sprite::insertSequence(std::string head_path, int num_frames, 
		                        std::string tail_path, SDL_Renderer* renderer)
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
      status = status & insertTail(head_path + std::to_string(i) + tail_path,
                                   renderer);
    else
      status = status & insertTail(head_path + "0" + std::to_string(i) + 
		                               tail_path, renderer);
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
 * Inputs: std::string path - the path to the image to add
 *         SDL_Renderer* renderer - the rendering engine pointer
 * Output: bool - status if insertion was successful
 */
bool Sprite::insertTail(std::string path, SDL_Renderer* renderer)
{
  return insert(path, renderer, size);
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
    texture_update = true;
    
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

/* Render the texture to the given renderer with the given parameters */
bool Sprite::render(SDL_Renderer* renderer, int x, int y, int h, int w)
{
  if(current != NULL && renderer != NULL)
  {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = current->getHeight();
    rect.w = current->getWidth();

    /* Use parameter height and width if both are viable */
    if(h > 0 && w > 0)
    {
      rect.h = h;
      rect.w = w;
    }

    /* Render and return status */
    return (SDL_RenderCopyEx(renderer, texture, NULL, &rect, 
                             rotation_angle, NULL, flip) == 0);
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
 * Description: Set the linked list current pointer to the head of the list 
 *
 * Inputs: none
 * Output: bool - status if resetting the linked list to the first element
 *                was successful.
 */
bool Sprite::setAtFirst()
{
  current = head;
  texture_update = true;
  return true;
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
    setColorMod();
    texture_update = true;
    
    return true;
  }
  
  return false;
}

/* Sets the color balance of the sprite */
void Sprite::setColorBalance(uint8_t red, uint8_t green, uint8_t blue)
{
  color_red = red;
  color_green = green;
  color_blue = blue;
  
  setColorMod();
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

void Sprite::setOpacity(uint8_t opacity)
{
  this->opacity = opacity;
  SDL_SetTextureAlphaMod(texture, opacity);
}

/*
 * Description: Sets the rotation that the frames will be rendered at around
 *              the center point.
 *
 * Inputs: int angle - the angle in degrees to rotate the frames to
 * Output: none
 */
void Sprite::setRotation(int angle)
{
  rotation_angle = angle;
}

bool Sprite::setWhiteMask(SDL_Texture* texture)
{
  if(texture != NULL)
  {
    white_mask = texture;
    return true;
  }
  
  return false;
}

/* 
 * Description: Shifts to the given position in the sequence 
 *
 * Inputs: int position - position to shift to in the linked list
 * Output: bool - status if moving in the linked list was successful
 */
bool Sprite::shift(int position)
{
  Frame* old_current = current;
  
  /* Only shift if the position is within the bounds of the sprite */
  if(position < size && position >= 0)
  {
    Frame* new_current_frame = head;

    for(int i = 0; i < position; i++)
      new_current_frame = new_current_frame->getNext();
    current = new_current_frame;
    
    /* Only update the texture if the current frame has changed */
    if(old_current != current)
      texture_update = true;
    
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
  Frame* old_current = current;

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
    
    /* Only update the texture if the current frame has changed */
    if(old_current != current)
      texture_update = true;
    
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
void Sprite::updateSprite(int cycle_time, SDL_Renderer* renderer)
{
  /* Start by updating the animation and shifting, if necessary */
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
  
  /* Proceed to update the running texture if it's changed */
  if(texture_update && size > 0)
  {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    
    /* Render current frame */
    current->render(renderer);
    
    /* Render white mask, if relevant */
    if(brightness > kDEFAULT_BRIGHTNESS && white_mask != NULL)
    {
      double bright_mod = (brightness - kDEFAULT_BRIGHTNESS);
      if(bright_mod > kDEFAULT_BRIGHTNESS)
        bright_mod = kDEFAULT_BRIGHTNESS;
      
      SDL_SetTextureAlphaMod(white_mask, 255 * brightness);
      SDL_RenderCopy(renderer, white_mask, NULL, NULL);
    }
    
    /* Release the renderer and end the update */
    SDL_SetRenderTarget(renderer, NULL);
    texture_update = false;
  }
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the degree interpretation of a string representation
 *              for the 3 classifications. 
 *              90, clockwise: "90", CW, or cw
 *              180, half circle: "180", HC, or hc
 *              270, counter clockwise: "270", CCW, or ccw
 *
 * Inputs: std::string identifier - the angle string identity
 * Output: int - angle in degrees.
 */
int Sprite::getAngle(std::string identifier)
{
  if(identifier == "90" || identifier == "CW" || identifier == "cw")
    return getAngle(CLOCKWISE);
  else if(identifier == "270" || identifier == "CCW" || identifier == "ccw")
    return getAngle(COUNTERCLOCKWISE);
  else if(identifier == "180" || identifier == "HC" || identifier == "hc")
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
