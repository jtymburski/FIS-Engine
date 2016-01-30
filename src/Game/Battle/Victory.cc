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
  auto success = true;
  auto base_person = actor->getBasePerson();

  /* Victory Card */
  auto card = VictoryCard();

  card.card_actor = actor;
  card.sprite_actor = base_person->getDialogSprite();
  card.frame_backdrop = new Frame(
      config->getBasePath() + "sprites/Overlay/victory_card.png", renderer);

  auto frame_x = card.frame_backdrop->getWidth();
  auto frame_y = card.frame_backdrop->getHeight();
  frame_x = frame_x * config->getScreenWidth() / Options::kDEF_SCREEN_WIDTH;
  frame_y = frame_y * config->getScreenHeight() / Options::kDEF_SCREEN_HEIGHT;

  card.location.point.x = config->getScreenWidth();
  card.location.point.y = (config->getScreenHeight() - frame_y) / 2;
  card.location.width = frame_x;
  card.location.height = frame_y;
  card.state_backdrop = SpriteState::HIDDEN;

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

  if(index < victory_cards.size())
  {
    auto card = victory_cards.at(index);

    test_render->render(renderer, card.location.point.x, card.location.point.y,
                        card.location.width, card.location.height);
  }
}

bool Victory::update(int32_t cycle_time)
{
  if(dim_time > 0)
  {
    dim_time -= (uint32_t)cycle_time;

    if(dim_time <= 0 && victory_state == VictoryState::DIM_BATTLE)
      victory_state = VictoryState::FADE_IN_HEADER;
    else if(dim_time <= 0 && victory_state == VictoryState::FADE_IN_HEADER)
    {
      victory_state = VictoryState::SLIDE_IN_CARD;
      index = 0;
    }
  }

  /* Slide the card(s) towards their locations */
  if(victory_state == VictoryState::SLIDE_IN_CARD)
  {
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
