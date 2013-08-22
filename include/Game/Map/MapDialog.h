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

#include "EnumDb.h"
#include "Game/Frame.h"
#include "Game/Map/MapThing.h"

/* Struct to handle the conversation throughout the map */
// TODO: Add MapAction, tile image?
struct Conversation
{
  QString text;
  int thing_id;
  EnumDb::DialogCategory category;
  QList<Conversation> next;
};

class MapDialog : public QObject
{
  Q_OBJECT

public:
  /* Constructor function */
  MapDialog(QFont font = QFont());

  /* Destructor function */
  ~MapDialog();

  /* The dialog display classifier to define how it's displayed:
   *  OFF - not showing
   *  SHOWING - rising from the bottom
   *  ON - fully displayed
   *  HIDING - lowering to the bottom */
  enum DialogStatus{OFF, SHOWING, ON, HIDING};

  // TODO: add the shop implementation [2013-08-20]
  /* The dialog mode classifier to define the running mode:
   *  DISABLED - The dialog is not in use
   *  CONVERSATION - A conversation is currently running
   *  NOTIFICATION - A notification display, shifts up then down 
   *  SHOP - A numerical question for buying, in a shop for example */
  enum DialogMode{DISABLED, CONVERSATION, NOTIFICATION, SHOP};
  
private:
  /* The animation info for displaying the dialog */
  short animation_offset;
  short animation_height;
 
  /* The currently running conversation information */
  Conversation conversation_info;

  /* The current string being shown */
//  QString* current_dialog;

  /* The conversational display frame */
  Frame dialog_display;

  /* The mode that the dialog is running in */
  DialogMode dialog_mode;
  
  /* Logic if the conversation is taking place */
  DialogStatus dialog_status;

  /* Timer that handles shifting the bounding boxes when the talking source
    shifts */
//  QTimer dialog_shift;

  /* The font information to render the dialog to */
  QFont display_font;
  
  /* The display text, inside the visible dialog */
  QList<QString> display_text;
  
  /* A time counter, for determining how long things have been displayed */
  short display_time;
  
  /* The left and right conversationists */
//  MapPerson* left,right;

  /* Bounding boxes for the left and right conversationists and the central
    dialog box */
//  QRect leftbox, rightbox, midbox;

  /* The display npc that gets shown during the conversation */
  Frame npc_display;
  QString npc_name;

  /* The display person that gets shown during the conversation */
  Frame person_display;
  QString person_name;

  /* The bounding box for the popout at the top right of screen (On bubby
    pickup for example) */
//  QRect popoutbox;

  /* The dialog displayed in the popout */
//  QString * popout_dialog;

  /* The image displayed in the popout */
//  QImage * popout_image;

  /* Timer to handle the popout box rendering */
//  QTimer popout_shift;

  /* The data for the associated things. This is pertinent for the
   * conversation access and anything displayed */
  QList<MapThing*> thing_data;

  /* Flag for if a converation is taking place */
//  bool CONVERSATION;

  /* Flag for if the popout box is visible (For nullifying the popout image
    and dialog) */
//  bool POPOUT_VISIBLE;

  /* Flag for which person is talking(1-right, 0-left) */
//  bool RIGHT_VISIBLE;

  /* -------------------------- Constants ------------------------- */
  const static short kFONT_SIZE;   /* The font size, used for rendering */
  const static short kFONT_SPACING; /* The spacing between lines of font */
  const static short kMARGIN_SIDES; /* The left and right margin size */
  const static short kMARGIN_TOP;   /* The top margin size */
  const static short kMSEC_PER_WORD; /* The read speed per word */
  const static short kPIXELS_PER_100MS; /* The animation pixels per 100 ms */
  const static short kSHIFT_TIME;  /* The time it takes to shift the display
                                      into view (in msec) */

 /*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Calculates a complete list of thing IDs that are used in the given
   * conversation */
  QList<int> calculateThingList(Conversation conversation);

  /* Halts the dialog, if it's being shown or showing */
  void initiateAnimation(QFont display_font);

  /* Removes duplicates from the given qlist and returns it. Used in
   * conjunction with "calculateThingList" */
  QList<int> removeDuplicates(QList<int> duplicate_list);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void setThingData(QList<int> thing_ids);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Halts the dialog, if it's being shown or showing */
  bool haltDialog();

  /* Initializes a conversation with the two given people. */
  bool initConversation(Conversation dialog_info);
  
  /* Sets up a dialog with the initial parameters */
  bool initDialog();//MapPerson* left, MapPerson* right);

  /* Initializes a notification, using a QString */
  bool initNotification(QString notification, int time_visible = -1, 
                                              bool single_line = false);
  
  /* Sets up the popout box */
//  void initPopout (QImage* img, QString* dialog);

  /* Returns if the dialog image has been set (and proper size) */
  bool isDialogImageSet();
  
  /* Paint call, that paints the dialog */
  bool paintGl(QGLWidget* painter);

  /* Proceeds in the conversation, enter key triggers this */
//  void proceed();

  /* Sets the dialog image within the class, for conversation display */
  bool setDialogImage(QString path);

  /* Sets the rendering font */
  void setFont(QFont font);
  
  /* Sets the npc display information, for conversations */
  bool setNpcDisplay(QString path);
  void setNpcName(QString name);

  /* Sets the person display information, for conversations */
  bool setPersonDisplay(QString path);
  void setPersonName(QString name);

  /* Sets the thing data, needed for the conversation */
  void setThingData(QList<MapThing*> data);

  /* Updates the dialog, based on an elapsed time */
  void update(float cycle_time);
  
/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Fits a line into a list of lines that are less than the size using the
   * given parameters */
  static QList<QString> lineSplitter(QString line, int line_length, QFont font);
};

#endif // MAPDIALOG_H
