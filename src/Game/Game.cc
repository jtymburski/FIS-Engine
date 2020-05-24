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

const std::string Game::kSAVE_IMG_BACK = ".bmp";
const std::string Game::kSAVE_PATH_AUTO = "_auto";
const std::string Game::kSAVE_PATH_BACK = ".save";
const std::string Game::kSAVE_PATH_FRONT = "saves/slot";
const uint8_t Game::kSAVE_SLOT_DEFAULT = 1;
const uint8_t Game::kSAVE_SLOT_MAX = 30;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Game::Game(Options* running_config)
{
  ticks_total = 0;

  /* Initalize class variables */
  active_renderer = nullptr;
  battle_ctrl = nullptr;
  battle_display_data = nullptr;
  config = nullptr;
  event_disable = false;
  game_directory = "";
  game_path = "";
  loaded_core = false;
  loaded_sub = false;
  map_lvl = -1;
  map_menu_enabled = true;
  mode = DISABLED;
  mode_load = NOLOAD;
  mode_next = NONE;
  player_main = nullptr;
  player_name = "Player";
  player_sex = Sex::FEMALE;
  save_slot = 0;

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
BattleScene* Game::addBattleScene(const int32_t& id)
{
  BattleScene scene = Helpers::createBlankScene();
  scene.id = id;
  scene.music_id = Sound::kID_MUSIC_BATTLE;

  /* Push back and return reference */
  list_battles.push_back(scene);
  return &list_battles[list_battles.size() - 1];
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
    //new_item = new Bubby();
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

/* Add Item to Inventory function */
bool Game::addItemToInv(Inventory* inv, const int32_t& item_id,
                        const int32_t& item_count, const bool& total)
{
  bool success = false;

  if(inv != nullptr && item_count > 0)
  {
    Item* base_item = getItem(item_id);
    if(base_item != nullptr)
    {
      Item* new_item = new Item(base_item);
      success = true;

      /* Attempt to remove the item if the new count is total */
      int curr_count = 0;
      if(total)
        curr_count = inv->getItemCount(item_id);

      /* Add (or remove) depending on the count delta */
      if(item_count > curr_count)
      {
        AddStatus status = inv->add(new_item, item_count - curr_count);
        if(status != AddStatus::GOOD_KEEP)
          delete new_item;
        if(status == AddStatus::FAIL)
          success = false;
      }
      else if(item_count < curr_count)
      {
        success &= inv->removeItemID(item_id, curr_count - item_count);
      }
    }
  }

  return success;
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
bool Game::addPersonToParty(Party* ref_party, const int32_t& base_id,
                            const uint32_t index, const uint32_t& lvl)
{
  if(ref_party != nullptr && index < ref_party->getMaxSize())
  {
    /* Determine if the instance exists */
    Person* ref_person = ref_party->getMember(index);
    if(ref_person == nullptr || ref_person->getGameID() != base_id)
    {
      /* Try and generate the new person */
      Person* new_person = addPersonInst(base_id, lvl);
      if(new_person != nullptr)
      {
        /* Select how to handle based on conditions - no person exists */
        if(ref_person == nullptr)
        {
          /* Fill up to the index point */
          while(index < ref_party->getSize())
          {
            Person* filler_person = new Person();
            list_person_inst.push_back(filler_person);
            ref_party->addMember(filler_person);
          }

          /* Append new person */
          ref_party->addMember(new_person);
          ref_person = new_person;
        }
        /* The person is not the same base reference */
        else
        {
          ref_party->replaceMember(index, new_person);
          if(!removePersonInstance(ref_person))
            delete ref_person;
          ref_person = new_person;
        }

        return true;
      }
    }
    /* Otherwise, the reference person is valid */
    else
    {
      ref_person->loseExp(ref_person->getTotalExp());
      ref_person->addExp(ref_person->getExpAt(lvl), true, true);
      return true;
    }
  }

  return false;
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
bool Game::changeMode(GameMode mode, bool map_change)
{
  // mode is MENU

  /* Run logic to determine if mode switch is allowed - currently always true */
  bool allow = true;

  /* If allowed, make change */
  if(allow && this->mode != mode && mode_next != mode)
  {
    /* Changes to execute on the view closing */
    if((this->mode == MAP || this->mode == MENU) && mode != MENU)
      map_ctrl.enableView(false);

    /* Set the next mode */
    mode_next = mode;
    updateMode(0);

    /*  Log the game mode */
    if(mode == DISABLED)
      event_handler.log("GAME MODE: DISABLED");
    else if(mode == MAP)
      event_handler.log("GAME MODE: MAP");
    else if(mode == BATTLE)
      event_handler.log("GAME MODE: BATTLE");
    else if(mode == LOADING)
      event_handler.log("GAME MODE: LOADING");
    else if(mode == MENU)
      event_handler.log("GAME MODE: MENU");

    /* Changes to execute on the view opening */
    if(mode == MAP)
      map_ctrl.enableView(true, map_change);
  }

  return allow;
}

/* A give item event, based on an ID and count (triggered from stored event */
int Game::eventGiveItem(int id, int count, GiveItemFlags flags, int chance,
                        bool from_pickup)
{
  int insert_count = 0;

  /* Determine if the give item occurs */
  if(from_pickup || Helpers::chanceHappens(chance, 100))
  {
    /* Attempt to find the item in the core reference */
    Item* found_item = getItem(id);
    if(found_item != nullptr && count > 0)
    {
      /* Get the flag */
      bool auto_drop;
      EventSet::dataEnumGiveFlags(flags, auto_drop);

      /* Try and insert into sleuth inventory */
      if(player_main != nullptr && !auto_drop)
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

      /* Notify and drop that item could not be received - depending on
       * properties */
      if(count > insert_count)
      {
        int delta = (count - insert_count);

        /* From pickup - just notify */
        if(from_pickup)
        {
          map_ctrl.initNotification(
              "Insufficient room in inventory to pickup " +
              std::to_string(delta) + " {I" + std::to_string(id) + "}.");
        }
        /* Otherwise: notify and drop remaining items */
        else
        {
          /* Notify if not auto drop and items could not fit */
          if(!auto_drop)
          {
            map_ctrl.initNotification(
                "Insufficient room in inventory to receive " +
                std::to_string(delta) + " {I" + std::to_string(id) +
                "}. Remaining will be dropped");
          }

          /* Drop onto map */
          map_ctrl.dropItem(id, delta);
        }
      }
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
  if(item != nullptr && item->getBase() != nullptr &&
     item->isWalkover() == walkover)
  {
    int insert_count = eventGiveItem(item->getBase()->getID(), item->getCount(),
                                     GiveItemFlags::NONE, 100, true);

    /* If the insert was successful, pickup the item */
    if(insert_count > 0)
      map_ctrl.pickupItem(item, insert_count);
  }
}

/* The property modifier event */
void Game::eventPropMod(MapThing* source, ThingBase type, int id,
                        ThingProperty props, ThingProperty bools, int respawn,
                        int speed, TrackingState track, int inactive)
{
  map_ctrl.modifyThing(source, type, id, props, bools, respawn, speed, track,
                       inactive);
}

/* Show the Menu */
bool Game::eventMenuShow()
{
  if(active_renderer && config)
  {
    if(map_ctrl.getDialogMode() == DialogMode::NOTIFICATION)
      map_ctrl.setDialogPaused(true);

    updatePlayerSteps();
    menuPreparation();
    
    map_menu.setRenderer(active_renderer);
    map_menu.setBattleDisplayData(battle_display_data);
    map_menu.setConfig(config);
    map_menu.setEventHandler(&event_handler);
    map_menu.setMap(&map_ctrl);
    map_menu.setInventory(getInvSleuth());
    map_menu.setPlayer(player_main);
    map_menu.setSaveData(getSaveData());
    map_menu.show();
  }

  return true;
}

/* Hide the Menu */
bool Game::eventMenuHide()
{
  map_menu.hide();
  map_ctrl.setDialogPaused(false);

  return true;
}

/* Trigger IO event, based on the IO object and interaction state */
void Game::eventShiftIO(MapInteractiveObject* io, int interaction_state,
                        MapPerson* initiator, bool check_lock)
{
  if(io != nullptr && io->getInteraction() != MapState::NOINTERACTION &&
     static_cast<int>(io->getInteraction()) == interaction_state)
  {
    /* Handle lock struct */
    if(check_lock)
    {
      Locked lock_struct = io->getLock();
      if(parseLock(lock_struct))
        io->setLock(lock_struct);
    }

    /* Handle trigger */
    io->handlerTrigger(interaction_state, initiator);
  }
}

/* Starts a battle event. Using the given information */
bool Game::eventStartBattle(int person_id, int source_id)
{
  if(person_id >= 0 && source_id >= 0 && mode == BATTLE)
  // MAP && mode_next == NONE)
  {
    if(battle_ctrl)
    {
      battle_ctrl->setRenderer(active_renderer);
      battle_ctrl->setEventHandler(&event_handler);
      battle_display_data->setRenderer(active_renderer);

      /* Access battle scene information and apply */
      BattleScene* scene = nullptr;
      int id = map_ctrl.getBattleScene();
      if(id >= 0)
        scene = getBattleScene(id);
      if(scene != nullptr)
      {
        battle_ctrl->setBackground(scene->background);
        battle_ctrl->setMusicID(scene->music_id);

        /* Underlays */
        for(const auto& lay : scene->underlays)
        {
          Floatinate velocity(lay.velocity_x, lay.velocity_y);

          battle_ctrl->createLay(game_directory + lay.path, lay.anim_time, velocity,
                                 LayType::UNDERLAY);
        }

        /* Midlays */
        for(const auto& lay : scene->midlays)
        {
          Floatinate velocity(lay.velocity_x, lay.velocity_y);

          battle_ctrl->createLay(game_directory + lay.path, lay.anim_time, velocity,
                                 LayType::MIDLAY);
        }

        /* Overlays */
        for(const auto& lay : scene->overlays)
        {
          Floatinate velocity(lay.velocity_x, lay.velocity_y);

          battle_ctrl->createLay(game_directory + lay.path, lay.anim_time, velocity,
                                 LayType::OVERLAY);
        }
      }

      /* Build display data */
      if(!battle_display_data->isDataBuilt())
      {
        battle_display_data->buildData(game_directory);
        battle_display_data->buildItemMap(list_item);
      }

      /* Start battle */
      battle_ctrl->startBattle(getParty(person_id), getParty(source_id));

      // changeMode(BATTLE);
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
    load(active_renderer, false);
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

/* Trigger IO event, based on ID */
void Game::eventTriggerIO(MapThing* source, int io_id, MapPerson* initiator)
{
  /* If the reference ID is less than 0, use source */
  if(io_id < 0)
  {
    if(source != nullptr && source->classDescriptor() == ThingBase::INTERACTIVE)
      eventShiftIO(static_cast<MapInteractiveObject*>(source),
                   static_cast<int>(MapState::TRIGGER), initiator);

  }
  /* Otherwise, find the ID and begin processing */
  else
  {
    MapInteractiveObject* found_io = map_ctrl.getIO(io_id);
    if(found_io != nullptr)
      eventShiftIO(found_io, static_cast<int>(MapState::TRIGGER), initiator);
  }
}

/* Unlock events, based on parameter information */
void Game::eventUnlockIO(MapThing* source, int io_id, UnlockIOMode mode,
                         int state_num, UnlockIOEvent mode_events,
                         UnlockView mode_view, int view_time)
{
  map_ctrl.unlockIO(source, io_id, mode, state_num, mode_events, mode_view,
                    view_time);
}

/* Unlock events, based on parameter information */
void Game::eventUnlockThing(MapThing* source, int thing_id,
                            UnlockView mode_view, int view_time)
{
  map_ctrl.unlockThing(source, thing_id, mode_view, view_time);
}

/* Unlock events, based on parameter information */
void Game::eventUnlockTile(int section_id, int tile_x, int tile_y,
                           UnlockTileMode mode, UnlockView mode_view,
                           int view_time)
{
  map_ctrl.unlockTile(section_id, tile_x, tile_y, mode, mode_view, view_time);
}

/* Returns the core struct item data for correlation purposes */
std::vector<ItemData> Game::getItemData()
{
  std::vector<ItemData> data_set;

  /* Loop through all core data and add to vector */
  for(uint32_t i = 0; i < list_item.size(); i++)
    data_set.push_back(list_item[i]->getItemData());

  return data_set;
}

/* Load game - main function call */
bool Game::load(std::string base_file, SDL_Renderer* renderer, uint8_t slot,
                bool encryption, bool full_load)
{
  bool success = true;

  /* Update the player step count */
  updatePlayerSteps();

  /* Ensure nothing is loaded - if full load is false, just unloads map */
  unload(full_load);

  /* Loading trigger */
  // changeMode(LOADING); // TODO: Keep or Remove

  /* Initial set-up */
  if(full_load)
    player_main = new Player();

  /* Create the base file handler */
  FileHandler fh_base(base_file, false, true, encryption);
  success &= fh_base.start();

  /* Create the save slot file handler, if applicable */
  FileHandler fh_slot(getSlotPath(slot, config->getBasePath()), false, true, encryption);
  bool slot_valid = (slot > 0);
  if(slot_valid)
    slot_valid &= fh_slot.start();

  /* Timer to calculate the game load time */
  Timer t;

  // std::cout << "1: " << success << std::endl;

  /* Core data first, if applicable */
  if(success && full_load)
  {
    /* Base file */
    success &= loadData(&fh_base, renderer, true, false);

    // std::cout << "2: " << success << std::endl;

    /* Player set-up */
    player_main->setSleuth(getParty(Party::kID_SLEUTH));
    player_main->setBearacks(getParty(Party::kID_BEARACKS));

    // std::cout << "3: " << success << std::endl;

    /* Slot file */
    if(slot_valid)
      success &= loadData(&fh_slot, renderer, true, true);
  }

  // std::cout << "4: " << success << std::endl;

  /* Map data to follow */
  if(success)
  {
    std::string level = std::to_string(map_lvl);

    /* Core item to map correlation */
    map_ctrl.setBaseItems(getItemData(), renderer);

    /* Base file */
    fh_base.xmlToHead();
    success &= loadData(&fh_base, renderer, false, false, level);

    // std::cout << "5: " << success << std::endl;

    /* Slot file */
    if(slot_valid)
    {
      fh_slot.xmlToHead();
      success &= loadData(&fh_slot, renderer, false, true, level);
    }
  }

  // std::cout << "6: " << success << std::endl;

  /* Log the game load time */
  event_handler.log("Game load time: " + std::to_string(t.elapsed()) + " s");

  /* Stop the handler */
  success &= fh_base.stop();
  if(slot_valid)
    success &= fh_slot.stop();

  /* If the load was successful, proceed to clean-up */
  if(success)
  {
    /* Clean up map */
    map_ctrl.loadDataFinish(renderer);
    map_ctrl.disableInteraction(event_disable);

    if(player_main)
    {
      player_main->setName(player_name);
      player_main->setSex(player_sex);
    }
    changeMode(MAP, true);
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

/* Load game data */
bool Game::loadData(FileHandler* fh, SDL_Renderer* renderer, bool core_data,
                    bool save_data, std::string level)
{
  XmlData data;
  bool done = false;
  int index = 0;
  bool read_success = true;
  bool success = true;

  do
  {
    /* Read set of XML data */
    data = fh->readXmlData(&done, &read_success);
    success &= read_success;

    /* Only proceed if inside game */
    if(data.getElement(index) == "game")
    {
      /* If core data load */
      if(core_data)
      {
        /* General game data */
        if(data.getElement(index + 1) == "core")
        {
          success &= loadData(data, index + 2, renderer, save_data);
        }
        /* Current Map Index */
        else if(data.getElement(index + 1) == "currentmap" && save_data)
        {
          int new_level = data.getDataInteger(&success);
          if(success && new_level >= 0)
            map_lvl = new_level;
        }
      }
      /* If map data load */
      else
      {
        /* Map data */
        if(data.getElement(index + 1) == "map" &&
           data.getKeyValue(index + 1) == level)
        {
          success &= map_ctrl.loadData(data, index + 2, renderer, game_directory,
                                       save_data);
        }
      }
    }
  } while(!done); // && success); /* Success in loop?? */

  return success;
}

/* Load game specific data */
// TODO: Comment
bool Game::loadData(XmlData data, int index, SDL_Renderer* renderer,
                    bool from_save)
{
  (void)from_save;//TODO

  bool success = true;
  std::string element = data.getElement(index);

  /* ID index */
  int id = -1;
  std::string id_str = data.getKeyValue(index);
  if(!id_str.empty())
    id = std::stoi(id_str);

  /* ---- ACTIONS ---- */
  if(element == "action")
  {
    addAction(data.getDataString());
  }
  /* ---- BATTLE SCENES ---- */
  else if(element == "battlescene")
  {
    /* Get edit scene */
    BattleScene* edit_scene = getBattleScene(id);
    if(edit_scene == nullptr)
      edit_scene = addBattleScene(id);

    /* Modify */
    *edit_scene = Helpers::updateScene(*edit_scene, data, index + 1);
  }
  /* ---- CLASSES ---- */
  else if(element == "class")
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
  else if(element == "item")
  {
    Item* edit_item = getItem(id);
    if(edit_item == nullptr)
      edit_item = addItem(id);

    /* Data for skill */
    if(data.getElement(index + 1) == "skill")
      edit_item->setUseSkill(getSkill(data.getDataInteger(&success)));
    else
      success &= edit_item->loadData(data, index + 1, renderer, game_directory);
  }
  /* ---- PARTIES ---- */
  else if(element == "party")
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

          success &= addPersonToParty(edit_party, person_id,
                                      edit_party->getSize(), person_lvl);
        }
      }
    }
    else if(data.getElement(index + 1) == "inventory" &&
            edit_party->getInventory() != nullptr)
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
            success &= addItemToInv(edit_party->getInventory(), item_id,
                                    item_count, true);
          }
        }
      }
      /* The Rest */
      else
      {
        edit_party->getInventory()->loadData(data, index + 2, renderer,
                                             game_directory);
      }
    }
    else
    {
      success &= edit_party->loadData(data, index + 1, renderer, game_directory);
    }
  }
  /* ---- PERSONS ---- */
  else if(element == "person")
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
      success &= edit_person->loadData(data, index + 1, renderer, game_directory);
  }
  /* ---- PLAYER ---- */
  else if(element == "player")
  {
    // TODO: Bubbies - FUTURE
    /* The base indicator for the person */
    if(data.getElement(index + 3) == "base" &&
       data.getElement(index + 2) == "person" &&
       (data.getElement(index + 1) == "sleuth" ||
        data.getElement(index + 1) == "bearacks"))
    {
      int base_id = data.getDataInteger();
      std::string index_str = data.getKeyValue(index + 2);
      if(base_id >= 0 && !index_str.empty())
      {
        int party_index = std::stoi(index_str);
        if(index >= 0)
        {
          if(data.getElement(index + 1) == "sleuth")
            success &= addPersonToParty(player_main->getSleuth(), base_id,
                                        static_cast<uint32_t>(party_index));
          else
            success &= addPersonToParty(player_main->getBearacks(), base_id,
                                        static_cast<uint32_t>(party_index));
        }
      }
    }
    /* Inventory: Items */
    else if(data.getElement(index + 3) == "item" &&
            data.getElement(index + 2) == "inventory" &&
            (data.getElement(index + 1) == "sleuth" ||
             data.getElement(index + 1) == "bearacks"))
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

          if(data.getElement(index + 1) == "sleuth")
            success &= addItemToInv(player_main->getSleuth()->getInventory(),
                                    item_id, item_count, true);
          else
            success &= addItemToInv(player_main->getBearacks()->getInventory(),
                                    item_id, item_count, true);
        }
      }
    }
    /* The learned skills */
    else if(data.getElement(index + 3) == "learned" &&
            data.getElement(index + 2) == "person" &&
            (data.getElement(index + 1) == "sleuth" ||
             data.getElement(index + 1) == "bearacks"))
    {
      std::string index_str = data.getKeyValue(index + 2);
      std::string learn_str = data.getDataString();
      if(!index_str.empty() && !learn_str.empty())
      {
        /* Process the values */
        int index = std::stoi(index_str);
        std::vector<std::string> learn_split = Helpers::split(learn_str, ',');
        if(index >= 0 && learn_split.size() == 2)
        {
          int first = std::stoi(learn_split.front());
          int second = std::stoi(learn_split.back());
          success &= player_main->addLearnedSkill(data.getElement(index + 1),
                                                  getSkill(first), second);
        }
      }
    }
    /* All other cases */
    else
    {
      player_main->loadData(data, index + 1, renderer, game_directory);
    }
  }
  else if(element == "options")
  {
    std::cout << "Trying to load options data" << std::endl;
    if(config)
    {
      std::cout << "Loading options data" << std::endl;
      success &= config->loadData(data, index + 1);
    }
  }
  /* ---- RACES ---- */
  else if(element == "race")
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
  else if(element == "skill")
  {
    Skill* edit_skill = getSkill(id);
    if(edit_skill == nullptr)
      edit_skill = addSkill(id);

    /* Data for skill */
    if(data.getElement(index + 1) == "action")
      edit_skill->addAction(getAction(data.getDataInteger(&success)), false);
    else
      success &= edit_skill->loadData(data, index + 1, renderer, game_directory);

    /* Flag setup after changes */
    edit_skill->flagSetup();
  }
  /* ---- SKILLSETS ---- */
  else if(element == "skillset")
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

  // std::cout << "Core loadData: " << element << ","
  //          << data.getElement(index + 1) << "." << success << std::endl;
  return success;
}

/* Menu preparation functionality */
void Game::menuPreparation()
{
  /* Save screenshot */
  saveScreenshot(getSlotPath(0, config->getBasePath(), true, true),
                 map_ctrl.getSnapshotRect(), active_renderer);
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

        /* Attempt unlock - has items */
        if(count_avail >= count)
        {
          if(EventSet::unlockItem(lock_struct, id, count_avail))
            eventTakeItem(id, count);
          return true;
        }
        /* Does not have items - notify */
        else
        {
          map_ctrl.initNotification("Locked. Requires " +
                                    std::to_string(count) + " {I" +
                                    std::to_string(id) +"} to unlock.");
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
  if(!event_disable && event_handler.pollEventAvailable())
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
      /* -- BATTLE START -- */
      if(classification == EventClassifier::BATTLESTART)
      {
        /* Get the reference objects and check if valid */
        EventPair event_win, event_lose;
        BattleFlags flags;
        MapPerson* person;
        MapThing* source;
        if(event_handler.pollStartBattle(&person, &source, flags, event_win,
                                         event_lose))
        {
          if(person != nullptr && getParty(Party::kID_SLEUTH) != nullptr &&
             source != nullptr && getParty(source->getGameID()) != nullptr)
          {
            map_ctrl.initBattle(person, source, flags, event_win, event_lose);
          }
        }
      }
      /* -- CONVERSATION -- */
      else if(classification == EventClassifier::CONVERSATION)
      {
        ConvoPair convo_pair;
        MapThing* source;
        if(event_handler.pollConversation(convo_pair, &source))
          eventInitConversation(convo_pair, source);
      }
      /* -- ITEM GIVE -- */
      else if(classification == EventClassifier::ITEMGIVE)
      {
        int id;
        int chance;
        int count;
        GiveItemFlags flags;
        if(event_handler.pollGiveItem(id, count, flags, chance))
          eventGiveItem(id, count, flags, chance);
      }
      /* -- ITEM PICKUP -- */
      else if(classification == EventClassifier::ITEMPICKUP)
      {
        MapItem* item;
        bool walkover;
        if(event_handler.pollPickupItem(&item, &walkover))
          eventPickupItem(item, walkover);
      }
      /* -- ITEM TAKE -- */
      else if(classification == EventClassifier::ITEMTAKE)
      {
        int id;
        int count;
        if(event_handler.pollTakeItem(&id, &count))
          eventTakeItem(id, count);
      }
      /* -- MAP SWITCH -- */
      else if(classification == EventClassifier::MAPSWITCH)
      {
        int id;
        if(event_handler.pollStartMap(&id))
          eventSwitchMap(id);
      }
      /* -- MULTIPLE -- */
      else if(classification == EventClassifier::MULTIPLE)
      {
        MapThing* source;
        MapPerson* person;
        std::vector<EventPair> events;
        if(event_handler.pollMultiple(person, source, events))
        {
          for(uint16_t i = 0; i < events.size(); i++)
          {
            event_handler.executeEventRef(events[i].base, events[i].inst,
                                          person, source);
          }
        }
      }
      /* -- NOTIFICATION -- */
      else if(classification == EventClassifier::NOTIFICATION)
      {
        std::string notification;
        if(event_handler.pollNotification(&notification))
          eventInitNotification(notification);
      }
      /* -- PROPERTY MODIFIER -- */
      else if(classification == EventClassifier::PROPERTY)
      {
        int id, inactive, respawn, speed;
        MapThing* source;
        ThingBase type;
        ThingProperty bools, props;
        TrackingState track;
        if(event_handler.pollPropMod(source, type, id, props, bools, respawn,
                                     speed, track, inactive))
          eventPropMod(source, type, id, props, bools, respawn, speed, track,
                       inactive);
      }
      /* -- SHIFT IO -- */
      else if(classification == EventClassifier::SHIFTIO)
      {
        MapPerson* initiator;
        int interaction_state;
        MapInteractiveObject* source;
        if(event_handler.pollShiftIO(&source, &interaction_state, &initiator))
          eventShiftIO(source, interaction_state, initiator);
      }
      /* -- SOUND ONLY -- */
      else if(classification == EventClassifier::SOUNDONLY)
      {
        event_handler.pollSound();
      }
      /* -- TELEPORT THING -- */
      else if(classification == EventClassifier::TELEPORTTHING)
      {
        int thing_id, x, y, section_id;
        if(event_handler.pollTeleportThing(&thing_id, &x, &y, &section_id))
          eventTeleportThing(thing_id, x, y, section_id);
      }
      /* -- TRIGGER IO -- */
      else if(classification == EventClassifier::TRIGGERIO)
      {
        int io_id;
        MapPerson* initiator;
        MapThing* source;
        if(event_handler.pollTriggerIO(source, io_id, initiator))
          eventTriggerIO(source, io_id, initiator);
      }
      /* -- UNLOCK IO -- */
      else if(classification == EventClassifier::UNLOCKIO)
      {
        int io_id, state_num, view_time;
        MapThing* source;
        UnlockIOMode mode;
        UnlockIOEvent mode_events;
        UnlockView mode_view;
        if(event_handler.pollUnlockIO(source, &io_id, &mode, &state_num,
                                      &mode_events, &mode_view, &view_time))
        {
          eventUnlockIO(source, io_id, mode, state_num, mode_events, mode_view,
                        view_time);
        }
      }
      /* -- UNLOCK THING -- */
      else if(classification == EventClassifier::UNLOCKTHING)
      {
        int thing_id, view_time;
        MapThing* source;
        UnlockView mode_view;
        if(event_handler.pollUnlockThing(source, &thing_id, &mode_view,
                                         &view_time))
        {
          eventUnlockThing(source, thing_id, mode_view, view_time);
        }
      }
      /* -- UNLOCK TILE -- */
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

  /* Delete unconnected lists */
  removeBattleScenes();

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
void Game::removeBattleScenes()
{
  list_battles.clear();
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
bool Game::removePersonInstance(Person* const person_inst)
{
  bool found = false;

  /* Find and delete if found */
  for(auto it = begin(list_person_inst); !found && it != end(list_person_inst);
      ++it)
  {
    if(*it == person_inst)
    {
      delete(*it);
      list_person_inst.erase(it);
      found = true;
    }
  }

  return found;
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

bool Game::setSaveSlot(uint8_t save_slot)
{
  if(save_slot <= kSAVE_SLOT_MAX)
  {
    this->save_slot = save_slot;

    return true;
  }

  return false;
}
/* Update mode */
void Game::updateMode(int cycle_time)
{
  (void)cycle_time;
  if(mode_next != NONE)
  {
    if(mode == MAP && mode_next != MENU)
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
      if(mode_next == MENU)
      {
        if(map_ctrl.getPerson(0))
          map_ctrl.getPerson(0)->keyFlush();

        eventMenuShow();
      }

      mode = mode_next;
      mode_next = NONE;
    }
  }
}

/* Updates the menu enabled state as per the appropriate flag states */
void Game::updateMenuEnabledState()
{
  map_menu_enabled = true;

  /* If the higher game state allows the menu to be accessed, first check for
   * other conditions to assert the menu can be accessed at this time */
  if(map_ctrl.getDialogMode() == DialogMode::CONVERSATION)
    map_menu_enabled = false;
  else if(map_ctrl.getFadeStatus() != MapFade::VISIBLE)
    map_menu_enabled = false;
  else if(battle_ctrl && battle_ctrl->getTurnState() != TurnState::STOPPED)
    map_menu_enabled = false;
}

/* Updates the state of the menu saving */
void Game::updateMenuSaving()
{
  /* Update the state of the menu saving */
  auto save_index = map_menu.getSaveIndex();
  auto save_state = map_menu.getMenuSaveState();

  if(save_state == MenuSaveState::WRITE)
  {
    save(save_index + 1, true);
    map_menu.setSaveData(getSaveData());
    map_menu.updateSaveTitles();
  }
  else if(save_state == MenuSaveState::CLEAR)
  {
    saveClear(save_index + 1);
    map_menu.setSaveData(getSaveData());
    map_menu.updateSaveTitles();
  }

  map_menu.setMenuSaveState(MenuSaveState::NONE);
}

/* Updates the player steps with the map information */
void Game::updatePlayerSteps()
{
  /* Load in steps assuming valid states */
  if(player_main != nullptr && loaded_core && loaded_sub)
  {
    player_main->addSteps(map_ctrl.getPlayerSteps());
    map_ctrl.resetPlayerSteps();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Enable view trigger */
void Game::enableView(bool enable)
{
  std::cout << "Enable view: " << enable << std::endl;
  if(enable)
    changeMode(LOADING);
  else
    changeMode(DISABLED);

  // if(enable && isLoadedCore() && isLoadedSub())
  //  changeMode(MAP);
  // else
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

/* Returns a pointer to a battle scene by index or by ID */
BattleScene* Game::getBattleScene(const int32_t& index, const bool& by_id)
{
  if(by_id)
  {
    for(uint32_t i = 0; i < list_battles.size(); i++)
      if(list_battles[i].id == index)
        return &list_battles[i];
  }
  else if(static_cast<uint32_t>(index) < list_battles.size())
  {
    return &list_battles[index];
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

/* Returns the event handler */
EventHandler& Game::getHandler()
{
  return event_handler;
}

/* Get the game mode */
Game::GameMode Game::getMode()
{
  return mode;
}

/* Gets save data - used for rendering and information */
std::vector<Save> Game::getSaveData(bool encryption)
{
  std::vector<Save> save_set;

  /* Go through all slots up to range and find data */
  for(uint8_t i = 1; i <= kSAVE_SLOT_MAX; i++)
  {
    std::string path = getSlotPath(i, config->getBasePath());
    std::string path_img = getSlotPath(i, config->getBasePath(), true);
    Save slot(i, config);

    /* Attempt to open the path with the file handling system */
    FileHandler fh_slot(path, false, true, encryption);
    if(FileHandler::fileExists(path) && fh_slot.start())
    {
      /* The snapshot path */
      slot.setSnapshotPath(path_img);

      /* The save date and time */
      std::string save_date = fh_slot.getDate();
      std::vector<std::string> split_date_time = Helpers::split(save_date, ' ');
      if(split_date_time.size() == 2)
      {
        /* Year, Month, Day */
        std::vector<std::string> split_date =
            Helpers::split(split_date_time.front(), '/');
        /* Hour, Minute, Second */
        std::vector<std::string> split_time =
            Helpers::split(split_date_time.back(), ':');
        if(split_date.size() == 3 && split_time.size() == 3)
        {
          /* Try and complete the conversion to int and set in the slot */
          try
          {
            int year = std::stoi(split_date[0]);
            int month = std::stoi(split_date[1]);
            int day = std::stoi(split_date[2]);
            int hour = std::stoi(split_time[0]);
            int minute = std::stoi(split_time[1]);
            // int second = std::stoi(split_time[2]);
            slot.setDate(year, month, day, hour, minute);
          }
          catch(std::exception& e)
          {
          }
        }
      }

      /* Parse the file for remaining information */
      XmlData data;
      bool done = false;
      int index = 0;
      bool read_success = true;
      do
      {
        /* Read set of XML data */
        data = fh_slot.readXmlData(&done, &read_success);
        if(read_success)
        {
          /* Only proceed if inside game */
          if(data.getElement(index) == "game")
          {
            /* Core data */
            if(data.getElement(index + 1) == "core" &&
               data.getElement(index + 2) == "player")
            {
              /* Credits */
              if(data.getElement(index + 3) == "credits")
              {
                int credits = data.getDataInteger(&read_success);
                if(read_success)
                  slot.setCountCredits(credits);
              }
              else if(data.getElement(index + 3) == "name")
              {
                std::string name = data.getDataString(&read_success);

                if(read_success)
                  slot.setCustomPlayerName(name);
              }
              /* Play time hours, minutes, seconds */
              else if(data.getElement(index + 3) == "playtime")
              {
                int hours = slot.getTimeHours();
                int minutes = slot.getTimeMinutes();
                int seconds = slot.getTimeSeconds();

                /* Read the time */
                int new_time = data.getDataInteger(&read_success);

                if(read_success)
                {
                  /* Determine the time allocation */
                  if(data.getElement(index + 4) == "hours")
                    hours = new_time;
                  else if(data.getElement(index + 4) == "minutes")
                    minutes = new_time;
                  else if(data.getElement(index + 4) == "milliseconds")
                    seconds = (new_time / 1000);

                  /* Set to slot */
                  slot.setTime(hours, minutes, seconds);
                }
              }
              else if(data.getElement(index + 3) == "sex")
              {
                std::string sex = data.getDataString(&read_success);

                if(read_success)
                  slot.setCustomPlayerSex(Helpers::sexFromStr(sex));
              }
              /* Sleuth information */
              else if(data.getElement(index + 3) == "sleuth")
              {
                if(data.getElement(index + 4) == "person" &&
                   data.getKeyValue(index + 4) == "0")
                {
                  if(data.getElement(index + 5) == "level")
                  {
                    int level = data.getDataInteger(&read_success);
                    if(read_success && level >= 0)
                      slot.setCountLevel(level);
                  }
                }
              }
              /* Steps */
              else if(data.getElement(index + 3) == "steps")
              {
                int steps = data.getDataInteger(&read_success);
                if(read_success)
                  slot.setCountSteps(steps);
              }
            }
            /* Current map name */
            else if(data.getElement(index + 1) == "currentmapname")
            {
              std::string map_name = data.getDataString(&read_success);
              if(read_success)
                slot.setMapName(map_name);
            }
          }
        }
      } while(!done);
    }

    save_set.push_back(slot);
  }

  return save_set;
}

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
bool Game::keyDownEvent(KeyHandler& key_handler)
{
  if(key_handler.isDepressed(GameKey::CANCEL) && map_menu_enabled &&
     (map_menu.getMenuLayer() == MenuLayer::TITLE ||
      !map_menu.getFlag(MenuState::SHOWING)))
  {
    if(mode == MAP && !map_menu.getFlag(MenuState::SHOWING))
    {
      changeMode(MENU);
      key_handler.setHeld(GameKey::CANCEL);
    }
    else if(mode == MENU && !map_menu.isMainSliding())
    {
      eventMenuHide();
      key_handler.setHeld(GameKey::MOVE_UP);
    }
  }
  /* Otherwise, send keys to the active view */
  else
  {
    if(mode == MENU)
    {
      map_menu.keyDownEvent(key_handler);
    }
    /* -- MAP MODE -- */
    else if(mode == MAP)
    {
      if(map_ctrl.keyDownEvent(key_handler))
        changeMode(DISABLED);
    }
    /* -- BATTLE MODE -- */
    else if(mode == BATTLE)
    {
      if(battle_ctrl->keyDownEvent(key_handler))
        changeMode(DISABLED);
    }
  }

  return false;
}

/* Key down event for test keys - isolated from key handler system */
#ifdef UDEBUG
void Game::keyTestDownEvent(SDL_KeyboardEvent event)
{
  /* Disable events Key */
  if(event.keysym.sym == SDLK_F1)
  {
    event_disable = !event_disable;
    map_ctrl.disableInteraction(event_disable);
  }
  /* ---- F2 USED BY MAP : PLAYER IGNORE PASSABILITY ---- */
  /* Print sleuth inventory to debug console */
  else if(event.keysym.sym == SDLK_F3)
  {
    if(player_main != nullptr && player_main->getSleuth() != nullptr &&
       player_main->getSleuth()->getInventory() != nullptr)
    {
      player_main->getSleuth()->getInventory()->print(false);
    }
  }
  /* Save test to slot 1 */
  else if(event.keysym.sym == SDLK_F4)
  {
    // menuPreparation();
    // save(1, true);
  }
  /* Save test to slot 3 */
  else if(event.keysym.sym == SDLK_F5)
  {
    // save(2);
    // std::cout << saveClear(3) << std::endl;
    // std::vector<Save> save_data = getSaveData();
    // for(uint32_t i = 0; i < save_data.size(); i++)
    //  save_data[i].print();
  }
  /* Load test from slot 1 */
  else if(event.keysym.sym == SDLK_F6)
  {
    save_slot = 1;
    changeMode(LOADING);
    mode_load = FULLLOAD;
  }
  /* Level Up Key */
  else if(event.keysym.sym == SDLK_F7 && mode != BATTLE)
  {
    if(player_main && player_main->getSleuth())
    {
      auto player_person = player_main->getSleuth()->getMember(0);

      if(player_person && player_person->getLevel() > 1)
      {
        auto new_level = player_person->getLevel() - 1;
        player_person->loseExp(player_person->getTotalExp());
        player_person->addExp(player_person->getExpAt(new_level));
        event_handler.log("[DEBUG] Setting player level to: " +
                          std::to_string(player_person->getLevel()));
      }
    }
  }
  /* Level Up Key */
  else if(event.keysym.sym == SDLK_F8 && mode != BATTLE)
  {
    if(player_main && player_main->getSleuth())
    {
      auto player_person = player_main->getSleuth()->getMember(0);

      if(player_person &&
         (size_t)(player_person->getLevel() + 1) <= Person::kNUM_LEVELS)
      {
        auto new_level = player_person->getLevel() + 1;
        player_person->loseExp(player_person->getTotalExp());
        player_person->addExp(player_person->getExpAt(new_level));
        event_handler.log("[DEBUG] Setting player level to: " +
                          std::to_string(player_person->getLevel()));
      }
    }
  }
  /* Give Rock Event */
  else if(event.keysym.sym == SDLK_LEFTBRACKET && mode != BATTLE)
  {
    if(getItem(2, true) && player_main && player_main->getSleuth())
    {
      auto inventory = player_main->getSleuth()->getInventory();

      if(inventory)
      {
        auto new_item = new Item(getItem(2, true));
        if(inventory->add(new_item, 1) != AddStatus::FAIL)
        {
          event_handler.log("[DEBUG] Giving player 1x Item ID 2");
        }
        else
        {
          delete new_item;
          new_item = nullptr;

          event_handler.log("[DEBUG] Player Inventory Full");
        }
      }
    }
  }
  /* Given Rotten Fish Event */
  else if(event.keysym.sym == SDLK_RIGHTBRACKET && mode != BATTLE)
  {
    if(getItem(2, true) && player_main && player_main->getSleuth())
    {
      auto inventory = player_main->getSleuth()->getInventory();

      if(inventory)
      {
        auto new_item = new Item(getItem(3, true));
        if(inventory->add(new_item, 1) != AddStatus::FAIL)
        {
          event_handler.log("[DEBUG] Giving Player x1 Medkit.");
        }
        else
        {
          delete new_item;
          new_item = nullptr;

          event_handler.log("[DEBUG] Player Inventory Full");
        }
      }
    }
  }
  /* Otherwise, send keys to the active view */
  else
  {
    if(mode == MENU)
    {
      // map_menu.keyDownEvent(key_handler);
    }
    /* -- MAP MODE -- */
    else if(mode == MAP)
    {
      map_ctrl.keyTestDownEvent(event);
    }
    /* -- BATTLE MODE -- */
    else if(mode == BATTLE)
    {
      // battle_ctrl->keyDownEvent(key_handler);
    }
  }
}
#endif

/* The key up events to be handled by the class */
void Game::keyUpEvent(KeyHandler& key_handler)
{
  if(mode == MAP)
  {
    map_ctrl.keyUpEvent(key_handler);
  }
}

/* Load game */
bool Game::load(SDL_Renderer* renderer, bool full_load, uint8_t slot)
{
  return load(game_path, renderer, slot, false, full_load);
}

/* Renders the title screen */
bool Game::render(SDL_Renderer* renderer)
{
  bool success{true};

  /* -- LOADING MODE -- */
  if(mode == LOADING)
  {
    if(!isLoadedCore() || mode_load == FULLLOAD)
      load(renderer, true, save_slot);
    else
      load(renderer, false, save_slot);
    mode_load = NOLOAD;
  }

  /* -- MAP MODE -- */
  if(mode == MAP)
  {
    success = map_ctrl.render(renderer);
  }
  /* -- BATTLE MODE -- */
  else if(mode == BATTLE)
  {
    /* Assign the rendererer to Battle and data container class */
    battle_display_data->setRenderer(renderer);

    /* Build the data if it isn't built */
    if(renderer && !battle_display_data->isDataBuilt())
      battle_display_data->buildData(game_directory);
    else if(battle_display_data->isDataBuilt())
    {
      battle_ctrl->setRenderer(renderer);

      /* Render the battle */
      success = battle_ctrl->render();
    }
  }
  else if(mode == MENU)
  {
    map_ctrl.render(renderer);
    map_menu.setRenderer(renderer);
    map_menu.render();
  }

  return success;
}

/* Save game based on the current slot number */
bool Game::save(uint8_t slot, bool from_menu)
{
  if(slot <= kSAVE_SLOT_MAX)
  {
    bool success = true;

    /* Get the proper save slot number and file name */
    if(slot == 0)
    {
      /* If the passed slot is out of range, try and use the stored slot */
      slot = save_slot;

      /* If the stored slot is out of range, use the default slot */
      if(slot == 0)
        slot = kSAVE_SLOT_DEFAULT;
    }
    std::string save_path = getSlotPath(slot, config->getBasePath());
    std::string save_path_img = getSlotPath(slot, config->getBasePath(), true);

    /* If the slot is different, the old data needs to be copied */
    if(slot != save_slot && save_slot > 0)
    {
      std::string old_path = getSlotPath(save_slot, config->getBasePath());
      if(FileHandler::fileExists(old_path))
        success &= FileHandler::fileCopy(old_path, save_path, true);
    }

    /* Start file write */
    if(save_handle.isAvailable() && save_handle.getFilename() != save_path)
      save_handle.stop(true);
    if(!save_handle.isAvailable())
    {
      save_handle.setEncryptionEnabled(false);
      save_handle.setFilename(save_path);
      save_handle.setWriteEnabled(true);
      save_handle.setFileType(FileHandler::XML);
      success &= save_handle.start(true);
    }

    /* If handle is ready to go, proceed */
    if(save_handle.isAvailable() && success)
    {
      /* If from menu, use auto save image */
      if(from_menu)
      {
        std::string save_auto_img = getSlotPath(0, config->getBasePath(), true, true);
        if(FileHandler::fileExists(save_auto_img))
          FileHandler::fileCopy(save_auto_img, save_path_img, true);
      }
      /* Otherwise, generate it on the fly */
      else
      {
        saveScreenshot(save_path_img, map_ctrl.getSnapshotRect(),
                       active_renderer);
      }

      /* Setup the core data */
      XmlData data_core;
      data_core.addElement("game");
      data_core.addElement("core");
      save_handle.purgeElement(data_core, true);

      /* Write the core data */
      if(player_main != nullptr)
      {
        /* Write player data which contains all other data related */
        updatePlayerSteps();
        success &= player_main->saveData(&save_handle);
      }

      if(config)
      {
        success &= config->saveData(&save_handle);
      }

      if(map_ctrl.isLoaded())
      {
        /* The current map index information */
        XmlData data_map_curr(map_lvl);
        data_map_curr.addElement("game");
        data_map_curr.addElement("currentmap");
        save_handle.writeXmlDataSet(data_map_curr);

        /* The current map name information */
        data_map_curr.removeLastElement();
        data_map_curr.addElement("currentmapname");
        data_map_curr.addDataOfType(map_ctrl.getName());
        save_handle.writeXmlDataSet(data_map_curr);

        /* Setup the map data */
        XmlData data_map;
        data_map.addElement("game");
        data_map.addElement("map", "id", std::to_string(map_lvl));
        save_handle.purgeElement(data_map, true);

        /* Write the map data */
        success &= map_ctrl.saveData(&save_handle);
      }

      /* Finish the file write */
      save_handle.stop(!success);
    }

    /* If success, save slot */
    if(success)
      save_slot = slot;

    return success;
  }
  return false;
}

/* Clears the passed in save slot number */
bool Game::saveClear(uint8_t slot)
{
  /* Ensure the slot is in range */
  if(slot <= kSAVE_SLOT_MAX)
  {
    /* Find the path and if it exists */
    std::string delete_path = getSlotPath(slot, config->getBasePath());
    if(FileHandler::fileExists(delete_path))
    {
      /* Delete the path */
      bool success = FileHandler::fileDelete(delete_path);

      /* Delete the image path, if it exists */
      std::string save_path = getSlotPath(slot, config->getBasePath(), true);
      if(FileHandler::fileExists(save_path))
        success &= FileHandler::fileDelete(save_path);

      return success;
    }
  }
  return false;
}

/* Set the running configuration, from the options class */
bool Game::setConfiguration(Options* running_config)
{
  if(running_config)
  {
    config = running_config;

    /* Set in secondary classes */
    map_ctrl.setConfiguration(running_config);
    map_menu.setConfig(running_config);

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
bool Game::setPath(std::string path, std::string directory, int level, bool load)
{
  if(!path.empty() && level >= 0)
  {
    /* Only execute change if it's different */
    if(path != game_path || map_lvl != level)
    {
      bool full_load = (path != game_path);

      /* Update info */
      if(full_load)
        save_slot = 0;
      game_path = path;
      game_directory = directory;
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

void Game::setPlayerName(std::string player_name)
{
  this->player_name = player_name;
}

void Game::setPlayerSex(Sex player_sex)
{
  this->player_sex = player_sex;
}


/* Sets the active renderer to be used */
void Game::setRenderer(SDL_Renderer* renderer)
{
  if(renderer != nullptr)
  {
    active_renderer = renderer;

    /* Assign the renderer to Battle Display Data if able */
    if(battle_display_data)
    {
      battle_display_data->setRenderer(renderer);
    }
  }
}

/* Sets the sound handler used. If unset, no sounds will play */
void Game::setSoundHandler(SoundHandler* new_handler)
{
  event_handler.setSoundHandler(new_handler);
}

/* Unload the game - full unload = false will only unload map*/
void Game::unload(bool full_unload)
{
  // changeMode(DISABLED);

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

  if(save_handle.isAvailable())
    save_handle.stop(true);
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
  // pollEvents();

  updateMenuEnabledState();
  updateMenuSaving();

  // event_handler.getKeyHandler().print(false, true);

  /* Mode next handling */
  updateMode(cycle_time);

  /* MAP MODE */
  if(mode == MAP)
  {
    /* Add play time */
    if(player_main != nullptr)
      player_main->addPlayTime(cycle_time);

    /* Poll Events */
    pollEvents();

    /* Change mode to battle if ready */
    if(map_ctrl.isBattleReady())
      changeMode(BATTLE);

    // eventStartBattle(Party::kID_SLEUTH, map_ctrl.getBattleThingID());
    return map_ctrl.update(cycle_time);
  }
  /* BATTLE MODE */
  else if(mode == BATTLE && battle_ctrl)
  {
    /* Add play time */
    if(player_main != nullptr)
      player_main->addPlayTime(cycle_time);

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
    else
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
  else if(mode == MENU)
  {
    /* Add play time */
    if(player_main != nullptr)
      player_main->addPlayTime(cycle_time);

    if(!map_menu.getFlag(MenuState::SHOWING))
      changeMode(MAP);

    // TODO [05-22-16]: Update alterations for map JMT // JT // J MICHAEL T
    return map_menu.update(cycle_time); // map_ctrl.update(cycle_time)
  }

  return false;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/* Static: Returns the save string based on the slot number */
std::string Game::getSlotPath(uint8_t slot, std::string base_path, bool image,
                              bool precall)
{
  std::string save_path = "";

  if(slot < 100)
  {
    /* Front */
    save_path = base_path + kSAVE_PATH_FRONT;

    /* Mid */
    if(precall)
    {
      save_path += kSAVE_PATH_AUTO;
    }
    else
    {
      if(slot < 10)
        save_path += "0";
      save_path += std::to_string(slot);
    }

    /* Tail */
    if(image)
      save_path += kSAVE_IMG_BACK;
    else
      save_path += kSAVE_PATH_BACK;
  }
  return save_path;
}

/* Saves screenshot of the current game as it stands - as a BMP */
bool Game::saveScreenshot(std::string path, SDL_Rect rect,
                          SDL_Renderer* renderer, uint8_t factor)
{
  if(!path.empty() && renderer != nullptr && factor > 0)
  {
    SDL_Surface* sshot = SDL_CreateRGBSurface(
        0, rect.w, rect.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_ARGB8888,
                         sshot->pixels, sshot->pitch);
    SDL_Surface* sshot2 =
        SDL_CreateRGBSurface(0, rect.w / factor, rect.h / factor, 32,
                             0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitScaled(sshot, nullptr, sshot2, nullptr);
    SDL_SaveBMP(sshot2, path.c_str());
    SDL_FreeSurface(sshot);
    SDL_FreeSurface(sshot2);

    return true;
  }
  return false;
}
