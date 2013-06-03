/******************************************************************************
* Class Name: EnumDatabase
* Date Created: January 30th, 2013
* Inheritance:
* Description:  EnumDatabase holds non-flag Enums
 *****************************************************************************/
#include "EnumString.h"

#ifndef ENUMDATABASE_H
#define ENUMDATABASE_H

/* ActorDeath enumeration is the enumerated type for reason for an actor's death
   in Battle */
enum ActorDeath {STANDARDDMG, POISONDMG, BURNDMG, BERSERKHITBACK, METABOLICTETHER,
                 METABOLICDMG, DEATHCOUNTDOWN, UNKNOWN};

enum ApplicationMode {TITLESCREEN, GAME};

enum Direction {NORTH,EAST,SOUTH,WEST};

enum ItemSorts {NONE, ID, NAME, FLAVOUR, LEVEL, EQUIPPABLE, VALUE, MASS,
                VALUEPERMASS};

/*
 * Description: Infliction enumeration represents a Status ailment or a Status
 *              Buff. String conversions are listed blow.
 */
enum Infliction
{
  POISON, BURN, SCALD, CHAR, BERSERK, CONFUSE, SILENCE, BUBBIFY, DEATHTIMER,
  PARALYSIS, BLINDNESS, DREADSTRUCK, DREAMSNARE, HELLBOUND, BOND, ALLATKBUFF,
  ALLDEFBUFF, PHYATKBUFF, PHYDEFBUFF, THRATKBUFF, THRDEFBUFF,POLATKBUFF,
  POLDEFBUFF, PRIATKBUFF, PRIDEFBUFF, CHGATKBUFF, CHGDEFBUFF, CYBATKBUFF,
  CYBDEFBUFF, NIHATKBUFF, NIHDEFBUFF, LIMBUFF, UNBBUFF, MOMBUFF, VITBUFF,
  QDBUFF, ROOTBOUND, DOUBLECAST, TRIPLECAST, HALFCOST, REFLECT, HIBERNATION,
  CURSE, METATETHER, NOAILMENT
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
  RegisterEnumerator(METATETHER, "Metabolic Tether");
  RegisterEnumerator(NOAILMENT, "No Effect");
}
End_Enum_String;

/*
 * Description:The Attribute enumerations is used in AttributeSet to describe
 *              then names of the statics used by Persons, Items, etc.
 */
enum Attribute
{
  VITALITY, QUANTUMDRIVE, PHYSICALAGGRESSION, PHYSICALFORTITUDE,
  THERMALAGGRESSION, THERMALFORTITUDE, PRIMALAGGRESSION, PRIMALFORTITUDE,
  POLARAGGRESSION, POLARFORTITUDE, CHARGEDAGGRESSION, CHARGEDFORTITUDE,
  CYBERNETICAGGRESSION, CYBERNETICFORTITUDE, NIHILAGGRESSION,
  NIHILFORTITUDE, MOMENTUM, LIMBERTUDE, UNBEARABILITY
};

Begin_Enum_String( Attribute )
{
  RegisterEnumerator(VITALITY, "Vitality");
  RegisterEnumerator(QUANTUMDRIVE, "Quantum Drive");
  RegisterEnumerator(PHYSICALAGGRESSION, "Physical Aggression");
  RegisterEnumerator(PHYSICALAGGRESSION, "Physical Aggression");
  RegisterEnumerator(PHYSICALFORTITUDE, "Physical Fortitude");
  RegisterEnumerator(THERMALAGGRESSION, "Thermal Aggression");
  RegisterEnumerator(THERMALFORTITUDE, "Thermal Fortitude");
  RegisterEnumerator(PRIMALAGGRESSION, "Primal Aggression");
  RegisterEnumerator(PRIMALFORTITUDE, "Primal Fortitude");
  RegisterEnumerator(POLARAGGRESSION, "Polar Aggression");
  RegisterEnumerator(POLARFORTITUDE, "Polar Fortitude");
  RegisterEnumerator(CHARGEDAGGRESSION, "Charged Aggression");
  RegisterEnumerator(CHARGEDFORTITUDE, "Charged Fortitude");
  RegisterEnumerator(CYBERNETICFORTITUDE, "Cybernetic Fortitude");
  RegisterEnumerator(CYBERNETICFORTITUDE, "Cybernetic Fortitude");
  RegisterEnumerator(CYBERNETICAGGRESSION, "Cybernetic Fortitude");
  RegisterEnumerator(NIHILAGGRESSION, "Nihil Aggression");
  RegisterEnumerator(NIHILFORTITUDE, "Nihil Fortitude");
  RegisterEnumerator(MOMENTUM, "Momentum");
  RegisterEnumerator(LIMBERTUDE, "Limbertude");
  RegisterEnumerator(UNBEARABILITY, "Unbearability");
}
End_Enum_String;

#endif // ENUMDATABASE_H
