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

Sprite* Victory::buildActorSprite(std::string path)
{
  if(path != "" && renderer)
  {
    auto new_sprite = new Sprite(path, renderer);
    new_sprite->setNonUnique(true, 1);
    new_sprite->createTexture(renderer);

    return new_sprite;
  }

  return nullptr;
}

bool Victory::buildCard(BattleActor* actor)
{
  assert(actor && actor->getBasePerson());
  assert(renderer);

  auto success = true;
  auto base_person = actor->getBasePerson();
  auto card = VictoryCard();
  auto path = base_person->getThirdPersonPath();
  auto card_sprite = buildActorSprite(path);

  success &= (card_sprite != nullptr);

  if(success)
  {
    card.card_actor = actor;
    card.sprite_actor = card_sprite;

    card.frame_backdrop = new Frame(
        config->getBasePath() + "sprites/Overlay/victory_card90.png", renderer);

    auto frame_x = card.frame_backdrop->getWidth();
    auto frame_y = card.frame_backdrop->getHeight();

    frame_x = (float)frame_x * (float)config->getScreenWidth() /
              (float)Options::kDEF_SCREEN_WIDTH;
    frame_y = (float)frame_y * (float)config->getScreenHeight() /
              (float)Options::kDEF_SCREEN_HEIGHT;

    card.location.point.x = config->getScreenWidth();
    card.location.point.y = (config->getScreenHeight() - frame_y) / 2;
    card.location.width = frame_x;
    card.location.height = frame_y;
    card.state_backdrop = SpriteState::HIDDEN;

    // TODO: End locations for layered cards? */
    card.end_location.point.x =
        (config->getScreenWidth() - frame_x) / 2;
    card.end_location.point.y = card.location.point.y;

    /* Sprite Actor Location */
    card.sprite_actor_location.point.x = std::floor(0.05 * (float)frame_x);
    card.sprite_actor_location.point.y = std::floor(0.05 * (float)frame_y);
    card.sprite_actor_location.width = std::floor(0.5 * (float)frame_x);
    card.sprite_actor_location.height = std::floor(0.65 * (float)frame_y);

    victory_cards.push_back(card);
  }
  else
  {
    std::cout << "[Error] Creating card" << std::endl;
  }

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
  for(auto& card : victory_cards)
  {
    renderCard(card);
  }
}

void Victory::renderCard(VictoryCard& card)
{
  auto x = card.location.point.x;
  auto y = card.location.point.y;
  auto width = card.location.width;
  auto height = card.location.height;

  /* Render the base frame */
  test_render->render(renderer, x, y, width, height);

  /* Render the Actor sprite */
  if(card.sprite_actor)
  {
    card.sprite_actor->render(renderer, x + card.sprite_actor_location.point.x,
                              y + card.sprite_actor_location.point.y,
                              card.sprite_actor_location.width,
                              card.sprite_actor_location.height);
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
    if(index < victory_cards.size())
    {
      auto& card = victory_cards.at(index);

      if(card.location.point.x != card.end_location.point.x)
      {
        auto distance =
            std::abs(card.location.point.x - card.end_location.point.x);

        if(distance == 1)
          card.location.point.x = card.end_location.point.x;
        else
        {
          auto travel = std::min(cycle_time * 2, distance);

          if(travel > 1)
            card.location.point.x -= travel;
          else
            card.location.point.x -= 1;
        }
      }
      else
      {
        victory_state = VictoryState::PROCESS_CARD;
      }
    }
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
