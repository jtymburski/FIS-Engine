/*******************************************************************************
* Class Name: BattleItem
* Date Created:
* Inheritance: None
*
* Description
* -----------

*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*******************************************************************************/
#ifndef BATTLE_SKILL_H
#define BATTLE_SKILL_H

class BattleActor;

//#include "EnumDb.h"
#include "Game/Battle/BattleActor.h"
#include "Game/Player/Item.h"

/* Structure representing a skill and its possible targets for targ. selec. */
class BattleSkill
{
public:
  /* Construct a BattleSkill object */
  BattleSkill();

  /* The validity of this BattleSkill choice */
  ValidStatus valid_status;

  /* The pointer to the skill for this BattleSkill*/
  Skill* skill;

  /* The true cost of the BattleSkill */
  uint32_t true_cost;

  /* The valid targets for the Battle Skill */
  std::vector<BattleActor*> targets;

public:
  /* Is the BattleSkill defensive? */
  bool isDefensive();

  /* Is the BattleSkill offensive? */
  bool isOffensive();

  /* Load the animation data for the Skill */
  bool loadData(SDL_Renderer* renderer);

  /* Print the information of the BattleSkill */
  void print();
};

#endif
