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
 * 1. Add pause function. Similar to map dialog pause
 ******************************************************************************/

#ifndef ITEMSTORE_H
#define ITEMSTORE_H

#include "EnumDb.h"
#include "Frame.h"
#include "Game/EventHandler.h"
#include "Game/Player/Inventory.h"
#include "Options.h"
#include "Text.h"

/* Notification queue structure */
//struct ItemDisplay
//{
//  Item* item;
//  uint32_t count;
//  uint32_t cost;
//  ItemFlags category;
//};

class ItemStore
{
public:
  /* Constructor function */
  ItemStore(Options* running_config = NULL);

  /* Destructor function */
  ~ItemStore();

  /* The item store mode which defines what the operation of the item selection
   * will do:
   * VIEW - just view the items (possibly use?)
   * BUY - player can buy the selected items
   * SELL - player can sell the selected items
   * TAKE - player can take the selected items
   */
   enum StoreMode{VIEW, BUY, SELL, TAKE};

private:
  /* The event handler information */
  EventHandler* event_handler;

  /* Rendered frames, that are displayed */
  Frame frame_main;
  bool frame_setup;

  /* The necessary rendering fonts, for displaying all text */
  TTF_Font* font_title;

  /* Image frames to be loaded for rendering */
  Frame img_backend_left;
  Frame img_backend_right;

  /* Active inventory being displayed by the store view */
  Inventory* inventory_active;
  std::vector<std::pair<Item*, bool>> inventory_items;

  /* The stack of items available in the store view */
  uint8_t store_alpha;
  StoreMode store_mode;
  //std::vector<ItemDisplay> store_stack;
  WindowStatus store_status;
  std::string store_title;

  /* The system options, used for rendering, settings, etc */
  Options* system_options;

  /* -------------------------- Constants ------------------------- */
  const static uint8_t kALPHA_MAX; /* The max alpha rating for frames */
  const static uint16_t kBACKEND_RIGHT_X; /* Right backend x offset render */
  const static uint16_t kBACKEND_RIGHT_Y; /* Right backend y offset render */
  const static uint8_t kBACKEND_TITLE_HEIGHT; /* Backend title render box */
  const static uint8_t kGRID_ALPHA; /* The grid alpha rating of white */
  const static uint8_t kGRID_HEIGHT; /* The grid height, in tile sections */
  const static uint8_t kGRID_SIZE; /* The grid coordinate size, in pixels */
  const static uint8_t kGRID_WIDTH; /* The grid width, in tile sections */
  const static uint16_t kSHOW_TIME; /* The time to hide or show the store */
  const static uint8_t kTILE_SIZE; /* The tile size, to fit in the grid */

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Clears all stored pointer data within the class */
  void clearData();

  /* Creates the rendering fonts, based on the system options font path */
  bool createFonts();

  /* Deletes the rendering fonts, if they've been created */
  void deleteFonts();

  /* Sets the alpha of all rendering textures on the dialog */
  void setAlpha(uint8_t alpha);

  /* Sets up the views, to be rendered on the screen */
  void setupMainView(SDL_Renderer* renderer, std::string title = "");
  void setupSecondaryView(SDL_Renderer* renderer);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Closes the active display */
  void close();

  /* Initializes the item store display */
  bool initDisplay(StoreMode mode, std::vector<Item*> items,
                   std::vector<uint32_t> counts,
                   std::vector<int32_t> cost_modifiers, std::string name = "");

  /* Returns if the item store is currently active */
  bool isActive();

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
};

#endif // ITEMSTORE_H
