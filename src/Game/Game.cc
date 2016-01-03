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

//const uint32_t Game::kSTARTING_MAP = 0;
//const std::string Game::kSTARTING_PATH = "maps/Univursa.ugv";

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Game::Game(Options* running_config)
{
  ticks_total = 0;

  /* Initalize class variables */
  active_renderer = nullptr;
  base_path = "";
  battle_ctrl = nullptr;
  battle_display_data = nullptr;
  config = nullptr;
  game_path = "";
  loaded_core = false;
  loaded_sub = false;
  map_lvl = -1;
  mode = DISABLED;
  mode_load = NOLOAD;
  mode_next = NONE;
  player_main = nullptr;

  /* Set up map class */
  map_ctrl.setConfiguration(config);
  map_ctrl.setEventHandler(&event_handler);

  /* Set up battle class */
  battle_ctrl = new Battle();
  battle_display_data = new BattleDisplayData();

  /* Set game configuration */
  setConfiguration(running_config);
  battle_ctrl->setDisplayData(battle_display_data);
}

/* Destructor function */
Game::~Game()
{
  /* First, unload the game */
  unload();

  /* Clean config */
  config = nullptr;

  /* Delete battle */
  if(battle_ctrl)
    delete battle_ctrl;
  battle_ctrl = nullptr;

  if(battle_display_data)
    delete battle_display_data;
  battle_display_data = nullptr;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Add functions for game objects */
Action* Game::addAction(const std::string& raw)
{
  Action* new_action = new Action(raw);
  list_action.push_back(new_action);

  return new_action;
}

/* Add functions for game objects */
Category* Game::addClass(const int32_t& id)
{
  Category* new_category = new Category();
  new_category->setID(id);
  list_class.push_back(new_category);

  return new_category;
}

/* Add functions for game objects */
Flavour* Game::addFlavour(const int32_t& id)
{
  Flavour* new_flavour = new Flavour();
  new_flavour->setID(id);
  list_flavour.push_back(new_flavour);

  return new_flavour;
}

/* Add functions for game objects */
Item* Game::addItem(const int32_t& id, SortObjects type)
{
  Item* new_item = nullptr;

  if(type == SortObjects::ITEMS)
  {
    new_item = new Item();
  }
  else if(type == SortObjects::BUBBIES)
  {
    new_item = new Bubby();
  }
  else if(type == SortObjects::EQUIPMENTS)
  {
    new_item = new Equipment();
  }
  else if(type == SortObjects::ZERO_BUBBIES)
  {
    new_item = new Item();
    new_item->setFlag(ItemFlags::BUBBY, true);
  }

  new_item->setGameID(id);
  list_item.push_back(new_item);

  return new_item;
}

/* Add functions for game objects */
Party* Game::addParty(const int32_t& id)
{
  Party* new_party = new Party();
  new_party->setID(id);
  list_party.push_back(new_party);

  return new_party;
}

/* Add functions for game objects */
Person* Game::addPersonBase(const int32_t& id)
{
  Person* new_person = new Person();
  new_person->setGameID(id);
  list_person_base.push_back(new_person);

  return new_person;
}

/* Add functions for game objects */
Person* Game::addPersonInst(const int32_t& base_id, const uint32_t& lvl)
{
  return addPersonInst(getPersonBase(base_id), lvl);
}

/* Add functions for game objects */
Person* Game::addPersonInst(Person* base_person, const uint32_t& lvl)
{
  Person* new_person = nullptr;

  if(base_person != NULL)
  {
    new_person = new Person(base_person);

    /* Set Level */
    new_person->loseExp(new_person->getTotalExp());
    new_person->addExp(new_person->getExpAt(lvl), true, true);

    list_person_inst.push_back(new_person);
  }

  return new_person;
}

/* Add functions for game objects */
Category* Game::addRace(const int32_t& id)
{
  Category* new_race = new Category();
  new_race->setID(id);
  list_race.push_back(new_race);

  return new_race;
}

/* Add functions for game objects */
Skill* Game::addSkill(const int32_t& id)
{
  Skill* new_skill = new Skill();
  new_skill->setID(id);
  list_skill.push_back(new_skill);

  return new_skill;
}

/* Add functions for game objects */
SkillSet* Game::addSkillSet(const int32_t& id)
{
  SkillSet* new_set = new SkillSet(id);
  list_set.push_back(new_set);

  return new_set;
}

/* Change the mode that the game is running */
bool Game::changeMode(GameMode mode)
{
  /* Run logic to determine if mode switch is allowed - currently always true */
  bool allow = true;

  /* If allowed, make change */
  if(allow && this->mode != mode && mode_next != mode)
  {
    /* Changes to execute on the view closing */
    if(this->mode == MAP)
      map_ctrl.enableView(false);

    /* Set the next mode */
    mode_next = mode;
    updateMode(0);

    /* PRINT - TEMPORARY */
    if(mode == DISABLED)
      std::cout << "GAME MODE: DISABLED" << std::endl;
    else if(mode == MAP)
      std::cout << "GAME MODE: MAP" << std::endl;
    else if(mode == BATTLE)
      std::cout << "GAME MODE: BATTLE" << std::endl;
    else if(mode == VICTORY_SCREEN)
      std::cout << "GAME MODE: VICTORY" << std::endl;
    else if(mode == LOADING)
      std::cout << "GAME MODE: LOADING" << std::endl;

    /* Changes to execute on the view opening */
    if(mode == MAP)
      map_ctrl.enableView(true);
  }

  return allow;
}

/* A give item event, based on an ID and count (triggered from stored event */
int Game::eventGiveItem(int id, int count)
{
  int insert_count = 0;

  /* Attempt to find the item and insert */
  Item* found_item = getItem(id);
  if(found_item != nullptr && count > 0)
  {
    /* Try and insert into sleuth inventory */
    if(player_main != nullptr)
    {
      /* If ID matches item ID, add credits */
      if(id == (int)Item::kID_MONEY)
      {
        if(player_main->addCredits(count))
          insert_count = count;
      }
      /* Otherwise, just general item */
      else if(player_main->getSleuth() != nullptr &&
              player_main->getSleuth()->getInventory() != nullptr)
      {
        Inventory* inv = player_main->getSleuth()->getInventory();

        /* Create item and check room */
        Item* new_item = new Item(found_item);
        int room = inv->hasRoom(new_item, count);

        /* Attempt add */
        AddStatus status =
            player_main->getSleuth()->getInventory()->add(new_item, room);
        if(status == AddStatus::GOOD_DELETE)
        {
          delete new_item;
          insert_count = room;
        }
        else if(status == AddStatus::GOOD_KEEP)
        {
          insert_count = room;
        }
      }
    }

    /* If inserted, notify that the pickup was a success */
    if(insert_count > 0)
    {
      map_ctrl.initNotification(found_item->getThumb(), insert_count);
    }
    /* Notify that item could not be received */
    if(count > insert_count)
    {
      map_ctrl.initNotification("Insufficient room in inventory to fit " +
                                std::to_string(count - insert_count) + " " +
                                found_item->getName());
    }
  }

  return insert_count;
}

/* Initiates a conversation event */
void Game::eventInitConversation(ConvoPair convo_pair, MapThing* source)
{
  map_ctrl.initConversation(convo_pair, source);
}

/* Initiates a notification event (in map) */
void Game::eventInitNotification(std::string notification)
{
  map_ctrl.initNotification(notification);
}

/* The pickup item event - from walking over or triggering from action key */
void Game::eventPickupItem(MapItem* item, bool walkover)
{
  if(item != nullptr && item->isWalkover() == walkover)
  {
    int insert_count = eventGiveItem(item->getGameID(), item->getCount());

    /* If the insert was successful, pickup the item */
    if(insert_count > 0)
      map_ctrl.pickupItem(item, insert_count);
  }
}

/* Starts a battle event. Using the given information */
bool Game::eventStartBattle(int person_id, int source_id)
{
  if(person_id >= 0 && source_id >= 0 && mode == BATTLE)//MAP && mode_next == NONE)
  {
    if(battle_ctrl)
    {
      battle_ctrl->setRenderer(active_renderer);
      battle_ctrl->setEventHandler(&event_handler);
      battle_display_data->setRenderer(active_renderer);

      if(!battle_display_data->isDataBuilt())
        battle_display_data->buildData();

      // TODO: Why is it seg faulting?
      battle_ctrl->startBattle(getParty(person_id), getParty(source_id));

      //changeMode(BATTLE);
      return true;
    }
  }
  return false;
}

/* Switch maps event. - utilizing a map ID */
void Game::eventSwitchMap(int map_id)
{
  if(map_id >= 0 && map_id != map_lvl)
  {
    map_lvl = map_id;
    load(game_path, active_renderer, "", false, false);
  }
}

/* A take item event, based on an ID and count (triggered from event) */
int Game::eventTakeItem(int id, int count)
{
  int remove_count = 0;

  if(player_main != nullptr && player_main->getSleuth() != nullptr &&
     player_main->getSleuth()->getInventory() != nullptr)
  {
    Inventory* inv = player_main->getSleuth()->getInventory();
    int item_count = 0;
    if(id == (int)Item::kID_MONEY)
      item_count = player_main->getCredits();
    else
      item_count = inv->getItemCount(id);
    if(item_count > 0)
    {
      /* Update if requested count is greater than range */
      if(item_count < count)
        remove_count = item_count;
      else
        remove_count = count;

      /* Attempt remove */
      bool removed = false;
      if(id == (int)Item::kID_MONEY)
        removed = player_main->removeCredits(remove_count);
      else
        removed = inv->removeID(id, remove_count);

      /* If removed, proceed to notify */
      if(removed)
      {
        /* Notify about removal */
        Item* found_item = getItem(id);
        if(found_item != nullptr)
           map_ctrl.initNotification(found_item->getThumb(), -remove_count);
      }
      else
      {
        remove_count = 0;
      }
    }
  }

  return remove_count;
}

/* Teleport thing event, based on ID and coordinates */
void Game::eventTeleportThing(int thing_id, int x, int y, int section_id)
{
  map_ctrl.teleportThing(thing_id, x, y, section_id);
}

/* Trigger IO event, based on the IO object and interaction state */
void Game::eventTriggerIO(MapInteractiveObject* io, int interaction_state,
                          MapPerson* initiator)
{
  if(io != nullptr)
  {
    /* Handle lock struct */
    Locked lock_struct = io->getLock();
    if(parseLock(lock_struct))
      io->setLock(lock_struct);

    /* Handle trigger */
    io->handlerTrigger(interaction_state, initiator);
  }
}

/* Unlock events, based on parameter information */
void Game::eventUnlockIO(int io_id, UnlockIOMode mode, int state_num,
                         UnlockIOEvent mode_events, UnlockView mode_view,
                         int view_time)
{
  map_ctrl.unlockIO(io_id, mode, state_num, mode_events, mode_view, view_time);
}

/* Unlock events, based on parameter information */
void Game::eventUnlockThing(int thing_id, UnlockView mode_view, int view_time)
{
  map_ctrl.unlockThing(thing_id, mode_view, view_time);
}

/* Unlock events, based on parameter information */
void Game::eventUnlockTile(int section_id, int tile_x, int tile_y,
                           UnlockTileMode mode, UnlockView mode_view,
                           int view_time)
{
  map_ctrl.unlockTile(section_id, tile_x, tile_y, mode, mode_view, view_time);
}

/* Load game */
// TODO: Comment
// Notes: Revise that when inst file is blank, it uses default starting map.
//        Otherwise, determine current map from inst first, then load correct
//        base followed by correct inst map and all associated data.
bool Game::load(std::string base_file, SDL_Renderer* renderer,
                std::string inst_file, bool encryption, bool full_load)
{
  (void)inst_file;

  bool done = false;
  bool read_success = true;
  int index = 0;
  std::string level = std::to_string(map_lvl);
  bool success = true;

  /* Ensure nothing is loaded - if full load is false, just unloads map */
  unload(full_load);

  /* Loading trigger */
  //changeMode(LOADING); // TODO: Keep or Remove

  /* Initial set-up */
  if(full_load)
    player_main = new Player();

  /* Create the file handler */
  FileHandler fh(base_file, false, true, encryption);
  XmlData data;

  /* Start the map read */
  success &= fh.start();
  std::string type = " - SUB";
  if(full_load)
    type = " - FULL";
  std::cout << "--" << std::endl << "Game Load: " << fh.getDate() << type
            << std::endl << "--" << std::endl;

  /* If file open was successful, move forward */
  if(success)
  {
    do
    {
      /* Read set of XML data */
      data = fh.readXmlData(&done, &read_success);
      success &= read_success;

      /* Only proceed if inside game */
      if(data.getElement(index) == "game")
      {
        /* Core game data */
        if(data.getElement(index + 1) == "core" && full_load)
        {
          success &= loadData(data, index + 2, renderer);
        }
        /* Map data */
        else if(data.getElement(index + 1) == "map" &&
                data.getKeyValue(index + 1) == level)
        {
          success &= map_ctrl.loadData(data, index + 2, renderer, base_path);
        }
      }
    } while(!done); // && success); // TODO: Success in loop??
  }

  success &= fh.stop();

  /* If the load was successful, proceed to clean-up */
  if(success)
  {
    if(full_load)
    {
      /* Player set-up */
      player_main->setSleuth(getParty(Party::kID_SLEUTH));
      player_main->setBearacks(getParty(Party::kID_BEARACKS));
    }

    /* Clean up map */
    map_ctrl.loadDataFinish(renderer);
    changeMode(MAP);
  }
  /* If failed, unload */
  else
  {
    unload(full_load);
    changeMode(DISABLED);
  }

  /* Update loaded status */
  if(full_load)
    loaded_core = success;
  loaded_sub = success;

  return success;
}

/* Load game specific data */
// TODO: Comment
bool Game::loadData(XmlData data, int index, SDL_Renderer* renderer)
{
  bool success = true;

  /* ID index */
  int id = -1;
  std::string id_str = data.getKeyValue(index);
  if(!id_str.empty())
    id = std::stoi(id_str);

  /* ---- ACTIONS ---- */
  if(data.getElement(index) == "action")
  {
    addAction(data.getDataString());
  }
  /* ---- CLASSES ---- */
  else if(data.getElement(index) == "class")
  {
    Category* edit_class = getClass(id);
    if(edit_class == nullptr)
      edit_class = addClass(id);

    /* Data for class */
    if(data.getElement(index + 1) == "skillset")
      edit_class->setSkills(getSkillSet(data.getDataInteger(&success)));
    else
      success &= edit_class->loadData(data, index + 1, renderer);
  }
  /* ---- ITEMS ---- */
  else if(data.getElement(index) == "item")
  {
    Item* edit_item = getItem(id);
    if(edit_item == nullptr)
      edit_item = addItem(id);

    /* Data for skill */
    if(data.getElement(index + 1) == "skill")
      edit_item->setUseSkill(getSkill(data.getDataInteger(&success)));
    else
      success &= edit_item->loadData(data, index + 1, renderer, base_path);
  }
  /* ---- PARTIES ---- */
  else if(data.getElement(index) == "party")
  {
    Party* edit_party = getParty(id);
    if(edit_party == nullptr)
      edit_party = addParty(id);

    /* Data for party */
    if(data.getElement(index + 1) == "person")
    {
      std::string person_str = data.getDataString(&success);
      if(success)
      {
        /* Comma split */
        std::vector<std::string> person_set = Helpers::split(person_str, ',');
        if(person_set.size() == 2)
        {
          int person_id = std::stoi(person_set.front());
          int person_lvl = std::stoi(person_set.back());

          success &=
              edit_party->addMember(addPersonInst(person_id, person_lvl));
        }
      }
    }
    else if(data.getElement(index + 1) == "inventory")
    {
      /* Items */
      if(data.getElement(index + 2) == "item")
      {
        std::string item_str = data.getDataString(&success);
        if(success)
        {
          /* Comma split */
          std::vector<std::string> item_set = Helpers::split(item_str, ',');
          if(item_set.size() == 2)
          {
            int item_id = std::stoi(item_set.front());
            int item_count = std::stoi(item_set.back());

            Item* new_item = new Item(getItem(item_id));

            AddStatus status =
                edit_party->getInventory()->add(new_item, item_count);
            if(status == AddStatus::GOOD_DELETE)
              delete new_item;
            else if(status == AddStatus::FAIL)
              success = false;
          }
        }
      }
    }
    else
    {
      success &= edit_party->loadData(data, index + 1, renderer, base_path);
    }
  }
  /* ---- PERSONS ---- */
  else if(data.getElement(index) == "person")
  {
    Person* edit_person = getPersonBase(id);
    if(edit_person == nullptr)
      edit_person = addPersonBase(id);

    /* Data for person */
    if(data.getElement(index + 1) == "class")
      edit_person->setClass(getClass(data.getDataInteger(&success)));
    else if(data.getElement(index + 1) == "race")
      edit_person->setRace(getRace(data.getDataInteger(&success)));
    else
      success &= edit_person->loadData(data, index + 1, renderer, base_path);
  }
  /* ---- PLAYER ---- */
  else if(data.getElement(index) == "player")
  {
    /* Data for player */
    if(data.getElement(index + 1) == "credits")
    {
      success &= player_main->setCredits(data.getDataInteger(&success));
    }
  }
  /* ---- RACES ---- */
  else if(data.getElement(index) == "race")
  {
    Category* edit_race = getRace(id);
    if(edit_race == nullptr)
      edit_race = addRace(id);

    /* Data for race */
    if(data.getElement(index + 1) == "skillset")
      edit_race->setSkills(getSkillSet(data.getDataInteger(&success)));
    else
      success &= edit_race->loadData(data, index + 1, renderer);
  }
  /* ---- SKILLS ---- */
  else if(data.getElement(index) == "skill")
  {
    Skill* edit_skill = getSkill(id);
    if(edit_skill == nullptr)
      edit_skill = addSkill(id);

    /* Data for skill */
    if(data.getElement(index + 1) == "action")
      edit_skill->addAction(getAction(data.getDataInteger(&success)), false);
    else
      success &= edit_skill->loadData(data, index + 1, renderer, base_path);

    /* Flag setup after changes */
    edit_skill->flagSetup();
  }
  /* ---- SKILLSETS ---- */
  else if(data.getElement(index) == "skillset")
  {
    SkillSet* edit_set = getSkillSet(id);
    if(edit_set == nullptr)
      edit_set = addSkillSet(id);

    /* Data for skill set */
    if(data.getElement(index + 1) == "skill")
    {
      std::string str_pair = data.getDataString(&success);
      if(success)
      {
        /* Split on the comma */
        std::vector<std::string> pair_split = Helpers::split(str_pair, ',');
        if(pair_split.size() == 2)
        {
          int first = std::stoi(pair_split.front());
          int second = std::stoi(pair_split.back());

          edit_set->addSkill(getSkill(first), second);
        }
      }
    }
  }

  return success;
}

/* Parse lock and attempt unlock */
bool Game::parseLock(Locked& lock_struct)
{
  /* Determine if struct is locked */
  if(EventSet::isLocked(lock_struct))
  {
    /* Parse state - ITEM locked event */
    if(lock_struct.state == LockedState::ITEM && player_main != nullptr &&
       player_main->getSleuth() != nullptr &&
       player_main->getSleuth()->getInventory() != nullptr)
    {
      /* Poll data */
      int id, count;
      bool consume;
      if(EventSet::dataLockedItem(lock_struct, id, count, consume))
      {
        /* Get count */
        Inventory* inv = player_main->getSleuth()->getInventory();
        int count_avail = 0;
        if(id == (int)Item::kID_MONEY)
          count_avail = player_main->getCredits();
        else
          count_avail = inv->getItemCount(id);

        /* Attempt unlock */
        if(count_avail >= count)
        {
          if(EventSet::unlockItem(lock_struct, id, count_avail))
            eventTakeItem(id, count);
          return true;
        }
      }
    }
  }
  return false;
}

// TODO: Comment
void Game::pollEvents()
{
  /* Only proceed if an event is available */
  if(event_handler.pollEventAvailable())
  {
    do
    {
      /* First check if the class is locked and could be unlocked (ITEM, etc) */
      Locked lock_struct;
      if(event_handler.pollLockGetData(lock_struct))
      {
        if(parseLock(lock_struct))
          event_handler.pollLockSetData(lock_struct);
      }

      /* Poll classification */
      EventClassifier classification = event_handler.pollEventType();
      if(classification == EventClassifier::GIVEITEM)
      {
        int id;
        int count;
        event_handler.pollGiveItem(&id, &count);
        eventGiveItem(id, count);
      }
      else if(classification == EventClassifier::JUSTSOUND)
      {
        event_handler.pollSound();
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
        /* Get the reference objects and check if valid */
        EventPair event_win, event_lose;
        BattleFlags flags;
        MapPerson* person;
        MapThing* source;
        if(event_handler.pollStartBattle(&person, &source, flags,
                                         event_win, event_lose))
        {
          if(person != nullptr && getParty(Party::kID_SLEUTH) != nullptr &&
             source != nullptr && getParty(source->getGameID()) != nullptr)
          {
            map_ctrl.initBattle(person, source, flags, event_win, event_lose);
          }
        }
      }
      else if(classification == EventClassifier::RUNMAP)
      {
        int id;
        event_handler.pollStartMap(&id);
        eventSwitchMap(id);
      }
      else if(classification == EventClassifier::STARTCONVO)
      {
        ConvoPair convo_pair;
        MapThing* source;
        event_handler.pollConversation(convo_pair, &source);
        eventInitConversation(convo_pair, source);
      }
      else if(classification == EventClassifier::TAKEITEM)
      {
        int id;
        int count;
        event_handler.pollTakeItem(&id, &count);
        eventTakeItem(id, count);
      }
      else if(classification == EventClassifier::TELEPORTTHING)
      {
        int thing_id, x, y, section_id;
        event_handler.pollTeleportThing(&thing_id, &x, &y, &section_id);
        eventTeleportThing(thing_id, x, y, section_id);
      }
      else if(classification == EventClassifier::TRIGGERIO)
      {
        MapPerson* initiator;
        int interaction_state;
        MapInteractiveObject* source;
        event_handler.pollTriggerIO(&source, &interaction_state, &initiator);
        eventTriggerIO(source, interaction_state, initiator);
      }
      else if(classification == EventClassifier::UNLOCKIO)
      {
        int io_id, state_num, view_time;
        UnlockIOMode mode;
        UnlockIOEvent mode_events;
        UnlockView mode_view;
        if(event_handler.pollUnlockIO(&io_id, &mode, &state_num, &mode_events,
                                      &mode_view, &view_time))
          eventUnlockIO(io_id, mode, state_num, mode_events, mode_view,
                        view_time);
      }
      else if(classification == EventClassifier::UNLOCKTHING)
      {
        int thing_id, view_time;
        UnlockView mode_view;
        if(event_handler.pollUnlockThing(&thing_id, &mode_view, &view_time))
          eventUnlockThing(thing_id, mode_view, view_time);
      }
      else if(classification == EventClassifier::UNLOCKTILE)
      {
        int section_id, tile_x, tile_y, view_time;
        UnlockTileMode mode;
        UnlockView mode_view;
        if(event_handler.pollUnlockTile(&section_id, &tile_x, &tile_y, &mode,
                                        &mode_view, &view_time))
          eventUnlockTile(section_id, tile_x, tile_y, mode, mode_view,
                          view_time);
      }
      /* Finally, check if no event */
      else if(classification == EventClassifier::NOEVENT)
      {
        event_handler.pollNone();
      }

    } while(event_handler.pollEvent());
  }

  event_handler.pollClear();
}

/* Remove functions for game objects */
void Game::removeActions()
{
  for(auto it = begin(list_action); it != end(list_action); ++it)
    delete(*it);
  list_action.clear();
}

/* Remove all game objects in the proper order */
void Game::removeAll()
{
  /* Delete main Player */
  delete player_main;
  player_main = nullptr;

  /* Delete parties and persons */
  removeParties();
  removePersonInstances();
  removePersonBases();

  /* Delete items and sets */
  removeItems();
  removeClasses();
  removeRaces();
  removeSkillSets();

  /* Delete skills and actions */
  removeFlavours();
  removeSkills();
  removeActions();
}

/* Remove functions for game objects */
void Game::removeClasses()
{
  for(auto it = begin(list_class); it != end(list_class); ++it)
    delete(*it);
  list_class.clear();
}

/* Remove functions for game objects */
void Game::removeFlavours()
{
  for(auto it = begin(list_flavour); it != end(list_flavour); ++it)
    delete(*it);
  list_flavour.clear();
}

/* Remove functions for game objects */
void Game::removeItems()
{
  for(auto it = begin(list_item); it != end(list_item); it++)
    delete(*it);
  list_item.clear();
}

/* Remove functions for game objects */
void Game::removeParties()
{
  for(auto it = begin(list_party); it != end(list_party); it++)
    delete(*it);
  list_party.clear();
}

/* Remove functions for game objects */
void Game::removePersonBases()
{
  for(auto it = begin(list_person_base); it != end(list_person_base); ++it)
    delete(*it);
  list_person_base.clear();
}

/* Remove functions for game objects */
void Game::removePersonInstances()
{
  for(auto it = begin(list_person_inst); it != end(list_person_inst); ++it)
    delete(*it);
  list_person_inst.clear();
}

/* Remove functions for game objects */
void Game::removeRaces()
{
  for(auto it = begin(list_race); it != end(list_race); ++it)
    delete(*it);
  list_race.clear();
}

/* Remove functions for game objects */
void Game::removeSkills()
{
  for(auto it = begin(list_skill); it != end(list_skill); ++it)
    delete(*it);
  list_skill.clear();
}

/* Remove functions for game objects */
void Game::removeSkillSets()
{
  for(auto it = begin(list_set); it != end(list_set); ++it)
    delete(*it);
  list_set.clear();
}

/* Update mode */
void Game::updateMode(int cycle_time)
{
  (void)cycle_time;
  if(mode_next != NONE)
  {
    if(mode == MAP)
    {
      if(map_ctrl.isModeDisabled())
      {
        mode = mode_next;
        if(mode_next == BATTLE)
        {
          eventStartBattle(Party::kID_SLEUTH, map_ctrl.getBattleThingID());
        }
        mode_next = NONE;
      }
    }
    else
    {
      mode = mode_next;
      mode_next = NONE;
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Enable view trigger */
void Game::enableView(bool enable)
{
  if(enable)
    changeMode(LOADING);
  else
    changeMode(DISABLED);

  //if(enable && isLoadedCore() && isLoadedSub())
  //  changeMode(MAP);
  //else
  //  changeMode(DISABLED);
}

/* Returns a pointer to a given action by index or by ID */
Action* Game::getAction(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_action); it != end(list_action); ++it)
      if((*it)->getID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_action.size())
  {
    return list_action.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a battle class by index or by ID */
Category* Game::getClass(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_class); it != end(list_class); ++it)
      if((*it)->getID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_class.size())
  {
    return list_class.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a flavour by index or by ID */
Flavour* Game::getFlavour(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_flavour); it != end(list_flavour); ++it)
      if((*it)->getGameID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_flavour.size())
  {
    return list_flavour.at(index);
  }

  return nullptr;
}

/* Returns the inventory of the corresponding party ID, if relevant */
Inventory* Game::getInventory(const int32_t& id)
{
  Party* found_party = getParty(id);
  if(found_party != nullptr)
    return found_party->getInventory();
  return nullptr;
}

/* Returns the player bearacks inventory pointer, if relevant */
Inventory* Game::getInvBearacks()
{
  /* Check if player is valid */
  if(player_main != nullptr)
  {
    /* Check if bearacks is valid */
    if(player_main->getBearacks() != nullptr)
    {
      return player_main->getBearacks()->getInventory();
    }
  }

  return nullptr;
}

/* Returns the player sleuth inventory pointer, if relevant */
Inventory* Game::getInvSleuth()
{
  /* Check if player is valid */
  if(player_main != nullptr)
  {
    /* Check if bearacks is valid */
    if(player_main->getSleuth() != nullptr)
    {
      return player_main->getSleuth()->getInventory();
    }
  }

  return nullptr;
}

/* Returns a pointer to a person by index or by ID */
Item* Game::getItem(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_item); it != end(list_item); ++it)
      if((*it)->getGameID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_item.size())
  {
    return list_item.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a party by index or by ID */
Party* Game::getParty(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_party); it != end(list_party); it++)
      if((*it)->getID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_party.size())
  {
    return list_party.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a base person by index or by ID (game) */
Person* Game::getPersonBase(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_person_base); it != end(list_person_base); ++it)
      if((*it)->getGameID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_person_base.size())
  {
    return list_person_base.at(index);
  }

  return nullptr;
}

/* Returns a pointer to an instance person by index or by ID (unique) */
Person* Game::getPersonInst(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_person_inst); it != end(list_person_inst); it++)
      if((*it)->getMyID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_person_inst.size())
  {
    return list_person_inst.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a race category by index or by ID */
Category* Game::getRace(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_race); it != end(list_race); ++it)
      if((*it)->getID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_race.size())
  {
    return list_race.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a skill by index or by ID */
Skill* Game::getSkill(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_skill); it != end(list_skill); ++it)
      if((*it)->getID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_skill.size())
  {
    return list_skill.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a skillset by index or by ID */
SkillSet* Game::getSkillSet(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(auto it = begin(list_set); it != end(list_set); it++)
      if((*it)->getID() == index)
        return (*it);
  }
  else if(static_cast<uint32_t>(index) < list_set.size())
  {
    return list_set.at(index);
  }

  return nullptr;
}

EventHandler& Game::getHandler()
{
  return event_handler;
}

/* Get the game mode */
Game::GameMode Game::getMode()
{
  return mode;
}

/* Is the game loaded */
//bool Game::isLoaded()
//{
//  return loaded;
//}

/* Is the game core data loaded */
bool Game::isLoadedCore()
{
  return loaded_core;
}

/* Is the game sub data loaded */
bool Game::isLoadedSub()
{
  return loaded_sub;
}

/* Mode checks - only returns true for ready if ready for control, reverse
 * for disabled */
bool Game::isModeDisabled()
{
  return (mode == DISABLED && mode_next == NONE);
}

/* Mode checks - only returns true for ready if ready for control, reverse
 * for disabled */
bool Game::isModeReady()
{
  return (mode != DISABLED && mode != NONE && mode_next == NONE);
}

/* The key down events to be handled by the class */
bool Game::keyDownEvent(SDL_KeyboardEvent event)
{
  /* TESTING section - probably remove at end */
  /* Inventory Testing */
  if(event.keysym.sym == SDLK_3)
  {
    if(player_main != nullptr && player_main->getSleuth() != nullptr &&
       player_main->getSleuth()->getInventory() != nullptr)
    {
      player_main->getSleuth()->getInventory()->print(false);
    }
  }
  /* Show item store dialog in map */
  else if(event.keysym.sym == SDLK_4)
  {
    std::cout << "TODO FUTURE: ITEM STORE TRIGGER" << std::endl;

    // TODO: Future
    // if (map_ctrl != nullptr)
    //{
    //  std::vector<Item*> items;
    //  items.push_back(list_item[0]);
    //  items.push_back(list_item[0]);
    //  std::vector<uint32_t> counts;
    //  counts.push_back(2);
    //  counts.push_back(3);
    //  std::vector<int32_t> cost_modifiers;
    //  cost_modifiers.push_back(0);
    //  cost_modifiers.push_back(10);
    //
    //  map_ctrl->initStore(ItemStore::BUY, items, counts,
    //                      cost_modifiers, "Kevin's Store", false);
    //}
  }
  /* Otherwise, send keys to the active view */
  else
  {
    /* -- MAP MODE -- */
    if(mode == MAP)
    {
      if(map_ctrl.keyDownEvent(event))
        changeMode(DISABLED);
    }
    /* -- TODO: TEMP - just for dev mode so you can escape battle -- */
    else if(event.keysym.sym == SDLK_BACKSPACE)
    {
      changeMode(DISABLED);
    }
    /* -- BATTLE MODE -- */
    else if(mode == BATTLE)
    {
      if(battle_ctrl->keyDownEvent(event))
        changeMode(DISABLED);
    }
  }

  return false;
}

/* The key up events to be handled by the class */
void Game::keyUpEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_LCTRL)
  {
    // if(battle != NULL)
    //   battle->setShowInfo(false);
  }
  else if(mode == MAP)
  {
    map_ctrl.keyUpEvent(event);
  }
}

/* Load game */
bool Game::load(SDL_Renderer* renderer, bool full_load)
{
  return load(base_path + game_path, renderer, "", false, full_load);
}

/* Renders the title screen */
bool Game::render(SDL_Renderer* renderer)
{
  /* -- LOADING MODE -- */
  if(mode == LOADING)
  {
    if(!isLoadedCore() || mode_load == FULLLOAD)
      load(renderer, true);
    else
      load(renderer, false);
    mode_load = NOLOAD;
  }

  /* -- MAP MODE -- */
  if(mode == MAP)
  {
    return map_ctrl.render(renderer);
  }
  /* -- BATTLE MODE -- */
  else if(mode == BATTLE)
  {
    /* Assign the rendererer to Battle and data container class */
    battle_display_data->setRenderer(renderer);

    /* Build the data if it isn't built */
    if(renderer && !battle_display_data->isDataBuilt())
      battle_display_data->buildData();
    else if(battle_display_data->isDataBuilt())
    {
      battle_ctrl->setRenderer(renderer);

      /* Render the battle */
      return battle_ctrl->render();
    }
  }

  return true;
}

/* Set the running configuration, from the options class */
bool Game::setConfiguration(Options* running_config)
{
  if(running_config)
  {
    config = running_config;
    base_path = config->getBasePath();

    /* Set in secondary classes */
    map_ctrl.setConfiguration(running_config);

    /* Battle configuration setup */
    if(battle_ctrl)
    {
      battle_ctrl->setConfig(running_config);
      battle_ctrl->setDisplayData(battle_display_data);
    }

    /* Battle display data configuration */
    if(battle_display_data)
      battle_display_data->setConfig(running_config);

    return true;
  }

  return false;
}

/* Sets the path of the game */
bool Game::setPath(std::string path, int level, bool load)
{
  if(!path.empty() && level >= 0)
  {
    /* Only execute change if it's different */
    if(path != game_path || map_lvl != level)
    {
      bool full_load = true;
      if(path == game_path)
        full_load = false;

      /* Update info */
      game_path = path;
      map_lvl = level;

      /* Handle what condition for the game */
      if(mode != DISABLED || load)
      {
        changeMode(LOADING);
      }
      else
      {
        unload(full_load);
      }

      /* Load mode */
      if(full_load)
        mode_load = FULLLOAD;
      else
        mode_load = SUBLOAD;
    }

    return true;
  }
  return false;
}

/* Sets the active renderer to be used */
void Game::setRenderer(SDL_Renderer* renderer)
{
  if(renderer != nullptr)
    active_renderer = renderer;
}

/* Sets the sound handler used. If unset, no sounds will play */
void Game::setSoundHandler(SoundHandler* new_handler)
{
  event_handler.setSoundHandler(new_handler);
}

/* Unload the game - full unload = false will only unload map*/
void Game::unload(bool full_unload)
{
  //changeMode(DISABLED);

  /* Unload map */
  event_handler.pollClear();
  unloadSub();

  /* Unload game data */
  if(full_unload)
    unloadCore();
}

/* Unloads the core data of the game */
void Game::unloadCore()
{
  removeAll();
  loaded_core = false;
}

/* Unloads the sub map data of the game */
void Game::unloadSub()
{
  map_ctrl.unloadMap();
  event_handler.pollClear();
  loaded_sub = false;
}

/* Updates the game state. Returns true if the class is finished */
bool Game::update(int32_t cycle_time)
{
  ++ticks_total;

  /* Poll System Events */
  //pollEvents();

  /* Update the key handler */
  event_handler.getKeyHandler().update(cycle_time);
  // event_handler.getKeyHandler().print(true);

  /* Mode next handling */
  updateMode(cycle_time);

  /* MAP MODE */
  if(mode == MAP)
  {
    /* Poll Events */
    pollEvents();

    /* Change mode to battle if ready */
    if(map_ctrl.isBattleReady())
      changeMode(BATTLE);

    //eventStartBattle(Party::kID_SLEUTH, map_ctrl.getBattleThingID());
    return map_ctrl.update(cycle_time);
  }
  /* BATTLE MODE */
  else if(mode == BATTLE && battle_ctrl)
  {
    if(battle_ctrl->getTurnState() == TurnState::FINISHED)
    {
      /* Check if health or qd should be restored */
      bool restore_health = map_ctrl.isBattleRestoreHealth();
      bool restore_qd = map_ctrl.isBattleRestoreQD();

      /* Check the status of the battle result */
      if(battle_ctrl->getOutcomeType() == OutcomeType::VICTORY)
      {
        map_ctrl.battleWon();
      }
      else if(battle_ctrl->getOutcomeType() == OutcomeType::DEFEAT)
      {
        if(map_ctrl.isBattleLoseGameOver())
          changeMode(DISABLED);
        else
          map_ctrl.battleLose();
      }
      else
      {
        map_ctrl.battleRun();
      }

      /* End the battle */
      battle_ctrl->stopBattle();

      /* Restore conditions */
      if(restore_health || restore_qd)
      {
        Party* sleuth = getParty(Party::kID_SLEUTH);
        if(sleuth != nullptr)
        {
          if(restore_health)
            sleuth->restorePartyVita();
          if(restore_qd)
            sleuth->restorePartyQtdr();
        }
      }
    }
    else if(battle_ctrl->getTurnState() == TurnState::STOPPED)
    {
      if(mode_next == NONE)
        changeMode(MAP);
    }
    else if(!event_handler.getKeyHandler().isDepressed(GameKey::PAUSE))
    {
      return battle_ctrl->update(cycle_time);
    }
  }
  /* LOADING MODE */
  else if(mode == LOADING)
  {
  }
  /* If DISABLED - processing complete: exit */
  else if(mode == DISABLED)
  {
    return true;
  }

  return false;
}
