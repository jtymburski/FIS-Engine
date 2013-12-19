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
#include <locale>
#include <random>
#include <sstream>
#include <string>
#include <vector> 
#include <cctype>

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

public:
  /* Decides whether a percent_chance occurs or not */
  static bool chanceHappens(const uint32_t &pc_chance, const uint32_t &mag);

  /* Generates a random coin flip */
  static bool flipCoin();

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
  /* Converts an enumerated element to a String */
  static std::string elementToString(const Element &element);

  /* Convert enumerated CellState to String */
  static std::string cellToStr(const CellState &cell_state);

/*=============================================================================
 * PLAYER / BATTLE HELPER FUNCTIONS
 *============================================================================*/
public:
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
  static std::vector<uint32_t> buildExpTable(const uint32_t &min, const uint32_t &max, 
                                             const uint32_t &iter);

  /* Splits the string using the given delimiter */
  static std::vector<std::string> split(const std::string &s, char delim);

  /* Methods for trimming whitespace from std::strings */
  static std::string& ltrim(std::string &s);
  static std::string& rtrim(std::string &s);
  static std::string& trim(std::string &s);
};

#endif // HELPERS_H
