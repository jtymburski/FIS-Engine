/*******************************************************************************
 * Class Name: Frame
 * Date Created: December 2, 2012
 * Inheritance: none
 * Description: The Frame class, this represents an animation frame within the
 *              Sprite class. It acts as a linked list node, in that it contains
 *              a pointer to the next Frame in the sequence. The image frame is
 *              stored as a SDL_Surface, which can be converted to a texture
 *              to be rendered.
 ******************************************************************************/
#include "Game/Frame.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructor for this class. Empty parameter, default 
 *              initialization. Clears pointers and sets up the class
 *              for new data.
 *
 * Inputs: none
 */
Frame::Frame()
{
  /* Initialize variables */
  image_set = false;
  next = NULL;
  previous = NULL;
  surface = NULL;
}

/* 
 * Description: Constructor for this class. Takes path, next ptr, and previous
 *              ptr. Pointers are defaulted to NULL if not given.
 *
 * Inputs: std::string path - the path to the image to create
 *         Frame* previous - pointer to previous frame, default to NULL
 *         Frame* next - pointer to next frame, default to NULL
 */
Frame::Frame(std::string path, Frame* previous, Frame* next)
{
  /* Initialize variables */
  image_set = false;
  next = NULL;
  previous = NULL;
  surface = NULL;
  
  /* Sets the class parameters */
  setSurface(path);
  setPrevious(previous);
  setNext(next);
}

/* 
 * Description: Destructor function 
 */
Frame::~Frame()
{
  next = NULL;
  previous = NULL;
  unsetSurface();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* 
 * Description: Gets next frame pointed to by this node
 *
 * Inputs: none
 * Output: Frame* - pointer to next node
 */
Frame* Frame::getNext()
{
  return next;
}

/* 
 * Description: Gets previous frame pointed to by this node
 *
 * Inputs: none
 * Output: Frame* - pointer to previous node
 */
Frame* Frame::getPrevious()
{
  return previous;
}

/*
 * Description: Returns the SDL surface that the texture has been stored in.
 *              It is not necessarily NULL if the surface isn't set. Call 
 *              isImageSet() to determine if it's been set.
 *
 * Inputs: none
 * Output: SDL_Surface* - the SDL surface of the image
 */
SDL_Surface* Frame::getSurface()
{
  return surface;
}

/* 
 * Description: Returns if an image is stored in this frame
 *
 * Inputs: none
 * Output: bool - Status if an image is actually set
 */
bool Frame::isImageSet()
{
  return image_set;
}

/* 
 * Description: Sets next frame pointer
 *
 * Inputs: Frame* next - pointer to next frame, can be NULL
 * Output: bool - true if successful
 */
bool Frame::setNext(Frame* next)
{
  this->next = next;
  return true;
}

/* 
 * Description: Sets next frame pointer
 *
 * Inputs: Frame* previous - pointer to previous frame, can be NULL
 * Output: bool - true if successful
 */
bool Frame::setPrevious(Frame* previous)
{
  this->previous = previous;
  return true;
}

/*
 * Description: Sets the SDL frame surface from a path file. This requires that
 *              the extension appropriately defines the file in order to 
 *              properly work. If the image can be loaded, it automatically
 *              unsets the previous surface and sets this as the new one.
 *
 * Inputs: std::string path - the path to the image
 * Output: bool - the success of loading the surface
 */
bool Frame::setSurface(std::string path)
{
	/* Attempt to load the surface */
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  
  /* If successful, set the surface in the class */
  if(loaded_surface != NULL)
  {
    unsetSurface();
    surface = loaded_surface;
    image_set = true;
    return true;
  }
  
  /* Otherwise, return failed success */
  printf("[WARNING] Unable to load image \"%s\". SDL_image error: %s\n", 
         path.c_str(), IMG_GetError());
  return false;
}

/*
 * Description: Unsets the surface, if it has been stored in the class.
 *              Otherwise, this call does nothing. After called, there is no
 *              image data to render.
 *
 * Inputs: none
 * Output: none
 */
void Frame::unsetSurface()
{
  if(image_set)
  {
    SDL_FreeSurface(surface);
    surface = NULL;
    image_set = false;
  }
}
