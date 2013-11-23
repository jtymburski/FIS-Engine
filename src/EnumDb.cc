/******************************************************************************
* File Name: EnumDb [Implementation]
* Date Created: November 12th, 2013
* 
* Description: EnumDatabase contains the database of global enumerations and
*              functions for Enum -> String
******************************************************************************/

#include "EnumDb.h"

/*============================================================================
 * GLOBAL ENUMS
 *===========================================================================*/

/*
 * Description: Possible occasions for an action (scenarios where the action
 *              can be used)
 *
 * ALWAYS - the action may be used in Battle and in Menu
 * BATTLE - the action may only be used in Battle
 * MENU   - the action may only be used in Menu
 * NONE   - the action has no usable scenario
 */
enum class ActionOccasion : std::uint8_t
{ 
  ALWAYS,
  BATTLE,
  MENU,
  NONE
};

/*
 * Description: Possible target scopes may have in Battle
 *
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
enum class ActionScope : std::uint16_t
{ 
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
  ALL_NOT_USER,
  NO_SCOPE 
};

/*
 * Description: Possible action types an actor in Battle may take.
 *
 * SKILL - the actor has chosen to utilize a skill
 * ITEM - the actor has chosen to use an item from inventory
 * DEFEND - the actor has chosen to defend, thereby increasing defense.
 * GUARD - the actor has chosen to guard another character
 * SELF_DESTRUCT - the actor has chosen to self destruct themselves
 * RUN - the actor has chosen to attempt to run from battle
 * PASS - the actor has chosen to pass their turn (inaction)
 * NULL_ACTION - no action has been yet chosen by the actor
 */
enum class ActionType : std::uint8_t
{ 
  SKILL,
  ITEM,
  DEFEND,
  GUARD,
  IMPLODE,
  RUN,
  PASS,
  NONE 
};

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
enum class ActorDeath : std::uint16_t
{ 
  STANDARD_DMG,
  POISON_DMG,
  BURN_DMG,
  BERSERK_HIT_BACK,
  METABOLIC_TETHER,
  METABOLIC_DMG,
  DEATH_COUNTDOWN,
  BOND,
  UNKNOWN
};

/*
 * Description: The Attribute enumerations are representations of the
 *              statistics contained within Battle-related entities: Persons,
 *              Items, BubbyFlavours, Equipments, etc.
 *
 * VITA - Vitality - hit points, when 0 character is incapacitated
 * QTDR - Quantum Drive - skill cost currency
 * PHAG - Physical Aggression - non-elemental attack statistic
 * PHFD - Physical Fortitude - non-elemental defense statistic
 * THAG - Thermal Aggression - fire-elemental attack statistic
 * THFD - Thermal Fortitude - fire-elemental defense statistic
 * PRAG - Primal Aggression - forest-elemental attack statistic
 * PRFD - Primal Fortitude - forest-elemental defense statistic
 * POAG - Polar Aggression - ice-elemental attack statistic
 * POFD - Polar Fortitude - ice-elemental defense statistic
 * CHAG - Charged Aggression - electric-elemental attack statistic
 * CHFD - Charged Fortitude - electric-elemental defense statistic
 * CYAG - Cybernetic Aggression - digital-elemental attack statistic
 * CYFD - Cybernetic Fortitude - digital-elemental defense statistic
 * NIAG - Nihil Aggression - void-elemental attack statistic
 * NIFD - Nihil Fortitude - void-elemental defense statistic
 * MMNT - Momentum - speed, in relation to turn order and action order
 * LIMB - Limbertude - dodge chance in avoiding attacks/ActionScope
 * UNBR - Unbearability - critical hit chance, increases base damage by factor
 * MANN - Manna - luck which has various effects on outcomes/earnings etc.
 * NOAT - No Attribute - the enumeration does not describe an attribute
 */
enum class Attribute : std::uint32_t
{ 
  VITA, 
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
  MANN,
  NONE
};

/*
 * Description: Dialog category for the talking in map
 */
enum class DialogCategory
{ 
  TEXT,
  NUMBERENTRY,
  EXTENTRY 
};

/*
 * Description: Handler for all direction related calls for movement, 
 *              passability, etc.
 */
enum class Direction
{
  NORTH         = 0x1,
  EAST          = 0x2,
  SOUTH         = 0x4,
  WEST          = 0x8,
  DIRECTIONLESS = 0 
};

/*
 * Description: Elements of the Game. Elements have weaknesses and strengths
 *
 * PHYSICAL - pure physical element
 * FIRE     > FOREST
 * FOREST   > ICE
 * ELECTRIC > DIGITAL
 * DIGITAL  > NIHIL
 * NIHIL    > FIRE
 * NONELEMENTAL - there is no element
 */
enum class Element : std::uint8_t
{ 
  PHYSICAL,
  FIRE,
  FOREST,
  ICE,
  ELECTRIC,
  DIGITAL,
  NIHIL,
  NONE
};

/*
 * Description: Equipment slots which represent compartments on a person which
 *              will hold the equipments on the person because of reasons.
 */
enum class EquipmentSlots : std::uint8_t
{
  HEAD,
  LARM,
  RARM,
  BODY,
  LEGS
};

/* 
 * Description: The event classification - for what the event will do 
 */
enum class EventClassifier 
{
  NOEVENT        = 0,
  GIVEITEM       = 1,
  RUNBATTLE      = 2, 
  RUNMAP         = 3,
  TELEPORTPLAYER = 4,
  TELEPORTTHING  = 5,
  TARTCONVO      = 6 
};

/*
 * Description: Possible states of sorting for inventory items
 */
enum class ObjectSorts : std::uint8_t
{ 
  UNSORTED,
  ID,
  NAME,
  FLAVOUR,
  LEVEL,
  VALUE,
  MASS,
  VALUEPERMASS
};


/*
 * Description: Infliction enumeration represents the possible ailments / 
 *              status buffs that an actor can have inflicted upon them
 *
 * Notes: See Ailment class for documentation on each Infliction
 */
enum class Infliction : std::uint64_t
{
  POISON,
  BURN,
  SCALD,
  CHARR,
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
  PHYBUFF,
  THRBUFF,
  POLBUFF,
  PRIBUFF,
  CHGBUFF,
  CYBBUFF,
  NIHBUFF,
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
  INVALID
};

/*
 * Description: The various types of parties which occur in the game
 *
 * SLEUTH - The party of the Player
 * BEARACKS - The storage party on the ship for the Player
 */
enum class PartyType : std::uint8_t
{ 
  SLEUTH,
  BEARACKS,
  REGULAR_FOE,
  MINI_BOSS,
  BOSS,
  FINAL_BOSS 
};

/* 
 * Description: The ranks a Person may hold 
 */
enum class Ranks : std::uint16_t
{ 
  NUBEAR,
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
  NONE
};

/*============================================================================
 * CONSTANTS
 *===========================================================================*/

/* scopeToString - converts ActionScope to string 
 const boost::unordered_map<ActionScope, const char*> scopeToString 
   = map_list_of
   (ONE_TARGET, "Single Target")
   (ONE_ENEMY, "Single Enemy")
   (TWO_ENEMIES, "Two Enemies")
   (ALL_ENEMIES, "All Enemies")
   (ONE_ALLY, "Single Ally")
   (ALL_ALLIES, "All Allies")
   (ONE_ALLY_KO, "Single Incapacitated Ally")
   (ALL_ALLIES_KO, "All Incapacitated Allies")
   (NOT_USER, "Single Target Except User")
   (ALL_NOT_USER, "Everyone Except User")
   (NO_SCOPE, "No Target");

actionTypeToString - converts ActionType to string 
const boost::unordered_map<ActionType, const char*> actionTypeToString
  = map_list_of
  (SKILL, "SKILL")
  (ITEM, "Item")
  (DEFEND, "Defend")
  (GUARD, "Guard")
  (IMPLODE, "Implode")
  (RUN, "RUN")
  (PASS, "PASS")
  (NONE, "NO ACTION TYPE");

actorDeathToString - converts ActorDeath to string 
const boost::unordered_map<ActorDeath, const char*> actorDeathToString
  = map_list_of
  (STANDARD_DMG, "Damage")
  (POISON_DMG, "Poison Damage")
  (BURN_DMG, "Burn Damage")
  (BERSERK_HIT_BACK, "Hit Back Damage")
  (METABOLIC_TETHER, "Tether")
  (METABOLIC_DMG, "Tether Damage")
  (DEATH_COUNTDOWN, "Death Clock")
  (BOND, "Bond")
  (UNKNOWN, "Unknown Damage");

equipmentSlotsToString - convers EquipmentSlots to string
const boost::unordered_map<EquipmentSlots,const char*> equpmentSlotsToString
  = map_list_of
  (HEAD)("Head Gear")
  (LARM)("Left Arm")
  (RARM)("Right Arm")
  (BODY)("Torso")
  (LEGS)("Legs")

objectSortsToString - converts ObjectSorts to string 
const boost::unordered_map<ItemSorts,const char*> objectSortsToString
  = map_list_of
  (UNSORTED, "Unsorted")
  (ID, "ID")
  (NAME, "Name")
  (FLAVOUR, "Flavour")
  (LEVEL, "Level")
  (VALUE, "Value")
  (MASS, "Mass")
  (VALUEPERMASS, "Value / Mass");

inflictionToString - converts inflictions to string 
const boost::unordered_map<Infliction,const char*> inflictonToString
  = map_list_of
  (POISON, "Poison")
  (BURN, "Burn")
  (SCALD, "Scald")
  (CHARR, "Charr")
  (BERSERK, "Berserk")
  (CONFUSE, "Confuse")
  (SILENCE, "Silence")
  (BUBBIFY, "Bubbify")
  (DEATHTIMER, "Death Timer")
  (PARALYSIS, "Paralysis")
  (BLINDNESS, "Blindness")
  (DREADSTRUCK, "Dreadstruck")
  (DREAMSNARE, "Dreamsnare")
  (HELLBOUND, "Hellbound")
  (BOND, "Bond")
  (BOUND, "Bound")
  (ALLATKBUFF, "All Attack Buff")
  (ALLDEFBUFF, "All Defense Buff")
  (PHYBUFF, "Physical Buff")
  (THRBUFF, "Thermal Buff")
  (POLBUFF, "Polar Buff")
  (PRIBUFF, "Primal Buff")
  (CHGBUFF, "Charged Buff")
  (CYBBUFF, "Cybernetic Buff")
  (NIHBUFF, "Nihil Buff")
  (LIMBUFF, "Limbertude Buff")
  (UNBBUFF, "Unbearability Buff")
  (MOMBUFF, "Momentum Buff")
  (VITBUFF, "Vitality Buff")
  (QDBUFF, "Quantum Buff")
  (ROOTBOUND, "Rootbound")
  (DOUBLECAST, "Double Cast")
  (TRIPLECAST, "Triple Cast")
  (HALFCOST, "Half Cost")
  (REFLECT, "Reflect")
  (HIBERNATION, "Hibernation")
  (CURSE, "Curse")
  (METATETHER, "Metatether")
  (STUBULATE, "Stubulate")
  (INVALID, "Invalid");

ranksToString - converts person ranks to string
const boost::unordered_map<Ranks, const char*> ranksToString
  = map_list_of
  (NUBEAR, "Nubear")
  (CUB, "Cub")
  (RECRUIT, "Recruit")
  (SLEUTHLING, "Sleuthling")
  (SERGEANT, "Sergeant")
  (SLEUTHMASTER, "Sleuth Master")
  (OFFICER, "Officer")
  (URSAMINOR, "Ursa Minor")
  (ADMIRAL, "Admiral")
  (URSAMAJOR, "Ursa Major")
  (FOREBEAR, "Forebear")
  (ALPHABEAR, "Alpha Bear")
  (BOAT, "Bear of Ancient Times")
  (NONE, "Unranked");
*/