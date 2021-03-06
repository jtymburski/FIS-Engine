/******************************************************************************
 * Class Name: MapDialog
 * Date Created: August 7, 2013
 * Inheritance: none
 * Description: The dialog display at the bottom of the map. Offers either
 *              conversation options and display or notifications.
 ******************************************************************************/
#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <string>
#include <vector>

#include "EnumDb.h"
#include "Frame.h"
#include "Game/EventHandler.h"
#include "Game/Map/MapPerson.h"
#include "Game/Map/MapThing.h"
#include "Helpers.h"
#include "Options.h"
#include "Sound.h"
#include "Text.h"

/* Notification queue structure */
struct Notification
{
  string text;
  int text_lines;
  Frame* thing_image;
  int32_t thing_count;
  uint16_t time_visible;

  SDL_Color text_color;
};

class MapDialog
{
public:
  /* Constructor function */
  MapDialog(Options* running_config = nullptr);

  /* Destructor function */
  ~MapDialog();

private:
  /* Animation shifters */
  float animation_cursor;
  bool animation_cursor_up;
  float animation_shifter;

  /* The currently running conversation information */
  int conversation_delay;
  Conversation* conversation_info;
  Conversation* conversation_inst;
  bool conversation_ready;
  bool conversation_update;
  bool conversation_waiting;

  /* The dialog alpha rating (for how visible) */
  uint8_t dialog_alpha;

  /* The running dialog mode and status - used for display control */
  DialogMode dialog_mode;
  WindowStatus dialog_status;

  /* Boolean control of text dialog */
  float dialog_offset;
  uint8_t dialog_option;
  uint8_t dialog_option_top;

  /* The event handler information */
  EventHandler* event_handler;

  /* The two render fonts, for displaying any text */
  TTF_Font* font_normal;
  TTF_Font* font_title;

  /* The frame controller for displaying the conversation and pickup */
  Frame frame_bottom;
  Frame frame_right;

  /* Image frames to be loaded for rendering */
  Frame img_convo; /* Main Dialog display */
  Sprite img_convo_c; /* Conversation delay to next indicator */
  Frame img_convo_m; /* Conversation more to display indicator */
  Frame img_convo_n; /* Conversation next arrow */
  Frame img_name_l;  /* Left dialog name display corner */
  Frame img_name_r;  /* Right dialog name display corner */
  Frame img_opt_c;   /* Option Circle */
  Frame img_opt_d;   /* Option Down Arrow */
  Frame img_opt_u;   /* Option Up Arrow */
  Frame img_pick_b;  /* Bottom pickup display corner */
  Frame img_pick_t;  /* Top pickup display corner */

  /* Item data */
  std::vector<ItemData> item_data;

  /* The queue that holds all bottom notifications that need to be displayed */
  vector<Notification> notification_queue;
  uint16_t notification_time;
  vector<Notification> notification_waiting;

  /* The paused control settings */
  bool paused;

  /* The pickup notification queue (right hand side) */
  float pickup_offset;
  vector<Notification> pickup_queue;
  WindowStatus pickup_status;
  uint16_t pickup_time;
  bool pickup_update;

  /* The system options, used for rendering, settings, etc. */
  Options* system_options;

  /* Used thing data for the current rendering application */
  MapThing* source;
  MapPerson* target;
  MapThing* thing_active;
  vector<MapThing*> thing_data;

  /* Text rendering pointers */
  float text_index;
  uint16_t text_index_max;
  vector<Text*> text_lines;
  float text_offset;
  uint16_t text_offset_max;
  vector<Text*> text_options;
  vector<vector<vector<pair<string, TextProperty>>>> text_strings;

  /* Text control */
  uint16_t text_top;
  bool text_update;

  /* -------------------------- Constants ------------------------- */
  const static uint8_t kBORDER_WIDTH;  /* The border width around the dialogs */
  const static float kBUBBLES_ANIMATE; /* The animation speed per bubble */
  const static uint8_t kBUBBLES_COUNT; /* The number of shifting bubbles */
  const static uint8_t kBUBBLES_OFFSET;   /* The offset from the bottom */
  const static uint8_t kBUBBLES_SPACING;  /* The spacing between bubbles */
  const static SDL_Color kCOLOR_ADD;      /* Add color for text - items */
  const static SDL_Color kCOLOR_REM;      /* Remove color for text - items */
  const static uint16_t kCURSOR_ANIMATE;  /* The cursor animation time */
  const static uint8_t kCURSOR_HEIGHT;    /* The cursor height on animation */
  const static uint8_t kHIGHLIGHT_MARGIN; /* Highlighted option margin pixels */
  const static uint16_t kHOLD_DELAY; /* The delay between frame sequencing */
  const static uint8_t kHOLD_OFFSET; /* Offset from bottom of screen to mid */
  const static std::string kITEM_COLOR; /* Hex color of item reference */
  const static uint8_t kLINE_SPACING; /* The spacing between lines of font */
  const static uint8_t kMARGIN_SIDES; /* The left and right margin size */
  const static uint8_t kMARGIN_TOP; /* The top margin size */
  const static uint16_t kMSEC_PER_WORD; /* The read speed per word */
  const static uint8_t kNAME_BOX_OFFSET; /* Name box dialog x offset */
  const static uint8_t kNAME_MARGIN;     /* Name margin on each side */
  const static uint8_t kNOTIFY_MAX_LINES; /* Max number of lines for notify */
  const static float kOPACITY_BACKEND;    /* Backend display box opacity */
  const static uint8_t kOPACITY_MAX; /* The max opacity rating (between 0-max */
  const static uint8_t kOPTION_OFFSET; /* The offset of the options from text */
  const static uint16_t kPAUSE_TIME;   /* The time to hide or show the dialog */
  const static uint16_t kPICKUP_DISPLAY_TIME; /* Default pickup display time */
  const static uint16_t kPICKUP_IMG_SIZE;     /* Size of pickup image */
  const static uint8_t kPICKUP_TEXT_MARGIN; /* Margin between image and text */
  const static uint16_t kPICKUP_Y;          /* The top left y coordinate */
  const static float kSHIFT_TIME;     /* Time to make the display visible */
  const static uint8_t kTEXT_LINES;   /* The max number of lines displayed */
  const static uint8_t kTEXT_OPTIONS; /* The max number of options displayed */
  const static float kTEXT_DISPLAY_SPEED; /* The character display speed */
  const static float kTEXT_SHIFT;   /* The speed at which the text shifts up */
  const static string kTHING_COLOR; /* Hex color of thing reference in text */

/*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Computes all IDs that are needed for displaying the conversation */
  vector<int> calculateThingList(Conversation* convo);
  vector<int> calculateThingList(string text);

  /* Clears all stored pointer data within the class */
  void clearData();

  /* Creates the rendering fonts, based on the system options font path */
  bool createFonts();

  /* Deletes the rendering fonts, if they've been created */
  void deleteFonts();

  /* Executes an event, triggered from a conversation */
  void executeEvent();

  /* Get name if reference item */
  std::string getItemName(int id);
  std::string getItemName(std::string id);

  /* Functions to acquire thing data, for painting to the screen */
  string getThingName(int id, vector<MapThing*>* things = nullptr);
  MapThing* getThingReference(int id, vector<MapThing*>* things = nullptr);

  /* Render the options. Deletes previous options, if they exist */
  void renderOptions(
      SDL_Renderer* renderer,
      vector<vector<vector<pair<string, TextProperty>>>> options = {});

  /* Takes the text and replaces {ID} references with name */
  string replaceIDReferences(string text, vector<MapThing*>* things = nullptr);

  /* Sets the alpha of all rendering textures on the dialog */
  void setAlpha(uint8_t alpha);

  /* Sets up the active conversation pointer to prepare for screen rendering */
  void setupConversation(SDL_Renderer* renderer);

  /* Sets up the top waiting queued notification, to be displayed */
  void setupNotification(SDL_Renderer* renderer);

  /* Sets up the top waiting pickup queued notification, to be displayed */
  void setupPickup(SDL_Renderer* renderer, bool update = false);

  /* Setup the render text display. Also manages deletion of Text pointers */
  void
  setupRenderText(vector<vector<vector<pair<string, TextProperty>>>> lines = {},
                  bool delete_old = false);

/*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Clear all trigger - wipes out all queues and the current conversation */
  void clearAll(bool include_convo = false);

  /* Returns the thing IDs from the waiting conversation - return nothing if
   * the conversation isn't waiting */
  vector<int> getConversationIDs();

  /* Return the DialogMode enumerated value */
  DialogMode getDialogMode();

  /* Retunr the WindowStatus enumerated value */
  WindowStatus getWindowStatus();

  /* Returns the thing IDs from the queued notification(s) - returns nothing if
   * there are no notifications on the waiting stack */
  vector<int> getNotificationIDs();

  /* Initializes a conversation with the two given people. */
  bool initConversation(ConvoPair convo_pair, MapPerson* target,
                        MapThing* source);

  /* Initializes a notification, using a string to be printed */
  bool initNotification(string notification, bool single_line = false,
                        int time_visible = -1);

  /* Initiailizes a pickup notification. These show up isolated from the
   * notification and conversation sequencing */
  bool initPickup(Frame* thing_image, int thing_count = 1,
                  int time_visible = -1);

  /* Returns if there is an active conversation (needs key presses passed in) */
  bool isConversationActive();

  /* Returns if there is a ready conversation just waiting to be displayed */
  bool isConversationReady();

  /* Returns if the conversation is waiting for thing data to be setup */
  bool isConversationWaiting();

  /* Are the rendering images set */
  bool isImagesSet(bool conversation = true, bool pickup = false);

  /* Returns if there is a notification on the waiting to be processed queue */
  bool isNotificationWaiting();

  /* Returns if the class control system is paused */
  bool isPaused();

  /* Key Down/Flush/Up events handled */
  void keyDownEvent(KeyHandler& key_handler);
  void keyFlush();

  /* Loads all appropriate image data for rendering */
  bool loadImageClock(std::string path_start, int frames,
                      std::string path_finish, SDL_Renderer* renderer);
  bool loadImageConversation(string path, SDL_Renderer* renderer);
  bool loadImageDialogShifts(std::string path_next, std::string path_more,
                             SDL_Renderer* renderer);
  bool loadImageNameLeftRight(string path, SDL_Renderer* renderer);
  bool loadImageOptions(string path_circle, string path_triangle,
                        SDL_Renderer* renderer);
  bool loadImagePickupTopBottom(string path, SDL_Renderer* renderer);

  /* Renders the Map Dialog */
  bool render(SDL_Renderer* renderer);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Set the conversation things as per to IDs from getConversationIDs() */
  bool setConversationThings(vector<MapThing*> things);

  /* Sets the event handler */
  void setEventHandler(EventHandler* event_handler);

  /* Set the item data as per all core items */
  void setItemData(std::vector<ItemData> data);

  /* Sets the notification things as per the IDs from getNotificationIDs() */
  bool setNotificationThings(vector<MapThing*> things);

  /* Sets if the class control is paused */
  void setPaused(bool paused);

  /* Updates the map dialog, called on the tick */
  void update(int cycle_time);
};

#endif // MAPDIALOG_H
