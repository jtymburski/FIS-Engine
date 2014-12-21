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

/* Constant Implementation - see header file for descriptions */
const uint8_t Frame::kDEFAULT_ALPHA = 255;

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
  alpha = kDEFAULT_ALPHA;
  flip = SDL_FLIP_NONE;
  grey_scale = false;
  height = 0;
  next = NULL;
  previous = NULL;
  texture = NULL;
  texture_grey = NULL;
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
 * Description: Returns the alpha rating of the frame. This is set whenever
 *              new textures are set.
 *
 * Inputs: none
 * Output: uint8_t - the alpha rating (0=invisible, 255=fully opaque)
 */
uint8_t Frame::getAlpha()
{
  return alpha;
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
 * Description: Returns the texture, based on the grey scale boolean. If true,
 *              it returns the grey scale texture and if false, returns the
 *              normal texture. NULL if unset.
 *
 * Inputs: bool grey_scale - boolean to choose grey scale texture or not
 * Output: SDL_Texture* - texture handle pointer
 */
SDL_Texture* Frame::getTexture(bool grey_scale)
{
  if(grey_scale)
    return texture_grey;
  return texture;
}

/*
 * Description: Returns the SDL texture that is being stored in the frame.
 *              It is not necessarily NULL if the texture isn't set. Call 
 *              isTextureSet() to determine if it's been set. isGreyScale()
 *              function indicates whether the returned texture is the greyscale
 *              version of the active texture or not.
 *
 * Inputs: none
 * Output: SDL_Texture* - the SDL texture of the image
 */
SDL_Texture* Frame::getTextureActive()
{
  if(grey_scale)
    return texture_grey;
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
 * Description: Checks if the grey scale texture is the active texture.
 *
 * Inputs: none
 * Output: bool - status if the grey scale texture is the active texture
 */
bool Frame::isGreyScale()
{
  return grey_scale;
}

/* 
 * Description: Returns if a texture is stored in this frame
 *
 * Inputs: bool grey_scale - set to true to check the grey scale texture
 * Output: bool - Status if an image is actually set
 */
bool Frame::isTextureSet(bool grey_scale)
{
  if(grey_scale)
    return (texture_grey != NULL);
  return (texture != NULL);
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
  if(isTextureSet(grey_scale) && renderer != NULL)
  {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;

    /* Use parameter height and width if both are viable */
    if(h > 0)
      rect.h = h;
    if(w > 0)
      rect.w = w;
    
    /* Render and return status */
    return (SDL_RenderCopyEx(renderer, getTextureActive(), NULL, 
                             &rect, 0, NULL, flip) == 0);
  }
  
  return false;
}

/*
 * Description: Renders the grey scale texture at the base and then renders the
 *              colored texture above that at the given alpha value. This is 
 *              used for blending the greyscale to color and vice versa. Fails
 *              if there is no grey scale texture.
 *
 * Inputs: SDL_Renderer* renderer - the rendering context for the GPU
 *         uint8_t alpha - the alpha rating of the colored texture
 *         int x - the x pixel location of the top left
 *         int y - the y pixel location of the top left
 *         int w - the width to render (in pixels)
 *         int h - the height to render (in pixels)
 * Output: bool - status if the render occurred
 */
bool Frame::renderBoth(SDL_Renderer* renderer, uint8_t alpha, int x, int y, 
                                                              int w, int h)
{
  if(isTextureSet() && isTextureSet(true) && renderer != NULL)
  {
    uint8_t old_alpha = getAlpha();
    bool grey_scale = isGreyScale();
    bool success = true;
    
    /* Render grey scale texture */
    useGreyScale(true);
    setAlpha(kDEFAULT_ALPHA - alpha);
    success &= render(renderer, x, y, w, h);
    
    /* Render colored texture */
    useGreyScale(false);
    setAlpha(alpha);
    success &= render(renderer, x, y, w, h);
    
    /* Return to normal parameters */
    useGreyScale(grey_scale);
    setAlpha(old_alpha);
    
    return success;
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
  this->alpha = alpha;
  SDL_SetTextureAlphaMod(texture, alpha);
  SDL_SetTextureAlphaMod(texture_grey, alpha);
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
 *         bool no_warnings - should warnings not fire? default false.
 *         bool enable_greyscale - should a greyscale texture be created?
 * Output: bool - the success of loading the texture
 */
bool Frame::setTexture(std::string path, SDL_Renderer* renderer, uint16_t angle, 
                       bool no_warnings, bool enable_greyscale)
{
  bool success = true;

	/* Attempt to load the image */
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  
  /* If successful, unset previous and set the new texture */
  if(loaded_surface != NULL && renderer != NULL)
  {
    /* Unset the previous texture */
    unsetTexture();
    
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
          pixels_row.push_back(pixels[i*loaded_surface->w + j]);
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
    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    height = loaded_surface->h;
    width = loaded_surface->w;
    
    /* Create the greyscale texture, if applicable */
    if(enable_greyscale && loaded_surface->format->BytesPerPixel == 4)
    {
      SDL_Surface* grey_surface = SDL_ConvertSurface(
                                     loaded_surface, loaded_surface->format, 0);
      
      /* Change all the pixels to greyscale */
      uint32_t* grey_pixels = static_cast<uint32_t*>(grey_surface->pixels);
      
      /* Parse each pixel and modify it's value */
      for(int i = 0; i < grey_surface->h; i++)
      { 
        for(int j = 0; j < grey_surface->w; j++)
        {
          uint32_t* pixel = &grey_pixels[i*loaded_surface->w + j];
          
          /* Get the color data */
          SDL_Color color;
          SDL_GetRGBA(*pixel, grey_surface->format, &color.r, &color.g, 
                                                    &color.b, &color.a);

          /* Modify the color data -> to greyscale */
          color.r = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b;
          color.g = color.r;
          color.b = color.r;
          
          /* Insert the new color data */
          *pixel = SDL_MapRGBA(grey_surface->format, color.r, color.g, 
                                                     color.b, color.a);
        }
      }
      
      /* Create greyscale texture and then clean up */
      texture_grey = SDL_CreateTextureFromSurface(renderer, grey_surface);
      SDL_FreeSurface(grey_surface);
    }
    
    /* Finally, set the alpha rating */
    setAlpha(alpha);
    
    /* Clean Up */
    SDL_FreeSurface(loaded_surface);
  }
  /* If the renderer is NULL, unload the surface */
  else if(loaded_surface != NULL)
  {
    SDL_FreeSurface(loaded_surface);
    if(!no_warnings)
      std::cerr << "[WARNING] Renderer required to set texture in frame for \""
                << path.c_str() << "\"" << std::endl;;
    success = false;
  }
  /* If the surface is unset, notify the terminal with the failed surface */
  else
  {
    /* Otherwise, return failed success */
    if(!no_warnings)
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
 *         bool no_warnings - should warnings not fire? default false.
 *         bool enable_greyscale - should a greyscale texture be created?
 * Output: bool - the success of loading the texture
 */
bool Frame::setTexture(std::string path, std::vector<std::string> adjustments, 
                                         SDL_Renderer* renderer, uint16_t angle,
                                         bool no_warnings, 
                                         bool enable_greyscale)
{
  bool success = true;

  success &= execImageAdjustments(adjustments);
  success &= setTexture(path, renderer, angle, no_warnings, enable_greyscale);
  
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
    uint32_t format;
    
    /* Clean up the existing texture */
    unsetTexture();
    
    /* Set the new texture and appropriate parameters */
    this->texture = texture;
    SDL_QueryTexture(texture, &format, NULL, &width, &height);
    
    setAlpha(alpha);
    
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
  /* Delete main texture */
  if(texture != NULL)
    SDL_DestroyTexture(texture);
  texture = NULL;
  
  /* Delete greyscale texture */
  if(texture_grey != NULL)
    SDL_DestroyTexture(texture_grey);
  texture_grey = NULL;
  
  /* Clear class parameters */
  grey_scale = false;
  height = 0;
  width = 0;
}

/*
 * Description: The enable to use the grey scale texture or the colored texture
 *              as the active one. If enable is true, the grey scale texture is
 *              selected and if false, the colored texture is selected. Fails
 *              if the grey scale texture is not set.
 *
 * Inputs: bool enable - true for grey scale texture, false for colored texture
 * Output: bool - status if set was successful
 */
bool Frame::useGreyScale(bool enable)
{
  if(enable && isTextureSet(true))
  {
    grey_scale = true;
    return true;
  }
  else if(!enable)
  {
    grey_scale = false;
    return true;
  }
  
  return false;
}

/*=============================================================================
 * PRIVATE STATIC FUNCTIONS
 *============================================================================*/
 
/*
 * Description: This draws a single horizontal line in SDL. It utilizes the
 *              SDL_RenderFillRect call since the drawLine with opacity leaves
 *              one double rendered pixel at the end.
 *
 * Inputs: int32_t x1 - one x coordinate
 *         int32_t x2 - the other x coordinate
 *         int32_t y - the y coordinate
 *         SDL_Renderer* renderer - the rendering pointer
 * Output: none
 */
void Frame::drawLine(int32_t x1, int32_t x2, int32_t y, 
                     SDL_Renderer* renderer)
{
  SDL_Rect rect;
  rect.x = x1;
  rect.y = y;
  rect.w = x2 - x1;
  rect.h = 1;

  SDL_RenderFillRect(renderer, &rect);
}

/*
 * Description: This renders an entire triangle but it requires that the bottom
 *              half of the triangle is horizontally flat (y2 and y3)
 *
 * Inputs: uint16_t x1 - first x coordinate
 *         uint16_t x2 - second x coordinate
 *         uint16_t x3 - third x coordinate
 *         uint16_t y1 - y coordinate for first x
 *         uint16_t y23 - y coordinate for second and third x
 *         SDL_Renderer* renderer - the rendering pointer
 *         bool aliasing - should aliasing be applied on edges?
 *         bool flat_side - should the flat side be aliased?
 * Output: none
 */
void Frame::renderBottomFlatTriangle(uint16_t x1, uint16_t x2, uint16_t x3, 
                                     uint16_t y1, uint16_t y23, 
                                     SDL_Renderer* renderer, bool aliasing,
                                     bool flat_side)
{
  /* Flip the x coordinates for the same y, to ensure positive */
  if(x2 > x3)
  {
    uint16_t temp_x = x2;
    x2 = x3;
    x3 = temp_x;
  }

  /* Get setup variables */
  float left_x = x1;
  float right_x = x1;
  float slope1 = (x2 - left_x) / (y23 - y1);
  float slope2 = (x3 - left_x) / (y23 - y1);
      
  /* Get alpha */
  uint8_t alpha = 255;
  uint8_t red = 255;
  uint8_t green = 255;
  uint8_t blue = 255;
  SDL_GetRenderDrawColor(renderer, &red, &green, &blue, &alpha);
  uint8_t half_alpha = alpha / 2;
  uint8_t quarter_alpha = half_alpha / 2;

  /* Draw the lines to display the triangle (only horizontal) */
  for(uint16_t y = y1; y <= y23; y++)
  {
    /* If aliasing, add pixel at half opacity at end */
    if(aliasing)
    {
      float diff_x = right_x - left_x;

      /* First, render the quarter alpha version pixel */
      SDL_SetRenderDrawColor(renderer, red, green, blue, quarter_alpha);
      if(diff_x >= -2 && diff_x <= 2)
      {
        Frame::drawLine((int)left_x, (int)right_x, y, renderer);
      }
      else
      {
        SDL_RenderDrawPoint(renderer, (int)left_x, y);
        SDL_RenderDrawPoint(renderer, (int)right_x - 1, y);
        if(!flat_side || y != y23)
          SDL_SetRenderDrawColor(renderer, red, green, blue, half_alpha);

        /* Next, render the half alpha pixel */
        if((diff_x >= -4 && diff_x <= 4) || (flat_side && y >= (y23 - 1)))
        {
          Frame::drawLine((int)left_x + 1, (int)right_x - 1, y, renderer);
        }
        else
        {
          SDL_RenderDrawPoint(renderer, (int)left_x + 1, y);
          SDL_RenderDrawPoint(renderer, (int)right_x - 2, y);

          /* Finally, render the full alpha central version */
          SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
          Frame::drawLine((int)left_x + 2, (int)right_x - 2, y, renderer);
        }
      }
    }
    /* Otherwise just draw full line */
    else
    {
      Frame::drawLine((int)left_x, (int)right_x, y, renderer);
    }

    /* Increment to next */
    left_x += slope1;
    right_x += slope2;
  }

  /* Reset color back to default */
  SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
}

/*
 * Description: This renders an entire triangle but it requires that the top
 *              half of the triangle is horizontally flat (y1 and y2)
 *
 * Inputs: uint16_t x1 - first x coordinate
 *         uint16_t x2 - second x coordinate
 *         uint16_t x3 - third x coordinate
 *         uint16_t y12 - y coordinate for first and second x
 *         uint16_t y3 - y coordinate for third x
 *         SDL_Renderer* renderer - the rendering pointer
 *         bool aliasing - should aliasing be applied on edges?
 *         bool flat_side - should the flat side be aliased?
 * Output: none
 */
void Frame::renderTopFlatTriangle(uint16_t x1, uint16_t x2, uint16_t x3, 
                                  uint16_t y12, uint16_t y3, 
                                  SDL_Renderer* renderer, bool aliasing,
                                  bool flat_side)
{
  /* Flip the x coordinates for the same y, to ensure positive */
  if(x1 > x2)
  {
    uint16_t temp_x = x1;
    x1 = x2;
    x2 = temp_x;
  }

  float left_x = x3;
  float right_x = x3;
  float slope1 = (left_x - x1) / (y3 - y12);
  float slope2 = (left_x - x2) / (y3 - y12);

  /* Get alpha */
  uint8_t alpha = 255;
  uint8_t red = 255;
  uint8_t green = 255;
  uint8_t blue = 255;
  SDL_GetRenderDrawColor(renderer, &red, &green, &blue, &alpha);
  uint8_t half_alpha = alpha / 2;
  uint8_t quarter_alpha = half_alpha / 2;
  
  /* Draw the lines to display the triangle (only horizontal) */
  for(uint16_t y = y3; y >= y12; y--)
  {
    /* If aliasing, add pixel at half opacity at end */
    if(aliasing)
    {
      float diff_x = right_x - left_x;

      /* First, render the quarter alpha version pixel */
      SDL_SetRenderDrawColor(renderer, red, green, blue, quarter_alpha);
      if(diff_x >= -2 && diff_x <= 2)
      {
        Frame::drawLine((int)left_x, (int)right_x, y, renderer);
      }
      else
      {
        SDL_RenderDrawPoint(renderer, (int)left_x, y);
        SDL_RenderDrawPoint(renderer, (int)right_x - 1, y);
        if(!flat_side || y != y12)
          SDL_SetRenderDrawColor(renderer, red, green, blue, half_alpha);

        /* Next, render the half alpha pixel */
        if((diff_x >= -4 && diff_x <= 4) || (flat_side && y <= (y12 + 1)))
        {
          Frame::drawLine((int)left_x + 1, (int)right_x - 1, y, renderer);
        }
        else
        {
          SDL_RenderDrawPoint(renderer, (int)left_x + 1, y);
          SDL_RenderDrawPoint(renderer, (int)right_x - 2, y);

          /* Finally, render the full alpha central version */
          SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
          Frame::drawLine((int)left_x + 2, (int)right_x - 2, y, renderer);
        }
      }
    }
    /* Otherwise just draw full line */
    else
    {
      Frame::drawLine((int)left_x, (int)right_x, y, renderer);
    }

    left_x -= slope1;
    right_x -= slope2;
  }
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
   
/*
 * Description: Renders a shifted rectangle bar. This allows for a slope to be
 *              defined for the left and right side which creates a symmetrical
 *              parrallelogram.
 *
 * Inputs: uint16_t x - the x coordinate of the top left corner of the bar
 *         uint16_t y - the y coordinate of the top left corner of the bar
 *         uint16_t length - distance horizontally of the bar
 *         uint16_t height - height of the bar
 *         float slope - the slope of the left and right sides
 *         SDL_Renderer* renderer - the rendering graphical engine
 * Output: bool - did the object render?
 */
bool Frame::renderBar(uint16_t x, uint16_t y, uint16_t length, 
                      uint16_t height, float slope, SDL_Renderer* renderer)
{
  /* Prechecks */
  if(renderer != NULL && length > 0 && height > 0)
  {
    for(uint16_t i = 0; i < height; i++)
    {
      uint16_t modified_x = x - i * slope;
      if(length == 1)
        SDL_RenderDrawPoint(renderer, modified_x, y);
      else
        Frame::drawLine(modified_x, modified_x + length, y, renderer);
      y++;
    }
    return true;
  }
  return false;
}

/*
 * Description: Renders a circle, with center x and y and radius. The option can
 *              be selected for a filled vs. a non-filled circle.
 *
 * Inputs: uint16_t center_x - the center x coordinate of circle
 *         uint16_t center_y - the center y coordinate of the circle
 *         uint16_t radius - the radius of the circle
 *         SDL_Renderer* renderer - the rendering graphical engine
 *         bool filled - should the circle be filled?
 * Output: bool - was the circle rendered?
 */
bool Frame::renderCircle(uint16_t center_x, uint16_t center_y, uint16_t radius, 
                         SDL_Renderer* renderer, bool filled)
{
  /* Prechecks */
  if(renderer != NULL && radius > 0)
  {
    /* Variable setup */
    int progress = 1 - radius;
    int step_x = 1;
    int step_y = -2 * radius;
    int top_left_y = center_y - radius;
    int x = 0;
    int y = radius;
    std::vector<bool> y_levels;

    /* Set up the vertical y levels to the height of the circle */
    y_levels.push_back(false);
    for(uint16_t i = 0; i < radius; i++)
    {
      y_levels.push_back(false);
      y_levels.push_back(false);
    }

    /* Render the farthest points (not part of the algorithm) */
    SDL_RenderDrawPoint(renderer, center_x, center_y + radius);
    SDL_RenderDrawPoint(renderer, center_x, center_y - radius);
    if(filled)
    {
      Frame::drawLine(center_x - radius, center_x + radius, center_y, renderer);
    }
    else
    {
      SDL_RenderDrawPoint(renderer, center_x + radius, center_y);
      SDL_RenderDrawPoint(renderer, center_x - radius, center_y);
    }

    /* Render through the guts of the circle */
    while(x < y)
    {
      bool y_changed = false;

      if(progress >= 0)
      {
        y--;
        step_y += 2;
        progress += step_y;

        y_changed = true;
      }

      x++;
      step_x += 2;
      progress += step_x;

      /* Render the arc */
      if(filled)
      {
        /* Get calculation values for top/bottom half */
        if(y_changed)
        {
          /* Calculate the line(s) */
          int y1 = center_y + y;
          int y2 = center_y - y;
          uint16_t delta_y1 = y1 - top_left_y;
          uint16_t delta_y2 = y2 - top_left_y;
        
          /* Only render the line if it hasn't been rendered */
          if(!y_levels[delta_y1])
          {
            Frame::drawLine(center_x - x, center_x + x, y1, renderer);
            y_levels[delta_y1] = true;
          }
          if(!y_levels[delta_y2])
          {
            Frame::drawLine(center_x - x, center_x + x, y2, renderer);
            y_levels[delta_y2] = true;
          }
        }

        /* Get calculation values for middle half line(s) */
        int y1 = center_y + x;
        int y2 = center_y - x;
        uint16_t delta_y1 = y1 - top_left_y;
        uint16_t delta_y2 = y2 - top_left_y;

        /* Only render the line if it hasn't been rendered */
        if(!y_levels[delta_y1])
        {
          Frame::drawLine(center_x - y, center_x + y, center_y + x, renderer);
          y_levels[delta_y1] = true;
        }
        if(!y_levels[delta_y2])
        {
          Frame::drawLine(center_x - y, center_x + y, center_y - x, renderer);
          y_levels[delta_y2] = true;
        }
      }
      else
      {
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
      }
    }
    return true;
  }
  return false;
}

/*
 * Description: Creates a right hand triangle, given the input parameters. The
 *              default right hand triangle with have its flat edges on the left
 *              and bottom sides. Reverse will put one of the flat edges
 *              on the right instead of left.
 *
 * Inputs: uint32_t x - the x coordinate for the top left
 *         uint32_t y - the y coordinate for the top left
 *         uint16_t height - the height/width of the unilateral triangle
 *         SDL_Renderer* renderer - the rendering engine pointer
 *         bool reverse - reverse the triangle?
 * Output: bool - was the triangle rendered?
 */
bool Frame::renderRHTriangle(uint32_t x, uint32_t y, uint16_t height, 
                             SDL_Renderer* renderer, bool reverse)
{
  if(reverse)
  {
    return Frame::renderTriangle(x, y, x, y + height - 1, x - height, y + 
                                 height - 1, renderer);
  }
  return renderTriangle(x, y, x, y + height - 1, x + height, y + height - 1, 
                        renderer);
}

/*
 * Description: Renders a filled triangle, given 3 vectors and a rendering 
 *              engine. Aliasing can be enabled on all edges. Utilizes private
 *              static functions for top and bottom half of triangle.
 *
 * Inputs: uint16_t x1 - the first x coordinate
 *         uint16_t y1 - the first y coordinate
 *         uint16_t x2 - the second x coordinate
 *         uint16_t y2 - the second y coordinate
 *         uint16_t x3 - the third x coordinate
 *         uint16_t y3 - the third y coordinate
 *         SDL_Renderer* renderer - the rendering engine pointer
 *         bool aliasing - should aliasing be used on the edges?
 * Output: bool - was the triangle rendered?
 */
bool Frame::renderTriangle(uint16_t x1, uint16_t y1, uint16_t x2, 
                           uint16_t y2, uint16_t x3, uint16_t y3, 
                           SDL_Renderer* renderer, bool aliasing)
{
  /* Only attempt to paint the triangle if renderer is valid */
  if(renderer != NULL)
  {
    bool success = true;
    int32_t x12_diff = x2 - x1;
    int32_t y12_diff = y2 - y1;
    int32_t x13_diff = x3 - x1;
    int32_t y13_diff = y3 - y1;

    /* Check if the triangle is a valid triangle */
    if((x1 == x2 && y1 == y2) || (x1 == x3 && y1 == y3) || 
       (x2 == x3 && y2 == y3) || (y12_diff == 0 && y12_diff == y13_diff) || 
       (y12_diff != 0 && y13_diff != 0 && 
        ((float)x12_diff / y12_diff) == ((float)x13_diff / y13_diff)))
    {
      success = false;
    }

    /* Attempt to render the triangle */
    if(success)
    {
      uint16_t temp_x;
      uint16_t temp_y;

      /* Get alpha */
      uint8_t alpha = 255;
      SDL_GetRenderDrawColor(renderer, NULL, NULL, NULL, &alpha);

      /* Sort the vertices ascending by y */
      if(y2 < y1)
      {
        temp_x = x1;
        temp_y = y1;
        x1 = x2;
        y1 = y2;
        x2 = temp_x;
        y2 = temp_y;
      }
      if(y3 < y2)
      {
        temp_x = x2;
        temp_y = y2;
        x2 = x3;
        y2 = y3;
        x3 = temp_x;
        y3 = temp_y;

        if(y2 < y1)
        {
          temp_x = x1;
          temp_y = y1;
          x1 = x2;
          y1 = y2;
          x2 = temp_x;
          y2 = temp_y;
        }
      }

      /* Check for arbitrary case of flat bottom */
      if(y2 == y3)
      {
        renderBottomFlatTriangle(x1, x2, x3, y1, y3, renderer, aliasing, true);
      }
      /* Check for arbitrary case of flat top */
      else if(y1 == y2)
      {
        renderTopFlatTriangle(x1, x2, x3, y1, y3, renderer, aliasing, true);
      }
      else
      {
        uint16_t x4 = (x1 + ((float)y2 - y1) / (float)(y3 - y1) * (x3 - x1));
        renderBottomFlatTriangle(x1, x2, x4, y1, y2, renderer, aliasing);
        renderTopFlatTriangle(x2, x4, x3, y2 + 1, y3, renderer, aliasing);
      }
    }

    return success;
  }
  return false;
}
