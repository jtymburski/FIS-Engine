/*******************************************************************************
* Class Name: BattleStats
* Date Created: September 3, 2015
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#ifndef BATTLESTATS_H
#define BATTLESTATS_H

#include <vector>
#include <iostream>

#include "Game/Player/Ailment.h"
#include "Game/Player/AttributeSet.h"

using std::begin;
using std::end;

/* Modifiers for Battle Attributes */
enum class ModifierType : int
{
  ADDITIVE = 0,
  MULTIPLICATIVE = 1,
};

/* Structure which describes the modifications for the Attributes */
struct StatModifier
{
  /* Does this modifier have a lifetime? */
  bool has_lifetime;

  /* Does this modifier have a linked ailment? */
  bool has_linked_ailment;

  /* The lifetime of the stat modifier */
  int32_t lifetime;

  /* The ailment which causes the StatModifier to exist */
  Ailment *linked_ailment;

  /* The enumerated attribute to be altered */
  BattleAttribute stat;

  /* The enumerated modification type to be applied to the base value */
  ModifierType type;

  /* The actual value by which the stat is modified */
  float value;
};

class BattleStats
{
public:
  /* Construct the BattleStats with values given by an AttributeSet */
  BattleStats(AttributeSet attr_set);

  /* A 'default' BattleStats element cannot be created */
  BattleStats() = delete;

private:
  /* The base values for the BattleStats */
  std::vector<std::pair<BattleAttribute, uint32_t>> values;

  /* All modifiers currently applied to the BattleAttributes */
  std::vector<StatModifier> modifiers;

  /* ------------ Constants --------------- */
  static const uint32_t kMAX_ADDITIVE_MODS; /* Max # of + mods for each Attr */

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Attempts to add a modifier given necessary information. */
  bool addModifier(BattleAttribute stat, ModifierType type, float value,
                   bool has_lifetime = false, int32_t lifetime = 0,
                   Ailment *linked_ailment = nullptr);

  /* Pops the oldest non-ailment linked additive modifier, if one exists */
  bool popBackAdditive();

  /* Removes a modifier from the Battle stats by the given ailment */
  bool removeLinked(Ailment* linked_ailment);

  /* Prints out information of the BattleStat */
  void print();

  /* Updates the modifiers of the BattleStat class based on given lifetime */
  void update(int32_t lifetime_update);

  /* Returns the base value stored of a given BattleAttribute */
  uint32_t getBaseValue(BattleAttribute stat);

  /* Returns the value of a given Attribute after modifiers are applied */
  uint32_t getValue(BattleAttribute stat);

  /* Returns a vector of all modifiers for a given Attribute stat */
  std::vector<StatModifier> getModifiersOfStat(BattleAttribute battle_stat);

  /* Returns the number of additive modifiers for a given Attribute */
  uint32_t getNumAddModifiers(BattleAttribute battle_stat);

  /* Assigns a given BattleAttribute stat to a given value */
  bool setBaseValue(BattleAttribute stat, uint32_t value);
};

#endif // BATTLESTATS_H
