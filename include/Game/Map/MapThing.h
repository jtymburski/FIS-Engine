/******************************************************************************
 * Class Name: MapThing
 * Date Created: October 28, 2012
 * Inheritance: QGraphicsObject
 * Description: This class handles the generic MapThing. It contains things on
 *              the map that don't fall under general scenary. It acts as the
 *              parent class to a sequence of others, for example, MapPerson,
 *              MapWalkOver, MapSolid, etc. Also handles the basic setup for 
 *              name, description, id, sprite. No interaction is handled in 
 *              this class since its a generic parent.
 *****************************************************************************/
#ifndef MAPTHING_H
#define MAPTHING_H

//#include <QDebug>
#include <QGraphicsObject>
#include <QPainter>

#include "Game/Map/MapState.h"

class MapThing : public QGraphicsObject
{
public:
  /* Constructor functions */
  MapThing();
  MapThing(MapState* state, int width, int height, QString name = "", 
           QString description = "", int id = kUNSET_ID);

  /* Destructor function */
  ~MapThing();

private:
  /* The animation delay, stored locally */
  int animation_delay;

  /* The thing classification */
  QString description;
  int height;
  int id;
  QString name;
  int width;

  /* The main state, the main one */
  MapState* state;

  /* -------------------------- Constants ------------------------- */
protected:
  const static int kANIMATION_OFFSET; /* The number of animate calls before
                                         the frame sequence is updated */
  const static int kMINIMUM_ID;       /* The minimum ID, for a thing */
  const static int kUNSET_ID;         /* The placeholder unset ID */

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Animates the thing. Skips the head of the list, if requested */
  bool animate(bool skip_head = false, bool just_started = false);
  
  /* Virtual bounding rectangle - The rectangle that encapsulates the item */
  QRectF boundingRect() const;
  
  /* Clears the entire class data */
  void clear();

  /* Gets the things decription */
  QString getDescription();

  /* Returns the height of the thing */
  int getHeight();

  /* Gets the things ID */
  int getID();

  /* Gets the things name */
  QString getName();

  /* Returns the map state that's defined */
  MapState* getState();
  
  /* Returns the width of the thing */
  int getWidth();

  /* Starts inteaction (conversation, giving something, etc) */
  void interaction();
  
  /* Virtual painter reimplementation - for painting the item */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
             QWidget* widget);
  
  /* Resets the animation of the thing, back to the starting frame */ 
  bool resetAnimation();

  /* Set the coordinates for the data in the map thing (for the parent) */
  void setCoordinates(int x, int y, int z = 0);

  /* Sets the things description */
  void setDescription(QString new_description);

  /* Sets the things height classification */
  bool setHeight(int new_height);

  /* Sets the things ID */
  bool setID(int new_id);

  /* Sets the things name */
  void setName(QString new_name);

  /* Sets the state of the thing */
  bool setState(MapState* state, bool unset_old = true);

  /* Sets the things width classification */
  bool setWidth(int new_width);

  /* Unsets the state, in the class */
  void unsetState(bool delete_state = true);
};

#endif // MAPTHING_H
