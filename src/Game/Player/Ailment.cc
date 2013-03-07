/******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: None
* Description: The ailment class represents a Status Ailment or Debuff
* // TODO: RANDOM CHANCE [03-06-13]
******************************************************************************/
#include "Game/Player/Ailment.h"

Ailment::Ailment(Infliction t, ushort max, float ch)
{
  setType(t);
  setDuration(max, ch);
}

Ailment::Ailment()
{

}

const bool Ailment::update()
{
  // TOOD: INCORPORATE CHANCE [03-06-13]
  if (max_turns_left == 1)
    return TRUE;
  if (max_turns_left <= kMAX_TURNS)
    max_turns_left--;
  return FALSE;
}

const bool Ailment::updateAndApply(AttributeSet* stats)
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
  }
  return TRUE;
}

ushort Ailment::getTurnsLeft()
{
  return max_turns_left;
}

Infliction Ailment::getType()
{
  return ailment_type;
}

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
  }
  return "";
}

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
  return POISON;
}

void Ailment::setDuration(ushort max, float ch)
{
  max_turns_left = max;
  chance = chance;
}

void Ailment::setType(Infliction t)
{
  ailment_type = t;
}


