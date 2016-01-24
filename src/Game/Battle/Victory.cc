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
      dim_time{0},
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


  test_render = new Sprite(
      config->getBasePath() + "sprites/Overlay/victory_card.png", renderer);
}

Victory::~Victory()
{
  if(test_render)
    delete test_render;

  test_render = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

bool Victory::buildCard(BattleActor* actor)
{
  assert(actor);

  auto base_person = actor->getBasePerson();

  /* Victory Card */
  auto card = VictoryCard();

  card.card_actor = actor;
  card.sprite_actor = base_person->getDialogSprite();



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
  // if(victory_state != VictoryState::FINISHED &&
  //    victory_state != VictoryState::DIM_BATTLE)
  // {
    auto frame_x = test_render->getFirstFrame()->getWidth();
    auto frame_y = test_render->getFirstFrame()->getHeight();

    auto x = (config->getScreenWidth() -
              (frame_x * (config->getScreenWidth() / 1216))) /
             2;
    auto y = (config->getScreenHeight() -
              (frame_y * (config->getScreenHeight() / 704))) /
             2;

    std::cout << "x: " << x << "y: " << y << std::endl;
    std::cout << "Frame x: " << frame_x << "frame y: " << frame_y << std::endl;

    test_render->render(renderer, x, y, frame_x, frame_y);
  // }
}

bool Victory::update(int32_t cycle_time)
{
  if(dim_time > 0)
  {
    dim_time -= (uint32_t)cycle_time;

    if(dim_time <= 0 && victory_state == VictoryState::DIM_BATTLE)
      victory_state = VictoryState::FADE_IN_HEADER;
    else if(dim_time <= 0 && victory_state == VictoryState::FADE_IN_HEADER)
      victory_state = VictoryState::SLIDE_IN_CARD;
  }

  return false;
}

bool Victory::setConfiguration(Options* new_config)
{
  config = new_config;

  return (config != nullptr);
}

void Victory::setDimTime(int32_t new_dim_time)
{
  std::cout << "Setting dim time to be : " << new_dim_time << std::endl;
  this->dim_time = new_dim_time;
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
