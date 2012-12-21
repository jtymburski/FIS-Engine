/******************************************************************************
* Class Name: Frame
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Frame class, this represents an animation frame within the
*              Sprite class. It acts as a linked list node, in that it contains
*              a pointer to the next Frame in the sequence.
******************************************************************************/
#include "Game/Frame.h"

/* 
 * Description: Constructor for this class. Takes path and next pointer.
 *              Next pointer is defaulted to 0 if not given.
 * Input: QString path - the path to the image to create
 *        Frame* next - pointer to next frame, default to 0
 * Output: none
 */
Frame::Frame(QString path, Frame* next)
{
  setImage(path);
  setNext(next);
}

/* Destructor function */
Frame::~Frame()
{
  next = 0;
}

/* 
 * Description: Gets image stored in this node
 * Input: none
 * Output: QImage - Returns a QImage initialized with the stored picture
 */
QImage Frame::getImage()
{
  return image;
}

/* 
 * Description: Gets next frame pointed to by this node
 * Input: none
 * Output: Frame* - pointer to next node
 */
Frame* Frame::getNext()
{
  return next;
}

/* 
 * Description: Sets stored image in the frame
 * Input: QString path - the path to the image to create
 * Output: Bool - true if successful
 */
bool Frame::setImage(QString path)
{
  QFile new_file(path);

  if(new_file.exists())
  {
    (this->image).load(path);
    return TRUE;
  }

  return FALSE;
}

/* 
 * Description: Sets next frame pointer
 * Input: Frame* next - pointer to next frame, can be 0
 * Output: Bool - true if successful
 */
bool Frame::setNext(Frame* next)
{
  this->next = next;

  return TRUE; // Can't fail so always true
}
