/*******************************************************************************
 * Class Name: TestBattle
 * Date Created: July 31, 2015
 * Inheritance: none
 * Description: This class is the testing interface for battle scenarios.
 ******************************************************************************/
#include "TestBattle.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t TestBattle::kLVL_MAX = 127;
const std::string TestBattle::kMENU_ITEMS[] = {"Arcadius",
                                         "Aurora Agent",
                                         "Aurora Agent x 2",
                                         "Aurora Agent x 5",
                                         "Aurora Agent, Heavy",
                                         "Aurora Heavy",
                                         "Aurora Engineer, Drone",
                                         "Aurora Engineer, Drone x 4",
                                         "Aurora Engineer, Agent, Heavy, Drone",
                                         "Reverdile, Reverdling",
                                         "Reverdile, Reverdling x 2",
                                         "Reverdile, Reverdling x 4",
                                         "Reverdling",
                                         "Reverdling x 2",
                                         "Reverdling x 5",
                                         "Exit"};
const std::string TestBattle::kMENU_FOE = "Enter Foe Level: ";
const std::string TestBattle::kMENU_FRIEND = "Enter Friend Level: ";
const uint8_t TestBattle::kNUM_MENU_ITEMS = 16;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* ------------------------------------------------------------------------- */
/* Constructor function */
/* ------------------------------------------------------------------------- */
TestBattle::TestBattle(Options* running_config)
{
  base_path = "";
  battle_display = new BattleDisplay(running_config);
  battle_logic = NULL;
  battle_start = false;
  first_run = true;
  font_normal = NULL;
  game_config = NULL;
  lvl_foe = 1;
  lvl_friend = 1;
  menu_index = 0;
  mode = SCENARIO;
  party_foes = NULL;
  party_friends = NULL;

  setConfiguration(running_config);
  create();
}

/* ------------------------------------------------------------------------- */
/* Destructor function */
/* ------------------------------------------------------------------------- */
TestBattle::~TestBattle()
{
  destroy();

  delete battle_display;
  battle_display = NULL;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* ------------------------------------------------------------------------- */
/* Builds the ailment frames */
/* ------------------------------------------------------------------------- */
void TestBattle::buildBattleDisplay(SDL_Renderer* renderer)
{
  /* Render fonts */
  for(uint8_t i = 0; i < menu_items.size(); i++)
  {
    menu_items[i]->setText(renderer, kMENU_ITEMS[i], {255,255,255,255});
    menu_items_sel[i]->setText(renderer, kMENU_ITEMS[i], {255,0,0,255});
  }

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

  /* Set up the extra frames */
  battle_display->setFramePercent(base_path +
                "sprites/Battle/Skills/Extras/Accuracy_AA_A00.png", renderer);
  battle_display->setFrameQD(base_path +
                "sprites/Battle/Skills/Extras/QDSymbol_AA_A00.png", renderer);
  battle_display->setFrameTime(base_path +
                "sprites/Battle/Skills/Extras/Cooldown_AA_A00.png", renderer);

  /* Set up the Ailment pleps */
  // TODO: Temporary
  auto poison = new Sprite(game_config->getBasePath() +
              "sprites/Battle/Pleps/hibernationplep_AA_A", 4, ".png", renderer);
  poison->insertTail(game_config->getBasePath() +
                   "sprites/Battle/Pleps/hibernationplep_AA_A00.png", renderer);
  poison->insertTail(game_config->getBasePath() +
                   "sprites/Battle/Pleps/hibernationplep_AA_A01.png", renderer);
  poison->insertTail(game_config->getBasePath() +
                   "sprites/Battle/Pleps/hibernationplep_AA_A02.png", renderer);
  poison->insertTail(game_config->getBasePath() +
                   "sprites/Battle/Pleps/hibernationplep_AA_A03.png", renderer);
  for(uint16_t i = 0; i < 5; i++)
    poison->insertTail(game_config->getBasePath() +
                            "sprites/blank.png", renderer);
  poison->setAnimationTime(180);
  battle_display->setAilmentPlep(Infliction::POISON, poison);

  /* Background and bar */
  Sprite* background = new Sprite(
                base_path + "sprites/Battle/Backdrop/battlebg00.png", renderer);
  battle_display->setBackground(background);
  battle_display->setBattleBar(new Frame(
                           base_path + "sprites/Overlay/battle.png", renderer));
}

/* ------------------------------------------------------------------------- */
/* Main create call - first construction */
/* ------------------------------------------------------------------------- */
void TestBattle::create()
{
  /* Attribute Sets */
  stats_weak        = AttributeSet(1, true, false);
  stats_not_as_weak = AttributeSet(2, true, false);
  stats_normal      = AttributeSet(3, true, false);
  stats_medium      = AttributeSet(4, true, false);
  stats_top         = AttributeSet(5, true, false);
  stats_boss        = AttributeSet(6, true, false);
  stats_boss2       = AttributeSet(7, true, false);

  /* Actions */
  createActions();
  createSkills();
  createSkillSets();
  createClasses();
  createRaces();

  /* Inventories */
  pouch_foes = new Inventory(1000, "Der Pouch");
  pouch_friends = new Inventory(1001, "Teh Pouch");
}

/* ------------------------------------------------------------------------- */
/* Create actions */
/* ------------------------------------------------------------------------- */
void TestBattle::createActions()
{
  /* Alter Actions */
  act_alt.push_back(new Action("200,ALTER,,,,VITA,PC.10,AMOUNT.10,,99"));
  act_alt.push_back(new Action("201,ALTER,,,,,PC.25,AMOUNT.20,VITA,99"));
  act_alt.push_back(new Action("202,ALTER,,,,VITA,AMOUNT.-100,AMOUNT.20,VITA,100"));
  act_alt.push_back(new Action("203,ALTER,,,,,PC.-12,AMOUNT.20,VITA,100"));
  act_alt.push_back(new Action("204,ALTER,,,,QTDR,PC.10,AMOUNT.20,THAG,96"));
  act_alt.push_back(new Action("205,ALTER-FLIP,,,,QTDR,PC.10,AMOUNT.20,THAG,96"));
  act_alt.push_back(
            new Action("206,ALTER-FLIP,,,,VITA,AMOUNT.100,AMOUNT.30,VITA,100"));
  act_alt.push_back(new Action("207,ALTER,,,,PHFD,AMOUNT.10,PC.5,,100"));
  act_alt.push_back(new Action("208,ALTER,,,,MMNT,AMOUNT.10,PC.5,,100"));
  act_alt.push_back(new Action("209,ALTER,,,,VITA,PC.-100,AMOUNT.0,,100"));
  act_alt.push_back(new Action("210,ALTER,,,,PHAG,AMOUNT.10,PC.5,,100"));
  act_alt.push_back(new Action("211,ALTER,,,,LIMB,AMOUNT.10,PC.5,,100"));
  act_alt.push_back(new Action("212,ALTER,,,,,AMOUNT.-10,PC.5,MMNT,100"));

  /* Assign Actions */
  act_asn.push_back(new Action("300,ASSIGN,,,,VITA,AMOUNT.1,AMOUNT.0,,100"));
  act_asn.push_back(new Action("301,ASSIGN,,,,VITA,AMOUNT.0,AMOUNT.0,,100"));
  act_asn.push_back(new Action("302,ASSIGN,,,,,PC.20,PC.5,VITA,1"));
  act_asn.push_back(new Action("303,ASSIGN-FLIP,,,,VITA,PC.20,PC.5,QTDR,1"));
  act_asn.push_back(new Action("400,REVIVE,,,,,AMOUNT.50,AMOUNT.10,,80"));

  /* Damage actions */
  act_dmg.push_back(new Action("1,DAMAGE,,,,,AMOUNT.1,AMOUNT.1,,95"));
  act_dmg.push_back(new Action("2,DAMAGE,,,,VITA,AMOUNT.20,AMOUNT.5,,95"));
  act_dmg.push_back(new Action("3,DAMAGE,,,,VITA,AMOUNT.20,AMOUNT.5,,95"));
  act_dmg.push_back(new Action("4,DAMAGE,,,,,AMOUNT.20,AMOUNT.5,,95"));
  act_dmg.push_back(new Action("5,DAMAGE,,,,VITA,AMOUNT.60,AMOUNT.20,,95"));
  act_dmg.push_back(new Action("6,DAMAGE,,,,VITA,AMOUNT.75,AMOUNT.25,,95"));
  act_dmg.push_back(new Action("7,DAMAGE,,,,VITA,AMOUNT.90,AMOUNT.25,,95"));
  act_dmg.push_back(new Action("8,DAMAGE,,,,VITA,AMOUNT.100,AMOUNT.30,,95"));
  act_dmg.push_back(new Action("9,DAMAGE,,,,VITA,AMOUNT.300,AMOUNT.35,,95"));
  act_dmg.push_back(new Action("10,DAMAGE,,,,VITA,AMOUNT.500,AMOUNT.50,,95"));
  act_dmg.push_back(new Action("11,DAMAGE,,,,VITA,AMOUNT.40,PC.1,,95"));
  act_dmg.push_back(new Action("12,DAMAGE,,,,VITA,AMOUNT.110,PC.5,,95"));
  act_dmg.push_back(new Action("13,DAMAGE,,,,VITA,AMOUNT.60,PC.10,,95"));
  act_dmg.push_back(new Action("14,DAMAGE,,,,VITA,AMOUNT.75,PC.15,,95"));
  act_dmg.push_back(new Action("15,DAMAGE,,,,VITA,AMOUNT.100,PC.25,,95"));
  act_dmg.push_back(new Action("16,DAMAGE,,,,VITA,AMOUNT.150,PC.30,,95"));
  act_dmg.push_back(new Action("17,DAMAGE,,,,VITA,PC.2,PC.1,,95"));
  act_dmg.push_back(new Action("18,DAMAGE,,,,VITA,PC.5,PC.2,,95"));
  act_dmg.push_back(new Action("19,DAMAGE,,,,VITA,PC.10,PC.5,,95"));
  act_dmg.push_back(new Action("20,DAMAGE,,,,VITA,PC.10,PC.9,,95"));
  act_dmg.push_back(new Action("22,DAMAGE,,,,VITA,PC.10,PC.20,,95"));
  act_dmg.push_back(new Action("23,DAMAGE,,,,VITA,PC.15,PC.25,,95"));
  act_dmg.push_back(new Action("24,DAMAGE,,,,VITA,AMOUNT.20,PC.1,,10"));
  act_dmg.push_back(new Action("25,DAMAGE,,,,VITA,AMOUNT.500,AMOUNT.50,,100"));
  act_dmg.push_back(new Action("26,DAMAGE,,,,VITA,AMOUNT.75,AMOUNT.25,,55"));

  /* Inflict Actions */
  act_inf.push_back(new Action("500,INFLICT,4.7,,,POISON,,,VITA,90"));
  act_inf.push_back(new Action("501,INFLICT,1.4,,,BURN,AMOUNT.50,,VITA,90"));
  act_inf.push_back(new Action("502,INFLICT,2.6,,,SCALD,AMOUNT.40,,VITA,90"));
  act_inf.push_back(new Action("503,INFLICT,4.8,,,CHARR,AMOUNT.30,,VITA,90"));
  act_inf.push_back(new Action("504,INFLICT,4.4,,,BERSERK,AMOUNT.0,,VITA,95"));
  act_inf.push_back(new Action("505,INFLICT,2.3,,,CONFUSE,AMOUNT.50,,VITA,99"));
  act_inf.push_back(new Action("506,INFLICT,2.3,,,SILENCE,AMOUNT.60,,VITA,99"));
  act_inf.push_back(new Action("507,INFLICT,4.8,,,BUBBIFY,AMOUNT.70,,VITA,99"));
  act_inf.push_back(new Action("508,INFLICT,2.5,,,ALLATKBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("509,INFLICT,2.5,,,ALLDEFBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("510,INFLICT,2.5,,,PHYBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("511,INFLICT,2.5,,,THRBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("512,INFLICT,2.5,,,POLBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("513,INFLICT,2.5,,,PRIBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("514,INFLICT,2.5,,,CHGBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("515,INFLICT,2.5,,,CYBBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("516,INFLICT,2.5,,,NIHBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("517,INFLICT,2.5,,,LIMBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("518,INFLICT,2.5,,,UNBBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("519,INFLICT,2.5,,,VITBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("520,INFLICT,2.5,,,QDBUFF,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("521,INFLICT,5.5,,,DEATHTIMER,AMOUNT.0,,VITA,99"));
  act_inf.push_back(new Action("522,INFLICT,3.5,,,PARALYSIS,AMOUNT.30,,VITA,99"));
  act_inf.push_back(new Action("523,INFLICT,2.5,,,BLINDNESS,AMOUNT.20,,VITA,99"));
  act_inf.push_back(new Action("524,INFLICT,2.4,,,DREADSTRUCK,AMOUNT.25,,VITA,99"));
  act_inf.push_back(new Action("525,INFLICT,2.6,,,DREAMSNARE,AMOUNT.25,,VITA,99"));
  act_inf.push_back(new Action("526,INFLICT,4.8,,,MODULATE,AMOUNT.10,,VITA,99"));

  /* Relieve Actions */
  act_rlv.push_back(new Action("600,RELIEVE,,,,POISON,,,VITA,100"));
}

/* ------------------------------------------------------------------------- */
/* Create classes */
/* ------------------------------------------------------------------------- */
void TestBattle::createClasses()
{
  /* Arcadius LVL1 Class */
  class_arcadius1 = new Category(100, "Arcadius", "arcadius", stats_boss,
                                stats_boss2, getSkillSet(100));
  //class_arcadius1->setVitaRegenRate(RegenRate::WEAK);
  //class_arcadius1->setQDRegenRate(RegenRate::WEAK);
  class_arcadius1->setDescription("The one and only");
  class_arcadius1->setFlag(CategoryState::DEF_ENABLED, true);
  class_arcadius1->setFlag(CategoryState::GRD_ENABLED, true);
  class_arcadius1->setFlag(CategoryState::E_CLAWS, true);

  /* Aurora Agent Class */
  class_aurora_agent = new Category(120, "Agent", "agent", stats_normal,
                                    stats_top, getSkillSet(120));
  class_aurora_agent->setVitaRegenRate(RegenRate::WEAK);
  class_aurora_agent->setQDRegenRate(RegenRate::WEAK);
  class_aurora_agent->setDescription("A less than smart agent");
  class_aurora_agent->setFlag(CategoryState::DEF_ENABLED, true);
  class_aurora_agent->setFlag(CategoryState::GRD_ENABLED, true);
  class_aurora_agent->setFlag(CategoryState::E_SWORD, true);

  /* Aurora Heavy Classs */
  class_aurora_heavy = new Category(140, "Heavy", "heavy", stats_normal,
                                    stats_top, getSkillSet(140));
  //class_aurora_heavy->setVitaRegenRate(RegenRate::WEAK);
  class_aurora_heavy->setQDRegenRate(RegenRate::WEAK);
  class_aurora_heavy->setDescription("He weighs more than your house");
  class_aurora_heavy->setFlag(CategoryState::DEF_ENABLED, true);
  class_aurora_heavy->setFlag(CategoryState::GRD_ENABLED, true);
  class_aurora_heavy->setFlag(CategoryState::E_SWORD, true);

  /* Aurora Drone Class */
  class_aurora_drone = new Category(180, "Drone", "drone", stats_not_as_weak,
                                    stats_medium, getSkillSet(180));
  class_aurora_drone->setVitaRegenRate(RegenRate::WEAK);
  class_aurora_drone->setQDRegenRate(RegenRate::WEAK);
  class_aurora_drone->setDescription("Cannon fodder");
  class_aurora_drone->setFlag(CategoryState::DEF_ENABLED, true);
  class_aurora_drone->setFlag(CategoryState::GRD_ENABLED, true);
  class_aurora_drone->setFlag(CategoryState::E_SWORD, true);

  /* Aurora Engineer Class */
  class_aurora_engg = new Category(160, "Engineer", "engineer", stats_normal,
                                   stats_top, getSkillSet(160));
  class_aurora_engg->setVitaRegenRate(RegenRate::WEAK);
  //class_aurora_engg->setQDRegenRate(RegenRate::WEAK);
  class_aurora_engg->setDescription("An engineer and much smarter than you");
  class_aurora_engg->setFlag(CategoryState::DEF_ENABLED, true);
  class_aurora_engg->setFlag(CategoryState::GRD_ENABLED, true);
  class_aurora_engg->setFlag(CategoryState::E_SWORD, true);

  /* Player Class */
  class_player = new Category(200, "Player", "player", stats_normal, stats_top,
                              getSkillSet(200));
  //class_player->setVitaRegenRate(RegenRate::WEAK);
  class_player->setQDRegenRate(RegenRate::WEAK);
  class_player->setDescription("This is me and I am awesome");
  class_player->setFlag(CategoryState::DEF_ENABLED, true);
  class_player->setFlag(CategoryState::GRD_ENABLED, true);
  class_player->setFlag(CategoryState::E_CLAWS, true);

  /* Reverdile Class */
  class_reverdile = new Category(220, "Reverdile", "reverdile", stats_normal,
                                 stats_top, getSkillSet(220));
  //class_reverdile->setVitaRegenRate(RegenRate::WEAK);
  //class_reverdile->setQDRegenRate(RegenRate::WEAK);
  class_reverdile->setDescription("What is the master of crap called");
  class_reverdile->setFlag(CategoryState::DEF_ENABLED, true);
  class_reverdile->setFlag(CategoryState::GRD_ENABLED, true);
  class_reverdile->setFlag(CategoryState::E_CLAWS, true);

  /* Reverdling Class */
  class_reverdling = new Category(240, "Reverdling", "reverdling", stats_weak,
                                  stats_medium, getSkillSet(240));
  class_reverdling->setVitaRegenRate(RegenRate::WEAK);
  class_reverdling->setQDRegenRate(RegenRate::WEAK);
  class_reverdling->setDescription("He's about as useful as fried turd");
  class_reverdling->setFlag(CategoryState::DEF_ENABLED, true);
  class_reverdling->setFlag(CategoryState::GRD_ENABLED, true);
  class_reverdling->setFlag(CategoryState::E_CLAWS, true);
}

/* ------------------------------------------------------------------------- */
/* Create fonts */
/* ------------------------------------------------------------------------- */
void TestBattle::createFonts()
{
  if(game_config != NULL)
  {
    /* Try and create the new fonts */
    TTF_Font* regular_font = Text::createFont(game_config->getBasePath() +
                                              game_config->getFont(),
                                              16, TTF_STYLE_BOLD);

    /* If successful, insert the new fonts. Otherwise, delete */
    if(regular_font != NULL)
    {
      deleteFonts();
      font_normal = regular_font;
    }
    else
    {
      TTF_CloseFont(regular_font);
      regular_font = NULL;
    }
  }
}

/* ------------------------------------------------------------------------- */
/* Create menu */
/* ------------------------------------------------------------------------- */
void TestBattle::createMenu()
{
  for(uint8_t i = 0; i < kNUM_MENU_ITEMS; i++)
  {
    menu_items.push_back(new Text(font_normal));
    menu_items_sel.push_back(new Text(font_normal));
  }
  menu_index = 0;
}

/* ------------------------------------------------------------------------- */
/* Create person of type */
/* ------------------------------------------------------------------------- */
Person* TestBattle::createPerson(int id, TestPerson type,
                                 SDL_Renderer* renderer, uint8_t level,
                                 bool include_ai)
{
  Person* new_person = NULL;
  Sprite* as = NULL;
  Sprite* ds = NULL;
  Sprite* fp = NULL;
  Sprite* tp = NULL;

  /* Create person based on enum - custom logic based on type */
  if(type == ARCADIUS)
  {
    new_person = new Person(id, "Arcadius", race_bear, class_arcadius1);
    new_person->setCurves(Element::FOREST, ElementCurve::S,
                          Element::PHYSICAL, ElementCurve::S, true);

    tp = new Sprite(base_path + "sprites/Battle/Battle_Persons/arcadius.png",
                    renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/arcadius.png",
                    renderer);
  }
  else if(type == AURORAAGENT)
  {
    new_person = new Person(id, "Agent", race_human, class_aurora_agent);
    new_person->setCurves(Element::ELECTRIC, ElementCurve::C,
                          Element::PHYSICAL, ElementCurve::C, true);

    tp = new Sprite(base_path + "sprites/Battle/Battle_Persons/auroraagent.png",
                    renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/auroraagent.png",
                    renderer);
  }
  else if(type == AURORAHEAVY)
  {
    new_person = new Person(id, "Heavy", race_human, class_aurora_heavy);
    new_person->setCurves(Element::ELECTRIC, ElementCurve::A,
                          Element::PHYSICAL, ElementCurve::A, true);

    tp = new Sprite(base_path + "sprites/Battle/Battle_Persons/auroraheavy.png",
                    renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/auroraheavy.png",
                    renderer);
  }
  else if(type == AURORAENGG)
  {
    new_person = new Person(id, "Engineer", race_human, class_aurora_engg);
    new_person->setCurves(Element::ELECTRIC, ElementCurve::B,
                          Element::PHYSICAL, ElementCurve::B, true);

    tp = new Sprite(
              base_path + "sprites/Battle/Battle_Persons/auroraengineer.png",
              renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/auroraengineer.png",
                    renderer);
  }
  else if(type == AURORADRONE)
  {
    new_person = new Person(id, "Drone", race_human, class_aurora_drone);
    new_person->setCurves(Element::ELECTRIC, ElementCurve::D,
                          Element::PHYSICAL, ElementCurve::D, true);

    tp = new Sprite(base_path + "sprites/Battle/Battle_Persons/auroradrone.png",
                    renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/auroradrone.png",
                    renderer);
  }
  else if(type == REVERDILE)
  {
    new_person = new Person(id, "Reverdile", race_treefolk, class_reverdile);
    new_person->setCurves(Element::FOREST, ElementCurve::C,
                          Element::PHYSICAL, ElementCurve::C, true);

    tp = new Sprite(base_path + "sprites/Battle/Battle_Persons/reverdile.png",
                    renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/treething.png",
                    renderer);
  }
  else if(type == REVERDLING)
  {
    new_person = new Person(id, "Reverdling", race_treefolk, class_reverdling);
    new_person->setCurves(Element::FOREST, ElementCurve::D,
                          Element::PHYSICAL, ElementCurve::D, true);

    tp = new Sprite(base_path + "sprites/Battle/Battle_Persons/reverdling.png",
                    renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/reverdling.png",
                    renderer);
  }
  else if(type == PLAYER)
  {
    new_person = new Person(id, "Player", race_bear, class_player);
    new_person->setCurves(Element::FOREST, ElementCurve::C,
                          Element::PHYSICAL, ElementCurve::C, true);

    fp = new Sprite(base_path + "sprites/Battle/Backs/player0.png", renderer);
    ds = new Sprite(base_path + "sprites/Overlay/DialogChar/player.png",
                    renderer);
    as = new Sprite(base_path + "sprites/Battle/Backs/player1.png", renderer);
  }

  /* Common person logic - why build a setExp(int) function when you
   * can do this wonderful code =) */
  new_person->loseExp(new_person->getExpAt(new_person->getLevel()));
  new_person->addExp(new_person->getExpAt(level));

  if(include_ai)
  {
    AIModule* ai_module = new AIModule();
    ai_module->setParent(new_person);
    new_person->setAI(ai_module);
  }
  new_person->setSprites(fp, tp, ds, as);

  return new_person;
}

/* ------------------------------------------------------------------------- */
/* Create races */
/* ------------------------------------------------------------------------- */
void TestBattle::createRaces()
{
  /* Aurora Race */
  race_aurora = new Category(1060, "Aurora", "aurora", stats_normal, stats_top,
                             getSkillSet(1060));
  race_aurora->setVitaRegenRate(RegenRate::WEAK);
  race_aurora->setQDRegenRate(RegenRate::WEAK);
  race_aurora->setDescription("A class of intelligent bots");
  race_aurora->setFlag(CategoryState::DEF_ENABLED, true);
  race_aurora->setFlag(CategoryState::GRD_ENABLED, true);
  race_aurora->setFlag(CategoryState::E_SWORD, true);

  /* Bear Race */
  race_bear = new Category(1020, "Bear", "bear", stats_normal, stats_top,
                           getSkillSet(1020));
  //race_bear->setVitaRegenRate(RegenRate::WEAK);
  race_bear->setQDRegenRate(RegenRate::WEAK);
  race_bear->setDescription("A sentient and intelligent bear");
  race_bear->setFlag(CategoryState::DEF_ENABLED, true);
  race_bear->setFlag(CategoryState::GRD_ENABLED, true);
  race_bear->setFlag(CategoryState::E_SWORD, true);

  /* Human Race */
  race_human = new Category(1000, "Human", "human", stats_normal, stats_top,
                            getSkillSet(1000));
  race_human->setVitaRegenRate(RegenRate::WEAK);
  race_human->setQDRegenRate(RegenRate::WEAK);
  race_human->setDescription("A regular old joe from Earth");
  race_human->setFlag(CategoryState::DEF_ENABLED, true);
  race_human->setFlag(CategoryState::GRD_ENABLED, true);
  race_human->setFlag(CategoryState::E_SWORD, true);

  /* Treefolk Race */
  race_treefolk = new Category(1040, "Treefolk", "treefolk", stats_normal,
                               stats_top, getSkillSet(1040));
  //race_treefolk->setVitaRegenRate(RegenRate::WEAK);
  //race_treefolk->setQDRegenRate(RegenRate::WEAK);
  race_treefolk->setDescription("They have long roots into society");
  race_treefolk->setFlag(CategoryState::DEF_ENABLED, true);
  race_treefolk->setFlag(CategoryState::GRD_ENABLED, true);
  //race_treefolk->setFlag(CategoryState::E_SWORD, true);
}

/* ------------------------------------------------------------------------- */
/* Create skills */
/* ------------------------------------------------------------------------- */
void TestBattle::createSkills()
{
  /* Light Push */
  Skill* light_push = new Skill(100, "Light Push", ActionScope::ONE_ENEMY,
                                act_dmg[0], 95, 0);
  light_push->setDescription("A weak, physical hit with huge cool down");
  light_push->setPrimary(Element::PHYSICAL);
  light_push->setFlag(SkillFlags::OFFENSIVE);
  light_push->setCooldown(5);
  skills.push_back(light_push);

  /* Light Shot */
  Skill* light_shot = new Skill(120, "Light Shot", ActionScope::ONE_ENEMY,
                                act_dmg[5], 95, 0);
  light_shot->setDescription("A standard, electric hit against a single foe");
  light_shot->setPrimary(Element::ELECTRIC);
  light_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(light_shot);

  /* Prismatic Shot */
  Skill* prismatic_shot = new Skill(121, "Prismatic Shot",
                                    ActionScope::ONE_ENEMY, act_dmg[4], 95, 15);
  prismatic_shot->addAction(act_dmg[4]);
  prismatic_shot->addAction(act_dmg[4]);
  prismatic_shot->setDescription("A 3 strike electric hit against a foe");
  prismatic_shot->setPrimary(Element::ELECTRIC);
  prismatic_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(prismatic_shot);

  /* Rail Shot */
  Skill* rail_shot = new Skill(140, "Rail Shot", ActionScope::ONE_ENEMY,
                               act_dmg[14], 95, 0);
  rail_shot->setDescription("A heavy damage electric hit against a foe");
  rail_shot->setPrimary(Element::ELECTRIC);
  rail_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(rail_shot);

  /* Shatter Shot */
  Skill* shatter_shot = new Skill(141, "ShatterShot", ActionScope::ALL_ENEMIES,
                                  act_dmg[15], 95, 15);
  shatter_shot->addAction(act_dmg[22]);
  shatter_shot->setDescription("A heavy shatter shot with potential splash");
  shatter_shot->setPrimary(Element::ELECTRIC);
  shatter_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(shatter_shot);

  /* Engineer Upgrade */
  Skill* engg_upgrade = new Skill(160, "Upgrade",
                                  ActionScope::ONE_ALLY_NOT_USER, act_alt[7],
                                  100, 5);
  engg_upgrade->addAction(act_alt[8]);
  engg_upgrade->setDescription("Team upgrade to ally for defense and speed");
  engg_upgrade->setPrimary(Element::ELECTRIC);
  engg_upgrade->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(engg_upgrade);

  /* Static Shot */
  Skill* static_shot = new Skill(180, "Static Shot", ActionScope::ONE_ENEMY,
                                 act_dmg[5], 95, 0);
  static_shot->setDescription("An electric shot from drone against a foe");
  static_shot->setPrimary(Element::ELECTRIC);
  static_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(static_shot);

  /* Locked Shot */
  Skill* locked_shot = new Skill(181, "Locked Shot", ActionScope::ONE_ENEMY,
                                 act_dmg[23], 100, 25);
  locked_shot->setDescription("A perfect shot against a foe");
  locked_shot->setPrimary(Element::ELECTRIC);
  locked_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(locked_shot);

  /* Detonate */
  Skill* detonate = new Skill(182, "Detonate", ActionScope::ALL_TARGETS,
                              act_dmg[8], 90, 15);
  detonate->addAction(act_alt[9]);
  detonate->setDescription("Inflict massive damage on everyone while dying.");
  detonate->setPrimary(Element::ELECTRIC);
  detonate->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(detonate);

  /* Hidden Amplification */
  Skill* hidden_amp = new Skill(200, "Hidden Amplification", ActionScope::USER,
                                act_alt[10], 90, 5);
  hidden_amp->addAction(act_alt[7]);
  hidden_amp->setDescription("Increase attack and defense of self");
  hidden_amp->setPrimary(Element::PHYSICAL);
  hidden_amp->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(hidden_amp);

  /* Surge of Will */
  // TODO: HOW TO DO MULTI-TURN??
  // You can't. I think it was supposed to be cooldown? Else it's another
  // large scope change for no gain
  //Skill* surge_will = new Skill(201, "Surge of Will", ActionScope::ALL_ALLIES,
  //                              act_alt[0], 90, 25);

  /* Canopy */
  Skill* canopy = new Skill(220, "Canopy", ActionScope::ALL_ALLIES,
                            act_alt[7], 90, 5);
  canopy->setDescription("Entire team DEF up.");
  canopy->setPrimary(Element::PHYSICAL);
  canopy->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(canopy);

  /* Updraft */
  Skill* updraft = new Skill(221, "Updraft", ActionScope::ONE_ALLY_NOT_USER,
                             act_alt[8], 90, 10);
  updraft->setDescription("Ally speed up");
  updraft->setPrimary(Element::PHYSICAL);
  updraft->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(updraft);

  /* Chlorophona */
  Skill* chlorophona = new Skill(222, "Chlorophona", ActionScope::ALL_ALLIES,
                                 act_alt[0], 90, 15);
  chlorophona->setDescription("Team health up");
  chlorophona->setPrimary(Element::PHYSICAL);
  chlorophona->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(chlorophona);

  /* Numbing Sting */
  Skill* numbing = new Skill(240, "Numbing Sting", ActionScope::ONE_ENEMY,
                             act_inf[22], 90, 5);
  numbing->setDescription("Paralysis Inflict");
  numbing->setPrimary(Element::PHYSICAL);
  //numbing->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(numbing);

  /* Hypnotic Sting */
  Skill* hypnotic = new Skill(241, "Hypnotic Sting", ActionScope::ONE_ENEMY,
                              act_inf[25], 90, 5);
  hypnotic->setDescription("Hypnotic Inflict");
  hypnotic->setPrimary(Element::PHYSICAL);
  //hypnotic->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(hypnotic);

  /* Toxic Sting */
  Skill* toxic = new Skill(242, "Toxic Sting", ActionScope::ONE_ENEMY,
                           act_inf[0], 90, 5);
  toxic->setDescription("Poision Inflict");
  toxic->setPrimary(Element::PHYSICAL);
  toxic->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(toxic);

  /* Befuddling Sting */
  Skill* befuddling = new Skill(243, "Befuddling Sting", ActionScope::ONE_ENEMY,
                                act_inf[5], 90, 5);
  befuddling->setDescription("Confusion Inflict");
  befuddling->setPrimary(Element::PHYSICAL);
  //befuddling->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(befuddling);

  /* Cunning */
  Skill* cunning = new Skill(1000, "Cunning", ActionScope::USER,
                             act_alt[7], 90, 5);
  cunning->addAction(act_alt[11]);
  cunning->setDescription("Self dodge and defense up");
  cunning->setPrimary(Element::PHYSICAL);
  cunning->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(cunning);

  /* Strike */
  Skill* strike = new Skill(1001, "Strike", ActionScope::ONE_ENEMY,
                            act_dmg[11], 90, 15);
  strike->setDescription("Physical strike against one foe");
  strike->setPrimary(Element::PHYSICAL);
  strike->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(strike);

  /* Paw Strike */
  Skill* paw_strike = new Skill(1020, "Paw Strike", ActionScope::ONE_ENEMY,
                                act_dmg[10], 95, 0);
  paw_strike->setDescription("A standard, physical hit against a single foe");
  paw_strike->setPrimary(Element::PHYSICAL);
  paw_strike->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(paw_strike);

  /* Maul */
  Skill* maul = new Skill(1021, "Maul", ActionScope::ONE_ENEMY,
                          act_dmg[13], 95, 10);
  maul->setDescription("A stronger, physical hit against a single foe");
  maul->setPrimary(Element::PHYSICAL);
  maul->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(maul);

  /* Multi-Strike */
  Skill* multi_strike = new Skill(1022, "Multi-Strike", ActionScope::ONE_ENEMY,
                                  act_dmg[24], 90, 15);
  multi_strike->addAction(act_dmg[24]);
  multi_strike->addAction(act_dmg[24]);
  multi_strike->addAction(act_dmg[24]);
  multi_strike->setDescription("A multi strike hit (4) against a foe");
  multi_strike->setPrimary(Element::PHYSICAL);
  multi_strike->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(multi_strike);

  /* Hibernate */
  Skill* hibernate = new Skill(1023, "Hibernate", ActionScope::USER,
                               act_alt[0], 100, 5);
  hibernate->setDescription("Heal self");
  hibernate->setPrimary(Element::PHYSICAL);
  hibernate->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(hibernate);

  /* Ensnare */
  Skill* ensnare = new Skill(1040, "Ensnare", ActionScope::ONE_ENEMY,
                             act_dmg[0], 80, 5);
  ensnare->addAction(act_alt[12]);
  ensnare->setDescription("Damage enemy and slow down");
  ensnare->setPrimary(Element::FOREST);
  ensnare->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(ensnare);

  /* Enrich */
  Skill* enrich = new Skill(1041, "Enrich", ActionScope::USER,
                            act_alt[0], 100, 5);
  enrich->setDescription("Small amount of heal to self");
  enrich->setPrimary(Element::FOREST);
  enrich->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(enrich);
}

/* ------------------------------------------------------------------------- */
/* Create Skill Sets */
/* ------------------------------------------------------------------------- */
void TestBattle::createSkillSets()
{
  /* ---- CLASSES ---- */

  /* Arcadius Class */
  SkillSet* set_arcadius = new SkillSet(100);
  set_arcadius->addSkill(getSkill(100), 1);
  skillsets.push_back(set_arcadius);

  /* Aurora Agent Class */
  SkillSet* set_agent = new SkillSet(120);
  set_agent->addSkill(getSkill(120), 1);
  set_agent->addSkill(getSkill(121), 12);
  skillsets.push_back(set_agent);

  /* Aurora Heavy Class */
  SkillSet* set_heavy = new SkillSet(140);
  set_heavy->addSkill(getSkill(140), 1);
  set_heavy->addSkill(getSkill(141), 12);
  skillsets.push_back(set_heavy);

  /* Aurora Engineer Class */
  SkillSet* set_engineer = new SkillSet(160);
  set_engineer->addSkill(getSkill(160), 1);
  set_engineer->addSkill(getSkill(120), 12);
  skillsets.push_back(set_engineer);

  /* Aurora Drone Class */
  SkillSet* set_drone = new SkillSet(180);
  set_drone->addSkill(getSkill(180), 1);
  set_drone->addSkill(getSkill(181), 5);
  set_drone->addSkill(getSkill(182), 13);
  skillsets.push_back(set_drone);

  /* Player Class */
  SkillSet* set_player = new SkillSet(200);
  set_player->addSkill(getSkill(200), 1);
  set_player->addSkill(getSkill(201), 15);
  skillsets.push_back(set_player);

  /* Reverdile */
  SkillSet* set_reverdile = new SkillSet(220);
  set_reverdile->addSkill(getSkill(220), 1);
  set_reverdile->addSkill(getSkill(221), 5);
  set_reverdile->addSkill(getSkill(222), 8);
  skillsets.push_back(set_reverdile);

  /* Reverdling */
  SkillSet* set_reverdling = new SkillSet(240);
  set_reverdling->addSkill(getSkill(240), 1);
  set_reverdling->addSkill(getSkill(241), 1);
  set_reverdling->addSkill(getSkill(242), 1);
  set_reverdling->addSkill(getSkill(243), 1);
  skillsets.push_back(set_reverdling);

  /* ---- RACES ---- */

  /* Human Race */
  SkillSet* set_human = new SkillSet(1000);
  set_human->addSkill(getSkill(1000), 1);
  set_human->addSkill(getSkill(1001), 6);
  skillsets.push_back(set_human);

  /* Bear Race */
  SkillSet* set_bear = new SkillSet(1020);
  set_bear->addSkill(getSkill(1020), 1);
  set_bear->addSkill(getSkill(1021), 5);
  set_bear->addSkill(getSkill(1022), 8);
  set_bear->addSkill(getSkill(1023), 13);
  skillsets.push_back(set_bear);

  /* Treefolk Race */
  SkillSet* set_treefolk = new SkillSet(1040);
  set_treefolk->addSkill(getSkill(1040), 1);
  set_treefolk->addSkill(getSkill(1041), 6);
  skillsets.push_back(set_treefolk);

  /* Aurora Race */
  SkillSet* set_aurora = new SkillSet(1060);
  skillsets.push_back(set_aurora);

  /* ---- BUBBIFIED SKILL SET ---- */
  SkillSet* set_bubbified = new SkillSet(1);
  set_bubbified->addSkill(getSkill(1020), 1);
  skillsets.push_back(set_bubbified);
}

/* ------------------------------------------------------------------------- */
/* Delete actions */
/* ------------------------------------------------------------------------- */
void TestBattle::deleteActions()
{
  /* Alter Actions */
  for(uint8_t i = 0; i < act_alt.size(); i++)
    delete act_alt[i];
  act_alt.clear();

  /* Assign Actions */
  for(uint8_t i = 0; i < act_asn.size(); i++)
    delete act_asn[i];
  act_asn.clear();

  /* Damage Actions */
  for(uint8_t i = 0; i < act_dmg.size(); i++)
    delete act_dmg[i];
  act_dmg.clear();

  /* Inflict Actions */
  for(uint8_t i = 0; i < act_inf.size(); i++)
    delete act_inf[i];
  act_inf.clear();

  /* Relieve Actions */
  for(uint8_t i = 0; i < act_rlv.size(); i++)
    delete act_rlv[i];
  act_rlv.clear();
}

/* ------------------------------------------------------------------------- */
/* Delete classes */
/* ------------------------------------------------------------------------- */
void TestBattle::deleteClasses()
{
  delete class_arcadius1;
  class_arcadius1 = NULL;

  delete class_aurora_agent;
  class_aurora_agent = NULL;

  delete class_aurora_heavy;
  class_aurora_heavy = NULL;

  delete class_aurora_drone;
  class_aurora_drone = NULL;

  delete class_aurora_engg;
  class_aurora_engg = NULL;

  delete class_player;
  class_player = NULL;

  delete class_reverdile;
  class_reverdile = NULL;

  delete class_reverdling;
  class_reverdling = NULL;
}

/* ------------------------------------------------------------------------- */
/* Delete fonts */
/* ------------------------------------------------------------------------- */
void TestBattle::deleteFonts()
{
  TTF_CloseFont(font_normal);
  font_normal = NULL;
}

/* ------------------------------------------------------------------------- */
/* Delete menu */
/* ------------------------------------------------------------------------- */
void TestBattle::deleteMenu()
{
  for(uint8_t i = 0; i < menu_items.size(); i++)
  {
    delete menu_items[i];
    delete menu_items_sel[i];
  }
  menu_items.clear();
  menu_items_sel.clear();
}

/* ------------------------------------------------------------------------- */
/* Delete races */
/* ------------------------------------------------------------------------- */
void TestBattle::deleteRaces()
{
  delete race_aurora;
  race_aurora = NULL;

  delete race_bear;
  race_bear = NULL;

  delete race_human;
  race_human = NULL;

  delete race_treefolk;
  race_treefolk = NULL;
}

/* ------------------------------------------------------------------------- */
/* Delete skills */
/* ------------------------------------------------------------------------- */
void TestBattle::deleteSkills()
{
  /* Delete entire set */
  for(uint16_t i = 0; i < skills.size(); i++)
    delete skills[i];
  skills.clear();
}

/* ------------------------------------------------------------------------- */
/* Delete skill sets */
/* ------------------------------------------------------------------------- */
void TestBattle::deleteSkillSets()
{
  /* Delete entire set */
  for(uint16_t i = 0; i < skillsets.size(); i++)
    delete skillsets[i];
  skillsets.clear();
}

/* ------------------------------------------------------------------------- */
/* Main destruction call - ending */
/* ------------------------------------------------------------------------- */
void TestBattle::destroy()
{
  mode = NONE;

  /* Delete battle and information */
  destroyBattle();

  /* Inventories */
  delete pouch_foes;
  delete pouch_friends;

  /* Delete skill information */
  deleteClasses();
  deleteRaces();
  deleteSkillSets();
  deleteSkills();
  deleteActions();

  /* Delete fonts and menus */
  deleteMenu();
  deleteFonts();
}

/* Battle destruction */
void TestBattle::destroyBattle()
{
  if(battle_logic != NULL)
  {
    battle_display->unsetBattle();
    delete battle_logic;
    battle_logic = NULL;
  }

  /* Foe party list delete */
  if(party_foes != NULL)
  {
    std::vector<Person*> foe_list = party_foes->getMembers();
    for(uint8_t i = 0; i < foe_list.size(); i++)
      delete foe_list[i];
    foe_list.clear();

    delete party_foes;
    party_foes = NULL;
  }

  /* Friend party list delete */
  if(party_friends != NULL)
  {
    std::vector<Person*> friend_list = party_friends->getMembers();
    for(uint8_t i = 0; i < friend_list.size(); i++)
      delete friend_list[i];
    friend_list.clear();

    delete party_friends;
    party_friends = NULL;
  }
}

/* ------------------------------------------------------------------------- */
/* Get call for Skill by ID */
/* ------------------------------------------------------------------------- */
Skill* TestBattle::getSkill(int id)
{
  Skill* found_skill = NULL;

  for(uint16_t i = 0; i < skills.size(); i++)
    if(skills[i]->getID() == id)
      found_skill = skills[i];

  return found_skill;
}

/* ------------------------------------------------------------------------- */
/* Get call for Skill Set by ID */
/* ------------------------------------------------------------------------- */
SkillSet* TestBattle::getSkillSet(int id)
{
  SkillSet* found_set = NULL;

  for(uint16_t i = 0; i < skillsets.size(); i++)
    if(skillsets[i]->getID() == id)
      found_set = skillsets[i];

  return found_set;
}

/* ------------------------------------------------------------------------- */
/* Set up the battle */
/* ------------------------------------------------------------------------- */
void TestBattle::initBattle(SDL_Renderer* renderer)
{
  MenuItems ref = (MenuItems)menu_index;

  /* Set up FRIEND party */
  Person* player = createPerson(100, PLAYER, renderer, lvl_friend, false);
  party_friends = new Party(200, player, PartyType::SLEUTH, 5, pouch_foes);

  /* Set up FOE party based on menu index */
  if(ref == AC)
  {
    Person* arc = createPerson(300, ARCADIUS, renderer, lvl_foe);
    party_foes = new Party(400, arc, PartyType::REGULAR_FOE, 5, pouch_foes);
  }
  else if(ref == AA)
  {
    Person* aa = createPerson(300, AURORAAGENT, renderer, lvl_foe);
    party_foes = new Party(400, aa, PartyType::REGULAR_FOE, 5, pouch_foes);
  }
  else if(ref == AAx2)
  {
    Person* aa1 = createPerson(300, AURORAAGENT, renderer, lvl_foe);
    Person* aa2 = createPerson(301, AURORAAGENT, renderer, lvl_foe);

    party_foes = new Party(400, aa1, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(aa2);
  }
  else if(ref == AAx5)
  {
    Person* aa1 = createPerson(300, AURORAAGENT, renderer, lvl_foe);
    Person* aa2 = createPerson(301, AURORAAGENT, renderer, lvl_foe);
    Person* aa3 = createPerson(302, AURORAAGENT, renderer, lvl_foe);
    Person* aa4 = createPerson(303, AURORAAGENT, renderer, lvl_foe);
    Person* aa5 = createPerson(304, AURORAAGENT, renderer, lvl_foe);

    party_foes = new Party(400, aa1, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(aa2);
    party_foes->addMember(aa3);
    party_foes->addMember(aa4);
    party_foes->addMember(aa5);
  }
  else if(ref == AAnAH)
  {
    Person* aa = createPerson(300, AURORAAGENT, renderer, lvl_foe);
    Person* ah = createPerson(301, AURORAHEAVY, renderer, lvl_foe);

    party_foes = new Party(400, aa, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(ah);
  }
  else if(ref == AH)
  {
    Person* ah = createPerson(300, AURORAHEAVY, renderer, lvl_foe);
    party_foes = new Party(400, ah, PartyType::REGULAR_FOE, 5, pouch_foes);
  }
  else if(ref == AEnAD)
  {
    Person* ae = createPerson(300, AURORAENGG, renderer, lvl_foe);
    Person* ad = createPerson(301, AURORADRONE, renderer, lvl_foe);

    party_foes = new Party(400, ae, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(ad);
  }
  else if(ref == AEnADx4)
  {
    Person* ae = createPerson(300, AURORAENGG, renderer, lvl_foe);
    Person* ad1 = createPerson(301, AURORADRONE, renderer, lvl_foe);
    Person* ad2 = createPerson(302, AURORADRONE, renderer, lvl_foe);
    Person* ad3 = createPerson(303, AURORADRONE, renderer, lvl_foe);
    Person* ad4 = createPerson(304, AURORADRONE, renderer, lvl_foe);

    party_foes = new Party(400, ae, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(ad1);
    party_foes->addMember(ad2);
    party_foes->addMember(ad3);
    party_foes->addMember(ad4);
  }
  else if(ref == AAnAHnADnAE)
  {
    Person* aa = createPerson(300, AURORAAGENT, renderer, lvl_foe);
    Person* ah = createPerson(301, AURORAHEAVY, renderer, lvl_foe);
    Person* ad = createPerson(302, AURORADRONE, renderer, lvl_foe);
    Person* ae = createPerson(303, AURORAENGG, renderer, lvl_foe);

    party_foes = new Party(400, aa, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(ah);
    party_foes->addMember(ad);
    party_foes->addMember(ae);
  }
  else if(ref == REnRG)
  {
    Person* re = createPerson(300, REVERDILE, renderer, lvl_foe);
    Person* rg = createPerson(301, REVERDLING, renderer, lvl_foe);

    party_foes = new Party(400, re, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(rg);
  }
  else if(ref == REnRGx2)
  {
    Person* re = createPerson(300, REVERDILE, renderer, lvl_foe);
    Person* rg1 = createPerson(301, REVERDLING, renderer, lvl_foe);
    Person* rg2 = createPerson(302, REVERDLING, renderer, lvl_foe);

    party_foes = new Party(400, re, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(rg1);
    party_foes->addMember(rg2);
  }
  else if(ref == REnRGx4)
  {
    Person* re = createPerson(300, REVERDILE, renderer, lvl_foe);
    Person* rg1 = createPerson(301, REVERDLING, renderer, lvl_foe);
    Person* rg2 = createPerson(302, REVERDLING, renderer, lvl_foe);
    Person* rg3 = createPerson(303, REVERDLING, renderer, lvl_foe);
    Person* rg4 = createPerson(304, REVERDLING, renderer, lvl_foe);

    party_foes = new Party(400, re, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(rg1);
    party_foes->addMember(rg2);
    party_foes->addMember(rg3);
    party_foes->addMember(rg4);
  }
  else if(ref == RG)
  {
    Person* rg = createPerson(300, REVERDLING, renderer, lvl_foe);
    party_foes = new Party(400, rg, PartyType::REGULAR_FOE, 5, pouch_foes);
  }
  else if(ref == RGx2)
  {
    Person* rg1 = createPerson(300, REVERDLING, renderer, lvl_foe);
    Person* rg2 = createPerson(301, REVERDLING, renderer, lvl_foe);

    party_foes = new Party(400, rg1, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(rg2);
  }
  else if(ref == RGx5)
  {
    Person* rg1 = createPerson(300, REVERDLING, renderer, lvl_foe);
    Person* rg2 = createPerson(301, REVERDLING, renderer, lvl_foe);
    Person* rg3 = createPerson(302, REVERDLING, renderer, lvl_foe);
    Person* rg4 = createPerson(303, REVERDLING, renderer, lvl_foe);
    Person* rg5 = createPerson(304, REVERDLING, renderer, lvl_foe);

    party_foes = new Party(400, rg1, PartyType::REGULAR_FOE, 5, pouch_foes);
    party_foes->addMember(rg2);
    party_foes->addMember(rg3);
    party_foes->addMember(rg4);
    party_foes->addMember(rg5);
  }

  /* Battle Prep */
  for(auto& member : party_friends->getMembers())
    member->battlePrep();
  for(auto& member : party_foes->getMembers())
    member->battlePrep();

  /* Set up battle */
  battle_logic = new Battle(party_friends, party_foes, getSkillSet(1), NULL);
  battle_display->setBattle(battle_logic, renderer);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* ------------------------------------------------------------------------- */
/* The key up and down events to be handled by the class */
/* ------------------------------------------------------------------------- */
bool TestBattle::keyDownEvent(SDL_KeyboardEvent event)
{
  if(mode == TEST_BATTLE)
  {
    if(battle_display->getRenderingState() == battle_logic->getTurnState())
      return battle_logic->keyDownEvent(event);
  }
  else
  {
    /* Exit the test battle: finished processing */
    if(event.keysym.sym == SDLK_UP)
    {
      if(mode == SCENARIO)
      {
        if(menu_index > 0)
          menu_index--;
      }
      else if(mode == FRIEND_LVL)
      {
        if(lvl_friend < kLVL_MAX)
          lvl_friend++;
      }
      else if(mode == FOE_LVL)
      {
        if(lvl_foe < kLVL_MAX)
          lvl_foe++;
      }
    }
    else if(event.keysym.sym == SDLK_DOWN)
    {
      if(mode == SCENARIO)
      {
        if((menu_index + 1) < kNUM_MENU_ITEMS)
          menu_index++;
      }
      else if(mode == FRIEND_LVL)
      {
        if(lvl_friend > 1)
          lvl_friend--;
      }
      else if(mode == FOE_LVL)
      {
        if(lvl_foe > 1)
          lvl_foe--;
      }
    }
    else if(event.keysym.sym == SDLK_RETURN)
    {
      if(mode == SCENARIO)
      {
        if(kMENU_ITEMS[menu_index] == "Exit")
        {
          return true;
        }
        else
        {
          mode = FRIEND_LVL;
        }
      }
      else if(mode == FRIEND_LVL)
      {
        mode = FOE_LVL;
      }
      else if(mode == FOE_LVL)
      {
        battle_start = true;
      }
    }
    else if(event.keysym.sym == SDLK_ESCAPE)
    {
      if(mode == SCENARIO)
      {
        menu_index = kNUM_MENU_ITEMS - 1;
      }
      else if(mode == FRIEND_LVL)
      {
        mode = SCENARIO;
      }
      else if(mode == FOE_LVL)
      {
        mode = FRIEND_LVL;
      }
    }
  }

  return false;
}

/* ------------------------------------------------------------------------- */
/* The key up and down events to be handled by the class */
/* ------------------------------------------------------------------------- */
void TestBattle::keyUpEvent(SDL_KeyboardEvent event)
{
  (void)event;
}

/* ------------------------------------------------------------------------- */
/* Renders the title screen */
/* ------------------------------------------------------------------------- */
bool TestBattle::render(SDL_Renderer* renderer)
{
  if(renderer != NULL)
  {
    /* First run rendering */
    if(first_run)
    {
      buildBattleDisplay(renderer);
      first_run = false;
    }

    /* Battle start first rendering */
    if(battle_start)
    {
      initBattle(renderer);
      mode = TEST_BATTLE;
      battle_start = false;
    }

    /* Rendering primary */
    if(mode == TEST_BATTLE)
    {
      battle_display->render(renderer);
    }
    else
    {
      /* Render text */
      for(uint8_t i = 0; i < menu_items.size(); i++)
      {
        if(i == menu_index)
          menu_items_sel[i]->render(renderer, 75, 40 + 40 * i);
        else
          menu_items[i]->render(renderer, 75, 40 + 40 * i);
      }

      /* Render friend level */
      if(mode == FRIEND_LVL || mode == FOE_LVL)
      {
        Text t(font_normal);
        t.setText(renderer, kMENU_FRIEND + std::to_string(lvl_friend),
                  {255,255,255,255});
        t.render(renderer, 600, 40);
      }

      /* Render foe level */
      if(mode == FOE_LVL)
      {
        Text t(font_normal);
        t.setText(renderer, kMENU_FOE + std::to_string(lvl_foe),
                  {255,255,255,255});
        t.render(renderer, 600, 140);
      }
    }
  }

  return true;
}

/* ------------------------------------------------------------------------- */
/* Set the running configuration, from the options class */
/* ------------------------------------------------------------------------- */
bool TestBattle::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    game_config = running_config;
    base_path   = game_config->getBasePath();

    /* Battle configuration setup */
    if(battle_display != NULL)
      battle_display->setConfiguration(running_config);

    /* Text setup */
    deleteMenu();
    deleteFonts();
    createFonts();
    createMenu();

    return true;
  }

  return false;
}

/* ------------------------------------------------------------------------- */
/* Updates the game state */
/* ------------------------------------------------------------------------- */
bool TestBattle::update(int cycle_time)
{
  if(mode == TEST_BATTLE)
  {
    battle_logic->update(cycle_time);

    if(battle_logic->getBattleFlag(CombatState::OUTCOME_PERFORMED))
    {
      mode = SCENARIO;
      destroyBattle();
    }
    else
    {
      battle_display->update(cycle_time);
    }
  }

  (void)cycle_time;
  return false;
}
