/*******************************************************************************
 * Class Name: ItemStore [Header]
 * Date Created: January 11, 2014
 * Inheritance: None
 * Description: This view is the item storage / access that appears on the map.
 *              It allows for direct access to items which can be useful for
 *              shops, inventories, chests, etc. Contains the appropriate API
 *              for setting up a view and then it runs off of update, render, 
 *              and keyPressEvents. 
 *
 * Notes
 * -----
 *
 * [1]:
 *
 * TODO
 * ----
 ******************************************************************************/

#ifndef ITEMSTORE_H
#define ITEMSTORE_H

#include "Frame.h"
#include "Game/EventHandler.h"
#include "Options.h"
#include "Text.h"

class ItemStore
{
public:
  /* Constructor function */
  ItemStore(Options* running_config = NULL);

  /* Destructor function */
  ~ItemStore();

private:
  /* The event handler information */
  EventHandler* event_handler;

  /* Rendered frames, that are displayed */
  Frame frame_main;

  /* The necessary rendering fonts, for displaying all text */
  TTF_Font* font_title;

  /* Image frames to be loaded for rendering */
  Frame img_backend_left;
  Frame img_backend_right;

  /* The system options, used for rendering, settings, etc */
  Options* system_options;

  /* -------------------------- Constants ------------------------- */
  const static uint16_t kBACKEND_RIGHT_X; /* Right backend x offset render */
  const static uint16_t kBACKEND_RIGHT_Y; /* Right backend y offset render */
  const static uint8_t kBACKEND_TITLE_HEIGHT; /* Backend title render box */
  const static uint8_t kGRID_HEIGHT; /* The grid height, in tile sections */
  const static uint8_t kGRID_WIDTH; /* The grid width, in tile sections */
  const static uint8_t kTILE_SIZE; /* The tile size, to fit in the grid */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Clears all stored pointer data within the class */
  void clearData();

  /* Creates the rendering fonts, based on the system options font path */
  bool createFonts();

  /* Deletes the rendering fonts, if they've been created */
  void deleteFonts();

  /* Sets up the views, to be rendered on the screen */
  void setupMainView(SDL_Renderer* renderer, std::string title = "");
  void setupSecondaryView(SDL_Renderer* renderer);

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Key Down/Up events handled */
  void keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Loads the image backend for the item store window */
  bool loadImageBackend(std::string left, std::string right, 
                                          SDL_Renderer* renderer);

  /* Renders the Item Store */
  bool render(SDL_Renderer* renderer);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the event handler */
  void setEventHandler(EventHandler* event_handler);

  /* Updates the item store, called on the tick */
  void update(int cycle_time);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

};

#endif // ITEMSTORE_H
