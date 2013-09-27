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
#include "Game/EventHandler.h"
#include "Game/Frame.h"
#include "Game/Map/MapThing.h"

/* Notification queue structure */
struct Notification
{
  QString text;
  Frame* thing_image;
  short thing_count;
  short time_visible;
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
  float animation_cursor;
  bool animation_cursor_up;
  short animation_height;
  float animation_offset;
  float animation_shifter;
 
  /* The currently running conversation information */
  Conversation conversation_info;
  bool conversation_waiting;

  /* Bottom dialog information, for displaying */
  Frame dialog_display;
  float dialog_letters;
  bool dialog_letters_done;
  short dialog_letters_max;
  DialogMode dialog_mode;
  short dialog_option;
  short dialog_option_top;
  bool dialog_shift_enable;
  short dialog_shift_max;
  short dialog_shift_offset;
  DialogStatus dialog_status;
  QList<QString> dialog_text;
  short dialog_text_index;
  short dialog_time;

  /* The font information to render the dialog to */
  QFont display_font;

  /* The queue of all notifications available */
  QList<Notification> notification_queue;
  
  /* The list of available options, being printed out */
  QList<QString> options_text;
  
  /* The pause indication, if control has been paused */
  bool paused;
  float paused_opacity;
  
  /* The top right notification data to be displayed - from a pickup */
  float pickup_offset;
  DialogStatus pickup_status;
  short pickup_time;
  short pickup_width;
 
  /* The player id that instantiated the conversation */
  int player_id;

  /* The data for the associated things. This is pertinent for the
   * conversation access and anything displayed */
  MapThing* thing;
  QList<MapThing*> thing_data;

  /* The map viewport dimensions */
  short viewport_height;
  short viewport_width;

  /* -------------------------- Constants ------------------------- */
  const static float kBUBBLES_ANIMATE; /* The text too long animation time */
  const static short kBUBBLES_COUNT; /* The number of dots to display */
  const static short kBUBBLES_OFFSET; /* Offset from the bottom of the screen */
  const static short kBUBBLES_RADIUS; /* Radius of display bubble */
  const static short kBUBBLES_SPACING; /* Spacing between dots */
  const static short kCURSOR_NEXT_SIZE; /* The size of the next shifter */
  const static short kCURSOR_NEXT_TIME; /* Time it takes to animate */
  const static short kFONT_SIZE; /* The font size, used for rendering */
  const static short kFONT_SPACING; /* The spacing between lines of font */
  const static short kMARGIN_SIDES; /* The left and right margin size */
  const static short kMARGIN_TOP; /* The top margin size */
  const static short kMAX_LINES; /* The max number of lines in display */
  const static short kMAX_OPTIONS; /* The max number of options displayed */
  const static short kMIN_DISPLAY_TIME; /* The minimum notification display */
  const static short kMSEC_PER_WORD; /* The read speed per word */
  const static short kNAME_BOX_ANGLE_X; /* Offset X on angle for name box */
  const static short kNAME_BOX_HEIGHT; /* Height of the name box */
  const static short kNAME_BOX_MIN_WIDTH; /* Minimum width of name box */
  const static short kNAME_BOX_X_OFFSET; /* Offset from dialog box for name */
  const static short kOPTION_MARGIN; /* The margin around option selection */
  const static short kOPTION_OFFSET; /* The option display offset in pixels */
  const static float kPAUSE_OPACITY_DIFF; /* Delta when changing opacity */
  const static float kPAUSE_OPACITY_MAX; /* Max opacity once pause has ended */
  const static short kPICKUP_ANGLE_X; /* The angled portion x width */
  const static short kPICKUP_ANGLE_Y; /* The angled portion y width */
  const static short kPICKUP_DEFAULT_TIME; /* The default animation time */
  const static short kPICKUP_HEIGHT; /* The overall full painted height */
  const static short kPICKUP_MARGIN; /* The margin between the tile and edge */
  const static short kPICKUP_TEXT_SPACE; /* The space between tile and text */
  const static short kPICKUP_Y; /* The top left y coordinate */
  const static short kSCROLL_CIRCLE_RADIUS; /* Circle radius, in pixels */
  const static short kSCROLL_OFFSET; /* The offset for scrollbar off of text */
  const static short kSCROLL_TRIANGLE_HEIGHT; /* Triangle pixel height */
  const static short kSHIFT_OFFSET; /* The number of pixels per update shift of
                                       text */
  const static short kSHIFT_TIME;  /* The time it takes to shift the display
                                      into view (in msec) */
  const static float kTEXT_DISPLAY_SPEED; /* The character display speed */

 /*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Calculates a complete list of thing IDs that are used in the given
   * conversation */
  QList<int> calculateThingList(Conversation conversation);

  /* The dialog shifting enable call. Used to rotate the text up before
   * shifting to the next when the entry is too long */
  void dialogShiftEnable(bool enable);

  /* Draws an approximate circle. More applicable for small circles */
  bool drawPseudoCircle(int x, int y, int radius);

  /* Executes the conversation event */
  void executeEvent();

  /* Functions to acquire thing data, for painting to the screen */
  bool getThingPtr(int id);
  
  /* Halts the dialog, if it's being shown or showing */
  void initiateAnimation(QFont display_font);
  
  /* Initiates the next notification, if there is a slot available */
  bool initiateNextNotification();
  
  /* Removes duplicates from the given qlist and returns it. Used in
   * conjunction with "calculateThingList" */
  QList<int> removeDuplicates(QList<int> duplicate_list);

  /* Sets the current conversation entry up, called after the previous was
   * finished */
  void setupConversation();
 
  /* Resets the counters that spell out the text character by character when
   * initially displayed */
  void resetConversationTextDisplay();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void finishThingTarget();
  void setThingData(QList<int> thing_ids);
  
/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Sets if the class control is paused */
  void setPaused(bool paused);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Called when the conversation needs to be abruptly ended */
  void endConversation();

  /* Initializes a conversation with the two given people. */
  bool initConversation(Conversation dialog_info, int player_id);

  /* Initialize all OpenGL calls needed for this class */
  void initializeGl();

  /* Initializes a notification, using a QString */
  bool initNotification(QString notification = "", int time_visible = -1, 
                        bool single_line = false);

  bool initPickup(Frame* thing_image, int thing_count = 1, 
                  int time_visible = -1);
                  
  /* Returns if the dialog image has been set (and proper size) */
  bool isDialogImageSet();

  /* Some status checks, of the state of the class */
  bool isDialogInUse();
  bool isInConversation();
  bool isPickupInUse();

  /* Returns if the dialog class is paused */
  bool isPaused();
  
  /* Key press event reimplemented */
  void keyPress(QKeyEvent* event);
  void keyRelease(QKeyEvent* event);

  /* Paint call, that paints the dialog */
  bool paintGl(QGLWidget* painter);

  /* Sets the dialog image within the class, for conversation display */
  bool setDialogImage(QString path);

  /* Sets the rendering font */
  void setFont(QFont font);

  /* Sets the thing data, needed for the conversation */
  void setThingData(QList<MapThing*> data);
 
  /* Sets the map dimension used for positioning dialog and notifications */
  bool setViewportDimension(short width, short height);

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
