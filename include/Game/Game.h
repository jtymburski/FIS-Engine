/*******************************************************************************
 * Class Name: Game
 * Date Created: Dec 2 2012
 * Inheritance: none
 * Description: This class is the top layer that handles all interclass
 *              communication between map, player, and battle as well as all
 *              setup and overall insight and control. The painting control
 *              won't be handled here and this will just act as an intermediate
 *              data highway / event handler.
 ******************************************************************************/
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

//#include "Game/Battle/Battle.h"
//#include "Game/EventHandler.h"
#include "Game/Map/Map.h"
//#include "Game/Player/Player.h"
//#include "Game/VictoryScreen.h"
//#include "MathHelper.h"
#include "Game/Player/Skill.h"
#include "Options.h"

class Game
{
public:
  /* Constructor function */
  Game(Options* running_config = NULL);

  /* Destructor function */
  ~Game();

  /* The game mode operator, for controlling the visible widget */
  enum GameMode {DISABLED       = 0, 
                 MAP            = 1, 
                 BATTLE         = 2, 
                 VICTORY_SCREEN = 3};

private:
  /* Handles all events throughout the game. */
  //EventHandler event_handler;
  
  /* A current battle pointer */
  //Battle* game_battle;
  
  /* The configuration for the display of the game */
  Options* game_config;

  /* The current loaded map */
  Map* game_map; // TODO: Make non-pointer?

  /* The maps in the game */
  //QVector<Map*> levels;
  //QList<QString> level_list;

  /* The level number currently on */
  //int level_num;

  /* The player */
  //Player main_player;

  /* The mode that the game is currently running at */
  GameMode mode;
  
  /* A current victory screen pointer */
  //VictoryScreen* victory_screen;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Connect the event handler to the game */
//  void connectEvents();
  
  /* Set up the battle */
  void setupBattle();

  /* Sets up the game - called on first creation */
//  void setupGame();

  /* Set up the map */
  void setupMap();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
//signals:
//  void closeGame();

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
//public slots:
  /* Gives the player an item, based on the id and number of that id */
//  bool giveItem(int id, int count);
  
  /* Initiates a conversation */
//  void initConversation(Conversation* convo, MapPerson* initiator, 
//                                             MapThing* source);
  
  /* Executes a pickup event on map */  
//  void pickupItem(MapItem* item, bool walkover);
  
  /* Starts a battle */
//  void startBattle();
  
  /* Teleports the player stored within the map */
//  void teleportThing(MapPerson* target, int x, int y, int section_id);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* The key up and down events to be handled by the class */
  bool keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Renders the title screen */
  bool render(SDL_Renderer* renderer);
  
  /* Set the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Updates the game state */
  bool update(int cycle_time, SDL_Renderer* renderer);
};

#endif // GAME_H
