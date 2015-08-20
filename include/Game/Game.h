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

#include "Game/Battle/AIModuleTester.h"
#include "Game/Battle/Battle.h"
#include "Game/Battle/BattleDisplay.h"
#include "Game/EventHandler.h"
#include "Game/Map/Map.h"
#include "Game/Player/Player.h"
#include "Game/Player/Bubby.h"
//#include "Game/VictoryScreen.h"
#include "Game/Player/Inventory.h" // TODO
#include "Options.h"

using std::begin;
using std::end;

class Game
{
public:
  /* Constructor function */
  Game(Options* running_config = nullptr);

  /* Destructor function */
  ~Game();

  /* The game mode operator, for controlling the visible widget */
  enum GameMode 
  {
    DISABLED       = 0, 
    MAP            = 1, 
    BATTLE         = 2, 
    VICTORY_SCREEN = 3 
  };

private:
  /* The active rendering engine */
  SDL_Renderer* active_renderer;
  
  /* The computed base path for resources in the application */
  std::string base_path;
    
  /* Battle control and visual */
  Battle* battle_ctrl;
  BattleDisplay* battle_vis;
  
  /* The configuration for the display of the game */
  Options* config;
  
  /* Handles all events throughout the game. */
  EventHandler event_handler;

  /* The game starting inventory */
  //Inventory* game_inventory; // TODO: Make part of bearacks party

  /* Game file path */
  std::string game_path;

  /* List of objects */
  std::vector<Action*> list_action;
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
  bool loaded;

  /* List of all actions */
  //std::vector<Action*> action_list;

  /* List of all Battle Class categories */
  //std::vector<Category*> battle_class_list;

  /* List of all Race categories */
  //std::vector<Category*> race_list;

  /* List of all flavours */
  //std::vector<Flavour*> flavour_list;

  /* List of all skills */
  //std::vector<Skill*> skill_list;

  /* List of all base persons */
  //std::vector<Person*> base_person_list;

  /* List of all available items in the game */
  //std::vector<Item*> base_item_list;

  /* The bubbified skill set */
  //SkillSet* bubbified_skills; // TODO: Make a const ID in skill set stack
  
  /* The maps in the game */
  // [09-07-14] TODO: Recommend Map*, std::string pair ?
  //std::vector<Map*> levels;
  //std::vector<std::string> level_list;

  /* Map variables */
  Map* map_ctrl; /* Main class */
  int map_lvl; /* Active level number */

  /* Test map path */
  std::string map_test_path; // TODO: REMOVE
  
  /* The mode that the game is currently running at */
  GameMode mode;
  
  /* The player */
  Player* player_main; 
  
  /* Number of ticks since inception */
  uint64_t ticks_total;

  /* A current victory screen pointer */
  //VictoryScreen* victory_screen;

  /* ------------ Constants --------------- */
  static const uint32_t kID_ITEM_MONEY; /* ID of money item */
  static const uint32_t kID_PARTY_BEARACKS; /* ID of player bearacks party */
  static const uint32_t kID_PARTY_SLEUTH; /* ID of player sleuth party */
  static const uint32_t kID_PERSON_PLAYER; /* ID of player primary person */
  static const uint32_t kID_SET_BUBBIFIED; /* ID of bubbified skillset */
  static const uint32_t kSTARTING_MAP; /* Starting map ID */
  static const std::string kSTARTING_PATH; /* Starting game path */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Add functions for game objects */
  Action* addAction(const std::string &raw);
  Category* addClass(const int32_t &id);
  Flavour* addFlavour(const int32_t &id);
  Item* addItem(const int32_t &id, SortObjects type = SortObjects::ITEMS);
  Party* addParty(const int32_t &id);
  Person* addPersonBase(const int32_t &id);
  Person* addPersonInst(const int32_t &base_id);
  Person* addPersonInst(Person* base_person);
  Category* addRace(const int32_t &id);
  Skill* addSkill(const int32_t &id);
  SkillSet* addSkillSet(const int32_t &id);

  /* Builds the ailment frames */
  void buildBattleDisplayFrames(SDL_Renderer* renderer);

  /* A give item event, based on an ID and count (triggered from stored event */
  bool eventGiveItem(int id, int count);
  
  /* Initiates a conversation event */
  void eventInitConversation(Conversation* convo, MapThing* source);

  /* Initiates a notification event (in map) */
  void eventInitNotification(std::string notification);
  
  /* The pickup item event - from walking over or triggering from action key */
  void eventPickupItem(MapItem* item, bool walkover);
  
  /* Starts a battle event. Using the given information - TODO */
  void eventStartBattle();
  
  /* Teleport thing event, based on ID and coordinates */
  void eventTeleportThing(int thing_id, int x, int y, int section_id);

  /* Load game */
  bool load(std::string base_file, SDL_Renderer* renderer, 
            std::string inst_file = "", bool encryption = false);

  /* Load game data */
  bool loadData(XmlData data, int index, SDL_Renderer* renderer);

  /* Polls events running from the event handler. Triggered from other places
   * in the game */
  void pollEvents();

  /* Remove functions for game objects */
  void removeActions();
  void removeAll(); /* Properly staged remove all call */
  void removeClasses();
  void removeFlavours();
  void removeItems();
  void removeParties();
  void removePersonBases();
  void removePersonInstances();
  void removeRaces();
  void removeSkills();
  void removeSkillSets();

  /* Set up the battle */
  void setupBattle();

  /* Unload the game */
  void unload();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Getter functions for game objects */
  Action* getAction(const int32_t &index, const bool &by_id = true);
  Category* getClass(const int32_t &index, const bool &by_id = true);
  Flavour* getFlavour(const int32_t &index, const bool &by_id = true);
  Item* getItem(const int32_t &index, const bool &by_id = true); 
  Party* getParty(const int32_t &index, const bool &by_id = true);
  Person* getPersonBase(const int32_t &index, const bool &by_id = true);
  Person* getPersonInst(const int32_t &index, const bool &by_id = true);
  Category* getRace(const int32_t &index, const bool &by_id = true);  
  Skill* getSkill(const int32_t &index, const bool &by_id = true);
  SkillSet* getSkillSet(const int32_t &index, const bool &by_id = true);

  /* The key up and down events to be handled by the class */
  bool keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);
  
  /* Pauses der game in der Wald, ja ja ja */
  //void pause(); // TODO: implement
  
  /* Renders the title screen */
  bool render(SDL_Renderer* renderer);
  
  /* Set the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the test map to run instead of current default */
  void setTestMap(std::string test_map);

  /* Unpause the game */
  //void unpause(); // TODO: implement

  /* Updates the game state */
  bool update(int cycle_time);
};

#endif // GAME_H
