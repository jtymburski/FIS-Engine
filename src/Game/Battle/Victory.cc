/*******************************************************************************
* Class Name: Victory [Implementation]
* Date Created: January 16, 2016
* Inheritance: None
* Description:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/Battle/Victory.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
const double Victory::kEXP_FACTOR{0.60};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Victory::Victory()
    : config{nullptr},
      index{0},
      renderer{nullptr},
      victory_state{VictoryState::DIM_BATTLE}
{
}

Victory::Victory(Options* config, SDL_Renderer* renderer,
                 std::vector<BattleActor*> victors,
                 std::vector<BattleActor*> losers)
    : Victory()
{
  this->config = config;
  this->renderer = renderer;
  this->victors = victors;
  this->losers = losers;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

bool Victory::buildCard(BattleActor* actor)
{
  assert(actor);
  auto success = true;

  return success;
}

bool Victory::buildLoot()
{
  auto success = true;

  return success;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Victory::buildCards()
{
  auto success = true;

  for(const auto& actor : victors)
    success &= buildCard(actor);

  if(success)
    success &= buildLoot();

  return success;
}

/* Calculates the experience a given Battle Actor will receive */
uint32_t Victory::calcActorExp(BattleActor* actor)
{
  assert(actor);
  assert(actor->getBasePerson());

  uint32_t exp_drop = 0;

  for(auto enemy : losers)
  {
    if(enemy && enemy->getBasePerson())
    {
      auto xp = enemy->getBasePerson()->getExpDrop();
      auto lv = enemy->getBasePerson()->getLevel();
      auto mod = actor->getBasePerson()->getExpMod();

      exp_drop += (xp + ((lv - 1) * xp * kEXP_FACTOR)) * mod;
    }
  }

  return exp_drop;
}

VictoryState Victory::getStateVictory()
{
  return victory_state;
}

// TODO
void Victory::render()
{
  if(victory_state != VictoryState::FINISHED &&
     victory_state != VictoryState::DIM_BATTLE)
  {
  }
}

bool Victory::update(int32_t cycle_time)
{
  (void)cycle_time;

  return false;
}

bool Victory::setConfiguration(Options* new_config)
{
  config = new_config;

  return (config != nullptr);
}

bool Victory::setRenderer(SDL_Renderer* new_renderer)
{
  renderer = new_renderer;

  return (renderer != nullptr);
}

void Victory::setVictoryState(VictoryState new_victory_state)
{
  victory_state = new_victory_state;
}
