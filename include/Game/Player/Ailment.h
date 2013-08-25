/*******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: N/A
* Description: The ailment class represents a Status Ailment or Debuff. Ailments
*              use the Infliction enumerated type to store their type as well
*              as the EnumString class to conver the enumerations to strings
*              instead of doing string compare (note that these are std::strings
*
* Note [1]: To add, remove or change a status ailment:
*              1 - (Add) an enumerated value to Infliction enum EnumDatabase.h
*              2 - (Add) a registered string to BEGIN_ENUM_STRING( Infliction )
*              3 - [(Add)] const static values to be used if necessary to class
*              4 - (Add) the Ailment's effect to apply(), and other flags
*              5 - (Add) the Ailment's immunities based off Person flags
*              6 - (Add) any effects or checks in Battle or subclasses
*******************************************************************************/
#ifndef AILMENT_H
#define AILMENT_H

#include <QtGui/QWidget>

#include "EnumDb.h"
#include "MathHelper.h"
#include "Game/Player/Person.h"

class Ailment : public QWidget
{
  Q_OBJECT

public:
  /* Default constructor */
  Ailment();

  /* Minimal Constructor */
  Ailment(Person* vic, QWidget* parent = 0);

  /* Constructor: Sets up ailment with type, turn and chance durations */
  Ailment(Person* vic, EnumDb::Infliction type, short max_turns = 0,
          double chance = 0, QWidget* parent = 0);

  /* Constructor: Sets up an ailment with a QString instead of an enum */
  Ailment(Person* vic, QString name, short max_turns = 0,
          double chance = 0, QWidget* parent = 0);

  /* Annihilates an AttributeSet object */
  ~Ailment();

  /*------------------- Enumerated QFlags -----------------------*/
  enum AilmentFlag
  {
    INFINITETIME = 1 << 0, /* Ailment does not alleviate by time? >kMAX_TURNS */
    CURABLE      = 1 << 1, /* Ailment cannot be cured except by time? */
    TOBECURED    = 1 << 1, /* The ailment is to be cured immediately */
    TOBEUPDATED  = 1 << 2, /* Ailment set to be updated on new turn */
    TOBEAPPLIED  = 1 << 3, /* Ailment effects set to be applied on new turn */
    BUFF         = 1 << 4, /* Is this ailment a favorable ailment? */
    ADVERSE      = 1 << 5, /* Is this ailment an adverse ailment? */
    NEUTRAL      = 1 << 6, /* (!FAVORABLE && !ADVERSE) */
    IMMUNITY     = 1 << 7, /* Is the inflicted person immune to this ailment? */
    CUREONDEATH  = 1 << 8  /* Does the ailment persist death? */
  };
  Q_DECLARE_FLAGS(AilmentFlags, AilmentFlag)

private:
  /* Inflinction of the Ailment */
  EnumDb::Infliction ailment_type;

  /* Chance the status effect will wear off per turn (>1 = 100%, 0 = 0%) */
  float chance;

  /* Set of flags for the current ailment */
  AilmentFlags flag_set;

  /* Durations of the status_ailment (>KMAX_TURNS = INFINITY) */
  short max_turns_left;
  short turns_occured;

  /* The victim (owner) of the ailment. */
  Person* victim;

  /* Checks the immunity of the ailment */
  bool checkImmunity(Person* new_victim);

  /*------------------- Constants -----------------------*/
  static const ushort kMAX_TURNS; /* Maximum # turns ailments will last */
  static const ushort kMIN_TURNS; /* The minimum # turns ailments last */
  static const uint   kPOISON_DMG_MAX; /* The max. daamge from Poison ailment */
  static const uint   kPOISON_DMG_MIN; /* The min. damage from Poison ailment */
  static const double kPOISON_DMG_INCR; /* Additional % per turn for Poison */
  static const double kPOISON_DMG_INIT; /* Initial % per turn for Poison */
  static const uint   kBURN_DMG_MAX; /* The max. damage from Burn */
  static const uint   kBURN_DMG_MIN; /* The min. damage from Burn */
  static const double kBURN_DMG_INCR; /* The increment (amt) for Burn lvls */
  static const double kBURN_DMG_INIT; /* The inital dmg caused by Burn */
  static const double kBURN_DMG_PC; /* Additional % dmg causedby Burn */
  static const double kBERSERK_DMG_INCR; /* % incr in damage against target */
  static const double kBERSERK_HITBACK_PC; /* % hitback on victim */
  static const uint   kBUBBIFY_MAX_QD; /* Maximum skill's QD cost when Bubby */
  static const double kBUBBIFY_STAT_MULR; /* % mod for stats while a Bubby */
  static const double kPARALYSIS_PC; /* % chance paralysis will skip turn */
  static const double kBLIND_PC; /* % chance Blind will miss attacks */
  static const double kDREADSTRUCK_PC; /* % Dreadstruck will miss attacks */
  static const double kDREAMSNARE_PC; /* % Dreamsnare causes attacks to miss */
  static const double kALLBUFF_PC; /* % by which all buffs will raise stats */
  static const double kPHYSBUFF_PC; /* % by which phys buffs will raise stats */
  static const double kELMBUFF_PC; /* % by which elm buffs will raise stats */
  static const double kLIMBUFF_PC; /* % value by which to incr limb stats */
  static const double kUNBBUFF_PC; /* % value by which to incr unbbuff stats */
  static const double kMOMBUFF_PC; /* % value by which to incr momuff stats */
  static const double kVITBUFF_PC; /* %  by which to incr vitality stats */
  static const double kQTMNBUFF_PC; /* % values by which to incr QD stats */
  static const double kROOTBOUND_PC; /* % values by which rootbound increases */
  static const double kHIBERNATION_INIT; /* % value for initial hibernation */
  static const double kHIBERNATION_INCR; /* % value by which to increase Hib */
  static const double kMETABOLIC_PC; /* % chance for metabolic teth to kill */
  static const double kMETABOLIC_DMG; /* % damage metabolic teth will deal */
  static const double kBOND_STATS_PC; /* % by which bond wil increase stats */

/*============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Applies the effect of the ailment */
  void apply();

  /* Updates the ailment by decrementing the turn counter if necessary */
  bool updateTurns();

  /* Sets the Inflinction of the Status Ailment */
  void setType(EnumDb::Infliction t);

  /* Assigns the victim of the object */
  void setVictim(Person* set_victim);

/*============================================================================
 * PUBLIC SLOTS
 *============================================================================*/
public slots:
  /* Handles ailment flag work on actor death */
  void death();

  /* Resets the ailment to an original state */
  void reset();

  /* Updates the ailment */
  void update();

/*============================================================================
 * SIGNALS
 *============================================================================*/
signals:
  /* Tells BIB and BSB an ailment of victim is being inflicted */
  void inflicting(QString victim_name, EnumDb::Infliction ailment_name);

  /* Tells Battle to remove all buffs
     (usually happens when Bubbify is added/removed) */
  void removeBuffs(QString victim_name);

  /* Tells BIB and BSB an ailment of victim is curing */
  void curing(QString victim_name, EnumDb::Infliction ailment_name);

  /* Tells Battle the ailment was reset */
  void resetComplete();

  /* Tells BIB and Battle that the ailment has been updated for the turn */
  void updated();

  /* Tells BIB and BSB that the victim dies because of reasons */
  void victimDeath(QString victim_name, EnumDb::ActorDeath reason);

  /*============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
  public:
    /* Undoes the effect (if exits) to the victim before curing */
    void unapply();

    /* Methods for printing all the information pertaining to the ailment */
    void printAll();
    void printFlags();
    void printInfo();

    /* Evaluates an ailment flag or flags */
    bool getFlag(AilmentFlag flags);

    /* Returns the number of turns left (assuming 0%) */
    ushort getTurnsLeft();

    /* Returns the Inflinction of the status ailment */
    EnumDb::Infliction getType();

    /* Gets a QString of the current ailment's enumerated value */
    QString getName();

    /* Obtains the victim of the Status Ailment */
    Person* getVictim();

    /* Sets the duration of the ailment */
    void setDuration(short max_turns, double chance = 0);

    /* Sets the value of an AilmentFlag to a set_value, defaulting to true */
    void setFlag(AilmentFlag flags, bool set_value = true);

    /* Public function to assign a new victom for the status ailment */
    bool setNewVictim(Person* new_victim, bool refresh_turns = false);

    /* Returns a QString corresponding to an enumerated Infliction */
    QString getAilmentStr(EnumDb::Infliction type);

    /* Returns an enumerated Infliction corresponding to a QString */
    EnumDb::Infliction getInfliction(QString name);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Ailment::AilmentFlags)

#endif // AILMENT_H
