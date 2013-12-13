/*******************************************************************************
* Class Name: Signature [Declaration]
* Date Created: December 13th, 2013
* Inheritance: None
* Description: A Signature is an abstraction of a 2-D X by Y grid where on 
*              Bubby objects and Signature Link objects can be placed. This
*              allows equipment customization for Persons as players may choose
*              to use Bubbies which amplify their Person's (battle actor) stats
*              in a desired way.
*
*              A Signature also allows a Bubby to gain experience as only a 
*              Bubby that is attached to a Signature can gain experience.
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* Signature Links - [12-13-13]
* Pathfinding Chains - [12-13-13]
* Gene Machine Recipes [12-13-13]
*******************************************************************************/

#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <vector>
#include <iostream>

//#include "Game/Player/Bubby.h"
#include "EnumFlags.h"
#include "Helpers.h"

//typedef uint32_t u32;
typedef uint8_t   u8;

ENUM_FLAGS(SigState)
enum class SigState
{
  BUBBABLE    = 1 << 0, /* Bubbies can be added or removed from the Signature? */
  UNLOCKABLE  = 1 << 1, /* Cells can be opened or closed? */
  SIZEABLE    = 1 << 2, /* A blank signature can be resized? */
  LINKABLE    = 1 << 3, /* Can links be placed in the Signature? */ 
  RECIPE_MODE = 1 << 4  /* Signature for Gene Machine mode */
};

class Signature
{
public:
	/* Default Constructor */
	Signature();

	/* Generates a blank x by y Signature object, potentially with rand slots */
	Signature(const size_t x, const size_t y, const bool random = false);

	//Signature(const size_t x, const size_t y, 
	//	      std::vector<const std::pair<const u8, const u8>> closed);

private:

  /* Size of the Signature */
  size_t x;
  size_t y;

  /* Vector of Bubby objects contained within */
  //std::vector<Bubby*> bubby_map;

  /* Flags of the current signature */
  SigState flags;

  /* 2-D coordinate pairs where the Bubbby objects are located */
  //std::vector<std::vector<const std::pair<const u8, const u8>>> occupied;

  /* ------------ Constants --------------- */
  static const size_t kMIN_X;
  static const size_t kMIN_Y;
  static const size_t kMAX_X;
  static const size_t kMAX_Y;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  bool isEmpty();

  bool getFlag(SigState test_flag);

  bool setSize(const size_t new_x, const size_t new_y);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
};

#endif //SIGNATURE_H
