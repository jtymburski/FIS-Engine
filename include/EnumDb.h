/******************************************************************************
* Class Name: EnumDb
* Date Created: January 30th, 2013
* Inheritance:
* Description:  Enum Database holds non-flag Enums
 *****************************************************************************/
#include "EnumString.h"

#ifndef ENUMDB_H
#define ENUMDB_H

class EnumDb
{
public:
  /* ActorDeath enumeration is the enumerated type for reason for an actor's 
   * death in Battle */
  enum ActorDeath {STANDARDDMG, POISONDMG, BURNDMG, BERSERKHITBACK, 
                   METABOLICTETHER, METABOLICDMG, DEATHCOUNTDOWN, UNKNOWN};

  enum ApplicationMode {TITLESCREEN, GAME};

  /*
   * Description: The Attribute enumerations is used in AttributeSet to
   *              describe then names of the statics used by Persons,
   *              Items, etc.
   */
  enum Attribute
  {
    VITALITY, QUANTUMDRIVE, PHYSICALAGGRESSION, PHYSICALFORTITUDE,
    THERMALAGGRESSION, THERMALFORTITUDE, PRIMALAGGRESSION, PRIMALFORTITUDE,
    POLARAGGRESSION, POLARFORTITUDE, CHARGEDAGGRESSION, CHARGEDFORTITUDE,
    CYBERNETICAGGRESSION, CYBERNETICFORTITUDE, NIHILAGGRESSION,
    NIHILFORTITUDE, MOMENTUM, LIMBERTUDE, UNBEARABILITY
  };

  /*
   * Description: Handler for all direction related calls for movement, 
   *              passability, etc.
   */
  enum Direction {NORTH         = 0x1,
                  EAST          = 0x2,
                  SOUTH         = 0x4,
                  WEST          = 0x8,
                  DIRECTIONLESS = 0};

  /*
   * Description: Describes the current state of the inventory (how it is
   *              currently sorted)
   */
  enum ItemSorts {NONE, ID, NAME, FLAVOUR, LEVEL, EQUIPPABLE, VALUE, MASS,
                  VALUEPERMASS};

  /*
   * Description: Infliction enumeration represents a Status ailment or a 
   *              Status Buff. String conversions are listed blow.
   */
  enum Infliction
  {
    POISON, BURN, SCALD, INFLICTCHAR, BERSERK, CONFUSE, SILENCE, BUBBIFY, 
    DEATHTIMER, PARALYSIS, BLINDNESS, DREADSTRUCK, DREAMSNARE, HELLBOUND, 
    BOND, ALLATKBUFF, ALLDEFBUFF, PHYATKBUFF, PHYDEFBUFF, THRATKBUFF, 
    THRDEFBUFF, POLATKBUFF, POLDEFBUFF, PRIATKBUFF, PRIDEFBUFF, CHGATKBUFF, 
    CHGDEFBUFF, CYBATKBUFF, CYBDEFBUFF, NIHATKBUFF, NIHDEFBUFF, LIMBUFF, 
    UNBBUFF, MOMBUFF, VITBUFF, QDBUFF, ROOTBOUND, DOUBLECAST, TRIPLECAST, 
    HALFCOST, REFLECT, HIBERNATION, CURSE, METATETHER, NOAILMENT
  };

  /*
   * Description: Describes the current state of a skill set (how it is
   *              currently known to be sortd)
   */
  enum SkillSorts {NONE_SKILL, NAME_SKILL, LEVEL_SKILL};
};

/* Registration of Infliction Enumerator */
Begin_Enum_String( EnumDb::Infliction )
{
  RegisterEnumerator(EnumDb::POISON, "Poison");
  RegisterEnumerator(EnumDb::BURN, "Burn");
  RegisterEnumerator(EnumDb::SCALD, "Scald");
  RegisterEnumerator(EnumDb::INFLICTCHAR, "Char");
  RegisterEnumerator(EnumDb::BERSERK, "Berserk");
  RegisterEnumerator(EnumDb::CONFUSE, "Confuse");
  RegisterEnumerator(EnumDb::SILENCE, "Silence");
  RegisterEnumerator(EnumDb::BUBBIFY, "Bubbify");
  RegisterEnumerator(EnumDb::DEATHTIMER, "Deathtimer");
  RegisterEnumerator(EnumDb::PARALYSIS, "Paralysis");
  RegisterEnumerator(EnumDb::BLINDNESS, "Blindness");
  RegisterEnumerator(EnumDb::DREADSTRUCK, "Dreadstruck");
  RegisterEnumerator(EnumDb::DREAMSNARE, "Dreamsnare");
  RegisterEnumerator(EnumDb::HELLBOUND, "Hellbound");
  RegisterEnumerator(EnumDb::BOND, "Bond");
  RegisterEnumerator(EnumDb::ALLATKBUFF, "All-Aggression Buff");
  RegisterEnumerator(EnumDb::ALLDEFBUFF, "All-Fortitude Buff");
  RegisterEnumerator(EnumDb::PHYATKBUFF, "Physical Aggression Buff");
  RegisterEnumerator(EnumDb::PHYDEFBUFF, "Physical Fortitude Buff");
  RegisterEnumerator(EnumDb::THRATKBUFF, "Thermal Aggression Buff");
  RegisterEnumerator(EnumDb::THRDEFBUFF, "Thermal Fortitude Buff");
  RegisterEnumerator(EnumDb::POLATKBUFF, "Polar Aggression Buff");
  RegisterEnumerator(EnumDb::POLDEFBUFF, "Polar Fortitude Buff");
  RegisterEnumerator(EnumDb::PRIATKBUFF, "Primal Aggression Buff");
  RegisterEnumerator(EnumDb::PRIDEFBUFF, "Primal Fortitude Buff");
  RegisterEnumerator(EnumDb::CHGATKBUFF, "Charged Aggression Buff");
  RegisterEnumerator(EnumDb::CHGDEFBUFF, "Charged Fortitude Buff");
  RegisterEnumerator(EnumDb::CYBATKBUFF, "Cybernetic Aggression Buff");
  RegisterEnumerator(EnumDb::CYBDEFBUFF, "Cybernetic Fortitude Buff");
  RegisterEnumerator(EnumDb::NIHATKBUFF, "Nihil Aggression Buff");
  RegisterEnumerator(EnumDb::NIHDEFBUFF, "Nihil Fortitude Buff");
  RegisterEnumerator(EnumDb::LIMBUFF, "Limbertude Aggression Buff");
  RegisterEnumerator(EnumDb::UNBBUFF, "Unbearability Buff");
  RegisterEnumerator(EnumDb::MOMBUFF, "Momentum Buff");
  RegisterEnumerator(EnumDb::VITBUFF, "Vitality Buff");
  RegisterEnumerator(EnumDb::QDBUFF, "Quantum Drive Buff");
  RegisterEnumerator(EnumDb::ROOTBOUND, "Rootbound");
  RegisterEnumerator(EnumDb::DOUBLECAST, "Double Cast");
  RegisterEnumerator(EnumDb::TRIPLECAST, "Triple Cast");
  RegisterEnumerator(EnumDb::HALFCOST, "Half Cost");
  RegisterEnumerator(EnumDb::REFLECT, "Reflect");
  RegisterEnumerator(EnumDb::HIBERNATION, "Hibernation");
  RegisterEnumerator(EnumDb::CURSE, "Curse");
  RegisterEnumerator(EnumDb::METATETHER, "Metabolic Tether");
  RegisterEnumerator(EnumDb::NOAILMENT, "No Effect");
}
End_Enum_String;

/* Registration of Attribute Enumerator */
Begin_Enum_String( EnumDb::Attribute )
{
  RegisterEnumerator(EnumDb::VITALITY, "Vitality");
  RegisterEnumerator(EnumDb::QUANTUMDRIVE, "Quantum Drive");
  RegisterEnumerator(EnumDb::PHYSICALAGGRESSION, "Physical Aggression");
  RegisterEnumerator(EnumDb::PHYSICALAGGRESSION, "Physical Aggression");
  RegisterEnumerator(EnumDb::PHYSICALFORTITUDE, "Physical Fortitude");
  RegisterEnumerator(EnumDb::THERMALAGGRESSION, "Thermal Aggression");
  RegisterEnumerator(EnumDb::THERMALFORTITUDE, "Thermal Fortitude");
  RegisterEnumerator(EnumDb::PRIMALAGGRESSION, "Primal Aggression");
  RegisterEnumerator(EnumDb::PRIMALFORTITUDE, "Primal Fortitude");
  RegisterEnumerator(EnumDb::POLARAGGRESSION, "Polar Aggression");
  RegisterEnumerator(EnumDb::POLARFORTITUDE, "Polar Fortitude");
  RegisterEnumerator(EnumDb::CHARGEDAGGRESSION, "Charged Aggression");
  RegisterEnumerator(EnumDb::CHARGEDFORTITUDE, "Charged Fortitude");
  RegisterEnumerator(EnumDb::CYBERNETICFORTITUDE, "Cybernetic Fortitude");
  RegisterEnumerator(EnumDb::CYBERNETICFORTITUDE, "Cybernetic Fortitude");
  RegisterEnumerator(EnumDb::CYBERNETICAGGRESSION, "Cybernetic Fortitude");
  RegisterEnumerator(EnumDb::NIHILAGGRESSION, "Nihil Aggression");
  RegisterEnumerator(EnumDb::NIHILFORTITUDE, "Nihil Fortitude");
  RegisterEnumerator(EnumDb::MOMENTUM, "Momentum");
  RegisterEnumerator(EnumDb::LIMBERTUDE, "Limbertude");
  RegisterEnumerator(EnumDb::UNBEARABILITY, "Unbearability");
}
End_Enum_String;

#endif // ENUMDB_H
