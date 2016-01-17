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
#include "Game/Battle/RenderElement.h"

/* The victory card */
struct VictoryCard
{
  VictoryCard()
      : card_actor{nullptr},
        sprite_actor{nullptr},
        sprite_head{nullptr},
        sprite_body{nullptr},
        sprite_feet{nullptr},
        sprite_larm{nullptr},
        sprite_rarm{nullptr},
        frame_backdrop{nullptr}
        {};

  /* The actor the card is built for */
  BattleActor* card_actor;

  /* Card Sprites */
  Sprite* sprite_actor;
  Sprite* sprite_head;
  Sprite* sprite_body;
  Sprite* sprite_feet;
  Sprite* sprite_larm;
  Sprite* sprite_rarm;
  Frame* frame_backdrop;

  /* The rendered experience value (for the Bar) */
  uint32_t exp_rendered;
  uint32_t exp_left;

  /* Coordinates */
  Coordinate box;
};

/* The loot information */
struct LootCard
{
};

class Victory
{
public:
  /* Construct a default victory object */
  Victory();

  /* Constructs a victory object given a renderer/configuration setting */
  Victory(Options* config, SDL_Renderer* renderer,
          std::vector<BattleActor*> victors, std::vector<BattleActor*> losers);

  /* Annihilate a victory object */
  ~Victory() = default;


  /* ----- Constants ----- */
  static const double kEXP_FACTOR;

private:
  /* Active configuration */
  Options* config;

  /* The active card index */
  uint32_t index;

  /* Vector of Battle actors to calculate experience/loot with */
  std::vector<BattleActor*> losers;

  /* The active Victory screen renderer */
  SDL_Renderer* renderer;

  /* Vector of Actors to build the victory screen for */
  std::vector<BattleActor*> victors;

  /* The state of the VictoryScreen */
  VictoryState victory_state;

  /* The VictoryCards to render */
  std::vector<VictoryCard> victory_cards;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
  /* Constructs the VictoryCard for the given BattleActor */
  bool buildCard(BattleActor* actor);

  /* Constructs the loot card */
  bool buildLoot();

  /* Calculates the experience a given Battle Actor will receive */
  uint32_t calcActorExp(BattleActor* actor);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Builds the victory cards and the loot card */
  bool buildCards();

  /* Render the victory screen in its current state */
  void render();

  /* Update the Victory Screen */
  bool update(int32_t cycle_time);

  /* Return the enumerated victory state */
  VictoryState getStateVictory();

  /* Assigns a new configuration to the Victory Screen */
  bool setConfiguration(Options* new_config);

  /* Assigns a new renderer to the Victory Screen */
  bool setRenderer(SDL_Renderer* new_renderer);

  /* Assigns a victory state to the victory screen */
  void setVictoryState(VictoryState new_victory_state);
};

#endif // VICTORY_H