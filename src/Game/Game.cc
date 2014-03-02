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

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const uint32_t Game::kMONEY_ITEM_ID{0};

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Game::Game(Options* running_config)
{
  /* Initalize class variables */
  base_path = "";
  game_inventory = nullptr;
  game_config = nullptr;
  game_map = nullptr;
  mode = DISABLED;
  
  /* Set game configuration */
  setConfiguration(running_config);

  /* Set up the render classes */
  setupPlayerInventory();
  setupBattle();
  setupMap();
}

/* Destructor function */
Game::~Game()
{
  game_config = nullptr;
  mode = DISABLED;

//  /* Delete battle */
//  if(game_battle != nullptr)
//  {
//    delete game_battle;
//    game_battle = nullptr;
//  }

  /* Delete map */
  if(game_map != nullptr)
  {
    delete game_map;
    game_map = nullptr;
  }

  if(game_inventory != nullptr)
  {
   delete game_inventory;
   game_inventory = nullptr;
  }
 
  /* Delete all game items */
  for(auto i = begin(item_list); i != end(item_list); i++)
    delete (*i);

  item_list.clear();  // does nothing?
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* A give item event, based on an ID and count (triggered from stored event */
bool Game::eventGiveItem(int id, int count)
{
  /* Attempt to find the item */
  Item* found_item = nullptr;
  for(auto i = item_list.begin(); i != item_list.end(); i++)
    if((*i)->getGameID() == id)
      found_item = (*i);

  /* If the item was inserted, display pickup notification */
  if(found_item != nullptr)
  {
    // TODO: Attempt insert. For now, according to variable
    bool inserted = true;
    
    /* If inserted, notify that the pickup was a success */
    if(inserted)
    {
      if(game_map != nullptr)
        game_map->initNotification(found_item->getThumb(), count);
      return true;
    }
    /* Otherwise, notify that item could not be received */
    else
    {
      if(game_map != nullptr)
        game_map->initNotification("Insufficient room in inventory to fit " + 
                                   std::to_string(count) + " " + 
                                   found_item->getName());
    }
  }
  
  return false;
}
  
 /* Initiates a conversation event */
 void Game::eventInitConversation(Conversation* convo, MapThing* source)
{
  if(game_map != nullptr)
    game_map->initConversation(convo, source);
}

/* Initiates a notification event (in map) */
void Game::eventInitNotification(std::string notification)
{
  if(game_map != nullptr)
    game_map->initNotification(notification);
}

/* The pickup item event - from walking over or triggering from action key */
void Game::eventPickupItem(MapItem* item, bool walkover)
{
  if(item != nullptr && item->isWalkover() == walkover)
  {
    bool was_inserted = eventGiveItem(item->getCoreID(), item->getCount());
    
    /* If the insert was successful, pickup the item */
    if(game_map != nullptr && was_inserted)
     game_map->pickupItem(item);
  }
}

/* Starts a battle event. Using the given information - TODO */
void Game::eventStartBattle()
{
  mode = BATTLE;
}

/* Teleport thing event, based on ID and coordinates */
void Game::eventTeleportThing(int thing_id, int x, int y, int section_id)
{
  if(game_map != nullptr)
    game_map->teleportThing(thing_id, x, y, section_id);
}
  
void Game::pollEvents()
{
  do
  {
    EventClassifier classification = event_handler.pollEventType();
    
    /* Poll classification */
    if(classification == EventClassifier::GIVEITEM)
    {
      int id;
      int count;
      event_handler.pollGiveItem(&id, &count);
      eventGiveItem(id, count);
    }
    else if(classification == EventClassifier::NOTIFICATION)
    {
      std::string notification;
      event_handler.pollNotification(&notification);
      eventInitNotification(notification);
    }
    else if(classification == EventClassifier::PICKUPITEM)
    {
      MapItem* item;
      bool walkover;
      event_handler.pollPickupItem(&item, &walkover);
      eventPickupItem(item, walkover);
    }
    else if(classification == EventClassifier::RUNBATTLE)
    {
      eventStartBattle();
    }
    else if(classification == EventClassifier::RUNMAP)
    {
    
    }
    else if(classification == EventClassifier::STARTCONVO)
    {
      Conversation* convo;
      MapThing* source;
      event_handler.pollConversation(&convo, &source);
      eventInitConversation(convo, source);
    }
    else if(classification == EventClassifier::TELEPORTTHING)
    {
      int thing_id, x, y, section_id;
      event_handler.pollTeleportThing(&thing_id, &x, &y, &section_id);
      eventTeleportThing(thing_id, x, y, section_id);
    }

  } while(event_handler.pollEvent());
  
  event_handler.pollClear();
}

/* Set up the battle - old battle needs to be deleted prior to calling */
void Game::setupBattle()
{
  bool enable_test = true;

  if (enable_test)
  {
  //Begin time test
  //using namespace std::chrono;

  //system_clock::time_point tp = system_clock::now();
  //system_clock::duration dtn = tp.time_since_epoch();
  //std::cout << "Starting clock: " << dtn.count() << std::endl;

  //std::vector<std::string> the_strings(1000000, " abcd ");

  //for (auto &s : the_strings)
  // s = Helpers::trim(s);

  // Helpers Testing
  //auto length =  10;

  //std::vector<int> values(length, 0);

  //for (auto i = 0; i < length * 100; i++)
  //{
    //auto value = Helpers::randU32();
    //std::cout << value << std::endl;
    //values[value]++;
  //}
  //for (auto i = 0; i < length * 100000; i++)
  //{
    //auto value = Helpers::randU64();
    //std::cout << value << std::endl;
    //values[value]++;
  //}
  //std::cout << "DONE!" << std::endl;

  //system_clock::time_point tp2 = system_clock::now();
  //system_clock::duration dtn2 = tp2.time_since_epoch();
  //std::cout << "Ending clock: " << dtn2.count() << std::endl;
  //std::cout << "Periods elapsed: " << (dtn2.count() - dtn.count()) / 1000 << std::endl;
  
  // AttributeSet testing
  AttributeSet min_scion_set(0, true);
  AttributeSet max_scion_set(3, true);
  AttributeSet min_hex_set(2, true);
  AttributeSet max_hex_set(3, true);
  AttributeSet spark_set(1);
  AttributeSet tumor_set(1);
  AttributeSet moldy_set(1);

  AttributeSet min_human_set = min_hex_set;
  min_human_set.setStat(Attribute::VITA, 200);
  AttributeSet max_human_set(3, true);

  //Action Testing
  std::vector<Action*> actions;
  std::vector<float> chances = {0.50, 0.40, 0.12, 0.46};
  actions.push_back(new Action("1,ALTER,THAG,,,,AMOUNT.50,AMOUNT.15"));
  actions.push_back(new Action("3,INFLICT,POISON,2.7,,,,,"));
  actions.push_back(new Action("4,RELIEVE,CURSE,,,,,"));
  actions.push_back(new Action("5,REVIVE,,,,,PC.25,AMOUNT.50"));
  actions.push_back(new Action("7,INFLICT,BURN,3.5,,,,,"));
  actions.push_back(new Action("8,ALTER,VITA,,,,AMOUNT.100,PC.10"));
  actions.push_back(new Action("9,ALTER,VITA,,,,AMOUNT.75,PC.5"));

  Action* special = new Action("10,ALTER,VITA,,,,PC.25,AMOUNT.50");
  
  // Skill Testing
  std::vector<Skill*> skills;
  std::vector<Skill*> other_skills;

  Skill* normal_attack = new Skill(13, "Attack",ActionScope::ONE_TARGET,actions[0],0.75);
  Skill* medium_attack = new Skill(14, "Med. Attack",ActionScope::ONE_TARGET,actions[5],0.85);
  Skill* hard_attack   = new Skill(15, "Hard Attack",ActionScope::ONE_TARGET,actions[6],0.90);
  normal_attack->addActions(actions, chances);

  skills.push_back(normal_attack);
  skills.push_back(new Skill(400, "Super Attack",ActionScope::ONE_ENEMY,special,0.65));
  skills.push_back(new Skill(3, "Poison Attack",ActionScope::ONE_ENEMY,actions[1],0.79));
  skills.push_back(new Skill(35, "Crappy Attack",ActionScope::ONE_TARGET,special,1.00));

  other_skills.push_back(medium_attack);
  other_skills.push_back(hard_attack);

  std::vector<uint32_t> levels;
  std::vector<uint32_t> other_levels;

  for (auto it = begin(skills); it != end(skills); ++it)
    levels.push_back(levels.size());

  for (auto it = begin(skills); it != end(skills); ++it)
    other_levels.push_back(1);

  // SkillSet Testing
  SkillSet* scion_skills = new SkillSet(skills, levels);
  SkillSet* hex_skills   = scion_skills;
  SkillSet* other_skill_set = new SkillSet(other_skills, other_levels);

  //scion_skills->print();
  //hex_skills->print();
  //other_skill_set->print();

  // General Item Testing
  // Item* potion      = new Item(45, "Potion", 70, nullptr, 1.01);
  // Item* unique_item = new Item(46, "Unique Item", 75, nullptr, 1.08);
  // Item* new_potion  = new Item(potion);

  // Key Item Testing
  // Item* master_key  = new Item(115, "Master Key", nullptr);
  // Item* master_key2 = new Item(master_key);
  // Item* super_box = new Item(116, "Super Box", nullptr);
  // Item* boxxy_box = new Item(117, "Boxxy Box", nullptr);
  // Item* epic_sword_comp = new Item(118, "Epic Sword Comp", nullptr);
  // Item* alpha_omega = new Item(119, "Alpha Omega", nullptr);

  // Flavour Testing
  // Flavour* spark = new Flavour(101, "Spark", spark_set, 1.04, 35);
  // Flavour* tumor = new Flavour(102, "Tumor", tumor_set, 1.09, 65);
  // Flavour* moldy = new Flavour(103, "Moldy", moldy_set, 1.11, 48);

  // Bubby* first  = new Bubby(spark);
  // Bubby* second = new Bubby(spark, 1);
  // Bubby* third  = new Bubby(tumor,1);
  // Bubby* fourth = new Bubby(moldy);
  // Bubby* fifth = new Bubby(tumor,2);
  // Bubby* sixth = new Bubby(moldy,2);

  // Signature Testing

  // Equipment Testing
  //Equipment* fated = new Equipment(201, "Fated Oak Saber", 1, 1, 100, nullptr, 10, 10);
  //Equipment* suit  = new Equipment(202, "Suit", 100, 3, 100, nullptr, 10, 10);

  // Inventory Testing
  // Inventory* test_pouch = new Inventory(1006, "Test Pouchey");
  // Inventory* test_shoppy = new Inventory(1007, "Test Shoppy");
  // test_pouch->setFlag(InvState::SHOP_STORAGE, false);
  // test_shoppy->setFlag(InvState::SHOP_STORAGE, true);

  //test_pouch->add(new_potion, 10);
  // test_pouch->add(first, 6);
  // test_pouch->add(first, 3);
  // test_pouch->add(second, 3);
  // test_pouch->add(third, 2);
  // test_pouch->add(fourth, 3);
  // test_pouch->removeBubbyID(first->getGameID(), 2);
  // test_pouch->add(fifth, 3);
  // test_pouch->add(sixth, 4);
  // test_pouch->add(fated, 2);
  // test_pouch->add(suit, 1);

  // test_shoppy->add(new_potion, 110);
  // test_shoppy->add(first, 1012);
  // test_shoppy->add(second, 1140);
  // test_shoppy->add(first, 1410);
  // test_shoppy->add(first, 10116);
  // test_shoppy->add(fated, 6401);
  // test_shoppy->add(suit, 18);

  // test_shoppy->removeEquipID(suit->getGameID(), 18);
  // test_shoppy->removeEquipID(fated->getGameID(), 6401);

  //test = test_pouch->addItem(new_potion, 4);
  // if (test == AddStatus::GOOD_DELETE)
  // {
  //   std::cout << "deleting new_potion\n";
  //   delete new_potion2;
  //   new_potion2 = nullptr;
  // }
  // else if (test == AddStatus::GOOD_KEEP)
  // {
  //   std::cout << "Successful addition to keep\n";
  // }
  // else
  //   std::cout << "Addition failed\n";

  // test = test_pouch->add(first, 7);
  // if (test == AddStatus::GOOD_DELETE)
  // {
  //   std::cout << "deleting first bubby\n";
  //   delete first;
  //   first = nullptr;
  // }
  // else if (test == AddStatus::GOOD_KEEP)
  // {
  //   std::cout << "Successful addition to keep\n";
  // }
  // else
  //   std::cout << "Addition failed\n";

  // test = test_pouch->add(second, 3);
  // if (test == AddStatus::GOOD_DELETE)
  // {
  //   std::cout << "deleting second bubby\n";
  //   delete second;
  //   second = nullptr;
  // }
  // else if (test == AddStatus::GOOD_KEEP)
  // {
  //   std::cout << "Successful addition to keep\n";
  // }
  // else
  //   std::cout << "Addition failed\n";

  //test_pouch->removeBubbyID(fifth->getGameID());
  //test_pouch->removeBubbyID(fifth->getGameID());
  //test_pouch->removeBubbyID(fifth->getGameID());

  //test_pouch->removeBubbyID(fifth->getGameID());
  //test_pouch->removeBubbyID(fifth->getGameID());
  //test_pouch->removeBubbyID(fifth->getGameID());

  //test_pouch->removeBubbyID(fifth->getGameID());
  //test_pouch->addBubby(fifth);

  //test_pouch->removeBubbyID(fifth->getGameID());
  //test_pouch->removeBubbyIndex(0);
  //test_pouch->removeBubbyIndex(0);
  //test_pouch->removeBubbyIndex(0);
  //test_pouch->removeBubbyIndex(0);
  //test_pouch->removeBubbyIndex(0);

  //test_pouch->addBubby(fifth);
  //test_pouch->addBubby(fifth);
  //test_pouch->addBubby(fifth, 1);

  //test_pouch->addEquipment(fated);
  //test_pouch->addEquipment(suit);
  //test_pouch->removeEquipID(fated->getGameID());
  //test_pouch->removeEquipID(suit->getGameID());

  // test_pouch->print(false);
  // test_shoppy->print(false);

  // test_pouch->sort(SortType::NAME, SortObjects::ZERO_BUBBIES, false);
  // test_pouch->print(false);

  // auto key_items = test_pouch->getKeyItems();

  // for (auto it = begin(key_items); it != end(key_items); ++it)
  // {
  //  std::cout << "Item: " << (*it).first->getName() << " " 
  //           << static_cast<int>((*it).second) << "\n";
  // }

  // Category Testing
  Category* blood_scion = new Category("Blood Scion", "Scion", min_scion_set, 
                                       max_scion_set, scion_skills);
  blood_scion->setDescription("User of blood magicks.");
  blood_scion->setFlag(CategoryState::E_STAFF, true);

  Category* bear = new Category("Bear", "Bears", min_hex_set, 
                                    max_hex_set, hex_skills);
  Category* human = new Category("Human", "Humans", min_human_set, max_human_set, other_skill_set);
  bear->setDescription("Has a right to bear arms.");

  // Person Testing
  Person* berran   = new Person(455, "Berran", blood_scion, bear);
  Person* arcadius = new Person(456, "Arcadius", blood_scion, human);
  Person* malgidus = new Person(457, "Malgidus", blood_scion, bear);

  Person* cloud_dude = new Person(555, "Cloud Dude", blood_scion, bear);
  Person* ball_man = new Person(556, "Ball Man", blood_scion, bear);
  Person* thruster_barrow = new Person(557, "Thruster Barrow", blood_scion, human); 

  Party* allies = new Party(berran, PartyType::SLEUTH, 5, nullptr);
  Party* foes   = new Party(cloud_dude, PartyType::REGULAR_FOE, 5, nullptr);

  allies->addMember(arcadius);
  allies->addMember(malgidus);

  foes->addMember(ball_man);
  foes->addMember(thruster_barrow);

  for (uint32_t i = 0; i < allies->getSize(); i++)
    allies->getMember(i)->battlePrep();
  for (uint32_t i = 0; i < foes->getSize(); i++)
    foes->getMember(i)->battlePrep();

  allies->print();
  foes->print();
  //berran->print(false, true, true, true);
  
  // berran->setCurves(Element::FIRE, ElementCurve::S, 
  //                   Element::PHYSICAL, ElementCurve::B);
  // berran->setLoot(25, 150, {14, 12, 16});
  // berran->setExpMod(1.5);
  // berran->addExp(1000, true);
  // berran->battlePrep();
  // berran->doDmg(145);

  // berran->print(false, true, true, false);

  // delete test_pouch;
  // delete spark;
  // delete moldy;
  // delete tumor;

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

/* Sets up the default player inventory */
void Game::setupPlayerInventory()
{
  /* Create the inventory */
  game_inventory = new Inventory(7, "Default Inventory", nullptr);

  /* General inventory setup */
  Inventory::setMoneyID(kMONEY_ITEM_ID);

  /* Add starting items to the Inventory */
  //TODO: Starting items [01-11-14]
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* The key down events to be handled by the class */
bool Game::keyDownEvent(SDL_KeyboardEvent event)
{
  /* Exit the game, game has finished processing */
  if(mode == BATTLE && event.keysym.sym == SDLK_ESCAPE)
  {
    return true;
  }
  /* TESTING section - probably remove at end */
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
  /* Show item store dialog in map */
  else if(event.keysym.sym == SDLK_5)
  {
    if(game_map != NULL)
    {
      std::vector<Item*> items;
      items.push_back(item_list[0]);
      items.push_back(item_list[0]);
      std::vector<uint32_t> counts;
      counts.push_back(2);
      counts.push_back(3);
      std::vector<int32_t> cost_modifiers;
      cost_modifiers.push_back(0);
      cost_modifiers.push_back(10);
      
      game_map->initStore(ItemStore::BUY, items, counts, 
                          cost_modifiers, "Sam's Store", false);
    }
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
  /* Create temporary list of items - TODO: Pull into file */
  if(item_list.empty())
  {
    Item* item1 = new Item(5, "Sword of Power", 125, 
                          new Frame("sprites/sword_AA_A00.png", renderer));
    Item* item2 = new Item(7, "Frost Bubby", 5, 
                           new Frame("sprites/Battle/Bubbies/frosty_t1.png", 
                                     renderer));
    Item* item3 = new Item(0, "Coins", 1, 
                           new Frame("sprites/coins_AA_A00.png", renderer));
    
    item_list.push_back(item1);
    item_list.push_back(item2);
    item_list.push_back(item3);
  }
  
  if(!game_map->isLoaded())
   game_map->loadMap(base_path + "maps/test_05", renderer);

  if(mode == MAP)
    return game_map->render(renderer);
  
  return true;
}
  
/* Set the running configuration, from the options class */
bool Game::setConfiguration(Options* running_config)
{
  if(running_config != nullptr)
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
  /* Poll System Events */
  pollEvents();

  if(mode == MAP)
    return game_map->update(cycle_time);

  return false;
}