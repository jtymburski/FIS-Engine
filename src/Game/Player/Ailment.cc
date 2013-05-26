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
*
* // TODO: Finish Curse apply effect [03-30-13]
*******************************************************************************/
#include "Game/Player/Ailment.h"

/*============================================================================
 * CONSTANTS (Explanation for each in header file)
 *============================================================================*/
const ushort Ailment::kMAX_TURNS           =   25;
const ushort Ailment::kMIN_TURNS           =    1;
const ushort Ailment::kPOISON_DMG_MAX      = 5000;
const ushort Ailment::kPOISON_DMG_MIN      =   50;
const double Ailment::kPOISON_DMG_INCR     = 1.05;
const double Ailment::kPOISON_DMG_INIT     = 1.08;
const ushort Ailment::kBURN_DMG_MAX        = 5000;
const ushort Ailment::kBURN_DMG_MIN        =  100;
const double Ailment::kBURN_DMG_INCR       = 1.02;
const double Ailment::kBURN_DMG_PC         = 1.05;
const double Ailment::kBERSERK_DMG_INCR    = 1.75;
const double Ailment::kBERSERK_HITBACK_PC  = 0.35;
const ushort Ailment::kBUBBIFY_MAX_QD      =   10;
const double Ailment::kBUBBIFY_STAT_MULR   = 0.68;
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

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

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
Ailment::Ailment(Person* victim, Infliction type, short max_turns,
                 double chance, QWidget* parent) : QWidget(parent)
{
  setVictim(victim);
  setType(type);

  /* NOAILMENT cannot have a turn length or a chance */
  if (type == NOAILMENT)
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
Ailment::Ailment(Person* victim, QString name, short max_turns,
                 double chance, QWidget* parent) : QWidget(parent)
{
  setVictim(victim);
  /* Grabs the Enumeration of the given QString, then sets the type */
  setType(getInfliction(name));

  /* NOAILMENT cannot have a turn length or a chance */
  if (getType() == NOAILMENT)
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

  emit inflicting(victim->getName(), this->getType());
}

/*
 * Description: Default ailment constructor (constructs a NOAILMENT type)
 *
 * Inputs: Person* victim - pointer to the Victim (owner) of the Ailment
 *         QWidget* parent - parent the Ailment was created from
 */
Ailment::Ailment(Person* victim, QWidget* parent) : QWidget(parent)
{
  setVictim(victim);
  setType(NOAILMENT);
  setFlag(Ailment::TOBEUPDATED, false);
  setDuration(-1,-1);

  emit inflicting(victim->getName(), this->getType());
}

/*
 * Description: Annihilates a Status Ailment object
 */
Ailment::~Ailment()
{
  emit curing(victim->getName(), this->getType());
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns a random integer up to max
 *
 * Inputs: int - maximum integer value needed in randomization
 * Output: int - randomized integer
 */
int Ailment::getRandomNumber(int max)
{
  QTime midnight(0, 0, 0);
  qsrand(midnight.secsTo(QTime::currentTime()));
  return qrand() % max;
}

/*
 * Description: Applies the effect of the status ailment to the Person (victim)
 *              if one so exists, checking for recurring effects, etc.
 *
 * Inputs: none
 * Output: bool - true if the ailment is to be cured
 */
void Ailment::apply()
{
  /* Helper variables */
  const ushort kHEALTH = victim->tempStats()->getStat(0);
  AttributeSet* stats = victim->tempStats();
  SkillSet* skills = victim->getSkills();
  ushort damage = 0;

  /* Poison: Ailed actor takes increasing hit to HP every turn
   * Constants: kPOISON_DMG_MAX -- maximum amount poison damage can do
   *            kPOISON_DMG_MIN -- minimum amount poison damage can do
   *            kPOISON_DMG_INCR -- % increase poison damage does
   *            kPOISON_DMG_INIT -- % dmg incurred by poison initially
   */
  if (ailment_type == POISON)
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
        emit victimDeath(victim->getName(), POISONDMG);
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
  else if (ailment_type == BURN || ailment_type == SCALD || ailment_type == CHAR)
  {
    damage = kBURN_DMG_MIN;
    ushort burn_damage = kHEALTH * kBURN_DMG_PC;

    /* Compute basic damage */
    if (damage < burn_damage)
      damage = burn_damage;

    /* Increase the damage if the burn is level 2 or level 3 */
    if (ailment_type == SCALD)
    {
      ushort scald_damage = kHEALTH * (kBURN_DMG_PC + kBURN_DMG_INCR - 1);
      if (damage < scald_damage)
        damage = scald_damage;
    }
    else if (ailment_type == CHAR)
    {
      ushort char_damage = kHEALTH * (kBURN_DMG_PC + (2 * kBURN_DMG_INCR) - 2);
      if (damage < char_damage)
        damage = char_damage;
    }
    if (damage > kBURN_DMG_MAX)
      damage = kBURN_DMG_MAX;

    if (victim->damage(damage))
      emit victimDeath(victim->getName(), BURNDMG);
  }

  /* Berserk - Ailed actor physically attacks enemy target for extreme damage
   *           while receiving damage themselves.
   * Constants: kBERSERK_DMG_INCR - multiplier for normal damage on target
   *            kBERSERK_HITBACK_PC - multiplier for damage done to self.
   */
  else if (ailment_type == BERSERK)
  {
    /* On initial application, disable non physical skills and running */
    victim->setPersonFlag(Person::CANUSESKILLS, false);
    victim->setPersonFlag(Person::CANUSEITEM, false);
    victim->setPersonFlag(Person::CANRUN, false);
    victim->setDmgMod(2);

    /* Hitback damage during Berserk will take place in Battle */
  }

  /* Confuse - ailed actor attacks a random target with a random skills */
  else if (ailment_type == CONFUSE)
  {
    /* Confuse effect takes place in battle--calculates useable skills,
       useable targets and performs random permutation of each. */
  }

  /* Ailed actor cannot use skills if they require QD */
  else if (ailment_type == SILENCE)
  {
    /* On application, remove skills which have a QD cost > 0 from useable */
    for (int i = 0; i < skills->getSkills().size(); i++)
      if (skills->getSkill(i)->getQdValue() > 0)
        skills->setSkillState(i, false);
  }

  /* Bubbify - ailed actor is turned into a near-useless Bubby
   */
  else if (ailment_type == BUBBIFY)
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
    victim->setPersonFlag(Person::ISBUBBY, true);
  }

  /* Death Timer - Ailed actor KOs upon reaching max_turns */
  else if (ailment_type == DEATHTIMER)
  {
    if (turns_occured >= max_turns_left)
      emit victimDeath(victim->getName(), DEATHCOUNTDOWN);
  }

  /* Ailed actor has a 70% chance of skipping their turn */
  else if (ailment_type == PARALYSIS)
  {
    int random_percent = getRandomNumber(100);
    if (random_percent > 30)
        victim->setPersonFlag(Person::SKIPNEXTTURN, true);
  }

  /* Blindness - Ailed actor has a much higher chance of missing targets */
  else if (ailment_type == BLINDNESS)
  {
    int random_percent = getRandomNumber(100);
    if (random_percent <= kBLIND_PC * 100)
        victim->setPersonFlag(Person::MISSNEXTTARGET, false);
  }

  /* Dreadstruck - formerly "Stun": Ailed actor has an extreme chance of
   *               skipping just one turn
   */
  else if (ailment_type == DREADSTRUCK)
  {
    int random_percent = getRandomNumber(100);
    if (random_percent <= kDREADSTRUCK_PC * 100)
        victim->setPersonFlag(Person::MISSNEXTTARGET, false);
  }

  /* Dreamsnare - Ailed actor's actions have a 50% chance of being benign
   *              illusions (no effect
   */
  else if (ailment_type == DREAMSNARE)
  {
    int random_percent = getRandomNumber(100);
    if (random_percent <= kDREAMSNARE_PC * 100)
      victim->setPersonFlag(Person::NOEFFECT, true);
  }

  /* Hellbound - If this actor dies, another living actor on the same team
   *             (if one exists) dies with them.
   *           - Hellbound effect handled in Battle
   */

  /* Bond - Two actors are afflicted simultaneously. Affected actor's stats are
  *  combined, damage dealt to one is also dealt to the other, as with status
  *  ailments and other effects (ex. death)
  */
  else if (ailment_type == BOND)
  {
    //TODO: Bond combination experimentation [05-05-13]
  }

  /* Buffs -- Increases the user's stats by a specified amount on application
   * Constants: kALLBUFF_PC  - % to incr all values
   *            kPHYSBUFF_PC - % to incr phys values
   *            kELMBUFF_PC  - % to incr elemental values
   */
  if (getFlag(Ailment::TOBEAPPLIED) && getFlag(Ailment::BUFF))
  {

    if (ailment_type == ALLATKBUFF)
    {
      for (int i = 0; i < stats->getSize(); i++)
      {
        QStringList split_stats = stats->getName(i).split("");
        if (split_stats.at(2) == "A" && split_stats.at(3) == "G")
          stats->setStat(i, stats->getStat(i) * kALLBUFF_PC);
      }
      setFlag(Ailment::TOBEAPPLIED, false);
    }
    else if (ailment_type == ALLDEFBUFF)
    {
      for (int i = 0; i < stats->getSize(); i++)
      {
        QStringList split_stats = stats->getName(i).split("");
        if (split_stats.at(2) == "F" && split_stats.at(3) == "D")
          stats->setStat(i, stats->getStat(i) * kALLBUFF_PC);
      }
      setFlag(Ailment::TOBEAPPLIED, false);
    }
    else if (ailment_type == PHYATKBUFF)
      stats->setStat("PHAG", stats->getStat("PHAG") * kPHYSBUFF_PC);
    else if (ailment_type == PHYDEFBUFF)
      stats->setStat("PHFD", stats->getStat("PHFD") * kPHYSBUFF_PC);
    else if (ailment_type == THRATKBUFF)
      stats->setStat("THAG", stats->getStat("THAG") * kELMBUFF_PC);
    else if (ailment_type == THRDEFBUFF)
      stats->setStat("THFD", stats->getStat("THFD") * kELMBUFF_PC);
    else if (ailment_type == POLATKBUFF)
      stats->setStat("POAG", stats->getStat("POAG") * kELMBUFF_PC);
    else if (ailment_type == POLDEFBUFF)
      stats->setStat("POFD", stats->getStat("POFD") * kELMBUFF_PC);
    else if (ailment_type == PRIATKBUFF)
      stats->setStat("PRAG", stats->getStat("PRAG") * kELMBUFF_PC);
    else if (ailment_type == PRIDEFBUFF)
      stats->setStat("PRFD", stats->getStat("PRFD") * kELMBUFF_PC);
    else if (ailment_type == CHGATKBUFF)
      stats->setStat("CHAG", stats->getStat("CHAG") * kELMBUFF_PC);
    else if (ailment_type == CHGDEFBUFF)
     stats->setStat("CHFD", stats->getStat("CHFD") * kELMBUFF_PC);
    else if (ailment_type == CYBATKBUFF)
      stats->setStat("CYAG", stats->getStat("CYAG") * kELMBUFF_PC);
    else if (ailment_type == CYBDEFBUFF)
      stats->setStat("CYFD", stats->getStat("CYFD") * kELMBUFF_PC);
    else if (ailment_type == NIHATKBUFF)
      stats->setStat("NIAG", stats->getStat("NIAG") * kELMBUFF_PC);
    else if (ailment_type == NIHDEFBUFF)
      stats->setStat("NIAG", stats->getStat("NIFD") * kELMBUFF_PC);
    else if (ailment_type == UNBBUFF)
      stats->setStat("UNBR", stats->getStat("UNBR") * kELMBUFF_PC);
    else if (ailment_type == LIMBUFF)
      stats->setStat("NIFD", stats->getStat("LIMB") * kLIMBUFF_PC);
    else if (ailment_type == MOMBUFF)
      stats->setStat("MMTM", stats->getStat("MMTM") * kELMBUFF_PC);
    else if (ailment_type == VITBUFF)
      stats->setStat("VITA", stats->getStat("VITA") * kVITBUFF_PC);
    else if (ailment_type == QDBUFF)
      stats->setStat("QTMN", stats->getStat("QTMN") * kQTMNBUFF_PC);
  }

  /* Rootbound - Ailed actor (if biological in nature) gains a % HP / turn
   * Constats - kROOTBOUND_PC - % vitality to be gained each turn */
  else if (ailment_type == ROOTBOUND)
  {
    ushort value = stats->getStat("VITA") * kROOTBOUND_PC;
    stats->setStat("VITA", value);
  }

  /* Double cast allows the user to use two skils per turn */
  else if (ailment_type == DOUBLECAST)
    victim->setPersonFlag(Person::TWOSKILLS, true);

  /* Triple cast allows the user to use three skills per turn */
  else if (ailment_type == TRIPLECAST)
    victim->setPersonFlag(Person::THREESKILLS, true);

  /* Triple cast allows the user to use three skills per turn */

  /* Half Cost - On application, the user's useable skill costs are halved
     -- Handle half cost in Battle
     -- Find users currently useable skills
     -- Half their cost (temporarily)
  */

  /* Hibernation - Gain a % health back per turn in exchange for skipping it,
   *               but the % gain grows
   * Constants - kHIBERNATION_INIT - Initial % (of cur value) hibernation adds
   *             kHIBERNATION_INCR - Increasing % for each turn
   */
  else if (ailment_type == HIBERNATION)
  {
    ushort gain_pc = kHIBERNATION_INIT;
    for (int i = 0; i < turns_occured; i++)
      gain_pc += kHIBERNATION_INCR;
    stats->setStat("VITA", stats->getMax("VITA") * (1 + gain_pc));
  }

  /* Reflect - reflect handled in Battle */
  else if (ailment_type == REFLECT)
  {
      victim->setPersonFlag(Person::REFLECT, true);
  }

  /* Curse - Character is inflicted with a random ailment every turn.
   *         (Curse can inflict a new Curse--in which case just the remaining
   *         turns is reset.) Curse may also reset curse.
   */
  else if (ailment_type == CURSE)
  {
    //TODO: Curse effect unfinishd [05-05-13]
  }

  /* Metabolic Tether - Metabolic tether has a kMETABOLIC_PC chance for killing
   *   the inflicted, but also a kMETABOLIC_DMG percent that it will deal to
   *   the target.
   */
  else if (ailment_type == METATETHER)
  {
    int random_percent = getRandomNumber(100);

    /* Do kMETABOLIC_DMG % upon victim, emit signal if dead */
    if (victim->damage(stats->getMax("VITALITY") * kMETABOLIC_DMG))
        emit victimDeath(victim->getName(), METABOLICTETHER);

    /* Check for kMETABOLIC_PC chance for instant death */
    if (random_percent <= kMETABOLIC_PC * 100)
        emit victimDeath(victim->getName(), METABOLICDMG);
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
  /* Helper variables */
  QString race_name = new_victim->getRace()->getName();
  QString category_name = new_victim->getCategory()->getName();

  /* Flag immunity section */
  if (new_victim->getPersonFlag(Person::MINIBOSS))
  {
    if (ailment_type == DEATHTIMER || ailment_type == BUBBIFY)
      return false;
  }

  /* Bosses are immune to some ailments */
  else if (new_victim->getPersonFlag(Person::BOSS))
  {
    if (ailment_type == DEATHTIMER || ailment_type == BUBBIFY   ||
        ailment_type == SILENCE    || ailment_type == PARALYSIS ||
        ailment_type == BLINDNESS)
      return false;
  }

  /* Final boss will be immune to many ailments */
  else if (new_victim->getPersonFlag(Person::FINALBOSS))
  {
    if (ailment_type == DEATHTIMER || ailment_type == BUBBIFY   ||
        ailment_type == SILENCE    || ailment_type == PARALYSIS ||
        ailment_type == BLINDNESS  || ailment_type == CONFUSE   ||
        ailment_type == BERSERK)
      return false;
  }

  /* Bubby effect immunity section */
  if (ailment_type == ALLATKBUFF || ailment_type == ALLDEFBUFF ||
      ailment_type == PHYATKBUFF || ailment_type == PHYDEFBUFF ||
      ailment_type == THRATKBUFF || ailment_type == THRDEFBUFF ||
      ailment_type == POLATKBUFF || ailment_type == POLDEFBUFF ||
      ailment_type == PRIATKBUFF || ailment_type == PRIDEFBUFF ||
      ailment_type == CHGATKBUFF || ailment_type == CHGDEFBUFF ||
      ailment_type == CYBATKBUFF || ailment_type == CYBDEFBUFF ||
      ailment_type == NIHATKBUFF || ailment_type == NIHDEFBUFF ||
      ailment_type == LIMBUFF    || ailment_type == UNBBUFF    ||
      ailment_type == MOMBUFF    || ailment_type == VITBUFF    ||
      ailment_type == QDBUFF)
  return false;

  /* Race immunity section */
  if (race_name == "Human")
  {
    if (ailment_type == HIBERNATION || ailment_type == REFLECT ||
        ailment_type == CONFUSE)
      return false;
  }

  /* Bsians are not bears and don't get angry */
  else if (race_name == "Bsian")
  {
    if (ailment_type == HIBERNATION || ailment_type == BERSERK)
      return false;
  }

  /* Cyborgs are non-biological */
  else if (race_name == "Cyborg")
  {
    if (ailment_type == HIBERNATION || ailment_type == REFLECT ||
        ailment_type == POISON      || ailment_type == ROOTBOUND)
      return false;
  }

  /* Artilects are non-biological and immune to some others */
  else if (race_name == "Artilect")
  {
    if (ailment_type == HIBERNATION || ailment_type == REFLECT ||
        ailment_type == POISON      || ailment_type == BURN    ||
        ailment_type == CHAR        || ailment_type == SCALD)
      return false;
  }

  /* Gyrokin are ot bears */
  else if (race_name == "Gyrokin")
  {
    if (ailment_type == HIBERNATION || ailment_type == REFLECT)
      return false;
  }

  /* Necross are not bears */
  else if (race_name == "Necross")
  {
    if (ailment_type == HIBERNATION || ailment_type == REFLECT)
      return false;
  }

  /* Bears have hibernate instead of Rootbound */
  else if (race_name == "Bear")
  {
    if (ailment_type == ROOTBOUND)
      return false;
  }

  /* BOATs can't really be buffed up--they are extremely powerful */
  else if (race_name == "Boat")
  {
    if (ailment_type == ALLATKBUFF || ailment_type == ALLDEFBUFF ||
        ailment_type == PHYATKBUFF || ailment_type == PHYDEFBUFF ||
        ailment_type == THRATKBUFF || ailment_type == THRDEFBUFF ||
        ailment_type == POLATKBUFF || ailment_type == POLDEFBUFF ||
        ailment_type == PRIATKBUFF || ailment_type == PRIDEFBUFF ||
        ailment_type == CHGATKBUFF || ailment_type == CHGDEFBUFF ||
        ailment_type == CYBATKBUFF || ailment_type == CYBDEFBUFF ||
        ailment_type == NIHATKBUFF || ailment_type == NIHDEFBUFF ||
        ailment_type == LIMBUFF    || ailment_type == UNBBUFF    ||
        ailment_type == MOMBUFF    || ailment_type == VITBUFF    ||
        ailment_type == QDBUFF)
    return false;
  }

  /* Fiends are immune to reflect */
  else if (race_name == "Fiend")
  {
    if (ailment_type == REFLECT)
      return false;
  }

  /* Spirits can't be buffed and are immune to many other ailments */
  else if (race_name == "Spirit")
  {
    if (ailment_type == HIBERNATION || ailment_type == ALLATKBUFF    ||
        ailment_type == PHYATKBUFF  || ailment_type == PHYDEFBUFF ||
        ailment_type == THRATKBUFF  || ailment_type == THRDEFBUFF ||
        ailment_type == POLATKBUFF  || ailment_type == POLDEFBUFF ||
        ailment_type == PRIATKBUFF  || ailment_type == PRIDEFBUFF ||
        ailment_type == CHGATKBUFF  || ailment_type == CHGDEFBUFF ||
        ailment_type == CYBATKBUFF  || ailment_type == CYBDEFBUFF ||
        ailment_type == NIHATKBUFF  || ailment_type == NIHDEFBUFF ||
        ailment_type == LIMBUFF     || ailment_type == UNBBUFF    ||
        ailment_type == MOMBUFF     || ailment_type == VITBUFF    ||
        ailment_type == QDBUFF      || ailment_type == POISON     ||
        ailment_type == BURN        || ailment_type == SCALD      ||
        ailment_type == CHAR        || ailment_type == BUBBIFY    ||
        ailment_type == DEATHTIMER  || ailment_type == ROOTBOUND  ||
        ailment_type == ALLDEFBUFF)
    return false;
  }

  /* Category immunity section -- each category also allows an
   * additional ailment immunity */
  if (category_name == "Bardic Sage" && ailment_type == SILENCE)
      return false;
  else if (category_name == "Bloodclaw Scion" && ailment_type == DEATHTIMER)
      return false;
  else if (category_name == "Druidic Avenger" && ailment_type == POISON)
      return false;
  else if (category_name == "Eidoloncer" && ailment_type == SILENCE)
      return false;
  else if (category_name == "Goliath Rogue" && ailment_type == BLINDNESS)
      return false;
  else if (category_name == "Hexblade" && ailment_type == BERSERK)
      return false;
  else if (category_name == "Psion" && ailment_type == SILENCE)
      return false;
  else if (category_name == "Shadow Dancer" && ailment_type == PARALYSIS)
      return false;
  else if (category_name == "Storm Paladin" && ailment_type == DREADSTRUCK)
      return false;
  else if (category_name == "Swordsage" && ailment_type == DREAMSNARE)
      return false;
  else if (category_name == "Tactical Samurai" && ailment_type == CONFUSE)
      return false;
  else if (category_name == "Warmage" && ailment_type == HELLBOUND)
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
  qDebug() << "Beginning update turns: " << max_turns_left;

  /* If the ailment is finite, cure it based on chance */;
  if (max_turns_left <= kMAX_TURNS && chance != 0)
  {
    int random_num = getRandomNumber(1000);
    if (floor(chance * 1000) > random_num)
    {
      max_turns_left = 1;
      qDebug() << "Random chance!";
    }
  }
  /* If the ailment currently has one turn left, it's cured! */
  if (max_turns_left == 1)
    return true;
  /* If the ailment doesn't have one turn left, if it's finite, decrement it */
  if (max_turns_left <= kMAX_TURNS)
    max_turns_left--;

  qDebug() << " Ending update turns: " << max_turns_left;
  return false;
}

/*
 * Description: Sets the type of Infliction of the ailment
 *
 * Inputs: Infliction - type of Infliction to be set.
 * Output: none
 */
void Ailment::setType(Infliction t)
{
  ailment_type = t;
}

/*
 * Description: Sets the victim of the status ailment.
 *
 * Inputs: Infliction - type of Infliction to be set.
 * Output: none
 */
void Ailment::setVictim(Person* set_victim)
{
  victim = set_victim;
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
  AttributeSet* stats = victim->tempStats();
  SkillSet* skills = victim->getSkills();

  /* On removing Berserk, the person's abilities need to be re-enabled */
  if (getType() == BERSERK)
  {
    victim->setPersonFlag(Person::CANRUN, true);
    victim->setPersonFlag(Person::CANUSESKILLS, true);
    victim->setPersonFlag(Person::CANUSEITEM, true);
    victim->setDmgMod(1);
  }

  /* When silence is removed, skills need to be recalculated */
  else if (getType() == SILENCE)
  {

    for (int i = 0; i < skills->getSkills().size(); i++)
      skills->setSkillState(i, true);
  }

  /* When bubbify is removed, actor needs to return to normal (all other buffs
     are removed as well) */
  else if (getType() == BUBBIFY)
  {
    emit removeBuffs(victim->getName());
  }

  /* Remove the buffing effects from all attack stats */
  if (getType() == ALLATKBUFF)
  {
    for (int i = 0; i < stats->getSize(); i++)
    {
      QStringList split_stats = stats->getName(i).split("");
      if (split_stats.at(2) == "A" && split_stats.at(3) == "G")
        stats->setStat(i, stats->getStat(i) / kALLBUFF_PC);
    }
  }

  /* Remove the buffing effect from all defense stats */
  else if (getType() == ALLDEFBUFF)
  {
    for (int i = 0; i < stats->getSize(); i++)
    {
      QStringList split_stats = stats->getName(i).split("");
      if (split_stats.at(2) == "F" && split_stats.at(3) == "D")
        stats->setStat(i, stats->getStat(i) / kALLBUFF_PC);
    }
  }

  /* Remove the buffing effect from each stat */
  else if (ailment_type == PHYATKBUFF)
    stats->setStat("PHAG", stats->getStat("PHAG") / kPHYSBUFF_PC);
  else if (ailment_type == PHYDEFBUFF)
    stats->setStat("PHFD", stats->getStat("PHFD") / kPHYSBUFF_PC);
  else if (ailment_type == THRATKBUFF)
    stats->setStat("THAG", stats->getStat("THAG") / kELMBUFF_PC);
  else if (ailment_type == THRDEFBUFF)
    stats->setStat("THFD", stats->getStat("THFD") / kELMBUFF_PC);
  else if (ailment_type == POLATKBUFF)
    stats->setStat("POAG", stats->getStat("POAG") / kELMBUFF_PC);
  else if (ailment_type == POLDEFBUFF)
    stats->setStat("POFD", stats->getStat("POFD") / kELMBUFF_PC);
  else if (ailment_type == PRIATKBUFF)
    stats->setStat("PRAG", stats->getStat("PRAG") / kELMBUFF_PC);
  else if (ailment_type == PRIDEFBUFF)
    stats->setStat("PRFD", stats->getStat("PRFD") / kELMBUFF_PC);
  else if (ailment_type == CHGATKBUFF)
    stats->setStat("CHAG", stats->getStat("CHAG") / kELMBUFF_PC);
  else if (ailment_type == CHGDEFBUFF)
   stats->setStat("CHFD", stats->getStat("CHFD") / kELMBUFF_PC);
  else if (ailment_type == CYBATKBUFF)
    stats->setStat("CYAG", stats->getStat("CYAG") / kELMBUFF_PC);
  else if (ailment_type == CYBDEFBUFF)
    stats->setStat("CYFD", stats->getStat("CYFD") / kELMBUFF_PC);
  else if (ailment_type == NIHATKBUFF)
    stats->setStat("NIAG", stats->getStat("NIAG") / kELMBUFF_PC);
  else if (ailment_type == NIHDEFBUFF)
    stats->setStat("NIAG", stats->getStat("NIFD") / kELMBUFF_PC);
  else if (ailment_type == UNBBUFF)
    stats->setStat("UNBR", stats->getStat("UNBR") / kELMBUFF_PC);
  else if (ailment_type == LIMBUFF)
    stats->setStat("NIFD", stats->getStat("LIMB") / kLIMBUFF_PC);
  else if (ailment_type == MOMBUFF)
    stats->setStat("MMTM", stats->getStat("MMTM") / kELMBUFF_PC);
  else if (ailment_type == VITBUFF)
    stats->setStat("VITA", stats->getStat("VITA") / kVITBUFF_PC);
  else if (ailment_type == QDBUFF)
    stats->setStat("QTMN", stats->getStat("QTMN") / kQTMNBUFF_PC);

  /* Flip the flags allowing the person to use two or three skills per turn */
  else if (getType() == DOUBLECAST)
      victim->setPersonFlag(Person::TWOSKILLS, false);
  else if (getType() == TRIPLECAST)
      victim->setPersonFlag(Person::THREESKILLS, false);

  /* Half cost removal handled within battle */

  else if (getType() == CURSE)
  {
    /* Curse effect incomplete [05-13-13] */
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
  qDebug() << "AILMENT TYPE: " << getAilmentStr(ailment_type);
  qDebug() << "INFINITE: " << getFlag(Ailment::INFINITE);
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
Infliction Ailment::getType()
{
  return ailment_type;
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
  const std::string &string = EnumString<Infliction>::From( ailment_type );
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
void Ailment::setDuration(ushort max_turns, double chance)
{
  if (max_turns > kMAX_TURNS)
    setFlag(Ailment::INFINITE);
  else
    setFlag(Ailment::INFINITE, false);

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
  (set_value) ? (flag_set |= flags) : (flag_set ^= flags);
}

/*
 * Description: Assigns a PersonState flag or flags to (a) given value(s)
 *
 * Inputs: Perosn* new_victim - Potential new victim of the status ailment.
 *         bool refresh_turns - Whether to reset the turn count
 * Output: Returns true if the new victim was set successfully.
 */
bool Ailment::setNewVictim(Person* new_victim, bool refresh_turns)
{
  /* The ailment cannot be assigned if the new victim is immune */
  if (checkImmunity(new_victim))
    return false;

  if (refresh_turns)
  {
    max_turns_left += turns_occured;
    turns_occured = 0;
  }

  setFlag(Ailment::TOBEAPPLIED);
  setVictim(new_victim);
  update();
  return true;
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
    if (!getFlag(Ailment::INFINITE))
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

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the string value of a given infliction. Uses the
 *              EnumString class which generates std::strings, the actual
 *              registered strings are in EnumDatabase
 *
 * Inputs: Infliction - type of Infliction to be evaluated.
 * Output: QString - A QString of the given Infliction type.
 */
QString Ailment::getAilmentStr(Infliction type)
{
  const std::string &ailment_string = EnumString<Infliction>::From(type);
  QString ailment_qstring(ailment_string.c_str());
  return ailment_qstring;
}

/*
 * Description: Returns the Infliction of a given string (if one exists)
 *
 * Inputs: QString - name of Infliction to be checked for
 * Output: Infliction - the corresponding Infliction (NOAILMENT for default)
 */
Infliction Ailment::getInfliction(QString name)
{
  const std::string &ailment_string = name.toUtf8().constData();
  Infliction ailment_type;
  EnumString<Infliction>::To(ailment_type, ailment_string);
  return ailment_type;
}
