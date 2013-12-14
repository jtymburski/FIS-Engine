/*******************************************************************************
* File Name: Signature & Cell [Implementation]
* Date Created: December 13th, 2013
*
* See .h file for TODOs
*******************************************************************************/

#include "Game/Player/Signature.h"

/*******************************************************************************
* Class Name: Cell [Implementation]
* Inheritance: None
* Description: A Cell is an element in the 2D Cell Matrix of a Signature.
*              A Cell ha a coordinate [x, y], a ptr to a Bubby object that
*              may be stored on it, and an enumerated state (open, closed, etc.)
*
* See .h file for TODOs
*******************************************************************************/

/*=============================================================================
 * CELL CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Cell::Cell(const uint8 a, const uint8 b)
  : bubby_ptr(nullptr)
  , state(CellState::OPEN)
  , link_tier(0)
  , x(a)
  , y(b)
{}

/*=============================================================================
 * CELL PUBLIC FUNCTIONS
 *============================================================================*/

void Cell::clear()
{
  bubby_ptr = nullptr;
  state     = CellState::OPEN;
}

Bubby* Cell::getBubby()
{
  return bubby_ptr;
}

uint8 Cell::getLinkTier()
{
  return link_tier;
}

CellState Cell::getState()
{
  return state;
}

uint8 Cell::getX()
{
  return x;
}

uint8 Cell::getY()
{
  return y;
}

void Cell::setState(const CellState new_state, Bubby* const new_bubby,
	                const uint8 new_link_tier)
{
  state      = new_state;
  bubby_ptr  = new_bubby;
  link_tier  = new_link_tier;
}

/*******************************************************************************
* Class Name: Signature [Implementation]
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
* See .h file for TODOs
*******************************************************************************/

/*=============================================================================
 * CONSTANTS - See implementation for details
 *============================================================================*/

const size_t Signature::kMIN_X  {1};
const size_t Signature::kMIN_Y  {1};
const size_t Signature::kMAX_X {10};
const size_t Signature::kMAX_Y {10};
const uint8 Signature::kMAX_LINK_TIER{15};
const uint8 Signature::kMAX_E_LINK_TIER{15};

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Signature::Signature()
{
  classSetup(false);
}

Signature::Signature(const size_t x, const size_t y, const bool random)
{
  classSetup(true);

  if (setSize(x, y) && random)
  {
  	/* Random # closed cells */
    auto cells = Helpers::randInt(static_cast<int>(x) * static_cast<int>(y));

    /* Randomly close that number of cells -- some may be repeats */
    for (int i = 0; i < cells; i++)
      close(static_cast<uint8>(Helpers::randInt(kMAX_X)), 
      	    static_cast<uint8>(Helpers::randInt(kMAX_Y)));
  }
  else
    std::cerr << "Invalid sig of size: " << x << " by " << y << "\n";
}

Signature::Signature(const size_t x, const size_t y, std::vector<uint8Pair> closed_cells)
  : flags(static_cast<SigState>(0))
{
  classSetup(true);

  if (setSize(x, y))
    for (auto closed : closed_cells)
      if (isOpen(closed.first, closed.second))
        close(closed.first, closed.second);
  else
    std::cerr << "Invalid sig of size: " << x << " by " << y << "\n";
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

 void Signature::classSetup(const bool bubbable)
 {
   flags = static_cast<SigState>(0);

   if (bubbable)
     setFlag(SigState::BUBBABLE, true);

   setFlag(SigState::UNLOCKABLE, true);
   setFlag(SigState::LINKABLE, true);
   setFlag(SigState::RECIPE_MODE, false);
 }

void Signature::clearAndBuild(const size_t x, const size_t y)
{
  cells.clear();

  CellRow new_row;
  for (size_t i = 0; i < x; i++)
  {
    for (size_t j = 0; j < y; j++)
      new_row.push_back(Cell(static_cast<uint8>(i), static_cast<uint8>(j)));

    cells.push_back(new_row);
    new_row.clear();
  }
}

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool Signature::attach(const uint8 a, const uint8 b, Bubby* new_bubby)
{
  /* Assert the Signature can have Bubbies added to it */
  auto can_attach = getFlag(SigState::BUBBABLE);

  /* Assert all corresponding cells are open and the Bubby ID does not match */
  if (can_attach && new_bubby != nullptr)
  {
    uint8 tier = static_cast<uint8>(new_bubby->getTier());
    can_attach &= isOpen(a, b, tier);
    can_attach &= hasID(new_bubby->getID());
  }
  else
    can_attach = false;

  if (can_attach)
  {
  	uint8 tier = static_cast<uint8>(new_bubby->getTier());

    /* Determine and assign States to all required cells */
    for (auto i = a; i < a + tier; i++)
      for (auto j = b; j < b + tier; j++)
        cells[i][j].setState(CellState::BUBBY, new_bubby);
  }

  return can_attach;
}

bool Signature::close(const uint8 a, const uint8 b)
{
  if (isOpen(a, b))
  {
    cells[a][b].setState(CellState::CLOSED);

    return true;
  }

  return false;
}

bool Signature::hasID(const int id_check)
{
  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getBubby() != nullptr)
        if ((*it_e).getBubby()->getID() == id_check)
          return true;

  return false;
}

bool Signature::isEmpty()
{
  auto empty = true;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      empty &= (*it_e).getState() == CellState::OPEN;

  return empty;
}

bool Signature::isOpen(const uint8 a, const uint8 b, const uint8 tier)
{
  if (tier < 1)
    return false;

  auto is_open = true;

  for (auto i = a; i < a + tier; i++)
    for (auto j = b; j < b + tier; j++)
      if (inRange(i, j))
        is_open &= (cells.at(i).at(j).getState() == CellState::OPEN);

  return is_open;
}

//TODO
bool Signature::inRange(const uint8 a, const uint8 b)
{
  (void)a;
  (void)b;
  return true;
}

bool Signature::open(const uint8 a, const uint8 b)
{
  if (inRange(a, b) && cells.at(a).at(b).getState() == CellState::CLOSED)
  {
    cells[a][b].setState(CellState::OPEN);

    return true;
  }

  return false;
}

//TODO
void Signature::print()
{

}

Bubby* Signature::unattachBubby(const uint8 a, const uint8 b)
{
  auto cell_bubby = getBubby(a, b);

  if (cell_bubby != nullptr)
  {
    auto tier = static_cast<uint8>(cell_bubby->getTier());

    for (auto i = a; i < a + tier; i++)
      for (auto j = b; j < b + tier; j++)
        cells[i][j].setState(CellState::OPEN);
  }
  
  return cell_bubby;
}

Bubby* Signature::getBubby(const uint8 a, const uint8 b)
{
  if (inRange(a, b))
    return cells.at(a).at(b).getBubby();

  return nullptr;
}

std::vector<Bubby*> Signature::getBubbies()
{
  std::vector<Bubby*> bubby_vec;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getState() == CellState::BUBBY)
        bubby_vec.push_back((*it_e).getBubby());

  return bubby_vec;
}

bool Signature::getFlag(SigState test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

void Signature::setConfig(SigState new_config)
{
  flags = new_config;
}

void Signature::setFlag(SigState flag, const bool set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

bool Signature::setSize(const size_t new_x, const size_t new_y)
{
  auto can_set  = Helpers::isInRange(new_x, kMIN_X, kMAX_X);
       can_set &= Helpers::isInRange(new_y, kMIN_X, kMAX_X);
 
  if (can_set && isEmpty() && getFlag(SigState::SIZEABLE))
    clearAndBuild(new_x, new_y);

  return can_set;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

size_t Signature::getMaxX()
{
  return kMAX_X;
}

size_t Signature::getMaxY()
{
  return kMAX_Y;
}

size_t Signature::getMinX()
{
  return kMIN_X;
}

size_t Signature::getMinY()
{
  return kMIN_Y;
}

uint8 Signature::getMaxLinkTier()
{
  return kMAX_LINK_TIER;
}

uint8 Signature::getMaxELinkTier()
{
  return kMAX_E_LINK_TIER;
}