/*******************************************************************************
* Class Name: Window
* Date Created: June 18, 2016
* Inheritance: None
* Description:
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
*****************************************************************************/
#include "Box.h"
#include "EnumDb.h"
#include "Frame.h"
#include "Helpers.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window
{
public:
  Window()
      : alpha{0},
        point{Coordinate(0, 0)},
        status{WindowStatus::OFF},
        backdrop{nullptr} {};

public:
  /* Current alpha for rendering the window */
  uint8_t alpha;

  /* Resting point for the Window */
  Coordinate point;

  /* The location of the Window */
  Box location;

  /* Enumerated status of the Window */
  WindowStatus status;

  /* Window background */
  Frame* backdrop;
};

#endif //WINDOW_H
