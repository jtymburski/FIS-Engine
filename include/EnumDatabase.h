/******************************************************************************
* Class Name: EnumDatabase
* Date Created: January 30th, 2013
* Inheritance:
* Description:  EnumDatabase holds non-flag Enums
 *****************************************************************************/
#include "EnumString.h"

#ifndef ENUMDATABASE_H
#define ENUMDATABASE_H

enum ApplicationMode {TITLESCREEN, GAME};

enum Direction {NORTH,EAST,SOUTH,WEST};

/*
 * Description: Infliction enumeration represents a Status ailment or a Status
 *              Buff. String conversions are listed blow.||||||||||
 */
enum Infliction
{
  POISON, BURN, SCALD, CHAR, BERSERK, CONFUSE, SILENCE, BUBBIFY, DEATHTIMER,
  PARALYSIS, BLINDNESS, DREADSTRUCK, DREAMSNARE, HELLBOUND, BOND, ALLATKBUFF,
  ALLDEFBUFF, PHYATKBUFF, PHYDEFBUFF, THRATKBUFF, THRDEFBUFF,POLATKBUFF,
  POLDEFBUFF, PRIATKBUFF, PRIDEFBUFF, CHGATKBUFF, CHGDEFBUFF, CYBATKBUFF,
  CYBDEFBUFF, NIHATKBUFF, NIHDEFBUFF, LIMBUFF, UNBBUFF, MOMBUFF, VITBUFF,
  QDBUFF, ROOTBOUND, DOUBLECAST, TRIPLECAST, HALFCOST, REFLECT, HIBERNATION,
  CURSE, NOAILMENT
};

Begin_Enum_String( Infliction )
{
  RegisterEnumerator(POISON, "Poison");
  RegisterEnumerator(BURN, "Burn");
  RegisterEnumerator(SCALD, "Scald");
  RegisterEnumerator(CHAR, "Char");
  RegisterEnumerator(BERSERK, "Berserk");
  RegisterEnumerator(CONFUSE, "Confuse");
  RegisterEnumerator(SILENCE, "Silence");
  RegisterEnumerator(BUBBIFY, "Bubbify");
  RegisterEnumerator(DEATHTIMER, "Deathtimer");
  RegisterEnumerator(PARALYSIS, "Paralysis");
  RegisterEnumerator(BLINDNESS, "Blindness");
  RegisterEnumerator(DREADSTRUCK, "Dreadstruck");
  RegisterEnumerator(DREAMSNARE, "Dreamsnare");
  RegisterEnumerator(HELLBOUND, "Hellbound");
  RegisterEnumerator(BOND, "Bond");
  RegisterEnumerator(ALLATKBUFF, "All-Aggression Buff");
  RegisterEnumerator(ALLDEFBUFF, "All-Fortitude Buff");
  RegisterEnumerator(PHYATKBUFF, "Physical Aggression Buff");
  RegisterEnumerator(PHYDEFBUFF, "Physical Fortitude Buff");
  RegisterEnumerator(THRATKBUFF, "Thermal Aggression Buff");
  RegisterEnumerator(THRDEFBUFF, "Thermal Fortitude Buff");
  RegisterEnumerator(POLATKBUFF, "Polar Aggression Buff");
  RegisterEnumerator(POLDEFBUFF, "Polar Fortitude Buff");
  RegisterEnumerator(PRIATKBUFF, "Primal Aggression Buff");
  RegisterEnumerator(PRIDEFBUFF, "Primal Fortitude Buff");
  RegisterEnumerator(CHGATKBUFF, "Charged Aggression Buff");
  RegisterEnumerator(CHGDEFBUFF, "Charged Fortitude Buff");
  RegisterEnumerator(CYBATKBUFF, "Cybernetic Aggression Buff");
  RegisterEnumerator(CYBDEFBUFF, "Cybernetic Fortitude Buff");
  RegisterEnumerator(NIHATKBUFF, "Nihil Aggression Buff");
  RegisterEnumerator(NIHDEFBUFF, "Nihil Fortitude Buff");
  RegisterEnumerator(LIMBUFF, "Limbertude Aggression Buff");
  RegisterEnumerator(UNBBUFF, "Unbearability Buff");
  RegisterEnumerator(MOMBUFF, "Momentum Buff");
  RegisterEnumerator(VITBUFF, "Vitality Buff");
  RegisterEnumerator(QDBUFF, "Quantum Drive Buff");
  RegisterEnumerator(ROOTBOUND, "Rootbound");
  RegisterEnumerator(DOUBLECAST, "Double Cast");
  RegisterEnumerator(TRIPLECAST, "Triple Cast");
  RegisterEnumerator(HALFCOST, "Half Cost");
  RegisterEnumerator(REFLECT, "Reflect");
  RegisterEnumerator(HIBERNATION, "Hibernation");
  RegisterEnumerator(CURSE, "Curse");
  RegisterEnumerator(NOAILMENT, "No Effect");
}
End_Enum_String;

/*
 * Description:The Attribute enumerations is used in AttributeSet to describe
 *              then names of the statics used by Persons, Items, etc.
 */
enum Attribute
{
  VITALITY, QUANTUM_DRIVE, PHYSICAL_AGGRESSION, PHYSICAL_FORTITUDE,
  THERMAL_AGGRESSION, THERMAL_FORTITUDE, PRIMAL_AGGRESSION, PRIMAL_FORTITUDE,
  POLAR_AGGRESSION, POLAR_FORTITUDE, CHARGED_AGGRESSION, CHARGED_FORTITUDE,
  CYBERNETIC_AGGRESSION, CYBERNETIC_FORTITUDE, NIHIL_AGGRESSION,
  NIHIL_FORTITUDE, MOMENTUM, LIMBERTUDE, UNBEARABILITY
};

Begin_Enum_String( Attribute )
{
  RegisterEnumerator(VITALITY, "Vitality");
  RegisterEnumerator(QUANTUM_DRIVE, "Quantum Drive");
  RegisterEnumerator(PHYSICAL_AGGRESSION, "Physical Aggression");
  RegisterEnumerator(PHYSICAL_AGGRESSION, "Physical Aggression");
  RegisterEnumerator(PHYSICAL_FORTITUDE, "Physical Fortitude");
  RegisterEnumerator(THERMAL_AGGRESSION, "Thermal Aggression");
  RegisterEnumerator(THERMAL_FORTITUDE, "Thermal Fortitude");
  RegisterEnumerator(PRIMAL_AGGRESSION, "Primal Aggression");
  RegisterEnumerator(PRIMAL_FORTITUDE, "Primal Fortitude");
  RegisterEnumerator(POLAR_AGGRESSION, "Polar Aggression");
  RegisterEnumerator(POLAR_FORTITUDE, "POLAR_FORTITUDE");
  RegisterEnumerator(CHARGED_AGGRESSION, "Charged Aggression");
  RegisterEnumerator(CHARGED_FORTITUDE, "Charged Fortitude");
  RegisterEnumerator(CYBERNETIC_FORTITUDE, "Cybernetic Fortitude");
  RegisterEnumerator(CYBERNETIC_FORTITUDE, "Cybernetic Fortitude");
  RegisterEnumerator(CYBERNETIC_AGGRESSION, "Cybernetic Fortitude");
  RegisterEnumerator(NIHIL_AGGRESSION, "Nihil Aggression");
  RegisterEnumerator(NIHIL_FORTITUDE, "Nihil Fortitude");
  RegisterEnumerator(MOMENTUM, "Momentum");
  RegisterEnumerator(LIMBERTUDE, "Limbertude");
  RegisterEnumerator(UNBEARABILITY, "Unbearability");
}
End_Enum_String;

#endif // ENUMDATABASE_H
