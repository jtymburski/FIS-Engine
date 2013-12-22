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
  /* The frame controller for displaying the conversation and pickup */
  Frame frame_convo; 
  Frame frame_pickup;
  
  /* Image frames to be loaded for rendering */
  Frame img_convo;
  Frame img_name_l;
  Frame img_name_r;
  Frame img_pick_b;
  Frame img_pick_t;
  
  /* The system options, used for rendering, settings, etc. */
  Options* system_options;
  
  /* Used thing data for the current rendering application */
  std::vector<MapThing*> thing_data;
  
  /* -------------------------- Constants ------------------------- */
  //const static float kBUBBLES_ANIMATE; /* The text too long animation time */

 /*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Computes all IDs that are needed for displaying the conversation */
  std::vector<int> calculateThingList(Conversation convo);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Key Down/Flush/Up events handled */
  void keyDownEvent(SDL_KeyboardEvent event);
  void keyFlush();
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Loads all appropriate image data for rendering */
  bool loadImageConversation(std::string path, SDL_Renderer* renderer);
  bool loadImageNameLeft(std::string path, SDL_Renderer* renderer);
  bool loadImageNameRight(std::string path, SDL_Renderer* renderer);
  bool loadImagePickupBottom(std::string path, SDL_Renderer* renderer);
  bool loadImagePickupTop(std::string path, SDL_Renderer* renderer);

  /* Renders the Map Thing */
  bool render(SDL_Renderer* renderer);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);
  
  /* Updates the thing, called on the tick */
  void update(int cycle_time);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
};

#endif // MAPDIALOG_H
