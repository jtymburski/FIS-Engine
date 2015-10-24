#include "Game/Battle/BattleActor.h"
#include "Game/Battle/BattleItem.h"

BattleItem::BattleItem()
: valid_status{ValidStatus::INVALID}
, item{nullptr}
, amount{0}
, targets{}
{

}
