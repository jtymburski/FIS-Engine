/******************************************************************************
* Class Name: Frame
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The Frame class, this represents an animation frame within the
*              Sprite class. It acts as a linked list node, in that it contains
*              a pointer to the next Frame in the sequence.
******************************************************************************/
#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <QtGui/QWidget>

class Frame : public QWidget
{
public:
  /* Constructor function */
  Frame(QWidget *parent = 0);

  /* Destructor function */
  ~Frame();

protected: // TODO: Paint Event Here?? [12-02-12]
  void paintEvent(QPaintEvent *); 

private:
  /* The image for this frame */
  QImage img;

};

#endif // FRAME_H
