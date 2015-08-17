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
 *
 *     - Construct Battle object empty.
 *     - after battle transition, map sends:
 *     - eventStartBattle()
 *          -> battle prep friends -> load into Battle
 *          -> battle prep foes -> load into battle
 *          -> assert ai check
 *          -> battle->begin()
 *               -  setup class, determine turn mode, load flags, create objects
 *          -> swap display/updating to battle
 *     - victory conditions->
 *        - battle sends victory "signal"
 *        - allies clean-up
 *        - foes, delete if created as copies of base persons from map
 *     - run conditions->
 *        - battle sends run "signal"
 *        - allies clean up
 *        - turn back to map
 *     - defeat
 *       - battle sends defeat "signal"
 *       - back to Title.
 *
 *  3. Add victory screen logic
 *
 *     -- I wanted to do this just inside of Battle while all the info is
 *        available, and event handler just has to handle post-victory screen.
*
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
  num_ticks = 0;

  /* Initalize class variables */
  active_renderer = nullptr;
  base_path = "";
  battle_display = nullptr;
  game_battle    = nullptr;
  game_inventory = nullptr;
  game_config    = nullptr;
  game_map       = nullptr;
  mode = DISABLED;
  test_map = "";

  /* Set game configuration */
  setConfiguration(running_config);

  /* Set up the render classes */
  setupPlayerInventory();
  setupMap();
}

/* Destructor function */
Game::~Game()
{
  game_config = nullptr;
  mode = DISABLED;

  /* Delete battle */
  if(game_battle != nullptr)
  {
    delete game_battle;
    game_battle = nullptr;
  }

  if(battle_display != nullptr)
  {
    delete battle_display;
    battle_display = nullptr;
  }

  /* Delete map */
  if(game_map != nullptr)
  {
    delete game_map;
    game_map = nullptr;
  }

  //TODO: Delete player -> delete inventory instead.
  if(game_inventory != nullptr)
  {
    delete game_inventory;
    game_inventory = nullptr;
  }

  /* Delete all game actions */
  for(auto it = begin(action_list); it != end(action_list); ++it)
    delete(*it);

  /* Delete battle class categories actions */
  for(auto it = begin(battle_class_list); it != end(battle_class_list); ++it)
    delete(*it);

  /* Delete all race categories*/
  for(auto it = begin(race_list); it != end(race_list); ++it)
    delete(*it);

  /* Delete all flavour categories */
  for(auto it = begin(flavour_list); it != end(flavour_list); ++it)
    delete(*it);

  /* Delete all skills */
  for(auto it = begin(skill_list); it != end(skill_list); ++it)
    delete(*it);

  for(auto it = begin(base_person_list); it != end(base_person_list); ++it)
  /* Delete all game actions */
    delete(*it);

  /* Delete all game base items */
  for(auto it = begin(base_item_list); it != end(base_item_list); ++it)
    delete(*it);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

//TODO: Battle display data container, instead of instantiating upon
// display creation? [07-18-15]
void Game::buildBattleDisplayFrames(SDL_Renderer* renderer)
{
  if (battle_display != nullptr)
  {
    /* Set the ailments */
    battle_display->setAilment(Infliction::POISON,
        base_path + "sprites/Battle/Ailments/Poison_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::BURN,
        base_path + "sprites/Battle/Ailments/Burn01_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::SCALD,
        base_path + "sprites/Battle/Ailments/Burn02_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::CHARR,
        base_path + "sprites/Battle/Ailments/Burn03_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::BERSERK,
        base_path + "sprites/Battle/Ailments/Berserk_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::CONFUSE,
        base_path + "sprites/Battle/Ailments/Confused_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::SILENCE,
        base_path + "sprites/Battle/Ailments/Silence_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::BUBBIFY,
        base_path + "sprites/Battle/Ailments/Bubbified_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::DEATHTIMER,
        base_path + "sprites/Battle/Ailments/DeathTimer_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::PARALYSIS,
        base_path + "sprites/Battle/Ailments/Paralysis_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::BLINDNESS,
        base_path + "sprites/Battle/Ailments/Blind_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::DREADSTRUCK,
        base_path + "sprites/Battle/Ailments/DreadStruck_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::DREAMSNARE,
        base_path + "sprites/Battle/Ailments/DreamSnare_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::HELLBOUND,
        base_path + "sprites/Battle/Ailments/HellBound_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::BOND,
        base_path + "sprites/Battle/Ailments/Bond02_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::BONDED,
        base_path + "sprites/Battle/Ailments/Bond01_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::ALLATKBUFF,
        base_path + "sprites/Battle/Ailments/AllAtkBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::ALLDEFBUFF,
        base_path + "sprites/Battle/Ailments/AllDefBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::PHYBUFF,
        base_path + "sprites/Battle/Ailments/PhyBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::THRBUFF,
        base_path + "sprites/Battle/Ailments/ThrBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::POLBUFF,
        base_path + "sprites/Battle/Ailments/PolBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::PRIBUFF,
        base_path + "sprites/Battle/Ailments/PriBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::CHGBUFF,
        base_path + "sprites/Battle/Ailments/ChgBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::CYBBUFF,
        base_path + "sprites/Battle/Ailments/CybBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::NIHBUFF,
        base_path + "sprites/Battle/Ailments/NihBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::LIMBUFF,
        base_path + "sprites/Battle/Ailments/LimBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::UNBBUFF,
        base_path + "sprites/Battle/Ailments/_placeholder.png", renderer);
    battle_display->setAilment(Infliction::VITBUFF,
        base_path + "sprites/Battle/Ailments/VitBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::QDBUFF,
        base_path + "sprites/Battle/Ailments/QDBuff_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::ROOTBOUND,
        base_path + "sprites/Battle/Ailments/RootBound_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::DOUBLECAST,
        base_path + "sprites/Battle/Ailments/DoubleCast_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::TRIPLECAST,
        base_path + "sprites/Battle/Ailments/TripleCast_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::HALFCOST,
        base_path + "sprites/Battle/Ailments/HalfCost_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::REFLECT,
        base_path + "sprites/Battle/Ailments/Reflect_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::HIBERNATION,
        base_path + "sprites/Battle/Ailments/Hibernation_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::CURSE,
        base_path + "sprites/Battle/Ailments/Curse_AA_A00.png", renderer);
    battle_display->setAilment(Infliction::METATETHER,
        base_path + "sprites/Battle/Ailments/MetaTether_AA_A00.png", renderer);

    /* Set the elements */
    battle_display->setElement(Element::PHYSICAL, base_path +
            "sprites/Battle/Skills/Elements/Physical_AA_A00.png", renderer);
    battle_display->setElement(Element::FIRE, base_path +
            "sprites/Battle/Skills/Elements/Thermal_AA_A00.png", renderer);
    battle_display->setElement(Element::ICE, base_path +
            "sprites/Battle/Skills/Elements/Polar_AA_A00.png", renderer);
    battle_display->setElement(Element::FOREST, base_path +
            "sprites/Battle/Skills/Elements/Primal_AA_A00.png", renderer);
    battle_display->setElement(Element::ELECTRIC, base_path +
            "sprites/Battle/Skills/Elements/Charge_AA_A00.png", renderer);
    battle_display->setElement(Element::DIGITAL, base_path +
            "sprites/Battle/Skills/Elements/Cyber_AA_A00.png", renderer);
    battle_display->setElement(Element::NIHIL, base_path +
            "sprites/Battle/Skills/Elements/Nihil_AA_A00.png", renderer);

    /* Set the action scopes */
    battle_display->setScope(ActionScope::USER, base_path +
        "sprites/Battle/Skills/Targets/user_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ONE_TARGET, base_path +
        "sprites/Battle/Skills/Targets/single_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ONE_ENEMY, base_path +
        "sprites/Battle/Skills/Targets/singleenemy_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::TWO_ENEMIES, base_path +
        "sprites/Battle/Skills/Targets/twoenemies_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ALL_ENEMIES, base_path +
        "sprites/Battle/Skills/Targets/allenemies_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ONE_ALLY, base_path +
        "sprites/Battle/Skills/Targets/singleally_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ONE_ALLY_NOT_USER, base_path +
        "sprites/Battle/Skills/Targets/singlenotuserally_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::TWO_ALLIES, base_path +
        "sprites/Battle/Skills/Targets/twoallies_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ALL_ALLIES, base_path +
        "sprites/Battle/Skills/Targets/allallies_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ONE_ALLY_KO, base_path +
        "sprites/Battle/Skills/Targets/singlekoedally_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ALL_ALLIES_KO, base_path +
        "sprites/Battle/Skills/Targets/allkoedallies_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ONE_PARTY, base_path +
        "sprites/Battle/Skills/Targets/singleparty_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ALL_TARGETS, base_path +
        "sprites/Battle/Skills/Targets/all_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::NOT_USER, base_path +
        "sprites/Battle/Skills/Targets/singlenotuser_AA_A00.png", renderer);
    battle_display->setScope(ActionScope::ALL_NOT_USER, base_path +
        "sprites/Battle/Skills/Targets/allnotuser_AA_A00.png", renderer);

    /* Set up the extram frames */
    battle_display->setFramePercent(base_path +
                  "sprites/Battle/Skills/Extras/Accuracy_AA_A00.png", renderer);
    battle_display->setFrameQD(base_path +
                  "sprites/Battle/Skills/Extras/QDSymbol_AA_A00.png", renderer);
    battle_display->setFrameTime(base_path +
                  "sprites/Battle/Skills/Extras/Cooldown_AA_A00.png", renderer);

    /* Set up the Ailment pleps */
    //TODO: Temporary - fix
    auto temp_sprite = new Sprite(game_config->getBasePath() +
        "sprites/Battle/Pleps/hibernationplep_AA_A", 4, ".png", renderer);
    temp_sprite->insertTail(game_config->getBasePath() +
                            "sprites/Battle/Pleps/hibernationplep_AA_A00.png", renderer);
    temp_sprite->insertTail(game_config->getBasePath() +
                            "sprites/Battle/Pleps/hibernationplep_AA_A01.png", renderer);
    temp_sprite->insertTail(game_config->getBasePath() +
                            "sprites/Battle/Pleps/hibernationplep_AA_A02.png", renderer);
    temp_sprite->insertTail(game_config->getBasePath() +
                            "sprites/Battle/Pleps/hibernationplep_AA_A03.png", renderer);
    for(uint16_t i = 0; i < 5; i++)
      temp_sprite->insertTail(game_config->getBasePath() +
                              "sprites/blank.png", renderer);
    temp_sprite->setAnimationTime(180);

    battle_display->setAilmentPlep(Infliction::POISON, temp_sprite);
  }
}

/* A give item event, based on an ID and count (triggered from stored event */
bool Game::eventGiveItem(int id, int count)
{
  /* Attempt to find the item */
  Item* found_item = nullptr;
  for(auto it = base_item_list.begin(); it != base_item_list.end(); it++)
    if((*it)->getGameID() == id)
      found_item = (*it);

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
    was_inserted = true; // TODO: Fix once give item is working

    /* If the insert was successful, pickup the item */
    if(game_map != nullptr && was_inserted)
      game_map->pickupItem(item);
  }
}

/* Starts a battle event. Using the given information */
// TODO: Need to revise. It needs a party attacking ID
void Game::eventStartBattle()
{
  setupBattle();
  //mode = BATTLE; // TODO: REPAIR
}

/* Teleport thing event, based on ID and coordinates */
void Game::eventTeleportThing(int thing_id, int x, int y, int section_id)
{
  if(game_map != nullptr)
    game_map->teleportThing(thing_id, x, y, section_id);
}

// TODO: Comment
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
  // TODO
  std::cout << "Battle TODO - in game" << std::endl;
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

/* Returns a pointer to a given action by index or by ID */
Action* Game::getAction(const int32_t &index, const bool& by_id)
{
  if (by_id)
  {
    for (auto it = begin(action_list); it != end(action_list); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < action_list.size())
  {
    return action_list.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a battle class by index or by ID */
Category* Game::getBattleClass(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(battle_class_list); it != end(battle_class_list); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < battle_class_list.size())
  {
    return battle_class_list.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a race category by index or by ID */
Category* Game::getCategory(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(race_list); it != end(race_list); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < race_list.size())
  {
    return race_list.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a flavour by index or by ID */
Flavour* Game::getFlavour(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(flavour_list); it != end(flavour_list); ++it)
      if ((*it)->getGameID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < flavour_list.size())
  {
    return flavour_list.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a skill by index or by ID */
Skill* Game::getSkill(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(skill_list); it != end(skill_list); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < skill_list.size())
  {
    return skill_list.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a person by index or by ID */
Person* Game::getPerson(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(base_person_list); it != end(base_person_list); ++it)
      if ((*it)->getGameID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < base_person_list.size())
  {
    return base_person_list.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a person by index or by ID */
Item* Game::getItem(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(base_item_list); it != end(base_item_list); ++it)
      if ((*it)->getGameID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < base_item_list.size())
  {
    return base_item_list.at(index);
  }

  return nullptr;
}

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

    if(game_battle != nullptr)
    {
      delete game_battle;
      game_battle = nullptr;
    }
  }
  /* Switch the view to the battle */
  else if(event.keysym.sym == SDLK_F2)
  {
    if (game_battle == nullptr)
      eventStartBattle();
  }
  else if(event.keysym.sym == SDLK_F5 && mode == MAP && game_map != nullptr)
  {
    game_map->reloadMap(active_renderer);
  }
  /* Show item store dialog in map */
  else if(event.keysym.sym == SDLK_5)
  {
    if (game_map != nullptr)
    {
      std::vector<Item*> items;
      items.push_back(base_item_list[0]);
      items.push_back(base_item_list[0]);
      std::vector<uint32_t> counts;
      counts.push_back(2);
      counts.push_back(3);
      std::vector<int32_t> cost_modifiers;
      cost_modifiers.push_back(0);
      cost_modifiers.push_back(10);

      game_map->initStore(ItemStore::BUY, items, counts,
                          cost_modifiers, "Kevin's Store", false);
    }
  }
  else if(event.keysym.sym == SDLK_LCTRL)
  {
    // if(battle_display != NULL)
    //   battle_display->setRenderFlag(RenderState::SHOW_INFO);
  }
  /* Otherwise, send keys to the active view */
  else
  {
    if(mode == MAP)
      return game_map->keyDownEvent(event);
    else if(mode == BATTLE)
    {
      if(battle_display->getRenderingState() == game_battle->getTurnState())
         // !battle_display->getRenderFlag(RenderState::SHOW_INFO))
        return game_battle->keyDownEvent(event);
    }
  }

  return false;
}

/* The key up events to be handled by the class */
void Game::keyUpEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_LCTRL)
  {
    // if(battle_display != NULL)
    //   battle_display->setShowInfo(false);
  }
  else if(mode == MAP)
  {
    game_map->keyUpEvent(event);
  }
}

/* Renders the title screen */
bool Game::render(SDL_Renderer* renderer)
{
  // TODO: Create temporary list of items - Pull into file and remove */
  if(base_item_list.empty())
  {
    Item* item1 = new Item(0, "Bubby Saber", 125, new Frame(
        "sprites/Map/Tiles/00_Generic/Scenery/Interactables/Weapons/02_Uncommon/BubbySaber01_AA_A00.png",
        renderer));
    Item* item2 = new Item(1, "Frost Bubby", 5, new Frame(
                           "sprites/Battle/Bubbies/frosty_t1.png", renderer));
    Item* item3 = new Item(2, "Coins", 1, new Frame(
                           "sprites/Map/_TEST/coins_AA_A00.png", renderer));
    Item* item4 = new Item(3, "Ravizer Sword", 250, new Frame(
        "sprites/Map/Tiles/00_Generic/Scenery/Interactables/Weapons/03_Rare/RavizerSword07_AA_A00.png",
        renderer));
    Item* item5 = new Item(4, "Blazing Bubby", 4, new Frame(
                            "sprites/Battle/Bubbies/blazing_t1.png", renderer));
    base_item_list.push_back(item1);
    base_item_list.push_back(item2);
    base_item_list.push_back(item3);
    base_item_list.push_back(item4);
    base_item_list.push_back(item5);
  }

  /* Make sure the active renderer is set up */
  // TODO: Possibly revise. Change how the game handles maps and changing
  if(active_renderer == NULL)
    active_renderer = renderer;

  /* Map initialization location */
  if(!game_map->isLoaded())
  {
    if(test_map.empty())
      game_map->loadMap(base_path + "maps/Univursa.ugv", renderer);
    else
      game_map->loadMap(test_map, renderer);
  }

  if(mode == MAP)
    return game_map->render(renderer);
  else if(mode == BATTLE)
    return battle_display->render(renderer);

  return true;
}

/* Set the running configuration, from the options class */
bool Game::setConfiguration(Options* running_config)
{
  if(running_config != nullptr)
  {
    game_config = running_config;
    base_path   = game_config->getBasePath();

    /* Set in secondary classes */
    if(game_map != nullptr)
      game_map->setConfiguration(running_config);

    /* Battle configuration setup */
    if(battle_display != nullptr)
      battle_display->setConfiguration(running_config);

    return true;
  }

  return false;
}

/* Sets the test map to run instead of current default */
void Game::setTestMap(std::string test_map)
{
  this->test_map = test_map;
}

/* Updates the game state. Returns true if the class is finished */
bool Game::update(int32_t cycle_time)
{
  num_ticks++;

  /* Poll System Events */
  pollEvents();

  /* Update the key handler */
  event_handler.getKeyHandler().update(cycle_time);

  if(mode == MAP && game_map != nullptr)
    return game_map->update(cycle_time);

  if(mode == BATTLE)
  {
    bool success = true;

    if(game_battle != nullptr)
    {
      success &= game_battle->update(cycle_time);

      if(game_battle->getBattleFlag(CombatState::OUTCOME_PERFORMED))
        mode = MAP;
    }
    if(battle_display != nullptr)
      success &= battle_display->update(cycle_time);

    return success;
  }

  return false;
}
