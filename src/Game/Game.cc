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

/* Starts a battle event. Using the given information - TODO */
void Game::eventStartBattle()
{
  setupBattle();
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
  // Test Actions
  std::vector<Action*> damage_actions;
  damage_actions.push_back(new Action("1,DAMAGE,,,,,AMOUNT.20,AMOUNT.5,,95"));
  damage_actions.push_back(new Action("2,DAMAGE,,,,VITA,AMOUNT.20,AMOUNT.5,,95"));
  damage_actions.push_back(new Action("3,DAMAGE,,,,VITA,AMOUNT.20,AMOUNT.5,,95"));
  damage_actions.push_back(new Action("4,DAMAGE,,,,,AMOUNT.20,AMOUNT.5,,95"));
  damage_actions.push_back(new Action("5,DAMAGE,,,,VITA,AMOUNT.60,AMOUNT.20,,95"));
  damage_actions.push_back(new Action("6,DAMAGE,,,,VITA,AMOUNT.75,AMOUNT.25,,95"));
  damage_actions.push_back(new Action("7,DAMAGE,,,,VITA,AMOUNT.90,AMOUNT.25,,95"));
  damage_actions.push_back(new Action("8,DAMAGE,,,,VITA,AMOUNT.100,AMOUNT.30,,95"));
  damage_actions.push_back(new Action("9,DAMAGE,,,,VITA,AMOUNT.300,AMOUNT.35,,95"));
  damage_actions.push_back(new Action("10,DAMAGE,,,,VITA,AMOUNT.400,AMOUNT.50,,95"));
  damage_actions.push_back(new Action("11,DAMAGE,,,,VITA,AMOUNT.600,PC.1,,95"));
  damage_actions.push_back(new Action("12,DAMAGE,,,,VITA,AMOUNT.800,PC.5,,95"));
  damage_actions.push_back(new Action("13,DAMAGE,,,,VITA,AMOUNT.60,PC.10,,95"));
  damage_actions.push_back(new Action("14,DAMAGE,,,,VITA,AMOUNT.75,PC.15,,95"));
  damage_actions.push_back(new Action("15,DAMAGE,,,,VITA,AMOUNT.100,PC.25,,95"));
  damage_actions.push_back(new Action("16,DAMAGE,,,,VITA,AMOUNT.150,PC.30,,95"));
  damage_actions.push_back(new Action("17,DAMAGE,,,,VITA,PC.2,PC.1,,95"));
  damage_actions.push_back(new Action("18,DAMAGE,,,,VITA,PC.5,PC.2,,95"));
  damage_actions.push_back(new Action("19,DAMAGE,,,,VITA,PC.10,PC.5,,95"));
  damage_actions.push_back(new Action("20,DAMAGE,,,,VITA,PC.10,PC.9,,95"));
  damage_actions.push_back(new Action("22,DAMAGE,,,,VITA,PC.10,PC.20,,95"));
  damage_actions.push_back(new Action("23,DAMAGE,,,,VITA,PC.15,PC.25,,95"));

  std::vector<Action*> alter_actions;
  alter_actions.push_back(new Action("200,ALTER,,,,VITA,PC.10,AMOUNT.10,,99"));
  alter_actions.push_back(new Action("201,ALTER,,,,,PC.25,AMOUNT.20,VITA,99"));
  alter_actions.push_back(new Action("202,ALTER,,,,VITA,AMOUNT.-100,AMOUNT.20,VITA,100"));
  alter_actions.push_back(new Action("203,ALTER,,,,,PC.-12,AMOUNT.20,VITA,100"));
  alter_actions.push_back(new Action("204,ALTER,,,,QTDR,PC.10,AMOUNT.20,THAG,96"));
  alter_actions.push_back(new Action("204,ALTER-FLIP,,,,QTDR,PC.10,AMOUNT.20,THAG,96"));
  alter_actions.push_back(new Action("205,ALTER-FLIP,,,,VITA,AMOUNT.100,AMOUNT.30,VITA,100"));

  std::vector<Action*> assign_actions;
  assign_actions.push_back(new Action("300,ASSIGN,,,,VITA,AMOUNT.1,AMOUNT.0,,100"));
  assign_actions.push_back(new Action("301,ASSIGN,,,,VITA,AMOUNT.0,AMOUNT.0,,100"));
  assign_actions.push_back(new Action("302,ASSIGN,,,,,PC.20,PC.5,VITA,1"));
  assign_actions.push_back(new Action("303,ASSIGN-FLIP,,,,VITA,PC.20,PC.5,QTDR,1"));
  assign_actions.push_back(new Action("400,REVIVE,,,,,AMOUNT.50,AMOUNT.10,,80"));

  std::vector<Action*> inflict_actions;
  inflict_actions.push_back(new Action("500,INFLICT,4.7,,,POISON,,,VITA,90"));
  inflict_actions.push_back(new Action("501,INFLICT,1.4,,,BURN,AMOUNT.50,,VITA,90"));
  inflict_actions.push_back(new Action("502,INFLICT,2.6,,,SCALD,AMOUNT.40,,VITA,90"));
  inflict_actions.push_back(new Action("503,INFLICT,4.8,,,CHARR,AMOUNT.30,,VITA,90"));
  inflict_actions.push_back(new Action("504,INFLICT,4.4,,,BERSERK,AMOUNT.0,,VITA,95"));
  inflict_actions.push_back(new Action("505,INFLICT,2.3,,,CONFUSE,AMOUNT.50,,VITA,99"));
  inflict_actions.push_back(new Action("506,INFLICT,2.3,,,SILENCE,AMOUNT.60,,VITA,99"));
  inflict_actions.push_back(new Action("507,INFLICT,4.8,,,BUBBIFY,AMOUNT.70,,VITA,99"));
  inflict_actions.push_back(new Action("508,INFLICT,2.5,,,ALLATKBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("509,INFLICT,2.5,,,ALLDEFBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("510,INFLICT,2.5,,,PHYBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("511,INFLICT,2.5,,,THRBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("512,INFLICT,2.5,,,POLBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("513,INFLICT,2.5,,,PRIBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("514,INFLICT,2.5,,,CHGBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("515,INFLICT,2.5,,,CYBBUFF,AMOUNT.0,,VITA,99")); 
  inflict_actions.push_back(new Action("516,INFLICT,2.5,,,NIHBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("517,INFLICT,2.5,,,LIMBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("518,INFLICT,2.5,,,UNBBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("519,INFLICT,2.5,,,VITBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("520,INFLICT,2.5,,,QDBUFF,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("521,INFLICT,5.5,,,DEATHTIMER,AMOUNT.0,,VITA,99"));
  inflict_actions.push_back(new Action("522,INFLICT,3.5,,,PARALYSIS,AMOUNT.30,,VITA,99"));
  inflict_actions.push_back(new Action("523,INFLICT,2.5,,,BLINDNESS,AMOUNT.20,,VITA,99"));
  inflict_actions.push_back(new Action("524,INFLICT,2.4,,,DREADSTRUCK,AMOUNT.25,,VITA,99"));
  inflict_actions.push_back(new Action("525,INFLICT,2.6,,,DREAMSNARE,AMOUNT.25,,VITA,99"));
  inflict_actions.push_back(new Action("526,INFLICT,4.8,,,MODULATE,AMOUNT.10,,VITA,99"));

  std::vector<Action*> relieve_actions;
  relieve_actions.push_back(new Action("600,RELIEVE,,,,POISON,,,VITA,100"));

  // for(auto it = begin(damage_actions); it != end(damage_actions); ++it)
  //   std::cout << (*it)->actionFlag(ActionFlags::VALID) << std::endl;

  // for (auto it = begin(alter_actions); it != end(alter_actions); ++it)
  //   std::cout << (*it)->actionFlag(ActionFlags::VALID) << std::endl;

  // for (auto it = begin (assign_actions); it != end(assign_actions); ++it)
  //   std::cout << (*it)->actionFlag(ActionFlags::VALID) << std::endl;

  //for (auto& inflict_action : inflict_actions)
  //  std::cout << inflict_action->actionFlag(ActionFlags::VALID) << std::endl;
  
  for (auto& relieve_action : relieve_actions)
    std::cout << relieve_action->actionFlag(ActionFlags::VALID) << std::endl;

  // Test Skills
  Skill* cure_poison = new Skill(600, "Cure Poison", ActionScope::ONE_TARGET,
      relieve_actions[0], 95, 0);
  cure_poison->setPrimary(Element::PHYSICAL);
  cure_poison->setThumbnail(new Frame("sprites/Battle/Skills/_sample_skill_2.png", active_renderer));
  cure_poison->setDescription("Cures an inflicted person from poison!");
  cure_poison->setFlag(SkillFlags::DEFENSIVE);

  Skill* physical_01 = new Skill(100, "Wee Strike", ActionScope::ONE_ENEMY, 
      damage_actions[3], 95, 0);
  physical_01->setPrimary(Element::PHYSICAL);
  physical_01->setThumbnail(new Frame("sprite3/Battle/Skills/_sample_skill_2.png", active_renderer));
  physical_01->setDescription("Jordan is a weirdo... Lor em ips um do lor sit amet, mel omnis nomin ati an, atom orum facil isis in pri, adipi scing argum entum in pri. Duo ei tempor dicunt sanctus, per ut hinc oporteat conceptam. Iisque euismod albucius vel ut, duo ea singulis eleifend. Veri offendit vim ut, at pri tale adolescens, putant veritus sea no. Atqui blandit assentior ne eam. Et rebum deserunt pericula eum.");
  physical_01->setFlag(SkillFlags::OFFENSIVE);

  Skill* physical_02 = new Skill(101, "Triple Whelp", ActionScope::ONE_ENEMY, 
      damage_actions[4],  95, 5);
  physical_02->addAction(damage_actions[5]);
  physical_02->addAction(damage_actions[6]);
  physical_02->setPrimary(Element::PHYSICAL);
  physical_02->setFlag(SkillFlags::OFFENSIVE);

  // Skill* physical_03 = new Skill(102, "Two Smackeroos", 
  //     ActionScope::TWO_ENEMIES, damage_actions[6], 95, 100);
  // physical_03->setPrimary(Element::PHYSICAL);

  // Skill* physical_04 = new Skill(103, "Mass Smack", ActionScope::ALL_ENEMIES, 
  //     damage_actions[8], 85, 5); 
  // physical_04->setPrimary(Element::PHYSICAL);

  Skill* fire_01 = new Skill(110, "Burninate The Countryside", ActionScope::ONE_PARTY, 
    damage_actions[11], 85, 55);
  fire_01->setCooldown(1);
  fire_01->setThumbnail(new Frame("sprites/Battle/Skills/_sample_skill_1.png", 
                                  active_renderer));
  fire_01->setPrimary(Element::FIRE);
  fire_01->setSecondary(Element::PHYSICAL);
  fire_01->setFlag(SkillFlags::OFFENSIVE);

  Skill* forest_01 = new Skill(111, "Earth Strike", ActionScope::ONE_ENEMY,
                               damage_actions[4], 91, 4);
  forest_01->setPrimary(Element::FOREST);

  // Skill* ice_01 = new Skill(112, "Frosty Spray", ActionScope::TWO_ENEMIES, 
  //   damage_actions[5], 99, 5);
  // ice_01->setPrimary(Element::ICE);

  // Skill* electric_01 = new Skill(113, "Shock", ActionScope::ALL_ENEMIES,
  //   damage_actions[6], 99, 6);
  // electric_01->setPrimary(Element::ELECTRIC);

  // Skill* digital_01 = new Skill(114, "Compile", ActionScope::ONE_ENEMY,
  //   damage_actions[6], 99, 6);
  // digital_01->setPrimary(Element::DIGITAL);

  // Skill* void_01 = new Skill(115, "Demi", ActionScope::ONE_ENEMY,
  //   damage_actions[7], 99, 7);
  // void_01->setPrimary(Element::NIHIL);

  Skill* self_heal = new Skill(120, "Heal Self", ActionScope::USER,
    alter_actions[0], 100, 5);
  self_heal->setPrimary(Element::FOREST);
  self_heal->setPrimary(Element::PHYSICAL);

  Skill* ally_heal = new Skill(121, "Heal Ally", ActionScope::ONE_ALLY_NOT_USER,
    alter_actions[1], 100, 5);
  ally_heal->setPrimary(Element::FOREST);
  ally_heal->setSecondary(Element::PHYSICAL);
  ally_heal->setFlag(SkillFlags::DEFENSIVE);

  // Skill* user_reduce_by_ally = new Skill(122, "Reduce User By Ally", ActionScope::USER,
  //   alter_actions[2], 100, 5);
  // ally_heal->setPrimary(Element::PHYSICAL);

  // Skill* alter_qtdr_by_thag = new Skill(123, "Alter Qtdr By Thag", ActionScope::ONE_TARGET,
  //   alter_actions[4], 100, 5);

  // Skill* alter_qtdr_by_thag_flip = new Skill(123, "Alter Qtdr By Thag - Flip", ActionScope::ONE_TARGET,
  //   alter_actions[5], 100, 5);

  Skill* revive_ally = new Skill(130, "Revive Ally", ActionScope::ONE_ALLY_KO,
    assign_actions[4], 100, 5);
  revive_ally->setPrimary(Element::FOREST);
  revive_ally->setFlag(SkillFlags::DEFENSIVE);

  Skill* life_steal = new Skill(131, "Life Steal", ActionScope::ONE_ENEMY,
    alter_actions[2], 100, 5);
  life_steal->addAction(alter_actions[6], true);
  life_steal->setFlag(SkillFlags::DEFENSIVE);

  Skill* poison = new Skill(140, "Inflict Poison", ActionScope::ONE_TARGET,
    inflict_actions[0], 100, 10);
  poison->setPrimary(Element::FOREST);
  poison->setFlag(SkillFlags::OFFENSIVE);

  Skill* burn = new Skill(141, "Inflict Burn", ActionScope::ONE_TARGET,
    inflict_actions[1], 100, 10);
  burn->setPrimary(Element::ICE);
  burn->setFlag(SkillFlags::OFFENSIVE);

  Skill* scald = new Skill(142, "Inflict Scald", ActionScope::ONE_TARGET,
    inflict_actions[2], 100, 10);
  scald->setPrimary(Element::FOREST);
  scald->setFlag(SkillFlags::OFFENSIVE);

  Skill* charr = new Skill(143, "Inflict Charr", ActionScope::ONE_TARGET,
    inflict_actions[3], 100, 10);
  charr->setPrimary(Element::ELECTRIC);
  charr->setFlag(SkillFlags::OFFENSIVE);

  Skill* berserk = new Skill(144, "Inflict Berserker", ActionScope::ONE_TARGET,
    inflict_actions[4], 100, 10);
  berserk->setPrimary(Element::DIGITAL);
  berserk->setFlag(SkillFlags::OFFENSIVE);

  Skill* confuse = new Skill(145, "Inflict Befuddle", ActionScope::ONE_ALLY,
    inflict_actions[5], 100, 10);
  confuse->setPrimary(Element::NIHIL);
  confuse->setFlag(SkillFlags::OFFENSIVE);

  Skill* silence = new Skill(146, "Inflict Silence", ActionScope::ONE_TARGET,
    inflict_actions[6], 100, 10);

  // Skill* bubbinate = new Skill(147, "Bubbinate", ActionScope::ONE_TARGET,
  //   inflict_actions[7], 100, 10);

  // Skill* all_atk_buff = new Skill(148, "All Atk Buff", ActionScope::ONE_ALLY,
  //   inflict_actions[8], 100, 10);

  // Skill* all_def_buff = new Skill(149, "All Def Buff", ActionScope::ONE_ALLY,
  //   inflict_actions[9], 100, 10);

  // Skill* thermal_buff = new Skill(150, "Thermal Buff", ActionScope::ONE_ALLY,
  //   inflict_actions[11], 100, 10);

  // Skill* deathtimer = new Skill(151, "Deathtimer", ActionScope::ONE_TARGET,
  //   inflict_actions[21], 100, 10);

  // Skill* paralysis = new Skill(152, "Paralysis", ActionScope::ONE_ENEMY,
  //   inflict_actions[22], 100, 10);
  
  // Skill* blindness = new Skill(153, "Blindness", ActionScope::ONE_TARGET,
  //   inflict_actions[23], 100, 10);

  // Skill* dreadstruck = new Skill(154, "Dreadstruck", ActionScope::ONE_TARGET,
  //   inflict_actions[24], 100, 10);

  // Skill* dreamsnare = new Skill(155, "Dreamsnare", ActionScope::ONE_TARGET,
  //   inflict_actions[25], 100, 10);

  Skill* modulate = new Skill(156, "Modulate", ActionScope::USER, 
    inflict_actions[26], 100, 10);
  modulate->setFlag(SkillFlags::DEFENSIVE, true);
  modulate->setPrimary(Element::PHYSICAL);

  Skill* life_all = new Skill(155, "Regen All", ActionScope::ONE_PARTY,
    alter_actions[1], 94, 45);
  life_all->setFlag(SkillFlags::DEFENSIVE, true);
  life_all->setPrimary(Element::PHYSICAL);

  // Test Skill Sets
  SkillSet* physical_skills = new SkillSet(physical_01, 1);
  physical_skills->addSkill(cure_poison, 1);
  physical_skills->addSkill(physical_02, 1);

  // physical_skills->addSkill(physical_03, 1);
  // physical_skills->addSkill(user_reduce_by_ally, 1);
  // physical_skills->addSkill(alter_qtdr_by_thag, 1);
  // physical_skills->addSkill(alter_qtdr_by_thag_flip, 1);
  // physical_skills->addSkill(physical_04, 1);
  // physical_skills->addSkill(self_heal, 1);
  // physical_skills->addSkill(ally_heal, 1);
  // physical_skills->addSkill(revive_ally, 1);
  // physical_skills->addSkill(life_steal, 1);
  physical_skills->addSkill(poison, 1);
  // physical_skills->addSkill(burn, 1);
  // physical_skills->addSkill(paralysis, 1);
  // physical_skills->addSkill(scald, 1);
  // physical_skills->addSkill(charr, 1);
  physical_skills->addSkill(berserk, 1);
  physical_skills->addSkill(confuse, 1);
  // physical_skills->addSkill(bubbinate, 1);
  // physical_skills->addSkill(all_atk_buff, 1);
  // physical_skills->addSkill(all_def_buff, 1);
  // physical_skills->addSkill(thermal_buff, 1);
  // physical_skills->addSkill(deathtimer, 1);
  // physical_skills->addSkill(blindness, 1);
  // physical_skills->addSkill(dreadstruck, 1);
  // physical_skills->addSkill(dreamsnare, 1);

  physical_skills->addSkill(silence, 1);
  physical_skills->addSkill(fire_01, 1);

  SkillSet* elemental_skills = new SkillSet(fire_01, 1);

  elemental_skills->addSkill(physical_01, 1);
  // elemental_skills->addSkill(forest_01, 1);
  // elemental_skills->addSkill(life_all, 10);
  // elemental_skills->addSkill(modulate, 15);
  // elemental_skills->addSkill(ice_01, 1);
  // elemental_skills->addSkill(electric_01, 1);
  // elemental_skills->addSkill(digital_01, 1);
  // elemental_skills->addSkill(void_01, 1);

  bubbified_skills = new SkillSet(physical_01, 1);
  // bubbified_skills->addSkill(physical_02, 2);

  // Test Attribute Sets
  AttributeSet weak_stats        = AttributeSet(1, true, false);
  AttributeSet not_as_weak_stats = AttributeSet(2, true, false);
  AttributeSet normal_stats      = AttributeSet(3, true, false);
  AttributeSet medium_stats      = AttributeSet(4, true, false);
  AttributeSet top_stats         = AttributeSet(5, true, false);
  AttributeSet boss_stats        = AttributeSet(6, true, false);

  // Test Categories
  Category* human = new Category(200, "Human", "human", normal_stats, 
      top_stats, physical_skills);
  human->setVitaRegenRate(RegenRate::WEAK);
  human->setDescription("A regular old joe from Earth.");
  human->setFlag(CategoryState::DEF_ENABLED, true);
  human->setFlag(CategoryState::GRD_ENABLED, true);
  human->setFlag(CategoryState::E_SWORD, true);

  Category* bear = new Category(201, "Bear", "bear", normal_stats,
    top_stats, physical_skills);
  bear->setVitaRegenRate(RegenRate::WEAK);
  bear->setDescription("A sentient and intelligent bear!");
  bear->setFlag(CategoryState::DEF_ENABLED, true);
  bear->setFlag(CategoryState::GRD_ENABLED, true);
  bear->setFlag(CategoryState::E_CLAWS, true);

  Category* bloodclaw_scion = new Category(251, "Bloodclaw Scion", "scions", 
      weak_stats, boss_stats, elemental_skills);
  bloodclaw_scion->setVitaRegenRate(RegenRate::WEAK);
  bloodclaw_scion->setDescription("A class of soldier excelling in physical combat.");
  bloodclaw_scion->setFlag(CategoryState::DEF_ENABLED, true);
  bloodclaw_scion->setFlag(CategoryState::POWER_DEFENDER, false);
  bloodclaw_scion->setFlag(CategoryState::POWER_GUARDER, false);
  bloodclaw_scion->setFlag(CategoryState::E_SWORD, true);
  bloodclaw_scion->setFlag(CategoryState::E_CLAWS, true);

  Category* tactical_samurai = new Category(252, "Tactical Samurai", "samurai",
      not_as_weak_stats, medium_stats, physical_skills);
  tactical_samurai->setVitaRegenRate(RegenRate::WEAK);
  tactical_samurai->setDescription("A class of swordsman like no other.");
  tactical_samurai->setFlag(CategoryState::DEF_ENABLED, true);
  tactical_samurai->setFlag(CategoryState::POWER_DEFENDER, true);
  tactical_samurai->setFlag(CategoryState::POWER_GUARDER, false);
  tactical_samurai->setFlag(CategoryState::E_SWORD, true);
  tactical_samurai->setFlag(CategoryState::E_CLAWS, false);

  Category* modulator = new Category(253, "Modulator", "modulator",
    weak_stats, boss_stats, elemental_skills);
  modulator->setVitaRegenRate(RegenRate::WEAK);
  modulator->setQDRegenRate(RegenRate::NORMAL);
  human->setQDRegenRate(RegenRate::NORMAL);

  // Test Persons
  base_person_list.push_back(new Person(300, "Malgidus", human,
      tactical_samurai));
  getPerson(300)->addExp(2800);
  getPerson(300)->setCurves(Element::DIGITAL, ElementCurve::C,
                            Element::FOREST, ElementCurve::D, true);

  base_person_list.push_back(new Person(301, "Arcadius", bear, 
    tactical_samurai));
  getPerson(301)->addExp(2800);
  getPerson(301)->setCurves(Element::FIRE, ElementCurve::B,
                            Element::FOREST, ElementCurve::A, true);

  base_person_list.push_back(new Person(302, "Berran", bear,
    tactical_samurai));
  getPerson(302)->addExp(2800);
  getPerson(302)->setCurves(Element::FOREST, ElementCurve::A,
                            Element::ICE, ElementCurve::C, true);

  base_person_list.push_back(new Person(303, "Atkst", bear,
    modulator));
  getPerson(303)->addExp(2800);
  getPerson(303)->setCurves(Element::FOREST, ElementCurve::A,
                            Element::ICE, ElementCurve::C, true);
  
  base_person_list.push_back(new Person(304, "Kevin", bear,
    tactical_samurai));
  getPerson(304)->addExp(2800);
  getPerson(304)->setCurves(Element::FOREST, ElementCurve::A,
                            Element::ICE, ElementCurve::C, true);

  std::vector<BattleItem> items;

  base_person_list.push_back(new Person(310, "Frosty", human, bloodclaw_scion));
  getPerson(310)->addExp(2500);
  getPerson(310)->setCurves(Element::ICE, ElementCurve::C,
                            Element::PHYSICAL, ElementCurve::D, true);

  base_person_list.push_back(new Person(311, "Cloud Dude", human, bloodclaw_scion));
  getPerson(311)->addExp(2500);
  getPerson(311)->setCurves(Element::NIHIL, ElementCurve::A,
                            Element::PHYSICAL, ElementCurve::B, true);

  base_person_list.push_back(new Person(312, "Thruster Barrow", human, bloodclaw_scion));
  getPerson(312)->addExp(2500);
  getPerson(312)->setCurves(Element::NIHIL, ElementCurve::A,
                            Element::PHYSICAL, ElementCurve::B, true);


  base_person_list.push_back(new Person(313, "Dragon", human, bloodclaw_scion));
  getPerson(313)->addExp(2500);
  getPerson(313)->setCurves(Element::NIHIL, ElementCurve::A,
                            Element::PHYSICAL, ElementCurve::B, true);


  base_person_list.push_back(new Person(314, "Splurge", human, bloodclaw_scion));
  getPerson(314)->addExp(2500);
  getPerson(314)->setCurves(Element::NIHIL, ElementCurve::A,
                            Element::PHYSICAL, ElementCurve::B, true);

  AIModule* frosty_module = new AIModule();
  frosty_module->setParent(getPerson(310));
  getPerson(310)->setAI(frosty_module);

  AIModule* cloud_module = new AIModule();
  cloud_module->setParent(getPerson(311));
  getPerson(311)->setAI(cloud_module);

  AIModule* thruster_barrow_module = new AIModule();
  thruster_barrow_module->setParent(getPerson(312));
  getPerson(312)->setAI(thruster_barrow_module);

  AIModule* dragon_module = new AIModule();
  dragon_module->setParent(getPerson(313));
  getPerson(313)->setAI(dragon_module);

  AIModule* splurge_module = new AIModule();
  splurge_module->setParent(getPerson(314));
  getPerson(314)->setAI(splurge_module);

  // AIModule* schwep_module = new AIModule();
  // schwep_module->setParent(schwep);
  // schwep->setAI(schwep_module);

  // AIModule* hamburger_module = new AIModule();
  // hamburger_module->setParent(hamburger);
  // hamburger->setAI(hamburger_module);

  // AIModule* swiss_cheese_module = new AIModule();
  // swiss_cheese_module->setParent(swiss_cheese);
  // swiss_cheese->setAI(swiss_cheese_module);

 // Inventory Testinggd
  Inventory* friends_pouch = new Inventory(401, "Teh Pouch");
  Inventory* foes_pouch = new Inventory(402, "Der Pouch");

  // Party Testing
  Party* friends = new Party(401, getPerson(300), PartyType::SLEUTH, 10,
      friends_pouch);
  friends->addMember(getPerson(301));
  // friends->addMember(getPerson(302));
  // friends->addMember(getPerson(303));
  // friends->addMember(getPerson(304));
  // friends->addMember(george);
  // friends->addMember(gregory);
  // friends->addMember(georgina);
  // friends->addMember(gerald);
  // friends->addMember(geoff);

  Party* enemies = new Party(402, getPerson(310), PartyType::REGULAR_FOE, 
      10, foes_pouch);
  enemies->addMember(getPerson(311));
  // enemies->addMember(getPerson(312));
  // enemies->addMember(getPerson(313));
  // enemies->addMember(getPerson(314));
  // enemies->addMember(schwep);
  // enemies->addMember(hamburger);
  // enemies->addMember(swiss_cheese);
  
  // Battle Testing
  for (uint32_t i = 0; i < friends->getSize(); i++)
    friends->getMember(i)->battlePrep();
  for (uint32_t i = 0; i < enemies->getSize(); i++)
    enemies->getMember(i)->battlePrep();

  //getPerson(300)->print(false, false, true, true);

  game_battle = new Battle(game_config, friends, enemies, bubbified_skills,
      &event_handler);

  if(battle_display == nullptr)
  {
    battle_display = new BattleDisplay(game_config);
    SDL_Renderer* renderer = active_renderer;

    /* Sprites */
    Sprite* background = new Sprite(
           base_path + "sprites/Battle/Backdrop/battlebg", 8, ".png", renderer);
    background->setDirectionReverse();
    background->shiftNext();
    background->setAnimationTime(2500);
    
    Sprite* midlay = new Sprite(
                base_path + "sprites/Battle/Midlays/rain", 6, ".png", renderer);
    midlay->setAnimationTime(64);
    midlay->setOpacity(64);

    Sprite* midlay2 = new Sprite(
                     base_path + "sprites/Map/Lays/fog_underlay.png", renderer);
    midlay2->setColorRed(150);
    midlay2->setColorGreen(150);

    /* Display */
    battle_display->setBackground(background);
    battle_display->setMidlay(midlay2);
    battle_display->addMidlay(midlay);
    battle_display->setBattleBar(new Frame(
                           base_path + "sprites/Overlay/battle.png", renderer));

    /* Set the character sprites */
    getPerson(310)->setSprites(nullptr, new Sprite(
              base_path + "sprites/Battle/Battle_Persons/bsian.png", renderer));
    getPerson(311)->setSprites(nullptr, new Sprite(
            base_path + "sprites/Battle/Battle_Persons/ballman.png", renderer));
    getPerson(312)->setSprites(nullptr, new Sprite(
               base_path + "sprites/Battle/Battle_Persons/forgemasternexus.png", 
               renderer));
    getPerson(313)->setSprites(nullptr, new Sprite(
          base_path + "sprites/Battle/Battle_Persons/peltrance.png", renderer));
    getPerson(314)->setSprites(nullptr, new Sprite(
            base_path + "sprites/Battle/Battle_Persons/necross.png", renderer));
    getPerson(300)->setSprites(new Sprite(base_path + 
        "sprites/Battle/Backs/manbear1_brown_grey.png", renderer), nullptr, 
        new Sprite(base_path + "sprites/Map/Dialog/manbear_brown_grey.png", 
        renderer));
    getPerson(301)->setSprites(new Sprite(base_path + 
        "sprites/Battle/Backs/arcadius0.png", renderer), nullptr, 
        new Sprite(base_path + "sprites/Map/Dialog/arcadius.png", 
        renderer));
    getPerson(302)->setSprites(new Sprite(base_path + 
        "sprites/Battle/Backs/player0.png", renderer), nullptr, 
        new Sprite(base_path + "sprites/Battle/Battle_Persons/player.png", 
        renderer));
    getPerson(303)->setSprites(new Sprite(base_path + 
        "sprites/Battle/Backs/sarkova0.png", renderer), nullptr, 
        new Sprite(base_path + "sprites/Battle/Battle_Persons/sarkova.png", 
        renderer));
    getPerson(304)->setSprites(new Sprite(base_path + 
        "sprites/Battle/Backs/subleezer0.png", renderer), nullptr, 
        new Sprite(base_path + "sprites/Battle/Battle_Persons/subleezer.png", 
        renderer));

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
  }

  battle_display->setBattle(game_battle, active_renderer);

  //malgidus->print(false, false, true, true);
  //frosty->print(false, false, true, true);

  // Begin Time Test
  // auto length = 2000;

  // std::vector<std::vector<std::vector<uint64_t>>> values;

  // for (auto k = 0; k < length; k++)
  // {
  //   std::vector<std::vector<uint64_t>> new_2d_vecs;

  //   for (auto i = 0; i < length; i++)
  //   {
  //     std::vector<uint64_t> new_values;
  //     for (auto j = 0; j < length; j++)
  //     {
  //       new_values.push_back(25000);
  //     }

  //     new_2d_vecs.push_back(new_values);
  //   }

  //   values.push_back(new_2d_vecs);
  // }

  // General Item Testing
  // Item* potion      = new Item(45, "Potion", 70, nullptr, 1.01);
  // potion->setFlag(ItemFlags::CONSUMED, true);
  // potion->setFlag(ItemFlags::DEFENSIVE, true);
  // potion->setFlag(ItemFlags::HEALING_ITEM, true);
  // potion->setOccasion(ActionOccasion::ALWAYS);

  // Item* bubby_bomb  = new Item(47, "Bubby Bomb", 85, nullptr, 2.62);
  // bubby_bomb->setFlag(ItemFlags::CONSUMED, true);
  // bubby_bomb->setFlag(ItemFlags::OFFENSIVE, true);
  // bubby_bomb->setOccasion(ActionOccasion::BATTLE);

  // //Item* unique_item = new Item(46, "Unique Item", 75, nullptr, 1.08);s
  // Item* new_potion     = new Item(potion);
  // Item* new_bubby_bomb = new Item(bubby_bomb);

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

  // test_pouch->sort(SortType::NAME, SortObjects::ZERO_BUBBIES, false);
  // test_pouch->print(false);

  // auto key_items = test_pouch->getKeyItems();

  // for (auto it = begin(key_items); it != end(key_items); ++it)
  // {
  //  std::cout << "Item: " << (*it).first->getName() << " " 
  //           << static_cast<int>((*it).second) << "\n";
  // }

  // delete test_pouch;
  // delete spark;
  // delete moldy;
  // delete tumor

  // new_potion->setUseSkill(medium_attack);
  // new_bubby_bomb->setUseSkill(normal_attack);
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
  else if (event.keysym.sym == SDLK_F3)
  {

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
    if(battle_display != NULL)
      battle_display->setShowInfo(true);
  }
  /* Otherwise, send keys to the active view */
  else
  {
    if(mode == MAP)
      return game_map->keyDownEvent(event);
    else if(mode == BATTLE)
    {
      if(battle_display->getRenderingState() == game_battle->getTurnState() &&
         !battle_display->isPaused())
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
    if(battle_display != NULL)
      battle_display->setShowInfo(false);
  }
  else if(mode == MAP)
  {
    game_map->keyUpEvent(event);
  }
}

/* Renders the title screen */
bool Game::render(SDL_Renderer* renderer)
{
  // TODO Create temporary list of items - Pull into file and remove */
  if(base_item_list.empty())
  {
    Item* item1 = new Item(5, "Bubby Saber", 125, new Frame(
            "sprites/Map/Icons/02_Uncommon/BubbySaber01_AA_A00.png", renderer));
    Item* item2 = new Item(7, "Frost Bubby", 5, new Frame(
                             "sprites/Battle/Bubbies/frosty_t1.png", renderer));
    Item* item3 = new Item(0, "Coins", 1, new Frame(
                               "sprites/Map/_TEST/coins_AA_A00.png", renderer));
    Item* item4 = new Item(6, "Ravizer Sword", 250, new Frame(
              "sprites/Map/Icons/03_Rare/RavizerSword07_AA_A00.png", renderer));
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
      game_map->loadMap(base_path + "maps/test_08.utv", renderer);
      //game_map->loadMap(base_path + "maps/test_06", renderer);
      //game_map->loadMap(base_path + "maps/test01.usv", renderer);
      //game_map->loadMap(base_path + "maps/test_output.ugv", renderer);
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
    if (game_battle != nullptr)
      game_battle->setConfiguration(running_config);
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

  // /* Update the key handler */
  event_handler.getKeyHandler().update(cycle_time);

  //Print out pressed/held keys
  if (num_ticks % 20 == 0)
    event_handler.getKeyHandler().print(true, false);

  if(mode == MAP && game_map != nullptr)
    return game_map->update(cycle_time);

  if(mode == BATTLE)
  {
    bool success = true;

    if(game_battle != nullptr)
    {
      success &= game_battle->update(cycle_time);

      if (game_battle->getBattleFlag(CombatState::OUTCOME_PERFORMED))
        mode = MAP;
    }
    if(battle_display != nullptr)
      success &= battle_display->update(cycle_time);

    return success;
  }

  return false;
}
