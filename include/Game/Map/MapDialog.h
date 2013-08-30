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
#include <QKeyEvent>
#include <QObject>
//#include <QTime>

#include "EnumDb.h"
#include "Game/Frame.h"
#include "Game/Map/MapThing.h"

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
  float animation_cursor;
  bool animation_cursor_up;
  short animation_height;
  short animation_offset;
 
  /* The currently running conversation information */
  Conversation conversation_info;

  /* The conversational display frame */
  Frame dialog_display;

  /* The mode that the dialog is running in */
  DialogMode dialog_mode;
  
  /* Logic if the conversation is taking place */
  DialogStatus dialog_status;

  /* The display text index, the first line currently being shown */
  short display_index;

  /* The font information to render the dialog to */
  QFont display_font;
 
  /* The display option index, for option based selection */
  short display_option;

  /* The display text, inside the visible dialog */
  QList<QString> display_text;
  
  /* A time counter, for determining how long things have been displayed */
  short display_time;

  /* The bounding box for the popout at the top right of screen (On bubby
    pickup for example) */
//  QRect popoutbox;

  /* The data for the associated things. This is pertinent for the
   * conversation access and anything displayed */
  MapThing* thing;
  QList<MapThing*> thing_data;

  /* -------------------------- Constants ------------------------- */
  const static short kCURSOR_NEXT_SIZE; /* The size of the next shifter */
  const static short kCURSOR_NEXT_TIME; /* Time it takes to animate */
  const static short kFONT_SIZE;   /* The font size, used for rendering */
  const static short kFONT_SPACING; /* The spacing between lines of font */
  const static short kMARGIN_SIDES; /* The left and right margin size */
  const static short kMARGIN_TOP;   /* The top margin size */
  const static short kMAX_LINES;    /* The max number of lines in display */
  const static short kMSEC_PER_WORD; /* The read speed per word */
  const static short kNAME_BOX_ANGLE_X; /* Offset X on angle for name box */
  const static short kNAME_BOX_HEIGHT; /* Height of the name box */
  const static short kNAME_BOX_MIN_WIDTH; /* Minimum width of name box */
  const static short kNAME_BOX_X_OFFSET; /* Offset from dialog box for name */
  const static short kOPTION_MARGIN; /* The margin around option selection */
  const static short kOPTION_OFFSET; /* The option display offset in pixels */
  const static short kSHIFT_TIME;  /* The time it takes to shift the display
                                      into view (in msec) */

 /*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Calculates a complete list of thing IDs that are used in the given
   * conversation */
  QList<int> calculateThingList(Conversation conversation);

  /* Functions to acquire thing data, for painting to the screen */
  bool getThingPtr(int id);
  
  /* Halts the dialog, if it's being shown or showing */
  void initiateAnimation(QFont display_font);
  
  /* Removes duplicates from the given qlist and returns it. Used in
   * conjunction with "calculateThingList" */
  QList<int> removeDuplicates(QList<int> duplicate_list);

  /* Sets the current conversation entry up, called after the previous was
   * finished */
  void setupConversation();
  
/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void finishThingTarget();
  void setThingData(QList<int> thing_ids);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Initializes a conversation with the two given people. */
  bool initConversation(Conversation dialog_info);

  /* Initializes a notification, using a QString */
  bool initNotification(QString notification, int time_visible = -1, 
                                              bool single_line = false);

  /* Returns if the dialog image has been set (and proper size) */
  bool isDialogImageSet();

  /* Some status checks, of the state of the class */
  bool isInConversation();
  bool isInUse();

  /* Key press event reimplemented */
  void keyPress(QKeyEvent* event);
  void keyRelease(QKeyEvent* event);

  /* Paint call, that paints the dialog */
  bool paintGl(QGLWidget* painter);

  /* Proceeds in the conversation, enter key triggers this */
//  void proceed();

  /* Sets the dialog image within the class, for conversation display */
  bool setDialogImage(QString path);

  /* Sets the rendering font */
  void setFont(QFont font);

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
