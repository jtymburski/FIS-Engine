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

const uint32_t Game::kID_ITEM_MONEY = 0;
const uint32_t Game::kID_PARTY_BEARACKS = 1;
const uint32_t Game::kID_PARTY_SLEUTH = 0;
const uint32_t Game::kID_PERSON_PLAYER = 0;
const uint32_t Game::kID_SET_BUBBIFIED = 0;
const uint32_t Game::kSTARTING_MAP = 0;
const std::string Game::kSTARTING_PATH = "maps/Univursa.ugv";

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
  battle_vis = nullptr;
  config = nullptr;
  game_path = kSTARTING_PATH;
  loaded = false;
  map_ctrl = nullptr;
  map_lvl = kSTARTING_MAP;
  mode = DISABLED;
  player_main = nullptr;

  /* Set up battle and map classes - initial */
  map_ctrl = new Map(config, &event_handler);
  battle_vis = new BattleDisplay(config);

  /* Set game configuration */
  setConfiguration(running_config);
}

/* Destructor function */
Game::~Game()
{
  /* First, unload the game */
  unload();

  /* Clean config */
  config = nullptr;

  /* Delete battle */
  if(battle_ctrl != nullptr)
    delete battle_ctrl;
  if(battle_vis != nullptr)
    delete battle_vis;
  battle_ctrl = nullptr;
  battle_vis = nullptr;

  /* Delete map */
  if(map_ctrl != nullptr)
    delete map_ctrl;
  map_ctrl = nullptr;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/* Add functions for game objects */
Action* Game::addAction(const std::string &raw)
{
  Action* new_action = new Action(raw);
  list_action.push_back(new_action);

  return new_action;
}

/* Add functions for game objects */
Category* Game::addClass(const int32_t &id)
{
  Category* new_category = new Category();
  new_category->setID(id);
  list_class.push_back(new_category);

  return new_category;
}

/* Add functions for game objects */
Flavour* Game::addFlavour(const int32_t &id)
{
  Flavour* new_flavour = new Flavour();
  new_flavour->setID(id);
  list_flavour.push_back(new_flavour);

  return new_flavour;
}

/* Add functions for game objects */
Item* Game::addItem(const int32_t &id, SortObjects type)
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
Party* Game::addParty(const int32_t &id)
{
  Party* new_party = new Party();
  new_party->setID(id);
  list_party.push_back(new_party);

  return new_party;
}

/* Add functions for game objects */
Person* Game::addPersonBase(const int32_t &id)
{
  Person* new_person = new Person();
  new_person->setGameID(id);
  list_person_base.push_back(new_person);

  return new_person;
}

/* Add functions for game objects */
Person* Game::addPersonInst(const int32_t &base_id, const uint32_t &lvl)
{
  return addPersonInst(getPersonBase(base_id), lvl);
}

/* Add functions for game objects */
Person* Game::addPersonInst(Person* base_person, const uint32_t &lvl)
{
  Person* new_person = nullptr;

  if(base_person != NULL)
  {
    new_person = new Person(base_person);

    /* Set Level */
    new_person->loseExp(new_person->getTotalExp());
    new_person->addExp(new_person->getExpAt(lvl));

    list_person_inst.push_back(new_person);
  }

  return new_person;
}

/* Add functions for game objects */
Category* Game::addRace(const int32_t &id)
{
  Category* new_race = new Category();
  new_race->setID(id);
  list_race.push_back(new_race);

  return new_race;
}

/* Add functions for game objects */
Skill* Game::addSkill(const int32_t &id)
{
  Skill* new_skill = new Skill();
  new_skill->setID(id);
  list_skill.push_back(new_skill);

  return new_skill;
}

/* Add functions for game objects */
SkillSet* Game::addSkillSet(const int32_t &id)
{
  SkillSet* new_set = new SkillSet(id);
  list_set.push_back(new_set);

  return new_set;
}

//TODO: Battle display data container, instead of instantiating upon
// display creation? [07-18-15] - from file??
void Game::buildBattleDisplayFrames(SDL_Renderer* renderer)
{
  if(battle_vis != nullptr)
  {
    /* Set the ailments */
    battle_vis->setAilment(Infliction::POISON,
        base_path + "sprites/Battle/Ailments/Poison_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::BURN,
        base_path + "sprites/Battle/Ailments/Burn01_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::SCALD,
        base_path + "sprites/Battle/Ailments/Burn02_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::CHARR,
        base_path + "sprites/Battle/Ailments/Burn03_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::BERSERK,
        base_path + "sprites/Battle/Ailments/Berserk_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::CONFUSE,
        base_path + "sprites/Battle/Ailments/Confused_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::SILENCE,
        base_path + "sprites/Battle/Ailments/Silence_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::BUBBIFY,
        base_path + "sprites/Battle/Ailments/Bubbified_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::DEATHTIMER,
        base_path + "sprites/Battle/Ailments/DeathTimer_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::PARALYSIS,
        base_path + "sprites/Battle/Ailments/Paralysis_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::BLINDNESS,
        base_path + "sprites/Battle/Ailments/Blind_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::DREADSTRUCK,
        base_path + "sprites/Battle/Ailments/DreadStruck_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::DREAMSNARE,
        base_path + "sprites/Battle/Ailments/DreamSnare_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::HELLBOUND,
        base_path + "sprites/Battle/Ailments/HellBound_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::BOND,
        base_path + "sprites/Battle/Ailments/Bond02_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::BONDED,
        base_path + "sprites/Battle/Ailments/Bond01_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::ALLATKBUFF,
        base_path + "sprites/Battle/Ailments/AllAtkBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::ALLDEFBUFF,
        base_path + "sprites/Battle/Ailments/AllDefBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::PHYBUFF,
        base_path + "sprites/Battle/Ailments/PhyBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::THRBUFF,
        base_path + "sprites/Battle/Ailments/ThrBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::POLBUFF,
        base_path + "sprites/Battle/Ailments/PolBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::PRIBUFF,
        base_path + "sprites/Battle/Ailments/PriBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::CHGBUFF,
        base_path + "sprites/Battle/Ailments/ChgBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::CYBBUFF,
        base_path + "sprites/Battle/Ailments/CybBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::NIHBUFF,
        base_path + "sprites/Battle/Ailments/NihBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::LIMBUFF,
        base_path + "sprites/Battle/Ailments/LimBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::UNBBUFF,
        base_path + "sprites/Battle/Ailments/_placeholder.png", renderer);
    battle_vis->setAilment(Infliction::VITBUFF,
        base_path + "sprites/Battle/Ailments/VitBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::QDBUFF,
        base_path + "sprites/Battle/Ailments/QDBuff_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::ROOTBOUND,
        base_path + "sprites/Battle/Ailments/RootBound_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::DOUBLECAST,
        base_path + "sprites/Battle/Ailments/DoubleCast_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::TRIPLECAST,
        base_path + "sprites/Battle/Ailments/TripleCast_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::HALFCOST,
        base_path + "sprites/Battle/Ailments/HalfCost_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::REFLECT,
        base_path + "sprites/Battle/Ailments/Reflect_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::HIBERNATION,
        base_path + "sprites/Battle/Ailments/Hibernation_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::CURSE,
        base_path + "sprites/Battle/Ailments/Curse_AA_A00.png", renderer);
    battle_vis->setAilment(Infliction::METATETHER,
        base_path + "sprites/Battle/Ailments/MetaTether_AA_A00.png", renderer);

    /* Set the elements */
    battle_vis->setElement(Element::PHYSICAL, base_path +
            "sprites/Battle/Skills/Elements/Physical_AA_A00.png", renderer);
    battle_vis->setElement(Element::FIRE, base_path +
            "sprites/Battle/Skills/Elements/Thermal_AA_A00.png", renderer);
    battle_vis->setElement(Element::ICE, base_path +
            "sprites/Battle/Skills/Elements/Polar_AA_A00.png", renderer);
    battle_vis->setElement(Element::FOREST, base_path +
            "sprites/Battle/Skills/Elements/Primal_AA_A00.png", renderer);
    battle_vis->setElement(Element::ELECTRIC, base_path +
            "sprites/Battle/Skills/Elements/Charge_AA_A00.png", renderer);
    battle_vis->setElement(Element::DIGITAL, base_path +
            "sprites/Battle/Skills/Elements/Cyber_AA_A00.png", renderer);
    battle_vis->setElement(Element::NIHIL, base_path +
            "sprites/Battle/Skills/Elements/Nihil_AA_A00.png", renderer);

    /* Set the action scopes */
    battle_vis->setScope(ActionScope::USER, base_path +
        "sprites/Battle/Skills/Targets/user_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ONE_TARGET, base_path +
        "sprites/Battle/Skills/Targets/single_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ONE_ENEMY, base_path +
        "sprites/Battle/Skills/Targets/singleenemy_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::TWO_ENEMIES, base_path +
        "sprites/Battle/Skills/Targets/twoenemies_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ALL_ENEMIES, base_path +
        "sprites/Battle/Skills/Targets/allenemies_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ONE_ALLY, base_path +
        "sprites/Battle/Skills/Targets/singleally_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ONE_ALLY_NOT_USER, base_path +
        "sprites/Battle/Skills/Targets/singlenotuserally_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::TWO_ALLIES, base_path +
        "sprites/Battle/Skills/Targets/twoallies_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ALL_ALLIES, base_path +
        "sprites/Battle/Skills/Targets/allallies_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ONE_ALLY_KO, base_path +
        "sprites/Battle/Skills/Targets/singlekoedally_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ALL_ALLIES_KO, base_path +
        "sprites/Battle/Skills/Targets/allkoedallies_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ONE_PARTY, base_path +
        "sprites/Battle/Skills/Targets/singleparty_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ALL_TARGETS, base_path +
        "sprites/Battle/Skills/Targets/all_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::NOT_USER, base_path +
        "sprites/Battle/Skills/Targets/singlenotuser_AA_A00.png", renderer);
    battle_vis->setScope(ActionScope::ALL_NOT_USER, base_path +
        "sprites/Battle/Skills/Targets/allnotuser_AA_A00.png", renderer);

    /* Set up the extram frames */
    battle_vis->setFramePercent(base_path +
                  "sprites/Battle/Skills/Extras/Accuracy_AA_A00.png", renderer);
    battle_vis->setFrameQD(base_path +
                  "sprites/Battle/Skills/Extras/QDSymbol_AA_A00.png", renderer);
    battle_vis->setFrameTime(base_path +
                  "sprites/Battle/Skills/Extras/Cooldown_AA_A00.png", renderer);

    /* Background and bar */
    battle_vis->setBackground(new Sprite(
            base_path + "sprites/Battle/Backdrop/battlebg00.png", renderer));
    battle_vis->setBattleBar(new Frame(
            base_path + "sprites/Overlay/battle.png", renderer));

    /* Set up the Ailment pleps */
    // TODO: Implement
//    auto temp_sprite = new Sprite(config->getBasePath() +
//        "sprites/Battle/Pleps/hibernationplep_AA_A", 4, ".png", renderer);
//    temp_sprite->insertTail(config->getBasePath() +
//                            "sprites/Battle/Pleps/hibernationplep_AA_A00.png", renderer);
//    temp_sprite->insertTail(config->getBasePath() +
//                            "sprites/Battle/Pleps/hibernationplep_AA_A01.png", renderer);
//    temp_sprite->insertTail(config->getBasePath() +
//                            "sprites/Battle/Pleps/hibernationplep_AA_A02.png", renderer);
//    temp_sprite->insertTail(config->getBasePath() +
//                            "sprites/Battle/Pleps/hibernationplep_AA_A03.png", renderer);
//    for(uint16_t i = 0; i < 5; i++)
//      temp_sprite->insertTail(config->getBasePath() +
//                              "sprites/blank.png", renderer);
//    temp_sprite->setAnimationTime(180);
//
//    battle_vis->setAilmentPlep(Infliction::POISON, temp_sprite);
  }
}

/* A give item event, based on an ID and count (triggered from stored event */
bool Game::eventGiveItem(int id, int count)
{
  /* Attempt to find the item */
  Item* found_item = nullptr;
  for(auto it = list_item.begin(); it != list_item.end(); it++)
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
      if(map_ctrl != nullptr)
        map_ctrl->initNotification(found_item->getThumb(), count);
      return true;
    }
    /* Otherwise, notify that item could not be received */
    else
    {
      if(map_ctrl != nullptr)
        map_ctrl->initNotification("Insufficient room in inventory to fit " +
                                   std::to_string(count) + " " +
                                   found_item->getName());
    }
  }

  return false;
}

/* Initiates a conversation event */
void Game::eventInitConversation(Conversation* convo, MapThing* source)
{
  if(map_ctrl != nullptr)
    map_ctrl->initConversation(convo, source);
}

/* Initiates a notification event (in map) */
void Game::eventInitNotification(std::string notification)
{
  if(map_ctrl != nullptr)
    map_ctrl->initNotification(notification);
}

/* The pickup item event - from walking over or triggering from action key */
void Game::eventPickupItem(MapItem* item, bool walkover)
{
  if(item != nullptr && item->isWalkover() == walkover)
  {
    bool was_inserted = eventGiveItem(item->getCoreID(), item->getCount());
    was_inserted = true; // TODO: Fix once give item is working

    /* If the insert was successful, pickup the item */
    if(map_ctrl != nullptr && was_inserted)
      map_ctrl->pickupItem(item);
  }
}

/* Starts a battle event. Using the given information */
void Game::eventStartBattle(int person_id, int source_id)
{
  if(person_id >= 0 && source_id >= 0 &&
     setupBattle(getParty(person_id), getParty(source_id)))
  {
    mode = BATTLE;
  }
}

/* Teleport thing event, based on ID and coordinates */
void Game::eventTeleportThing(int thing_id, int x, int y, int section_id)
{
  if(map_ctrl != nullptr)
    map_ctrl->teleportThing(thing_id, x, y, section_id);
}
  
/* Load game */
// TODO: Comment
// Notes: Revise that when inst file is blank, it uses default starting map.
//        Otherwise, determine current map from inst first, then load correct
//        base followed by correct inst map and all associated data.
bool Game::load(std::string base_file, SDL_Renderer* renderer, 
                std::string inst_file, bool encryption)
{
  (void)inst_file;

  bool done = false;
  bool read_success = true;
  int index = 0;
  std::string level = std::to_string(map_lvl);
  bool success = true;

  /* Ensure nothing is loaded */
  unload();

  /* Create the file handler */
  FileHandler fh(base_file, false, true, encryption);
  XmlData data;

  /* Start the map read */
  success &= fh.start();
  std::cout << "Date: " << fh.getDate() << std::endl;
 
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
        if(data.getElement(index + 1) == "core")
        {
          success &= loadData(data, index + 2, renderer);
        }
        /* Map data */
        else if(data.getElement(index + 1) == "map" && 
                data.getKeyValue(index + 1) == level)
        {
          success &= map_ctrl->loadData(data, index + 2, renderer, base_path);
        }
      }
    } while(!done);// && success); // TODO: Success in loop??
  }

  success &= fh.stop();

  /* If the load was successful, proceed to clean-up */
  if(success)
  {
    /* Player set-up */
    player_main = new Player(getParty(kID_PARTY_SLEUTH), 
                             getParty(kID_PARTY_BEARACKS));

    /* Clean up map */
    map_ctrl->loadDataFinish(renderer);

    mode = MAP;
  }
  /* If failed, unload */
  else
  {
    unload();
  }
  loaded = success;

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

          success &= edit_party->addMember(
                                     addPersonInst(person_id, person_lvl));
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
            AddStatus status = edit_party->getInventory()
                                         ->add(new_item, item_count);
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
      /* Get the reference objects and check if valid */
      MapPerson* person;
      MapThing* source;
      if(event_handler.pollStartBattle(&person, &source))
      {
        /* Try and find parties and start battle */
        if(person != nullptr && source != nullptr)
          eventStartBattle(person->getGameID(), source->getGameID());
      }
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
  
/* Remove functions for game objects */
void Game::removeActions()
{
  for(auto it = begin(list_action); it != end(list_action); ++it)
    delete (*it);
  list_action.clear();
}

/* Remove all game objects in the proper order */
void Game::removeAll()
{
  removeParties();
  removePersonInstances();
  removePersonBases();

  removeItems();
  removeClasses();
  removeRaces();
  removeSkillSets();

  removeFlavours();
  removeSkills();
  removeActions();
}

/* Remove functions for game objects */
void Game::removeClasses()
{
  for(auto it = begin(list_class); it != end(list_class); ++it)
    delete (*it);
  list_class.clear();
}

/* Remove functions for game objects */
void Game::removeFlavours()
{
  for(auto it = begin(list_flavour); it != end(list_flavour); ++it)
    delete (*it);
  list_flavour.clear();
}

/* Remove functions for game objects */
void Game::removeItems()
{
  for(auto it = begin(list_item); it != end(list_item); it++)
    delete (*it);
  list_item.clear();
}

/* Remove functions for game objects */
void Game::removeParties()
{
  for(auto it = begin(list_party); it != end(list_party); it++)
    delete (*it);
  list_party.clear();
}

/* Remove functions for game objects */
void Game::removePersonBases()
{
  for(auto it = begin(list_person_base); it != end(list_person_base); ++it)
    delete (*it);
  list_person_base.clear();
}

/* Remove functions for game objects */
void Game::removePersonInstances()
{
  for(auto it = begin(list_person_inst); it != end(list_person_inst); ++it)
    delete (*it);
  list_person_inst.clear();
}

/* Remove functions for game objects */
void Game::removeRaces()
{
  for(auto it = begin(list_race); it != end(list_race); ++it)
    delete (*it);
  list_race.clear();
}

/* Remove functions for game objects */
void Game::removeSkills()
{
  for(auto it = begin(list_skill); it != end(list_skill); ++it)
    delete (*it);
  list_skill.clear();
}

/* Remove functions for game objects */
void Game::removeSkillSets()
{
  for(auto it = begin(list_set); it != end(list_set); ++it)
    delete (*it);
  list_set.clear();
}

/* Set up the battle - old battle needs to be deleted prior to calling */
bool Game::setupBattle(Party* allies, Party* foes)
{
  if(allies != nullptr && foes != nullptr)
  {
    /* Battle prep */
    for(auto &member : allies->getMembers())
      member->battlePrep();
    for(auto &member : foes->getMembers())
      member->battlePrep();

    /* Set up battle */
    battle_ctrl = new Battle(allies, foes, getSkillSet(kID_SET_BUBBIFIED), 
                             &event_handler);
    battle_vis->setBattle(battle_ctrl, active_renderer);

    return true;
  }

  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns a pointer to a given action by index or by ID */
Action* Game::getAction(const int32_t &index, const bool& by_id)
{
  if (by_id)
  {
    for (auto it = begin(list_action); it != end(list_action); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < list_action.size())
  {
    return list_action.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a battle class by index or by ID */
Category* Game::getClass(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(list_class); it != end(list_class); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < list_class.size())
  {
    return list_class.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a flavour by index or by ID */
Flavour* Game::getFlavour(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(list_flavour); it != end(list_flavour); ++it)
      if ((*it)->getGameID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < list_flavour.size())
  {
    return list_flavour.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a person by index or by ID */
Item* Game::getItem(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(list_item); it != end(list_item); ++it)
      if ((*it)->getGameID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < list_item.size())
  {
    return list_item.at(index);
  }

  return nullptr;
}
  
/* Returns a pointer to a party by index or by ID */
Party* Game::getParty(const int32_t &index, const bool &by_id)
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
Person* Game::getPersonBase(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(list_person_base); it != end(list_person_base); ++it)
      if ((*it)->getGameID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < list_person_base.size())
  {
    return list_person_base.at(index);
  }

  return nullptr;
}
  
/* Returns a pointer to an instance person by index or by ID (unique) */
Person* Game::getPersonInst(const int32_t &index, const bool &by_id)
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
Category* Game::getRace(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(list_race); it != end(list_race); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < list_race.size())
  {
    return list_race.at(index);
  }

  return nullptr;
}

/* Returns a pointer to a skill by index or by ID */
Skill* Game::getSkill(const int32_t &index, const bool &by_id)
{
  if (by_id)
  {
    for (auto it = begin(list_skill); it != end(list_skill); ++it)
      if ((*it)->getID() == index)
        return (*it);
  }
  else if (static_cast<uint32_t>(index) < list_skill.size())
  {
    return list_skill.at(index);
  }

  return nullptr;
}
  
/* Returns a pointer to a skillset by index or by ID */
SkillSet* Game::getSkillSet(const int32_t &index, const bool &by_id)
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
  
/* Is the game loaded */
bool Game::isLoaded()
{
  return loaded;
}

/* The key down events to be handled by the class */
bool Game::keyDownEvent(SDL_KeyboardEvent event)
{
  /* Exit the game, game has finished processing */
  if(event.keysym.sym == SDLK_ESCAPE)
  {
    return true;
  }
  /* TESTING section - probably remove at end */
  /* Switch the view to the map */
  else if(event.keysym.sym == SDLK_F1)
  {
    mode = MAP;

    if(battle_ctrl != nullptr)
    {
      delete battle_ctrl;
      battle_ctrl = nullptr;
    }
  }
  /* Switch the view to the battle */
  else if(event.keysym.sym == SDLK_F2)
  {
    if (battle_ctrl == nullptr)
      eventStartBattle(0, 400);
  }
  /* Load game */
  else if(event.keysym.sym == SDLK_F5)
  {
    /* Preliminary handling */
    if(game_path != "maps/design_map.ugv")
    {
      game_path = "maps/design_map.ugv";
      map_lvl = 0;
    }
    else
    {
      map_lvl++;
      if(map_lvl > 2)
        map_lvl = 0;
    }

    /* Load map */
    load(active_renderer);
  }
  /* Show item store dialog in map */
  else if(event.keysym.sym == SDLK_5)
  {
    // TODO: Future
    //if (map_ctrl != nullptr)
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
  else if(event.keysym.sym == SDLK_LCTRL)
  {
    // if(battle_vis != NULL)
    //   battle_vis->setRenderFlag(RenderState::SHOW_INFO);
  }
  /* Otherwise, send keys to the active view */
  else
  {
    if(mode == MAP)
      return map_ctrl->keyDownEvent(event);
    else if(mode == BATTLE)
    {
      if(battle_vis->getRenderingState() == battle_ctrl->getTurnState())
        return battle_ctrl->keyDownEvent(event);
    }
  }

  return false;
}

/* The key up events to be handled by the class */
void Game::keyUpEvent(SDL_KeyboardEvent event)
{
  if(event.keysym.sym == SDLK_LCTRL)
  {
    // if(battle_vis != NULL)
    //   battle_vis->setShowInfo(false);
  }
  else if(mode == MAP)
  {
    map_ctrl->keyUpEvent(event);
  }
}

/* Load game */
bool Game::load(SDL_Renderer* renderer)
{
  //return load(game_path, renderer);
  return load(base_path + game_path, renderer);
}

/* Renders the title screen */
bool Game::render(SDL_Renderer* renderer)
{
  // TODO: Create temporary list of items - Pull into file and remove */
//  if(list_item.empty())
//  {
//    Item* item1 = new Item(0, "Bubby Saber", 125, new Frame(
//        "sprites/Map/Tiles/00_Generic/Scenery/Interactables/Weapons/02_Uncommon/BubbySaber01_AA_A00.png",
//        renderer));
//    Item* item2 = new Item(1, "Frost Bubby", 5, new Frame(
//                           "sprites/Battle/Bubbies/frosty_t1.png", renderer));
//    Item* item3 = new Item(2, "Coins", 1, new Frame(
//                           "sprites/Map/_TEST/coins_AA_A00.png", renderer));
//    Item* item4 = new Item(3, "Ravizer Sword", 250, new Frame(
//        "sprites/Map/Tiles/00_Generic/Scenery/Interactables/Weapons/03_Rare/RavizerSword07_AA_A00.png",
//        renderer));
//    Item* item5 = new Item(4, "Blazing Bubby", 4, new Frame(
//                            "sprites/Battle/Bubbies/blazing_t1.png", renderer));
//    list_item.push_back(item1);
//    list_item.push_back(item2);
//    list_item.push_back(item3);
//    list_item.push_back(item4);
//    list_item.push_back(item5);
//  }

  /* Make sure the active renderer is set up */
  if(active_renderer == NULL)
  {
    buildBattleDisplayFrames(renderer);

    active_renderer = renderer;
  }

  if(mode == MAP)
    return map_ctrl->render(renderer);
  else if(mode == BATTLE)
    return battle_vis->render(renderer);

  return true;
}

/* Set the running configuration, from the options class */
bool Game::setConfiguration(Options* running_config)
{
  if(running_config != nullptr)
  {
    config = running_config;
    base_path = config->getBasePath();

    /* Set in secondary classes */
    if(map_ctrl != nullptr)
      map_ctrl->setConfiguration(running_config);

    /* Battle configuration setup */
    if(battle_vis != nullptr)
      battle_vis->setConfiguration(running_config);

    return true;
  }

  return false;
}

/* Sets the path of the game */
bool Game::setPath(std::string path, int level)
{
  if(!path.empty() && level >= 0)
  {
    game_path = path;
    map_lvl = level;
    return true;
  }
  return false;
}

/* Unload the game */
void Game::unload()
{
  if(loaded)
  {
    /* Unload map first */
    map_ctrl->unloadMap();

    /* Unload game data */
    removeAll();

    loaded = false;
    mode = DISABLED;
  }
}

/* Updates the game state. Returns true if the class is finished */
bool Game::update(int32_t cycle_time)
{
  ticks_total++;

  /* Poll System Events */
  pollEvents();

  /* Update the key handler */
  event_handler.getKeyHandler().update(cycle_time);

  /* MAP MODE */
  if(mode == MAP && map_ctrl != nullptr)
  {
    return map_ctrl->update(cycle_time);
  }
  /* BATTLE MODE */
  else if(mode == BATTLE)
  {
    bool success = true;

    if(battle_ctrl != nullptr)
    {
      success &= battle_ctrl->update(cycle_time);

      if(battle_ctrl->getTurnState() == TurnState::DESTRUCT)
      {
        mode = MAP;

        /* Delete logic */
        battle_vis->unsetBattle();
        delete battle_ctrl;
        battle_ctrl = nullptr;
      }
      else if(battle_vis != nullptr)
      {
        success &= battle_vis->update(cycle_time);
      }
    }

    return success;
  }

  return false;
}
