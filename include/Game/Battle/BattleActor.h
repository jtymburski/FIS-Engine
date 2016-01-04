/*******************************************************************************
* Class Name: BattleActor
* Date Created: September 3, 2015
* Inheritance: None
*
* Description
* -----------
*   BattleActor is a container class created by Battle based on a Person. In
*   addition to the data of Person, the BattleActor will have all necessary
*   flags for Battle as well as ailments and rendering information.
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* [09-05-15]: Guardee/Guard BattleActor pointers
* [09-05-15]: Bubbified sprites, static objects [link into active sprite]
*****************************************************************************/
#ifndef BATTLEACTOR_H
#define BATTLEACTOR_H

/* Ailment <-- --> Battle Actor communication */
class Ailment;
class Person;
class BattleSkill;
class BattleItem;

#include "Game/Battle/RenderElement.h"
#include "Game/Battle/BattleSkill.h"
#include "Game/Battle/BattleItem.h"
#include "Game/Battle/BattleStats.h"
#include "Game/Player/Ailment.h"
#include "Game/Player/Inventory.h"
#include "Game/Player/Person.h"
#include "EnumFlags.h"

using std::begin;
using std::end;

/* BState Flags - Flags which describe only battle-related flags */
ENUM_FLAGS(ActorState)
enum class ActorState
{
  // clang-format off
  ITM_ENABLED       = 1 << 5, /* Can the person use item in battle? */
  DEF_ENABLED       = 1 << 6, /* Can the person use 'Defend'? */
  GRD_ENABLED       = 1 << 7, /* Can theisin person use 'Guard?' */
  IMP_ENABLED       = 1 << 8, /* Can the person use 'Implode'? */
  INS_ENABLED       = 1 << 9, /* Can the person use 'Inspect' ? */
  RUN_ENABLED       = 1 << 10, /* Can the person use 'Run'? */
  PAS_ENABLED       = 1 << 11, /* Can the person use 'Pass'? */
  REVIVABLE         = 1 << 12, /* Can this person be revived if they are KO? */
  ALLY              = 1 << 15, /* Is this \actor an ally */
  SELECTION_RANDOM  = 1 << 16,
  SELECTION_SKIP    = 1 << 17,
  MENU_HOVERED      = 1 << 18,
  MISS_NEXT_TARGET  = 1 << 19
  // clang-format on
};

enum class ActiveSprite
{
  FIRST_PERSON,
  THIRD_PERSON,
  ACTION,
  BUBBIFIED_FIRST_PERSON,
  BUBBIFIED_THIRD_PERSON,
  NONE
};

enum class FlashingType
{
  NONE,
  DAMAGE,
  POISON,
  INFLICT,
  KOING,
  RELIEVE
};

enum class LivingState
{
  ALIVE,
  KO,
  DEAD,
  REMOVED
};

enum class SelectionState
{
  NOT_SELECTED,
  SELECTING,
  SELECTED_ACTION,
  SELECTING_2ND_ACTION,
  SELECTED_2ND_ACTION,
  SELECTING_3RD_ACTION,
  SELECTED_3RD_ACTION,
  NONE
};

enum class SpriteState
{
  HIDDEN,
  SLIDING_IN,
  SLID_IN,
  SLIDING_OUT,
  SLID_OUT,
  ATTEMPT_RUNNING,
  PASS_BOBBING,
  CYCLING_FADE,
  RUNNING,
  BOBBING,
  FLASHING,
  KOING,
  DYING,
  KOED,
  DEAD
};

enum class GuardingState
{
  NONE, /* This person is not guarding nor being guarded or shielded */
  DEFENDING, /* This person is defending */
  GUARDING, /* This person is guarding another person presently */
  GUARDED, /* This person is being guarded by another person */
  GUARDED_DEFENDING /* This person is defending & being guarded */
};

enum class UpkeepState
{
  VITA_REGEN,
  QTDR_REGEN,
  AILMENT_BEGIN,
  AILMENT_CLEAR,
  AILMENT_FLASH,
  AILMENT_DAMAGE,
  AILMENT_OUTCOME,
  AILMENT_PLEP,
  COMPLETE
};

struct ActionElement
{
  ActionElement()
      : element_state{SpriteState::HIDDEN}, frame_action{nullptr} {};

  Coordinate position_curr;
  Coordinate position_end;
  Coordinate position_start;

  SpriteState element_state;
  Frame* frame_action;
};

struct FlashingState
{
  FlashingState()
      : flashing_type{FlashingType::NONE},
        is_flashing{false},
        element{nullptr} {};

  FlashingType flashing_type;
  bool is_flashing;

  RenderElement* element;
};

class BattleActor
{
public:
  /* Constructs the Battle object by a given base person */
  BattleActor(Person* person_base, int32_t battle_index, int32_t menu_index, bool is_ally,
              bool can_run, SDL_Renderer* renderer);

  /* Default BattleActors may not be created */
  BattleActor() = delete;

  /* Annihilates the BattleActor */
  ~BattleActor();

private:
  /* The actor's AI Module */
  AIModule* ai;

  /* The action element */
  ActionElement action_element;

  /* The enumerated active sprite for the person */
  ActiveSprite active_sprite;

  /* The ailments which are inflicting the actor */
  std::vector<Ailment*> ailments;

  /* The actual battle index for the BattleActor */
  int32_t battle_index;

  /* Vector of Battle items */
  std::vector<BattleItem*> battle_items;

  /* Vector of Battle skills */
  std::vector<BattleSkill*> battle_skills;

  /* Flags for the actor */
  ActorState flags;

  /* The info frame for the Battle Actor */
  Frame* frame_info;

  /* The menu index. [- for enemies, + for allies] */
  int32_t menu_index;

  /* The base person for the BattleActor */
  Person* person_base;

  /* Sprite pointers - See active_sprite enum */
  Sprite* sprite_first_person;
  Sprite* sprite_third_person;
  Sprite* sprite_action;
  Sprite* sprite_dialog;

  /* The enumerated SpriteState for the active sprite */
  SpriteState state_active_sprite;

  /* The state of the death fade */
  FadeState state_death_fade;

  /* The elapsed time for the current state */
  uint32_t state_elapsed_time;

  /* The actor's flashing state */
  FlashingState state_flashing;

  /* The guarding state for the BattleActor */
  GuardingState state_guarding;

  /* Whether the actor is alive, KO, etc. */
  LivingState state_living;

  /* The state of selection for the BattleActor */
  SelectionState state_selection;

  /* The state of the upkeep (if an upkeep is ongoing) */
  UpkeepState state_upkeep;

  /* The current combat stats (base_values + modifiers for the BatleActor) */
  BattleStats stats_actual;

  /* The actual stats being rendered (for VITA/QTDR increase/decreases) */
  BattleStats stats_rendered;

  /* Temporarily stored alpha */
  uint32_t temp_alpha;

  /* Coordinates for offsetting the dialog sprite */
  int32_t dialog_x;
  int32_t dialog_y;

  /* ------------ Constants --------------- */
  static const uint8_t kACTOR_KO_ALPHA;
  static const float kVELOCITY_X;
  static const SDL_Color kFLASHING_DAMAGE_COLOR;
  static const SDL_Color kFLASHING_POISON_COLOR;
  static const SDL_Color kFLASHING_INFLICT_COLOR;
  static const SDL_Color kFLASHING_KO_COLOR;
  static const SDL_Color kFLASHING_RELIEVE_COLOR;

  static const float kREGEN_RATE_ZERO_PC;
  static const float kREGEN_RATE_WEAK_PC;
  static const float kREGEN_RATE_NORMAL_PC;
  static const float kREGEN_RATE_STRONG_PC;
  static const float kREGEN_RATE_GRAND_PC;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Prepare the BattleActor for a Battle */
  void battleSetup(bool is_ally, bool can_run);

  /* Clearing methods */
  void clearActionFrame();
  void clearAilments();
  void clearBattleItems();
  void clearBattleSkills();
  void clearInfoFrame();

  /* CLears the flashing element on top of the sprite */
  void clearFlashing();

  /* Clears all the sprites */
  void clearSprites();

  /* Creates the sprites for the BattleActor based on the base person */
  void createSprites(SDL_Renderer* renderer);

  /* Reset the action related flags */
  bool resetActionTypes();

  /* Updates the action element [sliding in/out] */
  void updateActionElement(int32_t cycle_time);
  void updateSpriteBobbing(int32_t cycle_time);
  void updateSpriteFlashing(int32_t cycle_time);
  void updateSpriteKO(int32_t cycle_time);
  void updateStats(int32_t cycle_time);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Inflicts the BattleActor with a given ailment */
  void addAilment(Infliction type, int32_t min_turns, int32_t max_turns,
                  double chance);

  /* Constructs BattleItem objects of the BattleActor */
  bool buildBattleItems(Inventory* inv, std::vector<BattleActor*> all_targets);

  /* Constructs BattleSkill objects of the BattleActor */
  bool buildBattleSkills(std::vector<BattleActor*> all_targets);

  /* Calculates the turn regeneration for a given enumerated attr (vita/qd) */
  int32_t calcTurnRegen(Attribute attr, int32_t outnumbered_val = 0);

  /* Cleans up the BattleActor for a new turn */
  void cleanUpForTurn();

  /* Method to deal damage to this actor */
  bool dealDamage(int32_t damage_amount);
  void dealQtdr(int32_t dealt_amount);
  void restoreVita(int32_t restore_amount);
  void restoreQtdr(int32_t restore_amount);

  /* Ends the sprite's flashing */
  void endFlashing();

  /* Checks whether the BattleActor would be immune to a given Infliction */
  bool isImmune(Infliction test_infliction);

  /* Determines whether the Actor is inflicted with a given infliction */
  bool isInflicted(Infliction test_infliction);

  /* Returns the next ailment which needs to be updated */
  Ailment* nextUpdateAilment();

  /* Frees the ailment from memory */
  void clearAilment(Ailment* remove_ailment);

  /* Removes a given ailment from the vector of ailmnents, if it is found */
  bool removeAilment(Ailment* remove_ailment);

  /* Remove all the ailments on the BattleActor for a KO Scenario */
  void removeAilmentsKO();

  /* Assigns the actor's active sprite to start flashing */
  void startFlashing(FlashingType type, int32_t time_left = 0);

  /* Clear the state of the BattleActor for a new turn */
  void turnSetup();

  /* Updates the state of the BattleActor */
  bool update(int32_t cycle_time);

  /* Returns pointer to the constructed action frame */
  Frame* getActionFrame();
  int32_t getActionFrameX();
  int32_t getActionFrameY();

  /* Returns a pointer to the active sprite based on the ActiveSprite enum */
  Sprite* getActiveSprite();

  /* Returns a vector of valid action types for the actor */
  std::vector<ActionType> getValidActionTypes();

  /* Returns the setup AI Module */
  AIModule* getAI();

  /* Returns the ailments of the Battle Actor */
  std::vector<Ailment*> getAilments();

  /* Returns a pointer to the base person */
  Person* getBasePerson();

  /* Returns the vector of BattleItems */
  std::vector<BattleItem*> getBattleItems();

  /* Returns the vector of BattleSkills */
  std::vector<BattleSkill*> getBattleSkills();

  /* Returns the value of a given ActorState flag */
  bool getFlag(const ActorState& test_flag);

  /* Returns the modified index of the BattleActor */
  int32_t getIndex();

  int32_t getMenuIndex();

  /* Pointer to the info frame */
  Frame* getInfoFrame();

  /* Returns the sprite for the dialog (for action frames) */
  Sprite* getDialogSprite();

  /* Returns the active sprites' x */
  int32_t getDialogX();

  /* Returns the active sprites' y */
  int32_t getDialogY();

  /* Returns the guarding state of the Battle Actor */
  GuardingState getGuardingState();

  /* Gets the next selection state */
  SelectionState getSelectionState();

  /* Obtains the real cost value of the Skill */
  uint32_t getSkillCost(Skill* test_skill);

  /* Obtains the state of sprites -- for the active sprite & the action frame */
  SpriteState getStateActiveSprite();
  SpriteState getStateActionFrame();

  /* Returns an enumerated living state of the actor */
  LivingState getStateLiving();

  /* Returns the enumerated upkeep state */
  UpkeepState getStateUpkeep();

  /* Returns the stats actual object */
  BattleStats& getStats();

  /* Returns the stats_rendered object */
  BattleStats& getStatsRendered();

  /* Returns the % of vitality the actor has available (rounded) */
  uint32_t getPCVita();

  /* Returns the % of quantum drive the actor has available (rounded) */
  uint32_t getPCQtdr();

  /* Methods to return the floats for calculating turn regen rates */
  float getRegenFactor(RegenRate regen_rate);

  /* Sets the battle actor's action frame */
  void setActionFrame(Frame* action_frame);
  void setActionFrameEnd(int32_t new_x, int32_t new_y);
  void setActionFrameStart(int32_t new_x, int32_t new_y);

  /* Assigns a new enumerated SpriteState value to the action frame */
  void setStateActionFrame(SpriteState new_state);

  /* Assigns a new enumerated living state to the Battle Actor */
  void setStateLiving(LivingState new_state);

  /* Assigns a new active sprite for the BattleActor */
  void setActiveSprite(ActiveSprite new_active_sprite);

  /* Assigns a given ActorState flag to a given value */
  void setFlag(ActorState set_flags, const bool& set_value = true);

  /* Assigns the enumerated guarding state value */
  void setGuardingState(GuardingState state_guarding);

  /* Assigns a new info frame to the Battle Actor */
  void setInfoFrame(Frame* info_frame);

  /* Assigns a selection state */
  void setSelectionState(SelectionState state_selection);

  /* Set the upkeep state of the BattleActor */
  void setUpkeepState(UpkeepState state_upkeep);

  /*=============================================================================
   * PUBLIC STATIC FUNCTIONS
   *============================================================================*/
public:
  /* Returns the flashing color corresponding to the flashing type */
  static SDL_Color getFlashingColor(FlashingType flashing_type);

  /* Returns vector of allied targets based on input user/targets */
  static std::vector<BattleActor*>
  getAllyTargets(BattleActor* user, std::vector<BattleActor*> targets);

  /* Returns vector of enemy targets baesd on input user/targets */
  static std::vector<BattleActor*>
  getEnemyTargets(BattleActor* user, std::vector<BattleActor*> targets);

  /* Gets vector of living targets based on input targets */
  static std::vector<BattleActor*>
  getLivingTargets(std::vector<BattleActor*> targets);

  /* Gets vector with removed user, if found, of given input user/targets */
  static std::vector<BattleActor*>
  getRemovedUser(BattleActor* user, std::vector<BattleActor*> targets);

  /* Static method for grabbing valid targets from a given scope */
  static std::vector<BattleActor*>
  getTargetsFromScope(BattleActor* user, ActionScope scope,
                      std::vector<BattleActor*> targets);
};

#endif // BATTLEACTOR_H
