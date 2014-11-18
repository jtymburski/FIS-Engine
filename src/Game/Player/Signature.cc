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

/*
 * Description: Constructs a Cell object given a size a x b)
 *
 * Inputs: 
 */
Cell::Cell(const uint8 a, const uint8 b)
    : bubby_ptr{nullptr}
    , state{CellState::OPEN}
    , link_tier{0}
    , x{a}
    , y{b}
{}

/*=============================================================================
 * CELL PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: This function clears the data contained in the Cell, including
 *              assigning a nullptr to the bubby ptr and declaring the cell open
 *
 * Inputs: none
 * Output: none
 */
void Cell::clear()
{
  bubby_ptr  = nullptr;
  state      = CellState::OPEN;
  link_tier  = 1;
}

/*
 * Description: Returns the Bubby pointer of the Cell signifying the Cell which
 *              is equipped with a Bubby object (thereby gaining experience for
 *              the Bubby and boosting the parent Equipment's stats)
 *
 * Inputs: none
 * Output: Bubby* - pointer to the Bubby object equipped
 */
Bubby* Cell::getBubby()
{
  return bubby_ptr;
}

/*
 * Description: Returns an 8-bit value for a link tier ~(Lv. 1 - 15)
 *
 * Inputs: none
 * Output: uint8 - unsigned value for link tier level of the curret cell
 */
uint8 Cell::getLinkTier()
{
  return link_tier;
}

/*
 * Description: Returns the enumerated cell state (ex. OPEN or CLOSED)
 *
 * Inputs: none
 * Output: CellState - enumerated state of the current cell.
 */
CellState Cell::getState()
{
  return state;
}

/*
 * Description: Returns the x-dimensional width of the Cell object
 *
 * Inputs: none
 * Output: uint8 - the x-dimensional width (less than the max width)
 */
uint8 Cell::getX()
{
  return x;
}

/*
 * Description: Retunrs the y-dimensional length of the Cell oject
 *
 * Inputs: none
 * Output: uint8 - the y-dimensional length (less than the max lenght)
 */
uint8 Cell::getY()
{
  return y;
}

/*
 * Description: Assigns state values to the current cell, including an 
 *              enumerated cell state (ex. OPEN, closed, a pointer to the
 *              Bubby object to be assigned and a value for a link tier).
 *
 * Inputs: CellState - enumerated CellState of the Signature (open/closed)
 *         Bubby* - new pointer to a Bubby object for the Cell
 *         uint8 - new unsgned value for link tier Lvl at the current cell
 * Output: bool - true if the assigned state is a valid state
 */
bool Cell::setState(const CellState new_state, Bubby* const new_bubby,
    const uint8 new_link_tier)
{
  auto valid_state = false;
  auto valid_link_tier = new_link_tier;

  // valid_state &= (new_state == CellState::OPEN && new_link_tier == 0)
  
  if (new_bubby != nullptr && new_link_tier != 0)
    valid_state = false;

  // if (new_state == CellState:LINK && valid_link_tier == 0)
  //   valid_state = false;
  // else if (new_state == CellState::LINK)
  //   Helpers::setInRange(valid_link_tier, 0, Signature::kMAX_LINK_TIER);

  if (valid_state)
  {
    state      = new_state;
    bubby_ptr  = new_bubby;
    link_tier  = valid_link_tier;
  }

  return valid_state;
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

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Default constructor for a signature, calls the setupClass
 *              function creating a non-Bubbable signature.
 * Inputs: none
 */
Signature::Signature()
{
  classSetup(false);
}

/*
 * Description: General constructor for a signature, creates a Signature object
 *              with a width size y by length size y and with random or 
 *              non-random closed cells.
 *
 * Inputs: size_t x - the width size to create the signature
 *         size_t y - the length size to create the signature
 *         bool random - whether to close random # of cells randomly
 */
Signature::Signature(const size_t x, const size_t y, const bool random)
{
  classSetup(true);

  if (setSize(x, y))
  {
    if (random)
    {
    	/* Random # closed cells */
      auto cells = Helpers::randInt(static_cast<int>(x) * static_cast<int>(y));

      /* Randomly close that number of cells -- some may be repeats */
      for (int i = 0; i < cells; i++)
      {
        close(static_cast<uint8>(Helpers::randInt(kMAX_X)), 
            static_cast<uint8>(Helpers::randInt(kMAX_Y)));
      }
    }
  }
  else
    std::cerr << "[Error] Invalid sig of size: " << x << " by " << y << "\n";
}

/*
 * Description: Another general constructor for signature, creates a signature
 *               given a size_t x-width by size_t y-length and a vector of
 *               [x, y] uint8_t pairs for cell values to close.
 *
 * Inputs: size_t x - the width to create the signature.
 *         size_t y - the legnth to create the width
 *         std::vector<uint8Pair> closed_cells
 */
Signature::Signature(const size_t x, const size_t y, 
	                 std::vector<uint8Pair> closed_cells)
  : flags(static_cast<SigState>(0))
{
  classSetup(true);

  if (setSize(x, y))
  {
    for (auto closed : closed_cells)
      if (isOpen(closed.first, closed.second))
        close(closed.first, closed.second);
  }
  else
  {
    std::cerr << "[Error] Invalid sig of size: " << x << " by " << y << "\n";
  }
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Class setup function. Sets up signatures with default flags 
 *              depending on whether to create a bubbable signature or not.
 *
 * Inputs: bool bubbable - true if the Signature can have bubbies assigned to it
 * Output: none
 */
 void Signature::classSetup(const bool bubbable)
 {
   flags = static_cast<SigState>(0);

   setFlag(SigState::BUBBABLE, bubbable);
   setFlag(SigState::SIZEABLE, true);
   setFlag(SigState::UNLOCKABLE, true);
   setFlag(SigState::LINKABLE, true);
   setFlag(SigState::RECIPE_MODE, false);
 }

/*
 * Description: Clears each cell of the signature (removes the Cell vector
 *              objects) and creates new cells (default Cell:clear function)
 *              by a given size x by given size y)
 *
 * Inputs: size_t x - given width size to clear and rebuild cells for
 *         size_t y - gven legnth size to clear and rebuild cells for
 * Output: bool - true if the function operated properly
 */
bool Signature::clearAndBuild(const size_t x, const size_t y)
{
  Helpers::setInRange(x, kMIN_X, kMAX_X);
  Helpers::setInRange(y, kMIN_Y, kMAX_Y);

  cells.clear();

  CellRow new_row;
  for (size_t i = 0; i < x; i++)
  {
    for (size_t j = 0; j < y; j++)
      new_row.push_back(Cell(static_cast<uint8>(i), static_cast<uint8>(j)));

    cells.push_back(new_row);
    new_row.clear();
  }

  return true;
}

/*
 * Description: Returns the top-left most occupied Cell matching the ID of the
 *              Bubby in the given cell [a, b].
 *
 * Note [1]: Example, if a 2x2 Bubby is occupying a squares [0, 0], [0,1], [0,2]
 *           and [0,3], given any of those coordinates, the top left-most 
 *           coordinate [0, 0] will be returned.
 *
 * Inputs: uint8 - x-coordinate of the ID of a Bubby to be checked
 *         uint8 - y-coordinate of the ID of a Bubby to be checked
 *
 * Output: uint8Pair - pair of uint8s of the matching top-left coordinate of
 *                    the found Bubby or, [max + 1, max + 1] if invalid
 */
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

/*
 * Description: Adds experience to each Bubby contained in the signature given
 *              an amount of signature to be added.
 *
 * Inputs: uint32_t amount - the amount of experience to be added to each Bubby
 * Output: bool - true if all the Bubbies were able to have exp. added
 */
bool Signature::addExp(const uint32_t amount)
{
  auto bubbies = getBubbies();
  auto added   = true;

  for (auto bubby : bubbies)
    if (bubby != nullptr)
      added &= bubby->addExperience(amount);

  return added;
}

/*
 * Description: Attempts to attach a Bubby to a Cell a given coordinate [a, b].
 *              This function asserts that the given Cell is in the open state
 *              and that the Bubby ID does not currently match any other ID
 *
 * Inputs: uint8 a - given x-coordinate of Bubby to be attached
 *         uint8 b - given y-coordinate of Bubby to be attached
 *         new_bubby - the Bubby ptr to be attached at the current cell.
 * Output: bool - true if the attachment occurd successfully
 */
bool Signature::attach(const uint8 a, const uint8 b, Bubby* new_bubby)
{
  /* Assert the Signature can have Bubbies added to it */
  auto can_attach = getFlag(SigState::BUBBABLE);

  /* Assert all corresponding cells are open and the Bubby ID does not match */
  if (can_attach && new_bubby != nullptr)
  {
    uint8 tier = static_cast<uint8>(new_bubby->getTier());
    can_attach &= isOpen(a, b, tier);
    can_attach &= !hasID(new_bubby->getID());
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

/*
 * Description: Attempts to close a given cell coordinate [a, b]. For a cell
 *              to be closed, the cell must first be in an Open State.
 *
 * Inputs: uint8 a - x coordinate for the Cell to be closed.
 *         uint8 b - y coordinate for the Cell to be closed.
 * Output: bool - true if the Cell was successfully closed
 */
bool Signature::close(const uint8 a, const uint8 b)
{
  if (isOpen(a, b))
  {
    cells[a][b].setState(CellState::CLOSED);

    return true;
  }

  return false;
}

/*
 * Description: Checks every Cell in the Signature for a Bubby matching
 *              a given ID and returns true if the ID was found.
 *
 * Inputs: int id_check - the ID to check the Signature for.
 * Output: bool - true if the ID for a Bubbt was found on the Signature.
 */
bool Signature::hasID(const int id_check)
{
  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getBubby() != nullptr)
        if ((*it_e).getBubby()->getID() == id_check)
          return true;

  return false;
}

/*
 * Description: Determines whether the Signature is empty (that is, there exists
 *              no Bubbies in the Signature). This function is important to
 *              determine whether the Signature is resizeable or not.
 *
 * Inputs: none
 * Output: bool - true if all Cells are non-Bubbified.
 */
bool Signature::isEmpty()
{
  auto empty = true;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      empty &= !((*it_e).getState() == CellState::BUBBY);

  return empty;
}

/*
 * Description: Determines whether a given cell coordinate [a, b] is open
 *
 * Inputs: uint8 - x-coordinate of cell to determine status of
 *         uint8 - y-coordinate of cell to determine status of
 * Output: bool - true if the given Signature cell is in an open state
 */
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

/*
 * Description: Determines whether a given coordinate [a, b] is valid on the
 *              current signature.
 *
 * Inputs: uint8 a - x-coordinate to check for being in range.
 *         uint8 b - y-coordinate to check for being in range
 * Output: bool - true if the coordinate [a, b] is in the valid range.
 */
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

/*
 * Description: Attempts to open a given Signature cell at coordinate [a, b]. 
 *              The opening of a Signature cell is only possible if the current
 *              cell [a, b] is in a closed state. Locked or otherwise occupied
 *              cells cannot be opened.
 *
 * Inputs: uint8 a - x-coordinate of cell to be opened
 *         uint8 b - y-coordinate of cell to be opened
 * Output: bool - true if the cell can and has been opened
 */
bool Signature::open(const uint8 a, const uint8 b)
{
  if (inRange(a, b) && cells.at(a).at(b).getState() == CellState::CLOSED)
  {
    cells[a][b].setState(CellState::OPEN);

    return true;
  }

  return false;
}

/*
 * Description: Prints out the information or the state of the Signature object.
 *              Can also turn on/off printing out details of each cell Info.
 *
 * Inputs: const bool print_cells - whether to print out the signature state.
 * Output: none
 */
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
      }
   }
     if (!(print_cells))
       std::cout << line << "\n";
  }
}

/*
 * Description: Attempts to grab the Bubby pointer attached to a given cell
 *              coordinate [a, b], and re-openes each cell that that Bubby was
 *              attached to
 *
 * Inputs: uint8 a - the x-coordinate to grab the Bubby for.
 *         uint8 b - the y-coordinate to grab the Bubby for
 * Output: Bubby* - pointer to the Bubby to be grabbed from the signature.
 */
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

/*
 * Description: Computes the total bonus AttributeSet from all Bubbies contained
 *              within the Signature.
 *
 * Inputs: none
 * Output: AttributeSet - the computed AttributeSet computed
 * //TODO: [08-23-14] - Compute bonus stats for the Signature
 */
AttributeSet Signature::getBonusStats()
{
  //TODO: Stat bonus computation
  return AttributeSet();
}

/*
 * Description: Returns the Bubby pointer at a given coordinate [a, b]
 *
 * Inputs: uint8 - x coordinate of the cell to grab the Bubby for.
 *         uint8 - y coordinate of the cell to grab the Bubby for
 * Output: Bubby* - pointer to the Bubby at the coordinate [a, b], or nullptr
 */
Bubby* Signature::getBubby(const uint8 a, const uint8 b)
{
  if (inRange(a, b))
    return cells.at(a).at(b).getBubby();

  return nullptr;
}

/*
 * Description: Returns the vector of Bubby pointers that are currently attached
 *              to this signature.
 *
 * Inputs: none
 * Output: std::vector<Bubby*> - the vector of Bubby pointers contained within
 */
std::vector<Bubby*> Signature::getBubbies()
{
  std::vector<Bubby*> bubby_vec;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getState() == CellState::BUBBY)
        bubby_vec.push_back((*it_e).getBubby());

  return bubby_vec;
}

/*
 * Description: Returns the boolean value of a given SigState flag.
 *
 * Inputs: test_flag - test_flag to be checked for.
 * Output: bool - the value of the given flag
 */
bool Signature::getFlag(SigState test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

/*
 * Description: Returns the highest tiered Bubby among all Bubbies of a given
 *              flavour in the Signature.
 *
 * Inputs: Flavour* flavour_check - pointer to a given Flavour object.
 * Output: uint8 - the highest tiered Bubby of the given Flavour
 */
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

/*
 * Description: Calculates and returns the total mass of the Signature (by 
 *              adding up all the mass of each contained Bubby.)
 *
 * Inputs: none
 * Output: double - the calculated mass of the Signature
 */
double Signature::getMass()
{
  auto temp_mass = 0.0;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getBubby() != nullptr)
        temp_mass += (*it_e).getBubby()->getMass();

  return temp_mass;
}

/*
 * Description: Returns a constructed SkillSet pointer of Skills which the
 *              Signature will grant to the Equipment bearer.
 *
 * Inputs: none
 * Output: SkillSet* - pointer to constructed SkillSet for the Signature.
 * //TODO: [08-24-14]: Calculate the Skill Bonus the Bubbies in the Signature 
 */
SkillSet* Signature::getSkillBonus()
{
  //TODO: Find skill Bonus
  return nullptr;
}

/*
 * Description: Calculates and returns a vector fo Flavour pointers of all
 *              unique Flavour types contained within the Signature.
 *
 * Inputs: none
 * Output: std::vector<Flavour*> - vector of unique flavour pointers.
 */
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

/*
 * Description: Calculates and returns the total value of the Bubbies contained
 *              within the Signature. This function is likely handy in selling
 *              an Equipment object with Bubbies attached to the Signature.
 *
 * Inputs: none
 * Output: uint32 - the total value of the Signature
 * TODO: [08-23-14]: Should this function take into account value_modifier?
 */
uint32 Signature::getValue()
{
  uint32 temp_value = 0;

  for (auto it_r = begin(cells); it_r != end(cells); ++it_r)
    for (auto it_e = begin(*it_r); it_e != end(*it_r); ++it_e)
      if ((*it_e).getBubby() != nullptr)
        temp_value += (*it_e).getBubby()->getValue();
    
  return temp_value;
}

/*
 * Description: Returns the current x-width size of the signature.
 *
 * Inputs: none
 * Output: uint8 - the x-width of the current signature.
 */
uint8 Signature::getX() const 
{
  return cells.size();
}

/*
 * Description: Returns the current y-width size of the signature.
 *
 * Inputs: none none
 * Output: uint8 - the y-width of the current signature
 */
uint8 Signature::getY() const 
{
  if (getX() > 0)
    return cells.at(0).size();

  return 0;
}

/*
 * Description: Assigns a new config (flag set) for the Signature.
 *
 * Inputs: new_config - flag set to be assigned to the Signature.
 * Output: none
 */
void Signature::setConfig(const SigState new_config)
{
  flags = new_config;
}

/*
 * Description: Assigns a given SigState flag a given boolean set_value.
 *
 * Inputs: sigState flag - given SigState flag to be assigned.
 *          bool set_value - the boolean value to assign teh flag to.
 * Output: none
 */
void Signature::setFlag(const SigState flag, const bool set_value)
{
  (set_value) ? (flags |= flag) : (flags &= ~flag);
}

/*
 * Description: Attempts to assign a size to the signature. For the signature
 *              to be resized, the new_x and new_y values must be within range
 *              and the Signature must be SIZEABLE (flag) and empty of Bubbies.
 *
 * Inputs: size_t new_x - the new x-coordinate of the new size of Signature
 *         size_t new_y - the new y-coordinate of the new size of Signature
 * Output: bool - true if the resizing happened successfully.
 */
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

/*
 * Description: Returns the static value for the largest x-coordinate
 *
 * Inputs: none
 * Output: size_t - the largest y-coordinate for a signature
 */
constexpr size_t Signature::getMaxX() noexcept
{
  return kMAX_X;
}

/*
 * Description: Returns the static value for the largest y-coordinate
 *
 * Inputs: none
 * Output: size_t - the largest y-coordinate for a signature
 */
constexpr size_t Signature::getMaxY() noexcept
{
  return kMAX_Y;
}

/*
 * Description: Returns the static value for the smallest x-coordinate
 *
 * Inputs: none
 * Output: size_t - smallest static value for the x-coordinate.
 */
constexpr size_t Signature::getMinX() noexcept
{
  return kMIN_X;
}

/*
 * Description: Returns the static value for the smallest y-coordinate.
 *
 * Inputs: none
 * Output: size_t - smallest static value for the y-coordinate
 */
constexpr size_t Signature::getMinY() noexcept
{
  return kMIN_Y;
}

/*
 * Description: Returns the static value for the maximum level standard link.
 *
 * Inputs: none
 * Output: uint8 - largest static value for standard link tier
 */
constexpr uint8 Signature::getMaxLinkTier() noexcept
{
  return kMAX_LINK_TIER;
}