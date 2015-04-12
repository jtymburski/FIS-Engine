/*******************************************************************************
* Class Name: KeyHandler [Declaration]
* Date Created: April 9, 2015
* Inheritance: None
* Description: KeyHandler manages the state of the Keyboard. It contains a 
*              vector of keys which map an enumerated GameKey (or game function
*              key) to physical SDL_Keycode key(s) and keeps track of which
*              keeps are depressed and how long they have been depressed for.
*
* Notes
* -----
*
* [1]: The time for a key to be considered 'held' is set in a constant.
* [2]: Whether multiple mapping are permitted is set in a constant.
*
* TODO
* ----
* [04-11-15]: Mapping of GameKey to multiple physical keys.
* [04-11-15]: Convert game KeyDownEvents to grab the state of the keyboard
*****************************************************************************/
#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <SDL2/SDL.h>
#include <vector>

#include "EnumDb.h"
#include "Helpers.h"

/* Key Map Structure */
struct Key
{
public:
  /* Key constructor */
  Key(GameKey new_key, SDL_Keycode new_keycode, bool enabled = true);

  /* The enumerated key value */
  GameKey game_key;

  /* The mapped SDL Keycode for that enumerated key value */
  SDL_Keycode keycode;

  /* Whether that key is depressed */
  bool depressed;

  /* Whether operation of that Key is currently enabled */
  bool enabled;

  /* How long that key has been depressed for */
  int32_t time_depressed;
};

class KeyHandler
{
public:
  /* Constructs the Handler of Keys */
  KeyHandler();

  ~KeyHandler() = default;

private:
  /* Vector of Keys */
  std::vector<Key> keys;

  /* ------------ Constants --------------- */
  static const SDL_Keycode kMoveLeftDefault;  /* Default moving left key */
  static const SDL_Keycode kMoveRightDefault; /* Default moving right key */
  static const SDL_Keycode kMoveUpDefault;    /* Default moving up key */
  static const SDL_Keycode kMoveDownDefault;  /* Default moving down key */
  static const SDL_Keycode kMenuDefault;      /* Default menu open key */
  static const SDL_Keycode kActionDefault;    /* Default action key */
  static const SDL_Keycode kCancelDefault;    /* Default cancel/close key */
  static const SDL_Keycode kRunDefault;       /* Default run key */
  static const SDL_Keycode kDebugDefault;     /* Default debug key */

  static const int32_t kMinHeldTime; /* Amount of time for key to be 'Held' */
  static const bool    kMultipleMappings; /* Can GameKeys be multi-mapped? */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Prints out the information of of one Key */
  void printIndex(Key key);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Checks if a Certain Game key is in a 'held' state */
  bool isHeld(GameKey game_key);
  bool isHeld(SDL_Keycode keycode, bool* found);

  /* Checks if a Certain Game key is in a depressed state */
  bool isDepressed(GameKey game_key);
  bool isDepressed(SDL_Keycode keycode, bool* found);

  /* Checks if a certain Key has an enabled use */
  bool isEnabled(GameKey game_key);
  bool isEnabled(SDL_Keycode keycode, bool* found);

  /* Checks to see if a given keycode has already been mapped */
  bool isKeycodeMapped(SDL_Keycode keycode);

  /* Load a default set of Keys */
  void loadDefaults();
  
  /* Prints out the Key states of the KeyHandler */
  void print(bool only_depressed = false, bool only_held = false);

  /* Update function */
  bool update(int32_t cycle_time);

  /* Functions to obtain keys */
  Key& getKey(GameKey game_key);
  Key& getKey(SDL_Keycode keycode, bool* found);

  /* Set Keys */
  bool setKey(GameKey game_key, SDL_Keycode new_keycode);

  /* Sets a Key to be enabled/disabled */
  void setEnabled(GameKey game_key, bool enabled = true);
  bool setEnabled(SDL_Keycode keycode, bool enabled = true);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Method to return min-held time constant */
  static int32_t getMinHeldTime();
};

#endif //KEYHANDLER_H
