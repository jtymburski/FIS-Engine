/*******************************************************************************
 * Class Name: Helpers [Implementation]
 * Date Created: November 17th, 2013
 * Inheritance: None
 * Description: Helpers is a collection of functions to be used as common
 *              helpers in other classes. This includes such functions
 *              as dealing with random numbers, working with general values
 *              and building tables, etc.
 *
 * Notes
 * -----
 * [1]: Was tuned to remove the global nature of both variables and functions.
 *      Also, to expand to general functions instead of math specific
 *
 * See .h file for TODOs
 ******************************************************************************/
#include "Helpers.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t Helpers::kMAX_RENDER_DEPTH = 10;
const uint16_t Helpers::kTILE_SIZE = 64;

SDL_Texture* Helpers::mask_black = nullptr;
SDL_Texture* Helpers::mask_white = nullptr;

/*=============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *============================================================================*/

/* Seed the random number generators with high-precision */
const uint32_t Helpers::seed_original =
    std::chrono::high_resolution_clock::now().time_since_epoch().count();

std::mt19937 Helpers::rand_eng(seed_original);
std::mt19937_64 Helpers::rand_64_eng(seed_original);

/*
 * Description: Determines if a certain event will occur, given a chance. The
 *              accuracy (or magnitude of accuracy) can be changed using the
 *              second paramater, but defaults to 100 for % value.
 *
 * Notes: example uses:
 *        - 67% chance : bool b = chanceHappens(67, 100)
 *        - 67.4% chance: bool b = chanceHappens(674, 1000)
 *        - chanceHappens(n + 1, n) -> always true
 *        - chanceHappens(0, n) -> always false
 *
 * Inputs: const uint32_t &pc_chance - const ref to the chance the event has
 *         const uint32_t &mag - magnitude of accuracy
 *                                    (bound to generate #s)
 * Output: bool - determination of the event
 */
bool Helpers::chanceHappens(const uint32_t& pc_chance, const uint32_t& mag)
{
  if(pc_chance >= mag)
    return true;
  if(pc_chance <= 0)
    return false;

  auto random = randU(1, static_cast<int>(mag));

  if(static_cast<uint32_t>(random) <= pc_chance)
    return true;
  return false;
}

/*
 * Description: Simulates the flipping of a coin and returns the result
 *
 * Inputs: none
 * Output: bool - outcome of the simulated coin flip
 */
bool Helpers::flipCoin()
{
  return randU(0, 1);
}

/*
 * Description: Calculates a random float between a and b (min/max independant)
 *
 * Inputs: a - one bound to find a random float for
 *         b - other bound to find a random float for
 * Output: float - random float between the two bounds
 */
float Helpers::randFloat(const float& a, const float& b)
{
  auto min = std::min(a, b);
  auto max = std::max(a, b);

  std::uniform_real_distribution<float> distribution(min, max);

  return distribution(rand_eng);
}

/*
 * Description: Generates a uniform random integer in the range [0, max]
 *              and returns the result
 *
 * Inputs: const int &max - ref to the maximum value
 * Output: int - generated number in the range [0, max]
 */
int Helpers::randInt(const int& max)
{
  return randU(0, max);
}

/*
 * Description: Generates a uniform integer between two given bounds
 *              (can be any order) of the form [a, b]
 *
 * Inputs: const int &a - const ref to one of the bounds
 8         const int &b - const ref to one of the bounds
 * Output: int - the generated number between the two bounds
 */
int Helpers::randU(const int& a, const int& b)
{
  auto min = std::min(a, b);
  auto max = std::max(a, b);

  std::uniform_int_distribution<int> distribution(min, max);

  return distribution(rand_eng);
}

/*
 * Description: Generates a random 32-bit integer using std::mt19937 and returns
 *              the result.
 *
 * Inputs: none
 * Output: uint32_t - pseudo random 32 bit integer
 */
uint32_t Helpers::randU32()
{
  return rand_eng();
}

/*
 * Description: Generates a random 64-bit integer using std::mt19937_64
 *              and returns the result.
 *
 * Inputs: none
 * Output: uint64_t - randomly generated 64-bit integer
 */
uint64_t Helpers::randU64()
{
  return rand_64_eng();
}

/*
 * Description: Simulates the rolling of an s-sided time x # of times and
 *              returns the result.
 *
 * Inputs: const int& s_sides - const ref to # of sided die to represent
 *         const int& x_times - const ref to # of times to roll the die
 * Output: int - the result of the die roll simulation
 */
int Helpers::rollXS(const int& x_sides, const int& s_times)
{
  auto total = 0;

  for(auto i = 0; i < s_times; i++)
    total += randU(0, x_sides + 1);

  return total;
}

/*=============================================================================
 * GRAMMAR FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns "an" for "apple" and "a" for "eulogy" and
 *              "a" for "sword" etc.
 *
 * Inputs: std::string - noun - noun to find a/an for
 * Output: std::string - "a" or "an" where required
 */
std::string Helpers::a_An(const std::string& noun)
{
  if(noun.size() == 0)
    return "";

  auto a_an = "a";
  const auto lower = noun;

  for(auto c : lower)
    c = tolower(c);

  const auto& initial = noun.at(0);

  /* General vowel vs. consonant rule ("u" is usually consonant) */
  if(lower.size() == 1)
  {
    if(isVowel(initial) && initial != 'u')
      a_an = "an";
    a_an = "a";
  }

  /* Generally words that start with vowels are "an" */
  if(isVowel(initial))
    a_an = "an";

  if((initial == 'y') && !isVowel(lower.at(1)))
    a_an = "a";

  /* Some exceptions for consonant and vowel sounds (e.g. "a eulogy") */
  if(lower.substr(0, 2) == "eu" || lower.substr(0, 3) == "one" ||
     lower.substr(0, 3) == "uni")
    a_an = "a";
  else if(lower.substr(0, 3) == "hon")
    a_an = "an";

  return a_an;
}

/*
 * Description: Returns string names of enumerated action types
 *
 * Inputs: ActionType - enumerated action type to find string for
 * Output: std::string - the string of the corresponding enumeration
 */
std::string Helpers::actionTypeToStr(const ActionType& action_type)
{
  if(action_type == ActionType::SKILL)
    return "Skill";
  else if(action_type == ActionType::ITEM)
    return "Item";
  else if(action_type == ActionType::DEFEND)
    return "Defend";
  else if(action_type == ActionType::GUARD)
    return "Guard";
  else if(action_type == ActionType::IMPLODE)
    return "Implode";
  else if(action_type == ActionType::RUN)
    return "Run";
  else if(action_type == ActionType::PASS)
    return "Pass";
  else if(action_type == ActionType::NONE)
    return "None";

  return "";
}

/*
 * Description: Returns the enum form a given ActionScope string
 *
 * Inputs: std::string scope - the string version of the scope
 * Output: ActionScope - enum of scope corresponding to string
 */
ActionScope Helpers::actionScopeFromStr(const std::string scope)
{
  std::string scope_up = scope;
  std::transform(scope_up.begin(), scope_up.end(), scope_up.begin(), toupper);

  /* Parse */
  if(scope_up == "USER")
    return ActionScope::USER;
  else if(scope_up == "ONE_TARGET")
    return ActionScope::ONE_TARGET;
  else if(scope_up == "ONE_ENEMY")
    return ActionScope::ONE_ENEMY;
  else if(scope_up == "TWO_ENEMIES")
    return ActionScope::TWO_ENEMIES;
  else if(scope_up == "ALL_ENEMIES")
    return ActionScope::ALL_ENEMIES;
  else if(scope_up == "ONE_ALLY")
    return ActionScope::ONE_ALLY;
  else if(scope_up == "ONE_ALLY_NOT_USER")
    return ActionScope::ONE_ALLY_NOT_USER;
  else if(scope_up == "TWO_ALLIES")
    return ActionScope::TWO_ALLIES;
  else if(scope_up == "ALL_ALLIES")
    return ActionScope::ALL_ALLIES;
  else if(scope_up == "ONE_ALLY_KO")
    return ActionScope::ONE_ALLY_KO;
  else if(scope_up == "ALL_ALLIES_KO")
    return ActionScope::ALL_ALLIES_KO;
  else if(scope_up == "ONE_PARTY")
    return ActionScope::ONE_PARTY;
  else if(scope_up == "ALL_TARGETS")
    return ActionScope::ALL_TARGETS;
  else if(scope_up == "NOT_USER")
    return ActionScope::NOT_USER;
  else if(scope_up == "ALL_NOT_USER")
    return ActionScope::ALL_NOT_USER;
  return ActionScope::NO_SCOPE;
}

/*
 * Description: Returns the string form a given ActionScope enum
 *
 * Inputs: ActionScope - enum of scope of actions for Skills
 * Output: std::string - string corresponding to the ActionScope
 */
std::string Helpers::actionScopeToStr(const ActionScope& action_scope)
{
  if(action_scope == ActionScope::USER)
    return "USER";
  else if(action_scope == ActionScope::ONE_TARGET)
    return "ONE_TARGET";
  else if(action_scope == ActionScope::ONE_ENEMY)
    return "ONE_ENEMY";
  else if(action_scope == ActionScope::TWO_ENEMIES)
    return "TWO_ENEMIES";
  else if(action_scope == ActionScope::ALL_ENEMIES)
    return "ALL_ENEMIES";
  else if(action_scope == ActionScope::ONE_ALLY)
    return "ONE_ALLY";
  else if(action_scope == ActionScope::ONE_ALLY_NOT_USER)
    return "ONE_ALLY_NOT_USER";
  else if(action_scope == ActionScope::TWO_ALLIES)
    return "TWO_ALLIES";
  else if(action_scope == ActionScope::ALL_ALLIES)
    return "ALL_ALLIES";
  else if(action_scope == ActionScope::ONE_ALLY_KO)
    return "ONE_ALLY_KO";
  else if(action_scope == ActionScope::ALL_ALLIES_KO)
    return "ALL_ALLIES_KO";
  else if(action_scope == ActionScope::ONE_PARTY)
    return "ONE_PARTY";
  else if(action_scope == ActionScope::ALL_TARGETS)
    return "ALL_TARGETS";
  else if(action_scope == ActionScope::NOT_USER)
    return "NOT_USER";
  else if(action_scope == ActionScope::ALL_NOT_USER)
    return "ALL_NOT_USER";
  else if(action_scope == ActionScope::NO_SCOPE)
    return "NO_SCOPE";

  return "";
}

/*
 * Description: Returns the AI difficulty enum of an AI Module
 *
 * Inputs: AIDifficulty - enumerated difficulty
 * Output: std::string - corresponding string form of the enum
 */
/* Convert enumerated AI Difficulty to String */
std::string Helpers::aiDifficultyToStr(const AIDifficulty& ai_difficulty)
{
  if(ai_difficulty == AIDifficulty::RANDOM)
    return "RANDOM";
  else if(ai_difficulty == AIDifficulty::PRIORITY)
    return "PRIORITY";

  return "";
}

/*
 * Description: Returns the string form of an AIPersonality enum.
 *
 * Inputs: AIPersonality - the enumerated personality type of an AI
 * Output: std::string - the corresponding string for the personality
 */
std::string Helpers::aiPersonalityToStr(const AIPersonality& ai_personality)
{
  if(ai_personality == AIPersonality::MODERATOR)
    return "MODERATOR";
  else if(ai_personality == AIPersonality::AGGRESSOR)
    return "AGGRESSOR";
  else if(ai_personality == AIPersonality::DEFENDER)
    return "DEFENDER";
  else if(ai_personality == AIPersonality::PROTECTOR)
    return "PROTECTOR";
  else if(ai_personality == AIPersonality::RETALIATOR)
    return "RETALIATOR";
  else if(ai_personality == AIPersonality::MASOCHISTIC)
    return "MASOCHISTIC";
  else if(ai_personality == AIPersonality::ANNIHILATOR)
    return "ANNIHILATOR";
  else if(ai_personality == AIPersonality::RUNNER)
    return "RUNNER";
  else if(ai_personality == AIPersonality::PASSER)
    return "PASSER";
  else if(ai_personality == AIPersonality::NONE)
    return "NONE";

  return "";
}

/*
 * Description: Returns the enum form of an ailment (string)
 *
 * Inputs: std::string ail - string form of ailment
 * Output: Infliction - ailment enumeration
 */
Infliction Helpers::ailmentFromStr(const std::string& ail)
{
  std::string ail_up = ail;
  std::transform(ail_up.begin(), ail_up.end(), ail_up.begin(), toupper);

  /* Parse */
  if(ail_up == "POISON")
    return Infliction::POISON;
  else if(ail_up == "BURN")
    return Infliction::BURN;
  else if(ail_up == "SCALD")
    return Infliction::SCALD;
  else if(ail_up == "CHARR")
    return Infliction::CHARR;
  else if(ail_up == "BERSERK")
    return Infliction::BERSERK;
  else if(ail_up == "CONFUSE")
    return Infliction::CONFUSE;
  else if(ail_up == "SILENCE")
    return Infliction::SILENCE;
  else if(ail_up == "BUBBIFY")
    return Infliction::BUBBIFY;
  else if(ail_up == "DEATHTIMER")
    return Infliction::DEATHTIMER;
  else if(ail_up == "PARALYSIS")
    return Infliction::PARALYSIS;
  else if(ail_up == "BLINDNESS")
    return Infliction::BLINDNESS;
  else if(ail_up == "DREADSTRUCK")
    return Infliction::DREADSTRUCK;
  else if(ail_up == "DREAMSNARE")
    return Infliction::DREAMSNARE;
  else if(ail_up == "HELLBOUND")
    return Infliction::HELLBOUND;
  else if(ail_up == "BOND")
    return Infliction::BOND;
  else if(ail_up == "BONDED")
    return Infliction::BONDED;
  else if(ail_up == "ALLATKBUFF")
    return Infliction::ALLATKBUFF;
  else if(ail_up == "ALLDEFBUFF")
    return Infliction::ALLDEFBUFF;
  else if(ail_up == "PHYBUFF")
    return Infliction::PHYBUFF;
  else if(ail_up == "THRBUFF")
    return Infliction::THRBUFF;
  else if(ail_up == "POLBUFF")
    return Infliction::POLBUFF;
  else if(ail_up == "PRIBUFF")
    return Infliction::PRIBUFF;
  else if(ail_up == "CHGBUFF")
    return Infliction::CHGBUFF;
  else if(ail_up == "CYBBUFF")
    return Infliction::CYBBUFF;
  else if(ail_up == "NIHBUFF")
    return Infliction::NIHBUFF;
  else if(ail_up == "LIMBUFF")
    return Infliction::LIMBUFF;
  else if(ail_up == "UNBBUFF")
    return Infliction::UNBBUFF;
  else if(ail_up == "VITBUFF")
    return Infliction::VITBUFF;
  else if(ail_up == "QDBUFF")
    return Infliction::QDBUFF;
  else if(ail_up == "ROOTBOUND")
    return Infliction::ROOTBOUND;
  else if(ail_up == "DOUBLECAST")
    return Infliction::DOUBLECAST;
  else if(ail_up == "TRIPLECAST")
    return Infliction::TRIPLECAST;
  else if(ail_up == "HALFCOST")
    return Infliction::HALFCOST;
  else if(ail_up == "REFLECT")
    return Infliction::REFLECT;
  else if(ail_up == "HIBERNATION")
    return Infliction::HIBERNATION;
  else if(ail_up == "CURSE")
    return Infliction::CURSE;
  else if(ail_up == "METATETHER")
    return Infliction::METATETHER;
  else if(ail_up == "MODULATE")
    return Infliction::MODULATE;
  return Infliction::INVALID;
}

/*
 * Description: Returns the string form of an ailment (Infliction enum)
 *
 * Inputs: Infliction - enumerated type of Infliction
 * Output: std::string - the string of the enumeration
 */
std::string Helpers::ailmentToStr(const Infliction& ail)
{
  if(ail == Infliction::POISON)
    return "POISON";
  else if(ail == Infliction::BURN)
    return "BURN";
  else if(ail == Infliction::SCALD)
    return "SCALD";
  else if(ail == Infliction::CHARR)
    return "CHARR";
  else if(ail == Infliction::BERSERK)
    return "BERSERK";
  else if(ail == Infliction::CONFUSE)
    return "CONFUSE";
  else if(ail == Infliction::SILENCE)
    return "SILENCE";
  else if(ail == Infliction::BUBBIFY)
    return "BUBBIFY";
  else if(ail == Infliction::DEATHTIMER)
    return "DEATHTIMER";
  else if(ail == Infliction::PARALYSIS)
    return "PARALYSIS";
  else if(ail == Infliction::BLINDNESS)
    return "BLINDNESS";
  else if(ail == Infliction::DREADSTRUCK)
    return "DREADSTRUCK";
  else if(ail == Infliction::DREAMSNARE)
    return "DREAMSNARE";
  else if(ail == Infliction::HELLBOUND)
    return "HELLBOUND";
  else if(ail == Infliction::BOND)
    return "BOND";
  else if(ail == Infliction::BONDED)
    return "BONDED";
  else if(ail == Infliction::ALLATKBUFF)
    return "ALLATKBUFF";
  else if(ail == Infliction::ALLDEFBUFF)
    return "ALLDEFBUFF";
  else if(ail == Infliction::PHYBUFF)
    return "PHYBUFF";
  else if(ail == Infliction::THRBUFF)
    return "THRBUFF";
  else if(ail == Infliction::POLBUFF)
    return "POLBUFF";
  else if(ail == Infliction::PRIBUFF)
    return "PRIBUFF";
  else if(ail == Infliction::CHGBUFF)
    return "CHGBUFF";
  else if(ail == Infliction::CYBBUFF)
    return "CYBBUFF";
  else if(ail == Infliction::NIHBUFF)
    return "NIHBUFF";
  else if(ail == Infliction::LIMBUFF)
    return "LIMBUFF";
  else if(ail == Infliction::UNBBUFF)
    return "UNBBUFF";
  else if(ail == Infliction::VITBUFF)
    return "VITBUFF";
  else if(ail == Infliction::QDBUFF)
    return "QDBUFF";
  else if(ail == Infliction::ROOTBOUND)
    return "ROOTBOUND";
  else if(ail == Infliction::DOUBLECAST)
    return "DOUBLECAST";
  else if(ail == Infliction::TRIPLECAST)
    return "TRIPLECAST";
  else if(ail == Infliction::HALFCOST)
    return "HALFCOST";
  else if(ail == Infliction::REFLECT)
    return "REFLECT";
  else if(ail == Infliction::HIBERNATION)
    return "HIBERNATION";
  else if(ail == Infliction::CURSE)
    return "CURSE";
  else if(ail == Infliction::METATETHER)
    return "METATETHER";
  else if(ail == Infliction::MODULATE)
    return "MODULATE";
  else if(ail == Infliction::INVALID)
    return "INVALID";

  return "";
}

/*
 * Description: Returns the enumerated Attribute to String.
 *
 * Inputs: Attribute attribute - the attribute to convert
 * Output: std::string - the converted string
 */
std::string Helpers::attributeToStr(Attribute attribute)
{
  if(attribute == Attribute::VITA)
    return "VITA";
  else if(attribute == Attribute::QTDR)
    return "QTDR";
  else if(attribute == Attribute::PHAG)
    return "PHAG";
  else if(attribute == Attribute::PHFD)
    return "PHFD";
  else if(attribute == Attribute::THAG)
    return "THAG";
  else if(attribute == Attribute::THFD)
    return "THFD";
  else if(attribute == Attribute::PRAG)
    return "PRAG";
  else if(attribute == Attribute::PRFD)
    return "PRFD";
  else if(attribute == Attribute::POAG)
    return "POAG";
  else if(attribute == Attribute::POFD)
    return "POFD";
  else if(attribute == Attribute::CHAG)
    return "CHAG";
  else if(attribute == Attribute::CHFD)
    return "CHFD";
  else if(attribute == Attribute::CYAG)
    return "CYAG";
  else if(attribute == Attribute::CYFD)
    return "CYFD";
  else if(attribute == Attribute::NIAG)
    return "NIAG";
  else if(attribute == Attribute::NIFD)
    return "NIFD";
  else if(attribute == Attribute::MMNT)
    return "MMNT";
  else if(attribute == Attribute::LIMB)
    return "LIMB";
  else if(attribute == Attribute::UNBR)
    return "UNBR";
  else if(attribute == Attribute::MANN)
    return "MANN";
  return "";
}

/*
 * Description: Convert enumerated cell type to string
 *
 * Inputs: CellState - enumerated state of a cell in signature
 * Output: std::string - string form of the given enumeration
 */
std::string Helpers::cellToStr(const CellState& cell_state)
{
  if(cell_state == CellState::OPEN)
    return "OPEN";
  else if(cell_state == CellState::BUBBY)
    return "BUBBY";
  else if(cell_state == CellState::CLOSED)
    return "CLOSED";
  else if(cell_state == CellState::BLOCKED)
    return "BLOCKED";
  else if(cell_state == CellState::LINK)
    return "LINK";

  return "";
}

/*
 * Description: Converts the string version of the ElementCurve to the
 *              enumerated version.
 *
 * Inputs: std::string curve - the string version of the ElementCurve
 * Output: ElementCurve - the enumerated version of the string
 */
ElementCurve Helpers::curveFromString(const std::string& curve)
{
  std::string curve_up = curve;
  std::transform(curve_up.begin(), curve_up.end(), curve_up.begin(), toupper);

  /* Parse */
  if(curve_up == "XS" || curve_up == "GODLIKE")
    return ElementCurve::XS;
  else if(curve_up == "S" || curve_up == "XPOWERFUL")
    return ElementCurve::S;
  else if(curve_up == "A" || curve_up == "POWERFUL")
    return ElementCurve::A;
  else if(curve_up == "B" || curve_up == "STRONG")
    return ElementCurve::B;
  else if(curve_up == "C" || curve_up == "MODERATE")
    return ElementCurve::C;
  return ElementCurve::D;
}

/*
 * Description: Converts the element curve enum to the string version.
 *
 * Inputs: ElementCurve curve - the curve to convert
 *         bool simple - true if simple ('D'). false ('Normal'). default false.
 * Output: std::string - the converted string version
 */
std::string Helpers::curveToString(const ElementCurve& curve, bool simple)
{
  /* Simple form is 'XS', 'S', etc. */
  if(simple)
  {
    if(curve == ElementCurve::XS)
      return "XS";
    else if(curve == ElementCurve::S)
      return "S";
    else if(curve == ElementCurve::A)
      return "A";
    else if(curve == ElementCurve::B)
      return "B";
    else if(curve == ElementCurve::C)
      return "C";
    return "D";
  }
  /* Complex form is 'God-like', 'Normal', etc. */
  else
  {
    if(curve == ElementCurve::XS)
      return "GodLike";
    else if(curve == ElementCurve::S)
      return "XPowerful";
    else if(curve == ElementCurve::A)
      return "Powerful";
    else if(curve == ElementCurve::B)
      return "Strong";
    else if(curve == ElementCurve::C)
      return "Moderate";
    return "Normal";
  }
}

/*
 * Description: Elemental enum form of an string element.
 *
 * Inputs: std::string element - string form of element
 * Output: Element - enum form of the element
 */
Element Helpers::elementFromString(const std::string& element)
{
  std::string ele_up = element;
  std::transform(ele_up.begin(), ele_up.end(), ele_up.begin(), toupper);

  /* Parse */
  if(ele_up == "PHYSICAL")
    return Element::PHYSICAL;
  else if(ele_up == "FIRE")
    return Element::FIRE;
  else if(ele_up == "FOREST")
    return Element::FOREST;
  else if(ele_up == "ICE")
    return Element::ICE;
  else if(ele_up == "ELECTRIC")
    return Element::ELECTRIC;
  else if(ele_up == "DIGITAL")
    return Element::DIGITAL;
  else if(ele_up == "VOID")
    return Element::NIHIL;
  return Element::NONE;
}

/*
 * Description: Elemental integer form of an enumerated element. Invalid element
 *              will always return the number of valid elements.
 *
 * Inputs: Element element - enumerated form of element
 * Output: uint16_t integer form of element
 */
uint16_t Helpers::elementToInt(Element element)
{
  if(element == Element::PHYSICAL)
    return 0;
  else if(element == Element::FIRE)
    return 1;
  else if(element == Element::FOREST)
    return 2;
  else if(element == Element::ICE)
    return 3;
  else if(element == Element::ELECTRIC)
    return 4;
  else if(element == Element::DIGITAL)
    return 5;
  else if(element == Element::NIHIL)
    return 6;
  else if(element == Element::NONE)
    return 7;
  return 0;
}

/*
 * Description: Elemental string form of an enumerated element.
 *
 * Inputs: Element - enumerated form of element.
 * Output: std::string - string form of the element
 */
std::string Helpers::elementToString(const Element& element)
{
  if(element == Element::PHYSICAL)
    return "PHYSICAL";
  else if(element == Element::FIRE)
    return "FIRE";
  else if(element == Element::FOREST)
    return "FOREST";
  else if(element == Element::ICE)
    return "ICE";
  else if(element == Element::ELECTRIC)
    return "ELECTRIC";
  else if(element == Element::DIGITAL)
    return "DIGITAL";
  else if(element == Element::NIHIL)
    return "VOID";

  return "";
}

// // TODO: Comment
// std::string Helpers::eventToStr(EventType event)
// {
//   if(event == EventType::ITEM_USE)
//     return "ITEM_USE";
//   if(event == EventType::SKILL_USE)
//     return "SKILL_USE";
//   if(event == EventType::SKILL_USE_FIZZLE)
//     return "SKILL_USE_FIZZLE";
//   if(event == EventType::SKILL_COOLDOWN)
//     return "SKILL_COOLDOWN";
//   if(event == EventType::IMPLODE)
//     return "IMPLODE";
//   if(event == EventType::INSPECT)
//     return "INSPECT";
//   if(event == EventType::ATTEMPT_RUN)
//     return "ATTEMPT_RUN";
//   if(event == EventType::SUCCEED_RUN)
//     return "SUCCEED_RUN";
//   if(event == EventType::FAIL_RUN)
//     return "FAIL_RUN";
//   if(event == EventType::MISS_TURN)
//     return "MISS_TURN";
//   if(event == EventType::PASS)
//     return "PASS";
//   if(event == EventType::SKILL_MISS)
//     return "SKILL_MISS";
//   if(event == EventType::ACTION_MISS)
//     return "ACTION_MISS";
//   if(event == EventType::BLIND_MISS)
//     return "BLIND_MISS";
//   if(event == EventType::DREAMSNARE_MISS)
//     return "DREAMSNARE_MISS";
//   if(event == EventType::FIZZLE)
//     return "FIZZLE";
//   if(event == EventType::STANDARD_DAMAGE)
//     return "STANDARD_DAMAGE";
//   if(event == EventType::CRITICAL_DAMAGE)
//     return "CRITICAL_DAMAGE";
//   if(event == EventType::POISON_DAMAGE)
//     return "POISON_DAMAGE";
//   if(event == EventType::BURN_DAMAGE)
//     return "BURN_DAMAGE";
//   if(event == EventType::HITBACK_DAMAGE)
//     return "HITBACK_DAMAGE";
//   if(event == EventType::METABOLIC_KILL)
//     return "METABOLIC_KILL";
//   if(event == EventType::METABOLIC_DAMAGE)
//     return "METABOLIC_DAMAGE";
//   if(event == EventType::DEATH_COUNTDOWN)
//     return "DEATH_COUNTDOWN";
//   if(event == EventType::BOND)
//     return "BOND";
//   if(event == EventType::BONDING)
//     return "BONDING";
//   if(event == EventType::BEGIN_DEFEND)
//     return "BEGIN_DEFEND";
//   if(event == EventType::BREAK_DEFEND)
//     return "BREAK_DEFEND";
//   if(event == EventType::PERSIST_DEFEND)
//     return "PERSIST_DEFEND";
//   if(event == EventType::BREAK_GUARD)
//     return "BREAK_GUARD";
//   if(event == EventType::BEGIN_GUARD)
//     return "BEGIN_GUARD";
//   if(event == EventType::PERSIST_GUARD)
//     return "PERSIST_GUARD";
//   if(event == EventType::BREAK_GUARD)
//     return "BREAK_GUARD";
//   if(event == EventType::FAIL_GUARD)
//     return "FAIL_GUARD";
//   if(event == EventType::DEATH)
//     return "DEATH";
//   if(event == EventType::INFLICTION)
//     return "INFLICTION";
//   if(event == EventType::CURE_INFLICTION)
//     return "CURE_INFLICTION";
//   if(event == EventType::ALTERATION)
//     return "ALTERATION";
//   if(event == EventType::ASSIGNMENT)
//     return "ASSIGNMENT";
//   if(event == EventType::REVIVAL)
//     return "REVIVAL";
//   if(event == EventType::HEAL_HEALTH)
//     return "HEAL_HEATLH";
//   if(event == EventType::REGEN_VITA)
//     return "REGEN_VITA";
//   if(event == EventType::HEAL_QD)
//     return "HEAL_QD";
//   if(event == EventType::REGEN_QTDR)
//     return "REGEN_QTDR";
//   if(event == EventType::ACTION_BEGIN)
//     return "ACTION_BEGIN";
//   if(event == EventType::PARTY_DEATH)
//     return "PARTY_DEATH";
//   if(event == EventType::INFLICTION_FIZZLE)
//     return "INFLICTION_FIZZLE";
//   if(event == EventType::DEATHTIMER_DEATH)
//     return "DEATHTIMER_DEATH";
//   if(event == EventType::EXP_GAIN)
//     return "EXP_GAIN";
//   if(event == EventType::EXP_LOSS)
//     return "EXP_LOSS";
//   if(event == EventType::LEVEL_UP)
//     return "LEVEL_UP";
//   if(event == EventType::EQUIP_EXP_GAIN)
//     return "EQUIP_EXP_GAIN";
//   if(event == EventType::BUBBY_LEVEL_UP)
//     return "BUBBY_LEVEL_UP";
//   if(event == EventType::GAIN_ITEM)
//     return "GAIN ITEM";
//   if(event == EventType::GAIN_MONEY)
//     return "GAIN MONEY";
//   if(event == EventType::ACTION_END)
//     return "ACTION END";
//   if(event == EventType::NONE)
//     return "NONE";

//   return "";
// }

/*
 * Description: Flushes some whitespace (or whatever) into the console window.
 *
 * Inputs: char c - character to put on each line, ex. ' '
 * Output: none
 */
void Helpers::flushConsole(const char& c)
{
  for(int i = 0; i < 100; i++)
    std::cout << c << std::endl;
}

// TODO - Comments
std::string Helpers::gameKeyToStr(const GameKey& game_key)
{
  if(game_key == GameKey::MOVE_LEFT)
    return "Left";
  if(game_key == GameKey::MOVE_RIGHT)
    return "Right";
  if(game_key == GameKey::MOVE_UP)
    return "Up";
  if(game_key == GameKey::MOVE_DOWN)
    return "Down";
  if(game_key == GameKey::MENU)
    return "Menu";
  if(game_key == GameKey::ACTION)
    return "Action";
  if(game_key == GameKey::CANCEL)
    return "Cancel";
  if(game_key == GameKey::RUN)
    return "Run";
  if(game_key == GameKey::DEBUG)
    return "Debug";

  return "None";
}

/*
 * Description: Determines vowel-ness. (See a_an fn)
 *
 * Inputs: char c - char to see if it is a vowel
 * Output: bool - true if the given char is a vowel
 */
bool Helpers::isVowel(const char& c)
{
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
          c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

/*
 * Description: Converts an integer to roman numeral form (ex. 6 -> VI)
 *
 * Inputs: int - the value of the integer to find roman numerals for
 * Output: std::string - the compiled roman numeral form for the value
 */
std::string Helpers::numToRoman(int value)
{
  struct roman_values
  {
    int numeral_value;
    std::string numeral;
  };

  std::vector<roman_values> values{
      {1000, "M"},
      {900, "CM"},
      {500, "D"},
      {400, "CD"},
      {100, "C"},
      {90, "XC"},
      {50, "L"},
      {40, "XL"},
      {10, "X"},
      {9, "IX"},
      {5, "V"},
      {4, "IV"},
      {1, "I"},
      {0, ""},
  };

  std::string result = "";

  for(auto it = begin(values); it != end(values); ++it)
  {
    while(value >= (*it).numeral_value && value > 0)
    {
      result += (*it).numeral;
      value -= (*it).numeral_value;
    }
  }

  return result;
}

/*
 * Description: Returns the enum form of an party type (string)
 *
 * Inputs: std::string tier - string representation of PartyType
 * Output: PartyType - the enumeration of the string
 */
PartyType Helpers::partyTypeFromStr(const std::string& type)
{
  std::string type_up = type;
  std::transform(type_up.begin(), type_up.end(), type_up.begin(), toupper);

  /* Parse */
  if(type_up == "FINAL_BOSS" || type_up == "FINAL BOSS")
    return PartyType::FINAL_BOSS;
  else if(type_up == "BOSS")
    return PartyType::BOSS;
  else if(type_up == "MINI_BOSS" || type_up == "MINI BOSS")
    return PartyType::MINI_BOSS;
  else if(type_up == "BEARACKS")
    return PartyType::BEARACKS;
  else if(type_up == "SLEUTH")
    return PartyType::SLEUTH;
  return PartyType::REGULAR_FOE;
}

/*
 * Description: Returns the string form of a party type (enum)
 *
 * Inputs: PartyType type - enum of type
 * Output: std::string - the string version of the enumeration
 */
std::string Helpers::partyTypeToStr(const PartyType& type)
{
  if(type == PartyType::SLEUTH)
    return "SLEUTH";
  else if(type == PartyType::BEARACKS)
    return "BEARACKS";
  else if(type == PartyType::MINI_BOSS)
    return "MINI_BOSS";
  else if(type == PartyType::BOSS)
    return "BOSS";
  else if(type == PartyType::FINAL_BOSS)
    return "FINAL_BOSS";
  return "REGULAR_FOE";
}

/*
 * Description: Returns the enum form of an regen rate (string)
 *
 * Inputs: std::string regen_rate - string representation of RegenRate
 * Output: RegenRate - the enumeration of the string
 */
RegenRate Helpers::regenRateFromStr(const std::string& regen_rate)
{
  std::string regen_up = regen_rate;
  std::transform(regen_up.begin(), regen_up.end(), regen_up.begin(), toupper);

  /* Parse */
  if(regen_up == "WEAK")
    return RegenRate::WEAK;
  else if(regen_up == "NORMAL")
    return RegenRate::NORMAL;
  else if(regen_up == "STRONG")
    return RegenRate::STRONG;
  else if(regen_up == "GRAND")
    return RegenRate::GRAND;
  return RegenRate::ZERO;
}

/*
 * Description: Returns the string form of an regen rate (enum)
 *
 * Inputs: RegenRate - enumerated type of RegenRate
 * Output: std::string - the string of the enumeration
 */
std::string Helpers::regenRateToStr(const RegenRate& regen_rate)
{
  if(regen_rate == RegenRate::ZERO)
    return "ZERO";
  else if(regen_rate == RegenRate::WEAK)
    return "WEAK";
  else if(regen_rate == RegenRate::NORMAL)
    return "NORMAL";
  else if(regen_rate == RegenRate::STRONG)
    return "STRONG";
  else if(regen_rate == RegenRate::GRAND)
    return "GRAND";

  return "";
}

/*
 * Description: Returns the enum form of an item tier (string)
 *
 * Inputs: std::string tier - string representation of ItemTier
 * Output: ItemTier - the enumeration of the string
 */
ItemTier Helpers::tierFromStr(const std::string& tier)
{
  std::string tier_up = tier;
  std::transform(tier_up.begin(), tier_up.end(), tier_up.begin(), toupper);

  /* Parse */
  if(tier_up == "TRASH")
    return ItemTier::TRASH;
  else if(tier_up == "COMMON")
    return ItemTier::COMMON;
  else if(tier_up == "UNCOMMON")
    return ItemTier::UNCOMMON;
  else if(tier_up == "RARE")
    return ItemTier::RARE;
  else if(tier_up == "VERYRARE")
    return ItemTier::VERYRARE;
  else if(tier_up == "LEGENDARY")
    return ItemTier::LEGENDARY;
  return ItemTier::NONE;
}

/*
 * Description: Returns the string form of an item tier (enum)
 *
 * Inputs: ItemTier tier - enum representation of the tier
 * Output: std::string - the enumeration to a string
 */
std::string Helpers::tierToStr(const ItemTier& tier)
{
  if(tier == ItemTier::TRASH)
    return "TRASH";
  else if(tier == ItemTier::COMMON)
    return "COMMON";
  else if(tier == ItemTier::UNCOMMON)
    return "UNCOMMON";
  else if(tier == ItemTier::RARE)
    return "RARE";
  else if(tier == ItemTier::VERYRARE)
    return "VERYRARE";
  else if(tier == ItemTier::LEGENDARY)
    return "LEGENDARY";
  return "";
}

std::string Helpers::turnStateToStr(const TurnState& turn_state)
{
  if(turn_state == TurnState::BEGIN)
    return "BEGIN";
  if(turn_state == TurnState::GENERAL_UPKEEP)
    return "GENERAL_UPKEEP";
  if(turn_state == TurnState::UPKEEP)
    return "UPKEEP";
  if(turn_state == TurnState::SELECT_ACTION_ALLY)
    return "SELECT_ACTION_ALLY";
  if(turn_state == TurnState::SELECT_ACTION_ENEMY)
    return "SELECT_ACTION_ENEMY";
  if(turn_state == TurnState::PROCESS_ACTIONS)
    return "PROCESS_ACTIONS";
  if(turn_state == TurnState::CLEAN_UP)
    return "CLEAN_UP";
  if(turn_state == TurnState::OUTCOME)
    return "OUTCOME";
  if(turn_state == TurnState::STOPPED)
    return "STOPPED";

  return "FINISHED";
}

/*=============================================================================
 * PLAYER / BATTLE HELPER FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the pair of corresponding offensive/defensive attributes
 *              which related to a given enumerated element
 *
 * Inputs: element - Enumerated element type to find attributes for
 * Output: std::pair<Attr, Attr> - the corresponding off/def enumerated attrs.
 */
std::pair<Attribute, Attribute> Helpers::elementToStats(const Element& element)
{
  if(element == Element::PHYSICAL)
    return std::make_pair(Attribute::PHAG, Attribute::PHFD);
  else if(element == Element::FIRE)
    return std::make_pair(Attribute::THAG, Attribute::THFD);
  else if(element == Element::FOREST)
    return std::make_pair(Attribute::PRAG, Attribute::PRFD);
  else if(element == Element::ICE)
    return std::make_pair(Attribute::POAG, Attribute::POFD);
  else if(element == Element::ELECTRIC)
    return std::make_pair(Attribute::CHAG, Attribute::CHFD);
  else if(element == Element::DIGITAL)
    return std::make_pair(Attribute::CYAG, Attribute::CYFD);
  else if(element == Element::NIHIL)
    return std::make_pair(Attribute::NIAG, Attribute::NIFD);

  return std::make_pair(Attribute::NONE, Attribute::NONE);
}

/*
 * Description: Determines and returns the element which a given element is
 *              strong against.
 *
 * Inputs: element - enumeration of an Element to find a strength for
 * Output: Element - the enumeration of the Element's strength
 */
Element Helpers::getStrength(const Element& element)
{
  uint8_t strength = static_cast<uint8_t>(element);

  if(strength <= (1 >> 1))
    return Element::NONE;
  else if(strength == 1 >> 2)
    return Element::NIHIL;
  else
    strength = strength >> 1;

  return static_cast<Element>(strength);
}

/*
 * Description: Determines and returns the element which a given element is
 *              weak against.
 *
 * Inputs: element - enumeration of an Element to find a weakness for
 * Output: Element - the enumeration of the Element's weakness
 */
Element Helpers::getWeakness(const Element& element)
{
  uint8_t weakness = static_cast<uint8_t>(element);

  if(weakness <= (1 >> 1))
    return Element::NONE;
  else if(weakness == 1 >> 7)
    return Element::FIRE;
  else
    weakness = weakness >> 7;

  return static_cast<Element>(weakness);
}

/*=============================================================================
 * GENERAL HELPER FUNCTIONS
 *============================================================================*/

/*
 * Description: Build an exponentially increasing table of values given a
 *              starting value, a maximum value, and the number of iterations
 *              the values were progress for. This table will be returned as
 *              an std::vector<int>
 *
 * Inputs: const int &min - const ref int of the minimum value for the table
 *         const int &max - const ref int of the maximum value for the table
 *         const int &iter - const ref int of the # of iterations for the table
 * Output: std::vector<int> - the vector containing the constructed table
 */
std::vector<uint32_t> Helpers::buildExpTable(const uint32_t& min,
                                             const uint32_t& max,
                                             const uint32_t& iter)
{
  std::vector<uint32_t> table;

  auto b = log((double)max / min) / (iter - 1);
  auto a = (double)min / (exp(b) - 1.0);

  for(uint32_t i = 1; i <= iter; i++)
  {
    auto old_exp = round(a * exp(b * (i - 1)));
    auto new_exp = round(a * exp(b * i));
    table.push_back(new_exp - old_exp);
  }

  return table;
}

/*
 * Description: Combines a string and char together with toggle on before or
 *              after. This was implemented since the compiler seems to take
 *              a string literal plus a char and it becomes a char and does
 *              undefined behavior.
 *
 * Inputs: std::string s - the string to combine
 *         char c - the char to append or prepend
 *         bool before - true if the char is attached before the string
 * Output: std::string - the resulting string
 */
std::string Helpers::combine(std::string s, char c, bool before)
{
  std::string output = "";

  /* If char is before, add */
  if(before)
    output += c;

  /* Main string add */
  output += s;

  /* If char is after, add */
  if(!before)
    output += c;

  return output;
}

/*
 * Description: Takes a string in the designed format for sprite handling and
 *              separates it via the letter range.
 *
 * Example: std::string test = "Test01_[A-C][A-B]_U00.png";
 *          Vector becomes: AA BA CA
 *                          AB BB CB -> in the appropriate string format
 *
 * Inputs: const std::string path - the starting sprite path
 * Output: std::vector<std::vector<std::string>> - a matrix of paths (in X
 *         and Y direction). As shown in the example above.
 */
std::vector<std::vector<std::string>> Helpers::frameSeparator(std::string path)
{
  /* Split string on '[' */
  std::vector<std::string> first_split = split(path, '[');

  /* Parse each string and split on the ']' character */
  std::vector<std::string> split_strings;
  std::vector<uint32_t> range_ids;
  int range_count = 0; /* Max of 2 */
  for(uint32_t i = 0; i < first_split.size(); i++)
  {
    std::vector<std::string> second_split = split(first_split[i], ']');

    if(second_split.size() == 1 && second_split.front() == first_split[i])
    {
      split_strings.push_back(second_split.front());
    }
    else if(second_split.size() >= 1)
    {
      /* Check if the first string in the set is valid in the 'A-B' category */
      std::vector<std::string> range_split = split(second_split.front(), '-');
      if(range_count < 2 && range_split.size() == 2 &&
         range_split.front().size() == 1 && range_split.back().size() == 1 &&
         range_split.front().front() >= 'A' &&
         range_split.front().front() <= 'Z' &&
         range_split.back().front() >= 'A' &&
         range_split.back().front() <= 'Z') // &&
      // range_split.front().front() <= range_split.back().front())
      {
        /* String contains a range that's identical (Eg B-B) */
        if(range_split.front().front() == range_split.back().front())
        {
          split_strings.push_back(range_split.front());
        }
        /* String contains a correct range (Eg A-B) or (B-A) */
        else
        {
          split_strings.push_back(second_split.front());
          range_ids.push_back(split_strings.size() - 1);
          range_count++;
        }

        for(uint32_t j = 1; j < second_split.size(); j++)
          split_strings.push_back(second_split[j]);
      }
      /* String either contains an invalid range or no range */
      else
      {
        std::string combined_version = "";
        for(uint32_t j = 0; j < second_split.size(); j++)
          combined_version += second_split[j];
        split_strings.push_back(combined_version);
      }
    }
  }

  /* Create a linear list of all paths generated from the single path */
  uint32_t j = 0;
  uint32_t range1 = 0;
  uint32_t range2 = 0;
  std::vector<std::string> linear_set;
  for(uint32_t i = 0; i < split_strings.size(); i++)
  {
    /* Check if the parsing needs to handle a range */
    if(j < range_ids.size() && range_ids[j] == i)
    {
      uint32_t base_elements = linear_set.size();
      std::vector<std::string> range_str = split(split_strings[i], '-');
      int32_t new_elements =
          range_str.back().front() - range_str.front().front();
      uint32_t abs_elements = new_elements;
      if(new_elements < 0)
        abs_elements = 0 - new_elements;

      /* Store the number of elements in the appropriate variable */
      if(j == 0)
        range1 = abs_elements + 1;
      else
        range2 = abs_elements + 1;

      /* Duplicate enough elements for the new range */
      for(uint32_t k = 0; k < abs_elements; k++)
        for(uint32_t m = 0; m < base_elements; m++)
          linear_set.push_back(linear_set[m]);

      /* Now append the ranges for each on to the end */
      for(uint32_t k = 0; k < linear_set.size(); k++)
      {
        char element;
        if(new_elements > 0)
          element = range_str.front().front() + (k / base_elements);
        else
          element = range_str.front().front() - (k / base_elements);
        linear_set[k] += element;
      }

      j++;
    }
    /* ... or a single element */
    else
    {
      if(linear_set.size() == 0)
      {
        linear_set.push_back(split_strings[i]);
      }
      else
      {
        for(uint32_t k = 0; k < linear_set.size(); k++)
          linear_set[k].append(split_strings[i]);
      }
    }
  }

  /* Sort the linear list into a 2D array */
  std::vector<std::vector<std::string>> set;
  if(range1 > 0 && range2 > 0)
  {
    for(uint32_t i = 0; i < range1; i++)
    {
      std::vector<std::string> set_row;
      for(uint32_t j = 0; j < range2; j++)
        set_row.push_back(linear_set[i + range1 * j]);
      set.push_back(set_row);
    }
  }
  else if(range1 > 0)
  {
    /* Find the differential location */
    uint32_t index = 0;
    bool found = false;
    for(uint32_t i = 0; i < linear_set.front().size(); i++)
    {
      if(!found && linear_set.front()[i] != linear_set.back()[i])
      {
        index = i;
        found = true;
      }
    }

    /* Determine if it's x or y direction */
    bool x_dir = true;
    if(index > 0 && linear_set.front()[index] >= 'A' &&
       linear_set.front()[index - 1] <= 'Z')
      x_dir = false;

    /* Handle it accordingly */
    if(x_dir)
    {
      std::vector<std::string> set_row;
      for(uint32_t i = 0; i < linear_set.size(); i++)
      {
        set_row.push_back(linear_set[i]);
        set.push_back(set_row);
        set_row.clear();
      }
    }
    else
    {
      std::vector<std::string> set_row;
      for(uint32_t i = 0; i < linear_set.size(); i++)
        set_row.push_back(linear_set[i]);
      set.push_back(set_row);
    }
  }
  /* Otherwise, it's just a single element */
  else if(linear_set.size() > 0)
  {
    std::vector<std::string> set_row;
    set_row.push_back(linear_set.front());
    set.push_back(set_row);
  }

  return set;
}

/*
 * Description: Returns the render depth of the count of things that can be
 *              stacked on top of each other. Relevant for thing motion control
 *              and rendering.
 *
 * Inputs: none
 * Output: uint8_t - the render depth
 */
uint8_t Helpers::getRenderDepth()
{
  return kMAX_RENDER_DEPTH;
}

uint32_t Helpers::getDistance(Coordinate a, Coordinate b)
{
  auto x = a.x - b.x;
  auto y = a.y - b.y;

  auto distance = std::sqrt(float(x * x) + float(y * y));

  return std::round(distance);
}

/*
 * Description: Returns the tile size for rendering, as defined by the stored
 *              constants.
 *
 * Inputs: none
 * Output: uint16_t - the integer representing the pixel size of a tile
 */
uint16_t Helpers::getTileSize()
{
  return kTILE_SIZE;
}

/*
 * Description: Trims white space from the left side of a std::string
 *
 * Inputs: std::string &s - the string to trim whitespace from
 * Output: std::string& - the string with the whitespace removed
 */
std::string& Helpers::ltrim(std::string& s)
{
  s.erase(begin(s),
          std::find_if(begin(s), end(s),
                       std::not1(std::ptr_fun<int, int>(std::isspace))));

  return s;
}

/*
 * Description: Takes a string range in the format x1-x2,y1-y2 and parses to
 *              return the parts of it, in unsigned integer form. It can also
 *              handle the case where it's not a range but a single coordinate
 *              i.e. x1,y1-y2 or x1,y1
 *
 * Inputs: std::string sequence - the string sequence to parse
 *         uint32_t x_min, x_max, y_min, y_max - the range variables for the
                                 parsed data to return. Passed by reference.
 * Output: bool - status if the data is good and referenced vars are set
 */
bool Helpers::parseRange(std::string sequence, uint32_t& x_min, uint32_t& x_max,
                         uint32_t& y_min, uint32_t& y_max)
{
  uint32_t x_max_tmp = 0;
  uint32_t x_min_tmp = 0;
  uint32_t y_max_tmp = 0;
  uint32_t y_min_tmp = 0;
  bool good_data = false;

  /* Get the range of elements to process */
  std::vector<std::string> range_set = split(sequence, ',');
  if(range_set.size() == 2)
  {
    good_data = true;
    std::vector<std::string> x_set = split(range_set.front(), '-');
    std::vector<std::string> y_set = split(range_set.back(), '-');

    /* Convert the data over */
    int new_coord = atoi(x_set.front().c_str());
    if(new_coord < 0)
      good_data = false;
    x_min_tmp = new_coord;

    new_coord = atoi(x_set.back().c_str());
    if(new_coord < 0)
      good_data = false;
    x_max_tmp = new_coord;

    new_coord = atoi(y_set.front().c_str());
    if(new_coord < 0)
      good_data = false;
    y_min_tmp = new_coord;

    new_coord = atoi(y_set.back().c_str());
    if(new_coord < 0)
      good_data = false;
    y_max_tmp = new_coord;

    /* Check validity of range */
    if(y_min_tmp > y_max_tmp || x_min_tmp > x_max_tmp)
      good_data = false;
  }

  /* If data is good, proceed to set the outgoing pass by reference */
  if(good_data)
  {
    x_min = x_min_tmp;
    x_max = x_max_tmp;
    y_min = y_min_tmp;
    y_max = y_max_tmp;
  }

  return good_data;
}

/*
 * Description: Takes a comma delimited set of ranges and splits them into
 *              individual ranges, within a 2d vector.
 * Example: "12-14,10,6-2" would become 12 to 14, 10, 2 to 6 in a matrix
 *
 * Inputs: std::string sequence - the comma delimited sequence
 * Output: std::vector<std::vector<uint16_t>> - the matrix of ints parsed
 */
std::vector<std::vector<uint16_t>> Helpers::parseRangeSet(std::string sequence)
{
  std::vector<std::vector<uint16_t>> set_elements;

  /* Get the range of elements to process */
  std::vector<std::string> range_set = split(sequence, ',');
  for(uint32_t i = 0; i < range_set.size(); i++)
  {
    std::vector<std::string> range = split(range_set[i], '-');
    std::vector<uint16_t> row;

    /* If a range, push both values back */
    if(range.size() > 1)
    {
      uint16_t first = atoi(range.front().c_str());
      uint16_t second = atoi(range.back().c_str());

      /* Sort based on range */
      if(first > second)
      {
        row.push_back(second);
        row.push_back(first);
      }
      else
      {
        row.push_back(first);
        row.push_back(second);
      }
    }
    /* If only a single value, only add it */
    else if(range.size() == 1)
    {
      row.push_back(atoi(range.front().c_str()));
    }

    set_elements.push_back(row);
  }

  return set_elements;
}

/*
 * Description: Trims white space from the right side of a std::string
 *
 * Inputs: std::string &s - the string to trim whitespace from
 * Output: std::string& - the string with the whitespace removed
 */
std::string& Helpers::rtrim(std::string& s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
          end(s));

  return s;
}

/*
 * Description: A split function that works with a string line and a character
 *              delimiter to cut the string line into a set of tokens. This
 *              function returns empty strings if they are delimited.
 *
 * Example: std::string test = "Hello,sunshine,,what,a,beautiful,day";
 *          std::vector<std::string> temp_list = split(test, ',');
 *
 * Note: Does not give an empty final element.
 *
 * Inputs: const std::string &line - the line to split
 *         char delim - the character delimiter
 * Output: std::vector<std::string> - the split strings in array returned.
 */
std::vector<std::string> Helpers::split(const std::string& line, char delim)
{
  std::vector<std::string> elements;
  std::string item;
  std::stringstream ss(line);

  /* Parse the string and separate as per each delimiter */
  while(std::getline(ss, item, delim))
    elements.push_back(item);

  return elements;
}

/*
 * Description Trims white space from both sides of a std::string
 *
 * Inputs: std::string &s - the string to trim whitespace from
 * Output: std::string& - the string with the whitespace removed
 */
std::string& Helpers::trim(std::string& s)
{
  return ltrim(rtrim(s));
}

/*=============================================================================
 * GRAPHICAL HELPER FUNCTIONS
 *============================================================================*/

uint8_t Helpers::calcColorRed(SDL_Color color, uint8_t alpha)
{
  auto pc_fade = 0;

  if(color.a != 0)
    pc_fade = (alpha * 100) / color.a;

  float red_float = (pc_fade * color.r) / (float)100;

  return std::round(red_float);
}

uint8_t Helpers::calcColorBlue(SDL_Color color, uint8_t alpha)
{
  auto pc_fade = 0;

  if(color.a != 0)
    pc_fade = (alpha * 100) / color.a;

  float blue_float = (pc_fade * color.b) / (float)100;
  return std::round(blue_float);
}

uint8_t Helpers::calcColorGreen(SDL_Color color, uint8_t alpha)
{
  auto pc_fade = 0;

  if(color.a != 0)
    pc_fade = (alpha * 100) / color.a;

  float green_float = (pc_fade * color.g) / (float)100;
  return std::round(green_float);
}

/*
 * Description: Creates the black mask (only needs to be called once for the
 *              duration of the game) that allows the engine to manipulate
 *              visibility.
 *
 * Inputs: SDL_Renderer* renderer - the graphical rendering engine
 * Output: none
 */
void Helpers::createMaskBlack(SDL_Renderer* renderer)
{
  if(mask_black == nullptr)
  {
    mask_black =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, kTILE_SIZE, kTILE_SIZE);
    SDL_SetRenderTarget(renderer, mask_black);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderDrawRect(renderer, nullptr);
    SDL_SetRenderTarget(renderer, nullptr);

    /* Set the new texture as the black mask (with additive blending) */
    SDL_SetTextureBlendMode(mask_black, SDL_BLENDMODE_BLEND);
  }
}

/*
 * Description: Creates the white mask (only needs to be called once for the
 *              duration of the game) that allows the engine to manipulate
 *              brightness.
 *
 * Inputs: SDL_Renderer* renderer - the graphical rendering engine
 * Output: none
 */
void Helpers::createMaskWhite(SDL_Renderer* renderer)
{
  if(mask_white == nullptr)
  {
    mask_white =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, kTILE_SIZE, kTILE_SIZE);
    SDL_SetRenderTarget(renderer, mask_white);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderDrawRect(renderer, nullptr);
    SDL_SetRenderTarget(renderer, nullptr);

    /* Set the new texture as the white mask (with additive blending) */
    SDL_SetTextureBlendMode(mask_white, SDL_BLENDMODE_ADD);
  }
}

/*
 * Description: Deletes the white mask, created on an earlier step. Only
 *              needs to be done once upon the entire close of the application.
 *
 * Inputs: none
 * Output: none
 */
void Helpers::deleteMasks()
{
  if(mask_black != nullptr)
    SDL_DestroyTexture(mask_black);
  mask_black = nullptr;

  if(mask_white != nullptr)
    SDL_DestroyTexture(mask_white);
  mask_white = NULL;
}

/*
 * Description: Returns the black mask for rendering on screen. Do not delete
 *              this pointer.
 *
 * Inputs: none
 * Output: SDL_Texture* - the brightmap texture
 */
SDL_Texture* Helpers::getMaskBlack()
{
  return mask_black;
}

/*
 * Description: Returns the white mask for rendering on screen. Do not delete
 *              this pointer.
 *
 * Inputs: none
 * Output: SDL_Texture* - the brightmap texture
 */
SDL_Texture* Helpers::getMaskWhite()
{
  return mask_white;
}
