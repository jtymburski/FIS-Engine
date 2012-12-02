/******************************************************************************
* Class Name: MapThing
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The MapThing class
******************************************************************************/
#ifndef MAPTHING_H
#define MAPTHING_H

#include <QtGui/QWidget>

//#include "Game/EnumDatabase.h" // TODO: Remove? [12-02-12]
#include "Game/Sprite.h"

class MapThing : public QWidget
{
public:
  /* Constructor function */
  MapThing();

  /* Destructor function */
  ~MapThing();

private:
  /* The things description */
  QString description;

  /* The id for the thing (Not currently used in the design, may need later) */
  int id;

  /* The things name */
  QString name;

  /* The facing sprites for the thing */
  Sprite* north,east,south,west;

protected:
  void paintEvent(QPaintEvent*);

public:
  /* Starts inteaction (conversation, giving something etc, reimplemented in
    subclasses polymorphically, called from Map class) */
  void interaction();

  /* Gets the things decription */
  QString getDescription();

  /* Gets the things name */
  QString getName();

  /* Gets the directional sprite of the thing */
  Sprite getSprite(Direction dir);
};

#endif // MAPTHING_H
