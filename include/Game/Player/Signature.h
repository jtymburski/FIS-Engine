/******************************************************************************
* Class Name: Signature
* Date Created: July 28, 2013
* Inheritance:
* Description: Abstraction of an equipment's signature. Contains information
*              on where Bubbies can be "attached" to equipment.
******************************************************************************/
#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <QVector>
#include <QDebug>
#include <vector>
#include <utility>

#include "Game/Player/Bubby.h"

class Signature
{
public:
  /* Creates a Signature object */
  Signature();
  Signature(ushort x, ushort y);
  Signature(ushort x, ushort y,
            std::vector< std::pair<ushort, ushort>  > closed);

  /* Annihilates a Signature object */
  ~Signature();

private:
  /* The size of the signature */
  ushort x;
  ushort y;

  /* Vector of Bubbies the signature is pointing to */
  QVector<Bubby*> bubby_map;

  /* Pairs of positions where Bubbies are located */
  std::vector< std::vector< std::pair<ushort,ushort> > > positions;

  /* Pairs of positions that are closed, all others open */
  std::vector< std::pair<ushort, ushort> > closed;

  /* -------------------- Constants ----------------- */
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
  QVector<Bubby*> getBubbyMap();

  /* Returns the value of the maximum size of the signature */
  ushort getMaxX();
  ushort getMaxY();

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Gets the top left corner coordinate of a Bubby */
  std::pair<ushort, ushort> getTopLeft(ushort x, ushort y);

  /* Attempts to resize the Bubby signature */
  bool resize(ushort size_x, ushort size_y);
};

#endif // SIGNATURE_H
