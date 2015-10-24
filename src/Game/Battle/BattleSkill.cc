#include "Game/Battle/BattleSkill.h"
#include "Game/Battle/BattleActor.h"

BattleSkill::BattleSkill()
  : valid_status{ValidStatus::INVALID}
  , skill{nullptr}
  , true_cost{0}
  , targets{}
{

}
