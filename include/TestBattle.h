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
#include "Game/EventHandler.h"
#include "Game/Map/Map.h"
#include "Game/Player/Player.h"
#include "Game/Player/Bubby.h"
//#include "Game/VictoryScreen.h"
#include "Game/Player/Inventory.h" //TODO
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
  /* The active rendering engine */
  //SDL_Renderer* active_renderer;
  
  /* The computed base path for resources in the application */
  //std::string base_path;
  
  /* Handles all events throughout the game. */
  //EventHandler event_handler;
  
  /* A current battle pointer */
  //Battle* game_battle;
  //BattleDisplay* battle_display;

  /* The configuration for the display of the game */
  //Options* game_config;

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

  /* ------------ Constants --------------- */
  //static const uint32_t kMONEY_ITEM_ID;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Builds the ailment frames */
  void buildBattleDisplayFrames(SDL_Renderer* renderer);

  /* Set up the battle */
  void setupBattle();

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
