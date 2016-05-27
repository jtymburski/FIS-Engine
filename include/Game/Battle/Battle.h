/*******************************************************************************
* Class Name: Battle
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
* ? - Bubbified skills
*
* KNOWN BUGS
* ----------
* - Confusion with silence skills case
* - Scroll bar
* - Black frames
* - RenderElement non-existent path seg faults
*******************************************************************************/
#ifndef BATTLE_H
#define BATTLE_H

//#include "Game/EventHandler.h"
#include "Game/Battle/Victory.h"
#include "Game/Battle/BattleEvent.h"
#include "Game/Battle/Buffer.h"
#include "Game/Battle/BattleMenu.h"
//#include "Game/Battle/RenderElement.h"
#include "Game/Player/Party.h"
#include "Game/Lay.h"

class RenderElement;

using std::begin;
using std::end;

/* CombatState flags */
ENUM_FLAGS(CombatState)
enum class CombatState
{
  PHASE_DONE = 1 << 0,
  EVENT_READY = 1 << 1,
  AILMENT_CLEARS = 1 << 2,
  CREATED_VICTORY_TEXT = 1 << 3
};

ENUM_FLAGS(RenderState)
enum class RenderState
{

};

/* Class for battle information */
class Battle
{
public:
  /* Constructs the Battle object */
  Battle();

  /* Annihilates a Battle object */
  ~Battle();

  /* ----- Public Constants ------ */
  const static float kDELAY_SLOW_FACTOR;
  const static float kDELAY_NORM_FACTOR;
  const static float kDELAY_FAST_FACTOR;

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

  /* The current battle event */
  BattleEvent* event;

  /* The event handler for the game */
  EventHandler* eh;

  /* Flags related to the combat state */
  CombatState flags_combat;

  /* Flags related to the render state */
  RenderState flags_render;

  /* The enemy backdrop frame */
  Frame* frame_enemy_backdrop;

  /* Vector of lays for the Battle */
  std::vector<Lay*> lays;

  /* Music reference ID */
  int music_id;

  /* Party pointers */
  Party* party_allies;
  Party* party_enemies;

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

  /* Curent actor being upkept */
  BattleActor* upkeep_actor;
  Ailment* upkeep_ailment;

  /* The victory screen */
  Victory* victory_screen;

  /*=============================================================================
   * CONSTANTS - Battle Operations
   *============================================================================*/
  static const size_t kMAX_AILMENTS;
  static const size_t kMAX_EACH_AILMENTS;
  static const uint16_t kBIGBAR_CHOOSE;

  /* -------- Battle Outcome Constants (See Implementation) ----------- */
  static const int16_t kALLY_KO_EXP_PC;

  /*=============================================================================
   * CONSTANTS - Battle Display
   *============================================================================*/

  const static uint16_t kACTION_BORDER; /* Border width on action slideout */

  const static uint16_t kACTION_COLOR_A;  /* Alpha for inner portion */
  const static uint16_t kACTION_COLOR_G;  /* Grey color on middle border */
  const static uint16_t kACTION_COLOR_R;  /* Red color for middle text */
  const static uint16_t kACTION_CORNER_X; /* Corner offset on inner triangle */
  const static uint16_t kACTION_CORNER_Y; /* Corner offset on inner triangle */
  const static uint16_t kACTION_H;        /* Height of action frame */

  const static uint16_t kACTION_W; /* Width of action frame */
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

  const static uint16_t kPERSON_SPREAD; /* Rendering overlay of persons */

public:
  const static uint16_t kPERSON_WIDTH; /* Width of persons on battle */

  /*=============================================================================
   * PRIVATE FUNCTIONS - Battle Operations
   *============================================================================*/
private:
  /* Organized Skill methods for ActionStates of the outer BattleEvent loop */
  void actionStateBegin();
  void actionStateSlideIn();
  void actionStateFadeInText();
  void actionStateSlideOut();
  void actionStateSwitchSprite();
  void actionStateSkillMiss();
  void actionStateActionStart();

  /* Adds a delay amount for processing relative to the current speed setting */
  void addDelay(int32_t delay_amount, bool for_outcomes = false);

  /* Buffer the current selection of the menu to the action buffer */
  bool bufferMenuSelection();

  /* Buffer the module selection */
  bool bufferModuleSelection();

  /* Construct the Battle actors from the given parties */
  void buildBattleActors(Party* allies, Party* enemies);

  /* Calculates an action selection for a given BattleActor // AIModule */
  bool calculateEnemySelection(BattleActor* next_actor, AIModule* curr_module);

  /* Checks if all of the allies are either KO or are dead */
  bool checkAlliesDeath();

  /* Checks if all of the enemies are either KO or are dead */
  bool checkEnemiesDeath();

  /* Checks the outcome state of the Battle */
  void checkIfOutcome();

  /* Cleans up data for the current turn */
  void cleanUpTurn();

  /* Clear function for the battle actors */
  void clearBattleActors();

  /* Clears the Battle event if built */
  void clearEvent();

  /* Creates a DamageElement RenderElement type on th given actor */
  void createDamageElement(
      BattleActor* actor, DamageType damage_type, uint32_t amount);

  /* Returns true if the given actor needs to select a menu option */
  bool doesActorNeedToSelect(BattleActor* actor);

  /* Returns whether an actor needs to be upkeeped or not */
  bool doesActorNeedToUpkeep(BattleActor* actor);

  /* Checks whether the current buffer element is valid for processing */
  bool isBufferElementValid();

  /* Loads the action buffer current element into a BattleEvent */
  void loadBattleEvent();

  /* Loads the menu for a given battle actor */
  bool loadMenuForActor(BattleActor* actor);

  /* Organizational methods for ActionStates of the inner BE outcome loop */
  void outcomeStateActionMiss(ActorOutcome& outcome);
  void outcomeStatePlep(ActorOutcome& outcome);
  void outcomeStateDamageValue(ActorOutcome& outcome);
  void outcomeStateSpriteFlash(ActorOutcome& outcome);
  void outcomeStateInflictFlash(ActorOutcome& outcome);
  void outcomeStateActionOutcome(ActorOutcome& outcome);

  /* Returns the outnumbered value */
  int32_t outnumberedVal();

  /* Prepare the actor upkeeps */
  void prepareActorUpkeeps();

  /* Process Event methods -> ex. DAMAGE actions for a Skill etc. */
  void processEvent();

  /* Processes an action against a target */
  void processEventAction(Action* curr_action, BattleActor* target);

  /* Processes an item on the buffer */
  void processEventItem();

  /* Processes a Skill on the buffer */
  void processEventSkill();

  /* Processes a given infliction onto the given BattleActor */
  void processInfliction(BattleActor* target, Infliction type);

  /* Update the begin step of the Battle */
  void updateBegin();

  /* Set the next elemenet in the buffer, check for phase done */
  void updateBufferNext();

  /* Updates the procesing delay for the Battle */
  void updateDelay(int32_t decrement_delay);

  /* Updates general upkeep phase */
  void updateGeneralUpkeep();

  /* Update to the screen dimming state of the Battle */
  void updateScreenDim();

  /* Updates the processing on the current battle buffer element */
  void updateEvent();

  /* Update the selection of the enemies */
  void updateEnemySelection();

  /* Update to the fade in text state of the Battle */
  void updateFadeInText();

  /* Updates the outcome state of the battle */
  void updateOutcome(int32_t cycle_time);

  /* Updates the personal upkeep step */
  void updatePersonalUpkeep();
  void updatePersonalVitaRegen();
  void updatePersonalQtdrRegen();
  void updatePersonalAilments();

  /* Updates the processing state of the Battle */
  void updateProcessing();

  /* Update the selecting state enum for a given BattleActor */
  void updateSelectingState(BattleActor* actor, bool set_selected);

  /* Updates the selection of the user */
  void updateUserSelection();

  /* Methods for upkeep ailment processing states */
  void upkeepAilmentClear();
  void upkeepAilmentDamage();
  void upkeepAilmentFlash();
  void upkeepAilmentOutcome();
  void upkeepAilmentPlep();

  /* Returns the modified index of a given index value */
  int32_t getBattleIndex(int32_t index);

  /* Returns the modified menu index for a given party index and whether ally */
  int32_t getMenuIndex(int32_t index, bool allies);

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

  /* Returns a pointer to the next actor needing to have an upkeep configured */
  BattleActor* getNextUpkeepActor();

  /* Sets the next turn state of the Battle */
  void setNextTurnState();

  /*============================================================================
   * PRIVATE FUNCTIONS - Battle Display
   *============================================================================*/
private:
  /* Upkeep functions for ailment damagin */
  void ailmentDamageFlash();
  void ailmentDamageAmount();

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
  void clearElementsTimedOut();

  /* Clears Enemy Backdrop */
  void clearEnemyBackdrop();

  void clearLays();

  /* Plays an infliction sound */
  void playInflictionSound(Infliction type);

  /* Renders the action frame if needed */
  bool renderActionFrame();

  /* Renders the Battle bar on the screen */
  bool renderBattleBar();

  /* Renders the ailments on a given actor */
  bool renderAilmentsActor(BattleActor* actor, uint32_t x, uint32_t y, bool f);

  /* Ally rendering functions */
  bool renderAllies();
  bool renderAlliesInfo();
  bool renderAllyInfo(BattleActor* ally, bool for_menu = false);

  bool renderElements();
  void renderElementPlep(RenderElement* element);
  void renderElementRGBOverlay(RenderElement* element);
  void renderElementText(RenderElement* element);

  /* Enemy rendering functions */
  bool renderEnemies();
  bool renderEnemiesInfo();

  /* Render midlays */
  void renderMidlays();

  /* Renders the Battle Menu */
  bool renderMenu();

  /* Render overlays */
  void renderOverlays();

  /* Render underlays */
  void renderUnderlays();

  /* Basic setup for health drawing */
  bool setupHealthDraw(BattleActor* actor, float health_pc);

  /* Rendering updates */
  void updateBarOffset();
  void updateRendering(int32_t cycle_time);
  void updateRenderSprites(int32_t cycle_time);

  /* Returns an X-value for a given Person */
  int32_t getActorX(BattleActor* actor);

  /* Returns the Y-value for a given Person */
  int32_t getActorY(BattleActor* actor);

  /*============================================================================
   * PUBLIC FUNCTIONS - Battle Operations
   *============================================================================*/
public:
  /* Creates a Lay of a given type */
  void createLay(std::string path, int32_t anim_time, Floatinate velocity,
      LayType lay_type);

  /* Processes a KeyDown event */
  bool keyDownEvent(KeyHandler& key_handler);

  /* Method to start a Battle */
  bool startBattle(Party* friends, Party* allies);

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

  /* Return the enumerated outcome type */
  OutcomeType getOutcomeType();

  /* Returns the turn state of the Battle */
  TurnState getTurnState();

  /* Assigns a configuration to the Battle */
  bool setConfig(Options* config);

  /* Assigns the DisplayData (various constructed frames) for display */
  bool setDisplayData(BattleDisplayData* battle_display_data);

  /* Assigns the event handler to the battle */
  bool setEventHandler(EventHandler* event_handler);

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
  bool setFrameScope(
      ActionScope scope, std::string path, SDL_Renderer* renderer);

  /* Assigns the music trigger ID */
  void setMusicID(int id);

  /* Assigns a new renderer to the Battle */
  bool setRenderer(SDL_Renderer* renderer);

  /* Sets the background sprite */
  bool setBackground(std::string bg_path);
};

#endif // BATTLE_H
