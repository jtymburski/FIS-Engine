/*******************************************************************************
* Class Name: KeyHandler [Implementation]
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
* See .h file for TODOs
******************************************************************************/
#include "Game/KeyHandler.h"

/*=============================================================================
 * KEY HANDLER
 *============================================================================*/

/*
 * Description: Constructs a Key object given a functional GameKey, SDL keycode
 *              and an enabled boolean (defaulted to true)
 *
 * Inputs: GameKey new_key - enumerated game function key
 *         SDL_Keycode new_keycode - the keycode to map the key to
 *         bool enabled - true if the Key is set to be useable by default
 */
Key::Key(GameKey new_key, SDL_Keycode new_keycode, bool enabled)
    : game_key{new_key},
      keycode{new_keycode},
      depressed{false},
      enabled{enabled},
      time_depressed{0}
{
}

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const SDL_Keycode KeyHandler::kMOVE_LEFT_DEFAULT = SDLK_LEFT;
const SDL_Keycode KeyHandler::kMOVE_RIGHT_DEFAULT = SDLK_RIGHT;
const SDL_Keycode KeyHandler::kMOVE_UP_DEFAULT = SDLK_UP;
const SDL_Keycode KeyHandler::kMOVE_DOWN_DEFAULT = SDLK_DOWN;
const SDL_Keycode KeyHandler::kMENU_DEFAULT = SDLK_d;
const SDL_Keycode KeyHandler::kACTION_DEFAULT = SDLK_SPACE;
const SDL_Keycode KeyHandler::kCANCEL_DEFAULT = SDLK_BACKSPACE;
const SDL_Keycode KeyHandler::kRUN_DEFAULT = SDLK_LSHIFT;
const SDL_Keycode KeyHandler::kDEBUG_DEFAULT = SDLK_f;
const SDL_Keycode KeyHandler::kPAUSE_DEFAULT = SDLK_RCTRL;

const bool KeyHandler::kMULTIPLE_MAPPINGS = false;
const int32_t KeyHandler::kMIN_HELD_TIME = 1000;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a default KeyHandler.
 *
 * Inputs: none
 */
KeyHandler::KeyHandler()
{
  loadDefaults();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Prints out the information describing a single given Key
 *
 * Inputs: Key key - the key to print out the information for
 * Output: none
 */
void KeyHandler::printIndex(Key key)
{
  std::cout << "[Game: " << Helpers::gameKeyToStr(key.game_key)
            << "] [P.Key: " << SDL_GetKeyName(key.keycode) << "] [Enabled? "
            << key.enabled << "] [Time Depressed: " << key.time_depressed << "]"
            << std::endl;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Determines whether a Key matching a given GameKey enumeration
 *              has been depressed long enough to be considered held
 *
 * Inputs: GameKey game_key - enumerated game function key looking for
 * Output: bool - true if the key is considered held
 */
bool KeyHandler::isHeld(GameKey game_key)
{
  auto& key = getKey(game_key);

  if(key.time_depressed >= kMIN_HELD_TIME)
    return true;

  return false;
}

/*
 * Description: Determines whether the Key matching a given SDL Keycode has
 *              been depressed for long enough to be considered 'held'
 *
 * Inputs: SDL_Keycode keycode - the keycode looked for
 *         found - pointer assigned true if the keycode is found
 * Output: bool - true if the keycode is being held
 */
bool KeyHandler::isHeld(SDL_Keycode keycode, bool* found)
{
  auto& key = getKey(keycode, found);

  if(*found && key.time_depressed >= kMIN_HELD_TIME)
    return true;

  return false;
}

/*
 * Description: Gets the depressed state of a Key matching an enumerated Game
 *              function key.
 *
 * Inputs: GameKey game_key - enumerated game function key
 * Output: bool - the depressed state of the matching Key
 */
bool KeyHandler::isDepressed(GameKey game_key)
{
  std::cout << "Checking if a game key is depressed?" << std::endl;
  auto key = getKey(game_key);

  if(key.depressed)
    std::cout << "Yes" << std::endl;

  return key.depressed;
}

/*
 * Description: Gets the depressed state of a Key matching a given SDL keycode,
 *              if such a Key can be found.
 *
 * Inputs: SDL_Keycode keycode - keycode of a Key to be looked for
 *         bool* found - pointer will be true if a Key can be found
 * Output: bool - the depressed state of the matching key
 */
bool KeyHandler::isDepressed(SDL_Keycode keycode, bool* found)
{
  auto& key = getKey(keycode, found);

  if(found)
    return key.depressed;

  return false;
}

/*
 * Description: Determines whether a Key matching a given game function key
 *              enumeration is currently enabled.
 *
 * Inputs: GameKey game_key - enuemrated game function key to check
 * Output: bool - true if the Key matching that game key enum is enabled
 */
bool KeyHandler::isEnabled(GameKey game_key)
{
  for(auto& element : keys)
    if(element.game_key == game_key)
      return element.enabled;

  return false;
}

/*
 * Description: Searches for a Key matching a given keycode to find whether the
 *              key is Enabled.
 *
 * Inputs: SDL_Keycode keycode - the keycode to find a matching Key for
 *         bool* found - assigned true if a Key matcing the keycode was found
 * Output:  bool - true if the desired Key is currently enabled
 */
bool KeyHandler::isEnabled(SDL_Keycode keycode, bool* found)
{
  for(auto& element : keys)
  {
    if(element.keycode == keycode)
    {
      *found = true;

      return element.enabled;
    }
  }

  return false;
}

/*
 * Description: Determines whether a given keycode has been mapped to any Key.
 *
 * Inputs: SDL_Keycode keycode - the keycode to look for mapping
 * Output: bool - true if a Key mapped to this keycode exists
 */
bool KeyHandler::isKeycodeMapped(SDL_Keycode keycode)
{
  for(auto& element : keys)
    if(element.keycode == keycode)
      return true;

  return false;
}

/*
 * Description: Resets the KeyHandler to a default set of key mapping and
 *              depressed states.
 *
 * Inputs: none
 * Output: none
 */
void KeyHandler::loadDefaults()
{
  keys.clear();

  keys.push_back(Key(GameKey::MOVE_LEFT, kMOVE_LEFT_DEFAULT));
  keys.push_back(Key(GameKey::MOVE_RIGHT, kMOVE_RIGHT_DEFAULT));
  keys.push_back(Key(GameKey::MOVE_UP, kMOVE_UP_DEFAULT));
  keys.push_back(Key(GameKey::MOVE_DOWN, kMOVE_DOWN_DEFAULT));
  keys.push_back(Key(GameKey::MENU, kMENU_DEFAULT));
  keys.push_back(Key(GameKey::ACTION, kACTION_DEFAULT));
  keys.push_back(Key(GameKey::CANCEL, kCANCEL_DEFAULT));
  keys.push_back(Key(GameKey::RUN, kRUN_DEFAULT));
  keys.push_back(Key(GameKey::DEBUG, kDEBUG_DEFAULT));
  keys.push_back(Key(GameKey::PAUSE, kPAUSE_DEFAULT));
}

/*
 * Description: Prints out the information contained in the KeyHandler. Will
 *              print out only keys which are considered to be depressed, or
 *              keys which are considered to be 'held' if desired.
 *
 * Inputs: bool only_depressed - true if only desired the depressed keys printed
 *         bool only_held - true if only held keys are desired to be printed
 * Output: none
 */
void KeyHandler::print(bool only_depressed, bool only_held)
{
  auto printed_once = false;

  for(auto element : keys)
  {
    if(only_depressed && element.depressed)
    {
      printIndex(element);
      printed_once = true;
    }
    else if(only_held && isHeld(element.game_key))
    {
      printIndex(element);
      printed_once = true;
    }
    else if(!only_depressed && !only_held)
    {
      printIndex(element);
      printed_once = true;
    }
  }

  if(printed_once)
    std::cout << std::endl;
}

/*
 * Description: Updates the state of the KeyHandler to match the state of the
 *              Keyboard.
 *
 * Inputs: int32_t cycle_time - time of the current cycle
 * Output: bool - signal for end of processing
 */
bool KeyHandler::update(int32_t cycle_time)
{
  /* Update the state of Keys */
  SDL_PumpEvents();
  auto state = SDL_GetKeyboardState(nullptr);

  for(auto& element : keys)
  {
    if(state != nullptr)
    {
      /* Grab the SDL_Scancode matching the current element's Keycode */
      auto scan_code = SDL_GetScancodeFromKey(element.keycode);

      /* Determine if the state of the Keyboard if the scan code is depressed */
      if(state[scan_code])
      {
        /* Assign the element to be a depressed state */
        element.depressed = true;
        element.time_depressed += cycle_time;
      }
      else
      {
        /* Assign the element to be an unpressed state */
        element.depressed = false;
        element.time_depressed = 0;
      }
    }
  }

  return false;
}

/*
 * Description: Returns a referene to a Key matching a given enumerated GameKey.
 *
 * Inputs: GameKey game_key - enumerated game function key to find a Key for
 * Output: Key& - ref. to the desired key
 */
Key& KeyHandler::getKey(GameKey game_key)
{
  for(auto& element : keys)
    if(element.game_key == game_key)
      return element;

  return keys.at(0);
}

/*
 * Description: Returns a reference to a Key matching a given keycode, if
 *              possible, else returns the first Key.
 *
 * Inputs: SDL_Keycode keycode - the SDL Keycode to find a Key for
 *          bool* found - bool pointer to assign truth of search results
 * Output: Key& - ref. to the desired key
 */
Key& KeyHandler::getKey(SDL_Keycode keycode, bool* found)
{
  for(auto& element : keys)
  {
    if(element.keycode == keycode)
    {
      *found = true;
      return element;
    }
  }

  return keys.at(0);
}

/*
 * Description: Assigns a given enumerated GameKey to be mapped to a given SDL
 *              Keycode if a match can be made. A GameKey cannot be mapped to
 *
 * Inputs: GameKey game_key - enumerated game function key to map keycode to
 *         SDL_Keycode new_keycode - the keycode to map the game function key to
 * Output: bool - true if a Key matching the game key was found.
 */
bool KeyHandler::setKey(GameKey game_key, SDL_Keycode new_keycode)
{
  if(!isKeycodeMapped(new_keycode))
  {
    auto& key = getKey(game_key);
    key.keycode = new_keycode;

    return true;
  }

  return false;
}

/*
 * Description: Assigns a Key matching a given GameKey enumerated to the desired
 *              enabled State, as well as all other Keys matching the GameKey.
 *
 * Inputs: GameKey game_key - enumerated game key to set to enabled
 *         bool enabled - the enabled value to assign the key to
 * Output: none
 */
void KeyHandler::setEnabled(GameKey game_key, bool enabled)
{
  for(auto& element : keys)
    if(element.game_key == game_key)
      element.enabled = enabled;
}

/*
 * Description: Assigns a Key matching a given SDL_Keycode to a desired enabled
 *              state, if such a Key can be found.
 *
 * Inputs: SDL_Keycode keycode - the keycode to find a Key for
 *         bool enabled - the enabled value to assign the Key to
 * Output: bool - true if a Key matching the keycode was found
 */
bool KeyHandler::setEnabled(SDL_Keycode keycode, bool enabled)
{
  for(auto& element : keys)
  {
    if(element.keycode == keycode)
    {
      element.enabled = enabled;

      return true;
    }
  }

  return false;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Static function to return the value of min held time
 *
 * Inputs: none
 * Output: int32_t - the kMIN_HELD_TIME constant value
 */
int32_t KeyHandler::getMinHeldTime()
{
  return kMIN_HELD_TIME;
}
