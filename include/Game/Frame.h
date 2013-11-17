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
#ifndef FRAME_H
#define FRAME_H

#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

//#ifdef __APPLE__
//  #include <OpenGL/glu.h>
//#else
//  #include <GL/glu.h>
//#endif
//#include <QDebug>
//#include <QFile>
//#include <QGLWidget>
//#include <QPixmap>

//#include "EnumDb.h"

class Frame
{
public:
  /* Constructor function - empty initialization */
  Frame();

  /* Constructor function - path and next pointer initialization */
  Frame(std::string path, Frame* previous = NULL, Frame* next = NULL);

  /* Destructor function */
  ~Frame();
  
private:
  /* Asserts that the image has been set */
  bool image_set;
  
  /* The next element in the linked list */
  Frame* next;

  /* The previous element in the linked list */
  Frame* previous;

  /* The surface for this frame */
  SDL_Surface* surface;

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Get next frame */
  Frame* getNext();

  /* Get previous frame */
  Frame* getPrevious();

  /* Returns the 2D SDL surface to be painted */
  SDL_Surface* getSurface();
  
  /* Returns if an image is set */
  bool isImageSet();

  /* Set next frame */
  bool setNext(Frame* next);
  
  /* Set previous frame */
  bool setPrevious(Frame* previous);

  /* Sets the frame surface */
  bool setSurface(std::string path);
  
  /* Unsets the surface, if one is set */
  void unsetSurface();
};

#endif // FRAME_H
