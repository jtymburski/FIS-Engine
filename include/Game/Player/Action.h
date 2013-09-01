/*******************************************************************************
* Class Name: Action
* Date Created: October 28th, 2012
* Updated: September 1st, 2013
* Inheritance: None
* ------------------------------------------------------------------------------
* Description: Action is an element of a skill. A Skill may have a number of
* actions which have various effects: altering stats, flipping flags, damage,
* inflicting ailments, etc. The effect of actions is built by parsing a scripted
* string from a file. Every action has a unique ID.
*******************************************************************************/
#ifndef ACTION_H
#define ACTION_H

#include <QDebug>
#include <QStringList>

#include "EnumDb.h"

class Action
{
public:
  /* Default action constructor */
  Action();

  /* Action constructor object (requires a QString of raw input) */
  Action(QString raw);

  /* Annihilates an action object */
  ~Action();

  /* Enumerated ActionType flags */
  enum ActionType
  {
    RAISE         = 1 <<  1, /* Does the action raise a stat? */
    LOWER         = 1 <<  2, /* Does the action lower a stat? */
    GIVE          = 1 <<  3, /* Does the action give a status ailment? */
    TAKE          = 1 <<  4, /* Does the action take a status ailment away? */
    OFFENSIVE     = 1 <<  5, /* Does the action affect offensive stats? */
    DEFENSIVE     = 1 <<  6, /* Does the action affect defensive stats? */
    PHYSICAL      = 1 <<  7, /* Does the action affect physical stats? */
    THERMAL       = 1 <<  8, /* Does the action affect thermal stats? */
    POLAR         = 1 <<  9, /* " polar stats? */
    PRIMAL        = 1 << 10, /* " primal stats? */
    CHARGED       = 1 << 11, /* " charged stats? */
    CYBERNETIC    = 1 << 12, /* " cybernetic stats? */
    NIHIL         = 1 << 13, /* " nihil stats? */
    VITALITY      = 1 << 14, /* " vitality stat? */
    QUANTUM_DRIVE = 1 << 15, /* " quantum drive stat? */
    MOMENTUM      = 1 << 16, /* " speed stat? */
    LIMBERTUDE    = 1 << 17, /* " dodge chance stats? */
    UNBEARABILITY = 1 << 18, /* " unbearability stat? */
    VALID_ACTION   = 1 << 19  /* Is the action a valid action? */
  };
  Q_DECLARE_FLAGS(ActionFlags, ActionType)

  /* Enumerated IgnoreFlags */
  enum IgnoreFlag
  {
    IGNORE_ELMN_ATK = 1 <<  1,
    IGNORE_PHYS_ATK = 1 <<  2,
    IGNORE_THER_ATK = 1 <<  3,
    IGNORE_POLA_ATK = 1 <<  4,
    IGNORE_PRIM_ATK = 1 <<  5,
    IGNORE_CHAR_ATK = 1 <<  6,
    IGNORE_CYBE_ATK = 1 <<  7,
    IGNORE_NIHI_ATK = 1 <<  8,
    IGNORE_ATK      = 1 <<  9,
    IGNORE_ELMN_DEF = 1 << 10,
    IGNORE_PHYS_DEF = 1 << 11,
    IGNORE_THER_DEF = 1 << 12,
    IGNORE_POLA_DEF = 1 << 13,
    IGNORE_PRIM_DEF = 1 << 14,
    IGNORE_CHAR_DEF = 1 << 15,
    IGNORE_CYBE_DEF = 1 << 16,
    IGNORE_NIHI_DEF = 1 << 17,
    IGNORE_DEF      = 1 << 18
  };
  Q_DECLARE_FLAGS(IgnoreFlags, IgnoreFlag)

private:
  /* Enumerated flag sets */
  ActionFlags action_flags;
  IgnoreFlags ignore_flags;

  /* Enumeration of the ailment the action may inflict */
  EnumDb::Infliction ailment;

  /* Value the effected stat will alter temporarily */
  uint base_change;

  /* ID of the action: Positive for status effects, negative for others */
  int id;

  /* Maximum duration the action will take */
  ushort max_duration;

  /* Minimum duration the action will take */
  ushort min_duration;

  /* Variance of base change (percentage) */
  double variance;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* The parsing script for action input */
  void parse(QString raw);

  /* Assigns an enumerated ailment */
  void setAilment(EnumDb::Infliction new_ailment);

  /* Assigns a given ActionType flag a given boolean value */
  void setActionFlag(ActionType flags, bool set_value = true);

  /* Assigns a base change value that will effect the given stat */
  void setBaseChange(uint new_value);

  /* Assigns a duration of the action (min 1, max 1 means single turn action) */
  void setDuration(uint min, uint max);

  /* Assigns the unique ID of the action */
  void setId(int id);

  /* Assigns a given IgnoreFlag flag a given boolean value */
  void setIgnoreFlag(IgnoreFlag flags, bool set_value = true);

  /* Variance of the base change (percentage) */
  void setVariance(float new_value);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Methods for test-printing the action class */
  void printAll();
  void printInfo();
  void printActionFlags();
  void printIgnoreFlags();

  /* Returns the truth of an Action Type Flag */
  bool getActionFlag(ActionType flags);

  /* Returns an enumeration of the action's status ailment infliction */
  EnumDb::Infliction getAilment();

  /* Returns the base change of the action */
  uint getBaseChange();

  /* Returns the ID of an action */
  int getId();

  /* Returns the truth of an Ignore Atk Flag */
  bool getIgnoreFlag(IgnoreFlag flags);

  /* Returns the maximum turns of action duration */
  uint getMaximum();

  /* Returns the minimum turns of action duration */
  uint getMinimum();

  /* Returns the variance of the action */
  double getVariance();

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns a QString corresponding to an enumerated Infliction */
  static QString getAilmentStr(EnumDb::Infliction type);

  /* Returns an Enumeration of an ailment given a QString */
  static EnumDb::Infliction getInfliction(QString ailment);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Action::IgnoreFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Action::ActionFlags)

#endif // ACTION_H
