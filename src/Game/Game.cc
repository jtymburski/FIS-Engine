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
  /* Initalize class variables */
  base_path = "";
  game_config = NULL;
  game_map = NULL;
  mode = DISABLED;
  
  /* Set game configuration */
  setConfiguration(running_config);

  /* Set up the render classes */
//  setupGame();
  setupBattle();
  setupMap();
}

/* Destructor function */
Game::~Game()
{
  game_config = NULL;
  mode = DISABLED;

//  /* Delete battle */
//  if(game_battle != 0)
//  {
//    delete game_battle;
//    game_battle = 0;
//  }

  /* Delete map */
 if(game_map != NULL)
 {
   delete game_map;
   game_map = NULL;
 }
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Set up the battle - old battle needs to be deleted prior to calling */
void Game::setupBattle()
{
  bool enable_test = false;

  if (enable_test)
  {
  //Begin time test
  using namespace std::chrono;

  system_clock::time_point tp = system_clock::now();
  system_clock::duration dtn = tp.time_since_epoch();
  std::cout << "Starting clock: " << dtn.count() << std::endl;

  std::vector<std::string> the_strings(1000000, " abcd ");

  for (auto &s : the_strings)
    s = Helpers::trim(s);

  // Helpers Testing
  auto length =  10;

  std::vector<int> values(length, 0);

  for (auto i = 0; i < length * 100; i++)
  {
    //auto value = Helpers::randU32();
    //std::cout << value << std::endl;
    //values[value]++;
  }
  for (auto i = 0; i < length * 100; i++)
  {
    //auto value = Helpers::randU64();
    //std::cout << value << std::endl;
    //values[value]++;
  }

  //system_clock::time_point tp2 = system_clock::now();
  //system_clock::duration dtn2 = tp2.time_since_epoch();
  //std::cout << "Ending clock: " << dtn2.count() << std::endl;
  //std::cout << "Periods elapsed: " << (dtn2.count() - dtn.count()) / 1000 << std::endl;
  

  //auto index = 0;
  //for (auto value : values)
  //  std::cout << index++ << " " << value << "\n";

  // Action Testing
  std::vector<Action*> actions;
  std::vector<float> chances = {0.50, 0.40, 0.12, 0.46};
  actions.push_back(new Action("1,ALTER,THAG,,,,AMOUNT.50,AMOUNT.15"));
  actions.push_back(new Action("3,INFLICT,POISON,2.7,,,,,"));
  actions.push_back(new Action("4,RELIEVE,CURSE,,,,,"));
  actions.push_back(new Action("5,REVIVE,,,,,PC.25,AMOUNT.50"));
  Action* special = new Action("6,ALTER,VITA,,,,PC.25,AMOUNT.50");
  
  // Skill Testing
  std::vector<Skill*> skills;

  Skill* normal_attack = new Skill(13, "Attack",ActionScope::ONE_TARGET,actions[0],0.75);
  normal_attack->addActions(actions, chances);

  skills.push_back(normal_attack);
  skills.push_back(new Skill(400, "Super Attack",ActionScope::ONE_ENEMY,special,0.65));
  skills.push_back(new Skill(3, "Poison Attack",ActionScope::ONE_ENEMY,actions[1],0.79));
  skills.push_back(new Skill(35, "Crappy Attack",ActionScope::NO_SCOPE,special,1.00));

  std::vector<uint32_t> levels;

  //for (Skill* s : skills)
  //{
    //s->print();
    levels.push_back(levels.size());
  //}

  // Skill Set Testing
  SkillSet* set1 = new SkillSet(skills, levels);
  set1->sort(SkillSorts::ENABLED);

  std::vector<Bubby*> bubby_pointas;

  // General Item Testing
  Item forty_six{46, "Item Forty Six", 75, nullptr, 1.08};

  forty_six.setValue(78);

  forty_six.print();

  // Flavour Testing
  Flavour* spark = new Flavour(101, "Spark", AttributeSet(1), 1.04, 35);
  Flavour* tumor = new Flavour(102, "Tumor", AttributeSet(2), 1.09, 65);
  Flavour* moldy = new Flavour(103, "Moldy", AttributeSet(1), 1.11, 48);

  spark->print();

  // Bubby Testing
  for (int i = 0; i < 10; i++)
  {
    bubby_pointas.push_back(new Bubby(spark));
    bubby_pointas.push_back(new Bubby(tumor));
    bubby_pointas.push_back(new Bubby(moldy));
  }

  //bubby_pointas.at(0)->print(true, true);

  // Signature Testing


  // Equipment Testing
  
  } // end enable test
  
  else
  {
    std::cout << "hey kevin if u see this u r kewl <3\n";
  }
}

/* Set up the map - old map needs to be deleted prior to calling */
void Game::setupMap()
{
  /* Create the map */
  game_map = new Map(game_config, &event_handler);
  mode = MAP;
  
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
bool Game::keyDownEvent(SDL_KeyboardEvent event)
{
  /* Exit the game, game has finished processing */
  if(event.keysym.sym == SDLK_ESCAPE)
  {
    if(game_map != NULL)
      game_map->unfocus();
    return true;
  }
  /* Switch the view to the map */
  else if(event.keysym.sym == SDLK_F1)
  {
    mode = MAP;
  }
  /* Switch the view to the battle */
  else if(event.keysym.sym == SDLK_F2)
  {
    mode = BATTLE;
  }
  /* Otherwise, send keys to the active view */
  else
  {
    if(mode == MAP)
      return game_map->keyDownEvent(event);
  }
  
  return false;
}

/* The key up events to be handled by the class */
void Game::keyUpEvent(SDL_KeyboardEvent event)
{
  if(mode == MAP)
    game_map->keyUpEvent(event);
}

/* Renders the title screen */
bool Game::render(SDL_Renderer* renderer)
{
  if(!game_map->isLoaded())
   game_map->loadMap(base_path + "maps/test_05", renderer);

  if(mode == MAP)
    return game_map->render(renderer);
  
  return true;
}
  
/* Set the running configuration, from the options class */
bool Game::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    game_config = running_config;
    base_path = game_config->getBasePath();
    
    /* Set in secondary classes */
    if(game_map != NULL)
      game_map->setConfiguration(running_config);
    
    return true;
  }
  
  return false;
}

/* Updates the game state. Returns true if the class is finished */
bool Game::update(int cycle_time)
{
  /* Event poll testing */
  if(event_handler.pollEventType() != EventClassifier::NOEVENT)
  {
    std::cout << "Event: " << static_cast<int>(event_handler.pollEventType()) 
              << std::endl;
    event_handler.pollClear();
  }

  if(mode == MAP)
    return game_map->update(cycle_time);

  return false;
}
