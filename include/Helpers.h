/*******************************************************************************
 * Class Name: Helpers [Declaration]
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
 * TODO
 * ----
 ******************************************************************************/
#ifndef HELPERS_H
#define HELPERS_H

#include <algorithm> /* std::min(), std::max() */
#include <cassert>
#include <cctype>
#include <chrono>
#include <functional>
#include <iostream>
#include <locale>
#include <memory>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <string>
#include <vector>

#include "EnumDb.h"
#include "XmlData.h"

/* High precision timer abstraction */
class Timer
{
public:
  Timer() : beg_(clock_::now()){};

  void reset()
  {
    beg_ = clock_::now();
  }

  double elapsed() const
  {
    return std::chrono::duration_cast<second_>(clock_::now() - beg_).count();
  }

private:
  typedef std::chrono::high_resolution_clock clock_;
  typedef std::chrono::duration<double, std::ratio<1>> second_;
  std::chrono::time_point<clock_> beg_;
};

/* Coordinate with uints */
struct UCoordinate
{
  UCoordinate() : x{0}, y{0} {};
  UCoordinate(uint32_t x, uint32_t y) : x{x}, y{y} {};

  uint32_t x;
  uint32_t y;

  bool operator==(const UCoordinate& b)
  {
    return ((x == b.x) && (y == b.y));
  }
};

/* Coordinate with ints */
struct Coordinate
{
  Coordinate() : x{0}, y{0} {};
  Coordinate(int32_t x, int32_t y) : x{x}, y{y} {};

  int32_t x;
  int32_t y;

  bool operator==(const Coordinate& b)
  {
    return ((x == b.x) && (y == b.y));
  }
};

/* Coordinate with floats */
struct Floatinate
{
  Floatinate() : x{0}, y{0} {};
  Floatinate(float x, float y) : x{x}, y{y} {};

  float x;
  float y;
};

/* Structure for large lay over frames */
struct LayOver
{
  std::string path;
  int anim_time;
  float velocity_x; /* velocity x cycle_time */
  float velocity_y; /* velocity x cycle_time */

  /* Map only */
  bool player_relative;
};

/* Structure for battle scene information */
struct BattleScene
{
  int id;
  std::string background;
  int music_id;
  std::vector<LayOver> underlays;
  std::vector<LayOver> midlays;
  std::vector<LayOver> overlays;
};

class Helpers
{
public:
/*=============================================================================
 * TEMPLATE FUNCTIONS
 *============================================================================*/
#include "Helpers.tcc" /* Template Implementation */

  /*=============================================================================
   * RANDOM GENERATOR FUNCTIONS
   *============================================================================*/
private:
  /* Mersenne Twister Engines */
  static const uint32_t seed_original;
  static std::mt19937 rand_eng;
  static std::mt19937_64 rand_64_eng;
  static SDL_Texture* mask_black; /* Fading manipulator */
  static SDL_Texture* mask_white; /* Brightness manipulator */

  /*------------------- Constants -----------------------*/
  const static uint8_t kMAX_RENDER_DEPTH; /* Max render depth */
  const static uint16_t kTILE_SIZE;       /* The default tile size for game */

public:
  /* Decides whether a percent_chance occurs or not */
  static bool chanceHappens(const uint32_t& pc_chance, const uint32_t& mag);

  /* Generates a random coin flip */
  static bool flipCoin();

  /* Generates a random float between min and max */
  static float randFloat(const float& a, const float& b);

  /* Generates a random number between 0 and max - 1, equal distribution */
  static int randInt(const int& max);

  /* Generates a uniform integer between min and max */
  static int randU(const int& a, const int& b);

  /* Generates and returns an unsigned p-random 32-bit unsigned int */
  static uint32_t randU32();

  /* Generates and returns an unsigned p-random 64-bit unsigned int */
  static uint64_t randU64();

  /* Rolls an X-Sided die S times */
  static int rollXS(const int& x_sides, const int& s_times);

  /*=============================================================================
   * GRAMMAR FUNCTIONS
   *============================================================================*/
public:
  /* Decides between "a" or "an" */
  static std::string a_An(const std::string& noun);

  /* Convert enumerated Action Type to String */
  static std::string actionTypeToStr(const ActionType& action_type);

  /* Convert enumerated Action Scope to String */
  static ActionScope actionScopeFromStr(const std::string scope);
  static std::string actionScopeToStr(const ActionScope& action_scope);

  /* Convert enumerated AI Difficulty to String */
  static std::string aiDifficultyToStr(const AIDifficulty& ai_difficulty);

  /* Convert enumerated AI Personality to String */
  static std::string aiPersonalityToStr(const AIPersonality& ai_personality);

  /* Return enumerated Ailment to String */
  static Infliction ailmentFromStr(const std::string& ail);
  static std::string ailmentToStr(const Infliction& ail);

  /* Returns the enumerated Attribute to String */
  static std::string attributeToStr(Attribute attribute, bool shortform = true);

  /* Convert enumerated CellState to String */
  static std::string cellToStr(const CellState& cell_state);

  /* Converts an ElementCurve from string to the enumerated element */
  static ElementCurve curveFromString(const std::string& curve);
  static std::string curveToString(const ElementCurve& curve,
                                   bool simple = true);

  /* Converts an enumerated direction to a string, and vice versa */
  static Direction directionFromString(const std::string& direction);
  static std::string directionToString(const Direction& direction);

  /* Converts an enumerated element to a String */
  static Element elementFromString(const std::string& element);
  static uint16_t elementToInt(Element element);
  static std::string elementToString(const Element& element);

  /* Flush the console with [blank] input */
  static void flushConsole(const char& c = ' ');

  /* Converts an enumerated GameKey to a String */
  static std::string gameKeyToStr(const GameKey& game_key);

  /* Determines whether a given character is a vowel */
  static bool isVowel(const char& c);

  /* Get the string of a roman numeral from a value */
  static std::string numToRoman(int value);

  /* Returns the enumerated party type from string form */
  static PartyType partyTypeFromStr(const std::string& type);
  static std::string partyTypeToStr(const PartyType& type);

  /* Returns a string form of a given rank enumerators */
  static std::string rankToStr(const Rank& rank_level);

  /* Returns the enumerated regen rate in string form */
  static RegenRate regenRateFromStr(const std::string& regen_rate);
  static std::string regenRateToStr(const RegenRate& regen_rate);

  /* Returns the enumerated tier to/from string form */
  static ItemTier tierFromStr(const std::string& tier);
  static std::string tierToStr(const ItemTier& tier);

  /* Returns the enumerated tracking state to/from string form */
  static TrackingState trackingFromStr(const std::string& track);
  static std::string trackingToStr(const TrackingState& track);

  /* Returns the enumerated thing type to/from string form */
  static ThingBase typeFromStr(const std::string& type);
  static std::string typeToStr(const ThingBase& type);

  /* Returns the enumerated turn state to string form */
  static std::string turnStateToStr(const TurnState& turn_state);

  /* Returns the enumerated valid status to string form */
  static std::string validStatusToStr(const ValidStatus& valid_status);

  /* Renders the string equivalent of a VictoryState enum */
  static std::string victoryStateToStr(VictoryState victory_state);

  /*=============================================================================
   * PLAYER / BATTLE HELPER FUNCTIONS
   *============================================================================*/
public:
  /* Returns the pair of off/def attributes corresponding to an element */
  static std::pair<Attribute, Attribute> elementToStats(const Element& element);

  /* Returns the elemental strength to a given element */
  static Element getStrength(const Element& element);

  /* Returns the elemental weakness to a given element */
  static Element getWeakness(const Element& element);

  /*===========================================================================
  * GENERAL HELPER FUNCTIONS
  *============================================================================*/
public:
  /* Builds an exponentially growing table from min to max with # iterations */
  static std::vector<uint32_t>
  buildExpTable(const uint32_t& min, const uint32_t& max, const uint32_t& iter);

  /* Color conversion helpers */
  static SDL_Color colorFromHex(std::string hex_color);
  static std::string colorToHex(SDL_Color color, bool alpha = false);

  /* Appends char and string together */
  static std::string combine(std::string s, char c, bool before = false);

  /* Creates a blank lay over and scene structures */
  static LayOver createBlankLayOver();
  static BattleScene createBlankScene();

  /* Formats a number ex. 35987 into 32 899 etc. */
  static std::string formatUInt(uint32_t number, std::string delim = " ");
  static std::string alignRight(std::string s, uint32_t length);

  /* Splites the string into a grid based on the frame naming convention */
  static std::vector<std::vector<std::string>> frameSeparator(std::string path);

  /* Returns the distance between two Coordinates */
  static uint32_t getDistance(Coordinate a, Coordinate b);

  /* Get render depth for thing stacks */
  static uint8_t getRenderDepth();

  /* Get render tile size */
  static uint16_t getTileSize();

  /* Hex conversions */
  static std::string hexFromBaseTen(uint32_t base_ten);
  static uint32_t hexToBaseTen(std::string base_sixteen);

  /* Methods for trimming white space from left of string ends */
  static std::string& ltrim(std::string& s);

  /* Takes a range and parses it to determine the x and y, min and max parts */
  static bool parseRange(std::string sequence, uint32_t& x_min, uint32_t& x_max,
                         uint32_t& y_min, uint32_t& y_max);

  /* Parses a comma delimited range and separates into integers */
  static std::vector<std::vector<uint16_t>> parseRangeSet(std::string sequence);

  /* Methods for trimming white space from right of string ends */
  static std::string& rtrim(std::string& s);

  /* Splits the string using the given delimiter */
  static std::vector<std::string> split(const std::string& s, char delim);

  /* Methods for trimming whitespace from both ends of std::strings */
  static std::string& trim(std::string& s);

  /* Update load calls for global structs */
  static LayOver updateLayOver(LayOver lay_over, XmlData data, int file_index);
  static BattleScene updateScene(BattleScene scene, XmlData data,
                                 int file_index);

  /*
*=============================================================================
* GRAPHICAL HELPER FUNCTIONS
*============================================================================
*/
public:
  /* Calculate points needing to be rendered between two coordinates */
  static std::vector<Coordinate> bresenhamPoints(Coordinate begin,
                                                 Coordinate end);

  /* Calculates the alpha fade in value given a cycle time */
  static uint8_t calcAlphaFadeIn(int32_t cycle_time, uint8_t alpha,
                                 uint32_t fade_time, uint8_t alpha_max = 255);

  /* Calculates the alpha fade out value given a cycle time */
  static uint8_t calcAlphaFadeOut(int32_t cycle_time, uint8_t alpha,
                                  uint32_t fade_time, uint8_t alpha_min = 0);

  /* Color getting functions for various alpha states */
  static uint8_t calcColorRed(SDL_Color color, uint8_t alpha);
  static uint8_t calcColorGreen(SDL_Color color, uint8_t alpha);
  static uint8_t calcColorBlue(SDL_Color color, uint8_t alpha);

  /* Creates the masks to use - needs to be called to init */
  static void createMaskBlack(SDL_Renderer* renderer);
  static void createMaskWhite(SDL_Renderer* renderer);

  /* Deletes the masks - when game shut down */
  static void deleteMasks();

  /* Checks to see whether a given test coordiante is within a range */
  static bool isWithinRange(Coordinate test, Coordinate top_l,
                            Coordinate bot_r);

  /* Returns the static masks created. NULL if not initialized */
  static SDL_Texture* getMaskBlack();
  static SDL_Texture* getMaskWhite();

  /* Update the position of a unit given a cycle time and velocity */
  static int32_t updatePosition(int32_t cycle_time, int32_t curr_pos,
                                int32_t end_pos, float velocity);

  /* Update an (X, Y) coordinate given a curr position and end position */
  static Coordinate updateCoordinate(int32_t cycle_time, Coordinate current,
                                     Coordinate final, float velocity);

  static float updateHoverBrightness(int32_t time_elapsed, float cycle_rate,
                                     float min_value, float max_value);
};

#endif // HELPERS_H
