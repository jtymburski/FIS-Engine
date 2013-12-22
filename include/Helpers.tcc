/*******************************************************************************
 * Class Name: Helpers [Template Implementation]
 * Date Created: December 13th, 2013
 * Inheritance: None
 * Description: Implementation of Helper template functions and structures to 
 *              conform to template implementation standards.
 *
 * Notes
 * -----
 * [1]: 
 *
 * See .h file for TODOs
 ******************************************************************************/

/*=============================================================================
 * TEMPLATE STRUCTURES
 *============================================================================*/

/*
 * Structure: Templated structure for value equivalence. Call the struct, 
 *            with value_equal(T, T), and the appropriate equivalent call will
 *            be checked. Uses the standard "==" operation so this must be
 *            implemented in order for the call to work.
 */
struct value_equal
{
  template<class T>
  bool operator()(T const &a, T const &b) const 
  { 
    return *a == *b; 
  }
};

/*
 * Structure: Templated structure for value less than. Call the struct, 
 *            with value_less(T, T), and the appropriate less than call will
 *            be checked. Uses the standard "<" operation so this must be
 *            implemented in order for the call to work.
 */
struct value_less
{
  template<class T>
  bool operator()(T const &a, T const &b) const 
  { 
    return *a < *b;
  }
};

/*=============================================================================
 * TEMPLATE FUNCTIONS
 *============================================================================*/

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
template<typename T, typename U> 
static T setWithinRange(T &value, const U &l_bound, const U &r_bound)
{
  auto min = std::min(l_bound, r_bound);
  auto max = std::max(l_bound, r_bound);

  if (value < min)      
    return min;
  else if (value > max)
    return max;
  
  return value;
}

/*
 * Description: Template function which takes two iterators and removes all
 *              duplicates without sorting. The call should be something as per
 *              the following, with a vector: 
 *           vec.erase(Helpers::uniqueSplit(vec.begin(), vec.end()), vec.end());
 *
 * Notes: This function requires the == and < operators to be overloaded
 *
 * Inputs: T begin - the front iterator to the vector
 *         T const end - the back iterator to the vector
 * Output: T - returns an iterator to where erase can start up to vector->end
 */
template<typename T> 
static T uniqueSplit(T begin, T const end)
{
  /* Reserve the appropriate vector size and push all iterators onto it */
  std::vector<T> v;
  v.reserve(static_cast<size_t>(std::distance(begin, end)));
  for (T i = begin; i != end; ++i)
  {
    v.push_back(i); 
  }

  /* Sort the iterators, from the objects < than to greater. Then, remove all
     duplicates where the value of the iterator is equivalent and erase the
     values that unique zeroed out. Finally, sort the iterators back to the
     actual array order. */
  std::sort(v.begin(), v.end(), value_less());
  v.erase(std::unique(v.begin(), v.end(), value_equal()), v.end());
  std::sort(v.begin(), v.end());

  /* Now go through and swap all the iterators that are unique to the front of
   * the list */
  size_t j = 0;
  for (T i = begin; i != end && j != v.size(); ++i)
  {
      if (i == v[j])
      {
          std::iter_swap(i, begin);
          ++j;
          ++begin;
      }
  }
  
  return begin;
}