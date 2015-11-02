/******************************************************************************
 * Class Name: Map
 * Date Created: Oct 28 2012
 * Inheritance: none
 * Description: The map class, this is the top level with regard to an actual
 *              in-game map. This contains all the tiles that a map is composed
 *              of, it also holds pointers to all of the NPC's contained in the
 *              maps tiles. This also handles the viewport for the map.
 ******************************************************************************/
#ifndef MAP_H
#define MAP_H

#include <vector>

#include "FileHandler.h"
#include "Game/EventHandler.h"
#include "Game/Map/ItemStore.h"
#include "Game/Map/MapDialog.h"
#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapItem.h"
// #include "Game/Map/MapMenu.h"
#include "Game/Map/MapNPC.h"
#include "Game/Map/MapPerson.h"
// #include "Game/Map/MapStatusBar.h"
#include "Game/Map/MapThing.h"
#include "Game/Map/MapViewport.h"
// #include "Game/Map/Sector.h"
#include "Game/Map/Tile.h"
// #include "Game/Weather.h"
#include "Helpers.h"
#include "Options.h"
#include "Sprite.h"

/* Sub map structure - contains all data related only to each sub */
// TODO: Move thing instances into this struct
struct SubMap
{
  /* Tile data */
  std::vector<std::vector<Tile*>> tiles;

  /* Sound data */
  std::vector<uint32_t> music;
  int32_t weather;
};

/* Class for map control */
class Map
{
public:
  /* Constructor function */
  Map(Options* running_config = NULL, EventHandler* event_handler = NULL);

  /* Destructor function */
  ~Map();

private:
  /* Base things */
  std::vector<MapInteractiveObject*> base_ios;
  std::vector<MapItem*> base_items;
  std::vector<MapPerson*> base_persons;
  std::vector<MapThing*> base_things;

  /* The base system path to the resources */
  std::string base_path;

  /* A reference blank event for setting events in the game */
  EventHandler* event_handler;

  /* The item store menu */
  ItemStore item_menu;

  /* Indication if the map has been loaded */
  bool loaded;

  /* The menus and dialogs on top of the map */
  MapDialog map_dialog;
  // MapMenu map_menu;

  /* Map reference index for current sub-map */
  uint16_t map_index;

  // /* The status bar on the map */
  // MapStatusBar map_status_bar; // TODO: Remove

  /* Music ID Stack */
  int music_id;
  int music_runtime;

  // /* The players info on the map */
  std::vector<MapInteractiveObject*> ios;
  std::vector<MapItem*> items;
  std::vector<MapPerson*> persons;
  MapPerson* player; /* The actively controlled player */
  std::vector<MapThing*> things;

  // /* The sectors on the map (for rooms, caves, houses etc) */
  // QList<Sector> sectors;

  /* The set of map data */
  std::vector<SubMap> sub_map;

  /* The system options, used for rendering, settings, etc. */
  Options* system_options;

  /* The number of pixels for each tile in the class */
  uint16_t tile_height;
  uint16_t tile_width;

  /* Tile Sprites */
  std::vector<Sprite*> tile_sprites;

  // /* The time that has elapsed for each draw cycle */
  // int time_elapsed;

  /* The viewport for the map, controls positioning */
  MapViewport viewport;

  // /* Weather effect on the overall map (May be pushed to the sector level at
   // * a later time) */
  // Weather* weather_effect;

  /* Status of the zoom on the map */
  bool zoom_in;
  bool zoom_out;

  /*------------------- Constants -----------------------*/
  const static uint8_t kFILE_CLASSIFIER; /* The file tile classification text */
  const static uint8_t kFILE_GAME_TYPE; /* The game type classifier */
  const static uint8_t kFILE_SECTION_ID; /* The section identifier, for file */
  const static uint8_t kFILE_TILE_COLUMN; /* The tile depth in XML of column */
  const static uint8_t kFILE_TILE_ROW; /* The tile depth in XML of row */
  const static uint32_t kMUSIC_REPEAT; /* Milliseconds each song repeats */
  const static uint8_t kPLAYER_ID;     /* The player ID for computer control */
  const static uint16_t kZOOM_TILE_SIZE;  /* The tile size, when zoomed out */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds sprite data, as per data from the file */
  bool addSpriteData(XmlData data, std::string id,
                     int file_index, SDL_Renderer* renderer);

  /* Adds tile data, as per data from the file */
  bool addTileData(XmlData data, uint16_t section_index);

  /* Adds thing data, as per data from the file */
  bool addThingBaseData(XmlData data, int file_index, SDL_Renderer* renderer);
  bool addThingData(XmlData data, uint16_t section_index,
                    SDL_Renderer* renderer);

  /* Audio start/stop triggers */
  void audioStart();
  void audioStop();
  void audioUpdate(bool sub_change = false);

  /* Returns the item, based on the ID */
  MapItem* getItem(uint16_t id);
  MapItem* getItemBase(uint16_t id);

  /* Returns the interactive object, based on the ID */
  MapInteractiveObject* getIO(uint16_t id);
  MapInteractiveObject* getIOBase(uint16_t id);

  /* Returns the person, based on the ID */
  MapPerson* getPerson(uint16_t id);
  MapPerson* getPersonBase(uint16_t id);

  /* Returns the thing, based on the ID */
  MapThing* getThing(uint16_t id);
  MapThing* getThingBase(uint16_t id);

  /* Returns a stack of map things that correspond to the ID stack */
  std::vector<MapThing*> getThingData(std::vector<int> thing_ids);

  /* Returns a matrix of tiles that match the frames in the thing */
  std::vector<std::vector<Tile*>> getTileMatrix(
               MapThing* thing, Direction direction = Direction::DIRECTIONLESS);
  std::vector<std::vector<Tile*>> getTileMatrix(uint16_t section,
                                                uint16_t x, uint16_t y,
                                                uint16_t width,
                                                uint16_t height);

  /* Initiates a section block of map. Triggered from the file data */
  bool initiateMapSection(uint16_t section_index, int width, int height);

  /* Initiates a thing action, based on the action key being hit */
  void initiateThingInteraction(MapPerson* initiator);

  /* Parse coordinate info from file to give the designated tile coordinates
   * to update */
  bool parseCoordinateInfo(std::string row, std::string col, uint16_t index,
                           uint16_t* r_start, uint16_t* r_end,
                           uint16_t* c_start, uint16_t* c_end);

  /* Changes the map section index - what is displayed */
  bool setSectionIndex(uint16_t index);

  /* Splits the ID into a vector of IDs */
  std::vector< std::vector<int32_t> > splitIdString(std::string id,
                                                    bool matrix = false);

  /* Updates the height and width, based on zoom factors */
  void updateTileSize();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Enable view trigger */
  void enableView(bool enable);

  // /* Returns the map person, for access */
  // MapPerson* getPlayer();

  // /* Returns the map viewport, for displaying the scene to the screen */
  // MapViewport* getViewport();

  // /* Initial call when map is displayed */
  // void initialization();

  /* Initiates a conversation, within the map. */
  bool initConversation(Conversation* convo, MapThing* source);

  /* Initiates a notification, within the map (either string or image based) */
  bool initNotification(std::string notification);
  bool initNotification(Frame* image, int count);

  /* Initializes item store display, within the map */
  bool initStore(ItemStore::StoreMode mode, std::vector<Item*> items,
                 std::vector<uint32_t> counts,
                 std::vector<int32_t> cost_modifiers,
                 std::string name = "", bool show_empty = false);

  // /* Checks whether the viewport contains any tiles with the given sector */
  // bool isInSector(int index);

  /* Returns if the map has been currently loaded with data */
  bool isLoaded();

  /* The key up and down events to be handled by the class */
  bool keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Loads the map data */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer,
                std::string base_path);
  void loadDataFinish(SDL_Renderer* renderer);

  /* Picks up the total number of the item */
  bool pickupItem(MapItem* item, int count = -1);

  /* Renders the map */
  bool render(SDL_Renderer* renderer);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the operational event handler */
  void setEventHandler(EventHandler* event_handler);

  /* Teleport a thing, based on the given coordinates */
  void teleportThing(int id, int tile_x, int tile_y, int section_id);

  /* Handles all the necessary clean up when map focus is lost */
  void unfocus();

  /* Unload the map, if there is one loaded */
  void unloadMap();

  /* Updates the game state */
  bool update(int cycle_time);
};

#endif // MAP_H
