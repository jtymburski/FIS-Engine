/*******************************************************************************
* Class Name: Signature & Cell [Declaration]
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
*              A Cell is an element in the 2D Cell Matrix of a Signature.
*              A Cell ha a coordinate [x, y], a ptr to a Bubby object that
*              may be stored on it, and an enumerated state (open, closed, etc.)
*
* Notes
* -----
*
* [1]:
*
* TODO
* ----
* Signature Links    - [12-13-13]
* Pathfinding Chains - [12-13-13]
* Gene Machine Recipes [12-13-13]
*******************************************************************************/
#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <vector>
#include <iostream>

#include "Game/Player/Bubby.h"
#include "EnumDb.h"
#include "EnumFlags.h"
#include "Helpers.h"

/* Shorthand types */
using std::begin;
using std::end;
using uint32 = uint32_t;
using uint8 =  uint8_t;
using uint8Pair = std::pair<const uint8, const uint8>;

/* Enumerated Signature State flags - Signature configuration */
ENUM_FLAGS(SigState)
enum class SigState
{
  BUBBABLE    = 1 << 0, /* Bubbies can be added or rem from the Signature? */
  UNLOCKABLE  = 1 << 1, /* Cells can be opened or closed? */
  SIZEABLE    = 1 << 2, /* A blank signature can be resized? */
  LINKABLE    = 1 << 3, /* Can links be placed in the Signature? */ 
  RECIPE_MODE = 1 << 4  /* Signature for Gene Machine mode */
};

/*******************************************************************************
* Class Name: Cell [Declaration]
* Inheritance: None
*******************************************************************************/
struct Cell
{
public:
  /* Cell constructor */
  Cell(const uint8 a, const uint8 b);

private:
  /* Cell Data */
  Bubby* bubby_ptr;
  CellState state;
  uint8 link_tier;
  const uint8 x;
  const uint8 y;

/*=============================================================================
 * CELL PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Clear the cell */
  void clear();

  /* Get the ptr to the Bubby of the Cell */
  Bubby* getBubby();

  /* Returns the current link tier level at the Cell */
  uint8 getLinkTier();

  /* Returns the current state of the Cell */
  CellState getState();

  /* Returns the x-index of the Cell in the Sig Matrix */
  uint8 getX();

  /* Returns the y-index of the Cell in the Sig Matrix */
  uint8 getY();

  /* Assigns a new state to the Cell */
  void setState(const CellState new_state, Bubby* const new_bubby = nullptr,
  	            const uint8 new_link_tier = 0);

};

/* Alias for 1-D and 2-D Cell vectors */
using CellRow = std::vector<Cell>;
using CellMatrix = std::vector<CellRow>;

/*******************************************************************************
* Class Name: Signature [Implementation]
* Inheritance: None
*******************************************************************************/

class Signature
{
public:
  /* Default Constructor */
  Signature();

  /* Generates a blank x by y Signature object, potentially with rand slots */
  Signature(const size_t x, const size_t y, const bool random = false);

  Signature(const size_t x, const size_t y, std::vector<uint8Pair> closed_cells);

private:
  /* Flags of the current signature */
  SigState flags;

  /* 2-D coordinate pairs where the Bubbby and Links are located */
  CellMatrix cells;

  /* ------------ Constants --------------- */
  static const size_t kMIN_X;
  static const size_t kMIN_Y;
  static const size_t kMAX_X;
  static const size_t kMAX_Y;
  static const uint8 kMAX_LINK_TIER;
  static const uint8 kMAX_E_LINK_TIER;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  void classSetup(const bool bubbable);

  void clearAndBuild(const size_t x, const size_t y);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  bool attach(const uint8 a, const uint8 b, Bubby* new_bubby);

  bool close(const uint8 a, const uint8 b);

  bool hasID(const int id_check);

  inline bool isEmpty();

  bool isOpen(const uint8 a, const uint8 b, const uint8 tier = 1);

  bool inRange(const uint8 a, const uint8 b);

  bool open(const uint8 a, const uint8 b);

  void print();

  Bubby* unattachBubby(const uint8 a, const uint8 b);

  Bubby* getBubby(const uint8 a, const uint8 b);

  std::vector<Bubby*> getBubbies();

  bool getFlag(SigState test_flag);

  uint8 getHighestTier(Flavour* flavour_check);

  double getMass();

  std::vector<Flavour*> getUniqueFlavours();

  void setConfig(SigState new_config);

  void setFlag(SigState flag, const bool set_value = true);

  bool setSize(const size_t new_x, const size_t new_y);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Methods for returning the possible sizes of a Signature */
  static size_t getMaxX();
  static size_t getMaxY();
  static size_t getMinX();
  static size_t getMinY();

  /* Methods for returning possible tiers of Links and E-Links */
  static uint8 getMaxLinkTier();
  static uint8 getMaxELinkTier();
};

#endif //SIGNATURE_H
