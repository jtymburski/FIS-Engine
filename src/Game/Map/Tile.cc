/******************************************************************************
* Class Name: Tile
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Tile class
******************************************************************************/
#include "Game/Map/Tile.h"

/* Constructor function */
Tile::Tile(QWidget *parent = 0)
{
}

/* Destructor function */
Tile::~Tile()
{
}

/* Animates all sprites on tile (Including thing and walkover sprites) */
void Tile::animate()
{
}

/* gets east passiblity */
bool Tile::getPassibilityEast()
{
    return east_passibility;
}

/* gets north passiblity */
bool Tile::getPassibilityNorth()
{
    return north_passibility;
}

/* gets south passiblity */
bool Tile::getPassibilitySouth()
{
    return south_passibility;
}

/* gets west passiblity */
bool Tile::getPassibilityWest()
{
    return west_passibility;
}

/* Sets all passibility */
void Tile::setPassibility(bool yorn)
{
}

/* Sets east passiblity */
void Tile::setPassibilityEast(bool yorn)
{
    east_passibility = yorn;
}

/* Sets north passiblity */
void Tile::setPassibilityNorth(bool yorn)
{
    north_passibility = yorn;
}

/* Sets south passiblity */
void Tile::setPassibilitySouth(bool yorn)
{
    south_passibility = yorn;
}

/* Sets west passiblity */
void Tile::setPassibilityWest(bool yorn)
{
    west_passibility = yorn;
}
