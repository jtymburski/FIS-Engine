/******************************************************************************
 * TODO
 *****************************************************************************/
#ifndef ENUMDATABASE_H
#define ENUMDATABASE_H

/* Flags for Action class */
enum ActionType
{
  HEALS           = 1ul <<  0, /* Lower or raise vit flag */
  INFLICTS        = 1ul <<  1, /* Inflict or cure flag */
  THERMAL         = 1ul <<  2, /* Is thermal in nature */
  POLAR           = 1ul <<  3, /* Is polar in nature */
  PRIMAL          = 1ul <<  4, /* Is primal in nature */
  CHARGED         = 1ul <<  5, /* Is charged in nature */
  CYBERNETIC      = 1ul <<  6, /* Is cybernetic in nature */
  NIHIL           = 1ul <<  7, /* Is nihil in nature */
  COUNTERABLE     = 1ul <<  8, /* Possibility of a counter action */
  BLOCKABLE       = 1ul <<  9, /* Possibility to block the attack */
  IGNOREDEFBUFFS  = 1ul << 10, /* Action ignores all defensive users buffs */
  IGNOREATKBUFFS  = 1ul << 11, /* Action ignores all offensive users buffs */
  INSTCRITICAL    = 1ul << 12, /* 100% chance of critical hit */
  HIGHCRITICAL    = 1ul << 13, /* Double (<= 100%) chance of critical hit */
  CAUSESSTATUS    = 1ul << 14, /* Action can or will cause a status effect */
  MODSSPEED       = 1ul << 15, /* Can modify the speed of a user's attacks */
  DEFENSIVE       = 1ul << 16, /* Is the action defensive in nature? */
  SINGLEUSE       = 1ul << 17, /* Can the action be used more than once? */
  MULTIHIT        = 1ul << 18, /* Can the action hit more than one target */
  DAMAGE          = 1ul << 19, /* Does the action cause nonzero damage? */
  INSTANTKILL     = 1ul << 20, /* Does the action cause an instant kill? */
  MELEE           = 1ul << 21, /* Is the action a melee or ranged attack? */
  SELF            = 1ul << 22, /* Does the action only work on the user? */
  PARTY           = 1ul << 23, /* Does itstrike all members on one party? */
  PARTYIGNOREUSER = 1ul << 24, /* Strikes members of party except user */
  ALLIGNOREUSER   = 1ul << 25, /* Does the action strike all except user? */
  MULTISTRIKE     = 1ul << 26 /* Does the action have more than one strike? */
};

enum ApplicationMode {TITLESCREEN, GAME};
enum Direction {NORTH,EAST,SOUTH,WEST};

/* Flags for Equipment class (in addition to item) */
enum EquipmentFlags
{
  WEAPON     = 1ul << 0, /* Is the item a weapon? */
  METAL      = 1ul << 1, /* Is the equipment metal? */
  BROKEN     = 1ul << 2, /* Is the item currently equippable? */
  HEAD       = 1ul << 3, /* Can be attached to the head */
  LEFTARM    = 1ul << 4, /* Can be attached to the left arm */
  RIGHTARM   = 1ul << 5, /* Can be attached to the right arm */
  TORSO      = 1ul << 6, /* Can be attached to torso */
  LEGS       = 1ul << 7, /* Can be attached to legs slot */
  TWOHAND    = 1ul << 8, /* Does the item require both hands? */
  EQUIPPED   = 1ul << 9  /* Is the item currently equipped to a person? */
};

/* Flags for Item class */
enum ItemFlags
{
  BATTLEREADY   = 1ul << 0, /* Can the item be used in battle */
  MENUREADY     = 1ul << 1, /* Can the item be used in the menu? */
  CONSUMABLE    = 1ul << 2, /* (BATTLEREADY || MENUREADY) && !INDEFINITE */
  HEALITEM      = 1ul << 3, /* Does the item heal vitality? */
  CURE          = 1ul << 4, /* Does the item cure ailments? */
  OFFENSIVE     = 1ul << 5, /* Does the item have an offensive battle use? */
  INDEFINITE    = 1ul << 5, /* If item has infinite uses */
  STACKABLE     = 1ul << 6, /* If item can be grouped with others */
  EQUIPMENT     = 1ul << 7, /* Is the item a piece of equipment? */
  KEYITEM       = 1ul << 8, /* is the item a unique quest item? */
  MULTIITEM     = 1ul << 9,  /* Does the item hit more than one target? */
  PARTYTITEM    = 1ul << 10 /* Does the item effect all members of a party? */
};

#endif // ENUMDATABASE_H
