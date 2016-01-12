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

enum class BattleEventType
{
  NONE,
  DAMAGE
};

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
  IGNORE_LUCK_DEF = 1 << 7,
};

enum class SkillHitStatus
{
  HIT,
  MISS,
  DREAMSNARED
};

enum class InflictionStatus
{
  INFLICTION,
  IMMUNE,
  ALREADY_INFLICTED,
  UPDATE,
  FIZZLE
};

/* ActionState - enumeration describing the processing state of the event */
enum class ActionState
{
  BEGIN,
  END_BOB,
  SLIDE_IN,
  FADE_IN_TEXT,
  SLIDE_OUT,
  SWITCH_SPRITE,
  SKILL_MISS,
  ACTION_START,
  ACTION_MISS,
  INFLICTION_MISS,
  PLEP,
  SPRITE_FLASH,
  INFLICT_FLASH,
  DAMAGE_VALUE,
  INFLICT,
  ACTION_END,
  OUTCOME,
  DONE
};

/* Actor Outcome Structure - Describes the outcome the greater BattleEvent
 * object will have during a specific action upon a specific target for
 * Battle to render and perform that outcome */
struct ActorOutcome
{
  ActorOutcome()
      : actor_outcome_state{ActionState::BEGIN},
        actor{nullptr},
        causes_ko{false},
        critical{false},
        damage{0},
        infliction_type{Infliction::INVALID},
        infliction_status{InflictionStatus::FIZZLE} {};

  /* The enumerated action outcome state this actor outcome's currently proc. */
  ActionState actor_outcome_state;

  /* Pointer to the Battle Actor for the outcome */
  BattleActor* actor;

  /* Whether this outcome caused the actor to become KOed */
  bool causes_ko;
  bool critical;

  /* The amount of damage that occured */
  int32_t damage;

  /* Type of infliction inflicted */
  Infliction infliction_type;

  /* The status of the infliction on this ActorOutcome [miss/immune/etc.] */
  InflictionStatus infliction_status;
};

class BattleEvent
{
public:
  /* Default BattleEvent */
  BattleEvent();

  /* Normal BattleEvent construction */
  BattleEvent(ActionType type, BattleActor* actor,
              std::vector<BattleActor*> targets);

  /* BattleEvent construction with just an action type */
  BattleEvent(ActionType type, BattleActor* actor);

  /* Index for processing actions */
  uint32_t action_index;

  /* The enumerated ActionState */
  ActionState action_state;

  /* The action type of this event */
  ActionType action_type;

  /* Actor describing the event */
  BattleActor* actor;

  /* Targets for the event */
  std::vector<BattleActor*> actor_targets;

  /* The amount of damage the event has currently calculated */
  std::vector<ActorOutcome> actor_outcomes;

  /* The item of this event, if it's an item event */
  Item* event_item;

  /* Skill of this event, if it's a skill event */
  BattleSkill* event_skill;

  /* The type of this event */
  BattleEventType event_type;

  /* IgnoreState flags */
  IgnoreState flags_ignore;

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
  static const float kDEFEND_MODIFIER;
  static const float kGUARD_MODIFIER;
  static const float kSHIELDED_MODIFIER;

  static const float kOFF_CRIT_FACTOR;
  static const float kBASE_CRIT_CHANCE;
  static const float kCRIT_MODIFIER;
  static const float kCRIT_LVL_MODIFIER;
  static const float kCRIT_DEFENDING_MODIFIER;
  static const float kCRIT_GUARDED_MODIFIER;

  static const float kDODGE_MODIFIER;
  static const float kDODGE_HIGHEST_RATE_PC;
  static const float kDODGE_PER_LEVEL_MODIFIER;

  static const uint32_t kMAXIMUM_DAMAGE;
  static const uint32_t kMINIMUM_DAMAGE;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Calculates the action variables for given skill */
  void calcActionVariables();

  /* Calculates the elemental modification vals for curr action */
  void calcElementalMods(BattleActor* curr_target);

  /* Calculates the experience the the current act. would receive if foes fell*/
  int32_t calcExperience();

  /* Calculates the ignore state for the given action against targets */
  void calcIgnoreState();

  /* Calculates the average diff. in level between a given actor and foes */
  int32_t calcLevelDifference();

  /* Calculation sub-functiosn for damage calculations */
  int32_t calcValPhysPow();
  int32_t calcValPhysDef(BattleStats target_stats);
  int32_t calcValPrimAtk(Skill* curr_skill);
  int32_t calcValPrimDef(Skill* curr_skill, BattleStats target_stats);
  int32_t calcValSecdAtk(Skill* curr_skill);
  int32_t calcValSecdDef(Skill* curr_skill, BattleStats target_stats);
  int32_t calcValLuckAtk();
  int32_t calcValLuckDef(BattleStats target_stats);

  /* Can a given BattleActor be inflicted with the */
  InflictionStatus canInflictTarget(BattleActor* curr_target, Infliction type);

  /* Methods for matching primary and secondary skills to their users */
  bool doesPrimMatch(Skill* skill);
  bool doesSecdMatch(Skill* skill);

  /* Obtains the stats of a given target */
  BattleStats getStatsOfTarget(BattleActor* curr_target);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Calculate alteration taking place */
  int32_t calcAltering(BattleActor* curr_target);

  /* Returns the calculated critical hit factor */
  float calcCritFactor(BattleActor* curr_target);

  /* Calculate the base damage against a given target */
  int32_t calcDamage(BattleActor* curr_target, float crit_factor);
  int32_t calcDamageImplode(BattleActor* curr_target);

  bool doesActionCrit(BattleActor* curr_target);
  bool doesActionHit(BattleActor* curr_target);
  SkillHitStatus doesSkillHit();

  /* Determines whether a given actor is among the targets */
  bool isActorAmongTargets(BattleActor* check_actor);

  /* Method for updating the stats for computing a new action outcome */
  void updateStats();

  /* Determines and returns the action name */
  std::string getActionName();

  /* Gets the current action (skills's or item skill's [action_index]) */
  Action* getCurrAction();

  /* Gets the current skill () */
  Skill* getCurrSkill();

  /* Evaluates a given IgnoreState flag */
  bool getFlagIgnore(IgnoreState test_flag);

  /* Assigns a given ignore state flag */
  void setFlagIgnore(IgnoreState flag, const bool& set_value = true);

  /* Updates the action index, returns true if one exists */
  bool setNextAction();
};

#endif // BATTLE_EVENT_H
