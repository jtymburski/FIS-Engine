
/********************************************************************************
* Class Name: Sector
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The Sector class
********************************************************************************/
#ifndef SECTOR_H
#define SECTOR_H
#include <QVector>
#include "SectorAction.h"
#include "Tile.h"

class Sector : public QWidget
{
public:
  /*Constructor function*/
  Sector(QWidget *parent = 0);

  /*Destructor function*/
  ~Sector();

private:
  /*Tiles included in sector*/
  QVector<Tile*> bound;

  /*Action called upon entry of sector*/
  SectorAction* enter_action;

  /*Action called upon exit of sector*/
  SectorAction* leave_action;

  /*Player is in sector*/
  bool inSector;

protected:
  void paintEvent(QPaintEvent *);

public:
  /*Sets all tiles in the sector to the given Status*/
  void setStatus(Status s);
};


#endif // SECTOR_H
