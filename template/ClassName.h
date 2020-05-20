/*******************************************************************************
* Class Name: ClassName [Header]
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

#ifndef CLASSNAME_H
#define CLASSNAME_H

struct Window
{
  Window()
      : alpha{0},
        point{Coordinate(0, 0)},
        status{WindowStatus::OFF},
        backdrop{nullptr} {};

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


class ClassName
{
public:

private:

  /* ------------ Constants --------------- */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/
public:

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:

};

#endif // CLASSNAME_H
