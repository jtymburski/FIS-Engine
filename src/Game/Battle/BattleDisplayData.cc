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
  for(auto& scope : frames_scopes)
    if(scope.second)
      delete scope.second;
  frames_scopes.clear();

  for(auto& element : frames_elements)
    if(element.second)
      delete element.second;
  frames_elements.clear();

  for(auto& ailment : frames_ailments)
    if(ailment.second)
      delete ailment.second;
  frames_ailments.clear();

  // TODO: THIS STILL FAILS -- WHY??
  // for(auto& ailment_plep : pleps_ailments)
  //   if(ailment_plep.second)
  //     delete ailment_plep.second;
  // pleps_ailments.clear();

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

void BattleDisplayData::buildBattleBar()
{
  frame_battle_bar =
      new Frame(config->getBasePath() + "sprites/Overlay/battle.png", renderer);
}

void BattleDisplayData::buildFramesAilments()
{
  /* Set the ailments */
  setFrameAilment(Infliction::POISON,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Poison_AA_A00.png");
  setFrameAilment(Infliction::BURN,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Burn01_AA_A00.png");
  setFrameAilment(Infliction::SCALD,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Burn02_AA_A00.png");
  setFrameAilment(Infliction::CHARR,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Burn03_AA_A00.png");
  setFrameAilment(Infliction::BERSERK,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Berserk_AA_A00.png");
  setFrameAilment(Infliction::CONFUSE,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Confused_AA_A00.png");
  setFrameAilment(Infliction::SILENCE,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Silence_AA_A00.png");
  setFrameAilment(Infliction::BUBBIFY,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Bubbified_AA_A00.png");
  setFrameAilment(Infliction::DEATHTIMER,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/DeathTimer_AA_A00.png");
  setFrameAilment(Infliction::PARALYSIS,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Paralysis_AA_A00.png");
  setFrameAilment(Infliction::BLINDNESS,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Blind_AA_A00.png");
  setFrameAilment(Infliction::DREADSTRUCK,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/DreadStruck_AA_A00.png");
  setFrameAilment(Infliction::DREAMSNARE,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/DreamSnare_AA_A00.png");
  setFrameAilment(Infliction::HELLBOUND,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/HellBound_AA_A00.png");
  setFrameAilment(Infliction::BOND,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Bond02_AA_A00.png");
  setFrameAilment(Infliction::BONDED,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Bond01_AA_A00.png");
  setFrameAilment(Infliction::ALLATKBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/AllAtkBuff_AA_A00.png");
  setFrameAilment(Infliction::ALLDEFBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/AllDefBuff_AA_A00.png");
  setFrameAilment(Infliction::PHYBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/PhyBuff_AA_A00.png");
  setFrameAilment(Infliction::THRBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/ThrBuff_AA_A00.png");
  setFrameAilment(Infliction::POLBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/PolBuff_AA_A00.png");
  setFrameAilment(Infliction::PRIBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/PriBuff_AA_A00.png");
  setFrameAilment(Infliction::CHGBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/ChgBuff_AA_A00.png");
  setFrameAilment(Infliction::CYBBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/CybBuff_AA_A00.png");
  setFrameAilment(Infliction::NIHBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/NihBuff_AA_A00.png");
  setFrameAilment(Infliction::LIMBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/LimBuff_AA_A00.png");
  setFrameAilment(Infliction::UNBBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/_placeholder.png");
  setFrameAilment(Infliction::VITBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/VitBuff_AA_A00.png");
  setFrameAilment(Infliction::QDBUFF,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/QDBuff_AA_A00.png");
  setFrameAilment(Infliction::ROOTBOUND,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/RootBound_AA_A00.png");
  setFrameAilment(Infliction::DOUBLECAST,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/DoubleCast_AA_A00.png");
  setFrameAilment(Infliction::TRIPLECAST,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/TripleCast_AA_A00.png");
  setFrameAilment(Infliction::HALFCOST,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/HalfCost_AA_A00.png");
  setFrameAilment(Infliction::REFLECT,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Reflect_AA_A00.png");
  setFrameAilment(Infliction::HIBERNATION,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Hibernation_AA_A00.png");
  setFrameAilment(Infliction::CURSE,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/Curse_AA_A00.png");
  setFrameAilment(Infliction::METATETHER,
                  config->getBasePath() +
                      "sprites/Battle/Ailments/MetaTether_AA_A00.png");
}

void BattleDisplayData::buildFramesElements()
{
  /* Set the elements */
  setFrameElement(Element::PHYSICAL,
                  config->getBasePath() +
                      "sprites/Battle/Skills/Elements/Physical_AA_A00.png");
  setFrameElement(Element::FIRE,
                  config->getBasePath() +
                      "sprites/Battle/Skills/Elements/Thermal_AA_A00.png");
  setFrameElement(Element::ICE,
                  config->getBasePath() +
                      "sprites/Battle/Skills/Elements/Polar_AA_A00.png");
  setFrameElement(Element::FOREST,
                  config->getBasePath() +
                      "sprites/Battle/Skills/Elements/Primal_AA_A00.png");
  setFrameElement(Element::ELECTRIC,
                  config->getBasePath() +
                      "sprites/Battle/Skills/Elements/Charge_AA_A00.png");
  setFrameElement(Element::DIGITAL,
                  config->getBasePath() +
                      "sprites/Battle/Skills/Elements/Cyber_AA_A00.png");
  setFrameElement(Element::NIHIL,
                  config->getBasePath() +
                      "sprites/Battle/Skills/Elements/Nihil_AA_A00.png");
}

void BattleDisplayData::buildFramesScopes()
{
  /* Set the action scopes */
  setFrameScope(ActionScope::USER,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/user_AA_A00.png");
  setFrameScope(ActionScope::ONE_TARGET,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/single_AA_A00.png");
  setFrameScope(ActionScope::ONE_ENEMY,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/singleenemy_AA_A00.png");
  setFrameScope(ActionScope::TWO_ENEMIES,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/twoenemies_AA_A00.png");
  setFrameScope(ActionScope::ALL_ENEMIES,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/allenemies_AA_A00.png");
  setFrameScope(ActionScope::ONE_ALLY,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/singleally_AA_A00.png");
  setFrameScope(
      ActionScope::ONE_ALLY_NOT_USER,
      config->getBasePath() +
          "sprites/Battle/Skills/Targets/singlenotuserally_AA_A00.png");
  setFrameScope(ActionScope::TWO_ALLIES,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/twoallies_AA_A00.png");
  setFrameScope(ActionScope::ALL_ALLIES,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/allallies_AA_A00.png");
  setFrameScope(ActionScope::ONE_ALLY_KO,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/singlekoedally_AA_A00.png");
  setFrameScope(ActionScope::ALL_ALLIES_KO,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/allkoedallies_AA_A00.png");
  setFrameScope(ActionScope::ONE_PARTY,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/singleparty_AA_A00.png");
  setFrameScope(ActionScope::ALL_TARGETS,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/all_AA_A00.png");
  setFrameScope(ActionScope::NOT_USER,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/singlenotuser_AA_A00.png");
  setFrameScope(ActionScope::ALL_NOT_USER,
                config->getBasePath() +
                    "sprites/Battle/Skills/Targets/allnotuser_AA_A00.png");
}

void BattleDisplayData::buildFramesExtra()
{
  if(renderer)
  {
    frame_percent =
        new Frame(config->getBasePath() +
                      "sprites/Battle/Skills/Extras/Accuracy_AA_A00.png",
                  renderer);
    frame_qd = new Frame(config->getBasePath() +
                             "sprites/Battle/Skills/Extras/QDSymbol_AA_A00.png",
                         renderer);
    frame_time =
        new Frame(config->getBasePath() +
                      "sprites/Battle/Skills/Extras/Cooldown_AA_A00.png",
                  renderer);
  }
}

void BattleDisplayData::buildPlepsAilments()
{
  bool success = true;

  /* Set up the Ailment pleps */
  auto sprite_paralysis = new Sprite(
      config->getBasePath() + "sprites/Battle/Pleps/paralysisplep_AA_A", 3,
      ".png", renderer);
  sprite_paralysis->setAnimationTime(70);
  pleps_ailments.emplace(Infliction::PARALYSIS, sprite_paralysis);

  auto sprite_confusion = new Sprite(
      config->getBasePath() + "sprites/Battle/Pleps/confusionplep_AA_A", 3,
      ".png", renderer);
  sprite_confusion->setAnimationTime(80);
  pleps_ailments.emplace(Infliction::CONFUSE, sprite_confusion);

  auto sprite_poison =
      new Sprite(config->getBasePath() + "sprites/Battle/Pleps/poisonplep_AA_A",
                 4, ".png", renderer);
  sprite_poison->setAnimationTime(95);
  pleps_ailments.emplace(Infliction::POISON, sprite_poison);

  auto sprite_buff =
      new Sprite(config->getBasePath() + "sprites/Battle/Pleps/raiseplep_AA_A",
                 9, ".png", renderer);
  sprite_buff->setAnimationTime(95);
  pleps_ailments.emplace(Infliction::ALLATKBUFF, sprite_buff);
  pleps_ailments.emplace(Infliction::ALLDEFBUFF, sprite_buff);
  pleps_ailments.emplace(Infliction::LIMBUFF, sprite_buff);

  auto sprite_hibernation = new Sprite(
      config->getBasePath() + "sprites/Battle/Pleps/hibernationplep_AA_A", 4,
      ".png", renderer);
  sprite_hibernation->insertTail(
      config->getBasePath() + "sprites/Battle/Pleps/hibernationplep_AA_A00.png",
      renderer);
  sprite_hibernation->insertTail(
      config->getBasePath() + "sprites/Battle/Pleps/hibernationplep_AA_A01.png",
      renderer);
  sprite_hibernation->insertTail(
      config->getBasePath() + "sprites/Battle/Pleps/hibernationplep_AA_A02.png",
      renderer);
  sprite_hibernation->insertTail(
      config->getBasePath() + "sprites/Battle/Pleps/hibernationplep_AA_A03.png",
      renderer);
  sprite_hibernation->setAnimationTime(70);
  pleps_ailments.emplace(Infliction::HIBERNATION, sprite_hibernation);

  if(!success)
    std::cout << "[Error] Building ailment pleps." << std::endl;
  else
    std::cout << "Successfully built sprites " << std::endl;
}

void BattleDisplayData::buildPlepsEvents()
{
  // assert(renderer);

  // auto sprite_defend_start =
  //     new Sprite(config->getBasePath() +
  //     "sprites/Battle/Pleps/defendplep_AA_A",
  //                7, ".png", renderer);
  // sprite_defend_start->setAnimationTime(375);
  // sprite_defend_start->switchDirection();

  // auto sprite_defend_break =
  //     new Sprite(config->getBasePath() +
  //     "sprites/Battle/Pleps/defendplep_AA_A",
  //                7, ".png", renderer);
  // sprite_defend_break->setAnimationTime(90);
  // // sprite_defend_break->switchDirection();

  // auto sprite_defend_persist =
  //     new Sprite(config->getBasePath() +
  //     "sprites/Battle/Pleps/defendplep_AA_A",
  //                7, ".png", renderer);
  // sprite_defend_persist->setAnimationTime(90);

  // sprite_implode = new Sprite(config->getBasePath() +
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

bool BattleDisplayData::buildData()
{
  if(renderer)
  {
    buildBattleBar();
    buildFramesAilments();
    buildFramesElements();
    buildFramesScopes();
    buildFramesExtra();
    buildPlepsAilments();
    // buildPlepsEvents();

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
