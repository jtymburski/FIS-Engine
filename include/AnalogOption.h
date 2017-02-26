/*******************************************************************************
* Class Name: AnalogOption
* Date Created: February 26, 2017
* Inheritance: None
*
* Description
* -----------

*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*******************************************************************************/
#ifndef ANALOG_OPTION_H
#define ANALOG_OPTION_H

#include "Box.h"
#include "Options.h"

class BattleActor;

/* Option Box Construction for Analog Values */
class AnalogOption
{
public:
  /* Constructs an AnalogOption */
  AnalogOption();

  /* Constructs an AnalogOption with a string name */
  AnalogOption(std::string name);

  /* Pointer to the value for the analog option (0 - 100) */
  uint32_t* val;

  /* Pointer to a default value */
  uint32_t default_val;

  /* The number of possible options */
  uint32_t num_options;

  /* Location for the analog option */
  Box location;

  /* String name of the option */
  std::string name;

  /* Decrease the value */
  void decrease();

  /* Reset the analog option to a default value */
  void reset();

  /* Increase the value */
  void increase();
};

#endif  //ANALOG_OPTION_H