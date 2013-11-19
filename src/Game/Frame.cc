/*******************************************************************************
 * Class Name: Frame
 * Date Created: December 2, 2012
 * Inheritance: none
 * Description: The Frame class, this represents an animation frame within the
 *              Sprite class. It acts as a linked list node, in that it contains
 *              a pointer to the next Frame in the sequence. The image frame is
 *              stored as a SDL_Texture which is used for rendering.
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
  height = 0;
  image_set = false;
  next = NULL;
  previous = NULL;
  texture = NULL;
  width = 0;
}

/* 
 * Description: Constructor for this class. Takes path, next ptr, and previous
 *              ptr. Pointers are defaulted to NULL if not given.
 *
 * Inputs: std::string path - the path to the image to create
 *         SDL_Renderer* renderer - the image renderer to handle the texture
 *         Frame* previous - pointer to previous frame, default to NULL
 *         Frame* next - pointer to next frame, default to NULL
 */
Frame::Frame(std::string path, SDL_Renderer* renderer, 
             Frame* previous, Frame* next)
{
  /* Initialize variables */
  height = 0;
  image_set = false;
  next = NULL;
  previous = NULL;
  texture = NULL;
  width = 0;
  
  /* Sets the class parameters */
  setTexture(path, renderer);
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
  unsetTexture();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the height of the texture stored in the class. Is 0 if
 *              the image isn't set.
 *
 * Inputs: none
 * Output: int - pixel height of image
 */
int Frame::getHeight()
{
  return height;
}

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
 * Description: Returns the SDL texture that is being stored in the frame.
 *              It is not necessarily NULL if the texture isn't set. Call 
 *              isImageSet() to determine if it's been set.
 *
 * Inputs: none
 * Output: SDL_Texture* - the SDL texture of the image
 */
SDL_Texture* Frame::getTexture()
{
  return texture;
}

/*
 * Description: Returns the width of the texture stored in the class. Is 0 if
 *              the image isn't set.
 *
 * Inputs: none
 * Output: int - pixel width of image
 */
int Frame::getWidth()
{
  return width;
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

/* Render the texture to the given renderer with the given parameters */
bool Frame::render(SDL_Renderer* renderer, int x, int y, int h, int w)
{
  if(isImageSet() && renderer != NULL)
  {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;

    /* Use parameter height and width if both are viable */
    if(h > 0 && w > 0)
    {
      rect.h = h;
      rect.w = w;
    }
    
    /* Render and return status */
    return (SDL_RenderCopy(renderer, getTexture(), NULL, &rect) == 0);
  }
  
  return false;
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
 * Description: Sets the SDL frame texture from a path file. This requires that
 *              the extension appropriately defines the file in order to 
 *              properly work. If the image can be loaded, it automatically
 *              unsets the previous texture and sets this as the new one.
 *
 * Inputs: std::string path - the path to the image
 *         SDL_Renderer* renderer - the renderer to associate the texture with
 * Output: bool - the success of loading the texture
 */
bool Frame::setTexture(std::string path, SDL_Renderer* renderer)
{
  bool success = true;
  
	/* Attempt to load the image */
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  
  /* If successful, unset previous and set the new texture */
  if(loaded_surface != NULL && renderer != NULL)
  {
    unsetTexture();
    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    image_set = true;
    height = loaded_surface->h;
    width = loaded_surface->w;
    SDL_FreeSurface(loaded_surface);
  }
  /* If the renderer is NULL, unload the surface */
  else if(loaded_surface != NULL)
  {
    SDL_FreeSurface(loaded_surface);
    printf("[WARNING] Renderer required to set texture in frame for \"%s\".\n",
           path.c_str());
    success = false;
  }
  /* If the surface is unset, notify the terminal with the failed surface */
  else
  {
    /* Otherwise, return failed success */
    printf("[WARNING] Unable to load image \"%s\". SDL_image error: %s\n", 
           path.c_str(), IMG_GetError());
    success = false;
  }
  
  return success;
}

/*
 * Description: Unsets the texture, if it has been stored in the class.
 *              Otherwise, this call does nothing. After called, there is no
 *              image data to render.
 *
 * Inputs: none
 * Output: none
 */
void Frame::unsetTexture()
{
  if(image_set)
  {
    SDL_DestroyTexture(texture);
    texture = NULL;
    image_set = false;
    height = 0;
    width = 0;
  }
}
