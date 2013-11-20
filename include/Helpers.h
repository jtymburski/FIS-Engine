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
 *  - Random # generator - use Mersene Twister [11-18-13]
 ******************************************************************************/
#ifndef HELPERS_H
#define HELPERS_H

#include <algorithm> /* std::min(), std::max() */
#include <sstream>
#include <stdlib.h>  /* srand, rand */
#include <string>
#include <time.h>    /* time */
#include <vector>

class Helpers
{
/*=============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *============================================================================*/
public:
  /* Decides whether a percent_chance occurs or not */
  static bool chanceHappens(const uint32_t &pc_chance, const uint32_t &mag);

  /* Generates a random coin flip */
  static bool flipCoin();

  /* Generates a random number between 0 and max - 1, equal distribution */
  static int randInt(const int &max);

  /* Generates a Gaussian uniform integer between min and max */
  static int randUniform(const int &a, const int &b);

  /* Rolls an X-Sided die S times */
  static int rollXS(const int &x_sides, const int &s_times);

  /* Seeds the random number generator */
  static void seed();

/*=============================================================================
 * GENERAL HELPER FUNCTIONS
 *============================================================================*/
public:
  /* Builds an exponentially growing table from min to max with iter
   * iterations */
  static std::vector<int> buildExponentialTable(const int &min, const int &max, 
                                                const int &iter);

  /* Assign a value within lower and upper boundaries */
  template<class T> 
  static T setWithinRange(T &value, const size_t &a, const size_t &b);
  
  /* Splits the string using the given delimiter */
  static std::vector<std::string> split(const std::string &s, char delim,
                                        std::vector<std::string> &elements);
};

#endif // HELPERS_H
