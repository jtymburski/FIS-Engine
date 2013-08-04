/******************************************************************************
 * Class Name: Math Helper
 * Date Created: August 4th, 2013
 * Inheritance: none
 * Description: Helper functions for generic math algorithms.
 *****************************************************************************/
#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <ctime>
#include <limits>
#include <QtGlobal>

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/* Decides whether a percent_chance occurs or not */
bool chanceHappens(uint percent_chance);

/* Generates a random coin flip */
bool flipCoin();

/* Generates a random unsigned integer [min, max]. */
uint randInt(uint min, uint max);

/* Generates a random unsigned 64-bit integer */
quint64 randInt64();

/* Generates a random unsigned 64-bit integer between min and max */
quint64 randInt64(quint64 min, quint64 max);

/* Generates a Gaussian uniform integer between min and max */
int randUniform(int min, int max);

/* Generates a random D6 roll */
int rollD6();

/* Generates a random D10 roll */
int rollD10();

/* Generates a random D20 roll */
int rollD20();

/* Generates a random D100 roll */
int rollD100();

#endif // MATHHELPER_H
