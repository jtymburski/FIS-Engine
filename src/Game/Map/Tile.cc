/******************************************************************************
* Class Name: Tile
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Tile class
******************************************************************************/
#include "Game/Map/Tile.h"

/* Constructor function */
Tile::Tile(int width, int height, int x, int y, QWidget* parent)
{
  setParent(parent);
  setGeometry(x, y, width, height);
  show();
}

/* Destructor function */
Tile::~Tile()
{
  delete base;
}

/* Animates all sprites on tile (Including thing and walkover sprites) */
void Tile::animate()
{
  //update();
}

/* Paint event for the tile */
void Tile::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, base->getCurrent());
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

/* Sets the base tile */
bool Tile::setBase(QString path)
{
  base = new Sprite(path);
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
