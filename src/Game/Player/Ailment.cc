/******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: None
* Description: The ailment class represents a Status Ailment or Debuff
* // TODO: finish updateAndApply() function [03-09-13]
******************************************************************************/
#include "Game/Player/Ailment.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs an Ailment object given an Infliction type,
 *              a maximum duration and a % chance that will cure the ailment
 *              every turn.
 *
 * Inputs: Infliction - The type of the ailment being created
 *         ushort max - the maximum duration the ailment will take place,
 *                      (the ailment will last forever if max is greater than
 *                       kMAX_TURNS)
 *         double ch  - % value the ailment will be cured per turn, >= 1 = 100%
 */
Ailment::Ailment(Infliction t, ushort max, double ch)
{
  setType(t);
  setDuration(max, ch);
}

/*
 * Description: Default ailment constructor (constructs a NOAILMENT type)
 *
 * Inputs: none
 */
Ailment::Ailment()
{
  setType(NOAILMENT);
  setDuration(1);
}

/*
 * Description: Annihilates a Status Ailment object
 */
Ailment::~Ailment() {}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Updates the turn counter on the status ailment based off
 *              the random chance to cure each turn (if not zero). Returns true
 *              if the ailment will be cured after this update, false otherwise.
 *
 * Inputs: none
 * Output: bool - TRUE if the ailment is to be cured
 */
const bool Ailment::update()
{
  if (max_turns_left <= kMAX_TURNS && chance != 0)
  {
    // QTime zero(0, 0, 0);
    // qsrand(zero.secsTo(QTime::currentTime()));
    // int random = qsrand() % 99;

    //if ((int)(chance * 100) > random)
      max_turns_left = 1;
  }
  if (max_turns_left == 1)
    return TRUE;
  if (max_turns_left <= kMAX_TURNS)
    max_turns_left--;
  return FALSE;
}

/*
 * Description: Updates the turn increment counter on the status ailment after
 *              applying the status ailments effect on a Person. Returns true
 *              if the status ailment is to be cured, false otherwise.
 *
 * Inputs: Person* - pointer to the person to apply the ailment to
 * Output: bool - TRUE if the ailment is to be cured
 */
const bool Ailment::updateAndApply(AttributeSet *attr_set, SkillSet *skill_set)
{
  switch (ailment_type)
  {
    case POISON:
      /* Do Poison damage */
      break;
    case BURN:
      /* Do Burn damage */
      break;
    case SCALD:
      /* Do scald damage */
      break;
    case CHAR:
      break;
    case BERSERK:
      break;
    case CONFUSE:
      break;
    case SILENCE:
      break;
    case BUBBIFY:
      break;
    case DEATHTIMER:
      break;
    case PARALYSIS:
      break;
    case BLINDNESS:
      break;
    case DREADSTRUCK:
      break;
    case DREAMSNARE:
      break;
    case HELLBOUND:
      break;
    case BOND:
      break;
    case ALLATKBUFF:
      break;
    case ALLDEFBUFF:
      break;
    case PHYATKBUFF:
      break;
    case PHYDEFBUFF:
      break;
    case THRATKBUFF:
      break;
    case THRDEFBUFF:
      break;
    case POLATKBUFF:
      break;
    case POLDEFBUFF:
      break;
    case PRIATKBUFF:
      break;
    case PRIDEFBUFF:
      break;
    case CHGATKBUFF:
      break;
    case CHGDEFBUFF:
      break;
    case CYBATKBUFF:
      break;
    case CYBDEFBUFF:
      break;
    case NIHATKBUFF:
      break;
    case NIHDEFBUFF:
      break;
    case LIMBUFF:
      break;
    case UNBBUFF:
      break;
    case MOMBUFF:
      break;
    case VITBUFF:
      break;
    case QDBUFF:
      break;
    case ROOTBOUND:
      break;
    case DOUBLECAST:
      break;
    case TRIPLECAST:
      break;
    case HALFCOST:
      break;
  case NOAILMENT:
      break;
  }
  return TRUE;
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
 * Description: Returns the string value of a given infliction
 *
 * Inputs: Infliction - type of Infliction to be checked.
 * Output: QString - string of the infliction
 */
QString Ailment::getName(Infliction type)
{
  switch (type)
  {
    case POISON:      return "POISON";
    case BURN:        return "BURN";
    case SCALD:       return "SCALD";
    case CHAR:        return "CHAR";
    case BERSERK:     return "BERSERK";
    case CONFUSE:     return "CONFUSE";
    case SILENCE:     return "SILENCE";
    case BUBBIFY:     return "BUBBIFY";
    case DEATHTIMER:  return "DEATHTIMER";
    case PARALYSIS:   return "PARALYSIS";
    case BLINDNESS:   return "BLINDNESS";
    case DREADSTRUCK: return "DREADSTRUCK";
    case DREAMSNARE:  return "DREAMSNARE";
    case HELLBOUND:   return "HELLBOUND";
    case BOND:        return "BOND";
    case ALLATKBUFF:  return "ALLATKBUFF";
    case ALLDEFBUFF:  return "ALLEFBUFF";
    case PHYATKBUFF:  return "PHYATKBUFF";
    case PHYDEFBUFF:  return "PHYDEFBUFF";
    case THRATKBUFF:  return "THRATKBUFF";
    case THRDEFBUFF:  return "THRDEFBUFF";
    case POLATKBUFF:  return "POLATKBUFF";
    case POLDEFBUFF:  return "POLDEFBUFF";
    case PRIATKBUFF:  return "PRIATKBUFF";
    case PRIDEFBUFF:  return "PRIDEFBUFF";
    case CHGATKBUFF:  return "CHGATKBUFF";
    case CHGDEFBUFF:  return "CHGDEFBUFF";
    case CYBATKBUFF:  return "CYBATKBUFF";
    case CYBDEFBUFF:  return "CYBDEFBUFF";
    case NIHATKBUFF:  return "NIHATKBUFF";
    case NIHDEFBUFF:  return "NIHDEFBUFF";
    case LIMBUFF:     return "LIMBUFF";
    case UNBBUFF:     return "UNBBUFF";
    case MOMBUFF:     return "MOMBUFF";
    case VITBUFF:     return "VITBUFF";
    case QDBUFF:      return "QDBUFF";
    case ROOTBOUND:   return "ROOTBOUND";
    case DOUBLECAST:  return "DOUBLECAST";
    case TRIPLECAST:  return "TRIPLECAST";
    case HALFCOST:    return "HALFCOST";
    case NOAILMENT:   return "ERROR";
  }
  return "";
}

/*
 * Description: Returns the Infliction of a given string (if one exists)
 *
 * Inputs: QString - name of Infliction to be checked for
 * Output: Infliction - the corresponding Infliction (NOAILMENT for default)
 */
Infliction Ailment::getInfliction(QString name)
{
  if (name == "POISON")       return POISON;
  if (name == "SCALD")        return SCALD;
  if (name == "CHAR")         return CHAR;
  if (name == "BERSERK")      return BERSERK;
  if (name == "SILENCE")      return SILENCE;
  if (name == "BUBBIFY")      return BUBBIFY;
  if (name == "DEATHTIMER")   return DEATHTIMER;
  if (name == "PARALYSIS")    return PARALYSIS;
  if (name == "BLINDNESS")    return BLINDNESS;
  if (name == "DREADSTRUCK")  return DREADSTRUCK;
  if (name == "DREAMSNARE")   return DREAMSNARE;
  if (name == "HELLBOUND")    return HELLBOUND;
  if (name == "BOND")         return BOND;
  if (name == "ALLATKBUFF")   return ALLATKBUFF;
  if (name == "ALLEFBUFF")    return ALLDEFBUFF;
  if (name == "PHYATKBUFF")   return PHYATKBUFF;
  if (name == "PHYDEFBUFF")   return PHYDEFBUFF;
  if (name == "THRATKBUFF")   return THRATKBUFF;
  if (name == "THRDEFBUFF")   return THRDEFBUFF;
  if (name == "POLATKBUFF")   return POLATKBUFF;
  if (name == "POLDEFBUFF")   return POLDEFBUFF;
  if (name == "PRIATKBUFF")   return PRIATKBUFF;
  if (name == "PRIDEFBUFF")   return PRIDEFBUFF;
  if (name == "CHGATKBUFF")   return CHGATKBUFF;
  if (name == "CHGDEFBUFF")   return CHGDEFBUFF;
  if (name == "CYBATKBUFF")   return CYBATKBUFF;
  if (name == "CYBDEFBUFF")   return CYBDEFBUFF;
  if (name == "NIHATKBUFF")   return NIHATKBUFF;
  if (name == "NIHDEFBUFF")   return NIHDEFBUFF;
  if (name == "LIMBUFF")      return LIMBUFF;
  if (name == "UNBUFFF")      return UNBBUFF;
  if (name == "MOMBUFF")      return MOMBUFF;
  if (name == "VITBUFF")      return VITBUFF;
  if (name == "QDBUFF")       return QDBUFF;
  if (name == "ROOTBOUND")    return ROOTBOUND;
  if (name == "DOUBLECAST")   return DOUBLECAST;
  if (name == "TRIPLECAST")   return TRIPLECAST;
  if (name == "HALFCOST")     return HALFCOST;
  return NOAILMENT;
}

/*
 * Description: Sets the maximum duration of the ailment as well as the chance
 *
 * Inputs: ushort max - maximum number of turns the ailment will persist
 *         float ch   - chance the ailment has to be cured per turn
 * Output: none
 */
void Ailment::setDuration(ushort max, double ch)
{
  max_turns_left = max;
  chance = chance;
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


