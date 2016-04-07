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
      display_data{nullptr},
      dim_time{0},
      index{0},
      renderer{nullptr},
      victory_state{VictoryState::DIM_BATTLE}
{
}

Victory::Victory(Options* config, BattleDisplayData* display_data, SDL_Renderer* renderer,
                 std::vector<BattleActor*> victors,
                 std::vector<BattleActor*> losers)
    : Victory()
{
  this->config = config;
  this->display_data = display_data;
  this->renderer = renderer;
  this->victors = victors;
  this->losers = losers;
}

Victory::~Victory()
{
  clearCard();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void Victory::clearCard()
{
  if(card.frame_backdrop)
    delete card.frame_backdrop;

  card.frame_backdrop = nullptr;
}

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

VictoryActor Victory::buildCard(BattleActor* actor)
{
  assert(actor && actor->getBasePerson());

  auto victory_actor = VictoryActor();

  victory_actor.actor = actor;
  victory_actor.base_person = actor->getBasePerson();
  victory_actor.exp_left = calcActorExp(actor);
  victory_actor.orig_exp = actor->getBasePerson()->findExpPercent();

  return victory_actor;
}

bool Victory::buildLoot()
{
  auto success = true;
  uint32_t credits;

  for(auto& enemy : losers)
  {
    if(enemy && enemy->getBasePerson() && display_data)
    {
      credits += enemy->getBasePerson()->getCreditDrop();
      auto enemy_loot = enemy->getBasePerson()->getItemDrops();

      for(auto& item : enemy_loot)
      {
        /* Create a pair of the item. Peek the Item name from BattleDIsplayData
         * by checking its ID against the known IDs. (On failure, will be "") */
        loot_card.item_drops.push_back(std::make_pair(item, display_data->getItemName(item)));
      }
    }
  }

  loot_card.credit_drop = credits;

  return success;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Victory::buildVictory()
{
  auto success = false;

  /* Clear the card if it was already constructed */
  clearCard();

  if(renderer)
  {
    success = true;

    card = VictoryCard();

    std::cout << "Setting ze cards sprite" << std::endl;
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

    card.end_location.point.x = (config->getScreenWidth() - frame_x) / 2;
    card.end_location.point.y = card.location.point.y;

    for(const auto& actor : victors)
    {
      if(actor && actor->getBasePerson())
        card.actors.push_back(buildCard(actor));
      else
        success = false;
    }

    if(success)
      success &= buildLoot();
  }

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
  renderCard(card);
}

void Victory::renderCard(VictoryCard& card)
{
  if(config)
  {
    auto x = card.location.point.x;
    auto y = card.location.point.y;
    auto width = card.location.width;
    auto height = card.location.height;

    /* Render the base frame */
    if(card.frame_backdrop)
      card.frame_backdrop->render(renderer, x, y, width, height);

    auto col1_x = static_cast<int32_t>(x + std::floor(0.13 * (float)width));
    auto col1_y = static_cast<int32_t>(y + std::floor(0.08 * (float)height));

    auto e_size = (int32_t)(std::round(0.18 * height));
    auto os_x = (int32_t)(std::round(0.15 * width));
    auto os_y = (int32_t)(std::round(0.1625 * height));

    x = col1_x;
    y = col1_y;

    /* Sleuth Member No. 1 */
    if(card.actors.size() > 0)
      renderMainActorData(card.actors.at(0), {x, y}, e_size);

    /* Sleuth Member No. 2 */
    if(card.actors.size() > 1)
      renderMainActorData(card.actors.at(1), {x + os_x, y + os_y}, e_size);
    else
      renderBlankActorData({x + os_x, y + os_y}, e_size);

    /* Sleuth Member No. 3 */
    if(card.actors.size() > 2)
      renderMainActorData(card.actors.at(2), {x, y + 2 * os_y}, e_size);
    else
      renderBlankActorData({x, y + 2 * os_y}, e_size);

    /* Sleuth Member No. 4 */
    if(card.actors.size() > 3)
      renderMainActorData(card.actors.at(3), {x + os_x, y + 3 * os_y}, e_size);
    else
      renderBlankActorData({x + os_x, y + 3 * os_y}, e_size);

    /* Sleuth Member No. 5 */
    if(card.actors.size() > 4)
      renderMainActorData(card.actors.at(4), {x, y + 4 * os_y}, e_size);
    else
      renderBlankActorData({x, y + 4 * os_y}, e_size);
  }
}

// render the loot
bool Victory::renderLoot(Coordinate start)
{
  (void)start;
  if(renderer)
  {
    auto title_font = config->getFontTTF(FontName::BATTLE_HEADER);
//    auto item_font = config->getFontTTF(FontName::BATTLE_SUBHEADER);
    SDL_Color color{255, 255, 255, 255};

    Text t_title(title_font);

    t_title.setText(renderer, "Loot", color);

    std::vector<Text> item_texts;

//    for(auto& item : loot_card.loot)
//    {
//      Text temp(item_font);
      // TODO - Obtain the item name
      // Item* item =
      // temp.setText(renderer,
//    }

    return true;
  }

  return false;
}

// main actor data with exp bar
bool Victory::renderMainActorData(VictoryActor actor, Coordinate start,
                                  uint32_t exp_bar_size)
{
  if(renderer && actor.actor && actor.actor->getBasePerson())
  {
    double cos60 = std::cos(60 * 3.14159265358 / 180.0);
    double sin60 = std::sin(60 * 3.14159265358 / 180.0);

    auto l = (int32_t)std::round(exp_bar_size / (1 + 2 * cos60));
    auto h = (int32_t)std::round(2 * l * sin60);

    auto base_person = actor.actor->getBasePerson();

    SDL_Color color{255, 255, 255, 255};
    auto name_font = config->getFontTTF(FontName::BATTLE_VICTORY_NAME);

    /* Render the Actor's Experience Bar */
    Frame::renderExpHex(start, exp_bar_size,
                        base_person->findExpPercent() / 100.0,
                        actor.orig_exp / 100.0, base_person->getLevel(),
                        actor.orig_lvl, renderer);

    /* Render the Name Data */
    Text name(name_font);

    name.setText(renderer, base_person->getName(), color);

    Coordinate name_location;
    name_location.x = start.x + exp_bar_size + exp_bar_size / 5;
    name_location.y = start.y + h / 2 - name.getHeight() / 2;

    name.render(renderer, name_location.x, name_location.y);
  }

  return false;
}

// blank actor data showing slot
bool Victory::renderBlankActorData(Coordinate start, uint32_t exp_bar_size)
{
  if(renderer)
    return Frame::renderExpHexBlank(start, exp_bar_size, renderer);

  return false;
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
    card.location.point = Helpers::updateCoordinate(
        cycle_time, card.location.point, card.end_location.point, 2.0);

    if(card.location.point.x == card.end_location.point.x)
      victory_state = VictoryState::PROCESS_CARD;
  }
  /* Update the actual state of victory */
  else if(victory_state == VictoryState::PROCESS_CARD)
  {
    bool done = true;

    for(auto& victory_actor : card.actors)
    {
      std::cout << "Experience Remaining: " << victory_actor.exp_left
                << "," << victory_actor.orig_exp << ","
                << victory_actor.orig_lvl << std::endl;

      if(victory_actor.exp_left > 0 && victory_actor.actor &&
         victory_actor.actor->getBasePerson())
      {
        auto add_exp =
            (int32_t)(std::floor(0.01 * (float)victory_actor.exp_left));

        add_exp = std::max(1, add_exp);
        victory_actor.exp_left -= add_exp;

        /* Add to person > TODO: MVR to review */
        victory_actor.base_person->addExp(add_exp);

        done &= (victory_actor.exp_left == 0);
      }
    }

    if(done)
      victory_state = VictoryState::FINISHED;
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
