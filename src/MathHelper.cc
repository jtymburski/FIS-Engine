/******************************************************************************
 * Class Name: Math Helper
 * Date Created: August 4th, 2013
 * Inheritance: none
 * Description: Helper functions for generic math algorithms.
 *
 * Notes [1]: These numbers are PSEUDO-random, seeded by QTime.
 * Notes [2]: The function seed() should be called once per application.
 *****************************************************************************/
#include "MathHelper.h"

/*============================================================================
 * FUNCTIONS
 *===========================================================================*/

/*
 * Description: Given a percent chance of something happening, returns true
 *              if the event will happen based on the generation of a random
 *              # from 1 to 100.
 *
 * Inputs: uint - percent chance for the event to occur
 * Output: bool - true if the event occured.
 */
bool chanceHappens(uint percent_chance)
{
  uint random = randUniform(1, 100);

  if (random <= percent_chance)
      return true;
  return false;
}

/*
 * Description: Flips a coin and returns the result.
 *
 * Inputs: none
 * Output: bool - state of the coin flip.
 */
bool flipCoin()
{
  return rand() % 2;
}

/*
 * Description: Returns a random direction
 *
 * Inputs: none
 * Output: EnumDb::Direction - randomly generated direction
 */
EnumDb::Direction randomDirection()
{
  int random_num = randInt(4);

  if (random_num == 0)
    return EnumDb::NORTH;
  if (random_num == 1)
    return EnumDb::EAST;
  if (random_num == 2)
    return EnumDb::SOUTH;

  return EnumDb::WEST;
}

/*
 * Description: Randomly genreates an integer from 0 to max, exclusive.
 *
 * Inputs: max - upper bound of number distribution (max will not be reached)
 * Output: int - randomly generated number from 1 to max - 1.
 */
int randInt(int max)
{
  return qrand() % max;
}

/*
 * Description: Generates a 64-bit random integer
 *
 * Inputs: none
 * Output: quint64 - randomly generated 64-bit number
 */
quint64 randInt64()
{
  quint64 low = qrand();
  quint64 high = qrand();
  return low | (high << 32);
}

/*
 * Description: Generates a 64-bit random integer with a lower to an
 *              upper boundary.
 *
 * Inputs: quint64 - minimum bound for generated number
 *         quint64 - maximum bound for generated number
 * Output: quint64 - randomly generated 64-bit number
 */
quint64 randInt64(quint64 min, quint64 max)
{
  quint64 difference = max - min;

  if (difference < std::numeric_limits<quint32>::max())
    return randUniform((uint)min, (uint)max);

  quint64 rlow  = qrand();
  quint64 rhigh = qrand();
  quint64 vlow  = difference % 0xFFFFFFFF;
  quint64 vhigh = difference >> 32;

  quint64 random = (rhigh * vlow) >> 32;
  random += (rlow * vhigh) >> 32;
  random += rhigh * vhigh;
  random += min;

  return random;
}

/*
 * Description: Genrates a random number from a to b inclusive with uniform
 *              distribution.
 *
 * Note [1]: a or b can be either min or max
 *
 * Inputs: a - first bound
 *         b - other bound
 * Output: int - uniform distributed random number from a to b
 */
int randUniform(int a, int b)
{
    int min = std::min(a, b);
    int max = std::max(a, b);

    return (qrand() % (max - min + 1)) + min;
}

/*
 * Description: Simulates the rolling of a 6-sided die and returns the roll
 *
 * Inputs: none
 * Output: int - randomly generated # from [1-6]
 */
int rollD6()
{
    return (qrand() % 6) + 1;
}

/*
 * Description: Simulates the rolling of a 10-sided die and returns the roll
 *
 * Inputs: none
 * Output: int - randomly generated # from [1-10]
 */
int rollD10()
{
  return (qrand() % 10) + 1;
}

/*
 * Description: Simulates the rolling of a 20-sided die and returns the roll
 *
 * Inputs: none
 * Output: int - randomly generated # from [1-20]
 */
int rollD20()
{
  return (qrand() % 20) + 1;
}

/*
 * Description: Simulates the rolling of a 100-sided die and returns the roll
 *
 * Inputs: none
 * Output: int - randomly generated # from [1-100]
 */
int rollD100()
{
  return (qrand() % 100) + 1;
}

/*
 * Description: Simulates the rolling of an x-sided die s-times.
 *
 * Inputs: int x_sides - number of sides to be rolled for.
 *         int s_dies - number of times to roll the die.
 * Output: int - the outcome of s_times rolls of an x_sides die.
 */
int rollXS(int x_sides, int s_times)
{
  int total = 0;

  for (int i = 0; i < s_times; i++)
    total += randInt(x_sides + 1);

  return total;
}

/*
 * Description: Reseeds the random number generator with a new seed based
 *              on the current time.
 *
 * Inputs: none
 * Output: none
 */
void seed()
{
  QTime midnight(0, 0, 0);
  qsrand(midnight.secsTo(QTime::currentTime()));
}

/*============================================================================
 * OTHER FUNCTIONS
 *===========================================================================*/

/*
 * Description: This functions computes an exponentially growing table from
 *              min to max with iter iterations.
 *
 * Inputs: min  - the starting value
 *         max  - the ending value
 *         iter - the # of iterations of the table
 * Output: QVector<int> - the QVector containing the generated table
 */
QVector<int> buildExponentialTable(int min, int max, int iter)
{
  QVector<int> table;
  double b = log((double)max / min) / (iter - 1);
  double a = (double)min / (exp(b) - 1.0);

  for  (int i = 1; i <= iter; i++)
  {
      int old_exp = round(a * exp(b * (i - 1)));
      int new_exp = round(a * exp(b * i));
      table.push_back(new_exp - old_exp);
  }

  return table;
}
