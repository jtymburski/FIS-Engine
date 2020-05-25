/*******************************************************************************
* Class Name: ClassName [Implementation]
* Date Created: June 22, 2014
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/Battle/BattleDisplayData.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
BattleDisplayData::BattleDisplayData()
    : data_built{false},
      config{nullptr},
      frame_battle_bar{nullptr},
      frame_percent{nullptr},
      frame_qd{nullptr},
      frame_time{nullptr},
      renderer{nullptr}
{
}

/*
 * Description:
 */
BattleDisplayData::~BattleDisplayData()
{
  for(auto& ailment : frames_ailments)
  {
    if(ailment.second)
      delete ailment.second;

    ailment.second = nullptr;
  }

  for(auto& attribute : frames_attributes)
  {
    if(attribute.second)
      delete attribute.second;

    attribute.second = nullptr;
  }

  for(auto& element : frames_elements)
  {
    if(element.second)
      delete element.second;

    element.second = nullptr;
  }

  for(auto& scope : frames_scopes)
  {
    if(scope.second)
      delete scope.second;

    scope.second = nullptr;
  }

  for(auto& ailment_plep : pleps_ailments)
    if(ailment_plep.second)
      delete ailment_plep.second;
  pleps_ailments.clear();

  if(frame_battle_bar)
    delete frame_battle_bar;
  frame_battle_bar = nullptr;

  if(frame_percent)
    delete frame_percent;
  frame_percent = nullptr;

  if(frame_qd)
    delete frame_qd;
  frame_qd = nullptr;

  if(frame_time)
    delete frame_time;
  frame_time = nullptr;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

void BattleDisplayData::buildBattleBar(std::string asset_path)
{
  frame_battle_bar =
      new Frame(asset_path + "assets/images/ui/backgrounds/battle.png", renderer);
}

void BattleDisplayData::buildFramesAilments(std::string asset_path)
{
  /* Set the ailments */
  setFrameAilment(Infliction::POISON,
                  asset_path +
                      "sprites/Battle/Ailments/Poison_AA_A00.png");
  setFrameAilment(Infliction::BURN,
                  asset_path +
                      "sprites/Battle/Ailments/Burn01_AA_A00.png");
  setFrameAilment(Infliction::SCALD,
                  asset_path +
                      "sprites/Battle/Ailments/Burn02_AA_A00.png");
  setFrameAilment(Infliction::CHARR,
                  asset_path +
                      "sprites/Battle/Ailments/Burn03_AA_A00.png");
  setFrameAilment(Infliction::BERSERK,
                  asset_path +
                      "sprites/Battle/Ailments/Berserk_AA_A00.png");
  setFrameAilment(Infliction::CONFUSE,
                  asset_path +
                      "sprites/Battle/Ailments/Confused_AA_A00.png");
  setFrameAilment(Infliction::SILENCE,
                  asset_path +
                      "sprites/Battle/Ailments/Silence_AA_A00.png");
  setFrameAilment(Infliction::BUBBIFY,
                  asset_path +
                      "sprites/Battle/Ailments/Bubbified_AA_A00.png");
  setFrameAilment(Infliction::DEATHTIMER,
                  asset_path +
                      "sprites/Battle/Ailments/DeathTimer_AA_A00.png");
  setFrameAilment(Infliction::PARALYSIS,
                  asset_path +
                      "sprites/Battle/Ailments/Paralysis_AA_A00.png");
  setFrameAilment(Infliction::BLINDNESS,
                  asset_path +
                      "sprites/Battle/Ailments/Blind_AA_A00.png");
  setFrameAilment(Infliction::DREADSTRUCK,
                  asset_path +
                      "sprites/Battle/Ailments/DreadStruck_AA_A00.png");
  setFrameAilment(Infliction::DREAMSNARE,
                  asset_path +
                      "sprites/Battle/Ailments/DreamSnare_AA_A00.png");
  setFrameAilment(Infliction::HELLBOUND,
                  asset_path +
                      "sprites/Battle/Ailments/HellBound_AA_A00.png");
  setFrameAilment(Infliction::BOND,
                  asset_path +
                      "sprites/Battle/Ailments/Bond02_AA_A00.png");
  setFrameAilment(Infliction::BONDED,
                  asset_path +
                      "sprites/Battle/Ailments/Bond01_AA_A00.png");
  setFrameAilment(Infliction::ALLATKBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/AllAtkBuff_AA_A00.png");
  setFrameAilment(Infliction::ALLDEFBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/AllDefBuff_AA_A00.png");
  setFrameAilment(Infliction::PHYBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/PhyBuff_AA_A00.png");
  setFrameAilment(Infliction::THRBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/ThrBuff_AA_A00.png");
  setFrameAilment(Infliction::POLBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/PolBuff_AA_A00.png");
  setFrameAilment(Infliction::PRIBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/PriBuff_AA_A00.png");
  setFrameAilment(Infliction::CHGBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/ChgBuff_AA_A00.png");
  setFrameAilment(Infliction::CYBBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/CybBuff_AA_A00.png");
  setFrameAilment(Infliction::NIHBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/NihBuff_AA_A00.png");
  setFrameAilment(Infliction::LIMBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/LimBuff_AA_A00.png");
  setFrameAilment(Infliction::UNBBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/_placeholder.png");
  setFrameAilment(Infliction::VITBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/VitBuff_AA_A00.png");
  setFrameAilment(Infliction::QDBUFF,
                  asset_path +
                      "sprites/Battle/Ailments/QDBuff_AA_A00.png");
  setFrameAilment(Infliction::ROOTBOUND,
                  asset_path +
                      "sprites/Battle/Ailments/RootBound_AA_A00.png");
  setFrameAilment(Infliction::DOUBLECAST,
                  asset_path +
                      "sprites/Battle/Ailments/DoubleCast_AA_A00.png");
  setFrameAilment(Infliction::TRIPLECAST,
                  asset_path +
                      "sprites/Battle/Ailments/TripleCast_AA_A00.png");
  setFrameAilment(Infliction::HALFCOST,
                  asset_path +
                      "sprites/Battle/Ailments/HalfCost_AA_A00.png");
  setFrameAilment(Infliction::REFLECT,
                  asset_path +
                      "sprites/Battle/Ailments/Reflect_AA_A00.png");
  setFrameAilment(Infliction::HIBERNATION,
                  asset_path +
                      "sprites/Battle/Ailments/Hibernation_AA_A00.png");
  setFrameAilment(Infliction::CURSE,
                  asset_path +
                      "sprites/Battle/Ailments/Curse_AA_A00.png");
  setFrameAilment(Infliction::METATETHER,
                  asset_path +
                      "sprites/Battle/Ailments/MetaTether_AA_A00.png");
}

void BattleDisplayData::buildFramesAttributes(std::string asset_path)
{
  /* Sets the attribute frames */
  setFrameAttribute(Attribute::UNBR,
                    asset_path +
                        "sprites/Overlay/Menu/Unbearability.png");
  setFrameAttribute(Attribute::LIMB, asset_path +
                                         "sprites/Overlay/Menu/Limbertude.png");
  setFrameAttribute(Attribute::WILL, asset_path +
                                         "sprites/Overlay/Menu/Momentum.png");
}

void BattleDisplayData::buildFramesElements(std::string asset_path)
{
  /* Set the elements */
  setFrameElement(Element::PHYSICAL,
                  asset_path +
                      "sprites/Battle/Skills/Elements/Physical_AA_A00.png");
  setFrameElement(Element::FIRE,
                  asset_path +
                      "sprites/Battle/Skills/Elements/Thermal_AA_A00.png");
  setFrameElement(Element::ICE,
                  asset_path +
                      "sprites/Battle/Skills/Elements/Polar_AA_A00.png");
  setFrameElement(Element::FOREST,
                  asset_path +
                      "sprites/Battle/Skills/Elements/Primal_AA_A00.png");
  setFrameElement(Element::ELECTRIC,
                  asset_path +
                      "sprites/Battle/Skills/Elements/Charge_AA_A00.png");
  setFrameElement(Element::DIGITAL,
                  asset_path +
                      "sprites/Battle/Skills/Elements/Cyber_AA_A00.png");
  setFrameElement(Element::NIHIL,
                  asset_path +
                      "sprites/Battle/Skills/Elements/Nihil_AA_A00.png");
}

void BattleDisplayData::buildFramesScopes(std::string asset_path)
{
  /* Set the action scopes */
  setFrameScope(ActionScope::USER,
                asset_path +
                    "sprites/Battle/Skills/Targets/user_AA_A00.png");
  setFrameScope(ActionScope::ONE_TARGET,
                asset_path +
                    "sprites/Battle/Skills/Targets/single_AA_A00.png");
  setFrameScope(ActionScope::ONE_ENEMY,
                asset_path +
                    "sprites/Battle/Skills/Targets/singleenemy_AA_A00.png");
  setFrameScope(ActionScope::TWO_ENEMIES,
                asset_path +
                    "sprites/Battle/Skills/Targets/twoenemies_AA_A00.png");
  setFrameScope(ActionScope::ALL_ENEMIES,
                asset_path +
                    "sprites/Battle/Skills/Targets/allenemies_AA_A00.png");
  setFrameScope(ActionScope::ONE_ALLY,
                asset_path +
                    "sprites/Battle/Skills/Targets/singleally_AA_A00.png");
  setFrameScope(
      ActionScope::ONE_ALLY_NOT_USER,
      asset_path +
          "sprites/Battle/Skills/Targets/singlenotuserally_AA_A00.png");
  setFrameScope(ActionScope::TWO_ALLIES,
                asset_path +
                    "sprites/Battle/Skills/Targets/twoallies_AA_A00.png");
  setFrameScope(ActionScope::ALL_ALLIES,
                asset_path +
                    "sprites/Battle/Skills/Targets/allallies_AA_A00.png");
  setFrameScope(ActionScope::ONE_ALLY_KO,
                asset_path +
                    "sprites/Battle/Skills/Targets/singlekoedally_AA_A00.png");
  setFrameScope(ActionScope::ALL_ALLIES_KO,
                asset_path +
                    "sprites/Battle/Skills/Targets/allkoedallies_AA_A00.png");
  setFrameScope(ActionScope::ONE_PARTY,
                asset_path +
                    "sprites/Battle/Skills/Targets/singleparty_AA_A00.png");
  setFrameScope(ActionScope::ALL_TARGETS,
                asset_path +
                    "sprites/Battle/Skills/Targets/all_AA_A00.png");
  setFrameScope(ActionScope::NOT_USER,
                asset_path +
                    "sprites/Battle/Skills/Targets/singlenotuser_AA_A00.png");
  setFrameScope(ActionScope::ALL_NOT_USER,
                asset_path +
                    "sprites/Battle/Skills/Targets/allnotuser_AA_A00.png");
}

void BattleDisplayData::buildFramesExtra(std::string asset_path)
{
  if(renderer)
  {
    frame_percent =
        new Frame(asset_path +
                      "sprites/Battle/Skills/Extras/Accuracy_AA_A00.png",
                  renderer);
    frame_qd = new Frame(asset_path +
                             "sprites/Battle/Skills/Extras/QDSymbol_AA_A00.png",
                         renderer);
    frame_time =
        new Frame(asset_path +
                      "sprites/Battle/Skills/Extras/Cooldown_AA_A00.png",
                  renderer);
  }
}

void BattleDisplayData::buildItemMap(std::vector<Item*> items)
{
  item_map.clear();

  for(const auto& item : items)
  {
    if(item)
    {
      item_map.push_back(std::make_pair(item->getGameID(), item->getName()));
    }
  }
}

void BattleDisplayData::buildPlepsAilments(std::string asset_path)
{
  /* Set up the Ailment pleps */
  auto sprite_paralysis = new Sprite();
  sprite_paralysis->setBuildInformation(
      asset_path + "sprites/Battle/Pleps/paralysisplep_AA_A", ".png",
      3);
  sprite_paralysis->setAnimationTime(70);
  pleps_ailments.emplace(Infliction::PARALYSIS, sprite_paralysis);

  auto sprite_confusion = new Sprite();
  sprite_confusion->setBuildInformation(
      asset_path + "sprites/Battle/Pleps/confusionplep_AA_A", ".png",
      3);
  sprite_confusion->setAnimationTime(80);
  pleps_ailments.emplace(Infliction::CONFUSE, sprite_confusion);

  auto sprite_poison = new Sprite();
  sprite_poison->setBuildInformation(asset_path +
                                         "sprites/Battle/Pleps/poisonplep_AA_A",
                                     ".png", 4);
  sprite_poison->setAnimationTime(95);
  pleps_ailments.emplace(Infliction::POISON, sprite_poison);

  auto sprite_all_atk_buff = new Sprite();
  sprite_all_atk_buff->setBuildInformation(
      asset_path + "sprites/Battle/Pleps/raiseplep_AA_A", ".png",
      88);
  sprite_all_atk_buff->setAnimationTime(20);
  pleps_ailments.emplace(Infliction::ALLATKBUFF, sprite_all_atk_buff);

  auto sprite_all_def_buff = new Sprite();
  sprite_all_def_buff->setBuildInformation(
      asset_path + "sprites/Battle/Pleps/raiseplep_AA_A", ".png",
      88);
  sprite_all_def_buff->setAnimationTime(20);
  pleps_ailments.emplace(Infliction::ALLDEFBUFF, sprite_all_def_buff);

  auto sprite_buff = new Sprite();
  sprite_buff->setBuildInformation(asset_path +
                                       "sprites/Battle/Pleps/raiseplep_AA_A",
                                   ".png", 88);
  sprite_buff->setAnimationTime(20);
  pleps_ailments.emplace(Infliction::LIMBUFF, sprite_buff);
  pleps_ailments.emplace(Infliction::ALLATKBUFF, sprite_buff);
  pleps_ailments.emplace(Infliction::ALLDEFBUFF, sprite_buff);
  pleps_ailments.emplace(Infliction::LIMBUFF, sprite_buff);

  auto sprite_hibernation = new Sprite();
  sprite_hibernation->setBuildInformation(
      asset_path + "sprites/Battle/Pleps/hibernationplep_AA_A",
      ".png", 9);
  sprite_hibernation->setAnimationTime(70);
  pleps_ailments.emplace(Infliction::HIBERNATION, sprite_hibernation);
}

void BattleDisplayData::buildPlepsEvents(std::string asset_path)
{
  // assert(renderer);

  // auto sprite_defend_start =
  //     new Sprite(asset_path +
  //     "sprites/Battle/Pleps/defendplep_AA_A",
  //                7, ".png", renderer);
  // sprite_defend_start->setAnimationTime(375);
  // sprite_defend_start->switchDirection();

  // auto sprite_defend_break =
  //     new Sprite(asset_path +
  //     "sprites/Battle/Pleps/defendplep_AA_A",
  //                7, ".png", renderer);
  // sprite_defend_break->setAnimationTime(90);
  // // sprite_defend_break->switchDirection();

  // auto sprite_defend_persist =
  //     new Sprite(asset_path +
  //     "sprites/Battle/Pleps/defendplep_AA_A",
  //                7, ".png", renderer);
  // sprite_defend_persist->setAnimationTime(90);

  // sprite_implode = new Sprite(asset_path +
  // "sprites/Battle/Pleps/detonateplep_AA_A", 6, ".png", renderer);

  // setPlepEvent(EventType::BEGIN_DEFEND, sprite_defend_start);
  // setPlepEvent(EventType::BREAK_DEFEND, sprite_defend_break);
  // setPlepEvent(EventType::PERSIST_DEFEND, sprite_defend_persist);
}

void BattleDisplayData::setFrameAilment(Infliction type, std::string path)
{
  if(renderer && type != Infliction::INVALID)
    frames_ailments.emplace(type, new Frame(path, renderer));
}

void BattleDisplayData::setFrameAttribute(Attribute type, std::string path)
{
  if(renderer && type != Attribute::NONE)
    frames_attributes.emplace(type, new Frame(path, renderer));
}

void BattleDisplayData::setFrameElement(Element type, std::string path)
{
  if(renderer && type != Element::NONE)
    frames_elements.emplace(type, new Frame(path, renderer));
}

void BattleDisplayData::setFrameScope(ActionScope type, std::string path)
{
  if(renderer && type != ActionScope::NO_SCOPE)
    frames_scopes.emplace(type, new Frame(path, renderer));
}

// void BattleDisplayData::setPlepEvent(EventType event, Sprite* plep)
// {
//   if(renderer)
//     pleps_events.emplace(event, plep);
// }

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool BattleDisplayData::isDataBuilt()
{
  return data_built;
}

bool BattleDisplayData::buildData(std::string base_game_path)
{
  if(renderer)
  {
    buildBattleBar(config->getBasePath());
    buildFramesAilments(base_game_path);
    buildFramesAttributes(base_game_path);
    buildFramesElements(base_game_path);
    buildFramesScopes(base_game_path);
    buildFramesExtra(base_game_path);
    buildPlepsAilments(base_game_path);
    // buildPlepsEvents(base_game_path);

    data_built = true;

    return true;
  }

  return false;
}

Frame* BattleDisplayData::getBattleBar()
{
  return frame_battle_bar;
}

Frame* BattleDisplayData::getFramePercent()
{
  return frame_percent;
}

Frame* BattleDisplayData::getFrameQD()
{
  return frame_qd;
}

Frame* BattleDisplayData::getFrameTime()
{
  return frame_time;
}

Frame* BattleDisplayData::getFrameAilment(Infliction ailment_frame)
{
  if(ailment_frame != Infliction::INVALID)
  {
    auto found = frames_ailments.find(ailment_frame);

    if(found != end(frames_ailments))
      return found->second;
  }

  return nullptr;
}

Frame* BattleDisplayData::getFrameAttribute(Attribute attribute_frame)
{
  if(attribute_frame != Attribute::NONE)
  {
    auto found = frames_attributes.find(attribute_frame);

    if(found != end(frames_attributes))
      return found->second;
  }

  return nullptr;
}

Frame* BattleDisplayData::getFrameElement(Element element_frame)
{
  if(element_frame != Element::NONE)
  {
    auto found = frames_elements.find(element_frame);

    if(found != end(frames_elements))
      return found->second;
  }

  return nullptr;
}

Frame* BattleDisplayData::getFrameScope(ActionScope scope_frame)
{
  if(scope_frame != ActionScope::NO_SCOPE)
  {
    auto found = frames_scopes.find(scope_frame);

    if(found != end(frames_scopes))
      return found->second;
  }

  return nullptr;
}

// Return the string of the Item name
std::string BattleDisplayData::getItemName(int32_t id)
{
  for(const auto& element : item_map)
    if(element.first == (uint32_t)id)
      return element.second;

  return "";
}

Sprite* BattleDisplayData::getPlepAilment(Infliction ailment)
{
  if(ailment != Infliction::INVALID)
  {
    auto found = pleps_ailments.find(ailment);

    if(found != end(pleps_ailments))
      return found->second;
  }

  return nullptr;
}

// Sprite* BattleDisplayData::getPlepEvent(EventType event_type)
// {
//   if(event_type != EventType::NONE)
//   {
//     auto found = pleps_events.find(event_type);

//     if(found != end(pleps_events))
//       return found->second;
//   }

//   return nullptr;
// }

void BattleDisplayData::setConfig(Options* config)
{
  this->config = config;
}

void BattleDisplayData::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;
}
