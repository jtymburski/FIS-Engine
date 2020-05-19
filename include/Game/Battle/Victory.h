/*******************************************************************************
* Class Name: Victory
* Date Created: January 16, 2016
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
*******************************************************************************/
#ifndef VICTORY_H
#define VICTORY_H

#include "Game/Battle/BattleActor.h"
#include "Game/Battle/BattleDisplayData.h"
#include "Game/Battle/RenderElement.h"
#include "Game/EventHandler.h"

struct VictoryActor
{
  VictoryActor()
      : actor{nullptr},
        actor_sprite{nullptr},
        base_person{nullptr},
        exp_left{0},
        orig_exp{0},
        orig_lvl{0} {};

  BattleActor* actor;
  Sprite* actor_sprite;
  Person* base_person;

  uint32_t exp_left;
  uint32_t orig_exp;
  uint32_t orig_lvl;
};

/* The victory card */
struct VictoryCard
{
  VictoryCard() : frame_backdrop{nullptr} {};

  /* The actors the card is built for */
  std::vector<VictoryActor> actors;

  /* Backdrop Frame */
  Frame* frame_backdrop;

  /* Backdrop Location and State */
  Box location;
  Box end_location;
  SpriteState state_backdrop;
};

/* The loot information */
struct LootCard
{
  /* Actual loot of the loot card ID, name */
  std::vector<std::pair<uint32_t, std::string>> item_drops;

  uint32_t credit_drop;
};

class Victory
{
public:
  /* Construct a default victory object */
  Victory();

  /* Constructs a victory object given a renderer/configuration setting */
  Victory(Options* config, BattleDisplayData* display_data,
          SDL_Renderer* renderer, std::vector<BattleActor*> victors,
          std::vector<BattleActor*> losers);

  /* Annihilate a victory object */
  ~Victory();

  /* ----- Constants ----- */
  static const double kEXP_FACTOR;

private:
  /* Active configuration */
  Options* config;

  /* Pointer to the display data */
  BattleDisplayData* display_data;

  /* Remaining time the Battle needs to dim */
  int32_t dim_time;

  /* Event Handler */
  EventHandler* event_handler;

  /* The active card index */
  uint32_t index;

  /* Experience frames for rendering */
  Frame* frame_exp_empty;
  Frame* frame_exp_full;
  Frame* frame_exp_middle;
  Frame* frame_exp_full_yellow;

  /* Vector of Battle actors to calculate experience/loot with */
  std::vector<BattleActor*> losers;

  /* The loot card */
  LootCard loot_card;

  /* The active Victory screen `er */
  SDL_Renderer* renderer;

  /* Vector of Actors to build the victory screen for */
  std::vector<BattleActor*> victors;

  /* The state of the VictoryScreen */
  VictoryState victory_state;

  /* The VictoryCards to render */
  VictoryCard card;

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  void clearCard();

  /* Constructs a sprite for the VictoryCard given a path */
  Sprite* buildActorSprite(std::string path);

  /* Constructs the VictoryCard for the given BattleActor */
  VictoryActor buildCard(BattleActor* actor);

  /* Constructs the loot card */
  bool buildLoot();

  /* Calculates the experience a given Battle Actor will receive */
  uint32_t calcActorExp(BattleActor* actor);

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Builds the victory cards and the loot card */
  bool buildVictory();

  /* Render the victory screen in its current state */
  void render();

  /* Renders a given card an all its information */
  void renderCard(VictoryCard& card);

  /* Render the main actor data with an experience bar */
  bool renderMainActorData(VictoryActor actor, Coordinate start,
                           uint32_t exp_bar_size);

  /* Render the blank actor data */
  bool renderBlankActorData(Coordinate start, uint32_t exp_bar_size);

  /* Render the loot information */
  bool renderLoot(Coordinate start);

  /* Update the Victory Screen */
  bool update(int32_t cycle_time);

  /* Return the enumerated victory state */
  VictoryState getStateVictory();

  /* Assigns a new configuration to the Victory Screen */
  bool setConfiguration(Options* new_config);

  /* Assigns a dim time from Battle */
  void setDimTime(int32_t new_dim_time);

  /* Assigns an event handler */
  void setEventHandler(EventHandler* event_handler);

  /* Assigns a new renderer to the Victory Screen */
  bool setRenderer(SDL_Renderer* new_renderer);

  /* Assigns a victory state to the victory screen */
  void setVictoryState(VictoryState new_victory_state);
};

#endif // VICTORY_H
