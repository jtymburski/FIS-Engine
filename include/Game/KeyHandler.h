/*******************************************************************************
* Class Name: ClassName [Declaration]
* Date Created: June 22, 2014
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
  Key(GameKey new_key, SDL_Keycode new_keycode);

  /* The enumerated key value */
  GameKey game_key;

  /* The mapped SDL Keycode for that enumerated key value */
  SDL_Keycode keycode;

  /* Whether that key is depressed */
  bool depressed;

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
  static const SDL_Keycode kDebugDefault;     /* Default debug key */

  static const int32_t kMinHeldTime; /* Amount of time for key to be 'Held' */

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

  /* Functions to assign a key's state to depressed */
  void setDepressed(GameKey game_key);
  bool setDepressed(SDL_Keycode keycode);

  /* Set Keys */
  bool setKey(GameKey game_key, SDL_Keycode new_keycode);

  /* Functions to assign a key's state to unpressed */
  void setUnpressed(GameKey game_key);
  bool setUnpressed(SDL_Keycode keycode);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:


};

#endif //KEYHANDLER_H
