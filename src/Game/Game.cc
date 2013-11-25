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
 * TODO:
 *  1. Add Event Handler. Integrate in to handle all exchanges between class
 *  2. Talk to Mike about Battle and how we should keep it constructed and then
 *     just change the player and foes when it switches. This is primarily
 *     for dealing with the stacked widget.
 *  3. Add victory screen logic
 *  4. Add speed up button to allow the game to accelerate movement and
 *     everything else. Do it by multiplying the time elapsed.
 ******************************************************************************/

#include "Game/Game.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Game::Game(Options* running_config)
{
  game_config = NULL;
  mode = MAP;
  
  setConfiguration(running_config);

//  game_config = running_config;
//  setParent(parent);
//  setupGame();
  setupBattle();
}

/* Destructor function */
Game::~Game()
{
  game_config = NULL;
  mode = DISABLED;

//  /* Clear out the widgets in the stack */
//  while(count() > 0)
//    removeWidget(widget(0));
//
//  /* Delete battle */
//  if(game_battle != 0)
//  {
//    delete game_battle;
//    game_battle = 0;
//  }

  /* Delete map */
//  if(game_map != 0)
//  {
//    delete game_map;
//    game_map = 0;
//  }
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/* Connect the event handler to the game */
/*void Game::connectEvents()
{
  QObject::connect(&event_handler, SIGNAL(giveItem(int, int)),
                   this,           SLOT(giveItem(int, int)));

  QObject::connect(
              &event_handler, 
              SIGNAL(initConversation(Conversation*, MapPerson*, MapThing*)), 
              this, 
              SLOT(initConversation(Conversation*, MapPerson*, MapThing*)));
  
  QObject::connect(&event_handler, SIGNAL(pickupItem(MapItem*, bool)),
                   this,           SLOT(pickupItem(MapItem*, bool)));

  QObject::connect(&event_handler, SIGNAL(startBattle()), 
                   this,           SLOT(startBattle()));

  QObject::connect(&event_handler, 
                   SIGNAL(teleportThing(MapPerson*, int, int, int)), 
                   this,           
                   SLOT(teleportThing(MapPerson*, int, int, int)));
}*/

/* Set up the battle - old battle needs to be deleted prior to calling */
void Game::setupBattle()
{
  /*
  std::vector<Action> actions;
  actions.push_back(Action("1,ALTER,THAG,,,,AMOUNT.50,AMOUNT.15"));
  actions.push_back(Action("2,ALTER,VITA,,PHYSICAL,PHYSICAL.THERMAL,PC.50,PC.10"));
  actions.push_back(Action("3,INFLICT,POISON,2.7,,,,,"));
  actions.push_back(Action("4,RELIEVE,CURSE,,,,,"));
  actions.push_back(Action("5,REVIVE,,,,,PC.25,AMOUNT.50"));

  for (Action a : actions)
    a.print();
  */
}

/* Set up the map - old map needs to be deleted prior to calling */
void Game::setupMap()
{
//  /* Sets up the map format and map with vsync and double buffering forced on */
//  /* TODO: add checking if OpenGL is not enabled */
//  QGLFormat gl_format(QGL::SampleBuffers);
//  gl_format.setDoubleBuffer(true);
//  if(game_config.isVsyncEnabled())
//    gl_format.setSwapInterval(1);
//  else
//    gl_format.setSwapInterval(0);
//  game_map = new Map(gl_format, game_config, &event_handler);
//
//  /* Load the map - temporary location */
//  game_map->loadMap("maps/test_04");
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Returns the number of items actually inserted into inventory. If less than
 * count, it only could fit that many */
/*bool Game::giveItem(int id, int count)
{
  // TODO: connect to player and inventory
  return true;
}

void Game::initConversation(Conversation* convo, MapPerson* initiator, 
                                                 MapThing* source)
{
  if(game_map != 0)
    game_map->initConversation(convo, initiator, source);
}

void Game::pickupItem(MapItem* item, bool walkover)
{
  if(item != 0 && item->isWalkover() == walkover)
  {
    bool was_inserted = giveItem(item->getID(), item->getCount());*/
    
    /* Only proceed if it was inserted */
    /*if(game_map != 0)
    {
      if(was_inserted)
      {
        game_map->pickupItem(item);
      }
      else
      {
        QString notification = "Inventory too full to pick up ";
        if(item->getCount() > 1)
          notification += QString::number(item->getCount()) + " " + 
                          item->getName();
        else
          notification += "the " + item->getName();
        notification += ".";
        
        game_map->initNotification(notification);
      }
    }
  }
}

void Game::startBattle()
{
  switchGameMode(BATTLE);
}

void Game::teleportThing(MapPerson* target, int x, int y, int section_id)
{
  if(game_map != 0 && target != 0)
  {
    if(section_id < 0)
      game_map->teleportThing(target->getID(), x, y);
    else
      game_map->teleportThing(target->getID(), x, y, section_id);
  }
}*/

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* The key down events to be handled by the class */
bool Game::keyDownEvent(SDL_Keysym symbol)
{
  /* Exit the game, game has finished processing */
  if(symbol.sym == SDLK_ESCAPE)
  {
    return true;
  }
  /* Switch the view to the map */
  else if(symbol.sym == SDLK_F1)
  {
    // TODO: Change view to map
  }
  /* Switch the view to the battle */
  else if(symbol.sym == SDLK_F2)
  {
    // TODO: Change view to battle
  }
  /* Otherwise, send keys to the active view */
  else
  {
    // TODO: Send keys to active view
  }
  
  return false;
}

/* The key up events to be handled by the class */
void Game::keyUpEvent(SDL_Keysym symbol)
{
  // TODO: Send up keys to relevant views
}

/* Renders the title screen */
bool Game::render(SDL_Renderer* renderer)
{
  // TODO: Send the render call to the relevant view
  
  return true;
}
  
/* Set the running configuration, from the options class */
bool Game::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    game_config = running_config;
    return true;
  }
  
  return false;
}

/* Updates the game state. Returns true if the class is finished */
bool Game::update(int cycle_time)
{
  // TODO: Send the update sequence to the relevant view
  
  return false;
}
