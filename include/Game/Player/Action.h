/******************************************************************************
* Class Name: Action
* Date Created: Sunday, October 28th, 2012
* Inheritance: 
* Description: Action is an element of a skill (skill can have up to 10 actions
* that will do alter stats, flip flags of a person, etc., or do damage to them.
*
* TODO: Constructors to be finished [02-17-13]
* TODO: Need to write action parser [02-17-13]
******************************************************************************/
#ifndef ACTION_H
#define ACTION_H

#include <QtGui/QWidget>

#include "QDebug"

class Action : public QWidget
{
public:
  /* Action constructor object (requires a QString of raw input) */
  Action(QString raw, QWidget *parent = NULL);

  /* Annihilates an action object */
  ~Action();

  /* Enumerated ActionType * IgnoreFlag flags */
  enum ActionType
  {
    RAISE         = 1 <<  0, /* Does the action raise a stat? */
    LOWER         = 1 <<  1, /* Does the action lower a stat? */
    GIVE          = 1 <<  2, /* Does the action give a status ailment? */
    TAKE          = 1 <<  3, /* Does the action take a status ailment away? */
    OFFENSIVE     = 1 <<  4, /* Does the action affect offensive stats? */
    DEFENSIVE     = 1 <<  5, /* Does the action affect defensive stats? */
    PHYSICAL      = 1 <<  6, /* Does the action affect physical stats? */
    THERMAL       = 1 <<  7, /* Does the action affect thermal stats? */
    POLAR         = 1 <<  8, /* " polar stats? */
    PRIMAL        = 1 <<  9, /* " primal stats? */
    CHARGED       = 1 << 10, /* " charged stats? */
    CYBERNETIC    = 1 << 11, /* " cybernetic stats? */
    NIHIL         = 1 << 12, /* " nihil stats? */
    VITALITY      = 1 << 13, /* " vitality stat? */
    QUANTUM_DRIVE = 1 << 14, /* " quantum drive stat? */
    MOMENTUM      = 1 << 15, /* " speed stat? */
    LIMBERTUDE    = 1 << 16, /* " dodge chance stats? */
    UNBEARABILITY = 1 << 17  /* " unbearability stat? */
  };
  Q_DECLARE_FLAGS(ActionFlags, ActionType)
  enum IgnoreFlag
  {
    IGNORE_ELMN_ATK = 1 <<  0,
    IGNORE_PHYS_ATK = 1 <<  1,
    IGNORE_THER_ATK = 1 <<  2,
    IGNORE_POLA_ATK = 1 <<  3,
    IGNORE_PRIM_ATK = 1 <<  4,
    IGNORE_CHAR_ATK = 1 <<  5,
    IGNORE_CYBE_ATK = 1 <<  6,
    IGNORE_NIHI_ATK = 1 <<  7,
    IGNORE_ATK      = 1 <<  8,
    IGNORE_ELMN_DEF = 1 <<  9,
    IGNORE_PHYS_DEF = 1 << 10,
    IGNORE_THER_DEF = 1 << 11,
    IGNORE_POLA_DEF = 1 << 12,
    IGNORE_PRIM_DEF = 1 << 13,
    IGNORE_CHAR_DEF = 1 << 14,
    IGNORE_CYBE_DEF = 1 << 15,
    IGNORE_NIHI_DEF = 1 << 16,
    IGNORE_DEF      = 1 << 17
  };
  Q_DECLARE_FLAGS(IgnoreFlags, IgnoreFlag)

private:
  /* Enumerated flag sets */
  ActionFlags action_flags;
  IgnoreFlags ignore_flags;

  /* String of Ailment (if exists) the action inflicts */
  QString action_ailment;

  /* ID of the action: Positive for status effects, negative for others */
  int id;

  /* Minimum duration the action will take */
  uint min_duration;

  /* Maximum duration the action will take */
  uint max_duration;

  /* Value the effected stat will alter temporarily */
  uint base_change;

  /* Variance of base change (percentage) */
  float variance;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* The raw language parser! */
  void parse(QString raw);

  /* Sets the ailment string */
  void setAilment(QString ailment);

  /* Sets the value of an Action Type Flag */
  void setActionFlag(ActionType flags, bool set_value = true);

  /* Sets the base change value that will effect the given stat */
  void setBaseChange(uint new_value);

  /* Sets the duration of the action (min 1, max 1 means single turn action) */
  void setDuration(uint min, uint max);

  /* Sets the *UNIQUE* ID of the action */
  void setId(int id);

  /* Sets the value of an Ignore Atk Flag */
  void setIgnoreFlag(IgnoreFlag flags, bool set_value = true);

  /* Variance of the base change (percentage) */
  void setVariance(float new_value);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  /* Methods for test-printing */
  void printAll();
  void printInfo();
  void printFlags();

  /* Gets the value of an Action Type Flag */
  bool getActionFlag(ActionType flags);

  /* Returns the affected status ailment */
  QString getAilment();

  /* Gets the base change of the action */
  uint getBaseChange();

  /* Gets the ID of an action NOTE: Ensure Unique */
  int getId();

  /* Gets the value of an Ignore Atk Flag */
  bool getIgnoreFlag(IgnoreFlag flags);

  /* Returns the maximum # of turns */
  uint getMaximum();

  /* Returns the minimum # of turns */
  uint getMinimum();

  /* Returns the variance of the action */
  float getVariance();
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Action::IgnoreFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Action::ActionFlags)

#endif // ACTION_H
