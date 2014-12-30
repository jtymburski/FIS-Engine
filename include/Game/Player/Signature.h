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
* - Signature Links    - [12-13-13]
* - Pathfinding Chains - [12-13-13]
* - Gene Machine Recipes [12-13-13]
*******************************************************************************/
#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <iostream>
#include <tuple>
#include <vector>

#include "Game/Player/Bubby.h"
#include "EnumDb.h"
#include "EnumFlags.h"
#include "Helpers.h"

/* Shorthand types */
using std::begin;
using std::end;

using uint16Pair = std::pair<uint16_t, uint16_t>;

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
  Cell(uint16_t a, uint16_t b);

private:
  /* Cell Data */
  Bubby* bubby_ptr;
  CellState state;
  
  uint16_t link_tier;
  uint16_t x;
  uint16_t y;

/*=============================================================================
 * CELL PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Clear the cell */
  void clear();

  /* Get the ptr to the Bubby of the Cell */
  Bubby* getBubby();

  /* Returns the current link tier level at the Cell */
  uint16_t getLinkTier();

  /* Returns the current state of the Cell */
  CellState getState();

  /* Returns the x-index of the Cell in the Sig Matrix */
  uint16_t getX();

  /* Returns the y-index of the Cell in the Sig Matrix */
  uint16_t getY();

  /* Assigns a new state to the Cell */
  bool setState(CellState new_state, Bubby* new_bubby = nullptr,
      uint16_t new_link_tier = 0);
};

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
  Signature(uint16_t x, uint16_t y, bool random = false);

  Signature(uint16_t x, uint16_t y, std::vector<uint16Pair> closed_cells);

private:
  /* Flags of the current signature */
  SigState flags;

  /* 2-D coordinate pairs where the Bubbby and Links are located */
  CellMatrix cells;

  /* ------------ Constants --------------- */
  static const uint16_t kMIN_X;
  static const uint16_t kMIN_Y;
  static const uint16_t kMAX_X;
  static const uint16_t kMAX_Y;
  static const uint16_t kMAX_LINK_TIER;

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Function for setting up flag values in the signature */
  void classSetup(bool bubbable);

  /* Clears and rebuilds the signature at a size x, y) */
  bool clearAndBuild(uint16_t x, uint16_t y);

  /* Returns the top left most coodinate of a Bubby at a given coordinate */
  uint16Pair getTopLeft(uint16_t a, uint16_t b);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Attempts to add an amount of experience to each Bubby in the Signature */
  bool addExp(uint32_t amount);

  /* Attempts to attach a given Bubby to a cell [a,b] */ 
  bool attach(uint16_t a, uint16_t b, Bubby* new_bubby);

  /* Attempts to close given Cell [a, b] */
  bool close(uint16_t a, uint16_t b);

  /* Determines whether the signature contains a Bubby of the given ID */
  bool hasID(int32_t id);

  /* Determines whether the signature is empty of Bubbies*/
  bool isEmpty();

  /* Determines whether the cell [a,b] is open for a given Tier of Bubby */
  bool isOpen(uint16_t a, uint16_t b, uint16_t tier = 1);

  /* Determines whether a given cell [a, b] is within a valid range */
  bool isInRange(uint16_t a, uint16_t b);

  /* Attempts to open a given cell [a, b] */
  bool open(uint16_t a, uint16_t b);

  /* Prints out the state of the signature */
  void print(bool print_cells = false);

  /* Unattaches a Bubby from all coordinates it contains on any occupied one */
  Bubby* unattachBubby(uint16_t a, uint16_t b);

  /* Computes the bonus AttributeSet from all bubbies */
  AttributeSet getBonusStats();

  /* Returns the pointer of a Bubby occupying the cell [a, b] */
  Bubby* getBubby(uint16_t a, uint16_t b);

  /* Returns the vector of pointers of all Bubbies contained */
  std::vector<Bubby*> getBubbies();

  /* Evaluates and returns the value of a given flag */
  bool getFlag(SigState test_flag);

  /* Determines the highest tier among Bubbies of a given Flavour */
  uint16_t getHighestTier(Flavour* flavour_check);

  /* Calculates and returns the total mass of the Signature. */
  uint32_t getMass();

  /* Calculates and returns a SkillSet bonus that the Signature grants */
  SkillSet* getSkillBonus();

  /* Returns a vector of unique flavours contained in the Signature */
  std::vector<Flavour*> getUniqueFlavours();

  /* Calculates and returns the money value of the Bubbies in the Signature */
  uint32_t getValue();

  /* Returns the x-dimensional width of the Signature. */
  uint16_t getX() const;

  /* Returns the y-dimensional length of the Signature */
  uint16_t getY() const;

  /* Assigns a SigState flag set to the Signature */
  void setConfig(const SigState new_config);

  /* Assigns a given SigState flag a given set value */
  void setFlag(const SigState flag, bool set_value = true);

  /* Assigns a new size of the signature (or rather, attempts to */
  bool setSize(uint16_t new_x, uint16_t new_y);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Methods for returning the possible sizes of a Signature */
  static constexpr uint16_t getMaxX(); //noexcept;
  static constexpr uint16_t getMaxY(); //noexcept;
  static constexpr uint16_t getMinX(); //noexcept;
  static constexpr uint16_t getMinY(); //noexcept;

  /* Methods for returning possible tiers of Links and E-Links */
  static constexpr uint16_t getMaxLinkTier();
};


#endif //SIGNATURE_H
