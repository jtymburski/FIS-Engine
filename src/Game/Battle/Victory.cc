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

  uint32_t credits;
  std::vector<uint32_t> loot;

  for(auto& enemy : losers)
  {
    if(enemy && enemy->getBasePerson())
    {
      //      credits += enemy->getCreditDrop());
      //    loot += enemy->getBasePerson()->getLoot();
    }
  }

  //TODO
  credits = 0;
  loot_card.credit_drop = credits;
  loot_card.item_drops = loot;
  loot_card.location.point.x = config->getScreenWidth();
  // loot_card.location.point.y = (config->getScreenHeight());

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
    //auto small_font = config->getFontTTF(FontName::BATTLE_SMALL);
    auto x = card.location.point.x;
    auto y = card.location.point.y;
    auto width = card.location.width;
    auto height = card.location.height;

    // double tan60 = std::tan(60.0 * 3.14159265358 / 180.0);
    double tan45 = std::tan(45.0 * 3.14159265358 / 180.0);
    double tan30 = std::tan(30.0 * 3.14159265358 / 180.0);

    auto col1_x = static_cast<int32_t>(x + std::floor(0.13 * (float)width));
    auto col1_y = static_cast<int32_t>(y + std::floor(0.05 * (float)height));

    auto sprite_x = x - std::floor(0.02 * (float)height);
    auto sprite_y = 0;
    auto sprite_width = std::floor(0.62 * (float)width);
    auto sprite_height = std::floor(0.77 * (float)height);

    auto tile_size = std::floor(0.21 * (float)width);

    auto col2_x = static_cast<int32_t>(x + std::floor(0.40 * (float)width));
    // auto col2_y = y + std::floor(0.05 * (float)height);

    Text t_name(victory_font);
    Text t_level(victory_font);
    Text t_level_no(victory_font);
    Text t_rank(victory_font);
    Text t_exp(victory_font);
    Text t_next(victory_font);
    Text t_exp_val(victory_font);
    Text t_next_val(victory_font);

    /* Render the base frame */
    test_render->render(renderer, x, y, width, height);

    /* Render the Actor sprite */
    if(card.sprite_actor && card.card_actor)
    {
      auto base_person = card.card_actor->getBasePerson();

      if(base_person)
      {
        auto level = base_person->getLevel();
        auto rank_str = "RANK   " + Helpers::rankToStr(base_person->getRank());
        t_name.setText(renderer, base_person->getName(), color);
        t_level.setText(renderer, "Level", color);
        t_level_no.setText(renderer, std::to_string(base_person->getLevel()),
                           color);
        t_rank.setText(renderer, rank_str, color);
        t_next.setText(renderer, "NEXT", color);

        auto total_exp = base_person->getTotalExp();
        auto exp_left = card.exp_left;

        int32_t next_exp = 0;
        if(level != Person::kNUM_LEVELS)
          next_exp = base_person->getExpAt(level + 1) - total_exp;

        auto gain_exp_str = "GAIN " + std::to_string(exp_left);
        auto next_exp_str = "NEXT " + std::to_string(next_exp);
        // std::to_string(static_cast<int>(base_person->getExpMod() * 100))

        t_exp_val.setText(renderer, gain_exp_str, exp_color);
        t_next_val.setText(renderer, next_exp_str, exp_color);

        /*----------------------------------------------------------------------
         *  COLUMN ONE
         *--------------------------------------------------------------------*/
        t_name.render(renderer, col1_x, col1_y);

        sprite_y = col1_y + t_name.getHeight() - std::floor(0.01 * height);
        card.sprite_actor->render(renderer, sprite_x, sprite_y, sprite_width,
                                  sprite_height);

        auto t_rank_y = sprite_y + sprite_height + std::floor(0.01 * height);

        t_rank.render(renderer, col1_x - std::floor(0.03 * width), t_rank_y);

        /*----------------------------------------------------------------------
         *  COLUMN TWO (col2_x, col1_y)
         *--------------------------------------------------------------------*/
        auto hex_size = tile_size;
        auto half_hex = hex_size / 2;
        auto hex_width = 2 * tan30 * half_hex + tan45 * half_hex;
        auto hex_inset = 15;
        auto inner_hex_size = tile_size - hex_inset * 2;
        auto xp_pc = (float)base_person->findExpPercent() / 100.0;

        /*  OUTER HEXAGON
         *----------------------------------------------------*/
        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);

        Coordinate outer_hex = {static_cast<int32_t>(col2_x) + 1,
                                static_cast<int32_t>(col1_y) + 1};
        Frame::renderHexagon(outer_hex, hex_size, renderer);

        /* TOP TRAPEZOID
         *----------------------------------------------------*/
        SDL_SetRenderDrawColor(renderer, 204, 102, 0, 255);

        if(xp_pc > 0.50)
        {
          auto height = (2 * (xp_pc - 0.5) * half_hex);
          auto delta_x = tan30 * height;
          auto b1 = hex_width - 2 * delta_x;

          Coordinate d;
          d.x = col2_x + 1;
          d.y = col1_y + half_hex - height + 2;

          Frame::renderTrapezoid(d, height, b1, hex_width, renderer);
        }

        /* BOTTOM TRAPEZOID
         *----------------------------------------------------*/
        if(xp_pc > 0)
        {
          auto height = 2 * xp_pc * half_hex;

          if(height > half_hex)
            height = half_hex;

          double inset_x = tan30 * height;
          double delta_y = half_hex - height;
          double delta_x = tan30 * delta_y;

          // TODO: Magical 6
          double b1 = hex_size + 8 - 2 * delta_x;
          double b2 = b1 - 2 * inset_x + 1;

          Coordinate c;
          c.x = col2_x + 2 + std::round(delta_x);
          c.y = col1_y + half_hex + std::floor(delta_y);
          height = std::round(height);
          b1 = std::round(b1);
          b2 = std::round(b2);

          Frame::renderTrapezoid(c, height + 1, b1, b2, renderer);
        }

        /* HEXAGONAL BORDER
         *----------------------------------------------------*/
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        Frame::renderHexagonBorder(outer_hex, hex_size, renderer);
        Frame::renderHexagonBorder({outer_hex.x - 1, outer_hex.y}, hex_size,
                                   renderer);
        Frame::renderHexagonBorder({outer_hex.x, outer_hex.y + 1}, hex_size,
                                   renderer);
        Frame::renderHexagonBorder({outer_hex.x + 1, outer_hex.y}, hex_size,
                                   renderer);

        /* INNER HEXAGON
         *----------------------------------------------------*/
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);

        Coordinate top_left = {col2_x + 1 + hex_inset, col1_y + 1 + hex_inset};
        Frame::renderHexagon(top_left, inner_hex_size, renderer);

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        Frame::renderHexagonBorder(top_left, inner_hex_size, renderer);

        auto inner_hex_w =
            2 * tan30 * (inner_hex_size / 2) + tan45 * (inner_hex_size / 2);
        auto third_of_hex = (uint32_t)(std::round(inner_hex_size / 3.0));
        auto t_level_x = top_left.x + (inner_hex_w - t_level.getWidth()) / 2;
        auto t_level_y = top_left.y + third_of_hex - t_level.getHeight() / 2;

        t_level.render(renderer, t_level_x, t_level_y);

        auto t_level_no_x =
            top_left.x + (inner_hex_w / 2) - (t_level_no.getWidth() / 2);
        auto t_level_no_y =
            top_left.y + 2 * third_of_hex - t_level_no.getHeight() / 2;

        t_level_no.render(renderer, t_level_no_x, t_level_no_y);

        auto t_exp_val_y = col1_y + hex_size + std::floor(0.02 * height);
        auto t_next_val_y =
            t_exp_val_y + t_exp_val.getHeight() + std::floor(0.01 * height);

        /* Render Column 2 */
        t_exp_val.render(renderer, col2_x, t_exp_val_y);
        t_next_val.render(renderer, col2_x, t_next_val_y);
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

      card.location.point = Helpers::updateCoordinate(
          cycle_time, card.location.point, card.end_location.point, 2.0);

      if(card.location.point.x == card.end_location.point.x)
        victory_state = VictoryState::PROCESS_CARD;
    }
  }
  else if(victory_state == VictoryState::SLIDE_IN_LOOT)
  {
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
        /* Add 3% of Remaining EXP_LEFT, or 1, if possible */
        auto add_exp = (int32_t)(std::floor(0.01 * (float)card.exp_left));
        add_exp = std::max(1, add_exp);

        card.card_actor->getBasePerson()->addExp(add_exp, true);
        card.exp_left -= add_exp;

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
