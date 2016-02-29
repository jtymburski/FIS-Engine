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
    card.orig_exp = actor->getBasePerson()->findExpPercent();
    card.orig_lvl = actor->getBasePerson()->getLevel();
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

  // TODO
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

    auto base_person = card.card_actor->getBasePerson();

    auto victory_font = config->getFontTTF(FontName::BATTLE_HEADER);
    // auto small_font = config->getFontTTF(FontName::BATTLE_SMALL);
    auto x = card.location.point.x;
    auto y = card.location.point.y;
    auto width = card.location.width;
    auto height = card.location.height;

    // double tan60 = std::tan(60.0 * 3.14159265358 / 180.);
    double tan45 = std::tan(45.0 * 3.14159265358 / 180.0);
    double tan30 = std::tan(30.0 * 3.14159265358 / 180.0);

    /* Render the base frame */
    test_render->render(renderer, x, y, width, height);

    auto col1_x = static_cast<int32_t>(x + std::floor(0.13 * (float)width));
    auto col1_y = static_cast<int32_t>(y + std::floor(0.08 * (float)height));

    auto exp_bar_size = (int32_t)(std::round(0.18 * height));
    auto exp_bar_offset_x = (int32_t)(std::round(0.15 * width));
    auto exp_bar_offset_y = (int32_t)(std::round(0.1625 * height));

    x = col1_x;
    y = col1_y;

    /* Sleuth Member #1 Exp Hex */
    Frame::renderExpHex({x, y}, exp_bar_size,
                        (base_person->findExpPercent() / 100.0),
                        card.orig_exp / 100.0, base_person->getLevel(),
                        card.orig_lvl, renderer);

    /* Sleuth Member #2 Exp Hex */
    Frame::renderExpHex({x + exp_bar_offset_x, y + exp_bar_offset_y},
                        exp_bar_size, (base_person->findExpPercent() / 100.0),
                        card.orig_exp / 100.0, base_person->getLevel(),
                        card.orig_lvl, renderer);

    /* Sleuth Member #3 Exp Hex */
    Frame::renderExpHex({x, y + 2 * exp_bar_offset_y}, exp_bar_size,
                        (base_person->findExpPercent() / 100.0),
                        card.orig_exp / 100.0, base_person->getLevel(),
                        card.orig_lvl, renderer);

    /* Sleuth Member #4 Exp Hex */
    Frame::renderExpHex({x + exp_bar_offset_x, y + 3 * exp_bar_offset_y},
                        exp_bar_size, (base_person->findExpPercent() / 100.0),
                        card.orig_exp / 100.0, base_person->getLevel(),
                        card.orig_lvl, renderer);

    /* Sleuth Member #5 Exp Hex */
    Frame::renderExpHex({x, y + 4 * exp_bar_offset_y}, exp_bar_size,
                        (base_person->findExpPercent() / 100.0),
                        card.orig_exp / 100.0, base_person->getLevel(),
                        card.orig_lvl, renderer);
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
          card.exp_left += 100;

        // dim_time += 2000;
      }
      else
      {
        // victory_state = VictoryState::SLIDE_OUT_CARD;
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
