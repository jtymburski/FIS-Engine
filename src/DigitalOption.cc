#include "DigitalOption.h"

/*=============================================================================
 * DIGITALOPTION
 *============================================================================*/

/* Constructs a DigitalOption */
DigitalOption::DigitalOption() : config{nullptr}, flag_index{35}
{
}

/* Constructs a DigitalOption */
DigitalOption::DigitalOption(Options* config, Coordinate point, int32_t width,
                             int32_t height, int32_t flag_index,
                             std::string name)
    : DigitalOption()
{
  this->config = config;
  this->flag_index = flag_index;
  this->name = name;

  if(config)
    location = Box{point, width, height};
}

/* Sets the DigitalOption to the Default Value */
void DigitalOption::reset()
{
  if(config)
  {
    config->setFlag(
        static_cast<OptionState>(1 << flag_index),
        config->getDefaultFlag(static_cast<OptionState>(1 << flag_index)));
  }
}

/* Sets the DigitalOption Flag */
void DigitalOption::set()
{
  if(config)
    config->setFlag(static_cast<OptionState>(1 << flag_index), true);
}

/* Toggles the DigitalOption Flag */
void DigitalOption::toggle()
{
  if(config)
  {
    config->setFlag(
        static_cast<OptionState>(1 << flag_index),
        !config->getFlag(static_cast<OptionState>(1 << flag_index)));
  }
}

/* Unsets the DigitalOption Flag */
void DigitalOption::unset()
{
  if(config)
    config->setFlag(static_cast<OptionState>(1 << flag_index), false);
}

bool DigitalOption::isSet()
{
  if(config)
  {
    return config->getFlag(static_cast<OptionState>(1 << flag_index));
  }

  return false;
}