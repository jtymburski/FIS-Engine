/*******************************************************************************
* Class Name: DigitalOption
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
#ifndef DIGITAL_OPTION_H
#define DIGITAL_OPTION_H

#include "Box.h"
#include "Options.h"

/* Option Box Construction for On/Off Values */
class DigitalOption
{
public:

  /* Constructs a DigitalOption */
  DigitalOption();

  /* Constructs a DigitalOption */
  DigitalOption(Options* config, Coordinate point, int32_t width,
                int32_t height, int32_t flag_index, std::string name);

  /* Configuraiton pointer */
  Options* config;

  /* Value */
  int32_t flag_index;

  /* Location for the DigitalOption */
  Box location;

  /* String name of the option */
  std::string name;

  /* Resets a DigitalOption */
  void reset();

  /* Sets a DigitalOption */
  void set();

  /* Toggles the DigitalOption */
  void toggle();

  /* Unsets the DigitalOption */
  void unset();

  /* Returns whether the Option is Set */
  bool isSet();
};


#endif  //DIGITAL_OPTION_H