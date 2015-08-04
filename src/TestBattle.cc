/*******************************************************************************
 * Class Name: TestBattle
 * Date Created: July 31, 2015
 * Inheritance: none
 * Description: This class is the testing interface for battle scenarios.
 ******************************************************************************/
#include "TestBattle.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
TestBattle::TestBattle(Options* running_config)
{
  base_path = "";
  battle_display = new BattleDisplay(running_config);
  battle_logic = NULL;
  first_run = true;
  game_config = NULL;

  setConfiguration(running_config);
  create();
}

/* Destructor function */
TestBattle::~TestBattle()
{
  destroy();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Builds the ailment frames */
void TestBattle::buildBattleDisplay(SDL_Renderer* renderer)
{

}

/* Main create call - first construction */
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
}

/* Create actions */
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

/* Create classes */
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

/* Create races */
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

/* Create skills */
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
  prismatic_shot->addAction(act_dmg[4], false);
  prismatic_shot->addAction(act_dmg[4], false);
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
  shatter_shot->addAction(act_dmg[23], false);
  shatter_shot->setDescription("A heavy shatter shot with potential splash");
  shatter_shot->setPrimary(Element::ELECTRIC);
  shatter_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(shatter_shot);

  /* Engineer Upgrade */
  Skill* engg_upgrade = new Skill(160, "Upgrade", 
                                  ActionScope::ONE_ALLY_NOT_USER, act_alt[7], 
                                  100, 5);
  engg_upgrade->addAction(act_alt[8], false);
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
                                 act_dmg[24], 100, 25);
  locked_shot->setDescription("A perfect shot against a foe");
  locked_shot->setPrimary(Element::ELECTRIC);
  locked_shot->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(locked_shot);

  /* Detonate */
  Skill* detonate = new Skill(182, "Detonate", ActionScope::ALL_TARGETS,
                              act_dmg[8], 90, 15);
  detonate->addAction(act_alt[9], false);
  detonate->setDescription("Inflict massive damage on everyone while dying.");
  detonate->setPrimary(Element::ELECTRIC);
  detonate->setFlag(SkillFlags::OFFENSIVE);
  skills.push_back(detonate);

  /* Hidden Amplification */
  Skill* hidden_amp = new Skill(200, "Hidden Amplification", ActionScope::USER,
                                act_alt[10], 90, 5);
  hidden_amp->addAction(act_alt[7], false);
  hidden_amp->setDescription("Increase attack and defense of self");
  hidden_amp->setPrimary(Element::PHYSICAL);
  hidden_amp->setFlag(SkillFlags::DEFENSIVE);
  skills.push_back(hidden_amp);

  /* Surge of Will */
  // TODO: HOW TO DO MULTI-TURN??
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
  cunning->addAction(act_alt[11], false);
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
                                  act_dmg[25], 90, 15);
  multi_strike->addAction(act_dmg[25], false);
  multi_strike->addAction(act_dmg[25], false);
  multi_strike->addAction(act_dmg[25], false);
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
  ensnare->addAction(act_alt[12], false);
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

/* Create Skill Sets */
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
}

/* Delete actions */
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

/* Delete classes */
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

/* Delete races */
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

/* Delete skills */
void TestBattle::deleteSkills()
{
  /* Delete entire set */
  for(uint16_t i = 0; i < skills.size(); i++)
    delete skills[i];
  skills.clear();
}

/* Delete skill sets */
void TestBattle::deleteSkillSets()
{
  /* Delete entire set */
  for(uint16_t i = 0; i < skillsets.size(); i++)
    delete skillsets[i];
  skillsets.clear();
}

/* Main destruction call - ending */
void TestBattle::destroy()
{
  deleteClasses();
  deleteRaces();
  deleteSkillSets();
  deleteSkills();
  deleteActions();
}
  
/* Get call for Skill by ID */
Skill* TestBattle::getSkill(int id)
{
  Skill* found_skill = NULL;

  for(uint16_t i = 0; i < skills.size(); i++)
    if(skills[i]->getID() == id)
      found_skill = skills[i];

  return found_skill;
}

/* Get call for Skill Set by ID */
SkillSet* TestBattle::getSkillSet(int id)
{
  SkillSet* found_set = NULL;

  for(uint16_t i = 0; i < skillsets.size(); i++)
    if(skillsets[i]->getID() == id)
      found_set = skillsets[i];

  return found_set;
}

/* Set up the battle */
void TestBattle::initBattle()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* The key up and down events to be handled by the class */
bool TestBattle::keyDownEvent(SDL_KeyboardEvent event)
{
  /* Exit the test battle: finished processing */
  if(event.keysym.sym == SDLK_ESCAPE)
  {
    return true;
  }
  return false;
}

/* The key up and down events to be handled by the class */
void TestBattle::keyUpEvent(SDL_KeyboardEvent event)
{

}

/* Renders the title screen */
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
  }

  return true;
}

/* Set the running configuration, from the options class */
bool TestBattle::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    game_config = running_config;
    base_path   = game_config->getBasePath();
    
    /* Battle configuration setup */
    if(battle_display != NULL)
      battle_display->setConfiguration(running_config);

    return true;
  }
  
  return false;
}

/* Updates the game state */
bool TestBattle::update(int cycle_time)
{
  return false;
}
