/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
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
#ifndef BATTLE_EVENT_H
#define BATTLE_EVENT_H

#include "Game/Battle/BattleActor.h"

/* IgnoreState flags - flags for various action ignore flags */
ENUM_FLAGS(IgnoreState)
enum class IgnoreState
{

};

class BattleEvent
{
public:
  BattleEvent();

  BattleEvent(ActionType type, BattleActor* actor,
              std::vector<BattleActor*> targets);

  /* Index for processing actions */
  uint32_t action_index;

  /* The action type of this event */
  ActionType action_type;

  /* Actor describing the event */
  BattleActor* actor;

  /* Targets for the event */
  std::vector<BattleActor*> actor_targets;

  /* Skill of this event, if it's a skill event */
  Skill* event_skill;

  /* IgnoreState flags */
  IgnoreState flags_ignore;

  /* The item of this event, if it's an item event */
  Item* event_item;

  //  The state of processing for this event
  // ProcessingState processing_state;

  /* Stored attribute types for the Battle Event */
  Attribute attr_prio;
  Attribute attr_prid;
  Attribute attr_seco;
  Attribute attr_secd;
  Attribute attr_user;
  Attribute attr_targ;

  /* ------------ Constants --------------- */

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /*=============================================================================
   * PUBLIC STATIC FUNCTIONS
   *============================================================================*/
public:
};

#endif // BATTLE_EVENT_H
