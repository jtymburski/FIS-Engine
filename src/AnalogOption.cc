#include "AnalogOption.h"

/*=============================================================================
 * ANALOGOPTION
 *============================================================================*/

AnalogOption::AnalogOption()
    : val{nullptr}, default_val{0}, num_options{1}, location{Box()}
{
}

AnalogOption::AnalogOption(std::string name) : AnalogOption()
{
  this->name = name;
}

/* Decrease the value */
void AnalogOption::decrease()
{
  if(val && (*val) > 0)
    *val -= 100 / num_options;
}

/* Reset the analog option to a default value */
void AnalogOption::reset()
{
  if(default_val && val)
    *val = default_val;
}

/* Increase the value */
void AnalogOption::increase()
{
  if(val && (*val) < 100)
    *val += 100 / num_options;
}