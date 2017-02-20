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

#include "EnumDb.h"
#include "Helpers.h"
#include "StringDb.h"
#include <SDL2/SDL.h>
#include <vector>

enum class KeyMode
{
  DISABLED,
  INPUT,
  TEXT_ENTRY,
  NAME_ENTRY
};

/* Key Map Structure */
struct Key
{
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

private:
  /* Vector of Keys */
  std::vector<Key> keys;

  /* Keys which are valid for text entry */
  std::vector<Key> text_keys;

  /* Current mode of the KeyHandler */
  KeyMode mode;

  /* The running text entry string */
  std::string text;

  /* ------------ Constants --------------- */
  static const SDL_Keycode kMOVE_LEFT_DEFAULT;  /* Default moving left key */
  static const SDL_Keycode kMOVE_RIGHT_DEFAULT; /* Default moving right key */
  static const SDL_Keycode kMOVE_UP_DEFAULT;    /* Default moving up key */
  static const SDL_Keycode kMOVE_DOWN_DEFAULT;  /* Default moving down key */
  static const SDL_Keycode kMENU_DEFAULT;       /* Default menu open key */
  static const SDL_Keycode kACTION_DEFAULT;     /* Default action key */
  static const SDL_Keycode kCANCEL_DEFAULT;     /* Default cancel/close key */
  static const SDL_Keycode kBACKSPACE_DEFAULT;  /* Default backspace key */
  static const SDL_Keycode kRUN_DEFAULT;        /* Default run key */
  static const SDL_Keycode kDEBUG_DEFAULT;      /* Default debug key */
  static const SDL_Keycode kPAUSE_DEFAULT;      /* Default pause key */

  static const int32_t kMIN_HELD_TIME; /* Amount of time for key to be 'Held' */
  static const bool kMULTIPLE_MAPPINGS; /* Can GameKeys be multi-mapped? */

  /*======================== PRIVATE FUNCTIONS ===============================*/
private:
  /* Prints out the information of of one Key */
  void printIndex(Key key);

  /* Updates the state of a given Key */
  void updateKey(Key& key, int32_t cycle_time, KeyMode call_mode);

  /* Add the current key depression to the string */
  void addKeyEntry(Key& key);

  /* Remove the last key entry on the KeyHandler */
  void removeKeyEntry();

  /*========================= PUBLIC FUNCTIONS ===============================*/
public:
  /* Clears the current state of text entry */
  void clearTextEntry();

  /* Returns the enumerated mode of the KeyHandler */
  KeyMode getMode();

  /* Checks if a Certain Game key is in a 'held' state */
  bool isHeld(GameKey game_key);
  bool isHeld(SDL_Keycode keycode);

  /* Checks if a Certain Game key is in a depressed state */
  bool isDepressed(GameKey game_key);
  bool isDepressed(SDL_Keycode keycode);

  /* Checks if a certain Key has an enabled use */
  bool isEnabled(GameKey game_key);
  bool isEnabled(SDL_Keycode keycode, bool* found);

  /* Checks to see if a given keycode has already been mapped */
  bool isKeycodeMapped(SDL_Keycode keycode);

  /* Checks if a key has been depressed and not held (sets to held) */
  bool isStruck(GameKey game_key);
  bool isStruck(SDL_Keycode keycode, bool* found);

  /* Load a default set of Keys */
  void loadDefaults();

  /* Prints out the Key states of the KeyHandler */
  void print(bool only_depressed = false, bool only_held = false);

  /* Update function */
  bool update(int32_t cycle_time);

  /* Functions to obtain keys */
  Key& getKey(GameKey game_key);
  Key& getKey(SDL_Keycode keycode, bool* found);

  /* Returns the current state of text entry */
  std::string getTextEntry();

  /* Set Keys */
  bool setKey(GameKey game_key, SDL_Keycode new_keycode);

  /* Assign a new enumerated mode to the KeyHandler */
  void setMode(KeyMode mode);

  /* Sets a Key to be enabled/disabled */
  void setEnabled(GameKey game_key, bool enabled = true);
  bool setEnabled(SDL_Keycode keycode, bool enabled = true);

  /* Set a given GameKey to be held */
  bool setHeld(GameKey key);

  /* Unassigns a current GameKey so that it can be assigned */
  bool unsetKey(GameKey key);

  /*===================== PUBLIC STATIC FUNCTIONS ============================*/
public:
  /* Method to return min-held time constant */
  static int32_t getMinHeldTime();
};

#endif // KEYHANDLER_H
