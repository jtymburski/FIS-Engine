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
class RenderElement;

#include "Game/Battle/RenderElement.h"
#include "Game/Battle/BattleSkill.h"
#include "Game/Battle/BattleItem.h"
#include "Game/Battle/BattleStats.h"
#include "Game/Player/Ailment.h"
#include "Game/Player/Person.h"
#include "EnumFlags.h"

using std::begin;
using std::end;

/* BState Flags - Flags which describe only battle-related flags */
ENUM_FLAGS(ActorState)
enum class ActorState
{
  // clang-format off
  KO                = 1 << 1, /* Is the person KO'd ? */
  ALIVE             = 1 << 2, /* Is the person alive? (not imploded) */
  ITM_ENABLED       = 1 << 5, /* Can the person use item in battle? */
  DEF_ENABLED       = 1 << 6, /* Can the person use 'Defend'? */
  GRD_ENABLED       = 1 << 7, /* Can theisin person use 'Guard?' */
  IMP_ENABLED       = 1 << 8, /* Can the person use 'Implode'? */
  INS_ENABLED       = 1 << 9, /* Can the person use 'Inspect' ? */
  RUN_ENABLED       = 1 << 10, /* Can the person use 'Run'? */
  PAS_ENABLED       = 1 << 11, /* Can the person use 'Pass'? */
  REVIVABLE         = 1 << 12, /* Can this person be revived if they are KO? */
  ALLY              = 1 << 15, /* Is this actor an ally */
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
  POISON
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
  FLASHING
};

enum class GuardingState
{
  NONE,      /* This person is not guarding nor being guarded or shielded */
  DEFENDING, /* This person is defending */
  GUARDING,  /* This person is guarding another person presently */
  GUARDED,   /* This person is being guarded by another person */
  GUARDED_DEFENDING /* This person is defending & being guarded */
};

enum class UpkeepState
{
  UPKEEP_BEGIN,
  UPKEEP_VITA_REGEN,
  UPKEEP_QTDR_REGEN,
  UPKEEP_AILMENT_BEGIN,
  UPKEEP_AILMENT_FINISH,
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
  BattleActor(Person* person_base, int32_t battle_index, bool is_ally,
              bool can_run, SDL_Renderer* renderer);

  /* Default BattleActors may not be created */
  BattleActor() = delete;

  /* Annihilates the BattleActor */
  ~BattleActor();

private:
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
  static const float kVELOCITY_X;
  static const SDL_Color kFLASHING_DAMAGE_COLOR;
  static const SDL_Color kFLASHING_POISON_COLOR;

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

  /* Clears all the sprites */
  void clearSprites();

  /* Creates the sprites for the BattleActor based on the base person */
  void createSprites(SDL_Renderer* renderer);

  /* Reset the action related flags */
  bool resetActionTypes();

  /* Updates the action element [sliding in/out] */
  void updateActionElement(int32_t cycle_time);
  void updateSpriteFlashing(int32_t cycle_time);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Constructs BattleItem objects of the BattleActor */
  bool buildBattleItems(std::vector<BattleActor*> all_targets);

  /* Constructs BattleSkill objects of the BattleActor */
  bool buildBattleSkills(std::vector<BattleActor*> all_targets);

  /* Ends the sprite's flashing */
  void endFlashing();

  /* Checks whether the BattleActor would be immune to a given Infliction */
  bool isImmune(Infliction test_infliction);

  /* Determines whether the Actor is inflicted with a given infliction */
  bool isInflicted(Infliction test_infliction);

  /* Removes a given ailment from the vector of ailmnents, if it is found */
  bool removeAilment(Ailment* remove_ailment);

  /* Assigns the actor's active sprite to start flashing */
  void startFlashing(FlashingType type, int32_t time_left);

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

  /* Returns the stats actual object */
  BattleStats& getStats();

  /* Returns the stats_rendered object */
  BattleStats& getStatsRendered();

  /* Returns the % of vitality the actor has available (rounded) */
  uint32_t getPCVita();

  /* Returns the % of quantum drive the actor has available (rounded) */
  uint32_t getPCQtdr();

  /* Sets the battle actor's action frame */
  void setActionFrame(Frame* action_frame);
  void setActionFrameEnd(int32_t new_x, int32_t new_y);
  void setActionFrameStart(int32_t new_x, int32_t new_y);

  /* Assigns a new enumerated SpriteState value to the action frame */
  void setStateActionFrame(SpriteState new_state);

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
