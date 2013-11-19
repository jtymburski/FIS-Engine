/*******************************************************************************
* Class Name: MathHelper [Declaration]
* Date Created: 
* Inheritance: 
* Description: MathHelper is a collection of math-related functions to be used
*              as common helpers in other classes. This includes such functions
*              as dealing with random numbers, working with general values
*              and building tables, etc.

* TODO
* ----
* //TODO - Random # generator -- user Mersenne Twister? [11-18-13]
*        - Else, deal with stray global bool for seed
*******************************************************************************/
#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <algorithm> /* std::min(), std::max() */
#include <stdlib.h>  /* srand, rand */
#include <time.h>    /* time */

bool seeded = false;

/*=============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *============================================================================*/

/* Decides whether a percent_chance occurs or not */
bool chanceHappens(const uint &pc_chance, const uint& magnitude = 100);

/* Generates a random coin flip */
bool flipCoin();

/* Generates a random number between 0 and max - 1, equal distribution */
int randInt(const int &max);

/* Generates a Gaussian uniform integer between min and max */
int randUniform(const int &a, const int &b);

/* Rolls an s-sided time x-times */
int rollSX(const int &s_sides, const int &x_times);

/* Seeds the random number generator */
bool seed();

/*=============================================================================
 * GENERAL HELPER FUNCTIONS
 *============================================================================*/

/* Assign a value within lower and upper boundaries */
template<class T> T setWithinRange(T &value, const size_t &a, const size_t &b);

/* Builds an exponentially growing table from min to max with iter iterations */
std::vector<int> buildExponentialTable(const int &min, const int &max, const int &iter);

#endif //MATHHELPER_H
