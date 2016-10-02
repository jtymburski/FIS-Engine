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
*                   i -- effect on apply() including per ter effects, flags
*                        flipped, stat changes, etc.
*                  ii -- effect on unapply(), best removal possible of the
*                        ailment--return to normal (except for other
*                        afflictions the user might be suffering from).
*                 iii -- some ailments effects
*                        are not practical to have an apply() effect per se
*                        but alter the flow of combat, such as berserk, and so
*                        they will be handled in Battle.
*                  iv -- remember to fully utilize any Signals and Slots which
*                        the class may use
*              5 - (Add) the Ailment's immunities based off Person flags
*              6 - (Add) any effects or checks in Battle or subclasses
*
* TODO: Create a stubulate effect [08-04-13]
*******************************************************************************/
#ifndef AILMENT_H
#define AILMENT_H

#include <limits>

#include "EnumFlags.h"
#include "EnumDb.h"
#include "Game/Battle/BattleStats.h"

/* Enumerated AilState Flags */
ENUM_FLAGS(AilState)
enum class AilState
{
  CURABLE_TIME = 1 << 0,
  CURABLE_KO = 1 << 1,
  CURABLE_DEATH = 1 << 2,
};

class Ailment
{
public:
  /* Cannot construct an empty Ailment object */
  Ailment() = delete;

  /* Minimal constructor */
  Ailment(Infliction type, BattleStats* stats_victim);

  /* Detailed constructor */
  Ailment(Infliction type, BattleStats* stats_victim, uint32_t min_turns,
          uint32_t max_turns, double chance);

  /* Annihilates an AttributeSet object */
  ~Ailment() = default;

private:
  /* Reference to the stats */
  BattleStats* stats_victim;

  /* The classification of the Ailment */
  AilmentClass ailment_class;

  /* Chance the status effect will wear off per turn (>1 = 100%, 0 = 0%) */
  double cure_chance;

  /* The damage amount present for the Ailment */
  int32_t damage_amount;

  /* Set of flags for the current ailment */
  AilState flag_set;

  /* Durations of the status_ailment */
  uint32_t min_turns_left;
  uint32_t max_turns_left;
  uint32_t total_turns;

  /* Inflinction of the Ailment */
  Infliction type;

  /* The status for updating the ailment */
  AilmentStatus update_status;

  /*------------------- Buff Constants -----------------------*/
  static const float kPC_ALL_ATK_BUFF;
  static const float kPC_ALL_DEF_BUFF;
  static const float kPC_PHYS_BUFF;
  static const float kPC_ELEMENTAL_BUFF;
  static const float kPC_LIMB_BUFF;
  static const float kPC_UNBR_BUFF;
  static const float kPC_VITA_BUFF;
  static const float kPC_QTDR_BUFF;

  /*------------------- Damaging Constants -----------------------*/
  static const float kPOISON_DMG_INIT; /* % damage for Turn # 1 for Poison */
  static const float kPOISON_DMG_INCR; /* % chance / turn for Poison */
  static const float kHIBER_PC_INIT;   /* % heal for 1st turn */
  static const float kHIBER_PC_INCR;   /* incr for per turn heal */
  static const float kHIBER_PC_MAX;    /* max value for hibernation */

  /* Confusion / Paralysis Chances */
  static const float kCONFUSION_CHANCE;
  static const float kPARALYSIS_CHANCE;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Calculates the amount of poison damage, assigns to damage_amount */
  bool calcPoisonDamage();
  bool calcHibernationHeal();

  /* Does the ailment cure this turn? */
  bool doesAilmentCure();

  /* Do confusion and paralysis occur? */
  bool doesConfusionOccur();
  bool doesParalysisOccur();

  /* Updates the effect of the Ailment for the turn */
  AilmentStatus updateEffect();

  /* Updates the ailment by decrementing the turn counter if necessary */
  void updateTurnCount();

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Applies a Buff to the BattleStats of the inflicted BattleActor's stats */
  bool applyBuffs();

  /* Updates the ailment for a turn, returns the amount of damage */
  void update();

  /* Returns the assigned amount of damage */
  int32_t getDamageAmount();

  /* Evaluates an ailment flag or flags */
  bool getFlag(AilState flags);

  /* Returns the enumerated type of the Ailment */
  Infliction getType();

  /* Returns the enumerated update status */
  AilmentStatus getUpdateStatus();

  /* Sets the value of an AilmentFlag to a set_value, defaulting to true */
  void setFlag(const AilState& flags, const bool& set_value = true);

  /* Assigns the update status to the ailment */
  void setUpdateStatus(AilmentStatus new_status);

  /*===================== PUBLIC STATIC FUNCTIONS ============================*/
public:
  /* Returns the classification of an ailment from a given Infliction */
  static AilmentClass getClassOfInfliction(Infliction type);
};

#endif
