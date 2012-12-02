/******************************************************************************
* Class Name: Weather
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The Weather class
******************************************************************************/
#ifndef WEATHER_H
#define WEATHER_H

//#include <QSound>
#include <QtGui/QWidget>

#include "Game/Sprite.h"

class Weather : public QWidget
{
public:
  /* Constructor function */
  Weather(QWidget *parent = 0);

  /* Destructor function */
  ~Weather();

protected:
  void paintEvent(QPaintEvent *);

private:
  /* Whether the weather is enabled or not */
  bool enabled;

  /* The seamless sound that repeats while the weather is enabled */
  //QSound* sound;

  /* The speed that the sprite moves across the screen */
  int speed;

  /* The seamless texture that represents the weather condition */
  Sprite* texture_overlay;

public:
  /* Flips the weather bit */
  void toggleEnabled();

  /* Gets the weather enabled status */
  bool getEnabled();

  /* Gets the weather sound */
  //QSound* getSound();

  /* Gets the weather speed */
  int getSpeed();

  /* Gets the weather sprite */
  Sprite* getSprite();

  /* Sets the weather enabled */
  void setEnabled(bool s);

  /* Sets the weather sound */
  //void setSound(QSound* s);

  /* Sets the weather speed */
  void setSpeed(int s);

  /* Sets the weather sprite */
  void setSprite(Sprite* s);
};

#endif // WEATHER_H
