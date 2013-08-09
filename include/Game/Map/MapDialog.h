/******************************************************************************
* Class Name: MapDialog
* Date Created: August 7, 2013
* Inheritance: none
* Description: The dialog display at the bottom of the map. Offers either
*              conversation options and display or notifications.
******************************************************************************/
#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDebug>
//#include <QImage>
#include <QObject>
//#include <QtGui/QWidget>
//#include <QRect>
//#include <QString>
//#include <QTimer>

#include "Game/Frame.h"
//#include "Game/Map/MapPerson.h"

class MapDialog : public QObject
{
  Q_OBJECT

public:
  /* Constructor function */
  MapDialog();

  /* Destructor function */
  ~MapDialog();

  /* The dialog display classifier to define it's current mode
   *  OFF - not showing
   *  SHOWING - rising from the bottom
   *  ON - fully displayed
   *  HIDING - lowering to the bottom */
  enum DialogDisplay{OFF, SHOWING, ON, HIDING};

private:
  /* The animation offset for displaying the dialog */
  short animation_offset;

  /* The current string being shown */
//  QString* current_dialog;

  /* The conversational display frame */
  Frame dialog_display;

  /* Logic if the conversation is taking place */
  DialogDisplay dialog_status;

  /* Timer that handles shifting the bounding boxes when the talking source
    shifts */
//  QTimer dialog_shift;

  /* The left and right conversationists */
//  MapPerson* left,right;

  /* Bounding boxes for the left and right conversationists and the central
    dialog box */
//  QRect leftbox, rightbox, midbox;

  /* The bounding box for the popout at the top right of screen (On bubby
    pickup for example) */
//  QRect popoutbox;

  /* The dialog displayed in the popout */
//  QString * popout_dialog;

  /* The image displayed in the popout */
//  QImage * popout_image;

  /* Timer to handle the popout box rendering */
//  QTimer popout_shift;

  /* Flag for if a converation is taking place */
//  bool CONVERSATION;

  /* Flag for if the popout box is visible (For nullifying the popout image
    and dialog) */
//  bool POPOUT_VISIBLE;

  /* Flag for which person is talking(1-right, 0-left) */
//  bool RIGHT_VISIBLE;

  /* -------------------------- Constants ------------------------- */
  const static short kSHIFT_TIME;  /* The time it takes to shift the display
                                      into view (in msec) */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Halts the dialog, if it's being shown or showing */
  bool haltDialog();

  /* Sets up a dialog with the initial parameters */
  bool initDialog();//MapPerson* left, MapPerson* right);

  /* Sets up the popout box */
//  void initPopout (QImage* img, QString* dialog);

  /* Paint call, that paints the dialog */
  bool paintGl();

  /* Proceeds in the conversation, enter key triggers this */
//  void proceed();

  /* Sets the dialog image within the class, for conversation display */
  bool setDialogImage(QString path);

  /* Updates the dialog, based on an elapsed time */
  void update(float cycle_time);
};

#endif // MAPDIALOG_H
