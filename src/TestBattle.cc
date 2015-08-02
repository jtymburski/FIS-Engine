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
void TestBattle::buildBattleDisplayFrames(SDL_Renderer* renderer)
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
  act_alt.push_back(new Action("204,ALTER-FLIP,,,,QTDR,PC.10,AMOUNT.20,THAG,96"));
  act_alt.push_back(
            new Action("205,ALTER-FLIP,,,,VITA,AMOUNT.100,AMOUNT.30,VITA,100"));

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

}

/* Create races */
void TestBattle::createRaces()
{

}

/* Create skills */
void TestBattle::createSkills()
{

}

/* Create Skill Sets */
void TestBattle::createSkillSets()
{

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

}

/* Delete races */
void TestBattle::deleteRaces()
{

}

/* Delete skills */
void TestBattle::deleteSkills()
{

}

/* Delete skill sets */
void TestBattle::deleteSkillSets()
{

}

/* Main destruction call - ending */
void TestBattle::destroy()
{
  deleteActions();
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
  return true;
}

/* Set the running configuration, from the options class */
bool TestBattle::setConfiguration(Options* running_config)
{
  return true;
}

/* Updates the game state */
bool TestBattle::update(int cycle_time)
{
  return false;
}
