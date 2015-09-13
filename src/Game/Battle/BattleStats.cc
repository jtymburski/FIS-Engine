/*******************************************************************************
* Class Name: BattleActor [Implementation]
* Date Created: June 22, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/Battle/BattleStats.h"
#include "Game/Battle/BattleActor.h"
#include "Game/Player/Ailment.h"

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/

const uint32_t BattleStats::kMAX_ADDITIVE_MODS{7};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTOR
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
BattleStats::BattleStats(AttributeSet attr_set)
{
  /* Add in the AttributeSet by pairing it with corres. Attribute enums */
  for(uint32_t i = 0; i < AttributeSet::getSize(); i++)
  {
    Attribute curr_attr = static_cast<Attribute>(i);
    values.push_back(std::make_pair(curr_attr, attr_set.getStat(i)));
  }

  /* Add Battle-only additional attribute values */
  values.push_back(
      std::make_pair(Attribute::MVIT, attr_set.getStat(Attribute::VITA)));
  values.push_back(
      std::make_pair(Attribute::MQTD, attr_set.getStat(Attribute::QTDR)));
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool BattleStats::addModifier(Attribute stat, ModifierType type, float value,
                              bool has_lifetime, int32_t lifetime,
                              Ailment* linked_ailment)
{
  bool success = true;

  /* Additive modifiers must be within limit for the given Attribute */
  if(type == ModifierType::ADDITIVE)
    success &= ((getNumAddModifiers(stat) + 1) <= kMAX_ADDITIVE_MODS);
  /* If stat multipliers have a lifetime, their lifetime should be non-zero */
  if(has_lifetime)
    success &= (lifetime > 0);

  if(success)
  {
    StatModifier new_stat_modifier;

    new_stat_modifier.stat = stat;
    new_stat_modifier.type = type;
    new_stat_modifier.value = value;
    new_stat_modifier.has_lifetime = has_lifetime;
    new_stat_modifier.lifetime = lifetime;
    new_stat_modifier.has_linked_ailment = (linked_ailment != nullptr);
    new_stat_modifier.linked_ailment = linked_ailment;

    modifiers.push_back(new_stat_modifier);
  }

  return success;
}

bool BattleStats::popBackAdditive()
{
  if(modifiers.size() > 0)
  {
    auto it = std::find_if(begin(modifiers), end(modifiers),
                           [&](const StatModifier a) -> bool
                           {
                             return (a.type == ModifierType::ADDITIVE);
                           });
    if(it != end(modifiers))
    {
      modifiers.erase(it);

      return true;
    }
  }

  return false;
}

bool BattleStats::removeLinked(Ailment* linked_ailment)
{
  /* If the index is valid and the linked ailment is not null, do an
     erase remove for the matching ailment pointer. */
  if(modifiers.size() > 0 && linked_ailment)
  {
    auto it = std::find_if(begin(modifiers), end(modifiers),
                           [&](const StatModifier a) -> bool
                           {
                             return a.linked_ailment == linked_ailment;
                           });

    if(it != end(modifiers))
    {
      modifiers.erase(it);

      return true;
    }
  }

  return false;
}

void BattleStats::print()
{
  /* Print out each of the base values */
  for(const auto& value : values)
  {
    if(static_cast<uint8_t>(value.first) < AttributeSet::getSize())
    {
      std::cout << AttributeSet::getName(static_cast<Attribute>(value.first))
                << ": " << value.second << std::endl;
    }
    else if(value.first == Attribute::MVIT)
      std::cout << "MVIT: " << value.second << std::endl;
    else if(value.first == Attribute::MQTD)
      std::cout << "MQTD: " << value.second << std::endl;
  }

  std::cout << std::endl;

  /* Print out the modifiers */
  for(const auto& modifier : modifiers)
  {
    if(modifier.type == ModifierType::MULTIPLICATIVE)
      std::cout << "---- Modifier Type: MULTIPLICATIVE ----" << std::endl;
    else
      std::cout << "---- Modifier Type: ADDITIVE ----" << std::endl;

    if(static_cast<uint8_t>(modifier.stat) < AttributeSet::getSize())
      std::cout << AttributeSet::getName(static_cast<Attribute>(modifier.stat));
    else if(modifier.stat == Attribute::MVIT)
      std::cout << "MVIT";
    else if(modifier.stat == Attribute::MQTD)
      std::cout << "MQTD";
    std::cout << " Value: " << modifier.value << std::endl;

    if(modifier.has_lifetime)
      std::cout << "Lifetime: " << modifier.lifetime << std::endl;
    if(modifier.has_linked_ailment)
      std::cout << "Linked Ailment: " << modifier.linked_ailment << std::endl;
  }

  if(modifiers.size() > 0)
    std::cout << std::endl;

  /* Print out the actual calculated values of the BattleStats */
  for(const auto& value : values)
  {
    auto final_value = getValue(value.first);

    if(static_cast<uint8_t>(value.first) < AttributeSet::getSize())
    {
      std::cout << AttributeSet::getName(static_cast<Attribute>(value.first))
                << ": " << final_value << std::endl;
    }
    else if(value.first == Attribute::MVIT)
      std::cout << "MVIT: " << final_value << std::endl;
    else if(value.first == Attribute::MQTD)
      std::cout << "MQTD: " << final_value << std::endl;
  }
}

void BattleStats::update(int32_t lifetime_update)
{
  /* Update lifetime based on the given value for elements which have life */
  for(auto& modifier : modifiers)
    if(modifier.has_lifetime)
      modifier.lifetime -= lifetime_update;

  /* Erase remove-if all elements with lifetime and negative lifetime */
  modifiers.erase(std::remove_if(begin(modifiers), end(modifiers),
                                 [&](const StatModifier a) -> bool
                                 {
                                   return (a.has_lifetime && a.lifetime <= 0);
                                 }),
                  end(modifiers));

  /* Erase remove-if all elements with nullptr ailments */
  modifiers.erase(std::remove_if(begin(modifiers), end(modifiers),
                                 [&](const StatModifier a) -> bool
                                 {
                                   return (a.linked_ailment == nullptr);
                                 }),
                  end(modifiers));
}

uint32_t BattleStats::getBaseValue(Attribute stat)
{
  if(static_cast<uint8_t>(stat) < (uint8_t)values.size())
    return values.at(static_cast<uint8_t>(stat)).second;

  return 0;
}

// Returns the actual value of the stat after all modifiers
uint32_t BattleStats::getValue(Attribute stat)
{
  double value = getBaseValue(stat);

  /* If the given Attribute is a valid index */
  if(static_cast<uint8_t>(stat) < (uint8_t)values.size())
  {
    /* Obtain all modifiers of the stats */
    auto stat_modifiers = getModifiersOfStat(stat);

    /* First, apply all multiplicitve modifiers */
    for(const auto& modifier : stat_modifiers)
      if(modifier.type == ModifierType::MULTIPLICATIVE)
        value *= modifier.value;

    /* Second ,apply all additive modifiers */
    for(const auto& modifier : stat_modifiers)
      if(modifier.type == ModifierType::ADDITIVE)
        value += modifier.value;
  }

  /* Cast, round, and set the value within proper limits */
  auto final_val = static_cast<uint32_t>(std::round(value));
  final_val =
      Helpers::setInRange(final_val, (uint32_t)0, AttributeSet::getMaxValue());

  return final_val;
}

std::vector<StatModifier> BattleStats::getModifiersOfStat(Attribute battle_stat)
{
  std::vector<StatModifier> found_modifiers{};

  for(const auto& modifier : modifiers)
    if(modifier.stat == battle_stat)
      found_modifiers.push_back(modifier);

  return found_modifiers;
}

uint32_t BattleStats::getNumAddModifiers(Attribute battle_stat)
{
  uint32_t num_add_modifiers;

  for(const auto& modifier : modifiers)
    if(modifier.type == ModifierType::ADDITIVE && modifier.stat == battle_stat)
      ++num_add_modifiers;

  return num_add_modifiers;
}

bool BattleStats::setBaseValue(Attribute stat, uint32_t value)
{
  if(static_cast<uint8_t>(stat) < (uint8_t)values.size())
  {
    values[static_cast<uint8_t>(stat)].second = value;

    return true;
  }

  return false;
}
