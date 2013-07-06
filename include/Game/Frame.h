/******************************************************************************
* Class Name: Frame
* Date Created: Oct 28, 2012
* Inheritance: none
* Description: The Frame class, this represents an animation frame within the
*              Sprite class. It acts as a linked list node, in that it contains
*              a pointer to the next Frame in the sequence. This class also
*              contains the functionality for painting the image to a straight
*              OpenGL call. The context must have been set up before this
*              but once down, initializeGl() and paintGl() can be called to
*              draw straight to the GL API.
******************************************************************************/
#ifndef FRAME_H
#define FRAME_H

#include <GL/glu.h>
#include <QFile>
#include <QGLWidget>
#include <QPixmap>

#include "EnumDb.h"

class Frame
{
public:
  /* Constructor function - empty initialization */
  Frame();

  /* Constructor function - image and next pointer initialization */
  Frame(QPixmap image, int rotate_angle = 0,
        Frame* next = 0, Frame* previous = 0);

  /* Constructor function - path and next pointer initialization */
  Frame(QString path, int rotate_angle = 0, 
        Frame* next = 0, Frame* previous = 0);

  /* Destructor function */
  ~Frame();

private:
  /* The image for this frame */
  QPixmap image;
  GLuint gl_image;

  /* The previous element in the linked list */
  Frame* previous;

  /* The next element in the linked list */
  Frame* next;

  /* Asserts that the image has been set */
  bool image_set;
  bool gl_image_set;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Initializes GL for this class. Called before painting */
  bool initializeGl();

  /* Returns if an image is set */
  bool isImageSet();

  /* Get stored image */
  QPixmap getImage();

  /* Get next frame */
  Frame* getNext();

  /* Get previous frame */
  Frame* getPrevious();

  /* Draws the frame in OpenGL, if the appropriate flags are set */
  bool paintGl(int x, int y, float opacity);

  /* Rotate the image an integer number of degrees */
  bool rotateImage(int angle);

  /* Sets the stored image, from QPixmap */
  bool setImage(QPixmap image);

  /* Sets the stored image, from QString */
  bool setImage(QString path);

  /* Set next frame */
  bool setNext(Frame* next);

  /* Set previous frame */
  bool setPrevious(Frame* previous);

  /* Removes set textures from within the frame if called */
  void uninitializeGl();

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Opens an image and returns the qpixmap, if it exists */
  static QPixmap openImage(QString path);
};

#endif // FRAME_H
