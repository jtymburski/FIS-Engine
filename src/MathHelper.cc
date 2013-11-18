/*******************************************************************************
* File Name: MathHelper [Implementation]
* Date Created: November 17th, 2013
* Inheritance: None
* Description: MathHelper is a collection of math-related functions to be used
*              as common helpers in other classes.
*
* Notes:
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "MathHelper.cc"

/*=============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *============================================================================*/

/* Decides whether a percent_chance occurs or not */
bool chanceHappens(const uint &pc_chance)
{
  if (pc_chance >= 100)
    return true;
  if (pc_chance <= 0)
    return false;

  auto random = randUniform(1, 100);

  if ((uint)random <= pc_chance) 
    return true;
  return false;
}

/* Generates a random coin flip */
bool flipCoin()
{
  return rand() % 2;
}

/* Generates a random number between 0 and max - 1, equal distribution */
int randInt(const int &max)
{
  return rand() % max;
}

/* Generates a Gaussian uniform integer between min and max */
int randUniform(const int &a, const int &b)
{
  auto min = std::min(a, b);
  auto max = std::max(a, b);

  return (rand() % (max - min + 1)) + min;
}

/* Rolls an X-Sided die S times */
int rollXS(int x_sides, int s_times)
{
  auto total = 0;

  for (auto i = 0; i < s_times; i++)
    total += randInt(x_sides + 1);

  return total;
}

/* Seeds the random number generator */
bool seed()
{
  if (!seeded)
  {
    srand(time(NULL));
    seeded = true;
    return true;
  }

  return false;
}

/*=============================================================================
 * GENERAL HELPER FUNCTIONS
 *============================================================================*/

/* Asserts and assigns a value within range */
template<class T> 
T setWithinRange(T &value, const size_t &a, const size_t &b)
{
  auto min = std::min(a, b);
  auto max = std::max(a, b);

  if (value < min)
    value = min;
  else if (value > max)
    value = max;
  return value;
}

/*
 * Description: This functions computes an exponentially growing table from
 *              min to max with iter iterations.
 *
 * Inputs: min  - the starting value
 *         max  - the ending value
 *         iter - the # of iterations of the table
 * Output: std::vector<int> - the vector containing the constructed table
 */
std::vector<int> buildExponentialTable(const int &min, const int &max, const int &iter)
{
  std::vector<int> table;

  auto b = log((double)max / min) / (iter - 1);
  auto a = (double)min / (exp(b) - 1.0);

  for  (auto i = 1; i <= iter; i++)
  {
    auto old_exp = round(a * exp(b * (i - 1)));
    auto new_exp = round(a * exp(b * i));
    table.push_back(new_exp - old_exp);
  }

  return table;
}