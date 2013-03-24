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
*              4 - (Add) the Ailment's effect to updateAndApply()
*
* // TODO: Add effects of apply() [03-12-13]
* // TODO: Add immunities to everything [03-12-13]
* // TODO: Finalize chance to wear off per turn [03-11-13]
* // TODO: Rigorous testing for class [03-11-13]
* // TODO: Setup temporary skills for person [03-12-13]
*******************************************************************************/
#include "Game/Player/Ailment.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/
const ushort Ailment::kMAX_TURNS          = 25;
const ushort Ailment::kMIN_TURNS          =  1;
const ushort Ailment::kPOISON_DMG_MAX     = 5000;
const ushort Ailment::kPOISON_DMG_MIN     = 50;
const double Ailment::kPOISON_DMG_INCR    = 1.05;
const double Ailment::kPOISON_DMG_INIT    = 1.08;
const ushort Ailment::kBURN_DMG_MAX       = 5000;
const ushort Ailment::kBURN_DMG_MIN       = 100;
const double Ailment::kBURN_DMG_INCR      = 1.02;
const double Ailment::kBURN_DMG_PC        = 1.05;
// const double Ailment::kBERSERK_DMG_INCR   = 1.75;
// const double Ailment::kBERSERK_HITBACK_PC = 0.35;

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
    setFlag(Ailment::TOBEUPDATED, FALSE);
  }
  else if (max_turns > kMAX_TURNS)
    setDuration(kMAX_TURNS, chance);
  else if (max_turns < kMIN_TURNS)
    setDuration(kMIN_TURNS, chance);
  else
    setDuration(max_turns, chance);
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
    setFlag(Ailment::TOBEUPDATED, FALSE);
  }
  else if (max_turns > kMAX_TURNS)
    setDuration(kMAX_TURNS, chance);
  else if (max_turns < kMIN_TURNS)
    setDuration(kMIN_TURNS, chance);
  else
    setDuration(max_turns, chance);
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
  setFlag(Ailment::TOBEUPDATED, FALSE);
  setDuration(-1,-1);
}

/*
 * Description: Annihilates a Status Ailment object
 */
Ailment::~Ailment() {}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Applies the effect of the status ailment to the Person (victim)
 *              if one so exists, checking for recurring effects, etc.
 *
 * Inputs: none
 * Output: bool - TRUE if the ailment is to be cured
 */
bool Ailment::apply()
{
  /* Helper variables */
  const ushort kHEALTH = victim->tempStats()->getStat(0);
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
  }

  /* Do the damage and if the actor is dead, set the Ailment to be cured */
  if (getFlag(Ailment::CUREONDEATH) && victim->damage(damage))
    setFlag(Ailment::TOBECURED);

  /* Berserk - Ailed actor physically attacks enemy target for extreme damage
   *           while receiving damage themselves.
   * Constants: kBERSERK_DMG_INCR - multiplier for normal damage on target
   *            kBERSERK_HITBACK_PC - multiplier for damage done to self.
   */
  else if (ailment_type == BERSERK)
  {
    /* On initial application, limit skills to Physical, disable run, items */
    /* Update effect handled in Battle -- damage calculations. */
  }

  /* Confuse - ailed actor attacks a random target with a random skills */
  else if (ailment_type == CONFUSE)
  {
    /* Find all useable skills */
    /* Find all valid targets */
    /* Pick a useable skill */
    /* Pick a valid target */
  }

  /* Ailed actor cannot use skills if they require QD */
  else if (ailment_type == SILENCE)
  {
    /* On application, remove skills which have a QD cost > 0 from useable */
  }

  /* Bubbify - ailed actor is turned into a near-useless Bubby */
  else if (ailment_type == BUBBIFY)
  {

  }

  /* Death Timer - Ailed actor KOs upon reaching max_turns */
  else if (ailment_type == DEATHTIMER)
  {
    /* Update death clock */
    /* On reaching max_turns, actor dies */
  }

  /* Ailed actor has a 70% chance of skipping their turn */
  else if (ailment_type == PARALYSIS)
  {
    /* 1-100, if >30, set skip next turn flag */
  }

  /* Ailed actor has a much higher chance of missing targets */
  else if (ailment_type == BLINDNESS)
  {

  }

  else if (ailment_type == DREADSTRUCK)
  {

  }
  else if (ailment_type == DREAMSNARE)
  {

  }
  else if (ailment_type == HELLBOUND)
  {

  }
  else if (ailment_type == BOND)
  {

  }
  else if (ailment_type == ALLATKBUFF)
  {

  }
  else if (ailment_type == ALLDEFBUFF)
  {

  }
  else if (ailment_type == PHYATKBUFF)
  {

  }
  else if (ailment_type == PHYDEFBUFF)
  {

  }
  else if (ailment_type == THRATKBUFF)
  {

  }
  else if (ailment_type == THRDEFBUFF)
  {

  }
  else if (ailment_type == POLATKBUFF)
  {

  }
  else if (ailment_type == POLDEFBUFF)
  {

  }
  else if (ailment_type == PRIATKBUFF)
  {

  }

  else if (ailment_type == PRIDEFBUFF)
  {

  }
  else if (ailment_type == CHGATKBUFF)
  {

  }
  else if (ailment_type == CHGDEFBUFF)
  {

  }
  else if (ailment_type == CYBATKBUFF)
  {

  }
  else if (ailment_type == CYBDEFBUFF)
  {

  }
  else if (ailment_type == NIHATKBUFF)
  {

  }
  else if (ailment_type == NIHDEFBUFF)
  {

  }
  else if (ailment_type == LIMBUFF)
  {

  }
  else if (ailment_type == UNBBUFF)
  {

  }
  else if (ailment_type == MOMBUFF)
  {

  }
  else if (ailment_type == VITBUFF)
  {

  }
  else if (ailment_type == QDBUFF)
  {
  }
  else if (ailment_type == ROOTBOUND)
  {
  }
  else if (ailment_type == DOUBLECAST)
  {
  }
  else if (ailment_type == TRIPLECAST)
  {
  }
  else if (ailment_type == HALFCOST)
  {
  }
  else if (ailment_type == REFLECT)
  {
  }
  else if (ailment_type == HIBERNATION)
  {
  }
  else if (ailment_type == CURSE)
  {

  }
  else
  {
    throw("Error: Unrecognized Ailment");
  }
  return TRUE;
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
    (void)new_victim;//warning
  /* Immunity for semi-bosses */

  /* Immunity for bosses */

  /* Immunity for final boss */

  /* Racial immunity */

  /* Class immunity */

  /* Other immunity */
  return TRUE;
}

/*
 * Description: Updates the turn counter on the status ailment based off
 *              the random chance to cure each turn (if not zero). Returns true
 *              if the ailment will be cured after this update, false otherwise.
 *
 * Inputs: none
 * Output: bool - TRUE if the ailment is to be cured after the update of Fn.
 */
bool Ailment::updateTurns()
{
  /* If the ailment is finite, cure it based on chance */;
  if (max_turns_left <= kMAX_TURNS && chance != 0)
  {
    qsrand(QTime::currentTime().msec());
    if (floor(chance * 1000) > rand()%(1000))
        max_turns_left = 1;
  }
  /* If the ailment currently has one turn left, it's cured! */
  if (max_turns_left == 1)
    return TRUE;
  /* If the ailment doesn't have one turn left, if it's finite, decrement it */
  if (max_turns_left <= kMAX_TURNS)
    max_turns_left--;
  return FALSE;
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
 * SIGNALS
 *============================================================================*/

/*=============================================================================
 * PUBLIC SLOTS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Public update function. This function will handle calling the
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
    bool cure_value = FALSE;
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
 * Description: Unapplies the effects of the status ailment to the victim.
 *
 * Inputs: none
 * Output: none
 */
void Ailment::unapply()
{
  if (getType() == BERSERK)
  {

  }
  if (getType() == SILENCE)
  {

  }
  if (getType() == BUBBIFY)
  {

  }
  if (getType() == PARALYSIS)
  {

  }
  if (getType() == BLINDNESS)
  {

  }
  if (getType() == DREADSTRUCK)
  {

  }
  if (getType() == DREAMSNARE)
  {

  }
  if (getType() == HELLBOUND)
  {

  }
  if (getType() == ALLATKBUFF)
  {

  }
  if (getType() == ALLDEFBUFF)
  {

  }
  if (getType() == PHYATKBUFF)
  {

  }
  else if (getType() == PHYDEFBUFF)
  {

  }
  else if (getType() == THRATKBUFF)
  {

  }
  else if (getType() == THRDEFBUFF)
  {

  }
  else if (getType() == POLATKBUFF)
  {

  }
  else if (getType() == POLDEFBUFF)
  {

  }
  else if (getType() == PRIATKBUFF)
  {

  }
  else if (getType() == PRIDEFBUFF)
  {

  }
  else if (getType() == CHGATKBUFF)
  {

  }
  else if (getType() == CHGDEFBUFF)
  {

  }
  else if (getType() == CYBATKBUFF)
  {

  }
  else if (getType() == CYBDEFBUFF)
  {

  }
  else if (getType() == NIHATKBUFF)
  {

  }
  else if (getType() == NIHDEFBUFF)
  {

  }
  else if (getType() == LIMBUFF)
  {

  }
  else if (getType() == UNBBUFF)
  {

  }
  else if (getType() == MOMBUFF)
  {

  }
  else if (getType() == VITBUFF)
  {

  }
  else if (getType() == QDBUFF)
  {

  }
  else if (getType() == DOUBLECAST)
  {

  }
  else if (getType() == TRIPLECAST)
  {

  }
  else if (getType() == HALFCOST)
  {

  }
  else if (getType() == REFLECT)
  {

  }
  else if (getType() == HIBERNATION)
  {

  }
  else if (getType() == CURSE)
  {

  }
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
    setFlag(Ailment::INFINITE, FALSE);

  max_turns_left = max_turns;
  this->chance = chance;
}

/*
 * Description: Assigns a PersonState flag or flags to (a) given value(s)
 *
 * Inputs: AilmentFlag flags - enumerated flag to be assigned
 *         set_value - the value to assign the flag(s) to (default: TRUE)
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
 * Output: Returns TRUE if the new victim was set successfully.
 */
bool Ailment::setNewVictim(Person* new_victim, bool refresh_turns)
{
  /* The ailment cannot be assigned if the new victim is immune */
  if (checkImmunity(new_victim))
    return FALSE;

  if (refresh_turns)
  {
    max_turns_left += turns_occured;
    turns_occured = 0;
  }

  setFlag(Ailment::TOBEAPPLIED);
  setVictim(new_victim);
  update();
  return TRUE;
}
