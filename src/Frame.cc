/*******************************************************************************
 * Class Name: Frame
 * Date Created: December 2, 2012
 * Inheritance: none
 * Description: The Frame class, this represents an animation frame within the
 *              Sprite class. It acts as a linked list node, in that it contains
 *              a pointer to the next Frame in the sequence. The image frame is
 *              stored as a SDL_Texture which is used for rendering.
 ******************************************************************************/
#include "Frame.h"

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
  flip = SDL_FLIP_NONE;
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
 *         uint16_t angle - angle for frame sequence rotation (mod 90)
 *         Frame* previous - pointer to previous frame, default to NULL
 *         Frame* next - pointer to next frame, default to NULL
 */
Frame::Frame(std::string path, SDL_Renderer* renderer, uint16_t angle,
             Frame* previous, Frame* next) : Frame()
{
  setTexture(path, renderer, angle);
  setPrevious(previous);
  setNext(next);
}

/*
 * Description: Constructor function for this class. Takes path, adjustment
 *              stack, renderer, and pointers for other frames.
 *
 * Inputs: std::string path - the path to the image to create
 *         std::vector<std::string> adjustments - the flip adjustment stack
 *         SDL_Renderer* renderer - the image renderer to handle the texture
 *         uint16_t angle - angle for frame based rotation (must be mod 90)
 *         Frame* previous - pointer to previous frame, default to NULL
 *         Frame* next - pointer to next frame, default to NULL
 */
Frame::Frame(std::string path, std::vector<std::string> adjustments, 
             SDL_Renderer* renderer, uint16_t angle, Frame* previous, 
             Frame* next) : Frame()
{
  setTexture(path, renderer, angle);
  execImageAdjustments(adjustments);
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
 * Description: Executes an image adjustment based on string data that is stored
 *              within the file. Usually tied to the path of the sprite to 
 *              indicate any extra adjustments.
 *
 * Inputs: std::string adjustment - the adjustment string (VF, HF)
 *                                - VF, HF: horizontal or vertical flip
 * Output: bool - status if successful
 */
bool Frame::execImageAdjustment(std::string adjustment)
{
  bool success = true;
  
  /* Parse the adjustment and do the appropriate activity */
  if(adjustment == "VF" || adjustment == "vf")
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
bool Frame::execImageAdjustments(std::vector<std::string> adjustments)
{
  bool success = true;
  
  /* Run through all the adjustments and execute them */
  for(size_t i = 0; i < adjustments.size(); i++)
    success &= execImageAdjustment(adjustments[i]);
    
  return success;
}

/*
 * Description: Sets the horizontal flip of the rendering texture.
 *
 * Inputs: bool flip - true if the horizontal flip should occur from the default
 * Output: none
 */
void Frame::flipHorizontal(bool flip)
{
  /* Enables / Disables the horizontal flip */
  if(flip)
    this->flip = 
               static_cast<SDL_RendererFlip>(this->flip |  SDL_FLIP_HORIZONTAL);
  else
    this->flip = 
               static_cast<SDL_RendererFlip>(this->flip & ~SDL_FLIP_HORIZONTAL);
}

/*
 * Description: Sets the vertical flip of the rendering texture.
 *
 * Inputs: bool flip - true if the vertical flip should occur from the default
 * Output: none
 */
void Frame::flipVertical(bool flip)
{
  /* Enables / Disables the vertical flip */
  if(flip)
    this->flip = static_cast<SDL_RendererFlip>(this->flip | SDL_FLIP_VERTICAL);
  else
    this->flip = 
                 static_cast<SDL_RendererFlip>(this->flip & ~SDL_FLIP_VERTICAL);
}

/*
 * Description: Returns the flip rating of the frame (from SDL).
 *
 * Inputs: none
 * Output: SDL_RendererFlip - the rendering struct flip
 */
SDL_RendererFlip Frame::getFlip()
{
  return flip;
}
  
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

/*
 * Description: Renders the stored image if it is set and with a viable renderer
 *              using the coordinates for location. 
 *
 * Inputs: SDL_Renderer* renderer - the rendering context for the GPU
 *         int x - the x pixel location of the top left
 *         int y - the y pixel location of the top left
 *         int w - the width to render (in pixels)
 *         int h - the height to render (in pixels)
 * Output: bool - status if the render occurred
 */
bool Frame::render(SDL_Renderer* renderer, int x, int y, int w, int h)
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
    return (SDL_RenderCopyEx(renderer, getTexture(), NULL, 
                             &rect, 0, NULL, flip) == 0);
  }
  
  return false;
}

/* 
 * Description: Sets the rendering alpha modification. Needs to be set for each
 *              texture as this just emulates the call to SDL.
 *
 * Inputs: uint8_t alpha - the alpha rating (255 - fully opaque, 0 - invisible)
 * Output: none
 */
void Frame::setAlpha(uint8_t alpha)
{
  if(getTexture() != NULL)
    SDL_SetTextureAlphaMod(getTexture(), alpha);
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
 *         uint16_t angle - the angle to texture rotate (only works for mod 90)
 * Output: bool - the success of loading the texture
 */
bool Frame::setTexture(std::string path, SDL_Renderer* renderer, uint16_t angle)
{
  bool success = true;

	/* Attempt to load the image */
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  
  /* If successful, unset previous and set the new texture */
  if(loaded_surface != NULL && renderer != NULL)
  {
    /* Angle surface modification - only works for %90 angles */
    if(angle > 0 && loaded_surface->h == loaded_surface->w && 
       loaded_surface->format->BytesPerPixel == 4)
    {
      uint32_t* pixels = static_cast<uint32_t*>(loaded_surface->pixels);
      std::vector< std::vector<uint32_t> > pixels_original;

      /* Make a copy of the original pixels */
      for(int i = 0; i < loaded_surface->h; i++)
      {
        std::vector<uint32_t> pixels_row;
        
        for(int j = 0; j < loaded_surface->w; j++)
          pixels_row.push_back(pixels[i*loaded_surface->h + j]);
        pixels_original.push_back(pixels_row);
      }

      /* Shift the pixels */
      for(uint32_t i = 0; i < pixels_original.size(); i++)
      {
        for(uint32_t j = 0; j < pixels_original[i].size(); j++)
        {
          int index = 0;

          /* Do the shift, based on which angle to use */
          if(angle == 90)
            index = j*loaded_surface->w + (loaded_surface->h - i - 1);
          else if(angle == 180)
            index = (loaded_surface->w - i - 1)*loaded_surface->w + 
                    (loaded_surface->h - j - 1);
          else if(angle == 270)
            index = (loaded_surface->w - j - 1)*loaded_surface->w + i;
          
          pixels[index] = pixels_original[i][j];
        }
      }
    }
    
    /* Create the texture from the surface */
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
    std::cerr << "[WARNING] Renderer required to set texture in frame for \""
              << path.c_str() << "\"" << std::endl;;
    success = false;
  }
  /* If the surface is unset, notify the terminal with the failed surface */
  else
  {
    /* Otherwise, return failed success */
    std::cerr << "[WARNING] Unable to load image \"" << path 
              << "\". SDL_image error: " << IMG_GetError() << std::endl;
    success = false;
  }
  
  return success;
}

/*
 * Description: Sets the SDL frame texture from a path file. This requires that
 *              the extension appropriately defines the file in order to 
 *              properly work. If the image can be loaded, it automatically
 *              unsets the previous texture and sets this as the new one. It 
 *              also takes a stack of adjustments for flipping.
 *
 * Inputs: std::string path - the path to the image
 *         std::vector<std::string> adjustments - adjustment flip stack
 *         SDL_Renderer* renderer - the renderer to associate the texture with
 *         uint16_t angle - the angle to texture rotate (only works for mod 90)
 * Output: bool - the success of loading the texture
 */
bool Frame::setTexture(std::string path, std::vector<std::string> adjustments, 
                                         SDL_Renderer* renderer, uint16_t angle)
{
  bool success = true;

  success &= execImageAdjustments(adjustments);
  success &= setTexture(path, renderer, angle);
  
  return success;
}

/*
 * Description: Sets the texture using a pre-created texture pointer. Once this
 *              set occurs, it binds to that texture and will perform deletion
 *              when it is finished. Unexpected results will happen if deletion
 *              occurs outside of this class.
 *
 * Inputs: SDL_Texture* texture - the created texture pointer
 * Output: bool - status if the texture was set
 */
bool Frame::setTexture(SDL_Texture* texture)
{
  if(texture != NULL)
  {
    unsetTexture();
    
    /* Set the new texture and appropriate parameters */
    this->texture = texture;
    image_set = true;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    
    return true;
  }
  
  return false;
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
