#include "Game/Battle/BattleActor.h"
#include "Game/Battle/BattleItem.h"

BattleItem::BattleItem()
    : valid_status{ValidStatus::INVALID}, item{nullptr}, amount{0}, targets{}
{
}

bool BattleItem::isDefensive()
{
  if(item && item->getUseSkill())
    return item->getUseSkill()->getFlag(SkillFlags::DEFENSIVE);

  return false;
}

bool BattleItem::isOffensive()
{
  if(item && item->getUseSkill())
    return item->getUseSkill()->getFlag(SkillFlags::OFFENSIVE);

  return false;
}

bool BattleItem::loadData(SDL_Renderer* renderer)
{
  if(renderer && item && item->getUseSkill() &&
     item->getUseSkill()->getAnimation())
  {
    item->getUseSkill()->getAnimation()->loadData(renderer);

    return true;
  }

  return false;
}

void BattleItem::print()
{
  std::cout << "-- Battle Skill --" << std::endl;
  std::cout << "ValidStatus: " << Helpers::validStatusToStr(valid_status);

  if(item)
    std::cout << "\nItem: " << item->getName() << std::endl;

  std::cout << "Items Available: " << amount << std::endl;

  std::cout << "Targets: ";

  for(auto& target : targets)
    if(target && target->getBasePerson())
      std::cout << target->getBasePerson()->getName() << ", ";

  std::cout << std::endl;
}
