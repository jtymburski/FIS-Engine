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

#include "EnumDb.h"
#include "EnumFlags.h"
#include "Helpers.h"
#include "Game/Player/Person.h"

/*------------------- Enumerated QFlags -----------------------*/
ENUM_FLAGS(AilState)
enum class AilState
{
  INFINITE      = 1 << 0,  /* Ailment does not alleviate by time? >kMAX_TURNS */
  CURABLE       = 1 << 1,  /* Ailment cannot be cured except by time? */
  TO_CURE       = 1 << 1,  /* The ailment is to be cured immediately */
  TO_UPDATE     = 1 << 2,  /* Ailment set to be updated on new turn */
  TO_APPLY      = 1 << 3,  /* Ailment effects set to be applied on new turn */
  TO_UNAPPLY    = 1 << 4,  /* Ailment effects to be unapplied [after update()]*/
  BUFF          = 1 << 5,  /* Is this ailment a favorable ailment? */
  ADVERSE       = 1 << 6,  /* Is this ailment an adverse ailment? */
  IMMUNITY      = 1 << 7,  /* Is the inflicted person immune to this ailment? */
  CURE_ON_DEATH = 1 << 8,  /* Does the ailment persist death? */
  VICTIM_SET    = 1 << 9,  /* Has the victim of the ailment been set? */
  INFLICTOR_SET = 1 << 10,
  DEALS_DAMAGE  = 1 << 11  /* Has the inflictor of the ailment been set? */
};

class Ailment
{
public:

  /* Minimal Constructor */
  Ailment(Person* ail_victim, const Infliction &type, 
      Person* inflictor = nullptr,  const uint16_t &max_turns = 1, 
      const uint16_t &min_turns = 1, const double &chance = 0);

  /* Annihilates an AttributeSet object */
  ~Ailment() = default;

private:
  /* Inflinction of the Ailment */
  Infliction type;

  /* Chance the status effect will wear off per turn (>1 = 100%, 0 = 0%) */
  double chance;

  /* Amount of damage computed for the ailment update on this turn */
  int32_t damage;

  /* The type of damage computed for the ailment update for the turn */
  DamageType damage_type;

  /* Set of flags for the current ailment */
  AilState flag_set;

  /* Durations of the status_ailment (>KMAX_TURNS = INFINITY) */
  uint16_t min_turns_left;
  uint16_t max_turns_left;
  uint16_t turns_occured;

  /* The person who inflicted the ailment */
  Person* inflictor;

  /* The victim (owner) of the ailment. */
  Person* victim;

  /*------------------- Constants -----------------------*/
  static const uint16_t kMAX_TURNS; /* Maximum # turns ailments will last */
  static const uint16_t kMIN_TURNS; /* The minimum # turns ailments last */
  static const uint32_t kPOISON_DMG_MAX; /* The max. dmg from Poison ailment */
  static const uint32_t kPOISON_DMG_MIN; /* The min. dmg from Poison ailment */
  static const double kPOISON_DMG_INCR; /* Additional % per turn for Poison */
  static const double kPOISON_DMG_INIT; /* Initial % per turn for Poison */
  static const uint32_t kBURN_DMG_MAX; /* The max. damage from Burn */
  static const uint32_t kBURN_DMG_MIN; /* The min. damage from Burn */
  static const double kBURN_DMG_INCR; /* The increment (amt) for Burn lvls */
  static const double kBURN_DMG_INIT; /* The inital dmg caused by Burn */
  static const double kBURN_DMG_PC; /* Additional % dmg causedby Burn */
  static const double kBERSERK_DMG_INCR; /* % incr in damage against target */
  static const double kBERSERK_HITBACK_PC; /* % hitback on victim */
  static const uint32_t kBUBBIFY_MAX_QD; /* Maximum skill's QD cost when Bubby */
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
  static const double kQDBUFF_PC; /* % values by which to incr QD stats */
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
  bool apply();

  /* Checks the immunity of the ailment */
  bool checkImmunity(Person* new_victim);

  /* Updates the ailment by decrementing the turn counter if necessary */
  bool updateTurns();

  /* Sets the Inflinction of the Status Ailment */
  void setType(const Infliction &type);

  /* Assigns the inflictor of the ailment */
  bool setInflictor(Person* set_inflictor);

  /* Assigns the victim of the object */
  bool setVictim(Person* set_victim);

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /*  */
  void death();

  /* Updates the ailment for upkeep condition */
  void update(bool update_turns = true);

  /* Reset the ailment to default conditions */
  void reset();

  /* Undoes the effect (if exits) to the victim before curing */
  void unapply();
  
  /* Determines whether to reapply affect upon recalculation */
  bool toReapplyFlags();

  /* Methods for printing all the information pertaining to the ailment */
  void print(const bool &simple = true, const bool &flags = false);

  /* Returns the amount of damage computed for the ailment update */
  int32_t getDamageAmount() const;

  /* Returns the type of damage computed for the ailment update */
  DamageType getDamageType() const;

  /* Evaluates an ailment flag or flags */
  bool getFlag(AilState flags);

  /* Returns the Inflictor of the ailment */
  Person* getInflictor();

  /* Returns the number of turns left (assuming 0%) */
  uint16_t getTurnsLeft();

  /* Returns the Inflinction of the status ailment */
  Infliction getType();

  /* Obtains the victim of the Status Ailment */
  Person* getVictim();

  /* Sets the duration of the ailment */
  void setDuration(const uint16_t &min_turns, const uint16_t &max_turns, 
                   const double &chance = 0);

  /* Sets the value of an AilmentFlag to a set_value, defaulting to true */
  void setFlag(const AilState &flags, const bool &set_value = true);

  /* Public function to assign a new victom for the status ailment */
  // bool setNewVictim(Person* new_victim, Person* new_inflictor = nullptr,
  //                   const bool &refresh_turns = false);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the maximum QD value for Skill useable by Bubbified person */
  static uint32_t getMaxBubbyQD();

  /* Returns the hitback % for Berserk */
  static double getBerserkHitbackPC();
};

#endif