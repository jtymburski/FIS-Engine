/*******************************************************************************
 * Class Name: TestBattle
 * Date Created: July 31, 2015
 * Inheritance: none
 * Description: This class is the testing interface for battle scenarios.
 ******************************************************************************/
#ifndef TESTBATTLE_H
#define TESTBATTLE_H

#include <SDL2/SDL.h>
#include <chrono>
#include <memory>

#include "Game/Battle/AIModuleTester.h"
#include "Game/Battle/Battle.h"
#include "Game/Battle/BattleDisplay.h"
//#include "Game/EventHandler.h"
//#include "Game/Map/Map.h"
#include "Game/Player/Player.h"
//#include "Game/Player/Bubby.h"
//#include "Game/VictoryScreen.h"
//#include "Game/Player/Inventory.h" //TODO
#include "Options.h"

class TestBattle
{
public:
  /* Constructor function */
  TestBattle(Options* running_config = nullptr);

  /* Destructor function */
  ~TestBattle();

  /* The game mode operator, for controlling the visible widget */
  //enum GameMode
  //{
  //  DISABLED       = 0,
  //  MAP            = 1,
  //  BATTLE         = 2,
  //  VICTORY_SCREEN = 3
  //};

private:
  /* Action sets */
  std::vector<Action*> act_alt; /* Alter */
  std::vector<Action*> act_asn; /* Assign */
  std::vector<Action*> act_dmg; /* Damage */
  std::vector<Action*> act_inf; /* Inflict */
  std::vector<Action*> act_rlv; /* Relieve */

  /* The active rendering engine */
  //SDL_Renderer* active_renderer;

  /* The computed base path for resources in the application */
  std::string base_path;
  
  /* A current battle pointer */
  BattleDisplay* battle_display;
  Battle* battle_logic;
  
  /* Classes */
  Category* class_arcadius1;
  Category* class_aurora_agent;
  Category* class_aurora_heavy;
  Category* class_aurora_drone;
  Category* class_aurora_engg;
  Category* class_player;
  Category* class_reverdile;
  Category* class_reverdling;

  /* Handles all events throughout the game. */
  //EventHandler event_handler;

  /* First run trigger */
  bool first_run;

  /* The configuration for the display of the game */
  Options* game_config;

  /* The game starting inventory */
  //Inventory* game_inventory; //TODO: Make part of player?

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
  //SkillSet* bubbified_skills;

  /* The mode that the game is currently running at */
  //GameMode mode;

  /* A current victory screen pointer */
  //VictoryScreen* victory_screen;

  /* Number of ticks since inception */
  //uint64_t num_ticks;

  /* Races */
  Category* race_aurora;
  Category* race_bear;
  Category* race_human;
  Category* race_treefolk;

  /* List of all skills and skillsets - just for management */
  std::vector<Skill*> skills;
  std::vector<SkillSet*> skillsets;

  /* Attribute Sets */
  AttributeSet stats_weak;
  AttributeSet stats_not_as_weak;
  AttributeSet stats_normal;
  AttributeSet stats_medium;
  AttributeSet stats_top;
  AttributeSet stats_boss;
  AttributeSet stats_boss2;

  /* ------------ Constants --------------- */
  //static const uint32_t kMONEY_ITEM_ID;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Builds the ailment frames */
  void buildBattleDisplay(SDL_Renderer* renderer);

  /* Main create call - first construction */
  void create();

  /* Create sub calls */
  void createActions();
  void createClasses();
  void createRaces();
  void createSkills();
  void createSkillSets();

  /* Delete sub calls */
  void deleteActions();
  void deleteClasses();
  void deleteRaces();
  void deleteSkills();
  void deleteSkillSets();

  /* Main destruction call - ending */
  void destroy();

  /* Get calls */
  Skill* getSkill(int id);
  SkillSet* getSkillSet(int id);

  /* Start a battle with the selected parameters */
  void initBattle();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns a pointer to an action by index or by ID */
  //Action* getAction(const int32_t &index, const bool &by_id = true);

  /* Returns a pointer to a battle class by index or by ID */
  //Category* getBattleClass(const int32_t &index, const bool &by_id = true);

  /* Returns a pointer to a race category by index or by ID */
  //Category* getCategory(const int32_t &index, const bool &by_id = true);

  /* Returns a pointer to a flavour by index or by ID */
  //Flavour* getFlavour(const int32_t &index, const bool &by_id = true);

  /* Returns a pointer to a skill by index or by ID */
  //Skill* getSkill(const int32_t &index, const bool &by_id = true);

  /* The key up and down events to be handled by the class */
  bool keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Renders the title screen */
  bool render(SDL_Renderer* renderer);

  /* Set the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Updates the game state */
  bool update(int cycle_time);
};

#endif // TESTBATTLE_H
