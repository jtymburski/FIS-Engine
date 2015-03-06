/*******************************************************************************
* File Name: EnumDb [Declaration]
* Date Created: November 12th, 2013
* 
* Description: EnumDatabase  contains the database of global enumerations and
*              functions for Enum -> String
*******************************************************************************/
#ifndef ENUMDB_H
#define ENUMDB_H

#ifndef UDEBUG
  #define UDEBUG
#endif

//#include <boost/assign/list_of.hpp>
//#include <boost/unordered_map.hpp>
#include <cstdint>

/*==============================================================================
 * GLOBAL ENUMS
 *============================================================================*/

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
  USER              = 0,
  ONE_TARGET        = 1,
  ONE_ENEMY         = 2,
  TWO_ENEMIES       = 3,
  ALL_ENEMIES       = 4,
  ONE_ALLY          = 5,
  ONE_ALLY_NOT_USER = 6,
  TWO_ALLIES        = 7,
  ALL_ALLIES        = 8,
  ONE_ALLY_KO       = 9,
  ALL_ALLIES_KO     = 10,
  ONE_PARTY         = 11,
  ALL_TARGETS       = 12,
  NOT_USER          = 13,
  ALL_NOT_USER      = 14,
  NO_SCOPE          = 15 
};

/*
 * Description: Enumerated types of difficulty an AI may have
 *
 */
enum class AIDifficulty
{
  RANDOM,       /* Randomly chooses action types, indexes, targets */
  PRIORITY,     /* Chooses actions based on arbitrary priority values */
  TACTICAL,     /* Tactically chooses combinations of actions */
  DEEP_THOUGHT  /* Computes possible actions to determine the best outcome */
};

/*
 * Description: Enumerated types of personality an AI may have
 *
 */
enum class AIPersonality
{
  MODERATOR,   /* Generic personality type */
  AGGRESSOR,   /* Focuses on actions damage the opposing team */
  DEFENDER,    /* Focuses actions on increasing one-self */
  PROTECTOR,   /* Focuses on actions protecting teammates (including self) */
  RETALIATOR,  /* Changes priority targeting if hit by a certain enemy */
  MASOCHISTIC, /* Performs self-destructive behaviour */
  ANNIHILATOR, /* Attempts to perform the most damage possible, at any cost */
  RUNNER,      /* Focuses on running or becoming able to run */
  PASSER,      /* Focuses on doing nothing */
  NONE         /* No personality type - error */
};

/*:
 * Description: Enumerated types of selection for AI
 *
 * RANDOM - randomly selects targets until filled for the skill
 * WEAKEST_HP_FIRST - targets the weakest enemy if atking/ally if healing
 * STRONGEST_FIRST  - targets the most powerful target first
 */
enum class AITarget
{
  RANDOM           = 1 << 0,
  LOWEST_HP_FIRST  = 1 << 1,
  STRONGEST_FIRST  = 1 << 2
};

/*:
 * Description: 
 *
 */
enum class AddStatus : std::uint8_t
{
  GOOD_DELETE,
  GOOD_KEEP,
  FAIL,
  UNKNOWN
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
  INSPECT,
  RUN,
  PASS,
  NONE 
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
enum class Attribute : std::uint8_t
{ 
  VITA = 0, 
  QTDR = 1, 
  PHAG = 2, 
  PHFD = 3, 
  THAG = 4, 
  THFD = 5,
  PRAG = 6, 
  PRFD = 7,
  POAG = 8, 
  POFD = 9,
  CHAG = 10, 
  CHFD = 11,
  CYAG = 12, 
  CYFD = 13,
  NIAG = 14, 
  NIFD = 15,
  MMNT = 16,
  LIMB = 17,
  UNBR = 18,
  MANN = 19,
  NONE = 20
};

enum class EventType : std::uint8_t
{
  ITEM_USE = 1,
  SKILL_USE = 2,
  SKILL_USE_FIZZLE = 3,
  SKILL_COOLDOWN = 4,
  //TODO: Implode -- break down to damage/character removal?
  IMPLODE = 5,
  //TODO: Inspect -- ???
  INSPECT = 6,
  ATTEMPT_RUN = 7,
  SUCCEED_RUN = 8,
  FAIL_RUN = 9,
  MISS_TURN = 10,
  PASS = 11,
  SKILL_MISS = 12,
  ACTION_MISS = 13,
  BLIND_MISS = 14,
  DREAMSNARE_MISS = 15,
  FIZZLE = 16,
  STANDARD_DAMAGE = 17,
  CRITICAL_DAMAGE = 18,
  POISON_DAMAGE = 19,
  BURN_DAMAGE = 20,
  HITBACK_DAMAGE = 21,
  METABOLIC_KILL = 22,
  METABOLIC_DAMAGE = 23,
  DEATH_COUNTDOWN = 24,
  BOND = 25,
  BONDING = 26,
  BEGIN_DEFEND = 27,
  PERSIST_DEFEND = 28,
  BREAK_DEFEND = 29,
  BEGIN_GUARD = 30,
  PERSIST_GUARD = 31,
  BREAK_GUARD = 32,
  FAIL_GUARD = 33,
  DEATH = 34,
  INFLICTION = 35,
  CURE_INFLICTION = 36,
  ALTERATION = 37,
  ASSIGNMENT = 38,
  REVIVAL = 39,
  HEAL_HEALTH = 40,
  REGEN_HEALTH = 41,
  HEAL_QD = 42,
  REGEN_QD = 43,
  ACTION_BEGIN = 44,
  PARTY_DEATH = 45,
  INFLICTION_FIZZLE = 46,
  NONE = 45
};

/*
 * Description: CellState enumerations for the possible states of a Cell
 *              (part of Equipment signature)
 *
 * OPEN - the cell can have objects placed onto it and has no Bubby
 * BUBBY - the cell is occupied by a Bubby object
 * CLOSED - the cell is currently locked (but may be unlocked)
 * BLOCKED - for all purposes, the cell does not really exist
 * LINK - the cell is occupied by a link object
 * E_LINK - the cell is occupied by an elemental link object
 */
enum class CellState : std::uint32_t
{
  OPEN,
  BUBBY,
  CLOSED,
  BLOCKED,
  LINK
};

/*
 * Description: Dialog category for the talking in map
 */
enum class DialogCategory
{ 
  TEXT,
  NUMBERENTRY,
  TEXTENTRY 
};

/*
 * Description:
 */
enum class DamageType
{
  BASE,
  ALTER,
  GUARD,
  ITEM,
  POISON,
  BURN,
  HITBACK,
  DEATHTIMER
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
 * ICE > ELECTRIC
 * ELECTRIC > DIGITAL
 * DIGITAL  > NIHIL
 * NIHIL    > FIRE
 * NONELEMENTAL - there is no element
 */
enum class Element : std::uint8_t
{ 
  PHYSICAL = 1 << 1,
  FIRE     = 1 << 2,
  ICE      = 1 << 3,
  FOREST   = 1 << 4,
  ELECTRIC = 1 << 5,
  DIGITAL  = 1 << 6,
  NIHIL    = 1 << 7,
  NONE     = 1 << 0
};

/* Description: Curve of Elemental modifiers - fast to slow progression
 *              [increase on top of normal progression]
 * 
 * XS - Godlike
 *  S - Extremely powerful 
 *  A - Powerful
 *  B - Strong
 *  C - Moderate
 *  D - Normal
 */
enum class ElementCurve : uint8_t
{ 
  XS = 0, 
  S  = 1, 
  A  = 2, 
  B  = 3,
  C  = 4, 
  D  = 5
};

/*
 * Description: Equipment slots which represent compartments on a person which
 *              will hold the equipments on the person because of reasons.
 */
enum class EquipSlots : std::uint8_t
{
  HEAD = 0,
  LARM = 1,
  RARM = 2,
  BODY = 3,
  LEGS = 4
};

/*
 * Description: Infliction enumeration represents the possible ailments / 
 *              status buffs that an actor can have inflicted upon them
 *
 * Notes: See Ailment class for documentation on each Infliction
 */
enum class Infliction : std::uint64_t
{
  POISON = 0,
  BURN = 1,
  SCALD = 2,
  CHARR = 3,
  BERSERK = 4,
  CONFUSE = 5,
  SILENCE = 6,
  BUBBIFY = 7,
  DEATHTIMER = 8,
  PARALYSIS = 9,
  BLINDNESS = 10,
  DREADSTRUCK = 11,
  DREAMSNARE = 12,
  HELLBOUND = 13,
  BOND = 14,
  BONDED = 15,
  ALLATKBUFF = 16,
  ALLDEFBUFF = 17,
  PHYBUFF = 18,
  THRBUFF = 19,
  POLBUFF = 20,
  PRIBUFF = 21,
  CHGBUFF = 22,
  CYBBUFF = 23,
  NIHBUFF = 24,
  LIMBUFF = 25,
  UNBBUFF = 26,
  VITBUFF = 27,
  QDBUFF = 28,
  ROOTBOUND = 29,
  DOUBLECAST = 30,
  TRIPLECAST = 31,
  HALFCOST = 32,
  REFLECT = 33,
  HIBERNATION = 34,
  CURSE = 35,
  METATETHER = 36,
  MODULATE = 37,
  INVALID = 38
};

/*
 * Description:
 */
enum class ItemTier : std::uint8_t
{
  TRASH,
  COMMON,
  UNCOMMON,
  RARE,
  VERYRARE,
  LEGENDARY,
  NONE
};

/*
 * Description: Possible states of sorting for inventory items
 */
enum class SortType : std::uint8_t
{ 
  ID,
  NAME,
  FLAVOUR,
  LEVEL,
  VALUE,
  MASS,
  VALUEPERMASS,
  COUNT
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

enum class RegenRate : std::uint16_t
{
  ZERO   = 0,
  WEAK   = 1,
  NORMAL = 2,
  STRONG = 3,
  GRAND  = 4
};

enum class SkillSorts : std::uint32_t
{
  COOLDOWN, 
  COST, 
  ID, 
  NAME,
  PRIMARY,
  SECONDARY,
  POINT_VALUE,
  LEVEL_REQ,
  ENABLED
};

enum class SortObjects : std::uint32_t
{ 
  ZERO_BUBBIES, 
  BUBBIES, 
  EQUIPMENTS, 
  ITEMS, 
};

/*
 * Description: Defines what the map thing base class is
 */
enum class ThingBase : std::uint8_t
{
  ISBASE = 0,
  THING = 1,
  ITEM = 2,
  PERSON = 3,
  NPC = 4,
  INTERACTIVE = 5
};

/* The window display classifier to define how it's displayed:
 *  OFF - not showing
 *  SHOWING - rising from the bottom
 *  ON - fully displayed
 *  HIDING - lowering to the bottom */
enum class WindowStatus : std::uint8_t
{
  OFF, 
  SHOWING, 
  ON, 
  HIDING
};

/*============================================================================
 * CONSTANTS
 *===========================================================================*/

/* actorDeathToString - converts ActorDeath to string 
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

#endif // ENUMDB_H
