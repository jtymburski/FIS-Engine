/******************************************************************************
* Class Name: Signature
* Date Created: July 28, 2013
* Inheritance:
* Description: Abstraction of an equipment's signature. Contains information
*              on where Bubbies can be "attached" to equipment.
******************************************************************************/

#include "Game/Player/Signature.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const ushort Signature::kMAX_X = 9;
const ushort Signature::kMAX_Y = 9;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Default Signature object
 */
Signature::Signature() {}

/*
 * Description: Creates a Signature object given a two dimensional size.
 *
 * Inputs: size_x - size of the signature in the x dimension
 *         size_y - size of the signature in the y dimension
 */
Signature::Signature(ushort x, ushort y)
{
  resize(x, y);
}

/*
 * Description: Creates a Signature object given a two dimensional size and a
 *              vector of pairs which correspond to closed positions in the
 *              signature.
 *
 * Inputs: size_x - size of the signature in the x dimension
 *         size_y - size of the signature in the y dimension
 */
Signature::Signature(ushort x, ushort y,
                     std::vector<std::pair<ushort, ushort> > closed)
{
  resize(x, y);

  for (int i = 0; i < closed.size(); i++)
    close(closed.at(i).first, closed.at(i).second);
}

/*
 * Description: Annihilates a Signature object
 */
Signature::~Signature() {}

/*=============================================================================
* PUBLIC FUNCTIONS
*============================================================================*/

/*
 * Description: Attempts to attach a given Bubby pointer to a cell (representing
 *              the top left most coordinate that the Bubby will occupy.
 *
 * Inputs: ushort x - the left most coordinate the Bubby will occupy
 *         ushort y - the top most coordinate the Bubby will occupy
 * Output: bool - true if the Bubby was successfully attached
 */
bool Signature::attach(ushort x, ushort y, Bubby* new_bubby)
{
  /* Check Tier 1 is open */
  bool can_attach = isOpen(x, y);
  std::vector< std::pair<ushort, ushort> > new_positions;
  new_positions.push_back(std::make_pair(x, y));

  /* If needed, check Tier 2 is open */
  if (new_bubby->getTier() > 1)
  {
    if (!isOpen(x + 1, y) || !isOpen(x, y + 1) || !isOpen(x + 1, y + 1))
      can_attach = false;

    new_positions.push_back(std::make_pair(x + 1, y));
    new_positions.push_back(std::make_pair(x, y + 1));
    new_positions.push_back(std::make_pair(x + 1, y + 1));
  }

  /* If needed, check Tier 3 is open */
  if (new_bubby->getTier() > 2)
  {
    if (!isOpen(x + 2, y) || !isOpen(x + 2, y + 1) || !isOpen(x + 2, y + 2) ||
        !isOpen(x, y + 2) || !isOpen(x + 1, y + 2))
      can_attach = false;

    new_positions.push_back(std::make_pair(x + 2, y));
    new_positions.push_back(std::make_pair(x + 2, y + 1));
    new_positions.push_back(std::make_pair(x + 2, y + 2));
    new_positions.push_back(std::make_pair(x, y + 2));
    new_positions.push_back(std::make_pair(x + 1, y + 2));
  }

  if (can_attach)
  {
    /* Add the Bubby to the vector of pointers */
    bubby_map.push_back(new_bubby);

    /* Add the new positions to the positions vector on the new index */
    positions.push_back(new_positions);
  }
}

/*
 * Description: Attempts to close a cell of the Signature
 *
 * Inputs: ushort x - x-coordinate of cell
 *         ushort y - y-coordinate of cell
 * Output: bool - true if the cell is now closed
 */
bool Signature::close(ushort x, ushort y)
{
  for (int i = 0; i < closed.size(); i++)
    if (closed[i].first == x && closed[i].second == y)
      return true;
  return false;
}

/*
 * Description: Evaluation of whether the Signature has zero Bubbies
 *
 * Inputs: one
 * Output: bool - true if the Signature is completely empty.
 */
bool Signature::isEmpty()
{
  return bubby_map.isEmpty();
}

/*
 * Description: Evaluation whether a single cell is open.
 *
 * Inputs: ushort x - x-coordinate of cell
 *         ushort y - y-coordinate of cell
 * Output: bool - true if the cell is both unoccupied and not closed
 */
bool Signature::isOpen(ushort x, ushort y)
{
  if (getBubby(x, y) == 0)
    return true;
  else
    return false;
}

/*
 * Description: Attempts to open a cell of the Signature
 *
 * Inputs: ushort x - x-coordinate of cell
 *         ushort y - y-coordinate of cell
 * Output: bool - true if the cell is now open
 */
bool Signature::open(ushort x, ushort y)
{
  if (getBubby(x, y) != 0)
    return false;

  for (int i = 0; i < closed.size(); i++)
  {
    if (closed[i].first == x && closed[i].second == y)
    {
      closed.erase(closed.begin() + i);
      return true;
    }
  }
}

/*
 * Description: Attempts to unattach a Bubby from the Signature
 *
 * Inputs: ushort x - x-coordinate of the Bubby to be unattached
 *         ushort y - y-coordinate of the Bubby to be unattached
 * Output: Bubby* - pointer to the Bubby if unattached, NULL [0] if not.
 */
Bubby* Signature::unattach(ushort x, ushort y)
{
  Bubby* unattachment = getBubby(x, y);
  int index = -1;

  /* Find the index the Bubby is stored */
  for (int i = 0; i < bubby_map.size(); i++)
    if (bubby_map.at(i)->getId() == unattachment->getId())
      index = i;

  /* Bubby exists and will be unattached */
  if (index != -1 && unattachment != 0)
  {
    positions[index].clear();
    bubby_map[index] = NULL;
    bubby_map.remove(index);
  }

  return unattachment;
}

/*
 * Description: Returns a pointer to the Bubby at a given index
 *
 * Inputs: ushort x - x-coordinate of the Bubby
 *         ushort y - y-coordinate of the Bubby
 * Output: Bubby* - pointer to the Bubby
 */
Bubby* Signature::getBubby(ushort x, ushort y)
{
  for (int i = 0; i < positions.size(); i++)
    for (int j = 0; j < positions[i].size(); j++)
      if (positions[i][j].first == x && positions[i][j].second == y)
        return bubby_map.at(i);

  return 0;
}

/*
 * Description: Returns the entire vectors of Bubbies attached to the Signature
 *
 * Inputs: none
 * Output: QVector<Bubby*> - the vector of attached Bubbies
 */
QVector<Bubby*> Signature::getBubbyMap()
{
  return bubby_map;
}

/*
 * Description: Evalutes kMAX_X
 *
 * Inputs: none
 * Output: ushort - the maximum x-dimension size of the signature
 */
ushort Signature::getMaxX()
{
  return kMAX_X;
}

/*
 * Description: Evaluates kMAX_Y
 *
 * Inputs: none
 * Output: ushort - the maximum y-dimension size of the signature
 */
ushort Signature::getMaxY()
{
  return kMAX_Y;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Returns the top-left-most coordinate a Bubby occupies
 *              given a known coordinate of the Bubby
 *
 * Inputs: ushort x - known x-coordinate of the Bubby
 *         ushort y - known y-coordinate of the Bubby
 * Output: std::pair<ushort, ushort> - the top left most coordinate of the Bubby
 */
std::pair<ushort, ushort> Signature::getTopLeft(ushort x, ushort y)
{
  Bubby* attached_bubby = getBubby(x, y);
  ushort tier = attached_bubby->getTier();
  int attached_id = attached_bubby->getId();

  std::pair<ushort, ushort> top_left;

  /* Tier 1 Bubbies are 1 x 1 */
  if (tier == 1)
  {
    top_left.first = x;
    top_left.second = y;
  }

  /* Tier 2 Bubbies are 2 x 2 */
  if (tier > 1)
  {
    if (x > 1 && getBubby(x - 1, y)->getId() == attached_id)
      top_left.first = x - 1;
    if (y > 1 && getBubby(x , y - 1)->getId() == attached_id)
      top_left.second = y - 1;
  }

  /* Tier 3 Bubbies are 3 x 3 */
  else if (tier > 2)
  {
    if (x > 2 && getBubby(x - 2, y)->getId() == attached_id)
      top_left.first = x - 2;
    if (y > 2 && getBubby(x, y - 2)->getId() == attached_id)
      top_left.second = y - 2;
  }

  return top_left;
}

/*
 * Description: Attempts to resize the Bubby Signature to a new size
 *
 * Notes [1]: The Bubby Signature must be emptied to be resized.
 *
 * Inputs: ushort size_x - the new x-dimensional size of the Signature
 *         ushort size_y - the new y-dimensional size of the Signature
 * Output: bool - true if the Signature was resized correctly.
 */
bool Signature::resize(ushort size_x, ushort size_y)
{
  bool resized = true;

  if (size_x > getMaxX() || size_y > getMaxY() || !isEmpty())
    resized = false;

  if (resized)
  {
    x = size_x;
    y = size_y;
  }

  return resized;
}
