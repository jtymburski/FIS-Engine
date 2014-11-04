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
#include <cctype>
#include <chrono>
#include <functional>
#include <iostream>
#include <locale>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <string>
#include <vector>

#include "EnumDb.h"

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
  static const unsigned int seed_original;
  static std::mt19937 rand_eng;
  static std::mt19937_64 rand_64_eng;
  static SDL_Texture* white_mask; /* Brightness manipulator */

  // /*------------------- Constants -----------------------*/
  const static uint16_t kTILE_SIZE; /* The default tile size for game */

public:
  /* Decides whether a percent_chance occurs or not */
  static bool chanceHappens(const uint32_t &pc_chance, const uint32_t &mag);

  /* Generates a random coin flip */
  static bool flipCoin();

  /* Generates a random float between min and max */
  static float randFloat(const float &a, const float &b);

  /* Generates a random number between 0 and max - 1, equal distribution */
  static int randInt(const int &max);

  /* Generates a uniform integer between min and max */
  static int randU(const int &a, const int &b);

  /* Generates and returns an unsigned p-random 32-bit unsigned int */
  static uint32_t randU32();

  /* Generates and returns an unsigned p-random 64-bit unsigned int */
  static uint64_t randU64();

  /* Rolls an X-Sided die S times */
  static int rollXS(const int &x_sides, const int &s_times);

/*=============================================================================
 * GRAMMAR FUNCTIONS
 *============================================================================*/
public:
  /* Decides between "a" or "an" */
  static std::string a_An(const std::string& noun);

  /* Determines whether a given character is a vowel */
  static bool isVowel(const char &c);

  /* Flush the console with [blank] input */
  static void flushConsole(const char &c = ' ');

  /* Converts an enumerated element to a String */
  static std::string elementToString(const Element &element);

  /* Convert enumerated CellState to String */
  static std::string cellToStr(const CellState &cell_state);

  /* Convert enumerated Action Type to String */
  static std::string actionTypeToStr(const ActionType &action_type);

  /* Convert enumerated Action Scope to String */
  static std::string actionScopeToStr(const ActionScope &action_scope);

  /* Convert enumerated AI Difficulty to String */
  static std::string aiDifficultyToStr(const AIDifficulty &ai_difficulty);

  /* Convert enumerated AI Personality to String */
  static std::string aiPersonalityToStr(const AIPersonality &ai_personality);

/*=============================================================================
 * PLAYER / BATTLE HELPER FUNCTIONS
 *============================================================================*/
public:
  /* Returns the pair of off/def attributes corresponding to an element */
  static std::pair<Attribute, Attribute> elementToStats(const Element &element);

  /* Returns the elemental strength to a given element */
  static Element getStrength(const Element &element);

  /* Returns the elemental weakness to a given element */
  static Element getWeakness(const Element &element);

/*=============================================================================
 * GENERAL HELPER FUNCTIONS
 *============================================================================*/
public:
  /* Builds an exponentially growing table from min to max with iter
   * iterations */
  static std::vector<uint32_t> buildExpTable(const uint32_t &min, 
      const uint32_t &max, const uint32_t &iter);
  
  /* Splites the string into a grid based on the frame naming convention */
  static std::vector<std::vector<std::string>> frameSeparator(
      std::string path);
  
  /* Get render tile size */
  static uint16_t getTileSize();

  /* Methods for trimming white space from left of string ends */
  static std::string& ltrim(std::string &s);

  /* Takes a range and parses it to determine the x and y, min and max parts */
  static bool parseRange(std::string sequence, uint32_t &x_min, uint32_t &x_max, 
                         uint32_t &y_min, uint32_t &y_max);
 
  /* Methods for trimming white space from right of string ends */
  static std::string& rtrim(std::string &s);

  /* Splits the string using the given delimiter */
  static std::vector<std::string> split(const std::string &s, char delim);

  /* Methods for trimming whitespace from both ends of std::strings */
  static std::string& trim(std::string &s);
  
/*=============================================================================
 * GRAPHICAL HELPER FUNCTIONS
 *============================================================================*/
public:
  /* Creates the white mask to use - needs to be called to init */
  static void createWhiteMask(SDL_Renderer* renderer);
  
  /* Deletes the white mask - when game shut down */
  static void deleteWhiteMask();

  /* Returns the static white mask created. NULL if not initialized */
  static SDL_Texture* getWhiteMask();
};

#endif // HELPERS_H
