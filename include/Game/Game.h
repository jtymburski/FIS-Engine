/*******************************************************************************
 * Class Name: Game
 * Date Created: Dec 2 2012
 * Inheritance: none
 * Description: This class is the top layer that handles all interclass
 *              communication between map, player, and battle as well as all
 *              setup and overall insight and control. The painting control
 *              won't be handled here and this will just act as an intermediate
 *              data highway / event handler.
 *
 *
 * // TODO: Starting inventory items [01-11-14]
 ******************************************************************************/
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <chrono>
#include <memory>

// #include "Game/Battle/AIModuleTester.h"
#include "Game/Battle/Battle.h"
#include "Game/EventHandler.h"
#include "Game/Map/Map.h"
#include "Game/Map/Menu.h"
#include "Game/Player/Player.h"
#include "Game/Player/Bubby.h"
#include "Game/Player/Inventory.h"
#include "Options.h"
#include "SoundHandler.h"

using std::begin;
using std::end;

class Game
{
public:
  /* Constructor function */
  Game(Options* running_config = nullptr);

  /* Destructor function */
  ~Game();

  enum LoadMode
  {
    NOLOAD = 0,
    SUBLOAD = 1,
    FULLLOAD = 2
  };

  /* The game mode operator, for controlling the visible widget */
  enum GameMode
  {
    DISABLED       = 0,
    MAP            = 1,
    BATTLE         = 2,
    VICTORY_SCREEN = 3,
    LOADING        = 4,
    MENU           = 5, /* Processing paused for Map */
    NONE           = 6
  };

private:
  /* The active rendering engine */
  SDL_Renderer* active_renderer;

  /* The computed base path for resources in the application */
  std::string base_path;

  /* Battle control and visual */
  Battle* battle_ctrl;

  /* Structure for containing BattleDisplay Data */
  BattleDisplayData* battle_display_data;

  /* The configuration for the display of the game */
  Options* config;

  /* Handles all events throughout the game. */
  EventHandler event_handler;

  /* Game file path */
  std::string game_path;

  /* List of objects */
  std::vector<Action*> list_action;
  std::vector<BattleScene> list_battles;
  std::vector<Category*> list_class;
  std::vector<Flavour*> list_flavour;
  std::vector<Item*> list_item;
  std::vector<Party*> list_party;
  std::vector<Person*> list_person_base;
  std::vector<Person*> list_person_inst;
  std::vector<Category*> list_race;
  std::vector<SkillSet*> list_set;
  std::vector<Skill*> list_skill;

  /* Is the game loaded? */
  bool loaded_core;
  bool loaded_sub;

  /* Map variables */
  Map map_ctrl; /* Main class */
  int map_lvl; /* Active level number */

  /* Map Menu */
  Menu map_menu;
  bool map_menu_enabled;

  /* The mode that the game is currently running at */
  GameMode mode;
  LoadMode mode_load;
  GameMode mode_next;

  /* The player */
  Player* player_main;

  /* Current save properties */
  FileHandler save_handle;
  uint8_t save_slot;

  /* Number of ticks since inception */
  uint64_t ticks_total;

  /* ------------ Constants --------------- */
public:
  static const std::string kSAVE_PATH_BACK; /* Back of save path */
  static const std::string kSAVE_PATH_FRONT; /* Front of save path */
  static const uint8_t kSAVE_SLOT_MAX; /* Max number of save slots */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Add functions for game objects */
  Action* addAction(const std::string &raw);
  BattleScene* addBattleScene(const int32_t &id);
  Category* addClass(const int32_t &id);
  Flavour* addFlavour(const int32_t &id);
  Item* addItem(const int32_t &id, SortObjects type = SortObjects::ITEMS);
  Party* addParty(const int32_t &id);
  Person* addPersonBase(const int32_t &id);
  Person* addPersonInst(const int32_t &base_id, const uint32_t &lvl);
  Person* addPersonInst(Person* base_person, const uint32_t &lvl);
  bool addPersonToParty(Party* ref_party, const int32_t &base_id,
                        const uint32_t index = 0, const uint32_t &lvl = 1);
  Category* addRace(const int32_t &id);
  Skill* addSkill(const int32_t &id);
  SkillSet* addSkillSet(const int32_t &id);

  /* Change the mode that the game is running */
  bool changeMode(GameMode mode, bool map_change = false);

  /* A give item event, based on an ID and count (triggered from stored event */
  int eventGiveItem(int id, int count);

  /* Initiates a conversation event */
  void eventInitConversation(ConvoPair convo_pair, MapThing* source);

  /* Initiates a notification event (in map) */
  void eventInitNotification(std::string notification);

  /* The pickup item event - from walking over or triggering from action key */
  void eventPickupItem(MapItem* item, bool walkover);

  /* The property modifier event */
  void eventPropMod(MapThing* source, ThingBase type, int id,
                    ThingProperty props, ThingProperty bools,
                    int respawn, int speed, TrackingState track,
                    int inactive);

  /* Show/Hide the Menu */
  bool eventMenuShow();
  bool eventMenuHide();

  /* Starts a battle event. Using the given information */
  bool eventStartBattle(int person_id, int source_id);

  /* Switch maps event. - utilizing a map ID */
  void eventSwitchMap(int map_id);

  /* A take item event, based on an ID and count (triggered from event) */
  int eventTakeItem(int id, int count);

  /* Teleport thing event, based on ID and coordinates */
  void eventTeleportThing(int thing_id, int x, int y, int section_id);

  /* Trigger IO event, based on the IO object and interaction state */
  void eventTriggerIO(MapInteractiveObject* io, int interaction_state,
                      MapPerson* initiator);

  /* Unlock events, based on parameter information */
  void eventUnlockIO(MapThing* source, int io_id, UnlockIOMode mode,
                     int state_num, UnlockIOEvent mode_events,
                     UnlockView mode_view, int view_time);
  void eventUnlockThing(MapThing* source, int thing_id, UnlockView mode_view,
                        int view_time);
  void eventUnlockTile(int section_id, int tile_x, int tile_y,
                       UnlockTileMode mode, UnlockView mode_view,
                       int view_time);

  /* Load game */
  bool load(std::string base_file, SDL_Renderer* renderer,
            std::string inst_file = "", bool encryption = false,
            bool full_load = true);

  /* Load game data */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer);

  /* Parse lock and attempt unlock */
  bool parseLock(Locked& lock_struct);

  /* Polls events running from the event handler. Triggered from other places
   * in the game */
  void pollEvents();

  /* Remove functions for game objects */
  void removeActions();
  void removeAll(); /* Properly staged remove all call */
  void removeBattleScenes();
  void removeClasses();
  void removeFlavours();
  void removeItems();
  void removeParties();
  void removePersonBases();
  bool removePersonInstance(Person* const person_inst);
  void removePersonInstances();
  void removeRaces();
  void removeSkills();
  void removeSkillSets();

  /* Update mode */
  void updateMode(int cycle_time);
  void updateMenuEnabledState();
  void updatePlayerSteps();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Enable view trigger */
  void enableView(bool enable);

  /* Getter functions for game objects */
  Action* getAction(const int32_t &index, const bool &by_id = true);
  BattleScene* getBattleScene(const int32_t &index, const bool &by_id = true);
  Category* getClass(const int32_t &index, const bool &by_id = true);
  Flavour* getFlavour(const int32_t &index, const bool &by_id = true);
  Inventory* getInventory(const int32_t &id);
  Inventory* getInvBearacks();
  Inventory* getInvSleuth();
  Item* getItem(const int32_t &index, const bool &by_id = true);
  Party* getParty(const int32_t &index, const bool &by_id = true);
  Person* getPersonBase(const int32_t &index, const bool &by_id = true);
  Person* getPersonInst(const int32_t &index, const bool &by_id = true);
  Category* getRace(const int32_t &index, const bool &by_id = true);
  Skill* getSkill(const int32_t &index, const bool &by_id = true);
  SkillSet* getSkillSet(const int32_t &index, const bool &by_id = true);

  /* Gets the handler */
  EventHandler& getHandler();

  /* Get the game mode */
  GameMode getMode();

  /* Is the game loaded */
  //bool isLoaded();
  bool isLoadedCore();
  bool isLoadedSub();

  /* Mode checks - only returns true for ready if ready for control, reverse
   * for disabled */
  bool isModeDisabled();
  bool isModeReady();

  /* The key up and down events to be handled by the class */
  bool keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Load game */
  bool load(SDL_Renderer* renderer, bool full_load = true);

  /* Pauses der game in der Wald, ja ja ja */
  //void pause(); // TODO: implement

  /* Renders the title screen */
  bool render(SDL_Renderer* renderer);

  /* Save game based on the current slot number */
  bool save();

  /* Set the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the path of the game */
  bool setPath(std::string path, int level = 0, bool load = false);

  /* Sets the active renderer to be used */
  void setRenderer(SDL_Renderer* renderer);

  /* Sets the active save slot. This needs to be set prior to load */
  bool setSaveSlot(uint8_t slot);

  /* Sets the sound handler used. If unset, no sounds will play */
  void setSoundHandler(SoundHandler* new_handler);

  /* Unpause the game */
  //void unpause(); // TODO: implement

  /* Unload game */
  void unload(bool full_unload = true);
  void unloadCore();
  void unloadSub();

  /* Updates the game state */
  bool update(int cycle_time);
};

#endif // GAME_H
