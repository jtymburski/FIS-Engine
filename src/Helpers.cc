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

SDL_Texture* Helpers::white_mask = NULL;

/*=============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *============================================================================*/

const unsigned int Helpers::seed_original = 
                    std::chrono::system_clock::now().time_since_epoch().count();

std::mt19937    Helpers::rand_eng(seed_original);
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
bool Helpers::chanceHappens(const uint32_t &pc_chance, const uint32_t &mag)
{
  if (pc_chance >= mag)
    return true;
  if (pc_chance <= 0)
    return false;

  auto random = randU(1, static_cast<int>(mag));

  if (static_cast<uint32_t>(random) <= pc_chance) 
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
float Helpers::randFloat(const float &a, const float &b)
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
int Helpers::randInt(const int &max)
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
int Helpers::randU(const int &a, const int &b)
{
  auto min = std::min(a, b);
  auto max = std::max(a, b);

  std::uniform_int_distribution<int> distribution(min, max);
  
  return distribution(rand_eng);
}

/*
 * Description: Generates a random 32-bit integer using std::mt1993 and returns
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
int Helpers::rollXS(const int &x_sides, const int &s_times)
{
  auto total = 0;

  for (auto i = 0; i < s_times; i++)
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
std::string Helpers::a_An(const std::string &noun)
{
  if (noun.size() == 0)
    return "";

  auto a_an        = "a";
  const auto lower = noun;

  for (auto c : lower)
    c = tolower(c);

  const auto &initial = noun.at(0);

  /* General vowel vs. consonant rule ("u" is usually consonant) */
  if (lower.size() == 1)
  {
    if (isVowel(initial) && initial != 'u')
      a_an = "an";
    a_an = "a";
  }

  /* Generally words that start with vowels are "an" */
  if (isVowel(initial))
    a_an = "an";

  if ((initial == 'y') && !isVowel(lower.at(1)))
    a_an = "a";

  /* Some exceptions for consonant and vowel sounds (e.g. "a eulogy") */
  if (lower.substr(0, 2) == "eu" || lower.substr(0, 3) == "one" || 
      lower.substr(0, 3) == "uni")
    a_an = "a";
  else if (lower.substr(0, 3) == "hon")
    a_an = "an";

  return a_an;
}

/*
 * Description: Flushes some whitespace (or whatever) into the console window.
 *
 * Inputs: char c - character to put on each line, ex. ' '
 * Output: none
 */
void Helpers::flushConsole(const char &c)
{
  for (int i = 0; i < 100; i++)
    std::cout << c << std::endl;
}

/*
 * Description: Determines vowel-ness. (See a_an fn)
 *
 * Inputs: char c - char to see if it is a vowel
 * Output: bool - true if the given char is a vowel
 */
bool Helpers::isVowel(const char &c)
{
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
          c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

/*
 * Description: Elemental string form of an enumerated element.
 *
 * Inputs: Element - enumerated form of element.
 * Output: std::string - string form of the element
 */
std::string Helpers::elementToString(const Element &element)
{
  if (element == Element::PHYSICAL)
    return "PHYSICIAL";
  else if (element == Element::FIRE)
    return "FIRE";
  else if (element == Element::FOREST)
    return "FOREST";
  else if (element == Element::ICE)
    return "ICE";
  else if (element == Element::ELECTRIC)
    return "ELECTRIC";
  else if (element == Element::DIGITAL)
    return "DIGITAL";
  else if (element == Element::NIHIL)
    return "VOID";
  
  return "";
}

/*
 * Description: Convert enumerated cell type to string
 *
 * Inputs: CellState - enumerated state of a cell in signature
 * Output: std::string - string form of the given enumeration
 */
std::string Helpers::cellToStr(const CellState &cell_state)
{
  if (cell_state == CellState::OPEN)
    return "OPEN";
  else if (cell_state == CellState::BUBBY)
    return "BUBBY";
  else if (cell_state == CellState::CLOSED)
    return "CLOSED";
  else if (cell_state == CellState::BLOCKED)
    return "BLOCKED";
  else if (cell_state == CellState::LINK)
    return "LINK";
  
  return "";
}

/*
 * Description: Returns string names of enumerated action types
 *
 * Inputs: ActionType - enumerated action type to find string for
 * Output: std::string - the string of the corresponding enumeration
 */
std::string Helpers::actionTypeToStr(const ActionType &action_type)
{
  if (action_type == ActionType::SKILL)
    return "SKILL";
  else if (action_type == ActionType::ITEM)
    return "ITEM";
  else if (action_type == ActionType::DEFEND)
    return "DEFEND";
  else if (action_type == ActionType::GUARD)
    return "GUARD";
  else if (action_type == ActionType::IMPLODE)
    return "IMPLODE";
  else if (action_type == ActionType::RUN)
    return "RUN";
  else if (action_type == ActionType::PASS)
    return "PASS";
  else if (action_type == ActionType::NONE)
    return "NONE";

  return "";
}

/*
 * Description: Returns the string form a given ActionScope enum
 *
 * Inputs: ActionScope - enum of scope of actions for Skills
 * Output: std::string - string corresponding to the ActionScope
 */
std::string Helpers::actionScopeToStr(const ActionScope &action_scope)
{
  if (action_scope == ActionScope::USER)
    return "USER";
  else if (action_scope == ActionScope::ONE_TARGET)
    return "ONE_TARGET";
  else if (action_scope == ActionScope::ONE_ENEMY)
    return "ONE_ENEMY";
  else if (action_scope == ActionScope::TWO_ENEMIES)
    return "TWO_ENEMIES";
  else if (action_scope == ActionScope::ALL_ENEMIES)
    return "ALL_ENEMIES";
  else if (action_scope == ActionScope::ONE_ALLY)
    return "ONE_ALLY";
  else if (action_scope == ActionScope::ONE_ALLY_NOT_USER)
    return "ONE_ALLY_NOT_USER";
  else if (action_scope == ActionScope::TWO_ALLIES)
    return "TWO_ALLIES";
  else if (action_scope == ActionScope::ALL_ALLIES)
    return "ALL_ALLIES";
  else if (action_scope == ActionScope::ONE_ALLY_KO)
    return "ONE_ALLY_KO";
  else if (action_scope == ActionScope::ALL_ALLIES_KO)
    return "ALL_ALLIES_KO";
  else if (action_scope == ActionScope::ONE_PARTY)
    return "ONE_PARTY";
  else if (action_scope == ActionScope::ALL_TARGETS)
    return "ALL_TARGETS";
  else if (action_scope == ActionScope::NOT_USER)
    return "NOT_USER";
  else if (action_scope == ActionScope::ALL_NOT_USER)
    return "ALL_NOT_USER";
  else if (action_scope == ActionScope::NO_SCOPE)
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
std::string Helpers::aiDifficultyToStr(const AIDifficulty &ai_difficulty)
{
  if (ai_difficulty == AIDifficulty::RANDOM)
    return "RANDOM";
  else if (ai_difficulty == AIDifficulty::PRIORITY)
    return "PRIORITY";
  else if (ai_difficulty == AIDifficulty::TACTICAL)
    return "TACTICAL";
  else if (ai_difficulty == AIDifficulty::DEEP_THOUGHT)
    return "DEEP_THOUGHT";

  return "";
}

/*
 * Description: Returns the string form of an AIPersonality enum.
 *
 * Inputs: AIPersonality - the enumerated personality type of an AI
 * Output: std::string - the corresponding string for the personality
 */
std::string Helpers::aiPersonalityToStr(const AIPersonality &ai_personality)
{
  if (ai_personality == AIPersonality::MODERATOR)
    return "MODERATOR";
  else if (ai_personality == AIPersonality::AGGRESSOR)
    return "AGGRESSOR";
  else if (ai_personality == AIPersonality::DEFENDER)
    return "DEFENDER";
  else if (ai_personality == AIPersonality::PROTECTOR)
    return "PROTECTOR";
  else if (ai_personality == AIPersonality::RETALIATOR)
    return "RETALIATOR";
  else if (ai_personality == AIPersonality::MASOCHISTIC)
    return "MASOCHISTIC";
  else if (ai_personality == AIPersonality::ANNIHILATOR)
    return "ANNIHILATOR";
  else if (ai_personality == AIPersonality::RUNNER)
    return "RUNNER";
  else if (ai_personality == AIPersonality::PASSER)
    return "PASSER";
  else if (ai_personality == AIPersonality::NONE)
    return "NONE";

  return "";
}

/*
 * Description: Returns the string form of an ailment (Infliction enum)
 *
 * Inputs: Infliction - enumerated type of Infliction
 * Output: std::string - the string of the enumeration
 */
std::string Helpers::ailmentToString(const Infliction &ail)
{
  if (ail == Infliction::POISON)
    return "POISON";
  else if (ail == Infliction::BURN)
    return "BURN";
  else if (ail == Infliction::SCALD)
    return "SCALD";
  else if (ail == Infliction::CHARR)
    return "CHARR";
  else if (ail == Infliction::BERSERK)
    return "BERSERK";
  else if (ail == Infliction::CONFUSE)
    return "CONFUSE";
  else if (ail == Infliction::SILENCE)
    return "SILENCE";
  else if (ail == Infliction::BUBBIFY)
    return "BUBBIFY";
  else if (ail == Infliction::DEATHTIMER)
    return "DEATHTIMER";
  else if (ail == Infliction::PARALYSIS)
    return "PARALYSIS";
  else if (ail == Infliction::BLINDNESS)
    return "BLINDNESS";
  else if (ail == Infliction::DREADSTRUCK)
    return "DREADSTRUCK";
  else if (ail == Infliction::DREAMSNARE)
    return "DREAMSNARE";
  else if (ail == Infliction::HELLBOUND)
    return "HELLBOUND";
  else if (ail == Infliction::BOND)
    return "BOND";
  else if (ail == Infliction::BONDED)
    return "BONDED";
  else if (ail == Infliction::ALLATKBUFF)
    return "ALLATKBUFF";
  else if (ail == Infliction::ALLDEFBUFF)
    return "ALLDEFBUFF";
  else if (ail == Infliction::PHYBUFF)
    return "PHYBUFF";
  else if (ail == Infliction::THRBUFF)
    return "THRBUFF";
  else if (ail == Infliction::POLBUFF)
    return "POLBUFF";
  else if (ail == Infliction::PRIBUFF)
    return "PRIBUFF";
  else if (ail == Infliction::CHGBUFF)
    return "CHGBUFF";
  else if (ail == Infliction::CYBBUFF)
    return "CYBBUFF";
  else if (ail == Infliction::NIHBUFF)
    return "NIHBUFF";
  else if (ail == Infliction::LIMBUFF)
    return "LIMBUFF";
  else if (ail == Infliction::UNBBUFF)
    return "UNBBUFF";
  else if (ail == Infliction::MOMBUFF)
    return "MOMBUFF";
  else if (ail == Infliction::VITBUFF)
    return "VITBUFF";
  else if (ail == Infliction::QDBUFF)
    return "QDBUFF";
  else if (ail == Infliction::ROOTBOUND)
    return "ROOTBOUND";
  else if (ail == Infliction::DOUBLECAST)
    return "DOUBLECAST";
  else if (ail == Infliction::TRIPLECAST)
    return "TRIPLECAST";
  else if (ail == Infliction::HALFCOST)
    return "HALFCOST";
  else if (ail == Infliction::REFLECT)
    return "REFLECT";
  else if (ail == Infliction::HIBERNATION)
    return "HIBERNATION";
  else if (ail == Infliction::CURSE)
    return "CURSE";
  else if (ail == Infliction::METATETHER)
    return "METATETHER";
  else if (ail == Infliction::STUBULATE)
    return "STUBULATE";
  else if (ail == Infliction::INVALID)
    return "INVALID";

  return "";
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

  std::vector<roman_values> values
  {
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

  for (auto it = begin(values); it != end(values); ++it)
  {
    while (value >= (*it).numeral_value && value > 0)
    {
      result += (*it).numeral;
      value  -= (*it).numeral_value;
    }
  }

  return result;
}

std::string Helpers::regenRateToStr(const RegenRate &regen_rate)
{
  if (regen_rate == RegenRate::ZERO)
    return "ZERO";
  else if (regen_rate == RegenRate::WEAK)
    return "WEAK";
  else if (regen_rate == RegenRate::NORMAL)
    return "NORMAL";
  else if (regen_rate == RegenRate::STRONG)
    return "STRONG";
  else if (regen_rate == RegenRate::GRAND)
    return "GRAND";

  return "";
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
std::pair<Attribute, Attribute> Helpers::elementToStats(const Element &element)
{
  if (element == Element::PHYSICAL)
    return std::make_pair(Attribute::PHAG, Attribute::PHFD);
  else if (element == Element::FIRE)
    return std::make_pair(Attribute::THAG, Attribute::THFD);
  else if (element == Element::FOREST)
    return std::make_pair(Attribute::PRAG, Attribute::PRFD);
  else if (element == Element::ICE)
    return std::make_pair(Attribute::POAG, Attribute::POFD);
  else if (element == Element::ELECTRIC)
    return std::make_pair(Attribute::CHAG, Attribute::CHFD);
  else if (element == Element::DIGITAL)
    return std::make_pair(Attribute::CYAG, Attribute::CYFD);
  else if (element == Element::NIHIL)
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

  if (strength <= (1 >> 1))
    return Element::NONE;
  else if (strength == 1 >> 2)
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

  if (weakness <= (1 >> 1))
    return Element::NONE;
  else if (weakness == 1 >> 7)
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
std::vector<uint32_t> Helpers::buildExpTable(const uint32_t &min, 
                                             const uint32_t &max, 
                                             const uint32_t &iter)
{
  std::vector<uint32_t> table;

  auto b = log((double)max / min) / (iter - 1);
  auto a = (double)min / (exp(b) - 1.0);

  for  (uint32_t i = 1; i <= iter; i++)
  {
    auto old_exp = round(a * exp(b * (i - 1)));
    auto new_exp = round(a * exp(b * i));
    table.push_back(new_exp - old_exp);
  }

  return table;
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
std::vector<std::vector<std::string>> Helpers::frameSeparator(
    std::string path)
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
         range_split.front().size() == 1 && 
         range_split.back().size() == 1 &&
         range_split.front().front() >= 'A' &&
         range_split.front().front() <= 'Z' &&
         range_split.back().front() >= 'A' &&
         range_split.back().front() <= 'Z' &&
         range_split.front().front() <= range_split.back().front())
      {
        /* String contains a correct range (Eg A-B) */
        if(range_split.front().front() < range_split.back().front())
        {
          split_strings.push_back(second_split.front());
          range_ids.push_back(split_strings.size() - 1);
          range_count++;
        }
        /* String contains a range that's identical (Eg B-B) */
        else
        {
          split_strings.push_back(range_split.front());
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
      uint32_t new_elements = range_str.back().front() - 
                              range_str.front().front();

      /* Store the number of elements in the appropriate variable */
      if(j == 0)
        range1 = new_elements + 1;
      else
        range2 = new_elements + 1;

      /* Duplicate enough elements for the new range */
      for(uint32_t k = 0; k < new_elements; k++)
        for(uint32_t m = 0; m < base_elements; m++)
          linear_set.push_back(linear_set[m]);

      /* Now append the ranges for each on to the end */
      for(uint32_t k = 0; k < linear_set.size(); k++)
      {
        char element = range_str.front().front() + (k / base_elements);
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
    for(uint32_t i = 0; i < range2; i++)
    {
      std::vector<std::string> set_row;
      for(uint32_t j = 0; j < range1; j++)
        set_row.push_back(linear_set[i+range1*j]);
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
std::string& Helpers::ltrim(std::string &s) 
{
  s.erase(begin(s), std::find_if(begin(s), end(s), 
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
bool Helpers::parseRange(std::string sequence, uint32_t &x_min, uint32_t &x_max, 
                         uint32_t &y_min, uint32_t &y_max)
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
std::string& Helpers::rtrim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), 
          std::not1(std::ptr_fun<int, int>(std::isspace))).base(), end(s));
  
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
std::vector<std::string> Helpers::split(const std::string &line, char delim)
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
std::string& Helpers::trim(std::string &s)
{
  return ltrim(rtrim(s));
}

/*=============================================================================
 * GRAPHICAL HELPER FUNCTIONS
 *============================================================================*/

/*
 * Description: Creates the white mask (only needs to be called once for the
 *              duration of the game) that allows the engine to manipulate
 *              brightness.
 *
 * Inputs: SDL_Renderer* renderer - the graphical rendering engine
 * Output: none
 */
void Helpers::createWhiteMask(SDL_Renderer* renderer)
{
  if(white_mask == NULL)
  {
    white_mask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                             SDL_TEXTUREACCESS_TARGET, 
                                             kTILE_SIZE, kTILE_SIZE);
    SDL_SetRenderTarget(renderer, white_mask);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderDrawRect(renderer, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    
    /* Set the new texture as the white mask (with additive blending) */
    SDL_SetTextureBlendMode(white_mask, SDL_BLENDMODE_ADD);
  }
}

/*
 * Description: Deletes the white mask, created on an earlier step. Only
 *              needs to be done once upon the entire close of the application.
 *
 * Inputs: none
 * Output: none
 */
void Helpers::deleteWhiteMask()
{
  if(white_mask != NULL)
    SDL_DestroyTexture(white_mask);
  white_mask = NULL;
}

/* 
 * Description: Returns the white mask for rendering on screen. Do not delete
 *              this pointer.
 *
 * Inputs: none
 * Output: SDL_Texture* - the brightmap texture
 */
SDL_Texture* Helpers::getWhiteMask()
{
  return white_mask;
}
