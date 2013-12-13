/*******************************************************************************
* Class Name: x [Implementation]
* Date Created: 
* Inheritance: 
* Description:
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Signature.h"

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/

const size_t Signature::kMIN_X = 0;
const size_t Signature::kMIN_Y = 0;
const size_t Signature::kMAX_X = 10;
const size_t Signature::kMAX_Y = 10;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
 Signature::Signature()
  : x(kMIN_X)
  , y(kMIN_Y)
  , flags(static_cast<SigState>(0))
 {

 }

Signature::Signature(const size_t x, const size_t y, const bool random)
{
  if (setSize(x, y))
  {
    if (random)
    {

    }
  }
  else
    std::cerr << "Invalid sig of size: " << x << " by " << y << "\n";
}

//Signature::Signature(const size_t x, const size_t y, 
//		             std::vector<const std::pair<const u8, const u8>> closed)
//{
//
//}


/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Signature::isEmpty()
{
  return true; //TODO
}

bool Signature::getFlag(SigState test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

bool Signature::setSize(const size_t new_x, const size_t new_y)
{
  //if (new_x >= kMIN_X && new_x = kMIN_Y && new_y >= kMIN_Y && new_y <= kMIN_Y)
  //{
  	if (isEmpty() && getFlag(SigState::SIZEABLE))
  	{
      x = new_x;
      y = new_y;

      return true;
    }
  //}

  return false;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/