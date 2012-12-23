/******************************************************************************
* Class Name: Tile
* Date Created: Dec 2 2012
* Inheritance: QWidget
* Description: The Tile class
******************************************************************************/
#include "Game/Map/Tile.h"

/* Constructor function */
Tile::Tile(QWidget* parent)
{
  base = new Sprite(":/zipper_", 15, "png");

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(100);

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
}

/* Paint event for the tile */
void Tile::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, base->getCurrentAndShift());
}

/* Paint event for the tile */
void Tile::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Up)
  {
    timer->setInterval(timer->interval() - 1);
  }
  else if(event->key() == Qt::Key_Down)
  {
    timer->setInterval(timer->interval() + 1);
  }
  else if(event->key() == Qt::Key_Left)
  {
    base->setDirectionReverse();
  }
  else if(event->key() == Qt::Key_Right)
  {
    base->setDirectionForward();
  }

  qDebug() << timer->interval();
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
