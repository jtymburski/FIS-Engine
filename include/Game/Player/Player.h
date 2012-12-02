 /*****************************************************************************
* Class Name: Player
* Date Created: Sunday, November 4th, 2012
* Inheritance: None?
* Description: Person object on map
*  
* TODO: CONSTRUCTORS TO BE FINISHED
* TODO: CHANGE SPEED (CONSTANT) ?
******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include <QtGUI/QWidget>

class Player
{

public:

  Player();
  ~Player(); 

private:

  /* Tick speed for the maps */
  int kMAPSPEED; //TODO: Change speed ?

  /* Current party (Sleuth) object */
  Party* sleuth; 

  /* Your position on the current map */
  int xpos, ypos; //Your position on the current map

public:

  /* Gets the number of frames in the main persons sprite multiplied by the kMAPSPEED */
  int getSpeed(); 

  /* Gets the x-position on the map */
  int getXPos(); 

  /* Gets the y-position on the map */
  int getYPos();

  /* Sets the x-position on the map */
  void setXPos(int);

  /* Sets the y-position on the map */
  void setYPos(int);
};

#endif
