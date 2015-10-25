/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
* Date Redesigned: September 5, 2015
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
* [09-07-15]: Overlays and Midlays. Make class?
* ? - Are temp user attributes needed?
* ? - Bubbified skills
* ? - Curr event system [build an event struct] ?
*****************************************************************************/
#ifndef BATTLE_H
#define BATTLE_H

//#include "Game/Battle/BattleDisplayData.h"
//#include "Game/Battle/BattleEvent.h"
#include "Game/Battle/BattleActor.h"
#include "Game/Battle/Buffer.h"
#include "Game/Battle/BattleMenu.h"
#include "Game/Battle/RenderElement.h"
#include "Game/Player/Party.h"

using std::begin;
using std::end;

/* CombatState flags */
ENUM_FLAGS(CombatState)
enum class CombatState
{
  PHASE_DONE = 1 << 0
};

ENUM_FLAGS(RenderState)
enum class RenderState
{

};

ENUM_FLAGS(IgnoreState)
enum class IgnoreState
{

};


struct ActorUpkeep
{
  BattleActor* battle_actor;
  std::vector<Ailment*> upkeep_aiilments;
  UpkeepState upkeep_state;
};

class Battle
{
public:
  /* Constructs the Battle object */
  Battle();

  /* Annihilates a Battle object */
  ~Battle();

private:
  /* Vector of actors for the Battle */
  std::vector<BattleActor*> actors;

  /* The background sprite */
  Sprite* background;

  /* The battle bar offest value */
  uint32_t bar_offset;

  /* Pointer to the BattleDisplayData */
  BattleDisplayData* battle_display_data;

  /* The Battle Menu */
  BattleMenu* battle_menu;

  /* The action buffer */
  Buffer* battle_buffer;

  /* The assigned configuration for the Battle */
  Options* config;

  /* Delay for all processing */
  uint32_t delay;

  /* Flags related to the combat state */
  CombatState flags_combat;

  /* Flags related to the render state */
  RenderState flags_render;

  /* The enemy backdrop frame */
  Frame* frame_enemy_backdrop;

  /* Enumerated outcome type of the battle */
  OutcomeType outcome;

  /* Pointer to the current renderer */
  SDL_Renderer* renderer;

  /* Render element objects for the Battle */
  std::vector<RenderElement*> render_elements;

  /* The turn state of the Battle */
  TurnState turn_state;

  /* The time elapsed for the Battle */
  uint32_t time_elapsed;

  /* Elapsed turns of hte battle */
  uint16_t turns_elapsed;

/*=============================================================================
 * CONSTANTS - Battle Operations
 *============================================================================*/
  static const size_t kMAX_AILMENTS;
  static const size_t kMAX_EACH_AILMENTS;
  static const uint16_t kMAXIMUM_DAMAGE;
  static const uint16_t kMINIMUM_DAMAGE;

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

  static const float kOFF_CRIT_FACTOR;
  static const float kBASE_CRIT_CHANCE;
  static const float kCRIT_MODIFIER;
  static const float kCRIT_LVL_MODIFIER;
  static const float kCRIT_DEFENDING_MODIFIER;
  static const float kCRIT_GUARDED_MODIFIER;

  static const float kDODGE_MODIFIER;
  static const float kDODGE_HIGHEST_RATE_PC;
  static const float kDODGE_PER_LEVEL_MODIFIER;

  static const float kBASE_RUN_CHANCE;
  static const float kUSER_RUN_MODIFIER;
  static const float kALLY_RUN_MODIFIER;
  static const float kENEMY_RUN_MODIFIER;
  static const float kRUN_PC_PER_POINT;

  static const float kDEFEND_MODIFIER;
  static const float kGUARD_MODIFIER;
  static const float kSHIELDED_MODIFIER;

  static const int16_t kREGEN_RATE_ZERO_PC;
  static const int16_t kREGEN_RATE_WEAK_PC;
  static const int16_t kREGEN_RATE_NORMAL_PC;
  static const int16_t kREGEN_RATE_STRONG_PC;
  static const int16_t kREGEN_RATE_GRAND_PC;

  static const uint16_t kBIGBAR_CHOOSE;

  /* -------- Battle Outcome Constants (See Implementation) ----------- */
  static const int16_t kALLY_KO_EXP_PC;
  static const int16_t kENEMY_RUN_EXP_PC;
  static const int16_t kRUN_PC_EXP_PENALTY;

/*=============================================================================
 * CONSTANTS - Battle Display
 *============================================================================*/
  const static uint16_t kACTION_BORDER;   /* Border width on action slideout */
  const static uint16_t kACTION_CENTER;   /* Center point to center triangle */
  const static uint16_t kACTION_COLOR_A;  /* Alpha for inner portion */
  const static uint16_t kACTION_COLOR_G;  /* Grey color on middle border */
  const static uint16_t kACTION_COLOR_R;  /* Red color for middle text */
  const static uint16_t kACTION_CORNER_X; /* Corner offset on inner triangle */
  const static uint16_t kACTION_CORNER_Y; /* Corner offset on inner triangle */
  const static uint16_t kACTION_H;        /* Height of action frame */
  const static uint16_t kACTION_TEXT_SHADOW; /* Shadow offset of middle text */
  const static uint16_t kACTION_TEXT_X;      /* Right edge of middle text */
  const static uint16_t kACTION_W;           /* Width of action frame */
  const static uint16_t kACTION_Y; /* Y location of peak of triangle */

  const static uint8_t kAILMENT_BORDER; /* Ailment border width */
  const static uint8_t kAILMENT_GAP;    /* Ailment gap between edges of space */
  const static uint8_t kAILMENT_OPACITY; /* Ailment background opacity */

  const static uint8_t kALLY_HEALTH_H;        /* Ally health bar height */
  const static uint8_t kALLY_HEALTH_TRIANGLE; /* Ally health triangle width */
  const static uint8_t kALLY_HEALTH_W;        /* Ally health bar width */
  const static uint16_t kALLY_HEIGHT;         /* Ally display section height */
  const static uint8_t kALLY_QD_H;            /* Ally qd bar height */
  const static uint8_t kALLY_QD_OFFSET;   /* Ally qd bar offset off health */
  const static uint8_t kALLY_QD_TRIANGLE; /* Ally qd triangle width */
  const static uint8_t kALLY_QD_W;        /* Ally qd bar width */

  const static uint16_t kANIMATION_PROCESS; /* Time to process actions */
  const static uint16_t kBIGBAR_OFFSET;     /* Offset of bar off bottom */
  // const static uint16_t kBIGBAR_CHOOSE; /* Additional offset for choice */
  const static float kBIGBAR_L;  /* The percentage of the left section */
  const static float kBIGBAR_M1; /* The percentage of the middle section */
  const static float kBIGBAR_M2; /* The percentage of the second middle */

  const static float kBIGBAR_R; /* The percentage of the right section */
  const static uint16_t kBIGBAR_R_OFFSET; /* Offset off end for right section */

  const static uint16_t kBOB_AMOUNT; /* Amount of PX to 'bob the sprites */
  const static float kBOB_RATE;      /* Rate at which to bob the sprites */
  const static uint32_t kBOB_TIME;   /* Time to bob the sprites for */

  const static uint16_t kRUN_AMOUNT;
  const static float kRUN_RATE;
  const static uint32_t kRUN_TIME;

  const static uint8_t kCOLOR_BASE; /* Base of color for shifting bars */
  const static float kCYCLE_RATE;   /* Rate of cycling for selecting persons */

  const static uint16_t kDELAY_DAMAGE; /* Standard damage delay (ms) */
  const static uint16_t kDELAY_SKILL;  /* Standard skill delay (ms) */

  const static uint8_t kINFO_BORDER;   /* Border width on enemy info bar */
  const static uint8_t kINFO_GREY;     /* Grey value for border bar */
  const static uint16_t kINFO_H;       /* Height of enemy info bar */
  const static uint8_t kINFO_OPACITY;  /* Opacity of black background in info */
  const static uint8_t kINFO_TRIANGLE; /* Height of triangle in info corner */
  const static uint16_t kINFO_W;       /* Width of enemy info bar */

  const static uint8_t kENEMY_BAR_H;      /* Height of health bar for foes */
  const static uint8_t kENEMY_BAR_OFFSET; /* Offset of foe health off center */
  const static uint8_t kENEMY_BAR_TRIANGLE; /* Width of foe health triangle */
  const static uint16_t kENEMY_BAR_W;     /* Width of rect in foe health bar */
  const static uint16_t kENEMIES_BAR_GAP; /* Offset of bar above foes */
  const static uint16_t kENEMIES_OFFSET;  /* Offset of foes from top */
  const static uint16_t kALLIES_OFFSET;   /* Offset of friends from bottom */

  const static uint8_t kMAX_CHARS;  /* Max number of foes in battle */
  const static uint8_t kMAX_LAYERS; /* Max number of layers that can be set */

  const static uint16_t kPERSON_SPREAD;  /* Rendering overlay of persons */
  const static uint16_t kPERSON_WIDTH;   /* Width of persons on battle */
  const static uint8_t kPERSON_KO_ALPHA; /* Opacity of a person at death */

  /* ---- Color Constants ---- */
  const static SDL_Color kSTRD_DMG_COLOR;
  const static SDL_Color kCRIT_DMG_COLOR;
  const static SDL_Color kPOIS_DMG_COLOR;
  const static SDL_Color kHEAL_DMG_COLOR;
  const static SDL_Color kBURN_DMG_COLOR;
  const static SDL_Color kVITA_REGEN_COLOR;
  const static SDL_Color kQTDR_REGEN_COLOR;
  const static SDL_Color kHIBERNATION_REGEN_COLOR;

/*=============================================================================
 * PRIVATE FUNCTIONS - Battle Operations
 *============================================================================*/
private:
  /* Constructs AI Modules for the Battle */
  void aiBuild();

  /* Deletes the AI modules at the end of a Battle */
  void aiClear();

  /* Buffer the current selection of the menu to the action buffer */
  bool bufferMenuSelection();

  /* Buffer the module selection */
  bool bufferModuleSelection();

  /* Construct the Battle actors from the given parties */
  void buildBattleActors(Party* allies, Party* enemies);

  /* Calculates an action selection for a given BattleActor // AIModule */
  bool calculateEnemySelection(BattleActor* next_actor, AIModule* curr_module);

  /* Clear function for the battle actors */
  void clearBattleActors();

  /* Returns true if the given actor needs to select a menu option */
  bool doesActorNeedToSelect(BattleActor* actor);

  /* General upkeep phase */
  void generalUpkeep();

  /* Loads the menu for a given battle actor */
  bool loadMenuForActor(BattleActor* actor);

  /* Update the selection of the enemies */
  void updateEnemySelection();

  /* Update the selecting state enum for a given BattleActor */
  void updateSelectingState(BattleActor* actor, bool set_selected);

  /* Updates the selection of the user */
  void updateUserSelection();

  /* Returns the modified index of a given index value */
  int32_t getBattleIndex(int32_t index);

  /* Returns the current AI Module pointer being worked on */
  AIModule* getCurrentModule();

  /* Obtain pointer to currently selecting module actor */
  BattleActor* getCurrentModuleActor();

  /* Returns the AI Module of a given BattleActor */
  AIModule* getModuleOfActor(BattleActor* actor);

  /* Obtain pointer to next selecting module actor */
  BattleActor* getNextModuleActor();

  /* Returns a pointer to the next actor needing to select for the menu */
  BattleActor* getNextMenuActor();

  /* Sets the next turn state of the Battle */
  void setNextTurnState();

/*============================================================================
 * PRIVATE FUNCTIONS - Battle Display
 *============================================================================*/
private:
  /* Creates action frames for a given BattleActor */
  void buildActionFrame(BattleActor* actor);

  /* Creates the enemy backdrop frame */
  void buildEnemyBackdrop();

  /* Creates an ally info panel for a given BattleActor */
  void buildInfoAlly(BattleActor* ally);

  /* Creates an enemy info for a given BattleActor */
  void buildInfoEnemy(BattleActor* enemy);

  /* Clears the Battle background */
  void clearBackground();

  /* Clears the elements out */
  void clearElements();

  /* Renders the Battle bar on the screen */
  bool renderBattleBar();

  /* Renders the ailments on a given actor */
  bool renderAilmentsActor(BattleActor* actor, uint32_t x, uint32_t y, bool f);

  /* Ally rendering functions */
  bool renderAllies();
  bool renderAlliesInfo();
  bool renderAllyInfo(BattleActor* ally, bool for_menu = false);

  /* Enemy rendering functions */
  bool renderEnemies();
  bool renderEnemiesInfo();
  bool renderEnemyInfo(BattleActor* actor);

  /* Basic setup for health drawing */
  bool setupHealthDraw(BattleActor* actor, float health_pc);

  /* Rendering updates */
  void updateBarOffset();
  void updateRendering(int32_t cycle_time);
  void updateRenderElements(int32_t cycle_time);
  void updateRenderSprites(int32_t cycle_time);

  /* Returns an X-value for a given Person */
  int32_t getActorX(BattleActor* actor);

  /* Returns the Y-value for a given Person */
  int32_t getActorY(BattleActor* actor);

/*============================================================================
 * PUBLIC FUNCTIONS - Battle Operations
 *============================================================================*/
public:
  /* Processes a KeyDown event */
  bool keyDownEvent(SDL_KeyboardEvent event);

  /* Method to start a Battle */
  bool startBattle(Party* friends, Party* allies, Sprite* background = nullptr);

  /* Stops a running Battle */
  void stopBattle();

  /* Update methods */
  bool update(int32_t cycle_time);

  /* Returns all allied BattleActors in a vector of ptrs */
  std::vector<BattleActor*> getAllies();

  /* Returns all enemy BattleActors in a vector of ptrs */
  std::vector<BattleActor*> getEnemies();

  /* Evaluates a given CombatState flag */
  bool getFlagCombat(CombatState test_flag);

  /* Evaluates a given RenderState flag */
  bool getFlagRender(RenderState test_flag);

  /* Return the enumerated outcom type */
  OutcomeType getOutcomeType();

  /* Returns the turn state of the Battle */
  TurnState getTurnState();

  /* Assigns a configuration to the Battle */
  bool setConfig(Options* config);

  /* Assigns the DisplayData (various constructed frames) for display */
  bool setDisplayData(BattleDisplayData* battle_display_data);

  /* Sets a CombatState flag */
  void setFlagCombat(CombatState test_flag, const bool& set_value = true);

  /* Set a RenderState flag */
  void setFlagRender(RenderState test_flag, const bool& set_value = true);

/*============================================================================
 * PUBLIC FUNCTIONS - Battle Display
 *============================================================================*/
public:
  /* Renders the Battle */
  bool render();

  /* Assigns the battle display container object */
  bool setBattleDisplayData(BattleDisplayData* battle_display_data);

  /* Assigns scope frames */
  bool setFrameScope(ActionScope scope, std::string path,
                     SDL_Renderer* renderer);

  /* Assigns a new renderer to the Battle */
  bool setRenderer(SDL_Renderer* renderer);

  /* Sets the background sprite */
  bool setBackground(Sprite* background);
};

#endif // BATTLE_H
