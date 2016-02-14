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
    card.exp_left = calcActorExp(actor);
    card.card_actor = actor;
    card.sprite_actor = card_sprite;

    card.frame_backdrop = new Frame(
        config->getBasePath() + "sprites/Overlay/victory_card.png", renderer);

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
    card.end_location.point.x = (config->getScreenWidth() - frame_x) / 2;
    card.end_location.point.y = card.location.point.y;

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

// for(auto& ally : getAllies())
// {
//   auto level_up_occured = false;

//   if(ally && ally->getBasePerson())
//   {
//     for(auto& ailment : ally->getAilments())
//       ally->removeAilment(ailment);

//     auto name = ally->getBasePerson()->getName();

//     auto old_level = ally->getBasePerson()->getLevel();
//     ally->getBasePerson()->addExp(total_exp_drop);
//     auto new_level = ally->getBasePerson()->getLevel();

//     if(new_level != old_level)
//       level_up_occured = true;

//     for(int i = (new_level - old_level); i > 0; i--)
//       std::cout << name << " has leveled up!" << std::endl;

//     if(!level_up_occured)
//     {
//       auto base = ally->getBasePerson();

//       if(base)
//       {
//         auto equip_stats = base->calcEquipStats();
//         auto max_health =
//             (uint32_t)base->getCurrMax().getStat(Attribute::VITA);

//         auto curr_health =
//         ally->getStats().getBaseValue(Attribute::VITA);
//         auto max_qtdr =
//             (uint32_t)base->getCurrMax().getStat(Attribute::QTDR);
//         auto curr_qtdr =
//         ally->getStats().getBaseValue(Attribute::QTDR);

//         if(curr_health <= max_health)
//           base->getCurr().setStat(Attribute::VITA, curr_health);
//         if(curr_qtdr <= max_qtdr)
//           base->getCurr().setStat(Attribute::QTDR, curr_qtdr);
//       }
//     }
//   }
// }

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
  if(config)
  {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Color exp_color = {200, 200, 200, 255};

    auto victory_font = config->getFontTTF(FontName::BATTLE_HEADER);
    auto small_font = config->getFontTTF(FontName::BATTLE_SMALL);
    auto x = card.location.point.x;
    auto y = card.location.point.y;
    auto width = card.location.width;
    auto height = card.location.height;

    auto col1_x = x + std::floor(0.13 * (float)width);
    auto col1_y = y + std::floor(0.05 * (float)height);

    auto sprite_x = x;
    auto sprite_y = 0;
    auto sprite_width = std::floor(0.55 * (float)width);
    auto sprite_height = std::floor(0.65 * (float)height);

    auto tile_size = std::floor(0.21 * (float)width);

    auto col2_x = x + std::floor(0.43 * (float)width);
    // auto col2_y = y + std::floor(0.05 * (float)height);

    Text t_name(victory_font);
    Text t_level(victory_font);
    Text t_rank(victory_font);
    Text t_exp(victory_font);
    Text t_next(victory_font);
    Text t_exp_val(small_font);
    Text t_next_val(small_font);

    /* Render the base frame */
    std::cout << " Frame height : " << height << std::endl;
    test_render->render(renderer, x, y, width, height);

    /* Render the Actor sprite */
    if(card.sprite_actor && card.card_actor)
    {
      auto base_person = card.card_actor->getBasePerson();

      if(base_person)
      {
        auto rank_str = "RANK :" + Helpers::rankToStr(base_person->getRank());
        // auto level_str = "Level " + std::to_string(base_person->getLevel());
        t_name.setText(renderer, base_person->getName(), color);
        // t_level.setText(renderer, level_str, color);
        t_rank.setText(renderer, rank_str, color);
        t_next.setText(renderer, "NEXT", color);

        auto total_exp = base_person->getTotalExp();

        auto next_exp =
            base_person->getExpAt(base_person->getLevel() + 1) - total_exp;

        auto total_exp_str = "EXP  " + std::to_string(total_exp);
        auto next_exp_str = "NEXT " + std::to_string(next_exp);
        // std::to_string(static_cast<int>(base_person->getExpMod() * 100))

        t_exp_val.setText(renderer, total_exp_str, exp_color);
        t_next_val.setText(renderer, next_exp_str, exp_color);

        /* Render Column 1 */
        auto t_level_y =
            col1_y + t_name.getHeight() + std::floor(0.01 * height);
        sprite_y = t_level_y + t_level.getHeight() + std::floor(0.01 * height);

        t_name.render(renderer, col1_x, col1_y);
        t_level.render(renderer, col1_x, t_level_y);

        card.sprite_actor->render(renderer, sprite_x, sprite_y, sprite_width,
                                  sprite_height);

        auto t_rank_y = sprite_y + sprite_height + std::floor(0.01 * height);

        t_rank.render(renderer, col1_x - std::floor(0.03 * width), t_rank_y);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        Frame::renderHexagon({static_cast<int32_t>(col2_x + 1), static_cast<int32_t>(col1_y + 1)}, tile_size, renderer);
        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);

        auto out_hex_x = col2_x  + 1;
        auto out_hex_y = col1_y + 1;
        auto out_hex_size = tile_size - 2;
        auto border = 2;

        Frame::renderHexagon({out_hex_x, out_hex_y}, out_hex_size, renderer);

        auto inset = std::round(0.04 * (float)(width));
        std::cout << "Inset: " << inset << std::endl;
        auto in_hex_x = 5 + out_hex_x + inset / 2;
        auto in_hex_y = out_hex_y + inset / 2;
        auto in_hex_size = out_hex_size - inset;
        Frame::renderHexagon({in_hex_x, in_hex_y}, in_hex_size, renderer);

        /* Render Column 2 */
        auto t_exp_y = col1_y + tile_size;
        t_exp_val.render(renderer, col2_x, t_exp_y);

        auto t_next_y =
            col1_y + t_exp_val.getHeight() + std::floor(0.01 * height);

        t_next_val.render(renderer, col2_x, t_next_y);

        /* Render Column 3 */
      }
    }
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
    else if(dim_time <= 0 && victory_state == VictoryState::SLIDE_OUT_CARD)
      victory_state = VictoryState::FINISHED;
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
  /* Update the actual state of victory */
  else if(victory_state == VictoryState::PROCESS_CARD)
  {
    if(index < victory_cards.size())
    {
      auto& card = victory_cards.at(index);

      if(card.exp_left > 0 && card.card_actor &&
         card.card_actor->getBasePerson())
      {
        card.card_actor->getBasePerson()->addExp(1, true);
        card.exp_left -= 1;

        if(card.exp_left == 0)
          dim_time += 2000;
      }
      else
      {
        victory_state = VictoryState::SLIDE_OUT_CARD;
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
