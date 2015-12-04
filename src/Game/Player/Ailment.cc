/*******************************************************************************
* Class Name: Ailment
* Date Created: March 6th, 2013
* Inheritance: QWidget
* Description: The ailment class represents a Status Ailment or Debuff. Ailments
*              use the Infliction enumerated type to store their type as well
*              as the EnumString class to conver the enumerations to strings
*              instead of doing string compare (note that these are std::strings
*
* THE FOLLOWING HAS NOT BEEN UPDATED
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
*                  iv -- remember to fully utilize any Signals and Slots  which
*                        the class may use
*              5 - (Add) the Ailment's immunities based off Person flags
*              6 - (Add) any effects or checks in Battle or subclasses
*******************************************************************************/
#include "Game/Player/Ailment.h"
#include "Game/Battle/BattleStats.h"

/*============================================================================
 * CONSTANTS
 *============================================================================*/

/*------------------- Buff Constants ----------------------- */
const float Ailment::kPC_ALL_ATK_BUFF{1.070};
const float Ailment::kPC_ALL_DEF_BUFF{1.0700};
const float Ailment::kPC_PHYS_BUFF{1.100};
const float Ailment::kPC_ELEMENTAL_BUFF{1.150};
const float Ailment::kPC_LIMB_BUFF{1.100};
const float Ailment::kPC_UNBR_BUFF{1.20};
const float Ailment::kPC_VITA_BUFF{1.20};
const float Ailment::kPC_QTDR_BUFF{1.200};

/* % damage for Turn # 1 for Poison */
/* % change / turn for Poison */
const float Ailment::kPOISON_DMG_INIT{0.14};
const float Ailment::kPOISON_DMG_INCR{-0.03};

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Ailment::Ailment(Infliction type, BattleStats* stats_victim)
    : stats_victim{stats_victim},
      ailment_class{AilmentClass::UNSET},
      cure_chance{0},
      damage_amount{0},
      flag_set{static_cast<AilState>(0)},
      min_turns_left{0},
      max_turns_left{0},
      total_turns{0},
      type{type},
      update_status{AilmentStatus::INCOMPLETE}
{
  ailment_class = getClassOfInfliction(type);
}

Ailment::Ailment(Infliction type, BattleStats* stats_victim, uint32_t min_turns,
                 uint32_t max_turns, double chance)
    : Ailment(type, stats_victim)
{
  cure_chance = chance;
  min_turns_left = min_turns;
  max_turns_left = max_turns;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/* Calculates poison damage, true if it can occur */
bool Ailment::calcPoisonDamage()
{
  if(stats_victim)
  {
    auto max_health = stats_victim->getValue(Attribute::MVIT);
    auto change = kPOISON_DMG_INIT + ((float)total_turns * kPOISON_DMG_INCR);

    if(std::round(max_health * change) >= 0)
      damage_amount = std::round(max_health * change);

    return true;
  }

  return false;
}

/* Does the ailment cure? */
bool Ailment::doesAilmentCure()
{
  bool to_cure = false;

  if(getFlag(AilState::CURABLE_TIME))
  {
    if(max_turns_left == 0)
    {
      to_cure = true;
    }
    if(min_turns_left == 0)
    {
      if(cure_chance > 1)
        to_cure = true;
      else
        to_cure = Helpers::chanceHappens(cure_chance * 1000, 1000);
    }
  }
  // else if(getFlag(AilState::CURABLE_KO))
  // {
  //   if(actor_victim && actor_victim->getStateLiving() == LivingState::KO)
  //     to_cure = true;
  // }

  // else if(getFlag(AilState::CURABLE_DEATH))
  // {
  //   if(actor_victim && !actor_victim->getFlag(ActorState::ALIVE))
  //     to_cure = true;
  // }

  return to_cure;
}

/* Updates effects of the Ailment, ex. METATETHER -> death */
AilmentStatus Ailment::updateEffect()
{
  if(type == Infliction::POISON)
  {
    if(calcPoisonDamage())
      return AilmentStatus::TO_DAMAGE;
  }

  return AilmentStatus::COMPLETED;
}

/* Update the turn counts for the Ailment */
void Ailment::updateTurnCount()
{
  if(min_turns_left > 0)
    --min_turns_left;
  if(max_turns_left > 0)
    --max_turns_left;
  if(total_turns < std::numeric_limits<uint32_t>::max())
    ++total_turns;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/* Applies a Buff to the BattleStats of the inflicted BattleActor */
bool Ailment::applyBuffs()
{
  float value = 0.0;
  std::vector<Attribute> stats_to_buff;

  if(type == Infliction::ALLATKBUFF)
  {
    value = kPC_ALL_ATK_BUFF;
    stats_to_buff = AttributeSet::getAllOffensive();
  }
  else if(type == Infliction::ALLDEFBUFF)
  {
    value = kPC_ALL_DEF_BUFF;
    stats_to_buff = AttributeSet::getAllDefensive();
  }
  else if(type == Infliction::PHYBUFF)
  {
    value = kPC_PHYS_BUFF;
    stats_to_buff.push_back(Attribute::PHAG);
    stats_to_buff.push_back(Attribute::PHFD);
  }
  else if(type == Infliction::THRBUFF)
  {
    value = kPC_ELEMENTAL_BUFF;
    stats_to_buff.push_back(Attribute::THAG);
    stats_to_buff.push_back(Attribute::THFD);
  }
  else if(type == Infliction::POLBUFF)
  {
    value = kPC_ELEMENTAL_BUFF;
    stats_to_buff.push_back(Attribute::POAG);
    stats_to_buff.push_back(Attribute::POFD);
  }
  else if(type == Infliction::PRIBUFF)
  {
    value = kPC_ELEMENTAL_BUFF;
    stats_to_buff.push_back(Attribute::PRAG);
    stats_to_buff.push_back(Attribute::PRFD);
  }
  else if(type == Infliction::CHGBUFF)
  {
    value = kPC_ELEMENTAL_BUFF;
    stats_to_buff.push_back(Attribute::CHAG);
    stats_to_buff.push_back(Attribute::CHFD);
  }
  else if(type == Infliction::CYBBUFF)
  {
    value = kPC_ELEMENTAL_BUFF;
    stats_to_buff.push_back(Attribute::CYAG);
    stats_to_buff.push_back(Attribute::CYFD);
  }
  else if(type == Infliction::NIHBUFF)
  {
    value = kPC_ELEMENTAL_BUFF;
    stats_to_buff.push_back(Attribute::NIAG);
    stats_to_buff.push_back(Attribute::NIFD);
  }
  else if(type == Infliction::LIMBUFF)
  {
    value = kPC_LIMB_BUFF;
    stats_to_buff.push_back(Attribute::LIMB);
  }
  else if(type == Infliction::UNBBUFF)
  {
    value = kPC_UNBR_BUFF;
    stats_to_buff.push_back(Attribute::UNBR);
  }
  else if(type == Infliction::VITBUFF)
  {
    value = kPC_VITA_BUFF;
    stats_to_buff.push_back(Attribute::MVIT);
  }
  else if(type == Infliction::QDBUFF)
  {
    value = kPC_QTDR_BUFF;
    stats_to_buff.push_back(Attribute::MQTD);
  }

  if(stats_victim)
  {
    for(const auto& attribute : stats_to_buff)
    {

      stats_victim->addModifier(attribute, ModifierType::MULTIPLICATIVE, value,
                                false, 0, this);
    }

    return true;
  }

  return false;
}

void Ailment::update()
{
  updateTurnCount();

  if(doesAilmentCure())
    update_status = AilmentStatus::TO_REMOVE;
  else
    update_status = updateEffect();
}

int32_t Ailment::getDamageAmount()
{
  return damage_amount;
}

bool Ailment::getFlag(AilState test_flag)
{
  return static_cast<bool>((flag_set & test_flag) == test_flag);
}

Infliction Ailment::getType()
{
  return type;
}

AilmentStatus Ailment::getUpdateStatus()
{
  return update_status;
}

void Ailment::setFlag(const AilState& flags, const bool& set_value)
{
  (set_value) ? (flag_set |= flags) : (flag_set &= ~flags);
}

void Ailment::setUpdateStatus(AilmentStatus new_status)
{
  update_status = new_status;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

AilmentClass Ailment::getClassOfInfliction(Infliction type)
{
  if(type == Infliction::POISON)
    return AilmentClass::DAMAGING;
  if(type == Infliction::BURN)
    return AilmentClass::DAMAGING;
  if(type == Infliction::SCALD)
    return AilmentClass::DAMAGING;
  if(type == Infliction::BURN)
    return AilmentClass::DAMAGING;
  if(type == Infliction::CHARR)
    return AilmentClass::DAMAGING;
  if(type == Infliction::BERSERK)
    return AilmentClass::PASSIVE;
  if(type == Infliction::CONFUSE)
    return AilmentClass::PASSIVE;
  if(type == Infliction::SILENCE)
    return AilmentClass::PASSIVE;
  if(type == Infliction::BUBBIFY)
    return AilmentClass::PASSIVE;
  if(type == Infliction::DEATHTIMER)
    return AilmentClass::DAMAGING;
  if(type == Infliction::PARALYSIS)
    return AilmentClass::PASSIVE;
  if(type == Infliction::BLINDNESS)
    return AilmentClass::PASSIVE;
  if(type == Infliction::DREADSTRUCK)
    return AilmentClass::PASSIVE;
  if(type == Infliction::DREAMSNARE)
    return AilmentClass::PASSIVE;
  if(type == Infliction::HELLBOUND)
    return AilmentClass::PASSIVE;
  if(type == Infliction::BOND)
    return AilmentClass::PASSIVE;
  if(type == Infliction::BONDED)
    return AilmentClass::PASSIVE;
  if(type == Infliction::ROOTBOUND)
    return AilmentClass::HEALING;
  if(type == Infliction::DOUBLECAST)
    return AilmentClass::PASSIVE;
  if(type == Infliction::TRIPLECAST)
    return AilmentClass::PASSIVE;
  if(type == Infliction::HALFCOST)
    return AilmentClass::PASSIVE;
  if(type == Infliction::REFLECT)
    return AilmentClass::PASSIVE;
  if(type == Infliction::HIBERNATION)
    return AilmentClass::HEALING;
  if(type == Infliction::CURSE)
    return AilmentClass::PASSIVE;
  if(type == Infliction::METATETHER)
    return AilmentClass::DAMAGING;
  if(type == Infliction::MODULATE)
    return AilmentClass::PASSIVE;

  /* All non-listed Ailments are BUFFS */
  return AilmentClass::BUFF;
}
