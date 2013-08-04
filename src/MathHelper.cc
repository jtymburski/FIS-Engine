/******************************************************************************
 * Class Name: Math Helper
 * Date Created: August 4th, 2013
 * Inheritance: none
 * Description: Helper functions for generic math algorithms.
 *****************************************************************************/
#include "MathHelper.h"

bool chanceHappens(uint percent_chance)
{
  uint random = randInt(1, 100);

  if (random <= percent_chance)
      return true;
  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool flipCoin()
{
  return rand() % 2;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
uint randInt(uint min, uint max)
{
  return min + (qrand() % (max + 1));
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
quint64 randInt64()
{
  quint64 low = qrand();
  quint64 high = qrand();
  return low | (high << 32);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
quint64 randInt64(quint64 min, quint64 max)
{
  quint64 difference = max - min;

  if (difference < std::numeric_limits<quint32>::max())
    return randInt((uint)min, (uint)max);

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
 * Description:
 *
 * Inputs:
 * Output:
 */
int randUniform(int a, int b)
{
  return 0; //TODO
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int rollD6()
{
    return (qrand() % 6) + 1;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int rollD10()
{
  return (qrand() % 10) + 1;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int rollD20()
{
  return (qrand() % 20) + 1;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
int rollD100()
{
  return (qrand() % 100) + 1;
}
