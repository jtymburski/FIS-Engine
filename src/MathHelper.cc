/*******************************************************************************
* File Name: MathHelper [Implementation]
* Date Created: November 17th, 2013
* Inheritance: None
* Description: MathHelper is a collection of math-related functions to be used
*              as common helpers in other classes. This includes such functions
*              as dealing with random numbers, working with general values
*              and building tables, etc.
*
* See .h file for TODOs
*******************************************************************************/

#include "MathHelper.cc"

/*=============================================================================
 * RANDOM GENERATOR FUNCTIONS
 *============================================================================*/

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
 * Inputs: const uint &pc_chance - const ref to the chance the event has
 *         const uint &magnitude - magnitude of accuracy (bound to generate #s)
 * Output: bool - determination of the event
 */
bool chanceHappens(const uint &pc_chance, const uint &magnitude)
{
  if (pc_chance >= magnitude)
    return true;
  if (pc_chance <= 0)
    return false;

  auto random = randUniform(1, static_cast<int>(magnitude));

  if (static_cast<uint>(random) <= pc_chance) 
    return true;
  return false;
}

/*
 * Description: Simulates the flipping of a coin and returns the result
 *
 * Inputs: none
 * Output: bool - outcome of the simulated coin flip
 */
bool flipCoin()
{
  return rand() % 2;
}

/*
 * Description: Generates a number between one and a given value - 1
 *              and returns the result.
 *
 * Inputs: const int &max - ref to the maximum value
 * Output: int - generated number between 0 and max -1
 */
int randInt(const int &max)
{
  return rand() % max;
}

/*
 * Description: Generates a Gaussian uniform integer between two given bounds
 *              (can be any order) and returns the result.
 *
 * Inputs: const int &a - const ref to one of the bounds
 8         const int &b - const ref to one of the bounds
 * Output: int - the generated number between the two bounds
 */
int randUniform(const int &a, const int &b)
{
  auto min = std::min(a, b);
  auto max = std::max(a, b);

  return (rand() % (max - min + 1)) + min;
}

/*
 * Description: Simulates the rolling of an s-sided time x # of times and 
 *              returns the result.
 *
 * Inputs: const int& s_sides - const ref to # of sided die to represent
 *         const int& x_times - const ref to # of times to roll the die
 * Output: int - the result of the die roll simulation
 */
int rollSX(const int &s_sides, const int &x_times)
{
  auto total = 0;

  for (auto i = 0; i < x_times; i++)
    total += randInt(s_times + 1);

  return total;
}

/*
 * Description: Seeds the random number generator if it hasn't been seeded.
 *
 * Notes: - currently uses a global bool to check
 *
 * Inputs: none
 * Output: bool - true if the generator is being seeded for the first time
 */
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

/*
 * Description: Template function which takes a reference to an object 
 *              and puts it within two bounds given by a and b (can be
 *              any order)
 *
 * Notes: - This function requires the < and > operators to be overloaded
 *
 * Inputs: T &value - ref to object to be set within a range
 *         size_t &a - ref to one of the boundaries
 *         size_t &b - ref to one of the boundaries
 * Output: T - object after it has been set within the range
 */
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