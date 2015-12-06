#include "Game/Battle/BattleSkill.h"
#include "Game/Battle/BattleActor.h"

BattleSkill::BattleSkill()
  : valid_status{ValidStatus::INVALID}
  , skill{nullptr}
  , true_cost{0}
  , targets{}
{

}

void BattleSkill::print()
{
  std::cout << "-- Battle Skill --" << std::endl;
  std::cout << "ValidStatus: " << Helpers::validStatusToStr(valid_status);

  if(skill)
    std::cout << "\nSkill: " << skill->getName();
  std::cout << "True Cost: " << true_cost << std::endl;

  std::cout << "Targets: ";

  for(auto& target : targets)
    if(target && target->getBasePerson())
      std::cout << target->getBasePerson()->getName() << ", ";

  std::cout << std::endl;
}
