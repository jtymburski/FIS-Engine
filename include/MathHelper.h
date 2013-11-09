/******************************************************************************
 * Class Name: Math Helper
 * Date Created: August 4th, 2013
 * Inheritance: none
 * Description: Helper functions for generic math algorithms.
 *****************************************************************************/
#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <QTime>
#include <QtGlobal>
#include <QDebug>

#include <cmath>
#include <algorithm>
#include <ctime>
#include <limits>

#include "EnumDb.h"

/*============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *===========================================================================*/

/* Decides whether a percent_chance occurs or not */
bool chanceHappens(uint percent_chance);

/* Generates a random coin flip */
bool flipCoin();

/* Returns a random direction */
EnumDb::Direction randDirection();

/* Generates a random number between 0 and max - 1, equal distribution */
int randInt(int max);

/* Generates a random unsigned 64-bit integer */
quint64 randInt64();

/* Generates a random unsigned 64-bit integer between min and max */
quint64 randInt64(quint64 min, quint64 max);

/* Generates a Gaussian uniform integer between min and max */
int randUniform(int a, int b);

/* Generates a random D6 roll */
int rollD6();

/* Generates a random D10 roll */
int rollD10();

/* Generates a random D20 roll */
int rollD20();

/* Generates a random D100 roll */
int rollD100();

/* Rolls an X-Sided die S times */
int rollXS(int x_sides, int s_times);

/* Seeds the random number generator */
void seed();

/*============================================================================
 * OTHER FUNCTIONS
 *===========================================================================*/

/* Builds an exponentially growing table from min to max with iter iterations */
QVector<int> buildExponentialTable(int min, int max, int iter);

#endif // MATHHELPER_H
