/*******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: QWidget
* Description: The ailment class represents a Status Ailment or Debuff. Ailments
*              use the Infliction enumerated type to store their type as well
*              as the EnumString class to conver the enumerations to strings
*              instead of doing string compare (note that these are std::strings
*
* THE FOLLOWING HAS NOT BEEN UPDATED
* Note [1]: To add, remove or change a status ailment:
*              1 - (Add) an enumerated value to Infliction enum EnumDatabase.h
*              2 - (Add) a registered string to BEGIN_ENUM_STRING( Infliction )
*              3 - [(Add)] const static values to be used if necessary to class
*              4 - (Add) the Ailment's effects
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
*******************************************************************************/

#include "Game/Player/Ailment.h"

/*============================================================================
 * CONSTANTS (Explanation for each in header file)
 *============================================================================*/

/*------------------- Ailment Constants -----------------------
 * kMAX_TURNS - maximum number of turns an ailment can be inflicted for
 * kMIN_TURNS - minimum number of turns an ailment can be inflicted for
 * kPOISON_DAMAGE_MAX - upper limit the poison ailment can inflict
 * kPOISON_DAMAGE_MIN - minimum damage poison can do
 * kPOISON_DAMAGE_INCR - factor by which poison damage increments per turn
 * kPOISON_DMG_INIT - factor of victim's health to deal poison damage on
 * kBURN_DMG_MAX - upper limit of burn damage the ailment can inflict
 * kBURN_DMG_MIN - lower limit of burn damage the ailment can inflict
 * kBURN_DMG_INCR - factor by which to increase burn damage per burn level
 * kBURN_DMG_PC - factor of health by which to deal burn damage
 * kBERSERK_DMG_INCR - factor by which to increase damage of Berserked person
 * kBERSERK_HITBACK_PC - factor of health to deal self-damage of Berserked
 * kBUBBIFY_STAT_MULR - factor applied to a person's stats during Bubbification
 * kPARALYSIS_PC - normal chance for paralaysis skipping of turn
 * kBLIND_PC - normal chance of missing attacks while blind
 * kDREADSTRUCK_PC - normal chance of missing turn while dreadstruck
 * kDREAMSNARE_PC - normal chance of attacks being benign illusions
 *
 * ---- Ailment Buff Factors ----
 * kALLBUFF_PC - factor by which to increase all stats during all buff
 * kPHYSBUFF_PC - factor by which to increase physical stats during phys buff
 * kELMBUFF_PC - factor by which to increase elemental stats during elmn buff
 * kLIMBUFF_PC - factor by which to increase limbertude stat during limb buff
 * kUNBBUFF_PC - factor by which to incrase unbearability stat during unbr buff
 * kMOMBUFF_PC - factor by which to increase momentum stat during mmtm buff
 * kVITBUFF_PC - factor by which to increase health during vitality buff
 * kQDBUFF_PC - factor by which to increase quantum drive during qtmn buff
 *
 *
 * kROOTBOUND_PC - factor by which to heal health during Rootbound
 * kHIBERNATION_INIT - base factor by which to heal health during Hibernating
 * kHIBERNATION_INCR - factor by which to increase healing during Hibernating
 * kMETABOLIC_PC - percent chance to die during metabolic tether
 * kMETABOLIC_DMG - percent hit taken to health if death does not occur
 * kBOND_STATS_PC - factor of others stats' by which inflicted increases stats
 */
const uint16_t Ailment::kMAX_TURNS           =   25;
const uint16_t Ailment::kMIN_TURNS           =    1;
const uint32_t Ailment::kPOISON_DMG_MAX      = 8000;
const uint32_t Ailment::kPOISON_DMG_MIN      =   20;
const double   Ailment::kPOISON_DMG_INCR     = 1.15;
const double   Ailment::kPOISON_DMG_INIT     = 0.08;
const uint32_t Ailment::kBURN_DMG_MAX        = 5000;
const uint32_t Ailment::kBURN_DMG_MIN        =  100;
const double   Ailment::kBURN_DMG_INCR       = 1.50;
const double   Ailment::kBURN_DMG_PC         = 0.05;
const double   Ailment::kBERSERK_DMG_INCR    = 1.75;
const double   Ailment::kBERSERK_HITBACK_PC  = 0.35;
const double   Ailment::kBUBBIFY_STAT_MULR   = 0.75;
const double   Ailment::kPARALYSIS_PC        = 0.70;
const double   Ailment::kBLIND_PC            = 0.50;
const double   Ailment::kDREADSTRUCK_PC      = 0.75;
const double   Ailment::kDREAMSNARE_PC       = 0.50;
const double   Ailment::kALLBUFF_PC          = 1.05;
const double   Ailment::kPHYSBUFF_PC         = 1.08;
const double   Ailment::kELMBUFF_PC          = 1.07;
const double   Ailment::kLIMBUFF_PC          = 1.10;
const double   Ailment::kUNBBUFF_PC          = 1.05;
const double   Ailment::kMOMBUFF_PC          = 1.01;
const double   Ailment::kVITBUFF_PC          = 1.20;
const double   Ailment::kQDBUFF_PC           = 1.15;
const double   Ailment::kROOTBOUND_PC        = 0.15;
const double   Ailment::kHIBERNATION_INIT    = 0.15;
const double   Ailment::kHIBERNATION_INCR    = 0.05;
const double   Ailment::kMETABOLIC_PC        = 0.25;
const double   Ailment::kMETABOLIC_DMG       = 0.25;
const double   Ailment::kBOND_STATS_PC       = 0.35;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/* 
 * Description: General ailment constructor, constructs an ailment of a certain
 *              type (with a duration), given a person to be inflicted and an
 *              inflictor doing the inflicting.
 *
 * Inputs: Person* ail_victim - the subject of the Infliction
 *         Infliction ail_type - the type of the Infliction
 *         Person* ail_inflictor - the source of the Infliction
 *         uint16_t ail_min_turns - minimum duration of the ailment
 *         uint16_t ail_max_turns - maximum duration of the ailmen
 *         double chance - ???
 * Output
 */
Ailment::Ailment(Person* ail_victim, const Infliction &ail_type, 
    Person* ail_inflictor, const uint16_t &ail_max_turns, 
    const uint16_t &ail_min_turns, const double &ail_chance)
      : type{ail_type}
      , chance{ail_chance}
      , damage{0}
      , damage_type{DamageType::BASE}
      , flag_set{static_cast<AilState>(0)}
      , turns_occured{0}
      , inflictor{ail_inflictor}
      , victim{ail_victim}
{
  if (type == Infliction::INVALID)
  {
    chance          = 0;
    max_turns_left  = 0;
    victim    = nullptr;
    inflictor = nullptr;
  }
  else
  {
    if (ail_victim != nullptr)
      //if (!checkImmunity(ail_victim))
        setFlag(AilState::VICTIM_SET, true);

    if (inflictor != nullptr)
      setFlag(AilState::INFLICTOR_SET, true);

    setDuration(ail_min_turns, ail_max_turns, chance);

    setFlag(AilState::TO_UPDATE, true);
    setFlag(AilState::TO_APPLY,  true);
  }
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Applies the effect of the status ailment to the Person (victim)
 *              if one so exists, checking for recurring effects, etc.
 *
 * Note [1]: The following are descriptions of ailments which have handling in
 *           the Battle class.
 *
 *           Hellbound - If this actor dies, another living actor on the same
 *                       team KOs, this actor will KO as well.
 *           Confuse   - Ailed actor performs random useable skill on a random
 *                       target.
 *           Curse     - Ailed actor is inflicted with a random ailment each
 *                       turn. Curse has a small chance of inflicting a new
 *                       Curse (turns are reset essentially)
 *
 * Inputs: none
 * Output: bool - true if the ailment is to be cured
 */
bool Ailment::apply()
{
  if (victim == nullptr)
    return false;

  /* Helper variables */
  const auto& kHEALTH   = victim->getCurr().getStat(Attribute::VITA);
  auto& stats           = victim->getCurr();
  auto& max_stats       = victim->getTemp();
  auto  skills          = victim->getCurrSkills();

  /* Poison: Ailed actor takes increasing hit to HP every turn
   * Constants: kPOISON_DMG_MAX -- maximum amount poison damage can do
   *            kPOISON_DMG_MIN -- minimum amount poison damage can do
   *            kPOISON_DMG_INCR -- % increase poison damage does
   *            kPOISON_DMG_INIT -- % dmg incurred by poison initially
   */
  if (type == Infliction::POISON)
  {
    auto min_value = static_cast<int32_t>(kPOISON_DMG_MIN);
    auto max_value = static_cast<int32_t>(kPOISON_DMG_MAX);

    if (min_value < (kPOISON_DMG_INIT * kHEALTH))
      min_value = kPOISON_DMG_INIT * kHEALTH;

    if (max_value > stats.getStat(Attribute::VITA))
      max_value = stats.getStat(Attribute::VITA);

    if (turns_occured > 0)
    {
      auto damage_amount = min_value *
          std::pow(static_cast<float>(kPOISON_DMG_INCR), turns_occured);
      damage = static_cast<int32_t>(std::floor(damage_amount));
    }
 
    damage = Helpers::setInRange(damage, min_value, max_value);
    damage_type = DamageType::POISON;

    setFlag(AilState::DEALS_DAMAGE, true);
  }

  /* Burn/Scald/Char - The three increasing levels of Burn
   * Burn - light damage over a few turns
   * Scald - moderate damage over a few more turns
   * Char - heavy damage over a long period
   * Constant: kBURN_DMG_MIN - minimum amt of burn damage
   *           kBURN_DMG_MAX - maximum amt of burn damage
   *           kBURN_DMG_INCR - % incr burn damage per additional lvl
   *           kBURN_DMG_INIT - % dmg incurred by level 1 burn
   */
  else if (type == Infliction::BURN || type == Infliction::SCALD || 
           type == Infliction::CHARR)
  {
    auto min_value = static_cast<int32_t>(kBURN_DMG_MIN);
    auto max_value = static_cast<int32_t>(kBURN_DMG_MAX);

    damage = min_value;
    if (damage < (kBURN_DMG_INCR * kBURN_DMG_PC))
      damage = kBURN_DMG_INCR * kBURN_DMG_PC;

    if (max_value > stats.getStat(Attribute::VITA))
      max_value = stats.getStat(Attribute::VITA);

    if (type == Infliction::SCALD)
      damage *= kBURN_DMG_INCR;
    if (type == Infliction::CHARR)
      damage *= kBURN_DMG_INCR;

    damage = Helpers::setInRange(damage, min_value, max_value);
    damage_type = DamageType::BURN;
    
    setFlag(AilState::DEALS_DAMAGE, true);
  }
  
  /* Berserk - Ailed actor physically attacks enemy target for extreme damage
   *           while receiving damage themselves. Hitback damage will take
   *           place in Battle.
   * Constants: kBERSERK_DMG_INCR - multiplier for normal damage on target
   *            kBERSERK_HITBACK_PC - multiplier for damage done to self.
   */
  else if (type == Infliction::BERSERK)
  {
    /* On initial application, disable non physical skills and running */
    victim->setBFlag(BState::SKL_ENABLED, false);
    victim->setBFlag(BState::DEF_ENABLED, false);
    victim->setBFlag(BState::GRD_ENABLED, false);
    victim->setBFlag(BState::ITM_ENABLED, false);
    victim->setBFlag(BState::RUN_ENABLED, false);
    victim->setBFlag(BState::PAS_ENABLED, false);
    victim->setDmgMod(2);

    setFlag(AilState::TO_APPLY, false);
    setFlag(AilState::TO_UNAPPLY, true);
  }

  /* Ailed actor cannot use skills if they require QD */
  else if (type == Infliction::SILENCE)
  {
    /* On application, remove skills which have a QD cost > 0 from useable */
    for (uint32_t i = 0; i < skills->getSize(); i++)
      if (skills->getElement(i).skill != nullptr)
        if (skills->getElement(i).skill->getCost() > 0)
          skills->setState(i, false);

    setFlag(AilState::TO_APPLY, false);
    setFlag(AilState::TO_UNAPPLY, true);
  }

  /* Bubbify - ailed actor is turned into a near-useless Bubby */
  else if (type == Infliction::BUBBIFY)
  {
    /* Decrease the stats of the person by a factor of kBUBBIFY_STAT_MULR */
    for (uint32_t i = 0; i < stats.getSize(); i++)
    {
      stats.setStat(i, stats.getStat(i) * kBUBBIFY_STAT_MULR);
      max_stats.setStat(i, stats.getStat(i) * kBUBBIFY_STAT_MULR);
    }

    /* Flip the Battle State Bubby flag */
    victim->setBFlag(BState::IS_BUBBY, true);
    setFlag(AilState::TO_APPLY, false);
    setFlag(AilState::TO_UNAPPLY, true);
  }

  /* Death Timer - Ailed actor KOs upon reaching max_turns */
  else if (type == Infliction::DEATHTIMER)
  {
    if (turns_occured >= max_turns_left)
      setFlag(AilState::TO_KILL, true);
  }

  /* Paralysis - Ailed actor has a kPARALYSIS_PC chance of skipping their turn,
   *             the effect persisting or a number of turns
   * Constants: kPARALYSIS_PC - Percent chance to skip each turn */
  else if (type == Infliction::PARALYSIS)
  {
    if (Helpers::chanceHappens((kPARALYSIS_PC * 100), 100))
      victim->setBFlag(BState::SKIP_NEXT_TURN, true);
  }

  /* Blindness - Ailed actor has a kBLIND_PC chance of missing targets
   * Constants: kBLIND_PC - Percet chance for ailed actor to miss targets
   */
  else if (type == Infliction::BLINDNESS)
  {
    if (Helpers::chanceHappens((kBLIND_PC * 100), 100))
      victim->setBFlag(BState::MISS_NEXT_TARGET, false);
  }

  /* Dreadstruck - formerly "Stun": Ailed actor has an extreme chance of
   *               skipping just one turn
   * Constants: kDREADSTRUCK_PC - Percent chance of skipping their next turn.
   */
  else if (type == Infliction::DREADSTRUCK)
  {
    if (Helpers::chanceHappens((kDREADSTRUCK_PC), 100))
      victim->setBFlag(BState::SKIP_NEXT_TURN, false);
  }

  /* Dreamsnare - Ailed actor's actions have a kDREAMSNARE_PC chance of being
   *              benign illusions (no effect)
   * Constants: kDREAMSNARE_PC - Percent chance of no effect attacks
   */
  else if (type == Infliction::DREAMSNARE)
  {
    if (Helpers::chanceHappens((kDREAMSNARE_PC * 100), 100))
      victim->setBFlag(BState::NEXT_ATK_NO_EFFECT, true);
  }

  /* Bond & Bonded - Two actors are afflicted simultaneously. One person is
   * Bonded to the other. The Bond person is essentially in a dead state, but
   * the other assumes a kBOND_STATS_PC percent of their statistical boost.
   * However, if the Bonded actor dies, the Bond actor wll also die (this
   * case will be handled in Battle)
   * Constants: kBOND_STATS_PC - % by which to increase actor's stats.
   */
  else if (type == Infliction::BOND)
  {
    victim->setBFlag(BState::BOND, true);
  }
  else if (type == Infliction::BONDED)
  {
    for (uint32_t i = 0; i < stats.getSize(); i++)
      stats.setStat(i, stats.getStat(i) * (1 + kBOND_STATS_PC));
  }

  /* Buffs -- Increases the user's stats by a specified amount on application
   * Constants: kALLBUFF_PC  - % to incr all values
   *            kPHYSBUFF_PC - % to incr phys values
   *            kELMBUFF_PC  - % to incr elemental values
   */
  /* Remove the buffing effects from all attack stats */
  else if (getType() == Infliction::ALLATKBUFF)
  {
    setFlag(AilState::BUFF, true);

    for (uint32_t i = 2; i < stats.getSize(); i++)
      if (i % 2 == 0 && i < 16)
        stats.setStat(i, stats.getStat(i) * kALLBUFF_PC);
  }

  /* Remove the buffing effect from all defense stats */
  else if (getType() == Infliction::ALLDEFBUFF)
  {
    setFlag(AilState::BUFF, true);
    
    for (uint32_t i = 2; i < stats.getSize(); i++)
      if (i % 2 != 0 && i < 16)
        stats.setStat(i, stats.getStat(i) * kALLBUFF_PC);
  }

  else if (getType() == Infliction::PHYBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(2, stats.getStat(2) * kPHYSBUFF_PC);
    stats.setStat(3, stats.getStat(3) * kPHYSBUFF_PC);
  }
  else if (type == Infliction::THRBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(4, stats.getStat(4) * kELMBUFF_PC);
    stats.setStat(5, stats.getStat(5) * kELMBUFF_PC);
  }
  else if (type == Infliction::PRIBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(6, stats.getStat(6) * kELMBUFF_PC);
    stats.setStat(7, stats.getStat(7) * kELMBUFF_PC);
  }
  else if (type == Infliction::POLBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(8, stats.getStat(8) * kELMBUFF_PC);
    stats.setStat(9, stats.getStat(9) * kELMBUFF_PC);
  }
  else if (type == Infliction::CHGBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(10, stats.getStat(10) * kELMBUFF_PC);
    stats.setStat(11, stats.getStat(11) * kELMBUFF_PC);
  }
  else if (type == Infliction::CYBBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(12, stats.getStat(12) * kELMBUFF_PC);
    stats.setStat(13, stats.getStat(13) * kELMBUFF_PC);
  }
  else if (type == Infliction::NIHBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(14, stats.getStat(14) * kELMBUFF_PC);
    stats.setStat(15, stats.getStat(15) * kELMBUFF_PC);
  }
  else if (type == Infliction::UNBBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(18, stats.getStat(18) * kELMBUFF_PC);
  }
  else if (type == Infliction::LIMBUFF)
  {
    setFlag(AilState::BUFF, true);
    stats.setStat(17, stats.getStat(17) * kLIMBUFF_PC);
    stats.setStat(16, stats.getStat(16) * kLIMBUFF_PC);
  }
  else if (type == Infliction::VITBUFF)
  {
    setFlag(AilState::BUFF, true);
    max_stats.setStat(0, stats.getStat(0) * kVITBUFF_PC);
  }
  else if (type == Infliction::QDBUFF)
  {
    setFlag(AilState::BUFF, true);
    max_stats.setStat(1, stats.getStat(1) * kQDBUFF_PC);
  }

  /* Rootbound - Ailed actor (if biological in nature) gains a % HP / turn
   * Constats - kROOTBOUND_PC - % vitality to be gained each turn */
  else if (type == Infliction::ROOTBOUND)
  {
    auto value = stats.getStat(Attribute::VITA) * kROOTBOUND_PC;
    stats.setStat(Attribute::VITA, value);
  }

  /* Double Cast allows the user to use two skils per turn */
  else if (type == Infliction::DOUBLECAST)
    victim->setBFlag(BState::TWO_SKILLS, true);

  /* Triple Cast allows the user to use three skills per turn */
  else if (type == Infliction::TRIPLECAST)
    victim->setBFlag(BState::THREE_SKILLS, true);

  /* Half Cost - On application, the user's useable skill costs are halved */
  else if (type == Infliction::HALFCOST)
    victim->setBFlag(BState::HALF_COST, true);

  /* Hibernation - Gain a % health back per turn in exchange for skipping it,
   *               but the % gain grows
   * Constants - kHIBERNATION_INIT - Initial % (of cur value) hibernation adds
   *             kHIBERNATION_INCR - Increasing % for each turn
   */
  else if (type == Infliction::HIBERNATION)
  {
    uint32_t gain_pc = kHIBERNATION_INIT;
    for (uint32_t i = 0; i < turns_occured; i++)
      gain_pc += kHIBERNATION_INCR;

    stats.setStat(Attribute::VITA, max_stats.getStat(Attribute::VITA) * 
                                    (1 + gain_pc));
  }

  /* Reflect - turn on Person flag to show they reflect skills */
  else if (type == Infliction::REFLECT)
    victim->setBFlag(BState::REFLECT, true);

  /* Metabolic Tether - Metabolic tether has a kMETABOLIC_PC chance for killing
   *   the inflicted, but also a kMETABOLIC_DMG percent that it will deal to
   *   the target.
   */
  else if (type == Infliction::METATETHER)
  {
    //TODO [01-26-14]
    /* Do kMETABOLIC_DMG % upon victim, emit signal if dead */
    //if (victim->damage(max_stats.getStat(EnumDb::VITA) * kMETABOLIC_DMG))
    //    emit victimDeath(victim->getName(), EnumDb::METABOLIC_TETHER);

    /* Check for kMETABOLIC_PC chance for instant death */
    //if (chanceHappens(kMETABOLIC_PC * 100))
    //    emit victimDeath(victim->getName(), EnumDb::METABOLIC_DMG);
  }

  /* Stubulate - //TODO - PC chance or remove a # or remove a fraction of skills
   *                      available to the user each turn
   *
   * Constants:
   */
  else if (type == Infliction::STUBULATE)
  {

  }

  return true;
}

/*
 * Description: Checks the immunity of a potential victim given the type of the
 * ailment.
 *
 * Inputs: none
 * Output: bool - evaluation of the immunity check
 */
bool Ailment::checkImmunity(Person* new_victim)
{
  if (new_victim == nullptr)
    return false;

  /* Flag immunity section */
  if (new_victim->getPFlag(PState::MINI_BOSS))
  {
    if (type == Infliction::DEATHTIMER ||
        type == Infliction::BUBBIFY)
      return false;
  }

  /* Bosses are immune to some ailments */
  else if (new_victim->getPFlag(PState::BOSS))
  {
    if (type == Infliction::DEATHTIMER ||
        type == Infliction::BUBBIFY    ||
        type == Infliction::SILENCE    ||
        type == Infliction::PARALYSIS  ||
        type == Infliction::BLINDNESS)
      return false;
  }

  /* Final boss will be immune to many ailments */
  else if (new_victim->getPFlag(PState::FINAL))
  {
    if (type == Infliction::DEATHTIMER ||
        type == Infliction::BUBBIFY    ||
        type == Infliction::SILENCE    ||
        type == Infliction::PARALYSIS  ||
        type == Infliction::BLINDNESS  ||
        type == Infliction::CONFUSE    ||
        type == Infliction::BERSERK)
      return false;
  }

  /* Bubby effect immunity section */
  if (type == Infliction::ALLATKBUFF ||
      type == Infliction::ALLDEFBUFF ||
      type == Infliction::PHYBUFF    ||
      type == Infliction::THRBUFF    ||
      type == Infliction::POLBUFF    ||
      type == Infliction::PRIBUFF    ||
      type == Infliction::CHGBUFF    ||
      type == Infliction::CYBBUFF    ||
      type == Infliction::NIHBUFF    ||
      type == Infliction::LIMBUFF    ||
      type == Infliction::UNBBUFF    ||
      type == Infliction::VITBUFF    ||
      type == Infliction::QDBUFF)
  return false;

  /* Check for category and racial immunity */
  if (victim->getClass()->isImmune(type))
    return false;

  if (victim->getRace()->isImmune(type))
    return false;

  return true;
}

/*
 * Description: Updates the turn counter on the status ailment based off
 *              the random chance to cure each turn (if not zero). Returns true
 *              if the ailment will be cured after this update, false otherwise.
 *
 * Inputs: none
 * Output: 
 */
// bool Ailment::getRemainingTurns()
// {

// }

/*
 * Description: Updates the turn counter on the status ailment based off
 *              the random chance to cure each turn (if not zero). Returns true
 *              if the ailment will be cured after this update, false otherwise.
 *
 * Inputs: none
 * Output: bool - true if the ailment is to be cured after the update of Fn.
 */
bool Ailment::updateTurns()
{
  turns_occured++;
  
  /* If the ailment doesn't have one turn left, if it's finite, decrement it */
  if (max_turns_left <= kMAX_TURNS && max_turns_left > 1)
    --max_turns_left;
  if (min_turns_left > 0)
    --min_turns_left;

  std::cout << "Update turns: " << max_turns_left << " " << min_turns_left << std::endl;

  if (turns_occured <= min_turns_left)
    return false;

  /* If the ailment is finite, cure it based on chance */;
  if (chance != 0 && Helpers::chanceHappens(std::floor(chance), 100))
    max_turns_left = 1;

  /* If the ailment currently has one turn left, it's cured! */
  if (max_turns_left == 1)
    return true;

  return false;
}

/*
 * Description: Sets the type of Infliction of the ailment
 *
 * Inputs: Infliction - type of Infliction to be set.
 * Output: none
 */
 void Ailment::setType(const Infliction &new_type)
{
  type = new_type;
}

/*
 * Description: Assigns the inflictor of the ailment
 *
 * Inputs: Person* - pointer to the person who inflicted the ailment
 * Output: bool - true if the inflictor is being set for the first time.
 */
bool Ailment::setInflictor(Person* new_inflictor)
{
  if (!getFlag(AilState::INFLICTOR_SET) && new_inflictor != 0)
  {
    inflictor = new_inflictor;
    setFlag(AilState::INFLICTOR_SET);

    return true;
  }

  return false;
}

/*
 * Description: Assigns the victim of the ailment.
 *
 * Inputs: Person* - pointer to the person the ailment is inflicting.
 * Output: bool - true if the victim is being set for the first time.
 */
bool Ailment::setVictim(Person* set_victim)
{
  if (!getFlag(AilState::VICTIM_SET))
  {
    victim = set_victim;
    setFlag(AilState::VICTIM_SET);

    return true;
  }

  return false;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Unapplies the effects of the status ailment to the victim.
 *
 * Inputs: none
 * Output: none
 */
void Ailment::unapply()
{
  //TODO: Retain the defaults or previous settings of things? [01-26-14]
  auto& stats     = victim->getCurr();
  auto& max_stats = victim->getTemp();
  auto  skills    = victim->getCurrSkills();

  /* On removing Berserk, person's dmg_mod needs to be reset. Action flags are
   * recomputed in the reCalcAilmentFlags() function in Battle */
  if (getType() == Infliction::BERSERK)
    victim->setDmgMod(1);

  /* Silence - When silence is removed, skills need to be recalculated */
  else if (getType() == Infliction::SILENCE)
  {
    //TODO: Reasons for skills to still be disabled on unapplication of SILENCE?
    for (uint32_t i = 0; i < skills->getSize(); i++)
      skills->setState(i, true);
  }

  /* Bond - on unapplication, the Person BOND flag is turned off
   */
  else if (getType() == Infliction::BOND)
  {
    victim->setBFlag(BState::BOND, false);
  }

  /* Bonded - on unapplication, the Persons' buffed stats are returned to normal
   */
  else if (getType() == Infliction::BONDED)
  {
    for (uint32_t i = 0; i < stats.getSize(); i++)
      stats.setStat(i, stats.getStat(i) / (1 + kBOND_STATS_PC));
  }

  /* When bubbify is removed, actor needs to return to normal (all other buffs
     are removed as well) */
  else if (getType() == Infliction::BUBBIFY)
  {
    victim->setBFlag(BState::IS_BUBBY, false);
  }

  /* Double Cast - on unapplication turn off the flag for DoubleCast */
  else if (getType() == Infliction::DOUBLECAST)
  {
    victim->setBFlag(BState::TWO_SKILLS, false);
  }

  /* Tripl Cast - on unapplication, turn off the flag for TripleCast */
  else if (getType() == Infliction::TRIPLECAST)
    victim->setBFlag(BState::THREE_SKILLS, false);

  /* Half Cost - on unapplication, turn off the flag for HalfCost */
  else if (getType() == Infliction::HALFCOST)
     victim->setBFlag(BState::HALF_COST, false);

  /* Reflect - on unapplication, turn off the Person flag for reflect */
  else if (getType() == Infliction::REFLECT)
    victim->setBFlag(BState::REFLECT, false);

  /* Stubulate - //TODO: Unknown Effect [08-04-13]
   *
   * Constants:
   */
  else if (type == Infliction::STUBULATE)
  {

  }
}


/*
 * Description:
 *
 * Inputs:
 * Output: 
 */
bool Ailment::toReapplyFlags()
{

  if (type == Infliction::BERSERK)
  {
    return true;
  }

  return false;
}

/*
 * Description: Prints all the info pertaining to Ailment by calling the other
 *              print functions
 *
 * Inputs: none
 * Output: none
 */
void Ailment::print(const bool &simple, const bool &flags)
{
  std::cout << " --- Ailment ---\n";

  if (simple)
  {
    std::cout << "Ch" << chance << " T Lf: " << max_turns_left 
              << " T Oc: " << turns_occured << "\n";
    std::cout << "Vic: " << victim->getName() << "\n";
  }
  else
  {
    std::cout << "Chance: " << chance;
    std::cout << "\nMax # turns left: " << max_turns_left;
    std::cout << "\nTurns Occured: " << turns_occured;
    std::cout << "\nCurrent Victim Name: " << victim->getName() << "\n";
  }

  if (flags)
  {
    std::cout << "INFINITE: " << getFlag(AilState::INFINITE) << "\n";
    std::cout << "CURABLE" << getFlag(AilState::CURABLE) << "\n";
    std::cout << "TO_CURE" << getFlag(AilState::TO_CURE) << "\n";
    std::cout << "TO_UPDATE" << getFlag(AilState::TO_UPDATE) << "\n";
    std::cout << "TO_APPLY" << getFlag(AilState::TO_APPLY) << "\n";
    std::cout << "TO_UNAPPLY" << getFlag(AilState::TO_UNAPPLY) << "\n";
    std::cout << "BUFF" << getFlag(AilState::BUFF) << "\n";
    std::cout << "ADVERSE" << getFlag(AilState::ADVERSE) << "\n";
    std::cout << "IMMUNITY" << getFlag(AilState::IMMUNITY) << "\n";
    std::cout << "CURE_ON_DEATH" << getFlag(AilState::CURE_ON_DEATH) << "\n";
    std::cout << "VICTIM_SET" << getFlag(AilState::VICTIM_SET) << "\n";
    std::cout << "INFLICTOR_SET" << getFlag(AilState::INFLICTOR_SET) << "\n";
  }

  std::cout << " -- /Ailment --- ";
}

/*
 * Description: Returns the amount of damage computed for the ailment update
 *
 * Inputs: none
 * Output: int32_t - the amount of damage
 */
int32_t Ailment::getDamageAmount() const
{
  return damage;
}

/*
 * Description: Returns the type of damage computed for the ailment update
 *
 * Inputs: none
 * Output: DamageType - the type of damage
 */
DamageType Ailment::getDamageType() const
{
  return damage_type;
}

/*
 * Description: Evaluates a given status Ailment flag.
 *
 * Inputs: AilState flags - the flags to be evaluated
 * Output: bool - the boolean evaluation of the given flag
 */
bool Ailment::getFlag(AilState test_flag)
{
  return static_cast<bool>((flag_set & test_flag) == test_flag);
}

/*
 * Description: Returns the number (max number) of turns left on the ailment
 *
 * Inputs: none
 * Output: ushort - maximum number of turns left on the ailment
 */
uint16_t Ailment::getTurnsLeft()
{
  return max_turns_left;
}

/*
 * Description: Returns the Infliction type of the ailment
 *
 * Inputs: none
 * Output: Infliction - type of Ailment
 */
Infliction Ailment::getType()
{
  return type;
}

/*
 * Description: Returns the assigned Victim
 *
 * Inputs: none
 * Output: Person* - pointer to the victim
 */
Person* Ailment::getVictim()
{
  return victim;
}

/*
 * Description: Sets the maximum duration of the ailment as well as the chance
 *
 * Inputs: ushort max - maximum number of turns the ailment will persist
 *         float ch   - chance the ailment has to be cured per turn
 * Output: none
 */
void Ailment::setDuration(const uint16_t &min_turns, const uint16_t &max_turns,
                          const double &chance)
{
  setFlag(AilState::INFINITE, (max_turns > kMAX_TURNS));

  min_turns_left = std::min(min_turns, max_turns);
  max_turns_left = std::max(min_turns, max_turns);

  //TODO: Turns occured?

  this->chance = chance;
}

/*
 * Description: Assigns a PersonState flag or flags to (a) given value(s)
 *
 * Inputs: AilmentFlag flags - enumerated flag to be assigned
 *         set_value - the value to assign the flag(s) to (default: true)
 * Output: none
 */
void Ailment::setFlag(const AilState &flags, const bool &set_value)
{
  (set_value) ? (flag_set |= flags) : (flag_set &= ~flags);
}

/*
 * Description: Assigns a PersonState flag or flags to (a) given value(s)
 *
 * Inputs: Perosn* new_victim - Potential new victim of the status ailment.
 *         Person* new_inflictor -
 *         bool refresh_turns - Whether to reset the turn count
 * Output: Returns true if the new victim was set successfully.
 */
// bool Ailment::setNewVictim(Person* new_victim, Person* new_inflictor,
//                            const bool &refresh_turns)
// {
//   /* Boolean to do checks for assigning new victim */
//   bool can_assign = true;

//   /* Assert the new victim exists */
//   can_assign &= !(new_victim == nullptr);

//   if (can_assign)
//   {
//     /* The ailment cannot be assigned if the new victim is immune */
//     can_assign &= !checkImmunity(new_victim);
//     can_assign &=  getFlag(AilState::VICTIM_SET);
//   }

//   if (can_assign)
//   {
//     /* Reset the turns the ailment has occured */
//     if (refresh_turns)
//     {
//       max_turns_left += turns_occured;
//       turns_occured = 0;
//     }

//     setFlag(AilState::TO_APPLY);
//     setVictim(new_victim);
//     setInflictor(new_inflictor);
//     update();
//   }

//   return can_assign;
// }

/*
 * Description: Update slot. This function will handle calling the
 *              apply function if the status ailment applies an effect (new or
 *              recurring) every turn, and also will handle calling the update
 *              turn function. This function will flip the TOBECURED flag if
 *               the ailment is to be removed immediately.
 *
 * Inputs: none
 * Output: none
 */
void Ailment::update(bool update_turns)
{
  /* The ailment may not be updated */
  if (getFlag(AilState::TO_UPDATE))
  {
    /* Update the turn count and set the TOBECURED flag if neccessary, only
     * update the turns if being called for (default - true) */
    if (!getFlag(AilState::INFINITE) && update_turns && updateTurns())
      setFlag(AilState::TO_CURE);

    /* If the ailment is not to be cured, apply an effect (if there is one) */
    if (!getFlag(AilState::TO_CURE) && getFlag(AilState::TO_APPLY))
      apply();
  }
}

/*
 * Description: Catches reset by Battle, resets the turn counter on the ailment.
 *
 * Inputs: none
 * Output: none
 */
void Ailment::reset()
{
  max_turns_left += turns_occured;
  turns_occured = 0;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs: 
 * Output: 
 */
double Ailment::getBerserkHitbackPC()
{
  return kBERSERK_HITBACK_PC;
}