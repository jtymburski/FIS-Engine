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
 * TEMPLATE FUNCTORS
 *============================================================================*/

struct CompPairByCount
{
  template <class T, class U>
  bool operator()(const std::pair<T*, U>& a, const std::pair<T*, U>& b) const
  {
    return a.second < b.second;
  }
};

struct CompPairByID
{
  template <class T, class U>
  bool operator()(const std::pair<T*, U>& a, const std::pair<T*, U>& b) const
  {
    return a.first->getID() < b.first->getID();
  }
};

struct CompPairByMass
{
  template <class T, class U>
  bool operator()(const std::pair<T*, U>& a, const std::pair<T*, U>& b) const
  {
    return a.first->getMass() < b.first->getMass();
  }
};

struct CompPairByName
{
  template <class T, class U>
  bool operator()(const std::pair<T*, U>& a, const std::pair<T*, U>& b) const
  {
    return a.first->getName() < b.first->getName();
  }
};

struct CompPairByValue
{
  template <class T, class U>
  bool operator()(const std::pair<T*, U>& a, const std::pair<T*, U>& b) const
  {
    return a.first->getValue() < b.first->getValue();
  }
};

struct CompPairByValueMass
{
  template <class T, class U>
  bool operator()(const std::pair<T*, U>& a, const std::pair<T*, U>& b) const
  {
    return ((static_cast<double>(a.first->getValue()) /
             static_cast<double>(a.first->getMass())) <
            (static_cast<double>(b.first->getValue()) /
             static_cast<double>(b.first->getMass())));
  }
};

struct CompProbability
{
  template <class T, class U>
  bool operator()(const std::pair<T, U>& a, const std::pair<T, U>& b) const
  {
    return (static_cast<double>(a.second < b.second));
  }
};

struct CompLimbertude
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.user->getStats().getValue(Attribute::LIMB) >
           b.user->getStats().getValue(Attribute::LIMB);
  }
};

struct CompItemFirst
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return (a.owned_item != nullptr && b.owned_item == nullptr);
  }
};

struct CompSkillFirst
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return (a.used_skill != nullptr && b.used_skill == nullptr);
  }
};

struct CompSkillCooldown
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.skill->getCooldown() < b.skill->getCooldown();
  }
};

struct CompSkillCost
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.skill->getCost() < b.skill->getCost();
  }
};

struct CompSkillID
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.skill->getID() < b.skill->getID();
  }
};

struct CompSkillName
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.skill->getName() < b.skill->getName();
  }
};

struct CompSkillPrimary
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return static_cast<uint32_t>(a.skill->getPrimary()) <
           static_cast<uint32_t>(b.skill->getPrimary());
  }
};

struct CompSkillSecondary
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return static_cast<uint32_t>(a.skill->getSecondary()) <
           static_cast<uint32_t>(b.skill->getSecondary());
  }
};

struct CompSkillPoint
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.skill->getValue() < b.skill->getValue();
  }
};

struct CompLevelRequired
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.level_available < b.level_available;
  }
};

struct CompEnabled
{
  template <class T> bool operator()(const T& a, const T& b) const
  {
    return a.enabled < b.enabled;
  }
};

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
  template <class T> bool operator()(T const& a, T const& b) const
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
  template <class T> bool operator()(T const& a, T const& b) const
  {
    return *a < *b;
  }
};

/*=============================================================================
 * TEMPLATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Calculates and returns the nearest percentage point for a given
 *              value within a given amount.
 *
 * Inputs: T value - the value which is within the range of amount
 *         T amount - the amount to check the value within
 * Output: uint32_t - the percentage of the value, to the nearest percent
 */
template <typename T> static uint32_t getPercent(T value, T amount)
{
  /* Grab the % to within XX.X% and then round to the nearest percent. */
  if(value > 0 && amount > 0 && amount >= value)
    return static_cast<uint32_t>(std::round((value * 1000) / (amount * 1000)));

  return 0;
}

/*
 * Description: Obtains a random number of elements from a vector of T and
 *              returns the elements in vector form. This function
 *              will return an empty vector unless the size of the vector
 *              is sufficient to match S, where S > 0.
 *
 * Inputs: std::vector<T> elements - vector to find random elements for
 *         S num_elements - the number of elements to find
 * Output: std::vector<T> - the calculated random elements.
 */
template <typename T, typename S>
static std::vector<T> getRandElements(std::vector<T> elements, S num_elements)
{
  std::vector<T> rand_elements;

  if(elements.size() >= static_cast<uint64_t>(num_elements) && num_elements > 0)
  {
    for(S i = num_elements; i > 0; i--)
    {
      auto rand_elm = randU(0, elements.size() - 1);
      rand_elements.push_back(elements.at(rand_elm));
      elements.erase(begin(elements) + rand_elm);
    }
  }

  return rand_elements;
}

/*
 * Description: Enum Vector Search : Searches for a given enumerated value in a
 *              given vector of the same enumerated type. Outputs true if found
 *
 * Inputs: enum_to_find - the enumerated value of type T to look for
 *         enumerated_vector - a vector of type T of possible enumerated values
 * Output: bool - true if enum_to_find was found in the enumerated vector.
 */
template <typename T>
static bool enumVectorSearch(const T& enum_to_find,
                             std::vector<T> enumerated_vector)
{
  auto found = false;

  for(auto enum_index : enumerated_vector)
    found |= (enum_index == enum_to_find);

  return found;
}

/*
 * Description: Normalized a list of probabilities between two iterators.
 *
 * Inputs: begin - it. to beginning of probabilities to normalize
 *         end   - it. to end of probabilities to normalize
 *
 * Output: bool - true if the sum of the probabilities was non-zero
 */
template <typename T> static bool normalize(T begin, T end)
{
  auto sum = 0.0;

  for(auto it = begin; it != end; ++it)
    sum += (*it);

  if(sum != 0)
  {
    for(auto it = begin; it != end; ++it)
      (*it) /= sum;

    return true;
  }

  return false;
}

/*
 * Description: Normalizes a list of probabilities based on second components
 *              of paired objects
 *
 * Inputs: begin - it. to beginning of probabilities to normalize
 *         end   - it. to ending probabilities to normalize
 *
 * Output: bool - true if the sum of the probabilities was non-zero
 */
template <typename T> static bool normalizePair(T begin, T end)
{
  auto sum = 0.0;

  for(auto it = begin; it != end; ++it)
    sum += (*it).second;

  if(sum != 0)
  {
    for(auto it = begin; it != end; ++it)
      (*it).second /= sum;

    return true;
  }

  return false;
}

/*
 * Description: Given a random double value, and an assumed sorted normalized
 *              distribution between two iterators of paired values, selects
 *              an iterator where the sum first reaches the given random value
 *              based upon the second part of the pair.
 *
 * Inputs: random - a previously selected random value for normalized comparison
 *         begin  - the (assumed) lowest value of the normalized dist.
 *         end    - the (assumed) highest value of the normalized dist.
 *
 * Output: T - iterator to the chosen normalized pair
 */
template <typename T>
static T selectNormalizedPair(const double& random, T begin, T end)
{
  auto sum = 0.0;

  if(random >= 0)
  {
    auto current = begin;
    sum += (*current).second;

    while(sum <= random)
    {
      current++;
      sum += (*current).second;
    }

    return current;
  }

  return end;
}

/*
 * Description: Template function to determine if a value is within a
 *              given range of two bounds, regardless of bound order.
 *
 * Inputs: value - value to check the range for
 *         l_bound - one of the bounds
 *         r_bound - the other bound
 * Output: bool - true if the value is within the two bounds
 */
template <typename T>
static bool isInRange(const T& value, const T& l_bound, const T& r_bound)
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
template <typename T, typename U>
static T setInRange(T& value, const U& l_bound, const U& r_bound)
{
  auto min = std::min(l_bound, r_bound);
  auto max = std::max(l_bound, r_bound);

  if(value < min)
    return min;
  else if(value > max)
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
template <typename T> static T uniqueSplit(T begin, T const end)
{
  /* Reserve the appropriate vector size and push all iterators onto it */
  std::vector<T> v;
  v.reserve(static_cast<size_t>(std::distance(begin, end)));
  for(T i = begin; i != end; i++)
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
  for(T i = begin; i != end && j != v.size(); i++)
  {
    if(i == v[j])
    {
      std::iter_swap(i, begin);
      j++;
      begin++;
    }
  }

  return begin;
}
