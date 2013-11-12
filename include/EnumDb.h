/******************************************************************************
* Class Name: EnumDb
* Date Created: January 30th, 2013
* Inheritance:
* Description:  Enum Database holds non-flag Enums
 *****************************************************************************/
#ifndef ENUMDB_H
#define ENUMDB_H

#include <QList>
#include <QObject>
#include <QString>

#include "EnumString.h"
//#include "Game/EventHandler.h"

class EnumDb
{
public:
  /*
   * Description: ActionType - types of actions an actor in battle may take
   *
   * SKILL - the actor has chosen to utilize a skill
   * ITEM - the actor has chosen to use an item from inventory
   * DEFEND - the actor has chosen to defend, thereby increasing defense.
   * RUN - the actor has chosen to attempt to run from battle
   * PASS - the actor has chosen to pass their turn (inaction)
   * NULL_ACTION - no action has been yet chosen by the actor
   */
  enum ActionType { SKILL,
                    ITEM,
                    DEFEND,
                    RUN,
                    PASS,
                    NULL_ACTION };

 /*
  * Description: ActionScope - possible scopes that a skill or an itemmay have
  *              use for
  *
  * NO_SCOPE - no scope (cannot be used)
  * ONE_ENEMY - the skill strikes a choice of one enemy (other party)
  * TWO_ENEMIES - the skills strikes a choice of two enemies (other party)
  * ALL_ENEMIES - the skill strikes all enemies on opposing team
  * ONE_ALLY - the skill hits a choice of one ally (including self)
  * TWO_ALLIES - the skill hits a choice of two allies (including self)
  * ONE_ALLY_KO - the skill hits an incapacitated ally
  * ALL_ALLIES_KO - the skill strikes all allies who are incapacitated
  * ALL_TARGETS - the skill strikes all available targets
  * NOT_USER - the skill strikes choice of everyone except the user
  * ALL_NOT_USER - the skill strikes all targets except user
  */
  enum ActionScope { NO_SCOPE,
                     ONE_TARGET,
                     ONE_ENEMY,
                     TWO_ENEMIES,
                     ALL_ENEMIES,
                     ONE_ALLY,
                     TWO_ALLIES,
                     ALL_ALLIES,
                     ONE_ALLY_KO,
                     ALL_ALLIES_KO,
                     ALL_TARGETS,
                     NOT_USER,
                     ALL_NOT_USER };

  /*
   * Description: ActionOccasion - describes possible scenarios that an action
   *              may be used
   *
   * ALWAYS - the skill may be used in Battle and in Menu
   * BATTLE - the skill may only be used in Battle
   * MENU   - the skill may only be used in Menu
   * NEVER  - the skill cannot be used
   */
  enum ActionOccasion { ALWAYS,
                        BATTLE,
                        MENU,
                        NEVER };

  /*
   * Description: ActorDeath - describes reasons for an actor becoming
   *              incapacitated.
   *
   * STANDARD_DMG - the actor was struck down by normal battle damage
   * POISON_DMG - the actor was struck down by incurred upkeep posion damage
   * BURN_DMG - the actor was struck down by incurred upkeep burn damage
   * BERSERK_HIT_BACK - the actor was struck by self berserk damage
   * METABOLIC_TETHER - the metabolic tether randomnly killed the ActorDeath
   * METABOLIC_DMG - the metabolic tether damage killed the actor
   * DEATH_COUNTDOWN - a death clock killed the actor
   * BOND_DEAD - the user was killed due to the effects of Bond/Bonded
   * UNKNOWN_DEATH - the actor was killed for unknown reasons.
   */
  enum ActorDeath { STANDARD_DMG,
                    POISON_DMG,
                    BURN_DMG,
                    BERSERK_HIT_BACK,
                    METABOLIC_TETHER,
                    METABOLIC_DMG,
                    DEATH_COUNTDOWN,
                    BOND_DEATH,
                    UNKNOWN_DEATH };

  /*
   * Description: The Attribute enumerations is used in AttributeSet to
   *              describe then names of the statics used by Persons,
   *              Items, etc.
   */
  enum Attribute { VITA,
                   QTDR,
                   PHAG,
                   PHFD,
                   THAG,
                   THFD,
                   PRAG,
                   PRFD,
                   POAG,
                   POFD,
                   CHAG,
                   CHFD,
                   CYAG,
                   CYFD,
                   NIAG,
                   NIFD,
                   MMNT,
                   LIMB,
                   UNBR,
                   NOAT};

  /*
   * Description: Handles the category for the talking inside the map
   */
  enum DialogCategory { TEXT,
                        NUMBERENTRY,
                        TEXTENTRY };

  /*
   * Description: Handler for all direction related calls for movement, 
   *              passability, etc.
   */
  enum Direction { NORTH         = 0x1,
                   EAST          = 0x2,
                   SOUTH         = 0x4,
                   WEST          = 0x8,
                   DIRECTIONLESS = 0 };
  
  /*
   * Description: Elements of the Game
   */
  enum Element { PHYSICAL,
                 FIRE,
                 ICE,
                 FOREST,
                 ELECTRIC,
                 DIGITAL,
                 NIHIL,
                 NONELEMENTAL };

  /* 
   * Description: The event classification - for what the event will do 
   */
  enum EventClassifier {NOEVENT        = 0,
                        GIVEITEM       = 1,
                        RUNBATTLE      = 2, 
                        RUNMAP         = 3,
                        TELEPORTPLAYER = 4,
                        TELEPORTTHING  = 5,
                        STARTCONVO     = 6 };

  /*
   * Description: Describes the current state of the inventory (how it is
   *              currently sorted)
   */
  enum ItemSorts { NONE,
                   ID,
                   NAME,
                   FLAVOUR,
                   LEVEL,
                   VALUE,
                   MASS,
                   VALUEPERMASS };

  /*
   * Description: Infliction enumeration represents a Status ailment or a
   *              Status Buff. String conversions are listed blow.
   */
  enum Infliction { POISON,
                    BURN,
                    SCALD,
                    INFLICTCHAR,
                    BERSERK,
                    CONFUSE,
                    SILENCE,
                    BUBBIFY,
                    DEATHTIMER,
                    PARALYSIS,
                    BLINDNESS,
                    DREADSTRUCK,
                    DREAMSNARE,
                    HELLBOUND,
                    BOND,
                    BONDED,
                    ALLATKBUFF,
                    ALLDEFBUFF,
                    PHYATKBUFF,
                    PHYDEFBUFF,
                    THRATKBUFF,
                    THRDEFBUFF,
                    POLATKBUFF,
                    POLDEFBUFF,
                    PRIATKBUFF,
                    PRIDEFBUFF,
                    CHGATKBUFF,
                    CHGDEFBUFF,
                    CYBATKBUFF,
                    CYBDEFBUFF,
                    NIHATKBUFF,
                    NIHDEFBUFF,
                    LIMBUFF,
                    UNBBUFF,
                    MOMBUFF,
                    VITBUFF,
                    QDBUFF,
                    ROOTBOUND,
                    DOUBLECAST,
                    TRIPLECAST,
                    HALFCOST,
                    REFLECT,
                    HIBERNATION,
                    CURSE,
                    METATETHER,
                    STUBULATE,
                    NOAILMENT };

  /*
   * Description: Ranks the Player may have
   */
  enum PersonRanks { NUBEAR,
                     CUB,
                     RECRUIT,
                     SLEUTHLING,
                     SERGEANT,
                     SLEUTHMASTER,
                     OFFICER,
                     URSAMINOR,
                     ADMIRAL,
                     URSAMAJOR,
                     FOREBEAR,
                     ALPHABEAR,
                     BOAT,
                     NULL_RANK };

  /*
   * Description: Describes the type of party to be created
   */
  enum PartyType { SLEUTH,
                   BEARACKS,
                   REGULAR_FOE,
                   MINI_BOSS,
                   BOSS,
                   FINAL_BOSS };

  /*
   * Description: Describes the current state of a skill set (how it is
   *              currently known to be sortd)
   */
  enum SkillSorts {NONE_SKILL,
                   NAME_SKILL,
                   LEVEL_SKILL };

  /*
   * Description: Enumerates all the containers which can be sorted
   */
  enum SortObjects {BUBBY_VECTOR,
                    EQUIPMENT_VECTOR,
                    ITEM_VECTOR,
                    KEY_ITEM_VECTOR };
};

/* Registration of Attribute Enumerator */
Begin_Enum_String( EnumDb::Attribute )
{
  RegisterEnumerator(EnumDb::VITA, "Vitality");
  RegisterEnumerator(EnumDb::QTDR, "Quantum Drive");
  RegisterEnumerator(EnumDb::PHAG, "Physical Aggression");
  RegisterEnumerator(EnumDb::PHFD, "Physical Fortitude");
  RegisterEnumerator(EnumDb::THAG, "Thermal Aggression");
  RegisterEnumerator(EnumDb::THFD, "Thermal Fortitude");
  RegisterEnumerator(EnumDb::PRAG, "Primal Aggression");
  RegisterEnumerator(EnumDb::PRFD, "Primal Fortitude");
  RegisterEnumerator(EnumDb::POAG, "Polar Aggression");
  RegisterEnumerator(EnumDb::POFD, "Polar Fortitude");
  RegisterEnumerator(EnumDb::CHAG, "Charged Aggression");
  RegisterEnumerator(EnumDb::CHFD, "Charged Fortitude");
  RegisterEnumerator(EnumDb::CYAG, "Cybernetic Aggression");
  RegisterEnumerator(EnumDb::CYFD, "Cybernetic Fortitude");
  RegisterEnumerator(EnumDb::NIAG, "Nihil Aggression");
  RegisterEnumerator(EnumDb::NIFD, "Nihil Fortitude");
  RegisterEnumerator(EnumDb::MMNT, "Momentum");
  RegisterEnumerator(EnumDb::LIMB, "Limbertude");
  RegisterEnumerator(EnumDb::UNBR, "Unbearability");
}
End_Enum_String;

Begin_Enum_String( EnumDb::Infliction )
{
  RegisterEnumerator(EnumDb::POISON, "Poison");
  RegisterEnumerator(EnumDb::BURN, "Burn");
  RegisterEnumerator(EnumDb::SCALD, "Scald");
  RegisterEnumerator(EnumDb::INFLICTCHAR, "Char");
  RegisterEnumerator(EnumDb::BERSERK, "Berserk");
  RegisterEnumerator(EnumDb::CONFUSE, "Confuse");
  RegisterEnumerator(EnumDb::SILENCE, "Silence");
  RegisterEnumerator(EnumDb::BUBBIFY, "Bubbify");
  RegisterEnumerator(EnumDb::DEATHTIMER, "Deathtimer");
  RegisterEnumerator(EnumDb::PARALYSIS, "Paralysis");
  RegisterEnumerator(EnumDb::BLINDNESS, "Blindness");
  RegisterEnumerator(EnumDb::DREADSTRUCK, "Dreadstruck");
  RegisterEnumerator(EnumDb::DREAMSNARE, "Dreamsnare");
  RegisterEnumerator(EnumDb::HELLBOUND, "Hellbound");
  RegisterEnumerator(EnumDb::BOND, "Bond");
  RegisterEnumerator(EnumDb::BONDED, "Bonded");
  RegisterEnumerator(EnumDb::ALLATKBUFF, "All-Aggression Buff");
  RegisterEnumerator(EnumDb::ALLDEFBUFF, "All-Fortitude Buff");
  RegisterEnumerator(EnumDb::PHYATKBUFF, "Physical Aggression Buff");
  RegisterEnumerator(EnumDb::PHYDEFBUFF, "Physical Fortitude Buff");
  RegisterEnumerator(EnumDb::THRATKBUFF, "Thermal Aggression Buff");
  RegisterEnumerator(EnumDb::THRDEFBUFF, "Thermal Fortitude Buff");
  RegisterEnumerator(EnumDb::POLATKBUFF, "Polar Aggression Buff");
  RegisterEnumerator(EnumDb::POLDEFBUFF, "Polar Fortitude Buff");
  RegisterEnumerator(EnumDb::PRIATKBUFF, "Primal Aggression Buff");
  RegisterEnumerator(EnumDb::PRIDEFBUFF, "Primal Fortitude Buff");
  RegisterEnumerator(EnumDb::CHGATKBUFF, "Charged Aggression Buff");
  RegisterEnumerator(EnumDb::CHGDEFBUFF, "Charged Fortitude Buff");
  RegisterEnumerator(EnumDb::CYBATKBUFF, "Cybernetic Aggression Buff");
  RegisterEnumerator(EnumDb::CYBDEFBUFF, "Cybernetic Fortitude Buff");
  RegisterEnumerator(EnumDb::NIHATKBUFF, "Nihil Aggression Buff");
  RegisterEnumerator(EnumDb::NIHDEFBUFF, "Nihil Fortitude Buff");
  RegisterEnumerator(EnumDb::LIMBUFF, "Limbertude Aggression Buff");
  RegisterEnumerator(EnumDb::UNBBUFF, "Unbearability Buff");
  RegisterEnumerator(EnumDb::MOMBUFF, "Momentum Buff");
  RegisterEnumerator(EnumDb::VITBUFF, "Vitality Buff");
  RegisterEnumerator(EnumDb::QDBUFF, "Quantum Drive Buff");
  RegisterEnumerator(EnumDb::ROOTBOUND, "Rootbound");
  RegisterEnumerator(EnumDb::DOUBLECAST, "Double Cast");
  RegisterEnumerator(EnumDb::TRIPLECAST, "Triple Cast");
  RegisterEnumerator(EnumDb::HALFCOST, "Half Cost");
  RegisterEnumerator(EnumDb::REFLECT, "Reflect");
  RegisterEnumerator(EnumDb::HIBERNATION, "Hibernation");
  RegisterEnumerator(EnumDb::CURSE, "Curse");
  RegisterEnumerator(EnumDb::METATETHER, "Metabolic Tether");
  RegisterEnumerator(EnumDb::STUBULATE, "Stubulate");
  RegisterEnumerator(EnumDb::NOAILMENT, "No Effect");
}
End_Enum_String;


Begin_Enum_String( EnumDb::PersonRanks )
{
  RegisterEnumerator(EnumDb::NUBEAR, "Nubear");
  RegisterEnumerator(EnumDb::RECRUIT, "Recruit");
  RegisterEnumerator(EnumDb::SLEUTHLING, "Sleuthling");
  RegisterEnumerator(EnumDb::SERGEANT, "Sergeant");
  RegisterEnumerator(EnumDb::SLEUTHMASTER, "Sleuth Master");
  RegisterEnumerator(EnumDb::OFFICER, "Officer");
  RegisterEnumerator(EnumDb::URSAMINOR, "Ursa Minor");
  RegisterEnumerator(EnumDb::ADMIRAL, "Admiral");
  RegisterEnumerator(EnumDb::URSAMAJOR, "Ursa Major");
  RegisterEnumerator(EnumDb::FOREBEAR, "Forebear");
  RegisterEnumerator(EnumDb::ALPHABEAR, "Alpha Bear");
  RegisterEnumerator(EnumDb::BOAT, "Bear of Ancient Times");
}
End_Enum_String;

/* -------------------------- Structures ------------------------------- */

#endif // ENUMDB_H
