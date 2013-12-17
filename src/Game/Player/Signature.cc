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

Signature::Signature(const size_t x, const size_t y, 
	                 std::vector<uint8Pair> closed_cells)
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

uint8Pair Signature::getTopLeft(const uint8 a, const uint8 b)
{
  auto cell_bubby = getBubby(a, b);

  if (cell_bubby != nullptr)
  {
    if (cell_bubby->getTier() == 1)
      return std::make_pair(a, b);

    for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
      for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
        if ((*it_e).getBubby() != nullptr)
          if ((*it_e).getBubby()->getID() == cell_bubby->getID())
            return std::make_pair((*it_e).getX(), (*it_e).getY());
  }

  return std::make_pair(kMAX_X + 1, kMAX_Y + 1);
}

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

bool Signature::inRange(const uint8 a, const uint8 b)
{
  if (a < cells.size())
  {
    if (cells.size() > 0)
      if (b < cells.at(0).size())
        return true;
  }

  return false;
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


void Signature::print(const bool print_cells)
{
  std::cout << "--- Signature ---\n";
  std::cout << "Size: X: " << cells.size() << " Y: ";

  if (cells.size() > 0)
    std::cout << cells.at(0).size();
  else
    std::cout << 0;
  std::cout << "\n";

  std::cout << "BUBBABLE: " << getFlag(SigState::BUBBABLE) << "\n";
  std::cout << "UNLOCKABLE: " << getFlag(SigState::UNLOCKABLE) << "\n";
  std::cout << "SIZEABLE: " << getFlag(SigState::SIZEABLE) << "\n";
  std::cout << "LINKABLE: " << getFlag(SigState::LINKABLE) << "\n";
  std::cout << "RECIPE_MODE: " << getFlag(SigState::RECIPE_MODE) << "\n";
 
  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
  {
    std::string line{"| "};

    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
    {
      if (print_cells)
      {
        std::cout << "--- Cell ---\n";
        std::cout << "Bubby Ptr?" << ((*it_e).getBubby() != nullptr) << "\n";
        std::cout << "Link Tier: " << (*it_e).getLinkTier() << "\n";
        std::cout << "State: " << Helpers::cellToStr((*it_e).getState()) << "\n";
        std::cout << "X: " << (*it_e).getX() << " Y: " << (*it_e).getY() << "\n";
        std::cout << "--- /Cell ---\n";
      }
      else
      {
        if ((*it_e).getState() == CellState::OPEN)
          line += " O |";
        else if ((*it_e).getState() == CellState::BUBBY)
          line += " B |";
        else if ((*it_e).getState() == CellState::CLOSED)
          line += " C |";
        else if ((*it_e).getState() == CellState::BLOCKED)
          line += " X |";
        else if ((*it_e).getState() == CellState::LINK)
          line += " L |";
        else if ((*it_e).getState() == CellState::E_LINK)
          line += " E |";
      }
   }
     if (!(print_cells))
       std::cout << line << "\n";
  }
}

Bubby* Signature::unattachBubby(const uint8 a, const uint8 b)
{
  auto cell_bubby = getBubby(a, b);

  if (cell_bubby != nullptr)
  {
    auto tier = static_cast<uint8>(cell_bubby->getTier());
    auto top_left = getTopLeft(a, b);

    if (inRange(top_left.first, top_left.second))
      for (auto i = top_left.first; i < top_left.first + tier; i++)
        for (auto j = top_left.second; j < top_left.second + tier; j++)
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

uint8 Signature::getHighestTier(Flavour* flavour_check)
{
  auto tier = 0;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
  {
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
    {
      if ((*it_e).getBubby() != nullptr)
      {
        auto cell_bubby = (*it_e).getBubby();

        if (cell_bubby->getType() == flavour_check &&
        	cell_bubby->getTier() > static_cast<uint32_t>(tier))
          tier = cell_bubby->getTier();
      }
    }
  }

  return tier;
}

double Signature::getMass()
{
  auto temp_mass = 0.0;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getBubby() != nullptr)
        temp_mass += (*it_e).getBubby()->getMass();

  return temp_mass;
}

std::vector<Flavour*> Signature::getUniqueFlavours()
{
  std::vector<Flavour*> flavour_list;
  std::vector<int> ids;
  
  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
  {
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
    {
      if ((*it_e).getBubby() != nullptr)
      {
        auto type = (*it_e).getBubby()->getType();
        auto result = std::find(begin(ids), end(ids), type->getGameID());

        if (result != end(ids))
        {
          flavour_list.push_back(type);
          ids.push_back(type->getGameID());	
        }
      }
    }
  }

  return flavour_list;
}

uint32 Signature::getValue()
{
  uint32 temp_value = 0;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getBubby() != nullptr)
        temp_value += (*it_e).getBubby()->getValue();
    
  return temp_value;
}

uint8 Signature::getX() const 
{
  return cells.size();
}

uint8 Signature::getY() const 
{
  if (getX() > 0)
    return cells.at(0).size();

  return 0;
}

void Signature::setConfig(const SigState new_config)
{
  flags = new_config;
}

void Signature::setFlag(const SigState flag, const bool set_value)
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

constexpr size_t Signature::getMaxX() noexcept
{
  return kMAX_X;
}

constexpr size_t Signature::getMaxY() noexcept
{
  return kMAX_Y;
}

constexpr size_t Signature::getMinX() noexcept
{
  return kMIN_X;
}

constexpr size_t Signature::getMinY() noexcept
{
  return kMIN_Y;
}

constexpr uint8 Signature::getMaxLinkTier() noexcept
{
  return kMAX_LINK_TIER;
}

constexpr uint8 Signature::getMaxELinkTier() noexcept
{
  return kMAX_E_LINK_TIER;
}