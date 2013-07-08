/******************************************************************************
* Class Name: Frame
* Date Created: Dec 2 2012
* Inheritance: none
* Description: The Frame class, this represents an animation frame within the
*              Sprite class. It acts as a linked list node, in that it contains
*              a pointer to the next Frame in the sequence. This class also
*              contains the functionality for painting the image to a straight
*              OpenGL call. The context must have been set up before this
*              but once down, initializeGl() and paintGl() can be called to
*              draw straight to the GL API.
******************************************************************************/
#include "Game/Frame.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor for this class. Empty parameter, default 
 *              initialization. Clears pointers and sets up the class
 *              for new data.
 *
 * Inputs: none
 */
Frame::Frame()
{
  previous = 0;
  next = 0;
  image_set = false;
  gl_image_set = false;
}

/* 
 * Description: Constructor for this class. Takes image and next pointer.
 *              Next pointer is defaulted to 0 if not given.
 *
 * Inputs: QPixmap image - the image to copy into the class
 *         int rotate_angle - the angle to rotate the image to, default to 0
 *         Frame* next - pointer to next frame, default to 0
 *         Frame* previous - pointer to previous frame, default to 0
 */
Frame::Frame(QPixmap image, int rotate_angle, Frame* next, Frame* previous)
{
  gl_image_set = false;

  setImage(image);
  rotateImage(rotate_angle);
  setPrevious(previous);
  setNext(next);
}

/* 
 * Description: Constructor for this class. Takes path and next pointer.
 *              Next pointer is defaulted to 0 if not given.
 *
 * Inputs: QString path - the path to the image to create
 *         int rotate_angle - the angle to rotate the image to, default to 0
 *         Frame* next - pointer to next frame, default to 0
 *         Frame* previous - pointer to previous frame, default to 0
 */
Frame::Frame(QString path, int rotate_angle, Frame* next, Frame* previous)
{
  gl_image_set = false;

  setImage(path);
  rotateImage(rotate_angle);
  setPrevious(previous);
  setNext(next);
}

/* 
 * Description: Destructor function 
 */
Frame::~Frame()
{
  uninitializeGl();
  previous = 0;
  next = 0;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Initializes GL functionality. This must be called anytime
 *              the image data is changed or when the class is first created
 *              to initialize the image data. Once this is complete, paintGl()
 *              can be called to paint the image data
 *
 * Inputs: none
 * Output: bool - status if the initialization was successful
 */
bool Frame::initializeGl()
{
  if(image_set && !gl_image_set)
  {
    /* Convert the image to GL format */
    QImage texture = QGLWidget::convertToGLFormat(image.toImage());

    /* Set up the bind */
    glGenTextures(1, &gl_image);
    glBindTexture(GL_TEXTURE_2D, gl_image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
    /* Set up the texture in GL */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    gl_image_set = true;
  }

  return image_set;
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
 * Description: Gets image stored in this node
 *
 * Inputs: none
 * Output: QPixmap - Returns a QPixmap initialized with the stored picture
 */
QPixmap Frame::getImage()
{
  return image;
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
 * Description: Paints the GL image data, given an x and a y coordinate
 *              on the field. This can offset the stored texture to wherever
 *              the higher layers want to place it. This function does nothing
 *              until initializeGl() is called in case of uninitializeGl() or
 *              that the image data has changed or for first run.
 *
 * Inputs: int x - the x offset on the plane (left/right)
 *         int y - the y offset on the plane (up/down)
 *         int width - the width to render the image to
 *         int height - the height to render the image to
 *         float opacity - the opacity of the paint (0-1)
 * Output: bool - status if the paint occurred (indication if the image was
 *                initialized.
 */
bool Frame::paintGl(int x, int y, int width, int height, float opacity)
{
  if(gl_image_set)
  {
    int img_width = image.width();
    int img_height = image.height();

    /* Set up the initial flags */
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    if(opacity == 1.0)
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    else
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, opacity);
    glBindTexture(GL_TEXTURE_2D, gl_image);
  
    /* Execute the draw */
    glBegin(GL_QUADS);
    
    glTexCoord2i(0, 0); 
    glVertex3i(x, y, 0);
    
    glTexCoord2i(img_width, 0); 
    glVertex3i(x + width, y, 0);
    
    glTexCoord2i(0, img_height); 
    glVertex3i(x, y + height, 0);
    
    glTexCoord2i(img_width, img_height); 
    glVertex3i(x + width, y + height, 0);
    
    glEnd();

    /* Initialize cleanup */
    glDisable(GL_TEXTURE_2D);

    return true;
  }

  return false;
}

/* 
 * Description: Rotates an image by the number of degrees specified. This will
 *              have unknown results if the rotation isn't a clean number
 *              that is a divisor of 90.
 *
 * Inputs: int angle - the angle to rotate the image by.
 * Output: bool - the status if the image was set so it can be rotated.
 */
bool Frame::rotateImage(int angle)
{
  if(image_set && angle != 0)
  {
    uninitializeGl();

    QMatrix matrix;
    matrix.rotate(angle);
    image = image.transformed(matrix);
    return true;
  }

  return false;
}

/* 
 * Description: Sets stored image in the frame, from a QPixmap
 *
 * Inputs: QPixmap image - the image to use within the class
 * Output: Bool - true if successful
 */
bool Frame::setImage(QPixmap image)
{
  /* Check if the image is actual data to determine if it has been set */
  if(!image.isNull())
  {
    uninitializeGl();
    this->image = image;
    image_set = true;
    return true;
  }
  
  return false;
}

/* 
 * Description: Sets stored image in the frame, from a string path
 *
 * Inputs: QString path - the path to the image to create
 * Output: Bool - true if successful
 */
bool Frame::setImage(QString path)
{
  return setImage(openImage(path));
}

/* 
 * Description: Sets next frame pointer
 *
 * Inputs: Frame* next - pointer to next frame, can be 0
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
 * Inputs: Frame* previous - pointer to previous frame, can be 0
 * Output: bool - true if successful
 */
bool Frame::setPrevious(Frame* previous)
{
  this->previous = previous;
  return true;
}

/* 
 * Description: Uninitializes GL. This removes the gl image from within
 *              the OpenGL stack that was initialized earlier. Once this is
 *              called, initializeGl() needs to be called again for painting
 *              to occur.
 *
 * Inputs: none
 * Output: none
 */
void Frame::uninitializeGl()
{
  if(gl_image_set)
  {
    glDeleteTextures(1, &gl_image);
    gl_image_set = false;
  }
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/* 
 * Description: Attempts to open an image. Returns the QPixmap as a non null
 *              image if its possible.
 *
 * Inputs: QString path - the path that the image is situated at
 * Output: QPixmap - the returned image that can be used (for painting, etc)
 */
QPixmap Frame::openImage(QString path)
{
  QFile new_file(path);

  if(new_file.exists())
  {
    QPixmap image(path);
    return image;
  }

  QPixmap blank_image;
  return blank_image;
}
