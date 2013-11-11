/*******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: QWidget
* Description: The ailment class represents a Status Ailment or Debuff. Ailments
*              use the Infliction enumerated type to store their type as well
*              as the EnumString class to conver the enumerations to strings
*              instead of doing string compare (note that these are std::strings
*
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
 * kBUBBIFY_MAX_QD - the maximum QD a Person may have being bubbified
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
 * kQTMNBUFF_PC - factor by which to increase quantum drive during qtmn buff
 *
 *
 * kROOTBOUND_PC - factor by which to heal health during Rootbound
 * kHIBERNATION_INIT - base factor by which to heal health during Hibernating
 * kHIBERNATION_INCR - factor by which to increase healing during Hibernating
 * kMETABOLIC_PC - percent chance to die during metabolic tether
 * kMETABOLIC_DMG - percent hit taken to health if death does not occur
 * kBOND_STATS_PC - factor of others stats' by which inflicted increases stats
 */
const ushort Ailment::kMAX_TURNS           =   25;
const ushort Ailment::kMIN_TURNS           =    1;
const uint   Ailment::kPOISON_DMG_MAX      = 5000;
const uint   Ailment::kPOISON_DMG_MIN      =   50;
const double Ailment::kPOISON_DMG_INCR     = 1.05;
const double Ailment::kPOISON_DMG_INIT     = 1.08;
const uint   Ailment::kBURN_DMG_MAX        = 5000;
const uint   Ailment::kBURN_DMG_MIN        =  100;
const double Ailment::kBURN_DMG_INCR       = 1.02;
const double Ailment::kBURN_DMG_PC         = 1.05;
const double Ailment::kBERSERK_DMG_INCR    = 1.75;
const double Ailment::kBERSERK_HITBACK_PC  = 0.35;
const uint   Ailment::kBUBBIFY_MAX_QD      =   10;
const double Ailment::kBUBBIFY_STAT_MULR   = 0.68;
const double Ailment::kPARALYSIS_PC        = 0.70;
const double Ailment::kBLIND_PC            = 0.50;
const double Ailment::kDREADSTRUCK_PC      = 0.75;
const double Ailment::kDREAMSNARE_PC       = 0.50;
const double Ailment::kALLBUFF_PC          = 1.05;
const double Ailment::kPHYSBUFF_PC         = 1.08;
const double Ailment::kELMBUFF_PC          = 1.07;
const double Ailment::kLIMBUFF_PC          = 1.10;
const double Ailment::kUNBBUFF_PC          = 1.05;
const double Ailment::kMOMBUFF_PC          = 1.01;
const double Ailment::kVITBUFF_PC          = 1.20;
const double Ailment::kQTMNBUFF_PC         = 1.15;
const double Ailment::kROOTBOUND_PC        = 0.15;
const double Ailment::kHIBERNATION_INIT    = 0.15;
const double Ailment::kHIBERNATION_INCR    = 0.05;
const double Ailment::kMETABOLIC_PC        = 0.25;
const double Ailment::kMETABOLIC_DMG       = 0.25;
const double Ailment::kBOND_STATS_PC       = 0.35;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default ailment constructor
 *
 * Inputs: QWidget* - pointer to parent object
 */
Ailment::Ailment()
    : ailment_type(EnumDb::NOAILMENT),
      chance(0.00),
      victim(0)
{
  setFlag(Ailment::TOBEUPDATED, false);
  setDuration(-1,-1);
}

/*
 * Description: Default ailment constructor (constructs a NOAILMENT type)
 *
 * Inputs: Person* victim - pointer to the Victim (owner) of the Ailment
 *         QWidget* parent - parent the Ailment was created from
 */
Ailment::Ailment(Person* victim, Person* inflictor, QWidget* parent)
  : QWidget(parent),
    ailment_type(EnumDb::NOAILMENT),
    chance(0.00),
    inflictor(inflictor),
    victim(victim)

{
  setFlag(Ailment::TOBEUPDATED, false);
  setDuration(-1,-1);

  emit inflicting(victim->getName(), getType());
}

/*
 * Description: Constructs an Ailment object given an Infliction type,
 *              a maximum duration and a double.
 *
 * Inputs: Person* victim - pointer to the Victim (owner) of the Ailment
 *         Infliction type - The Enumerated type of the ailment being created
 *         ushort max - the maximum duration the ailment will take place,
 *                      (the ailment will last forever if max is greater than
 *                       kMAX_TURNS)
 *         double ch  - % value the ailment will be cured per turn, >= 1 = 100%
 *         QWidget* parent - parent the Ailment was created from
 */
Ailment::Ailment(Person* victim, EnumDb::Infliction type, Person* inflictor,
                 short max_turns, double chance, QWidget* parent)
  : QWidget(parent),
    ailment_type(type),
    chance(chance),
    inflictor(inflictor),
    victim(victim)
{
  /* NOAILMENT cannot have a turn length or a chance */
  if (type == EnumDb::NOAILMENT)
  {
    setDuration(-1, -1);
    setFlag(Ailment::TOBEUPDATED, false);
  }
  else if (max_turns > kMAX_TURNS)
    setDuration(kMAX_TURNS, chance);
  else if (max_turns < kMIN_TURNS)
    setDuration(kMIN_TURNS, chance);
  else
    setDuration(max_turns, chance);

  setFlag(Ailment::TOBEUPDATED, true);
  emit inflicting(victim->getName(), getType());
}

/*
 * Description: Constructs an Ailment object given a QString instead of an
 *              Infliction type (grabs the Infliction type with this class'
 *              getInfliction(QString) function.
 *
 * Inputs: Person* victim - pointer to the Victim (owner) of the Ailment
 *         infliction_name - QString representing the enumerated Infliction
 *         ushort max - the maximum duration the ailment will take place,
 *                      (the ailment will last forever if max is greater than
 *                       kMAX_TURNS)
 *         double ch  - % value the ailment will be cured per turn, >= 1 = 100%
 *         QWidget* parent - parent the ailment was created from
 */
Ailment::Ailment(Person* victim, QString name, Person* inflictor, short max_turns,
                 double chance, QWidget* parent)
  : QWidget(parent),
    ailment_type(getInfliction(name)),
    chance(chance),
    inflictor(inflictor),
    victim(victim)
{
  /* NOAILMENT cannot have a turn length or a chance */
  if (getType() == EnumDb::NOAILMENT)
  {
    setDuration(-1, -1);
    setFlag(Ailment::TOBEUPDATED, false);
  }
  else if (max_turns > kMAX_TURNS)
    setDuration(kMAX_TURNS, chance);
  else if (max_turns < kMIN_TURNS)
    setDuration(kMIN_TURNS, chance);
  else
    setDuration(max_turns, chance);

  emit inflicting(victim->getName(), getType());
}

/*
 * Description: Annihilates a Status Ailment object
 */
Ailment::~Ailment()
{
  emit curing(victim->getName(), getType());
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
void Ailment::apply()
{
  /* Helper variables */
  const ushort kHEALTH    = victim->getTemp()->getStat(EnumDb::VITA);
  AttributeSet* stats     = victim->getTemp();
  AttributeSet* max_stats = victim->getMaxTemp();
  SkillSet* skills        = victim->getSkills();
  ushort damage           = 0;

  /* Poison: Ailed actor takes increasing hit to HP every turn
   * Constants: kPOISON_DMG_MAX -- maximum amount poison damage can do
   *            kPOISON_DMG_MIN -- minimum amount poison damage can do
   *            kPOISON_DMG_INCR -- % increase poison damage does
   *            kPOISON_DMG_INIT -- % dmg incurred by poison initially
   */
  if (ailment_type == EnumDb::POISON)
  {
    damage = kPOISON_DMG_MIN;
    if (damage < (kPOISON_DMG_INIT * kHEALTH))
      damage = kPOISON_DMG_INIT * kHEALTH;
    if (turns_occured > 0)
        for (int i = 0; i < turns_occured; i++)
          damage *= kPOISON_DMG_INCR;
    if (damage > kPOISON_DMG_MAX)
      damage = kPOISON_DMG_MAX;

    if (victim->damage(damage))
        emit victimDeath(victim->getName(), EnumDb::POISON_DMG);
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
  else if (ailment_type == EnumDb::BURN || ailment_type == EnumDb::SCALD || 
                                           ailment_type == EnumDb::INFLICTCHAR)
  {
    damage = kBURN_DMG_MIN;
    ushort burn_damage = kHEALTH * kBURN_DMG_PC;

    /* Compute basic damage */
    if (damage < burn_damage)
      damage = burn_damage;

    /* Increase the damage if the burn is level 2 or level 3 */
    if (ailment_type == EnumDb::SCALD)
    {
      ushort scald_damage = kHEALTH * (kBURN_DMG_PC + kBURN_DMG_INCR - 1);
      if (damage < scald_damage)
        damage = scald_damage;
    }
    else if (ailment_type == EnumDb::INFLICTCHAR)
    {
      ushort char_damage = kHEALTH * (kBURN_DMG_PC + (2 * kBURN_DMG_INCR) - 2);
      if (damage < char_damage)
        damage = char_damage;
    }
    if (damage > kBURN_DMG_MAX)
      damage = kBURN_DMG_MAX;

    if (victim->damage(damage))
      emit victimDeath(victim->getName(), EnumDb::BURN_DMG);
  }

  /* Berserk - Ailed actor physically attacks enemy target for extreme damage
   *           while receiving damage themselves. Hitback damage will take
   *           place in Battle.
   * Constants: kBERSERK_DMG_INCR - multiplier for normal damage on target
   *            kBERSERK_HITBACK_PC - multiplier for damage done to self.
   */
  else if (ailment_type == EnumDb::BERSERK)
  {
    /* On initial application, disable non physical skills and running */
    victim->setPersonFlag(Person::SKILL_ENABLED, false);
    victim->setPersonFlag(Person::ITEM_USE_ENABLED, false);
    victim->setPersonFlag(Person::RUN_ENABLED, false);
    victim->setDmgMod(2);
  }

  /* Ailed actor cannot use skills if they require QD */
  else if (ailment_type == EnumDb::SILENCE)
  {
    /* On application, remove skills which have a QD cost > 0 from useable */
    for (int i = 0; i < skills->getSkills().size(); i++)
      if (skills->getSkill(i)->getQdValue() > 0)
        skills->setSkillState(i, false);
  }

  /* Bubbify - ailed actor is turned into a near-useless Bubby */
  else if (ailment_type == EnumDb::BUBBIFY)
  {
    /* Bubbies cannot be affected by buffs */
    emit removeBuffs(victim->getName());

    /* Decrease the stats of the person by a factor of kBUBBIFY_STAT_MULR */
    for (int i = 0; i < stats->getSize(); i++)
        stats->setStat(i, stats->getStat(i) * kBUBBIFY_STAT_MULR);

    /* Disable skills which have a qd cost above kBUBBIFY_MAX_QD */
    for (int i = 0; i < skills->getSkills().size(); i++)
      if (skills->getSkill(i)->getQdValue() > kBUBBIFY_MAX_QD)
        skills->setSkillState(i, false);

    /* Flip the person flag */
    victim->setPersonFlag(Person::IS_BUBBY, true);
  }

  /* Death Timer - Ailed actor KOs upon reaching max_turns */
  else if (ailment_type == EnumDb::DEATHTIMER)
  {
    if (turns_occured >= max_turns_left)
      emit victimDeath(victim->getName(), EnumDb::DEATH_COUNTDOWN);
  }

  /* Paralysis - Ailed actor has a kPARALYSIS_PC chance of skipping their turn,
   *             the effect persisting or a number of turns
   * Constants: kPARALYSIS_PC - Percent chance to skip each turn */
  else if (ailment_type == EnumDb::PARALYSIS)
  {
    if (chanceHappens(kPARALYSIS_PC * 100))
      victim->setPersonFlag(Person::SKIP_NEXT_TURN, true);
  }

  /* Blindness - Ailed actor has a kBLIND_PC chance of missing targets
   * Constants: kBLIND_PC - Percet chance for ailed actor to miss targets
   */
  else if (ailment_type == EnumDb::BLINDNESS)
  {
    if (chanceHappens(kBLIND_PC * 100))
      victim->setPersonFlag(Person::MISS_NEXT_TARGET, false);
  }

  /* Dreadstruck - formerly "Stun": Ailed actor has an extreme chance of
   *               skipping just one turn
   * Constants: kDREADSTRUCK_PC - Percent chance of skipping their next turn.
   */
  else if (ailment_type == EnumDb::DREADSTRUCK)
  {
    if (chanceHappens(kDREADSTRUCK_PC))
      victim->setPersonFlag(Person::SKIP_NEXT_TURN, false);
  }

  /* Dreamsnare - Ailed actor's actions have a kDREAMSNARE_PC chance of being
   *              benign illusions (no effect)
   * Constants: kDREAMSNARE_PC - Percent chance of no effect attacks
   */
  else if (ailment_type == EnumDb::DREAMSNARE)
  {
    if (chanceHappens(kDREAMSNARE_PC * 100))
      victim->setPersonFlag(Person::NO_EFFECT, true);
  }

  /* Bond & Bonded - Two actors are afflicted simultaneously. One person is
   * Bonded to the other. The Bond person is essentially in a dead state, but
   * the other assumes a kBOND_STATS_PC percent of their statistical boost.
   * However, if the Bonded actor dies, the Bond actor wll also die (this
   * case will be handled in Battle)
   * Constants: kBOND_STATS_PC - % by which to increase actor's stats.
   */
  else if (ailment_type == EnumDb::BOND)
  {
    victim->setPersonFlag(Person::BOND, true);
  }
  else if (ailment_type == EnumDb::BONDED)
  {
    for (int i = 0; i < stats->getSize(); i++)
      stats->setStat(i, stats->getStat(i) * (1 + kBOND_STATS_PC));
  }

  /* Buffs -- Increases the user's stats by a specified amount on application
   * Constants: kALLBUFF_PC  - % to incr all values
   *            kPHYSBUFF_PC - % to incr phys values
   *            kELMBUFF_PC  - % to incr elemental values
   */
  if (getFlag(Ailment::TOBEAPPLIED) && getFlag(Ailment::BUFF))
  {

    if (ailment_type == EnumDb::ALLATKBUFF)
    {
      for (int i = 0; i < stats->getSize(); i++)
      {
        QStringList split_stats = stats->getName(i).split("");
        if (split_stats.at(2) == "A" && split_stats.at(3) == "G")
          stats->setStat(i, stats->getStat(i) * kALLBUFF_PC);
      }
      setFlag(Ailment::TOBEAPPLIED, false);
    }
    else if (ailment_type == EnumDb::ALLDEFBUFF)
    {
      for (int i = 0; i < stats->getSize(); i++)
      {
        QStringList split_stats = stats->getName(i).split("");
        if (split_stats.at(2) == "F" && split_stats.at(3) == "D")
          stats->setStat(i, stats->getStat(i) * kALLBUFF_PC);
      }
      setFlag(Ailment::TOBEAPPLIED, false);
    }
    else if (ailment_type == EnumDb::PHYATKBUFF)
      stats->setStat("PHAG", stats->getStat("PHAG") * kPHYSBUFF_PC);
    else if (ailment_type == EnumDb::PHYDEFBUFF)
      stats->setStat("PHFD", stats->getStat("PHFD") * kPHYSBUFF_PC);
    else if (ailment_type == EnumDb::THRATKBUFF)
      stats->setStat("THAG", stats->getStat("THAG") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::THRDEFBUFF)
      stats->setStat("THFD", stats->getStat("THFD") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::POLATKBUFF)
      stats->setStat("POAG", stats->getStat("POAG") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::POLDEFBUFF)
      stats->setStat("POFD", stats->getStat("POFD") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::PRIATKBUFF)
      stats->setStat("PRAG", stats->getStat("PRAG") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::PRIDEFBUFF)
      stats->setStat("PRFD", stats->getStat("PRFD") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::CHGATKBUFF)
      stats->setStat("CHAG", stats->getStat("CHAG") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::CHGDEFBUFF)
     stats->setStat("CHFD", stats->getStat("CHFD") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::CYBATKBUFF)
      stats->setStat("CYAG", stats->getStat("CYAG") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::CYBDEFBUFF)
      stats->setStat("CYFD", stats->getStat("CYFD") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::NIHATKBUFF)
      stats->setStat("NIAG", stats->getStat("NIAG") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::NIHDEFBUFF)
      stats->setStat("NIAG", stats->getStat("NIFD") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::UNBBUFF)
      stats->setStat("UNBR", stats->getStat("UNBR") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::LIMBUFF)
      stats->setStat("NIFD", stats->getStat("LIMB") * kLIMBUFF_PC);
    else if (ailment_type == EnumDb::MOMBUFF)
      stats->setStat("MMTM", stats->getStat("MMTM") * kELMBUFF_PC);
    else if (ailment_type == EnumDb::VITBUFF)
      stats->setStat("VITA", stats->getStat("VITA") * kVITBUFF_PC);
    else if (ailment_type == EnumDb::QDBUFF)
      stats->setStat("QTMN", stats->getStat("QTMN") * kQTMNBUFF_PC);
  }

  /* Rootbound - Ailed actor (if biological in nature) gains a % HP / turn
   * Constats - kROOTBOUND_PC - % vitality to be gained each turn */
  else if (ailment_type == EnumDb::ROOTBOUND)
  {
    ushort value = stats->getStat("VITA") * kROOTBOUND_PC;
    stats->setStat("VITA", value);
  }

  /* Double Cast allows the user to use two skils per turn */
  else if (ailment_type == EnumDb::DOUBLECAST)
    victim->setPersonFlag(Person::TWO_SKILLS, true);

  /* Triple Cast allows the user to use three skills per turn */
  else if (ailment_type == EnumDb::TRIPLECAST)
    victim->setPersonFlag(Person::THREE_SKILLS, true);

  /* Half Cost - On application, the user's useable skill costs are halved */
  else if (ailment_type == EnumDb::HALFCOST)
    victim->setPersonFlag(Person::HALF_COST, true);

  /* Hibernation - Gain a % health back per turn in exchange for skipping it,
   *               but the % gain grows
   * Constants - kHIBERNATION_INIT - Initial % (of cur value) hibernation adds
   *             kHIBERNATION_INCR - Increasing % for each turn
   */
  else if (ailment_type == EnumDb::HIBERNATION)
  {
    ushort gain_pc = kHIBERNATION_INIT;
    for (int i = 0; i < turns_occured; i++)
      gain_pc += kHIBERNATION_INCR;
    stats->setStat("VITA", max_stats->getStat("VITA") * (1 + gain_pc));
  }

  /* Reflect - turn on Person flag to show they reflect skills */
  else if (ailment_type == EnumDb::REFLECT)
    victim->setPersonFlag(Person::REFLECT, true);

  /* Metabolic Tether - Metabolic tether has a kMETABOLIC_PC chance for killing
   *   the inflicted, but also a kMETABOLIC_DMG percent that it will deal to
   *   the target.
   */
  else if (ailment_type == EnumDb::METATETHER)
  {
    /* Do kMETABOLIC_DMG % upon victim, emit signal if dead */
    if (victim->damage(max_stats->getStat(EnumDb::VITA) * kMETABOLIC_DMG))
        emit victimDeath(victim->getName(), EnumDb::METABOLIC_TETHER);

    /* Check for kMETABOLIC_PC chance for instant death */
    if (chanceHappens(kMETABOLIC_PC * 100))
        emit victimDeath(victim->getName(), EnumDb::METABOLIC_DMG);
  }

  /* Stubulate - //TODO: Unknown Effect [08-04-13]
   *
   * Constants:
   */
  else if (ailment_type == EnumDb::STUBULATE)
  {

  }

  /* Ailment update complete, emit signal */
  emit updated();
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
  if (new_victim == 0)
    return false;

  /* Helper variables */
  QString race_name = new_victim->getRace()->getName();
  QString cat_name = new_victim->getCategory()->getName();

  /* Flag immunity section */
  if (new_victim->getPersonFlag(Person::MINI_BOSS))
  {
    if (ailment_type == EnumDb::DEATHTIMER ||
        ailment_type == EnumDb::BUBBIFY)
      return false;
  }

  /* Bosses are immune to some ailments */
  else if (new_victim->getPersonFlag(Person::BOSS))
  {
    if (ailment_type == EnumDb::DEATHTIMER ||
        ailment_type == EnumDb::BUBBIFY    ||
        ailment_type == EnumDb::SILENCE    ||
        ailment_type == EnumDb::PARALYSIS  ||
        ailment_type == EnumDb::BLINDNESS)
      return false;
  }

  /* Final boss will be immune to many ailments */
  else if (new_victim->getPersonFlag(Person::FINAL_BOSS))
  {
    if (ailment_type == EnumDb::DEATHTIMER ||
        ailment_type == EnumDb::BUBBIFY    ||
        ailment_type == EnumDb::SILENCE    ||
        ailment_type == EnumDb::PARALYSIS  ||
        ailment_type == EnumDb::BLINDNESS  ||
        ailment_type == EnumDb::CONFUSE    ||
        ailment_type == EnumDb::BERSERK)
      return false;
  }

  /* Bubby effect immunity section */
  if (ailment_type == EnumDb::ALLATKBUFF ||
      ailment_type == EnumDb::ALLDEFBUFF ||
      ailment_type == EnumDb::PHYATKBUFF ||
      ailment_type == EnumDb::PHYDEFBUFF ||
      ailment_type == EnumDb::THRATKBUFF ||
      ailment_type == EnumDb::THRDEFBUFF ||
      ailment_type == EnumDb::POLATKBUFF ||
      ailment_type == EnumDb::POLDEFBUFF ||
      ailment_type == EnumDb::PRIATKBUFF ||
      ailment_type == EnumDb::PRIDEFBUFF ||
      ailment_type == EnumDb::CHGATKBUFF ||
      ailment_type == EnumDb::CHGDEFBUFF ||
      ailment_type == EnumDb::CYBATKBUFF ||
      ailment_type == EnumDb::CYBDEFBUFF ||
      ailment_type == EnumDb::NIHATKBUFF ||
      ailment_type == EnumDb::NIHDEFBUFF ||
      ailment_type == EnumDb::LIMBUFF    ||
      ailment_type == EnumDb::UNBBUFF    ||
      ailment_type == EnumDb::MOMBUFF    ||
      ailment_type == EnumDb::VITBUFF    ||
      ailment_type == EnumDb::QDBUFF)
  return false;

  /* Check for category and racial immunity */
  if (victim->getCategory()->isImmune(ailment_type))
    return false;

  if (victim->getRace()->isImmune(ailment_type))
    return false;

  return true;
}

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
  /* If the ailment is finite, cure it based on chance */;
  if (max_turns_left <= kMAX_TURNS && chance != 0)
    if (chanceHappens(floor(chance * 100)))
      max_turns_left = 1;

  /* If the ailment currently has one turn left, it's cured! */
  if (max_turns_left == 1)
    return true;
  /* If the ailment doesn't have one turn left, if it's finite, decrement it */
  if (max_turns_left <= kMAX_TURNS)
    max_turns_left--;

  return false;
}

/*
 * Description: Sets the type of Infliction of the ailment
 *
 * Inputs: Infliction - type of Infliction to be set.
 * Output: none
 */
 void Ailment::setType(EnumDb::Infliction t)
{
  ailment_type = t;
}

/*
 * Description: Assigns the inflictor of the ailment
 *
 * Inputs: Person* - pointer to the person who inflicted the ailment
 * Output: bool - true if the inflictor is being set for the first time.
 */
bool Ailment::setInflictor(Person* new_inflictor)
{
  if (!getFlag(Ailment::INFLICTOR_SET) && new_inflictor != 0)
  {
    inflictor = new_inflictor;
    setFlag(Ailment::INFLICTOR_SET);
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
  if (!getFlag(Ailment::VICTIM_SET))
  {
    victim = set_victim;
    setFlag(Ailment::VICTIM_SET);
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
  AttributeSet* stats = victim->getTemp();
  SkillSet* skills = victim->getSkills();

  /* On removing Berserk, the person's abilities need to be re-enabled */
  if (getType() == EnumDb::BERSERK)
  {
    victim->setPersonFlag(Person::RUN_ENABLED, true);
    victim->setPersonFlag(Person::SKILL_ENABLED, true);
    victim->setPersonFlag(Person::ITEM_USE_ENABLED, true);
    victim->setDmgMod(1);
  }

  /* Silence - When silence is removed, skills need to be recalculated */
  else if (getType() == EnumDb::SILENCE)
  {

    for (int i = 0; i < skills->getSkills().size(); i++)
      skills->setSkillState(i, true);
  }

  /* Bond - on unapplication, the Person BOND flag is turned off
   */
  else if (getType() == EnumDb::BOND)
  {
    victim->setPersonFlag(Person::BOND, false);
  }

  /* Bonded - on unapplication, the Persons' buffed stats are returned to normal
   */
  else if (getType() == EnumDb::BONDED)
  {
    for (int i = 0; i < stats->getSize(); i++)
      stats->setStat(i, stats->getStat(i) / (1 + kBOND_STATS_PC));
  }

  /* When bubbify is removed, actor needs to return to normal (all other buffs
     are removed as well) */
  else if (getType() == EnumDb::BUBBIFY)
  {
    emit removeBuffs(victim->getName());
  }

  /* Remove the buffing effects from all attack stats */
  if (getType() == EnumDb::ALLATKBUFF)
  {
    for (int i = 0; i < stats->getSize(); i++)
    {
      QStringList split_stats = stats->getName(i).split("");
      if (split_stats.at(2) == "A" && split_stats.at(3) == "G")
        stats->setStat(i, stats->getStat(i) / kALLBUFF_PC);
    }
  }

  /* Remove the buffing effect from all defense stats */
  else if (getType() == EnumDb::ALLDEFBUFF)
  {
    for (int i = 0; i < stats->getSize(); i++)
    {
      QStringList split_stats = stats->getName(i).split("");
      if (split_stats.at(2) == "F" && split_stats.at(3) == "D")
        stats->setStat(i, stats->getStat(i) / kALLBUFF_PC);
    }
  }

  /* Remove the buffing effect from each stat */
  else if (ailment_type == EnumDb::PHYATKBUFF)
    stats->setStat("PHAG", stats->getStat("PHAG") / kPHYSBUFF_PC);
  else if (ailment_type == EnumDb::PHYDEFBUFF)
    stats->setStat("PHFD", stats->getStat("PHFD") / kPHYSBUFF_PC);
  else if (ailment_type == EnumDb::THRATKBUFF)
    stats->setStat("THAG", stats->getStat("THAG") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::THRDEFBUFF)
    stats->setStat("THFD", stats->getStat("THFD") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::POLATKBUFF)
    stats->setStat("POAG", stats->getStat("POAG") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::POLDEFBUFF)
    stats->setStat("POFD", stats->getStat("POFD") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::PRIATKBUFF)
    stats->setStat("PRAG", stats->getStat("PRAG") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::PRIDEFBUFF)
    stats->setStat("PRFD", stats->getStat("PRFD") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::CHGATKBUFF)
    stats->setStat("CHAG", stats->getStat("CHAG") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::CHGDEFBUFF)
   stats->setStat("CHFD", stats->getStat("CHFD") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::CYBATKBUFF)
    stats->setStat("CYAG", stats->getStat("CYAG") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::CYBDEFBUFF)
    stats->setStat("CYFD", stats->getStat("CYFD") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::NIHATKBUFF)
    stats->setStat("NIAG", stats->getStat("NIAG") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::NIHDEFBUFF)
    stats->setStat("NIAG", stats->getStat("NIFD") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::UNBBUFF)
    stats->setStat("UNBR", stats->getStat("UNBR") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::LIMBUFF)
    stats->setStat("NIFD", stats->getStat("LIMB") / kLIMBUFF_PC);
  else if (ailment_type == EnumDb::MOMBUFF)
    stats->setStat("MMTM", stats->getStat("MMTM") / kELMBUFF_PC);
  else if (ailment_type == EnumDb::VITBUFF)
    stats->setStat("VITA", stats->getStat("VITA") / kVITBUFF_PC);
  else if (ailment_type == EnumDb::QDBUFF)
    stats->setStat("QTMN", stats->getStat("QTMN") / kQTMNBUFF_PC);

  /* Double Cast - on unapplication turn off the flag for DoubleCast */
  else if (getType() == EnumDb::DOUBLECAST)
    victim->setPersonFlag(Person::TWO_SKILLS, false);

  /* Tripl Cast - on unapplication, turn off the flag for TripleCast */
  else if (getType() == EnumDb::TRIPLECAST)
    victim->setPersonFlag(Person::THREE_SKILLS, false);

  /* Half Cost - on unapplication, turn off the flag for HalfCost */
  else if (getType() == EnumDb::HALFCOST)
     victim->setPersonFlag(Person::HALF_COST, false);

  /* Reflect - on unapplication, turn off the Person flag for reflect */
  else if (getType() == EnumDb::REFLECT)
    victim->setPersonFlag(Person::REFLECT, false);

  /* Stubulate - //TODO: Unknown Effect [08-04-13]
   *
   * Constants:
   */
  else if (ailment_type == EnumDb::STUBULATE)
  {

  }

}

/*
 * Description: Prints all the info pertaining to Ailment by calling the other
 *              print functions
 *
 * Inputs: none
 * Output: none
 */
void Ailment::printAll()
{
  qDebug() << " --- Ailment --- ";
  printFlags();
  printInfo();
  qDebug() << " -- /Ailment --- ";
}

/*
 * Description: Prints out the states of the flags of ailment
 *
 * Inputs: none
 * Output: none
 */
void Ailment::printFlags()
{
  //qDebug() << "AILMENT TYPE: " << getAilmentStr(ailment_type);
  qDebug() << "INFINITE: " << getFlag(Ailment::INFINITETIME);
  qDebug() << "CURABLE:  " << getFlag(Ailment::CURABLE);
  qDebug() << "TOBECURED  " << getFlag(Ailment::TOBECURED);
  qDebug() << "TOBEUPDATED:  " << getFlag(Ailment::TOBEUPDATED);
  qDebug() << "TOBEAPPLIED:  " << getFlag(Ailment::TOBEAPPLIED);
  qDebug() << "BUFF:  " << getFlag(Ailment::BUFF);
  qDebug() << "ADVERSE:  " << getFlag(Ailment::ADVERSE);
  qDebug() << "NEUTRAL:  " << getFlag(Ailment::NEUTRAL);
  qDebug() << "IMMUNITY:  " << getFlag(Ailment::IMMUNITY);
  qDebug() << "CUREONDEATH:  " << getFlag(Ailment::CUREONDEATH);
}

/*
 * Description: Prints out the info pertaining to the ailment
 *
 * Inputs: none
 * Output: none
 */
void Ailment::printInfo()
{
  qDebug() << "Chance: " << chance;
  qDebug() << "Max # turns left: " << max_turns_left;
  qDebug() << "Turns Occured: " << turns_occured;
  qDebug() << "Current Victim Name: " << victim->getName();
}

/*
 * Description: Evaluates a given status Ailment flag.
 *
 * Inputs: AilmentFlag flags - the flags to be evaluated
 * Output: bool - the boolean evaluation of the given flag
 */
bool Ailment::getFlag(AilmentFlag flags)
{
  return flag_set.testFlag(flags);
}

/*
 * Description: Returns the number (max number) of turns left on the ailment
 *
 * Inputs: none
 * Output: ushort - maximum number of turns left on the ailment
 */
ushort Ailment::getTurnsLeft()
{
  return max_turns_left;
}

/*
 * Description: Returns the Infliction type of the ailment
 *
 * Inputs: none
 * Output: Infliction - type of Ailment
 */
EnumDb::Infliction Ailment::getType()
{
  return ailment_type;
}


Person* Ailment::getVictim()
{
  return victim;
}

/*
 * Description: Returns the string value of the current object's enumerated
 *              value.
 *
 * Inputs: none
 * Output: QString - string of the infliction
 */
QString Ailment::getName()
{
  const std::string &string =
          EnumString<EnumDb::Infliction>::From(ailment_type);
  QString ailment_qstring(string.c_str());
  return ailment_qstring;
}

/*
 * Description: Sets the maximum duration of the ailment as well as the chance
 *
 * Inputs: ushort max - maximum number of turns the ailment will persist
 *         float ch   - chance the ailment has to be cured per turn
 * Output: none
 */
void Ailment::setDuration(short max_turns, double chance)
{
  if (max_turns > kMAX_TURNS)
    setFlag(Ailment::INFINITETIME);
  else
    setFlag(Ailment::INFINITETIME, false);

  max_turns_left = max_turns;
  this->chance = chance;
}

/*
 * Description: Assigns a PersonState flag or flags to (a) given value(s)
 *
 * Inputs: AilmentFlag flags - enumerated flag to be assigned
 *         set_value - the value to assign the flag(s) to (default: true)
 * Output: none
 */
void Ailment::setFlag(AilmentFlag flags, bool set_value)
{
  (set_value) ? (flag_set |= flags) : (flag_set &= flags);
}

/*
 * Description: Assigns a PersonState flag or flags to (a) given value(s)
 *
 * Inputs: Perosn* new_victim - Potential new victim of the status ailment.
 *         Person* new_inflictor -
 *         bool refresh_turns - Whether to reset the turn count
 * Output: Returns true if the new victim was set successfully.
 */
bool Ailment::setNewVictim(Person* new_victim, Person* new_inflictor,
                           bool refresh_turns)
{
  /* Boolean to do checks for assigning new victim */
  bool can_assign = true;

  /* Assert the new victim exists */
  if (new_victim == 0)
    can_assign = false;

  if (can_assign)
  {
    /* The ailment cannot be assigned if the new victim is immune */
    can_assign &= !checkImmunity(new_victim);
    can_assign &= getFlag(Ailment::VICTIM_SET);
  }

  if (can_assign)
  {
    /* Reset the turns the ailment has occured */
    if (refresh_turns)
    {
      max_turns_left += turns_occured;
      turns_occured = 0;
    }

    setFlag(Ailment::TOBEAPPLIED);
    setVictim(new_victim);
    setInflictor(new_inflictor);
    update();
  }

  return can_assign;
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Slot to catch an ailment which caused death, handles flag work
 *              for ailment destruction.
 *
 * Inputs: none
 * Output: none
 */
void Ailment::death()
{
  setFlag(Ailment::TOBEAPPLIED, false);
  setFlag(Ailment::TOBEUPDATED, false);
  setFlag(Ailment::TOBECURED, true);
}

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
void Ailment::update()
{
  /* The ailment may not be updated */
  if (getFlag(Ailment::TOBEUPDATED))
  {
    /* Update the turn count and set the TOBECURED flag if neccessary */
    bool cure_value = false;
    if (!getFlag(Ailment::INFINITETIME))
      cure_value = updateTurns();
    setFlag(Ailment::TOBECURED, cure_value);

    /* If the ailment is not to be cured, apply an effect (if there is one) */
    if (!getFlag(Ailment::TOBECURED))
    {
      if (getFlag(Ailment::TOBEAPPLIED))
        apply();
    }
  }
  if (getFlag(Ailment::TOBECURED))
      unapply();
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
  emit reset();
}

/*===========================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the string value of a given infliction. Uses the
 *              EnumString class which generates std::strings, the actual
 *              registered strings are in EnumDatabase
 *
 * Inputs: Infliction - type of Infliction to be evaluated.
 * Output: QString - A QString of the given Infliction type.
 */
QString Ailment::getAilmentStr(EnumDb::Infliction type)
{
  const std::string &ailment_string = EnumString<EnumDb::Infliction>::From(type);
  QString ailment_qstring(ailment_string.c_str());
  return ailment_qstring;
}

/*
 * Description: Returns the Infliction of a given string (if one exists)
 *
 * Inputs: QString - name of Infliction to be checked for
 * Output: Infliction - the corresponding Infliction (NOAILMENT for default)
 */
EnumDb::Infliction Ailment::getInfliction(QString name)
{
  const std::string &ailment_string = name.toUtf8().constData();
  EnumDb::Infliction ailment_type = EnumDb::NOAILMENT;
  EnumString<EnumDb::Infliction>::To(ailment_type, ailment_string);
  return ailment_type;
}
