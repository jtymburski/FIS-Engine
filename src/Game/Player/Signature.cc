/*******************************************************************************
* Class Name: Cell [Implementation]
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
 * CELL CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

Cell::Cell(const uint8 a, const uint8 b)
  : bubby_ptr(nullptr)
  , state(CellState::OPEN)
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

/*******************************************************************************
* Class Name: Signature [Implementation]
* Description:
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
    {
      for (auto j = b; j < b + tier; j++)
      {
        cells[i][j].state = CellState::BUBBY;
        cells[i][j].bubby_ptr = new_bubby;
      }
    }
    
  }

  return can_attach;
}

bool Signature::close(const uint8 a, const uint8 b)
{
  if (isOpen(a, b))
  {
    cells[a][b].state = CellState::CLOSED;

    return true;
  }

  return false;
}

bool Signature::hasID(const int id_check)
{
  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).bubby_ptr != nullptr)
        if ((*it_e).bubby_ptr->getID() == id_check)
          return true;

  return false;
}

bool Signature::isEmpty()
{
  auto empty = true;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      empty &= (*it_e).state == CellState::OPEN;

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
        is_open &= (cells[i][j].state == CellState::OPEN);

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
  if (inRange(a, b) && cells.at(a).at(b).state == CellState::CLOSED)
  {
    cells[a][b].state = CellState::OPEN;

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
    {
      for (auto j = b; j < b + tier; j++)
      {
        cells[i][j].state = CellState::OPEN;
        cells[i][j].bubby_ptr = nullptr;
      }
    }
  }
  
  return cell_bubby;
}

Bubby* Signature::getBubby(const uint8 a, const uint8 b)
{
  if (inRange(a, b))
    return cells[a][b].bubby_ptr;

  return nullptr;
}

std::vector<Bubby*> Signature::getBubbies()
{
  std::vector<Bubby*> bubby_vec;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).state == CellState::BUBBY)
        bubby_vec.push_back((*it_e).bubby_ptr);

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

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/