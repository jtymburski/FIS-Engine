/*******************************************************************************
* Class Name: ClassName [Implementation]
* Date Created: June 22, 2014
* Inheritance: None
* Description: 
*
* Notes
* -----
*
* [1]:
*
* See .h file for TODOs
******************************************************************************/
#include "Game/KeyHandler.h"

/*=============================================================================
 * KEY HANDLER
 *============================================================================*/

//TODO Conventions [04-09-15]
Key::Key(GameKey new_key, SDL_Keycode new_keycode)
    : game_key{new_key}
    , keycode{new_keycode}
    , depressed{false}
    , time_depressed{0}
{}

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

const SDL_Keycode KeyHandler::kMoveLeftDefault  = SDLK_LEFT;
const SDL_Keycode KeyHandler::kMoveRightDefault = SDLK_RIGHT;
const SDL_Keycode KeyHandler::kMoveUpDefault    = SDLK_UP;
const SDL_Keycode KeyHandler::kMoveDownDefault  = SDLK_DOWN;
const SDL_Keycode KeyHandler::kMenuDefault      = SDLK_d;
const SDL_Keycode KeyHandler::kActionDefault    = SDLK_a;
const SDL_Keycode KeyHandler::kCancelDefault    = SDLK_s;
const SDL_Keycode KeyHandler::kDebugDefault     = SDLK_f;

const int32_t KeyHandler::kMinHeldTime = 1000;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
 //TODO Conventions [04-09-15]
KeyHandler::KeyHandler()
{
  loadDefaults(); 
}

/*
 * Description:
 */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
 //TODO: Conventions
void KeyHandler::printIndex(Key key)
{
  std::cout << "[GameKey: " << Helpers::gameKeyToStr(key.game_key) << "]";
  std::cout << " [Time Depressed: " << key.time_depressed << "]" << std::endl;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

//TODO: Conventions
bool KeyHandler::isHeld(GameKey game_key)
{
  auto& key = getKey(game_key);

  if (key.time_depressed >= kMinHeldTime)
    return true;

  return false;
}

//TODO: Conventions
bool KeyHandler::isHeld(SDL_Keycode keycode, bool* found)
{
  auto& key = getKey(keycode, found);

  if (*found && key.time_depressed >= kMinHeldTime)
    return true;

  return false;
}

//TODO: Conventions
bool KeyHandler::isDepressed(GameKey game_key)
{
  auto key = getKey(game_key);

  return key.depressed;
}

//TODO Conventions [04-09-15]
bool KeyHandler::isDepressed(SDL_Keycode keycode, bool* found)
{
  auto key = getKey(keycode, found);

  return key.depressed;
}

//TODO Conventions [04-09-15]
bool KeyHandler::isKeycodeMapped(SDL_Keycode keycode)
{
  for (auto& element : keys)
    if (element.keycode == keycode)
      return true;

  return false;
}

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
//TODO Conventions [04-09-15]
void KeyHandler::loadDefaults()
{
  keys.clear();

  keys.push_back(Key(GameKey::MOVE_LEFT,   kMoveLeftDefault));
  keys.push_back(Key(GameKey::MOVE_RIGHT,  kMoveRightDefault));
  keys.push_back(Key(GameKey::MOVE_UP,     kMoveUpDefault));
  keys.push_back(Key(GameKey::MOVE_DOWN,   kMoveDownDefault));
  keys.push_back(Key(GameKey::MENU,   kMenuDefault));
  keys.push_back(Key(GameKey::ACTION, kActionDefault));
  keys.push_back(Key(GameKey::CANCEL, kCancelDefault));
  keys.push_back(Key(GameKey::DEBUG,  kDebugDefault));
}

//TODO Conventions [04-09-15]
void KeyHandler::print(bool only_depressed, bool only_held)
{
  auto printed_once = false;

  for (auto element : keys)
  {
    if (only_depressed && element.depressed)
    {
      printIndex(element);
      printed_once = true;
    }
    else if (only_held && isHeld(element.game_key))
    {
      printIndex(element);
      printed_once = true;
    }
    else if (!only_depressed && !only_held)
    {
      printIndex(element);
      printed_once = true;
    }
  }

  if (printed_once)
    std::cout << std::endl;
}

//TODO: Conventions
bool KeyHandler::update(int32_t cycle_time)
{
  for (auto& element : keys)
    if (element.depressed)
      element.time_depressed += cycle_time;

  return false;
}

//TODO Conventions [04-09-15]
Key& KeyHandler::getKey(GameKey game_key)
{
  for (auto &element : keys)
    if (element.game_key == game_key)
      return element;

  return keys.at(0);
}

//TODO Conventions [04-09-15]
Key& KeyHandler::getKey(SDL_Keycode keycode, bool* found)
{
  for (auto &element : keys)
  {
    if (element.keycode == keycode)
    {
      *found = true;
      return element;
    }
  }

  return keys.at(0);
}

//TODO Conventions [04-09-15]
void KeyHandler::setDepressed(GameKey game_key)
{
  auto& key = getKey(game_key);

  key.depressed = true;
}

//TODO Conventions [04-09-15]
bool KeyHandler::setDepressed(SDL_Keycode keycode)
{
  bool found = false;
  auto& key = getKey(keycode, &found);

  key.depressed = true;

  return found;
}

//TODO Conventions [04-09-15]
bool KeyHandler::setKey(GameKey game_key, SDL_Keycode new_keycode)
{
  if (!isKeycodeMapped(new_keycode))
  {
    auto& key = getKey(game_key);
    key.keycode = new_keycode;

    return true;
  }

  return false;
}

//TODO Conventions [04-09-15]
void KeyHandler::setUnpressed(GameKey game_key)
{
  auto& key = getKey(game_key);

  key.depressed      = false;
  key.time_depressed = 0;
}

//TODO Conventions [04-09-15]
bool KeyHandler::setUnpressed(SDL_Keycode keycode)
{
  bool found = false;
  auto& key = getKey(keycode, &found);

  if (found)
  {
    key.depressed = false;
    key.time_depressed = 0;

   return true;
 }

 return false;
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
