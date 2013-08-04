/******************************************************************************
* Class Name: Signature
* Date Created: July 28, 2013
* Inheritance: None
* Description: Abstraction of an equipment's signature. Contains information
*              on where Bubbies can be "attached" to equipment. The Signature
*              has a size X by Y (up to kMAX_X and kMAX_Y. Pointers to the
*              Bubbies stored in the Signature are contained within the
*              QList bubby_map. Positions which can not be occupied by a bubby
*              are in the std::vector closed_cells, whereas the positions which
*              are occupied by Bubbies are stored in the 2D std::vector of
*              std::pairs in such a way that occupied_cells[i] is the vector of
*              positions which are occupied by bubby_map[i].
******************************************************************************/
#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <QDebug>
#include <QList>
#include <utility>

#include "Game/Player/Bubby.h"

class Signature
{
public:
  /* Creates a Signature object */
  Signature();
  Signature(ushort x, ushort y);
  Signature(ushort x, ushort y, std::vector<std::pair<ushort, ushort> > list);

  /* Annihilates a Signature object */
  ~Signature();

  /* Enumerated flags for Signature */
  enum SignatureState
  {
    BUBBABLE    = 1ul << 0, /* Can the signature add Bubbies? */
    CHANGEABLE  = 1ul << 1, /* Can the signature have cells opened/closed? */
    RESIZEABLE  = 1ul << 2  /* Can the signature be resized? */
  };
  Q_DECLARE_FLAGS(SignatureFlags, SignatureState);
  SignatureFlags flag_set;

private:
  /* The size of the signature */
  ushort x;
  ushort y;

  /* Vector of Bubbies the signature is pointing to */
  QList<Bubby*> bubby_map;

  /* 2D Vector of coordinate pairs where Bubbies are located */
  std::vector<std::vector<std::pair<ushort,ushort> > > occupied_cells;

  /* Coordinates that are closed, all others open */
  std::vector<std::pair<ushort, ushort> > closed_cells;

  /* -------------------- Constants ----------------- */
  static const ushort kMIN_X;
  static const ushort kMIN_Y;
  static const ushort kMAX_X;
  static const ushort kMAX_Y;

/*=============================================================================
* PUBLIC FUNCTIONS
*============================================================================*/
public:
  /* Attach a Bubby to a given cell */
  bool attach(ushort x, ushort y, Bubby* new_bubby);

  /* Closes a particular valid open position without a bubby */
  bool close (ushort x, ushort y);

  /* Evaluates whether the signatures contains no Bubbies */
  bool isEmpty();

  /* Evaluates the state of a given cell */
  bool isOpen(ushort x, ushort y);

  /* Opens a particular valid closed position */
  bool open(ushort x, ushort y);

  /* Unattach a Bubby from a given cell */
  Bubby* unattach(ushort x, ushort y);

  /* Returns a pointer to a Bubby at the given cell, if one exists */
  Bubby* getBubby(ushort x, ushort y);

  /* Returns the entire Bubby map */
  QList<Bubby*> getBubbyMap();

  /* Gets the boolean value of a SignatureState flag */
  bool getFlag(SignatureState flag);

  /* Returns the highest Tier of a given flavour name */
  ushort getHighestTier(QString name);

  /* Evalutes and returns the mass of the Signature */
  double getMass();

  /* Returns the value of the maximum size of the signature */
  ushort getMaxX();
  ushort getMaxY();

  /* Returns a QList of the unique flavours the signature has */
  QList<BubbyFlavour*> getUniqueFlavours();

  /* Sets the value of a Signature flag */
  void setFlag(SignatureState flag, bool set_value = true);

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Gets the top left corner coordinate of a Bubby */
  std::pair<ushort, ushort> getTopLeft(ushort x, ushort y);

  /* Attempts to resize the Bubby signature */
  bool resize(ushort size_x, ushort size_y);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Signature::SignatureFlags);

#endif // SIGNATURE_H
