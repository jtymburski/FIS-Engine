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
const uint16_t Helpers::kTILE_SIZE = 64;

/*=============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *============================================================================*/

const unsigned int Helpers::seed_original = 
                    std::chrono::system_clock::now().time_since_epoch().count();
SDL_Texture* Helpers::white_mask = NULL;

std::mt19937 Helpers::rand_eng(seed_original);
std::mt19937_64 Helpers::rand_64_eng(seed_original);

/*
 * Description: Determines if a certain event will occur, given a chance. The
 *              accuracy (or magnitude of accuracy) can be changed using the
 *              second paramater, but defaults to 100 for % value.
 *
 * Notes: example uses:
 *        - 67% chance : bool b = chanceHappens(67, 100)
 *        - 67.4% chacne: bool b = chanceHappens(674, 1000)
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
 * Description: 
 *
 * Inputs: 
 * Output: 
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
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
void Helpers::flushConsole(const char &c)
{
  for (int i = 0; i < 100; i++)
    std::cout << c << std::endl;
}

/*
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
bool Helpers::isVowel(const char &c)
{
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
          c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

/*
 * Description: 
 *
 * Inputs: 
 * Output: 
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
 * Description: 
 *
 * Inputs: 
 * Output: 
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
  else if (cell_state == CellState::E_LINK)
    return "E_LINK";

  return "";
}

/*
 * Description: 
 *
 * Inputs: 
 * Output: 
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
 * Description: 
 *
 * Inputs: 
 * Output: 
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
 * Description: 
 *
 * Inputs: 
 * Output: 
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
 * Description: 
 *
 * Inputs: 
 * Output: 
 */
/* Convert enumerated AI Personality to String */
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

/* Get render tile size */
// TODO: Comment
uint16_t Helpers::getTileSize()
{
  return kTILE_SIZE;
}

/*
 * Description Trims white space from the left side of a std::string
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
 * Description Trims white space from the right side of a std::string
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

/* Splites the string into a grid based on the sprite naming convention */
// TODO: Comment and functionality
std::vector<std::vector<std::string>> Helpers::spriteGridSplitter(
    std::string path)
{
  /* Step 1: split on '[' -> finish with std::vector<std::string> */

  /* Step 2: Parse each string from 1 to n, and split on the ']' character 
           -> Finish with std::vector<std::string> for each */

  /* Step 3: For each string vector from step 2, check the compatibility 
             of the first string. Has to be "Capital Letter"-"Capital Letter"
             If it is not, use the original string.
             Otherwise, return all the rest of the string, after ']' with
             each letter in the range.
           -> Finishes with std::vector<std::string> of 1-n size 
           Note: only 2 good splits are valid. all remainder return string */

  /* Step 4: Re-assemble all the strings together and create every possible
             iteration assembled in the right order.
           Note: some complications: if [A-B]B, it needs to assemble a set 
                 with two columns and one row {AA},{BA}
                 If A[A-B], it needs to assemble a set with two rows and
                 one column. -> will have to do some analyze objects */

  // TESTING - temporary
  std::vector<std::vector<std::string>> set;
  std::vector<std::string> line;
  line.push_back("test");
  line.push_back("this");
  line.push_back("is");
  line.push_back("a");
  set.push_back(line);
  set.push_back(line);
  return set;
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
