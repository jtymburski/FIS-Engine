/*******************************************************************************
* File Name: EnumDb [Declaration]
* Date Created: November 12th, 2013
*
* Description: EnumDatabase  contains the database of global enumerations and
*              functions for Enum -> String
*******************************************************************************/
#ifndef ENUMDB_H
#define ENUMDB_H

#include <cstdint>

// DEBUG MACRO
#ifndef UDEBUG
#define UDEBUG
#endif

// uncomment to disable assert()
// #define NDEBUG

class EnumDb
{
  /*=========================================================================
   * CONSTANTS
   *========================================================================*/
public:
  const static int kBASE_ID_IOS = 50000;   /* The instant base ID for MapIOs */
  const static int kBASE_ID_ITEMS = 60000; /* The instant base ID for items */
  const static int kBASE_ID_PERSON = 0;    /* The instant base ID for persons */
  const static int kBASE_ID_NPC = 10000;   /* The instant base ID for npcs */
  const static int kBASE_ID_THING = 20000; /* The base thing ID */
  const static int kMAX_COUNT_BASES = 10000;   /* Max count of base things */
  const static int kMAX_COUNT_IOS = 10000;     /* Max count of instance IOs */
  const static int kMAX_COUNT_ITEMS = 5000;    /* Max count of instance items */
  const static int kMAX_COUNT_NPCS = 10000;    /* Max count of instance npcs */
  const static int kMAX_COUNT_PERSONS = 10000; /* Max count of inst persons */
  const static int kMAX_COUNT_THINGS = 30000;  /* Max count of inst things */
};

/*=============================================================================
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
  NONE = 0,
  ALWAYS = 1,
  BATTLE = 2,
  MENU = 3
};

/*
 * Description: Possible target scopes may have in Battle
 *
 * USER - Only the user is selectable
 * ONE_TARGET - any one target
 * ONE_ENEMY - the skill strikes a choice of one enemy (other party)
 * TWO_ENEMIES - the skills strikes a choice of two enemies (other party)
 * ALL_ENEMIES - the skill strikes all enemies on opposing team
 * ONE_ALLY - the skill hits a choice of one ally (including self)
 * ONE_ALLY_NOT_USER - anybody on the allies except for the
 * TWO_ALLIES - the skill hits a choice of two allies (including self)
 * ALL_ALLIES - all of the allies
 * ONE_ALLY_KO - the skill hits an incapacitated ally
 * ALL_ALLIES_KO - the skill strikes all allies who are incapacitated
 * ALL_TARGETS - the skill strikes all available targets
 * NOT_USER - the skill strikes choice of everyone except the user
 * ALL_NOT_USER - the skill strikes all targets except user
 * NO_SCOPE - the invalid scope [scope not yet assigned]
 */
enum class ActionScope : std::uint16_t
{
  USER = 0,
  ONE_TARGET = 1,
  ONE_ENEMY = 2,
  TWO_ENEMIES = 3,
  ALL_ENEMIES = 4,
  ONE_ALLY = 5,
  ONE_ALLY_NOT_USER = 6,
  TWO_ALLIES = 7,
  ALL_ALLIES = 8,
  ONE_ALLY_KO = 9,
  ALL_ALLIES_KO = 10,
  ONE_PARTY = 11,
  ALL_TARGETS = 12,
  NOT_USER = 13,
  ALL_NOT_USER = 14,
  NO_SCOPE = 15
};

/*
 * Description: Possible action types an actor in Battle may take.
 *
 * SKILL - the actor has chosen to utilize a skill
 * ITEM - the actor has chosen to use an item from inventory
 * DEFEND - the actor has chosen to defend, thereby increasing defense.
 * GUARD - the actor has chosen to guard another character
 * IMPLODE - the actor has chosen to self destruct themselves
 * RUN - the actor has chosen to attempt to run from battle
 * PASS - the actor has chosen to pass their turn (inaction)
 * NONE - no action has been yet chosen by the actor
 */
enum class ActionType : std::uint16_t
{
  SKILL = 1,
  ITEM = 2,
  DEFEND = 3,
  GUARD = 4,
  IMPLODE = 5,
  INSPECT = 6,
  PASS = 7,
  NONE = 0
};

/*:
 * Description: AddStatus for Inventory.
 *
 * GOOD_DELETE - item was added, the item should be deleted on addition to inv.
 * GOOD_KEEP - the item was added, the item should be kept on addition to inv.
 * FAIL - the item was not added
 * UNKNOWN - other failure unknown
 */
enum class AddStatus : std::uint8_t
{
  GOOD_DELETE,
  GOOD_KEEP,
  FAIL,
  UNKNOWN
};

/*
 * Description: Enumerated types of difficulty an AI may have
 *
 */
enum class AIDifficulty
{
  RANDOM,   /* Randomly chooses action types, indexes, targets */
  PRIORITY, /* Chooses actions based on arbitrary priority values */
};

/*
 * Description: Enumerated types of personality an AI may have
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
  RANDOM = 1 << 0,
  LOWEST_HP_FIRST = 1 << 1,
  STRONGEST_FIRST = 1 << 2
};

/*
 * Description: Class of ailments to determine processing.
 *
 * DAMAGING - ailment inflicts damage such as POISON / BURN
 * HEALING - ailment restores health such as HIBERNATION
 * PASSIVE - ailment has a passive effect ex. PARALYSIS
 * BUFF - ailment adds a buff ex. PHYS_BUFF
 * UNSET - invalid AilmentClass
 */
enum class AilmentClass
{
  DAMAGING,
  HEALING,
  PASSIVE,
  BUFF,
  UNSET
};

/*
 * Description: Enumerated status for ailment processing.
 *
 */
enum class AilmentStatus
{
  INCOMPLETE,
  TO_REMOVE,
  KO_USER,
  TO_DAMAGE,
  RANDOM,
  SKIP,
  NOTHING,
  COMPLETED,
  NONE
};

enum class Attribute
{
  VITA = 0,
  QTDR = 1,
  PRAG = 2,
  PRFD = 3,
  SEAG = 4,
  SEFD = 5,
  LIMB = 6,
  UNBR = 7,
  WILL = 8,
  MVIT = 9, // TODO: [10-24-16] Renumber
  MQTD = 10,
  NONE = 22
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
 * Description: Color mode enumerations for possible states of color within a
 *              sprite
 *
 * GREY - fully grey scale
 * GREYING - transitioning from color to grey scale
 * COLORING - transitioning from grey scale to color
 * COLOR - fully colored (default)
 */
enum class ColorMode
{
  GREY,
  GREYING,
  COLORING,
  COLOR,
  INVALID
};

/*
 * Description: Types of damage that a BattleActor can receive. Aids in
 *              determining colours of damage or other effects.
 */
enum class DamageType
{
  BASE,
  ALTER,
  CRITICAL,
  GUARD,
  ITEM,
  POISON,
  BURN,
  HITBACK,
  DEATHTIMER,
  HEALING,
  VITA_REGEN,
  QTDR_REGEN,
  HIBERNATION_REGEN,
  ACTION_MISS,
  IMMUNE,
  ALREADY_INFLICTED
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

// TODO: add the shop implementation [2013-08-20]
/* The dialog mode classifier to define the running mode:
 *  DISABLED - The dialog is not in use
 *  CONVERSATION - A conversation is currently running
 *  NOTIFICATION - A notification display, shifts up then down
 *  SHOP - A numerical question for buying, in a shop for example */
enum class DialogMode
{
  DISABLED,
  CONVERSATION,
  NOTIFICATION,
  SHOP
};

/*
 * Description: Handler for all direction related calls for movement,
 *              passability, etc.
 */
enum class Direction
{
  NORTH = 0x1,
  EAST = 0x2,
  SOUTH = 0x4,
  WEST = 0x8,
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
  FIRE = 1 << 2,
  ICE = 1 << 3,
  FOREST = 1 << 4,
  ELECTRIC = 1 << 5,
  DIGITAL = 1 << 6,
  NIHIL = 1 << 7,
  NONE = 1 << 0
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
  S = 1,
  A = 2,
  B = 3,
  C = 4,
  D = 5
};

/*
 * Description:
 *
 */
enum class ElementType : uint8_t
{
  PRIMARY = 1 << 1,
  SECONDARY = 1 << 2,
  NONE = 1 << 0
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
 * Description: FadeState of an object.
 */
enum class FadeState
{
  FADING_IN,
  DISPLAYING,
  FADING_OUT,
  NOT_SHOWN
};

/*
 * Description: Enumerated GameKey values. For KeyHandler to process what
 *              key corresponds to what game action.
 */
enum class GameKey
{
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_UP,
  MOVE_DOWN,
  MENU,
  ACTION,
  CANCEL,
  BACKSPACE,
  RUN,
  DEBUG,
  PAUSE,
  NONE
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
 * Description: Tier level that an item can be.
 */
enum class ItemTier : std::uint8_t
{
  TRASH = 0,
  COMMON = 1,
  UNCOMMON = 2,
  RARE = 3,
  VERYRARE = 4,
  LEGENDARY = 5,
  NONE = 6
};

/*
 * Description: Enumerated type of Lay.
 */
enum class LayType
{
  OVERLAY = 1,
  MIDLAY = 2,
  UNDERLAY = 3,
  NONE = 0
};

/*
 *   ----------------------
 *   |      |      |      |
 *   |  NW  |  N   |  NE  |
 *   |______|______|______|
 *   |      |      |      |
 *   |  W   |  C   |  E   |
 *   |______|______|______|
 *   |      |      |      |
 *   |  SW  |  S   |  SE  |
 *   |______|______|______|
 */
enum class LayIndex
{
  NORTH_WEST = 1,
  NORTH = 2,
  NORTH_EAST = 3,
  WEST = 4,
  CENTRE = 5,
  EAST = 6,
  SOUTH_WEST = 7,
  SOUTH = 8,
  SOUTH_EAST = 9
};

/*
 * Description: The map fade operator, for controlling how the screen appears
 */
enum class MapFade
{
  BLACK = 0,
  VISIBLE = 1,
  FADINGIN = 2,
  FADINGOUT = 3
};

/* Enumerated MenuLayer */
enum class MenuLayer
{
  TITLE,
  MAIN,
  MAIN_INDENT,
  POPUP,
  CONFIRMATION,
  INVALID
};

/* Enumerated MenuType */
enum class MenuType
{
  SIGNATURE,
  SLEUTH,
  SLEUTH_OVERVIEW,
  SLEUTH_EQUIPMENT,
  SLEUTH_SKILLS,
  SLEUTH_DETAILS,
  INVENTORY,
  INVENTORY_BUBBIES,
  INVENTORY_EQUIP,
  INVENTORY_ITEMS,
  INVENTORY_KEY,
  OPTIONS,
  SAVE,
  LOAD,
  QUIT,
  TITLE_PLAYER_SELECT,
  TITLE_NEW_GAME,
  TITLE_LOAD_GAME,
  TITLE_OPTIONS,
  TITLE_OPTIONS_MAIN,
  TITLE_OPTIONS_CONTROLS,
  TITLE_QUIT,
  INVALID
};

/*
 * Description: Type of outcome a Battle can have.
 */
enum class OutcomeType
{
  VICTORY,
  DEFEAT,
  ALLIES_RUN,
  ENEMIES_RUN,
  RETURN,
  NONE
};

/*
 * Description: The various types of parties which occur in the game
 *
 * SLEUTH - The party of the Player
 * BEARACKS - The storage party on the ship for the Player
 */
enum class PartyType : std::uint8_t
{
  SLEUTH = 0,
  BEARACKS = 1,
  REGULAR_FOE = 2,
  MINI_BOSS = 3,
  BOSS = 4,
  FINAL_BOSS = 5
};

/*
 * Description: Rank that a Player can be.
 */
enum class Rank : std::uint16_t
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

/*
 * Description:
 */
enum class RenderStatus
{
  FADING_IN,
  DISPLAYING,
  FADING_OUT,
  TIMED_OUT
};

/*
 * Description:
 */
enum class RegenRate : std::uint16_t
{
  ZERO = 0,
  WEAK = 1,
  NORMAL = 2,
  STRONG = 3,
  GRAND = 4
};

enum class MenuSaveState : std::uint16_t
{
  NONE = 0,
  WRITE = 1,
  CLEAR = 2
};

enum class Sex
{
  UNSET = 0,
  FEMALE = 1,
  MALE = 2
};

/*
 * Description:
 */
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

/*
 * Description:
 */
enum class SortObjects : std::uint32_t
{
  ZERO_BUBBIES,
  BUBBIES,
  ITEMS,
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
 * Description: State of a sprite
 */
enum class SpriteState
{
  HIDDEN,
  SLIDING_IN,
  SLID_IN,
  SLIDING_OUT,
  SLID_OUT,
  ATTEMPT_RUNNING,
  PASS_BOBBING,
  CYCLING_FADE,
  RUNNING,
  BOBBING,
  FLASHING,
  KOING,
  DYING,
  KOED,
  DEAD
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

/*
 * Description: Tracking state for npc node based movement
 *
 * NOTRACK - maintains normal movement (path or random)
 * AVOIDPLAYER - if player comes near, the NPC will try and stay away
 * TOPLAYER - if the player comes near, the NPC will try and reach it
 */
enum class TrackingState
{
  NOTRACK = 0,
  AVOIDPLAYER = 1,
  TOPLAYER = 2
};

/*
 * Description: Enumerated values for turn state
 */
enum class TurnState
{
  BEGIN, /* Setup of the battle */
  ENTER_DIM,
  FADE_IN_TEXT,
  GENERAL_UPKEEP,      /* General upkeep phase - weather etc. */
  UPKEEP,              /* Personal upkeep - ailments etc. */
  SELECT_ACTION_ALLY,  /* User choice of action/skill etc. */
  SELECT_ACTION_ENEMY, /* Enemy choice of skill -> AI */
  PROCESS_ACTIONS,     /* Determines outcomes of skills */
  CLEAN_UP,            /* Cleanup after turn, turn incr. etc. */
  OUTCOME,
  FINISHED,
  STOPPED /* Battle should be stopped */
};

/*
 * Description:
 */
enum class ValidStatus
{
  VALID,
  NOT_AFFORDABLE,
  SILENCED,
  NO_TARGETS,
  ZERO_COUNT,
  INVALID
};

/*
 * Description:
 */
enum class VictoryState
{
  DIM_BATTLE,
  FADE_IN_HEADER,
  SLIDE_IN_LOOT,
  SLIDE_IN_CARD,
  PROCESS_CARD,
  SLIDE_OUT_CARD,
  SLIDE_OUT_LOOT,
  FADE_OUT,
  FINISHED
};

/*
 * Description: The window display classifier to define how it's displayed:
 *
 *  OFF - not showing
 *  SHOWING - rising from the bottom
 *  ON - fully displayed
 *  HIDING - lowering to the bottom
 */
enum class WindowStatus : std::uint8_t
{
  OFF,
  SHOWING,
  ON,
  HIDING
};

#endif // ENUMDB_H
