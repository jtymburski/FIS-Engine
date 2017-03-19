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
Key::Key(GameKey new_key, SDL_Keycode new_keycode, bool enabled,
         bool assigned_prim)
    : game_key{new_key},
      keycode_prim{new_keycode},
      keycode_secd{new_keycode},
      depressed{false},
      assigned_prim{assigned_prim},
      assigned_secd{false},
      enabled{enabled},
      time_depressed{0},
      state{GameKeyState::READY}
{
}

/*=============================================================================
 * CONSTANTSd
 *============================================================================*/

const SDL_Keycode KeyHandler::kMOVE_LEFT_DEFAULT = SDLK_a;
const SDL_Keycode KeyHandler::kMOVE_LEFT_SECD = SDLK_LEFT;
const SDL_Keycode KeyHandler::kMOVE_RIGHT_DEFAULT = SDLK_d;
const SDL_Keycode KeyHandler::kMOVE_RIGHT_SECD = SDLK_RIGHT;
const SDL_Keycode KeyHandler::kMOVE_UP_DEFAULT = SDLK_w;
const SDL_Keycode KeyHandler::kMOVE_UP_SECD = SDLK_UP;
const SDL_Keycode KeyHandler::kMOVE_DOWN_DEFAULT = SDLK_s;
const SDL_Keycode KeyHandler::kMOVE_DOWN_SECD = SDLK_DOWN;
const SDL_Keycode KeyHandler::kACTION_DEFAULT = SDLK_SPACE;
const SDL_Keycode KeyHandler::kACTION_SECD = SDLK_RETURN;
const SDL_Keycode KeyHandler::kCANCEL_DEFAULT = SDLK_ESCAPE;
const SDL_Keycode KeyHandler::kCANCEL_SECD = SDLK_x;
const SDL_Keycode KeyHandler::kBACKSPACE_DEFAULT = SDLK_BACKSPACE;
const SDL_Keycode KeyHandler::kBACKSPACE_SECD = SDLK_DELETE;
const SDL_Keycode KeyHandler::kRUN_DEFAULT = SDLK_LSHIFT;
const SDL_Keycode KeyHandler::kRUN_SECD = SDLK_RSHIFT;
const SDL_Keycode KeyHandler::kPAUSE_DEFAULT = SDLK_p;
const SDL_Keycode KeyHandler::kPAUSE_SECD = SDLK_LCTRL;

const int32_t KeyHandler::kMIN_HELD_TIME = 100;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description: Constructs a default KeyHandler.
 *
 * Inputs: none
 */
KeyHandler::KeyHandler() : mode{KeyMode::INPUT}
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
            << "] [P.Key: " << SDL_GetKeyName(key.keycode_prim)
            << "] [Enabled? " << key.enabled
            << "] [Time Depressed: " << key.time_depressed << "]" << std::endl;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void KeyHandler::updateKey(Key& key, int32_t cycle_time, KeyMode call_mode)
{
  // TODO: Updating for secondary keys.

  /* Grab the SDL_Scancode matching the current element's Keycode */
  auto scan_code_prim = SDL_GetScancodeFromKey(key.keycode_prim);
  auto scan_code_secd = SDL_GetScancodeFromKey(key.keycode_secd);
  ;

  auto bp_keycode_prim = getKey(GameKey::BACKSPACE).keycode_prim;
  auto bp_keycode_secd = getKey(GameKey::BACKSPACE).keycode_secd;

  auto state = SDL_GetKeyboardState(nullptr);

  if(state)
  {
    /* Determine if the state of the Keyboard if scan code is depressed */
    if(key.assigned_prim)
    {
      if((state[scan_code_prim] || state[scan_code_secd]) && !key.depressed)
      {
        /* Assign the element to be a depressed state */
        key.depressed = true;
        key.time_depressed = 0;

        if(call_mode == KeyMode::TEXT_ENTRY)
          addKeyEntry(key);
        else if(call_mode == KeyMode::NAME_ENTRY &&
                text.size() < StringDb::kMAX_TITLE_NAME)
        {

          if(text.back() != ' ' || key.keycode_prim != SDLK_SPACE)
            addKeyEntry(key);
        }
        else if(call_mode == KeyMode::INPUT &&
                (key.keycode_prim == bp_keycode_prim ||
                 key.keycode_secd == bp_keycode_secd))
        {
          removeKeyEntry();
        }
      }
      else if(state[scan_code_prim] && key.depressed)
      {
        key.time_depressed += cycle_time;
      }
      else
      {
        /* Assign the element to be an unpressed state */
        key.depressed = false;
        key.time_depressed = 0;
      }
    }
  }
}

/*
 * Description: Adds the given Key's key name to the running text string
 *              stored for text entry, if found.
 *
 * Inputs: Key - reference to the key object being added
 * Output: none
 */
void KeyHandler::addKeyEntry(Key& key)
{
  /* Check if the key is valid within the parameters of text entry keys */
  bool found = false;

  for(auto& element : text_keys)
    if(element.keycode_prim == key.keycode_prim)
      found = true;

  if(found)
  {
    auto key_name = SDL_GetKeyName(key.keycode_prim);

    if(key_name)
    {
      if(key.keycode_prim == SDLK_SPACE)
        text += " ";
      else
        text += key_name;
    }
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void KeyHandler::removeKeyEntry()
{
  if(text.size())
    text.pop_back();
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void KeyHandler::clearTextEntry()
{
  text = "";
}

SDL_KeyboardEvent KeyHandler::getLastEvent()
{
  return last_event;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
KeyMode KeyHandler::getMode()
{
  return mode;
}

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
bool KeyHandler::isHeld(SDL_Keycode keycode)
{
  bool found{false};

  auto& key = getKey(keycode, &found);

  if(found)
    return (key.time_depressed >= kMIN_HELD_TIME);

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
  auto key = getKey(game_key);

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
bool KeyHandler::isDepressed(SDL_Keycode keycode)
{
  bool found{false};

  auto& key = getKey(keycode, &found);

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
    if(element.keycode_prim == keycode)
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
bool KeyHandler::isKeycodeMappedPrim(SDL_Keycode keycode)
{
  for(auto& element : keys)
    if(element.keycode_prim == keycode)
      return true;

  return false;
}

bool KeyHandler::isKeycodeMappedSecd(SDL_Keycode keycode)
{
  for(auto& element : keys)
    if(element.keycode_secd == keycode)
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
  text_keys.clear();

  /* Game Keys */
  keys.push_back(Key(GameKey::MOVE_LEFT, kMOVE_LEFT_DEFAULT));
  keys.push_back(Key(GameKey::MOVE_RIGHT, kMOVE_RIGHT_DEFAULT));
  keys.push_back(Key(GameKey::MOVE_UP, kMOVE_UP_DEFAULT));
  keys.push_back(Key(GameKey::MOVE_DOWN, kMOVE_DOWN_DEFAULT));
  keys.push_back(Key(GameKey::ACTION, kACTION_DEFAULT));
  keys.push_back(Key(GameKey::CANCEL, kCANCEL_DEFAULT));
  keys.push_back(Key(GameKey::BACKSPACE, kBACKSPACE_DEFAULT));
  keys.push_back(Key(GameKey::RUN, kRUN_DEFAULT));
  keys.push_back(Key(GameKey::PAUSE, kPAUSE_DEFAULT));

  /* */
  setKeySecondary(GameKey::MOVE_UP, kMOVE_UP_SECD);
  setKeySecondary(GameKey::MOVE_LEFT, kMOVE_LEFT_SECD);
  setKeySecondary(GameKey::MOVE_DOWN, kMOVE_DOWN_SECD);
  setKeySecondary(GameKey::MOVE_RIGHT, kMOVE_RIGHT_SECD);
  setKeySecondary(GameKey::ACTION, kACTION_SECD);
  setKeySecondary(GameKey::CANCEL, kCANCEL_SECD);
  setKeySecondary(GameKey::BACKSPACE, kBACKSPACE_SECD);
  setKeySecondary(GameKey::RUN, kRUN_SECD);
  setKeySecondary(GameKey::PAUSE, kPAUSE_SECD);

  /* Text Entry Keys */
  text_keys.push_back(Key(GameKey::NONE, SDLK_a));
  text_keys.push_back(Key(GameKey::NONE, SDLK_b));
  text_keys.push_back(Key(GameKey::NONE, SDLK_c));
  text_keys.push_back(Key(GameKey::NONE, SDLK_d));
  text_keys.push_back(Key(GameKey::NONE, SDLK_e));
  text_keys.push_back(Key(GameKey::NONE, SDLK_f));
  text_keys.push_back(Key(GameKey::NONE, SDLK_g));
  text_keys.push_back(Key(GameKey::NONE, SDLK_h));
  text_keys.push_back(Key(GameKey::NONE, SDLK_i));
  text_keys.push_back(Key(GameKey::NONE, SDLK_j));
  text_keys.push_back(Key(GameKey::NONE, SDLK_k));
  text_keys.push_back(Key(GameKey::NONE, SDLK_l));
  text_keys.push_back(Key(GameKey::NONE, SDLK_m));
  text_keys.push_back(Key(GameKey::NONE, SDLK_n));
  text_keys.push_back(Key(GameKey::NONE, SDLK_o));
  text_keys.push_back(Key(GameKey::NONE, SDLK_p));
  text_keys.push_back(Key(GameKey::NONE, SDLK_q));
  text_keys.push_back(Key(GameKey::NONE, SDLK_r));
  text_keys.push_back(Key(GameKey::NONE, SDLK_s));
  text_keys.push_back(Key(GameKey::NONE, SDLK_t));
  text_keys.push_back(Key(GameKey::NONE, SDLK_u));
  text_keys.push_back(Key(GameKey::NONE, SDLK_v));
  text_keys.push_back(Key(GameKey::NONE, SDLK_w));
  text_keys.push_back(Key(GameKey::NONE, SDLK_x));
  text_keys.push_back(Key(GameKey::NONE, SDLK_y));
  text_keys.push_back(Key(GameKey::NONE, SDLK_z));
  text_keys.push_back(Key(GameKey::NONE, SDLK_SPACE));

  text = "";
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

  if(mode == KeyMode::INPUT)
  {
    for(auto& element : keys)
      updateKey(element, cycle_time, KeyMode::INPUT);
  }
  else if(mode == KeyMode::TEXT_ENTRY)
  {
    for(auto& element : keys)
      updateKey(element, cycle_time, KeyMode::INPUT);

    for(auto& element : text_keys)
      updateKey(element, cycle_time, KeyMode::TEXT_ENTRY);
  }
  else if(mode == KeyMode::NAME_ENTRY)
  {
    for(auto& element : keys)
      updateKey(element, cycle_time, KeyMode::INPUT);

    for(auto& element : text_keys)
      updateKey(element, cycle_time, KeyMode::NAME_ENTRY);
  }

  return false;
}

std::vector<GameKey> KeyHandler::getGameKeys()
{
  std::vector<GameKey> game_keys;

  for(auto& element : keys)
    game_keys.push_back(element.game_key);

  return game_keys;
}

/*
 * Description: Returns a reference to a Key matching a given enumerated
 * GameKey.
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
    if(element.keycode_prim == keycode || element.keycode_secd)
    {
      if(found)
        *found = true;

      return element;
    }
  }

  return keys.at(0);
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::string KeyHandler::getKeyNamePrim(GameKey game_key, bool* found)
{
  for(auto& element : keys)
  {
    if(element.game_key == game_key)
    {
      if(found)
        *found = true;

      /* Return the name of the assigned key if it is designated asigned */
      if(element.assigned_prim)
        return SDL_GetKeyName(element.keycode_prim);
      else
        return "Unassigned";
    }
  }

  return "";
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::string KeyHandler::getKeyNameSecd(GameKey game_key, bool* found)
{
  for(auto& element : keys)
  {
    if(element.game_key == game_key)
    {
      if(found)
        *found = true;

      /* Return the name of the assigned key if it is designated assigned */
      if(element.assigned_secd)
        return SDL_GetKeyName(element.keycode_secd);
      else
        return "Unassigned";
    }
  }

  return "";
}

/*
 * Description: Assigns a given enumerated GameKey to be mapped to a given SDL
 *              Keycode if a match can be made. A GameKey cannot be mapped to
 *              a keycode that is already mapped with this function.
 *
 * Inputs: GameKey game_key - enumerated game function key to map keycode to
 *         SDL_Keycode new_keycode - the keycode to map the game function key to
 * Output: bool - true if a Key matching the game key was found.
 */
bool KeyHandler::setKeyPrimary(GameKey game_key, SDL_Keycode new_keycode)
{
  bool valid = true;
  auto& key = getKey(game_key);

  valid &= !isKeycodeMappedPrim(new_keycode);
  valid &= !isKeycodeMappedSecd(new_keycode);
  valid |= (new_keycode == key.keycode_prim);

  if(valid)
  {
    key.keycode_prim = new_keycode;
    key.assigned_prim = true;
  }

  return valid;
}

/*
 * Description: Assigns a given enumerated GameKey to be mapped to a given SDL
 *              Keycode if a match can be made. A GameKey cannot be mapped to
 *              a keycode that is already mapped with this function.
 *
 * Inputs: GameKey game_key - enumerated game function key to map keycode to
 *         SDL_Keycode new_keycode - the keycode to map the game function key to
 * Output: bool - true if a Key matching the game key was found.
 */
bool KeyHandler::setKeySecondary(GameKey game_key, SDL_Keycode new_keycode)
{
  bool valid = true;
  auto& key = getKey(game_key);

  valid &= !isKeycodeMappedPrim(new_keycode);
  valid &= !isKeycodeMappedSecd(new_keycode);
  valid |= (new_keycode == key.keycode_secd);

  if(valid)
  {
    key.keycode_secd = new_keycode;
    key.assigned_secd = true;
  }

  return valid;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool KeyHandler::unsetKey(GameKey game_key, bool* found)
{
  for(auto& element : keys)
  {
    if(element.game_key == game_key)
    {
      if(found)
        *found = true;

      element.assigned_prim = false;
      element.assigned_secd = false;
    }
  }
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
void KeyHandler::setMode(KeyMode mode)
{
  this->mode = mode;
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
    if(element.keycode_prim == keycode || element.keycode_secd == keycode)
    {
      element.enabled = enabled;

      return true;
    }
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
bool KeyHandler::setHeld(GameKey key)
{
  for(auto& element : keys)
  {
    if(element.game_key == key)
    {
      element.time_depressed += kMIN_HELD_TIME;

      return true;
    }
  }

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
std::string KeyHandler::getTextEntry()
{
  return text;
}

void KeyHandler::setLastEvent(SDL_KeyboardEvent event)
{
  this->last_event = event;
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
