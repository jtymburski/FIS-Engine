/******************************************************************************
* Class Name: MapStatusBar
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The MapStatusBar class, this represents the dialog box in the map.
******************************************************************************/
#ifndef MAPSTATUSBAR_H
#define MAPSTATUSBAR_H

#include <QImage>
#include <QtGui/QWidget>
#include <QRect>
#include <QString>
#include <QTimer>

#include "Game/Map/MapPerson.h"

class MapStatusBar : public QWidget
{
public:
  /* Constructor function */
  MapStatusBar(QWidget* parent = 0);

  /* Destructor function */
  ~MapStatusBar();

private:
  /* The current string being shown */
  QString* current_dialog;

  /* Timer that handles shifting the bounding boxes when the talking source
    shifts */
  QTimer dialog_shift;

  /* The left and right conversationists */
  MapPerson* left,right;

  /* Bounding boxes for the left and right conversationists and the central
    dialog box */
  QRect leftbox, rightbox, midbox;

  /* The bounding box for the popout at the top right of screen (On bubby
    pickup for example) */
  QRect popoutbox;

  /* The dialog displayed in the popout */
  QString * popout_dialog;

  /* The image displayed in the popout */
  QImage * popout_image;

  /* Timer to handle the popout box rendering */
  QTimer popout_shift;

  /* Flag for if a converation is taking place */
  bool CONVERSATION;

  /* Flag for if the popout box is visible (For nullifying the popout image
    and dialog) */
  bool POPOUT_VISIBLE;

  /* Flag for which person is talking(1-right, 0-left) */
  bool RIGHT_VISIBLE;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  void paintEvent(QPaintEvent *);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Sets up a conversation with the given persons */
  void initConversation (MapPerson* left, MapPerson* right);

  /* Sets up the popout box */
  void initPopout (QImage* img, QString* dialog);

  /* Proceeds in the conversation, enter key triggers this */
  void proceed();
};

#endif // MAPSTATUSBAR_H
