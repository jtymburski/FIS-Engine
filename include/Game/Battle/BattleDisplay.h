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

#include "Game/Battle/Battle.h"
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
  // TODO: Delete
  uint16_t offset;
  uint16_t offset_2;

  /* Background sprite */
  Sprite* background;
  
  /* Bar variables - for rendering */
  uint16_t bar_offset;
  
  /* Battle to be displayed */
  Battle* battle;

  /* The rendering battle bar */
  Frame* battle_bar;

  /* Mid scene overlays */
  std::vector<Sprite*> midlays; // TODO: Make overlay class when created

  /* Background overlays */
  std::vector<Sprite*> overlays; // TODO: Make overlay class when created

  /* The system options. Used for rendering, settings, etc */
  Options* system_options;

  /* ------------ Constants --------------- */
  const static uint16_t kBAR_OFFSET; /* Offset of bar off bottom */
  const static uint16_t kFOES_OFFSET; /* Offset of foes from top */
  const static uint16_t kFRIENDS_OFFSET; /* Offset of friends from bottom */
  const static uint8_t kMAX_LAYERS; /* Max number of layers that can be set */
  const static uint16_t kPERSON_OVERLAP; /* Rendering overlay of persons */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Get foes in battle */
  std::vector<Person*> getFoes();
  std::vector<Frame*> getFoesFrames();
  Party* getFoesParty();

  /* Get friends in battle */
  std::vector<Person*> getFriends();
  std::vector<Frame*> getFriendsFrames();
  Party* getFriendsParty();

  /* Renders the battle bar */
  bool renderBar(SDL_Renderer* renderer, uint16_t screen_width, 
                                         uint16_t screen_height);

  /* Renders the foes */
  bool renderFoes(SDL_Renderer* renderer, uint16_t screen_width);

  /* Renders the friends */
  bool renderFriends(SDL_Renderer* renderer, uint16_t screen_height);

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

  /* Gets the battle corresponding to the display */
  Battle* getBattle();

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

  /* Sets the battle connected to the display */
  bool setBattle(Battle* battle);

  /* Sets the rendering battle bar */
  bool setBattleBar(Frame* bar);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the midlay sprite - main one, removes all others */
  bool setMidlay(Sprite* midlay);

  /* Sets the overlay sprite - main one, removes all others */
  bool setOverlay(Sprite* overlay);

  /* Unsets the background sprite */
  void unsetBackground();

  /* Unsets the battle connected to the display */
  void unsetBattle();

  /* Unsets the rendering battle bar */
  void unsetBattleBar();

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
