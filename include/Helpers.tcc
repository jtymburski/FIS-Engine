/*******************************************************************************
 * Class Name: Helpers [Template Implementation]
 * Date Created: December 13th, 2013
 * Inheritance: None
 * Description: Implementation of Helper template functions to conform to
 *              template implementation standards.
 *
 * Notes
 * -----
 * [1]: 
 *
 * See .h file for TODOs
 ******************************************************************************/

/*
 * Description: Template function to determine if a value is within a 
 *              given range of two bounds, regardless of bound order.
 *
 * Inputs: value - value to check the range for
 *         l_bound - one of the bounds
 *         r_bound - the other bound
 * Output: bool - true if the value is within the two bounds
 */
template<typename T>
static bool isInRange(const T &value, const T &l_bound, const T &r_bound)
{
  auto min = std::min(l_bound, r_bound);
  auto max = std::max(l_bound, r_bound);

  return (value >= min && value <= max);
}

/*
 * Description: Template function which takes a reference to an object 
 *              and puts it within two bounds given by a and b (can be
 *              any order)
 *
 * Notes: - This function requires the < and > operators to be overloaded
 *
 * Inputs: T &value  - ref to object to be set within a range
 *         T &a - ref to one of the boundaries
 *         T &b - ref to one of the boundaries
 * Output: T - object after it has been set within the range
 */
template<typename T> 
static T setWithinRange(T &value, const T &l_bound, const T &r_bound)
{
  auto min = std::min(l_bound, r_bound);
  auto max = std::max(l_bound, r_bound);

  if (value < min)      
    return min;
  else if (value > max)
    return max;
  
  return value;
}

