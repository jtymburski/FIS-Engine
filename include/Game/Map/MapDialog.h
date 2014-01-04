/*******************************************************************************
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

#include "Frame.h"
#include "Game/EventHandler.h"
#include "Game/Map/MapPerson.h"
#include "Game/Map/MapThing.h"
#include "Helpers.h"
#include "Options.h"
#include "Text.h"

/* Notification queue structure */
struct Notification
{
  std::string text;
  Frame* thing_image;
  uint16_t thing_count;
  uint16_t time_visible;
};

class MapDialog
{
public:
  /* Constructor function */
  MapDialog(Options* running_config = NULL);

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
  /* Animation shifters */
  float animation_cursor;
  bool animation_cursor_up;
  float animation_shifter;
  
  /* The currently running conversation information */
  Conversation conversation_info;
  bool conversation_ready;
  bool conversation_update;
  bool conversation_waiting;

  /* The running dialog mode and status - used for display control */
  DialogMode dialog_mode;
  DialogStatus dialog_status;

  /* Boolean control of text dialog */
  float dialog_offset;
  uint8_t dialog_option;
  uint8_t dialog_option_top;
  //bool dialog_shift_enable;

  /* The event handler information */
  EventHandler* event_handler;

  /* The two render fonts, for displaying any text */
  TTF_Font* font_normal;
  TTF_Font* font_title;

  /* The frame controller for displaying the conversation and pickup */
  Frame frame_convo; 
  Frame frame_pickup;
  
  /* Image frames to be loaded for rendering */
  Frame img_convo; /* Main Dialog display */
  Frame img_convo_m; /* Conversation more to display indicator */
  Frame img_convo_n; /* Conversation next arrow */
  Frame img_name_l; /* Left dialog name display corner */
  Frame img_name_r; /* Right dialog name display corner */
  Frame img_opt_c; /* Option Circle */
  Frame img_opt_d; /* Option Down Arrow */
  Frame img_opt_u; /* Option Up Arrow */
  Frame img_pick_b; /* Bottom pickup display corner */
  Frame img_pick_t; /* Top pickup display corner */
  
  /* The system options, used for rendering, settings, etc. */
  Options* system_options;
  
  /* Used thing data for the current rendering application */
  MapPerson* target;
  MapThing* thing_active;
  std::vector<MapThing*> thing_data;
  
  /* Text rendering pointers */
  float text_index;
  uint16_t text_index_max;
  std::vector<Text*> text_lines;
  std::vector<Text*> text_options;
  std::vector<std::string> text_strings;
  uint16_t text_top;
  bool text_update;

  /* -------------------------- Constants ------------------------- */
  const static uint8_t kBORDER_WIDTH; /* The border width around the dialogs */
  const static float kBUBBLES_ANIMATE; /* The animation speed per bubble */
  const static uint8_t kBUBBLES_COUNT; /* The number of shifting bubbles */
  const static uint8_t kBUBBLES_OFFSET; /* The offset from the bottom */
  const static uint8_t kBUBBLES_SPACING; /* The spacing between bubbles */
  const static uint16_t kCURSOR_ANIMATE; /* The cursor animation time */
  const static uint8_t kCURSOR_HEIGHT; /* The cursor height on animation */
  const static uint8_t kHIGHLIGHT_MARGIN; /* Highlighted option margin pixels */
  const static uint8_t kLINE_SPACING; /* The spacing between lines of font */
  const static uint8_t kMARGIN_SIDES; /* The left and right margin size */
  const static uint8_t kMARGIN_TOP; /* The top margin size */
  const static uint8_t kNAME_BOX_OFFSET; /* Name box dialog x offset */
  const static float kOPACITY_BACKEND; /* Backend display box opacity */
  const static uint8_t kOPTION_OFFSET; /* The offset of the options from text */
  const static float kSHIFT_TIME; /* Time to make the display visible */
  const static uint8_t kTEXT_LINES; /* The max number of lines displayed */
  const static uint8_t kTEXT_OPTIONS; /* The max number of options displayed */
  const static float kTEXT_DISPLAY_SPEED; /* The character display speed */

 /*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Computes all IDs that are needed for displaying the conversation */
  std::vector<int> calculateThingList(Conversation convo);

  /* Clears the vector conversation data */
  void clearConversation(Conversation* convo);
  
  /* Clears all stored pointer data within the class */
  void clearData();

  /* Creates the rendering fonts, based on the system options font path */
  bool createFonts();

  /* Deletes the rendering fonts, if they've been created */
  void deleteFonts();

  /* Executes an event, triggered from a conversation */
  void executeEvent();
  
  /* Functions to acquire thing data, for painting to the screen */
  MapThing* getThingReference(int id);

  /* Render the options. Deletes previous options, if they exist */
  void renderOptions(SDL_Renderer* renderer, 
                     std::vector<std::string> options = {});

  /* Sets the alpha of all rendering textures on the dialog */
  void setAlpha(uint8_t alpha);
  
  /* Sets the conversation - internal to the class */
  void setConversation(Conversation* new_convo = NULL);

  /* Sets up the active conversation pointer to prepare for screen rendering */
  void setupConversation(SDL_Renderer* renderer);

  /* Setup the render text display. Also manages deletion of Text pointers */
  void setupRenderText(std::vector<std::string> lines = {}, 
                       bool delete_old = false);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the thing IDs from the waiting conversation - return nothing if
   * the conversation isn't waiting */
  std::vector<int> getConversationIDs();

  /* Initializes a conversation with the two given people. */
  bool initConversation(Conversation* dialog_info, MapPerson* target);
  bool initConversation(Conversation dialog_info, MapPerson* target);

  /* Returns if there is an active conversation (needs key presses passed in) */
  bool isConversationActive();

  /* Returns if the conversation is waiting for thing data to be setup */
  bool isConversationWaiting();

  /* Are the rendering images set */
  bool isImagesSet(bool conversation = true, bool pickup = false);

  /* Key Down/Flush/Up events handled */
  void keyDownEvent(SDL_KeyboardEvent event);
  void keyFlush();
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Loads all appropriate image data for rendering */
  bool loadImageConversation(std::string path, SDL_Renderer* renderer);
  bool loadImageDialogShifts(std::string path_next, std::string path_more, 
                             SDL_Renderer* renderer);
  bool loadImageNameLeftRight(std::string path, SDL_Renderer* renderer);
  bool loadImageOptions(std::string path_circle, std::string path_triangle, 
                        SDL_Renderer* renderer);
  bool loadImagePickupTopBottom(std::string path, SDL_Renderer* renderer);

  /* Renders the Map Thing */
  bool render(SDL_Renderer* renderer);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);
  
  /* Set the conversation things as per to IDs from getConversationIDs() */
  bool setConversationThings(std::vector<MapThing*> things);

  /* Sets the event handler */
  void setEventHandler(EventHandler* event_handler);

  /* Updates the thing, called on the tick */
  void update(int cycle_time);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
};

#endif // MAPDIALOG_H
