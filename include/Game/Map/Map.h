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
#include "Game/KeyHandler.h"
#include "Game/EventHandler.h"
#include "Game/Map/ItemStore.h"
#include "Game/Map/MapDialog.h"
#include "Game/Map/MapInteractiveObject.h"
#include "Game/Map/MapItem.h"
#include "Game/Map/MapNPC.h"
#include "Game/Map/MapPerson.h"
// #include "Game/Map/MapStatusBar.h"
#include "Game/Map/MapThing.h"
#include "Game/Map/MapViewport.h"
// #include "Game/Map/Sector.h"
#include "Game/Map/Tile.h"
#include "Game/Lay.h"
// #include "Helpers.h"
// #include "Options.h"
// #include "Sprite.h"

/* Sub map structure - contains all data related only to each sub */
struct SubMap
{
  /* Tile data */
  std::vector<std::vector<Tile*>> tiles;

  /* Thing data (and children) */
  std::vector<MapInteractiveObject*> ios;
  std::vector<MapItem*> items;
  std::vector<MapPerson*> persons;
  std::vector<MapThing*> things;

  /* Lay data */
  std::vector<LayOver> underlays;
  std::vector<LayOver> overlays;

  /* Sound data */
  std::vector<uint32_t> battles;
  std::vector<uint32_t> music;
  int32_t weather;
};

/* Class for map control */
class Map
{
public:
  /* Constructor function */
  Map(Options* running_config = nullptr, EventHandler* event_handler = nullptr);

  /* Destructor function */
  ~Map();

  /* The map mode operator, for controlling the visible widget */
  enum MapMode
  {
    DISABLED = 0,
    NORMAL = 1,
    SWITCHSUB = 2,
    VIEW = 3,
    NONE = 4
  };

private:
  /* Base things */
  std::vector<MapInteractiveObject*> base_ios;
  std::vector<MapItem*> base_items;
  std::vector<MapPerson*> base_persons;
  std::vector<MapThing*> base_things;

  /* The base system path to the resources */
  std::string base_path;

  /* Battling things/information */
  EventPair battle_eventlose;
  EventPair battle_eventwin;
  BattleFlags battle_flags;
  MapPerson* battle_person;
  std::vector<uint32_t> battle_scenes;
  MapThing* battle_thing;
  bool battle_trigger;

  /* The overall color control mode - only applies map side */
  ColorMode color_mode;

  /* A reference blank event for setting events in the game */
  EventHandler* event_handler;

  /* Fade status */
  uint8_t fade_alpha;
  uint16_t fade_delay;
  float fade_delta;
  bool fade_main;
  MapFade fade_status;

  /* Images used in rendering */
  Frame img_spotted;

  /* The item store menu */
  ItemStore item_menu;

  /* Lay data from animation triggers */
  int lay_acc; // TODO
  MapFade lay_fade; // TODO
  int lay_fade_index; // TODO
  std::vector<Lay*> lay_images; // TODO
  int lay_time; // TODO

  /* Map lays */
  std::vector<Lay*> lay_overs;
  std::vector<Lay*> lay_unders;

  /* Indication if the map has been loaded */
  bool loaded;

  /* The menus and dialogs on top of the map */
  MapDialog map_dialog;

  /* Map reference index for current sub-map */
  uint16_t map_index;
  int map_index_next;

  /* Map parsing modes */
  MapMode mode_curr;
  MapMode mode_next;

  /* Music ID Stack */
  int music_id;
  int music_runtime;

  /* Map name */
  std::string name;
  Text name_text;
  uint16_t name_view;

  /* Active player reference - controlled by keyboard (or gamepad) */
  MapPerson* player;

  // /* The sectors on the map (for rooms, caves, houses etc) */
  // QList<Sector> sectors;

  /* Speed factor - for map based elements */
  float speed_factor;

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

  /* View mode data */
  int view_acc;
  uint16_t view_section;
  bool view_start;
  MapThing* view_thing;
  Tile* view_tile;
  int view_time;
  bool view_travel;

  /* The viewport for the map, controls positioning */
  MapViewport viewport;

  // /* Weather effect on the overall map (May be pushed to the sector level at
  // * a later time) */
  // Weather* weather_effect;

  /* Status of the zoom on the map */
  bool zooming;
  uint16_t zoom_size;

  /*------------------- Constants -----------------------*/
  const static float kFADE_FACTOR; /* 1/x fade factor for ms cycle time */
  const static uint8_t kFADE_HOLD; /* The hold point in opacity to delay */
  const static uint16_t kFADE_HOLD_DELAY; /* The ms to delay at the hold */
  const static uint8_t kFADE_MAX; /* The max differential to fade delta */
  const static uint8_t kFADE_VIS; /* The fully visible faded in screen alpha */
  const static uint8_t kFILE_CLASSIFIER; /* The file tile classification text */
  const static uint8_t kFILE_GAME_TYPE;  /* The game type classifier */
  const static uint8_t kFILE_SECTION_ID; /* The section identifier, for file */
  const static uint8_t kFILE_TILE_COLUMN; /* The tile depth in XML of column */
  const static uint8_t kFILE_TILE_ROW; /* The tile depth in XML of row */
  const static uint8_t kMAX_U8BIT; /* Max unsigned 8 bit */
  const static uint32_t kMUSIC_REPEAT; /* Milliseconds each song repeats */
  const static uint16_t kNAME_DISPLAY; /* Time to display map name */
  const static uint16_t kNAME_FADE; /* The time remaining to fade out */
  const static uint8_t kNAME_SIZE; /* Size of name font to render */
  const static float kNAME_X; /* The top left x location of text */
  const static float kNAME_Y; /* The top left y location of text */
  const static uint8_t kPLAYER_ID; /* The player ID for computer control */
  const static uint16_t kZOOM_TILE_SIZE; /* The tile size, when zoomed out */

public:
    /*------------------- Public Constants -----------------------*/
  const static uint16_t kSNAPSHOT_W; /* The snapshot rect width */
  const static uint16_t kSNAPSHOT_H; /* The snapshot rect height */

/*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Adds sprite data, as per data from the file */
  bool addSpriteData(XmlData data, std::string id, int file_index,
                     SDL_Renderer* renderer, std::string base_game_path);

  /* Adds tile data, as per data from the file */
  bool addTileData(XmlData data, uint16_t section_index);

  /* Adds thing data, as per data from the file */
  bool addThingBaseData(XmlData data, int file_index, SDL_Renderer* renderer,
                        std::string base_game_path);
  bool addThingData(XmlData data, uint16_t section_index,
                    SDL_Renderer* renderer, bool from_save = false);

  /* Audio start/stop triggers */
  void audioStart();
  void audioStop();
  void audioUpdate(bool sub_change = false);

  /* Change the mode that the game is running */
  bool changeMode(MapMode mode);

  /* Returns the color mode from the active set of data */
  ColorMode getColorMode();

  /* Returns the interactive object, based on the ID */
  MapInteractiveObject* getIOBase(uint32_t id);

  /* Returns the item, based on the ID */
  MapItem* getItem(uint32_t id, int sub_id = -1);
  MapItem* getItemBase(uint32_t id);

  /* Returns the next item drop ID */
  int getNextItemID();

  /* Returns the person, based on the ID */
  MapPerson* getPersonBase(uint32_t id);

  /* Returns the thing, based on the ID */
  MapThing* getThing(uint32_t id, int sub_id = -1);
  MapThing* getThingBase(uint32_t id);

  /* Returns the general things based on type or ID. This searches all pools
   * of things */
  MapThing* getThingGeneral(uint32_t id, int sub_id = -1);
  MapThing* getThingGeneral(uint32_t id, ThingBase type, int sub_id = -1);

  /* Returns a stack of map things that correspond to the ID stack */
  std::vector<MapThing*> getThingData(std::vector<int> thing_ids);

  /* Returns a matrix of tiles that match the frames in the thing */
  std::vector<std::vector<Tile*>>
                getTileMatrix(MapThing* thing,
                              Direction direction = Direction::DIRECTIONLESS,
                              bool start_only = false);
  std::vector<std::vector<Tile*>> getTileMatrix(uint16_t section, uint16_t x,
                                                uint16_t y, uint16_t width,
                                                uint16_t height);

  /* Initiates a lay over change of the map. Triggered on section change */
  void initiateLayUpdate();

  /* Initiates a section block of map. Triggered from the file data */
  bool initiateMapSection(uint16_t section_index, int width, int height);

  /* Initiates a check for NPC if a forced interaction will occur */
  bool initiateNPCInteraction();

  /* Initiates a thing action, based on the action key being hit */
  void initiateThingInteraction(MapPerson* initiator);

  /* Returns if the color is currently in a transition status */
  bool isColorTransitioning();

  /* Mode view updates */
  bool modeViewStart(int cycle_time, bool travel);
  bool modeViewStop(int cycle_time, bool travel);

  /* Move thing sections. Strictly handles switching the array where a thing
   * can be found. This will not handle x, y changes of location */
  MapThing* moveThing(uint32_t thing_id, uint16_t section_old);
  MapThing* moveThing(MapThing* thing_ref, uint16_t section_old);

  /* Parse coordinate info from file to give the designated tile coordinates
   * to update */
  bool parseCoordinateInfo(std::string row, std::string col, uint16_t index,
                           uint16_t* r_start, uint16_t* r_end,
                           uint16_t* c_start, uint16_t* c_end);

  /* Save the passed in sub map based on the map ID and other information */
  bool saveSubMap(FileHandler* fh, const uint32_t &id = 0,
                  const std::string &wrapper = "section",
                  const bool &write_id = true);
  bool saveTiles(FileHandler* fh, SubMap* sub_map);
  bool saveTileSet(FileHandler* fh, SubMap* sub_map,
            const std::vector<std::pair<uint32_t, std::vector<uint32_t>>> &set,
            const bool &enter = true, const std::string &type_txt = "set");

  /* Sets and updates the color mode as per input and status available */
  void setColorMode(ColorMode mode);

  /* Changes the map section index - what is displayed */
  bool setSectionIndex(uint16_t index);
  bool setSectionIndexMode(int index_next = -1);

  /* Sets the starting (and next) tiles of a newly generated thing */
  bool setTiles(MapThing* ref);

  /* Splits the ID into a vector of IDs */
  std::vector<std::vector<int32_t>> splitIdString(std::string id,
                                                  bool matrix = false);

  /* Lay triggers based on passed in information */
  bool triggerLay(std::string path,
                  int anim_time = Sprite::kDEFAULT_ANIMATE_TIME,
                  float velocity_x = 0.0, float velocity_y = 0.0,
                  int lay_time = -1, bool force = false);
  bool triggerLayFinish(bool force = false);
  bool triggerLays(std::vector<LayOver> lay_data, int lay_time = -1,
                   bool force = false);
  bool triggerLays(std::vector<Lay*> lay_ptrs, int lay_time = -1,
                   bool force = false);

  /* Triggers a view of the passed in data */
  bool triggerViewFinish();
  bool triggerViewThing(MapThing* view_thing, UnlockView view_mode,
                        int view_time);
  bool triggerViewTile(Tile* view_tile, uint16_t view_section,
                       UnlockView view_mode, int view_time);

  /* Updates the map mode - lots of logic here */
  bool updateFade(int cycle_time);
  void updateMode(int cycle_time);

  /* Update the Running state of the Player */
  void updatePlayerRunState(KeyHandler& key_handler);

  /* Updates the height and width, based on zoom factors */
  void updateTileSize(bool force = false);

  /* Zoom trigger */
  void zoom(uint16_t tile_size = kZOOM_TILE_SIZE, bool force = false);
  void zoomRestore(bool force = false);

/*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Battle won/loss/end trigger for map */
  void battleLose();
  void battleRun();
  void battleWon();

  /* Disable interaction by NPCs with the player */
  void disableInteraction(bool disable);

  /* Drops the selected item under the players feet */
  bool dropItem(uint32_t id, uint32_t count);

  /* Enable view trigger */
  void enableView(bool enable, bool map_change = false);

  /* Returns the battle information */
  EventPair getBattleEventLose();
  EventPair getBattleEventWin();
  BattleFlags getBattleFlags();
  int getBattlePersonID();
  int getBattleScene();
  int getBattleThingID();

  /* Return the enumerated state of the DialogMode */
  DialogMode getDialogMode();

  /* Returns the current enumerated WindowState of the Map dialog */
  WindowStatus getDialogStatus();

  /* The fade status of the map */
  MapFade getFadeStatus();

  /* Returns the interactive object, based on the ID */
  MapInteractiveObject* getIO(uint32_t id, int sub_id = -1);

  /* Returns the string name of the map */
  std::string getName();

  /* Return a MapPerson */
  MapPerson* getPerson(uint32_t id, int sub_id = -1);

  /* Returns the number of steps the player has used on map */
  uint32_t getPlayerSteps();

  /* Returns the rect (in pixels) snapshot of the map viewport */
  SDL_Rect getSnapshotRect();

  /* Initiates a battle, within the map */
  bool initBattle(MapPerson* person, MapThing* source, BattleFlags flags,
                  EventPair event_win, EventPair event_lose);

  /* Initiates a conversation, within the map. */
  bool initConversation(ConvoPair convo_pair, MapThing* source);

  /* Initiates a notification, within the map (either string or image based) */
  bool initNotification(std::string notification);
  bool initNotification(Frame* image, int count);

  /* Initializes item store display, within the map */
  bool initStore(ItemStore::StoreMode mode, std::vector<Item*> items,
                 std::vector<uint32_t> counts,
                 std::vector<int32_t> cost_modifiers, std::string name = "",
                 bool show_empty = false);

  /* Returns battle flags and properties */
  bool isBattleLoseGameOver();
  bool isBattleReady();
  bool isBattleRestoreHealth();
  bool isBattleRestoreQD();
  bool isBattleWinDisappear();

  /* Returns if the map has been currently loaded with data */
  bool isLoaded();

  /* Mode checks - only returns true for normal if normal and visible, reverse
   * for disabled */
  bool isModeDisabled();
  bool isModeNormal();
  bool isModeView();

  /* The key up and down events to be handled by the class */
  bool keyDownEvent(KeyHandler& key_handler);
#ifdef UDEBUG
  void keyTestDownEvent(SDL_KeyboardEvent event);
#endif
  void keyUpEvent(KeyHandler& key_handler);

  /* Loads the map data */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer,
                std::string base_game_path, bool from_save = false);
  void loadDataFinish(SDL_Renderer* renderer);

  /* Modify thing properties based on passed in properties */
  void modifyThing(MapThing* source, ThingBase type, int id,
                   ThingProperty props, ThingProperty bools, int respawn_int,
                   int speed_int, TrackingState track_enum, int inactive_int);

  /* Picks up the total number of the item */
  bool pickupItem(MapItem* item, int count = -1);

  /* Renders the map */
  bool render(SDL_Renderer* renderer);

  /* Resets the player steps */
  void resetPlayerSteps();

  /* Saves the current map data to the active file handling pointer location */
  bool saveData(FileHandler* fh);

  /* Seta and correlates the base items within the map to the core data */
  bool setBaseItems(std::vector<ItemData> items, SDL_Renderer* renderer);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the MapDialog paused state */
  void setDialogPaused(bool paused);

  /* Sets the operational event handler */
  void setEventHandler(EventHandler* event_handler);

  /* Sets the speed factor of update call on the map side */
  bool setSpeedFactor(float factor = 1.0);

  /* Teleport a thing, based on the given coordinates */
  void teleportThing(int id, int tile_x, int tile_y, int section_id);

  /* Handles all the necessary clean up when map focus is lost */
  void unfocus();

  /* Unload the map, if there is one loaded */
  void unloadMap();

  /* Unlock triggers, based on parameter information */
  void unlockIO(MapThing* source, int io_id, UnlockIOMode mode, int state_num,
                UnlockIOEvent mode_events, UnlockView mode_view, int view_time);
  void unlockThing(MapThing* source, int thing_id, UnlockView mode_view,
                   int view_time);
  void unlockTile(int section_id, int tile_x, int tile_y, UnlockTileMode mode,
                  UnlockView mode_view, int view_time);

  /* Updates the game state */
  bool update(int cycle_time);
};

#endif // MAP_H
