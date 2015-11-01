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
  IGNORE_PHYS_ATK = 1 << 0,
  IGNORE_PHYS_DEF = 1 << 1,
  IGNORE_PRIM_ATK = 1 << 2,
  IGNORE_PRIM_DEF = 1 << 3,
  IGNORE_SECD_ATK = 1 << 4,
  IGNORE_SECD_DEF = 1 << 5,
  IGNORE_LUCK_ATK = 1 << 6,
  IGNORE_LUCK_DEF = 1 << 7
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

  /* Temporary stats used for damage calculations, etc. */
  BattleStats temp_user_stats;
  std::vector<BattleStats> temp_targ_stats;

  /* ------------ Constants --------------- */
  static const float kOFF_PHYS_MODIFIER;
  static const float kDEF_PHYS_MODIFIER;
  static const float kOFF_PRIM_ELM_MATCH_MODIFIER;
  static const float kDEF_PRIM_ELM_MATCH_MODIFIER;
  static const float kOFF_SECD_ELM_MATCH_MODIFIER;
  static const float kDEF_SECD_ELM_MATCH_MODIFIER;

  static const float kOFF_PRIM_ELM_MODIFIER;
  static const float kDEF_PRIM_ELM_MODIFIER;
  static const float kOFF_SECD_ELM_MODIFIER;
  static const float kDEF_SECD_ELM_MODIFIER;

  static const float kPRIM_ELM_ADV_MODIFIER;
  static const float kPRIM_ELM_DIS_MODIFIER;
  static const float kSECD_ELM_ADV_MODIFIER;
  static const float kSECD_ELM_DIS_MODIFIER;

  static const float kDOUBLE_ELM_ADV_MODIFIER;
  static const float kDOUBLE_ELM_DIS_MODIFIER;

  static const float kMANNA_POW_MODIFIER;
  static const float kMANNA_DEF_MODIFIER;
  static const float kUSER_POW_MODIFIER;
  static const float kTARG_DEF_MODIFIER;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  void calcActionVariables();

  void calcElementalMods(BattleActor* curr_target);

  void calcIgnoreState();

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Method for updating the stats for computing a new action outcome */
  void updateStats();

  Action* getCurrAction();

  Skill* getCurrSkill();

  /* Evaluates a given IgnoreState flag */
  bool getFlagIgnore(IgnoreState test_flag);

  /* Assigns a given ignore state flag */
  void setFlagIgnore(IgnoreState flag, const bool& set_value = true);
};

#endif // BATTLE_EVENT_H
