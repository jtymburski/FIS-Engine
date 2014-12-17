/*******************************************************************************
 * Class Name: BattleDisplay [Declaration]
 * Date Created: December 16, 2014
 * Inheritance: None
 * Description: 
 *
 * Notes
 * -----
 *
 * [1]:
 *
 * TODO
 * ----
 ******************************************************************************/

#ifndef BATTLEDISPLAY_H
#define BATTLEDISPLAY_H

#include "Helpers.h"
#include "Options.h"
#include "Sprite.h"

class BattleDisplay
{
public:
  /* Constructor function */
  BattleDisplay(Options* running_config = NULL);

  /* Destructor function */
  ~BattleDisplay();

private:
  /* Background sprite */
  Sprite* background;

  /* Mid scene overlays */
  std::vector<Sprite*> midlays; // TODO: Make overlay class when created

  /* Background overlays */
  std::vector<Sprite*> overlays; // TODO: Make overlay class when created

  /* The system options. Used for rendering, settings, etc */
  Options* system_options;

  /* ------------ Constants --------------- */
  const static uint8_t kMAX_LAYERS; /* Max number of layers that can be set */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Trims the midlay vector of NULL sprite pointers */
  void trimMidlays();

  /* Trims the overlay vector of NULL sprite pointers */
  void trimOverlays();

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/
public:

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/
protected:

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Adds a midlay sprite - appends to end of stack */
  bool addMidlay(Sprite* midlay);

  /* Adds an overlay sprite - appends to end of stack */
  bool addOverlay(Sprite* overlay);

  /* Clears the class data */
  void clear();

  /* Get the background */
  Sprite* getBackground();

  /* Get the midlay(s) */
  Sprite* getMidlay(uint8_t index);
  std::vector<Sprite*> getMidlays();

  /* Get the overlay(s) */
  Sprite* getOverlay(uint8_t index);
  std::vector<Sprite*> getOverlays();

  /* Renders the battle display */
  bool render(SDL_Renderer* renderer);

  /* Sets the background sprite */
  bool setBackground(Sprite* background);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the midlay sprite - main one, removes all others */
  bool setMidlay(Sprite* midlay);

  /* Sets the overlay sprite - main one, removes all others */
  bool setOverlay(Sprite* overlay);

  /* Unsets the background sprite */
  void unsetBackground();

  /* Unsets the midlay sprite(s) */
  void unsetMidlay(uint8_t index);
  void unsetMidlays();

  /* Unsets the overlay sprite(s) */
  void unsetOverlay(uint8_t index);
  void unsetOverlays();

  /* Updates the battle display */
  bool update(int cycle_time);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:

};

#endif // BATTLEDISPLAY_H
