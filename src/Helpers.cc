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

  return (static_cast<uint32_t>(random) <= pc_chance);
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
 *         bool shortform - true for short form. Default true
 * Output: std::string - the converted string
 */
std::string Helpers::attributeToStr(Attribute attribute, bool shortform)
{
  if(shortform)
  {
    if(attribute == Attribute::VITA)
      return "VITA";
    else if(attribute == Attribute::QTDR)
      return "QTDR";
    else if(attribute == Attribute::PRAG)
      return "PRAG";
    else if(attribute == Attribute::PRFD)
      return "PRFD";
    else if(attribute == Attribute::SEAG)
      return "SEAG";
    else if(attribute == Attribute::SEFD)
      return "SEFD";
    else if(attribute == Attribute::LIMB)
      return "LIMB";
    else if(attribute == Attribute::UNBR)
      return "UNBR";
    else if(attribute == Attribute::WILL)
      return "WILL";
  }
  /* Else long form - description */
  else
  {
    if(attribute == Attribute::VITA)
      return "Vitality - hit points, when 0 character is incapacitated";
    else if(attribute == Attribute::QTDR)
      return "Quantum Drive - skill cost currency";
    else if(attribute == Attribute::PRAG)
      return "Primary Aggression - Primary Elemental attack";
    else if(attribute == Attribute::PRFD)
      return "Primary Fortitude - Primary Elemental defense";
    else if(attribute == Attribute::SEAG)
      return "Secondary Aggression - Secondary Elemental attack";
    else if(attribute == Attribute::SEFD)
      return "Secondary Fortitutde - Secondary Elemental defense";
    else if(attribute == Attribute::LIMB)
      return "Limbertude - dodge chance in avoiding attacks";
    else if(attribute == Attribute::UNBR)
      return "Unbearability - critical hit chance";
    else if(attribute == Attribute::WILL)
      return "Will - various luck effect on outcome of Battle events";
  }

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
      return "SERAPHIC";
    else if(curve == ElementCurve::S)
      return "DOMINANT";
    else if(curve == ElementCurve::A)
      return "PREVAILING";
    else if(curve == ElementCurve::B)
      return "STEADFAST";
    else if(curve == ElementCurve::C)
      return "TYPICAL";

    return "RESTRAINED";
  }
}

/*
 * Description: Direction enum form of an string direction.
 *
 * Inputs: std::string direction - string form of direction
 * Output: Direction - enum form of the direction
 */
Direction Helpers::directionFromString(const std::string& direction)
{
  std::string dir_up = direction;
  std::transform(dir_up.begin(), dir_up.end(), dir_up.begin(), toupper);

  /* Parse */
  if(dir_up == "NORTH")
    return Direction::NORTH;
  else if(dir_up == "EAST")
    return Direction::EAST;
  else if(dir_up == "SOUTH")
    return Direction::SOUTH;
  else if(dir_up == "WEST")
    return Direction::WEST;

  return Direction::DIRECTIONLESS;
}

/*
 * Description: Direction string form of an enumerated direction.
 *
 * Inputs: Direction direction - enumerated form of direction.
 * Output: std::string - string form of the direction
 */
std::string Helpers::directionToString(const Direction& direction)
{
  if(direction == Direction::NORTH)
    return "NORTH";
  else if(direction == Direction::EAST)
    return "EAST";
  else if(direction == Direction::SOUTH)
    return "SOUTH";
  else if(direction == Direction::WEST)
    return "WEST";

  return "";
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
std::string Helpers::elementToDisplayString(const Element& element)
{
  if(element == Element::PHYSICAL)
    return "Physical";
  else if(element == Element::FIRE)
    return "Thermal";
  else if(element == Element::FOREST)
    return "Primal";
  else if(element == Element::ICE)
    return "Polar";
  else if(element == Element::ELECTRIC)
    return "Charged";
  else if(element == Element::DIGITAL)
    return "Cybernetic";
  else if(element == Element::NIHIL)
    return "Nihil";

  return "";
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

/*
 * Description: Returns the string equivalent of a given MenuType enum.
 *
 * Inputs: const MenuType& menu_type - enumerated MenuType object
 * Output: std::string the string equivalent of the MenuType
 */
std::string Helpers::menuTypeToStr(const MenuType& menu_type)
{
  if(menu_type == MenuType::SLEUTH)
    return "Sleuth";
  if(menu_type == MenuType::SLEUTH_OVERVIEW)
    return "Overview";
  if(menu_type == MenuType::SLEUTH_EQUIPMENT)
    return "Equipment";
  if(menu_type == MenuType::SLEUTH_SKILLS)
    return "Skills";
  if(menu_type == MenuType::SLEUTH_DETAILS)
    return "Details";
  if(menu_type == MenuType::INVENTORY)
    return "Inventory";
  if(menu_type == MenuType::INVENTORY_BUBBIES)
    return "Bubbies";
  if(menu_type == MenuType::INVENTORY_EQUIP)
    return "Equipment";
  if(menu_type == MenuType::INVENTORY_ITEMS)
    return "Items";
  if(menu_type == MenuType::INVENTORY_KEY)
    return "Key Items";
  if(menu_type == MenuType::OPTIONS)
    return "Options";
  if(menu_type == MenuType::SAVE)
    return "Save";
  if(menu_type == MenuType::LOAD)
    return "Load";
  if(menu_type == MenuType::QUIT)
    return "Quit";
  if(menu_type == MenuType::TITLE_PLAYER_SELECT)
    return "New Game";
  if(menu_type == MenuType::TITLE_NEW_GAME)
    return "New Game";
  if(menu_type == MenuType::TITLE_LOAD_GAME)
    return "Load Game";
  if(menu_type == MenuType::TITLE_OPTIONS)
    return "Options";
  if(menu_type == MenuType::TITLE_OPTIONS_MAIN)
    return "Main";
  if(menu_type == MenuType::TITLE_OPTIONS_CONTROLS)
    return "Controls";
  if(menu_type == MenuType::TITLE_QUIT)
    return "Quit";
  if(menu_type == MenuType::INVALID)
    return "INVALID";

  return "";
}

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

/*
 * Description: Returns the string equivalent of a given GameKey enum.
 *
 * Inputs: const GameKey& game_key - enumerated GameKey object
 * Output: std::string the string equivalent of the GameKey
 */
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
  if(game_key == GameKey::ACTION)
    return "Action";
  if(game_key == GameKey::CANCEL)
    return "Cancel / Menu";
  if(game_key == GameKey::RUN)
    return "Run";
  if(game_key == GameKey::DEBUG)
    return "Debug";
  if(game_key == GameKey::PAUSE)
    return "Pause";

  return "";
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
      {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
      {90, "XC"},  {50, "L"},   {40, "XL"}, {10, "X"},   {9, "IX"},
      {5, "V"},    {4, "IV"},   {1, "I"},   {0, ""},
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
  if(type == PartyType::BEARACKS)
    return "BEARACKS";
  if(type == PartyType::MINI_BOSS)
    return "MINI_BOSS";
  if(type == PartyType::BOSS)
    return "BOSS";
  if(type == PartyType::FINAL_BOSS)
    return "FINAL_BOSS";

  return "REGULAR_FOE";
}

std::string Helpers::rankToDisplayStr(const Rank& rank_level)
{
  if(rank_level == Rank::NUBEAR)
    return "Nubear";
  if(rank_level == Rank::CUB)
    return "Cub";
  if(rank_level == Rank::RECRUIT)
    return "Recruit";
  if(rank_level == Rank::SLEUTHLING)
    return "Sleuthling";
  if(rank_level == Rank::SERGEANT)
    return "Sergeant";
  if(rank_level == Rank::SLEUTHMASTER)
    return "Sleuth Master";
  if(rank_level == Rank::OFFICER)
    return "Officer";
  if(rank_level == Rank::URSAMINOR)
    return "Ursa Minor";
  if(rank_level == Rank::ADMIRAL)
    return "Admiral";
  if(rank_level == Rank::URSAMAJOR)
    return "Ursa Major";
  if(rank_level == Rank::FOREBEAR)
    return "Forebear";
  if(rank_level == Rank::ALPHABEAR)
    return "Alpha Bear";
  if(rank_level == Rank::BOAT)
    return "Bear of Ancient Times";

  return "INVALID RANK";
}

std::string Helpers::rankToStr(const Rank& rank_level)
{
  if(rank_level == Rank::NUBEAR)
    return "NUBEAR";
  if(rank_level == Rank::CUB)
    return "CUB";
  if(rank_level == Rank::RECRUIT)
    return "RECRUIT";
  if(rank_level == Rank::SLEUTHLING)
    return "SLEUTHLING";
  if(rank_level == Rank::SERGEANT)
    return "SERGEANT";
  if(rank_level == Rank::SLEUTHMASTER)
    return "SLEUTHMASTER";
  if(rank_level == Rank::OFFICER)
    return "OFFICER";
  if(rank_level == Rank::URSAMINOR)
    return "URSA MINOR";
  if(rank_level == Rank::ADMIRAL)
    return "ADMIRAL";
  if(rank_level == Rank::URSAMAJOR)
    return "URSA MAJOR";
  if(rank_level == Rank::FOREBEAR)
    return "FOREBEAR";
  if(rank_level == Rank::ALPHABEAR)
    return "ALPHABEAR";
  if(rank_level == Rank::BOAT)
    return "BEAR OF ANCIENT TIMES";

  return "NONE";
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

std::string Helpers::sexToStr(const Sex& player_sex)
{
  if(player_sex == Sex::FEMALE)
    return "FEMALE";
  else if(player_sex == Sex::MALE)
    return "MALE";

  return "UNSET";
}

Sex Helpers::sexFromStr(const std::string& player_sex)
{
  if(player_sex == "FEMALE")
    return Sex::FEMALE;
  else if(player_sex == "MALE")
    return Sex::MALE;

  return Sex::UNSET;
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
  if(tier_up == "COMMON")
    return ItemTier::COMMON;
  if(tier_up == "UNCOMMON")
    return ItemTier::UNCOMMON;
  if(tier_up == "RARE")
    return ItemTier::RARE;
  if(tier_up == "VERYRARE")
    return ItemTier::VERYRARE;
  if(tier_up == "LEGENDARY")
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
  if(tier == ItemTier::COMMON)
    return "COMMON";
  if(tier == ItemTier::UNCOMMON)
    return "UNCOMMON";
  if(tier == ItemTier::RARE)
    return "RARE";
  if(tier == ItemTier::VERYRARE)
    return "VERYRARE";
  if(tier == ItemTier::LEGENDARY)
    return "LEGENDARYu";

  return "";
}

std::string Helpers::tierToDisplayStr(const ItemTier& tier)
{
  if(tier == ItemTier::TRASH)
    return "Trash";
  if(tier == ItemTier::COMMON)
    return "Common";
  if(tier == ItemTier::UNCOMMON)
    return "Uncommon";
  if(tier == ItemTier::RARE)
    return "Rare";
  if(tier == ItemTier::VERYRARE)
    return "Very Rare";
  if(tier == ItemTier::LEGENDARY)
    return "Legendary";

  return "";
}

std::string Helpers::titleCase(const std::string& str)
{
  std::string title_case;

  if(str.size())
  {
    for(uint32_t index = 0; index < str.size(); index++)
    {
      auto curr_char = str.at(index);

      if(index > 0)
      {
        if(str.at(index - 1) == ' ')
          curr_char = toupper(curr_char);
        else
          curr_char = tolower(curr_char);
      }
      else
        curr_char = toupper(curr_char);

      title_case.push_back(curr_char);
    }
  }

  return title_case;
}

/*
 * Description: Returns the enum form of a tracking state (string)
 *
 * Inputs: std::string track - string representation of the tracking state
 * Output: TrackingState - the enumeration from a string
 */
TrackingState Helpers::trackingFromStr(const std::string& track)
{
  std::string track_up = track;
  std::transform(track_up.begin(), track_up.end(), track_up.begin(), toupper);

  /* Parse */
  if(track_up == "AVOID THE PLAYER")
    return TrackingState::AVOIDPLAYER;
  else if(track_up == "TO THE PLAYER")
    return TrackingState::TOPLAYER;

  return TrackingState::NOTRACK;
}

/*
 * Description: Returns the string form of a tracking state (enum)
 *
 * Inputs: TrackingState track - enum representation of the tracking state
 * Output: std::string - the enumeration to a string
 */
std::string Helpers::trackingToStr(const TrackingState& track)
{
  if(track == TrackingState::AVOIDPLAYER)
    return "Avoid the Player";
  else if(track == TrackingState::TOPLAYER)
    return "To the Player";

  return "None";
}

/*
 * Description: Returns the enum form of a thing type (string)
 *
 * Inputs: std::string type - string representation of the thing type
 * Output: ThingBase - the enumeration from a string
 */
ThingBase Helpers::typeFromStr(const std::string& type)
{
  std::string type_up = type;
  std::transform(type_up.begin(), type_up.end(), type_up.begin(), toupper);

  /* Parse */
  if(type_up == "THING")
    return ThingBase::THING;
  else if(type_up == "ITEM")
    return ThingBase::ITEM;
  else if(type_up == "PERSON")
    return ThingBase::PERSON;
  else if(type_up == "NPC")
    return ThingBase::NPC;
  else if(type_up == "INTERACTIVE")
    return ThingBase::INTERACTIVE;

  return ThingBase::ISBASE;
}

/*
 * Description: Returns the string form of a thing type (enum)
 *
 * Inputs: ThingBase type - enum representation of the thing type
 * Output: std::string - the enumeration to a string
 */
std::string Helpers::typeToStr(const ThingBase& type)
{
  if(type == ThingBase::THING)
    return "Thing";
  else if(type == ThingBase::ITEM)
    return "Item";
  else if(type == ThingBase::PERSON)
    return "Person";
  else if(type == ThingBase::NPC)
    return "NPC";
  else if(type == ThingBase::INTERACTIVE)
    return "Interactive";

  return "None";
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::string Helpers::turnStateToStr(const TurnState& turn_state)
{
  if(turn_state == TurnState::BEGIN)
    return "BEGIN";
  else if(turn_state == TurnState::ENTER_DIM)
    return "ENTER_DIM";
  else if(turn_state == TurnState::FADE_IN_TEXT)
    return "FADE_IN_TEXT";
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

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::string Helpers::validStatusToStr(const ValidStatus& valid_status)
{
  if(valid_status == ValidStatus::VALID)
    return "VALID";
  if(valid_status == ValidStatus::NOT_AFFORDABLE)
    return "NOT_AFFORDABLE";
  if(valid_status == ValidStatus::SILENCED)
    return "SILENCED";
  if(valid_status == ValidStatus::NO_TARGETS)
    return "NO_TARGETS";

  return "INVALID";
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::string Helpers::victoryStateToStr(VictoryState victory_state)
{
  if(victory_state == VictoryState::DIM_BATTLE)
    return "DIM_BATTLE";
  if(victory_state == VictoryState::SLIDE_IN_LOOT)
    return "SLIDE_IN_LOOT";
  if(victory_state == VictoryState::SLIDE_IN_CARD)
    return "SLIDE_IN_CARD";
  if(victory_state == VictoryState::SLIDE_OUT_CARD)
    return "SLIDE_OUT_CARD";
  if(victory_state == VictoryState::SLIDE_OUT_LOOT)
    return "SLIDE_OUT_LOOT";
  if(victory_state == VictoryState::FADE_OUT)
    return "FADE_OUT";
  if(victory_state == VictoryState::FINISHED)
    return "FINISHED";

  return "";
}

/*=============================================================================
 * PLAYER / BATTLE HELPER FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the pair of corresponding offensive/defensive
 *attributes
 *              which related to a given enumerated element
 *
 * Inputs: element - Enumerated element type to find attributes for
 * Output: std::pair<Attr, Attr> - the corresponding off/def enumerated attrs.
 */
std::pair<Attribute, Attribute>
Helpers::elementTypeToStats(const ElementType& element)
{
  if(element == ElementType::PRIMARY)
    return std::make_pair(Attribute::PRAG, Attribute::PRFD);
  else if(element == ElementType::SECONDARY)
    return std::make_pair(Attribute::SEAG, Attribute::SEFD);

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

std::string Helpers::alignRight(std::string s, uint32_t length)
{
  if(s.length() < length)
  {
    auto spaces_to_add = length - s.length();

    for(uint32_t i = 0; i < spaces_to_add; i++)
      s = " " + s;
  }

  return s;
}

/*
 * Description: Build an exponentially increasing table of values given a
 *              starting value, a maximum value, and the number of iterations
 *              the values were progress for. This table will be returned as
 *              an std::vector<int>
 *
 * Inputs: const int &min - const ref int of the minimum value for the table
 *         const int &max - const ref int of the maximum value for the table
 *         const int &iter - const ref int of the # of iterations for the
 *table
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
    /* Corner case if min and max are equivalent */
    if(min == max)
    {
      table.push_back(min);
    }
    /* All other cases */
    else
    {
      auto old_exp = round(a * exp(b * (i - 1)));
      auto new_exp = round(a * exp(b * i));
      table.push_back(new_exp - old_exp);
    }
  }

  return table;
}

/*
 * Description: Takes a hex color, in the form of aabbcc or aabbccdd, and
 *              returns the integers within the SDL_Color set. The format is
 *              {rr}{gg}{bb}{aa} for red, green, blue, alpha
 *
 * Inputs: std::string hex_color - the color in base 16 (hex)
 * Output: SDL_Color - the returned integer color struct
 */
SDL_Color Helpers::colorFromHex(std::string hex_color)
{
  SDL_Color color = {255, 255, 255, 255};
  if(hex_color.size() >= 6)
  {
    std::string red_str = "";
    red_str.push_back(hex_color.at(0));
    red_str.push_back(hex_color.at(1));
    color.r = hexToBaseTen(red_str);

    std::string green_str = "";
    green_str.push_back(hex_color.at(2));
    green_str.push_back(hex_color.at(3));
    color.g = hexToBaseTen(green_str);

    std::string blue_str = "";
    blue_str.push_back(hex_color.at(4));
    blue_str.push_back(hex_color.at(5));
    color.b = hexToBaseTen(blue_str);

    if(hex_color.size() >= 8)
    {
      std::string alpha_str = "";
      alpha_str.push_back(hex_color.at(6));
      alpha_str.push_back(hex_color.at(7));
      color.a = hexToBaseTen(alpha_str);
    }
  }

  return color;
}

/*
 * Description: Takes the color structure and returns it in the form of aabbcc
 *              or aabbccdd, depending on 'alpha' input, in the hex format
 *
 * Inputs: SDL_Color color - the color structure integer of information
 *         bool alpha - include the alpha. If false, will be 6 digits. else 8
 * Output: std::string - the color in base 16 (hex)
 */
std::string Helpers::colorToHex(SDL_Color color, bool alpha)
{
  std::string result = hexFromBaseTen(color.r) + hexFromBaseTen(color.g) +
                       hexFromBaseTen(color.b);
  if(alpha)
    result += hexFromBaseTen(color.a);

  return result;
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
 * Description: Creates a blank lay over structure with cleared data.
 *
 * Inputs: none
 * Output: LayOver - the lay coverage structure defining the sprite
 */
LayOver Helpers::createBlankLayOver()
{
  LayOver new_layover;
  new_layover.path = "";
  new_layover.anim_time = 250;
  new_layover.velocity_x = 0.0;
  new_layover.velocity_y = 0.0;
  new_layover.player_relative = false;

  return new_layover;
}

/*
 * Description: Creates a blank battle scene structure with cleared data.
 *
 * Inputs: none
 * Output: BattleScene - the scene structure defining pertinent information
 */
BattleScene Helpers::createBlankScene()
{
  BattleScene scene;
  scene.id = -1;
  scene.background = "";
  scene.music_id = -1;
  scene.underlays.clear();
  scene.midlays.clear();
  scene.overlays.clear();

  return scene;
}

std::string Helpers::formatUInt(uint32_t i, std::string d)
{
  std::string formatted_number = std::to_string(i);
  int32_t insertPosition = formatted_number.length() - 3;
  while(insertPosition > 0)
  {
    formatted_number.insert(insertPosition, d);
    insertPosition -= 3;
  }

  return formatted_number;
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
      /* Check if the first string in the set is valid in the 'A-B' category
       */
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
 * Description: Returns the distance between two coordinates
 *
 * Inputs: Coordinate a - first  coordinate
 *         Coordinate b - second coordinate
 * Output; uint32_t - distance between the two points
 */
uint32_t Helpers::getDistance(Coordinate a, Coordinate b)
{
  auto x = a.x - b.x;
  auto y = a.y - b.y;

  auto distance = std::sqrt(float(x * x) + float(y * y));

  return std::round(distance);
}

/*
 * Description: Returns the parent directory from a file name path. For example,
 * "path/to/file/data.txt" will return "path/to/file/". It expects at least one "/" in the path.
 *
 * Inputs: std::string file_name - full file name path
 * Output: std::string - returned parent directory path
 */
std::string Helpers::getParentDirectory(std::string file_name)
{
  return file_name.substr(0, file_name.find_last_of("/\\") + 1);
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
 * Description: Takes a base ten unsigned integer and converts it to a string
 *              base sixteen representation.
 *
 * Inputs: uint32_t base_ten - the base 10 number to convert
 * Output: std::string - the resulting base 16 num
 */
std::string Helpers::hexFromBaseTen(uint32_t base_ten)
{
  std::stringstream stream;
  stream << std::hex << base_ten;

  return stream.str();
}

/*
 * Description: Takes a base sixteen string number and converts it to an
 *              unsigned base ten representation.
 *
 * Inputs: std::string base_sixteen - the base 16 number to convert
 * Output: uint32_t - the resulting base 10 num
 */
uint32_t Helpers::hexToBaseTen(std::string base_sixteen)
{
  uint32_t value = 0;
  try
  {
    value = std::stoul(base_sixteen, nullptr, 16);
  }
  catch(std::exception)
  {
  }

  return value;
}

/*
 * Description: Trims white space from the left side of a std::string
 *
 * Inputs: std::string s - the string to trim whitespace from
 * Output: std::string - the string with the whitespace removed
 */
std::string Helpers::ltrim(std::string s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
    return !std::isspace(ch);
  }));

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
 * Inputs: std::string s - the string to trim whitespace from
 * Output: std::string - the string with the whitespace removed
 */
std::string Helpers::rtrim(std::string s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch);
  }).base(), s.end());

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
 * Inputs: std::string s - the string to trim whitespace from
 * Output: std::string - the string with the whitespace removed
 */
std::string Helpers::trim(std::string s)
{
  return ltrim(rtrim(s));
}

/*
 * Description: Updates the lay over structure passed in with the load data
 *              as defined by XmlData with the associated index
 *
 * Inputs: LayOver lay_over - the lay over to modify with the new data
 *         XmlData data - the load data element
 *         int file_index - the current index within the data set
 *         std::string base_directory - the root path for loaded asset data
 * Output: LayOver - the updated lay over with the load data. No changes if
 *the
 *                   load failed.
 */
LayOver Helpers::updateLayOver(LayOver lay_over, XmlData data, int file_index,
                               std::string base_directory)
{
  bool success = true;
  LayOver new_layover = lay_over;

  /* Modify */
  std::string element3 = data.getElement(file_index);
  if(element3 == "animation")
    new_layover.anim_time = data.getDataInteger(&success);
  else if(element3 == "path")
    new_layover.path = base_directory + data.getDataString(&success);
  else if(element3 == "player")
    new_layover.player_relative = data.getDataBool(&success);
  else if(element3 == "velx")
    new_layover.velocity_x = data.getDataFloat(&success);
  else if(element3 == "vely")
    new_layover.velocity_y = data.getDataFloat(&success);

  /* Return the appropriate set */
  if(success)
    return new_layover;

  return lay_over;
}

/*
 * Description: Updates the battle scene structure passed in with the load
 *data
 *              as defined by XmlData with the associated index
 *
 * Inputs: BattleScene scene - the battle scene to modify with the new data
 *         XmlData data - the load data element
 *         int file_index - the current index within the data set
 *         std::string base_directory - the root path for loaded asset data
 * Output: BattleScene - the updated scene with the load data. No changes if
 *                       the load failed.
 */
BattleScene Helpers::updateScene(BattleScene scene, XmlData data,
                                 int file_index, std::string base_directory)
{
  bool success = true;
  BattleScene new_scene = scene;

  /* Modify */
  std::string element = data.getElement(file_index);
  /* -- BACKGROUND PATH -- */
  if(element == "background")
  {
    new_scene.background = base_directory + data.getDataString(&success);
  }
  /* -- MUSIC INTEGER -- */
  else if(element == "music")
  {
    new_scene.music_id = data.getDataInteger(&success);
  }
  /* -- UNDERLAY/MIDLAY/OVERLAY INFO -- */
  else if(element == "underlay" || element == "midlay" || element == "overlay")
  {
    /* Get index */
    int index_ref = -1;
    std::string index_str = data.getKeyValue(file_index);
    if(!index_str.empty())
      index_ref = std::stoi(index_str);

    /* Proceed if index is valid */
    if(index_ref >= 0)
    {
      /* Get referenced layer */
      LayOver* lay_ref = nullptr;
      if(element == "underlay")
      {
        while(static_cast<int>(new_scene.underlays.size()) <= index_ref)
          new_scene.underlays.push_back(createBlankLayOver());
        lay_ref = &new_scene.underlays[index_ref];
      }
      else if(element == "midlay")
      {
        while(static_cast<int>(new_scene.midlays.size()) <= index_ref)
          new_scene.midlays.push_back(createBlankLayOver());
        lay_ref = &new_scene.midlays[index_ref];
      }
      else /* overlay */
      {
        while(static_cast<int>(new_scene.overlays.size()) <= index_ref)
          new_scene.overlays.push_back(createBlankLayOver());
        lay_ref = &new_scene.overlays[index_ref];
      }

      /* Modify referenced layer */
      *lay_ref = updateLayOver(*lay_ref, data, file_index + 1, base_directory);
    }
  }

  /* Return the appropriate set */
  return success ? new_scene : scene;
}

/*=============================================================================
 * GRAPHICAL HELPER FUNCTIONS
 *============================================================================*/

/*
 * Description: Given a starting and ending coordinate, use Bresenham's
 *              algorithm to determine which pixels should be rendered between
 *              the two coordinates. This uses the slope of the line and an
 *              updating error margin (floating math version).
 *
 * Inputs: Coordinate start - starting point of the line (X, Y)
 *         Coordinate end - ending point of the line (X, Y)
 * Output: std::vector<Coordinate> - coordinates of line to render
 */
std::vector<Coordinate> Helpers::bresenhamPoints(Coordinate begin,
                                                 Coordinate end)
{
  /* The determined Bresenham points vector */
  std::vector<Coordinate> bresenham_points;

  /* Get the actual starting and ending coordinates */
  auto x1 = begin.x;
  auto x2 = end.x;
  auto y1 = begin.y;
  auto y2 = end.y;

  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));

  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);

  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;

  const int max_x = (int)x2;

  for(int x = (int)x1; x <= max_x; x++)
  {
    if(steep)
      bresenham_points.push_back({y, x});
    else
      bresenham_points.push_back({x, y});

    error -= dy;
    if(error < 0)
    {
      y += ystep;
      error += dx;
    }
  }

  return bresenham_points;
}

/*
 * Description: Update the position of a value given its current position and
 *a
 *              velocity and cycle_time, towards its end position, by at least
 *              one unit per function call.
 *
 * Inputs: int32_t cycle_time - the given time for a cycle
 *         int32_t curr_pos - the current position of the object
 *         int32_t end_pos - the ending position of the object
 *         float velocity - the velocity of the object
 * Output:
 */
int32_t Helpers::updatePosition(int32_t cycle_time, int32_t curr_pos,
                                int32_t end_pos, float velocity)
{
  auto distance = std::abs(curr_pos - end_pos);

  if(distance == 1)
  {
    curr_pos = end_pos;
  }
  else
  {
    auto travel = std::min(cycle_time * velocity, (float)distance);

    if(travel > 1 && end_pos < curr_pos)
      curr_pos -= travel;
    else if(travel > 1 && end_pos > curr_pos)
      curr_pos += travel;
    else if(end_pos < curr_pos)
      curr_pos -= 1;
    else if(end_pos > curr_pos)
      curr_pos += 1;
  }

  return curr_pos;
}

/*
 * Description: Updates an (X, Y) coordinate given a cycle time, starting and
 *              ending coordinates and a velocity (in units / ms). This
 *function
 *              will update the object by at least one pixel.
 *
 * Inputs: int32_t cycle_time - given time of the cycle to update object on
 *         Coordinate current - the current position of the object
 *         Coordinate final - the final resting position of the object
 *         float velocity - movement of the object in pixels / ms.
 * Output:
 */
Coordinate Helpers::updateCoordinate(int32_t cycle_time, Coordinate current,
                                     Coordinate final, float velocity)
{
  if(current.x != final.x)
    current.x = updatePosition(cycle_time, current.x, final.x, velocity);
  if(current.y != final.y)
    current.y = updatePosition(cycle_time, current.y, final.y, velocity);

  return current;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
float Helpers::updateHoverBrightness(int32_t time_elapsed, float cycle_rate,
                                     float min_value, float max_value)
{
  auto factor = max_value - min_value;

  return fabs(factor * sin((float)time_elapsed * cycle_rate)) + min_value;
}

/*
 * Description: Calculates and returns a new alpha value for fading in,
 *              determined from a given cycle time as a portion of a fade in
 *              time. This function will increment the alpha fade in by at
 *least
 *              one point.
 *
 * Inputs: int32_t cycle_time - time for the cycle
 *         uint8_t alpha - current alpha value for the object fading in
 *         uint32_t fade_time - total time for the object to fade in
 *         uint8_t alpha_max - maximum alpha achievable
 * Output: uint8_t - the new calculated alpha value
 */
uint8_t Helpers::calcAlphaFadeIn(int32_t cycle_time, uint8_t alpha,
                                 uint32_t fade_time, uint8_t alpha_max)
{
  float delta_a = (float)alpha_max / fade_time * cycle_time;
  delta_a = std::max((float)1, delta_a);

  if(alpha + delta_a > alpha_max)
    return alpha_max;

  return alpha + delta_a;
}

/*
 * Description: Calculates and returns a new alpha value for an object fading
 *              out. Determined by using cycle time as a portion of a total
 *              fade in time. Will decrement the fade in until a minimum alpha
 *              opacity is reached, by at least 1 alpha point per call.
 *
 * Inputs: int32_t cycle_time - time for the cycle
 *         uint8_t alpha - current alpha value for the object fading out
 *         uint32_t fade_time - total time for the object fade out
 *         uint8_t alpha_min - minimum alpha achievable
 * Output: uint8_t - the new calculated alpha value
 */
uint8_t Helpers::calcAlphaFadeOut(int32_t cycle_time, uint8_t alpha,
                                  uint32_t fade_time, uint8_t alpha_min)
{
  float delta_a = (float)alpha / fade_time * cycle_time;
  delta_a = std::max((float)1, delta_a);

  if(delta_a > alpha)
    return 0;
  if(alpha - delta_a >= alpha_min)
    return alpha - delta_a;

  return alpha_min;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint8_t Helpers::calcColorRed(SDL_Color color, uint8_t alpha)
{
  auto pc_fade = 0;

  if(color.a != 0)
    pc_fade = (alpha * 100) / color.a;

  float red_float = (pc_fade * color.r) / (float)100;

  return std::round(red_float);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint8_t Helpers::calcColorBlue(SDL_Color color, uint8_t alpha)
{
  auto pc_fade = 0;

  if(color.a != 0)
    pc_fade = (alpha * 100) / color.a;

  float blue_float = (pc_fade * color.b) / (float)100;

  return std::round(blue_float);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
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
 *              needs to be done once upon the entire close of the
 *application.
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
 * Description: Given a coordinate to check, and it's top-left and bot-right
 *              as bounding box, returns whether the check coordinate is within
 *              the bounding box
 *
 * Inputs: Coordinate test - check coordinate
 *         Coordinate top_l - top left of bounding box
 *         Coordinate bot_r - bottom right of bounding box
 * Output: bool - true if check coordinate is within bounding box
 */
bool Helpers::isWithinRange(Coordinate test, Coordinate top_l, Coordinate bot_r)
{
  return (top_l.x <= test.x && test.x <= bot_r.x) &&
         (top_l.y <= test.y && test.y <= bot_r.y);
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
