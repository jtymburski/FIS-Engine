/*******************************************************************************
 * Class Name: Frame
 * Date Created: December 2, 2012
 * Inheritance: none
 * Description: The Frame class, this represents an animation frame within the
 *              Sprite class. It acts as a linked list node, in that it contains
 *              a pointer to the next Frame in the sequence. The image frame is
 *              stored as a SDL_Texture which is used for rendering.
 ******************************************************************************/
#ifndef FRAME_H
#define FRAME_H

#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

class Frame
{
public:
  /* Constructor function - empty initialization */
  Frame();

  /* Constructor function - path and next pointer initialization */
  Frame(std::string path, SDL_Renderer* renderer, uint16_t angle = 0, 
        Frame* previous = NULL, Frame* next = NULL);

  /* Constructor function - path, adjustments, and previous/next inits */
  Frame(std::string path, std::vector<std::string> adjustments, 
        SDL_Renderer* renderer, uint16_t angle = 0, Frame* previous = NULL, 
        Frame* next = NULL);
  
  /* Destructor function */
  ~Frame();
  
private:
  /* The stored alpha value for rendering */
  uint8_t alpha;
  
  /* How the SDL texture should be flipped while rendering */
  SDL_RendererFlip flip;
  
  /* Indicator if the grey scale texture is active */
  bool grey_scale;
  
  /* The height of the stored texture */
  int height;
  
  /* The next element in the linked list */
  Frame* next;

  /* The previous element in the linked list */
  Frame* previous;

  /* The texture for this frame */
  SDL_Texture* texture;
  SDL_Texture* texture_grey;
  
  /* The width of the stored texture */
  int width;

  /*------------------- Constants -----------------------*/
  const static uint8_t kDEFAULT_ALPHA; /* The default alpha rating */
  
/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Executes the necessary image adjustments, as per the file data handlers */
  bool execImageAdjustment(std::string adjustment);
  bool execImageAdjustments(std::vector<std::string> adjustments);
  
  /* Flips the sprite SDL image - either horizontal or vertical */
  void flipHorizontal(bool flip = true);
  void flipVertical(bool flip = true);
  
  /* Returns the alpha setting of the stored texture */
  uint8_t getAlpha();
  
  /* Returns the flip indication, for rendering */
  SDL_RendererFlip getFlip();
  
  /* Returns the height of the texture */
  int getHeight();
  
  /* Get next frame */
  Frame* getNext();

  /* Get previous frame */
  Frame* getPrevious();

  /* Returns the 2D SDL texture to be painted */
  SDL_Texture* getTexture(bool grey_scale = false);
  SDL_Texture* getTextureActive();
  
  /* Returns the width of the texture */
  int getWidth();
  
  /* Returns if the grey scale mode is active */
  bool isGreyScale();
  
  /* Returns if an image is set */
  bool isTextureSet(bool grey_scale = false);

  /* Render the texture to the given renderer with the given parameters */
  bool render(SDL_Renderer* renderer, int x = 0, int y = 0,
                                      int w = 0, int h = 0);
  bool renderBoth(SDL_Renderer* renderer, uint8_t alpha, int x = 0, int y = 0, 
                                                         int w = 0, int h = 0);
  
  /* Sets the alpha rating of the texture rendering */
  void setAlpha(uint8_t alpha = 255);

  /* Set next frame */
  bool setNext(Frame* next);
  
  /* Set previous frame */
  bool setPrevious(Frame* previous);

  /* Sets the frame texture */
  bool setTexture(std::string path, SDL_Renderer* renderer, 
                  uint16_t angle = 0, bool no_warnings = false,
                  bool enable_greyscale = true);
  bool setTexture(std::string path, std::vector<std::string> adjustments, 
                  SDL_Renderer* renderer, uint16_t angle = 0, 
                  bool no_warnings = false, bool enable_greyscale = true);
  bool setTexture(SDL_Texture* texture);

  /* Unsets the texture, if one is set */
  void unsetTexture();
  
  /* Sets if the greyscale texture is active and returned on getTexture() */
  bool useGreyScale(bool enable);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Creates a right hand triangle, given the parameters and a renderer */
  static bool renderRHTriangle(uint32_t x, uint32_t y, uint16_t height, 
                               SDL_Renderer* renderer, bool reverse = false);
};

#endif // FRAME_H
