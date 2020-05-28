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
      event_handler{nullptr},
      frame_exp_empty{nullptr},
      frame_exp_full{nullptr},
      frame_exp_full_yellow{nullptr},
      frame_exp_middle{nullptr},
      index{0},
      renderer{nullptr},
      victory_state{VictoryState::DIM_BATTLE}
{
}

Victory::Victory(Options* config, BattleDisplayData* display_data,
                 SDL_Renderer* renderer, std::vector<BattleActor*> victors,
                 std::vector<BattleActor*> losers)
    : Victory()
{
  this->config = config;
  this->display_data = display_data;
  this->renderer = renderer;
  this->victors = victors;
  this->losers = losers;

  if(config && renderer)
  {
    frame_exp_empty = new Frame(
        config->getBasePath() + "assets/images/ui/experience_empty.png", renderer);
    frame_exp_middle = new Frame(
        config->getBasePath() + "assets/images/ui/experience_middle.png", renderer);
    frame_exp_full = new Frame(
        config->getBasePath() + "assets/images/ui/experience_full.png", renderer);
    frame_exp_full_yellow = new Frame(
        config->getBasePath() + "assets/images/ui/experience_full_yellow.png", renderer);
  }
}

Victory::~Victory()
{
  clearCard();

  if(frame_exp_empty)
    delete frame_exp_empty;
  if(frame_exp_middle)
    delete frame_exp_middle;
  if(frame_exp_full)
    delete frame_exp_full;
  if(frame_exp_full_yellow)
    delete frame_exp_full_yellow;

  frame_exp_empty = nullptr;
  frame_exp_middle = nullptr;
  frame_exp_full = nullptr;
  frame_exp_full_yellow = nullptr;
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
  victory_actor.orig_lvl = actor->getBasePerson()->getLevel();

  return victory_actor;
}

bool Victory::buildLoot()
{
  auto success = true;
  uint32_t credits = 0;

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
        loot_card.item_drops.push_back(
            std::make_pair(item, display_data->getItemName(item)));
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
    card.frame_backdrop = new Frame(
        config->getBasePath() + "assets/images/ui/backgrounds/victory_card.png", renderer);

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

    auto e_size = (int32_t)(std::round(0.14 * height));
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

    // for(auto& item : loot_card.loot)
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
    assert(frame_exp_empty);
    assert(frame_exp_full);
    assert(frame_exp_middle);
    assert(frame_exp_full_yellow);

    double cos60 = std::cos(60 * 3.14159265358 / 180.0);
    double sin60 = std::sin(60 * 3.14159265358 / 180.0);

    auto l = (int32_t)std::round(exp_bar_size / (1 + 2 * cos60));
    auto h = (int32_t)std::round(2 * l * sin60);

    auto base_person = actor.actor->getBasePerson();

    SDL_Color color{255, 255, 255, 255};
    auto name_font = config->getFontTTF(FontName::BATTLE_VICTORY_NAME);

    // ==== START EXP RENDER (FACTOR OUT?) ==== //
    frame_exp_full->render(renderer, start.x, start.y);

    // auto original_height =
    //     frame_exp_full_yellow->getHeight() -
    //     (actor.orig_exp / 100.0 * frame_exp_full_yellow->getHeight());

    auto empty_height =
        frame_exp_empty->getHeight() -
        (base_person->findExpPercent() / 100.0 * frame_exp_empty->getHeight());

    auto gap = (int32_t)std::round(config->getScaledWidth() * 0.009);

    SDL_Rect exp_rect;
    exp_rect.x = 0;
    exp_rect.y = 0;
    exp_rect.h = empty_height;
    exp_rect.w = frame_exp_full->getWidth();

    frame_exp_empty->render(renderer, start.x, start.y, 0, empty_height,
                            &exp_rect);

    // exp_rect.h = original_height;

    // frame_exp_full_yellow->render(renderer, start.x, start.y, 0,
    //                               original_height, &exp_rect);

    frame_exp_middle->render(renderer,
                             start.x + frame_exp_empty->getWidth() / 2 -
                                 frame_exp_middle->getWidth() / 2,
                             start.y + frame_exp_empty->getHeight() / 2 -
                                 frame_exp_middle->getHeight() / 2);

    /* Render the level text centred to the frame middle exp */
    Text t_level_title{config->getFontTTF(FontName::M_ITEM_HEADER)};
    Text t_level{config->getFontTTF(FontName::M_TITLE_ELM)};

    t_level_title.setText(renderer, "LEVEL", {255, 255, 255, 255});
    t_level.setText(renderer, std::to_string(base_person->getLevel()),
                    {255, 255, 255, 255});

    auto x = start.x;
    auto y = start.y;
    start.x += frame_exp_empty->getWidth() / 2 - t_level_title.getWidth() / 2;
    start.y += frame_exp_empty->getHeight() * 0.24;
    t_level_title.render(renderer, start.x, start.y);

    start.x = x + frame_exp_empty->getWidth() / 2 - t_level.getWidth() / 2;
    start.y += 3 * gap / 2;
    t_level.render(renderer, start.x, start.y);

    // === END EXP RENDER === //

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
      if(victory_actor.exp_left > 0 && victory_actor.actor &&
         victory_actor.actor->getBasePerson())
      {
        auto add_exp =
            (int32_t)(std::floor(0.01 * (float)victory_actor.exp_left));

        add_exp = std::max(1, add_exp);

        // std::cout << "EXP Remain: " << victory_actor.exp_left << ","
        //           << victory_actor.orig_exp << "," << victory_actor.orig_lvl
        //           << "Actor can gain exp: "
        //           << victory_actor.actor->getBasePerson()->getPFlag(
        //                  PState::CAN_GAIN_EXP) << std::endl;

        victory_actor.exp_left -= add_exp;

        auto level_before = victory_actor.base_person->getLevel();
        auto skills = victory_actor.base_person->getCurrSkills();

        victory_actor.base_person->addExp(add_exp);

        /* Experience gain sound trigger */
        if(event_handler)
        {
          event_handler->triggerSound(Sound::kID_SOUND_VIC_EXPUP,
                                      SoundChannels::MENUS);
        }

        auto level_after = victory_actor.base_person->getLevel();

        if(level_after > level_before)
        {
          //TODO: Fire up level event and move to EventHandler
          std::cout << "Level up!" << std::endl;

          if(event_handler)
          {
            event_handler->triggerSound(Sound::kID_SOUND_VIC_LEVEL,
                                        SoundChannels::MENUS);
          }

          if(skills)
          {
            if(skills->getElements(level_after).size() >
               skills->getElements(level_before).size())
            {
              
              //TODO: Fire Skill gain event and move to EventHandler
              if(event_handler)
              {
                event_handler->triggerSound(Sound::kID_SOUND_VIC_SKILL,
                                            SoundChannels::MENUS);
              }

              std::cout << "Skill was gained!" << std::endl;
            }
          }

             }

        done &= (victory_actor.exp_left == 0);
      }
    }

    if(done)
    {
      for(auto& victory_actor : card.actors)
      {
        auto actor = victory_actor.actor;

        for(auto& ailment : actor->getAilments())
          actor->removeAilment(ailment);

        auto base = actor->getBasePerson();

        if(victory_actor.orig_lvl == base->getLevel())
        {
          // auto equip_stats = base->calcEquipStats();
          auto max_health =
              (uint32_t)base->getCurrMax().getStat(Attribute::VITA);
          auto curr_health = actor->getStats().getBaseValue(Attribute::VITA);
          auto max_qtdr = (uint32_t)base->getCurrMax().getStat(Attribute::QTDR);
          auto curr_qtdr = actor->getStats().getBaseValue(Attribute::QTDR);

          if(curr_health <= max_health)
            base->getCurr().setStat(Attribute::VITA, curr_health);
          if(curr_qtdr <= max_qtdr)
            base->getCurr().setStat(Attribute::QTDR, curr_qtdr);
        }
      }

      victory_state = VictoryState::FINISHED;
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

void Victory::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;
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
