/******************************************************************************
* Class Name: Frame
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Frame class, this represents an animation frame within the
*              Sprite class. It acts as a linked list node, in that it contains
*              a pointer to the next Frame in the sequence.
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
  previous = 0;
  next = 0;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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
  this->image = image;
  
  /* Check if the image is actual data to determine if it has been set */
  if(image.isNull())
    image_set = false;
  else
    image_set = true;
  
  return image_set;
}

/* 
 * Description: Sets stored image in the frame, from a string path
 *
 * Inputs: QString path - the path to the image to create
 * Output: Bool - true if successful
 */
bool Frame::setImage(QString path)
{
  QPixmap new_image = openImage(path);
  image_set = false;

  if(!new_image.isNull())
  {
    this->image = new_image;
    image_set = true;
  }

  return image_set;
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

  return true; // can't fail so always true
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

  return true; // can't fail so always true
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
