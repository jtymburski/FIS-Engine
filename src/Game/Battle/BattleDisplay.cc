/*******************************************************************************
 * Class Name: BattleDisplay [Implementation]
 * Date Created: December 16, 2014
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
#include "Game/Battle/BattleDisplay.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t BattleDisplay::kMAX_LAYERS = 10;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
BattleDisplay::BattleDisplay(Options* running_config)
{
  /* Blank out variables */
  background = NULL;
  system_options = NULL;

  /* Set up variables */
  setConfiguration(running_config);
}

/*
 * Description:
 */
BattleDisplay::~BattleDisplay()
{
  clear();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 * Output:
 */
  
/* Trims the midlay vector of NULL sprite pointers */
void BattleDisplay::trimMidlays()
{
  std::vector<Sprite*> non_null;

  /* Find all non-null sprites */
  for(uint8_t i = 0; i < midlays.size(); i++)
    if(midlays[i] != NULL)
      non_null.push_back(midlays[i]);

  /* If the vectors are different size, make the new one the non-null version */
  if(non_null.size() < midlays.size())
    midlays = non_null;
}

/* Trims the overlay vector of NULL sprite pointers */
void BattleDisplay::trimOverlays()
{
  std::vector<Sprite*> non_null;

  /* Find all non-null sprites */
  for(uint8_t i = 0; i < overlays.size(); i++)
    if(overlays[i] != NULL)
      non_null.push_back(overlays[i]);

  /* If the vectors are different size, make the new one the non-null version */
  if(non_null.size() < overlays.size())
    overlays = non_null;
}

/*=============================================================================
 * VIRTUAL FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PROTECTED FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
  
/* Adds a midlay sprite - appends to end of stack */
bool BattleDisplay::addMidlay(Sprite* midlay)
{
  if(midlay != NULL && midlays.size() < kMAX_LAYERS)
  {
    midlays.push_back(midlay);
    return true;
  }
  return false;
}

/* Adds an overlay sprite - appends to end of stack */
bool BattleDisplay::addOverlay(Sprite* overlay)
{
  if(overlay != NULL && overlays.size() < kMAX_LAYERS)
  {
    overlays.push_back(overlay);
    return true;
  }
  return false;
}

/* Clears the class data */
void BattleDisplay::clear()
{
  /* Clears variables */
  system_options = NULL;

  /* Unsets the flat rendering sprites */
  unsetBackground();
  unsetMidlays();
  unsetOverlays();
}

/* Get the background */
Sprite* BattleDisplay::getBackground()
{
  return background;
}

/* Get the midlay(s) */
Sprite* BattleDisplay::getMidlay(uint8_t index)
{
  if(midlays.size() > index)
    return midlays[index];

  Sprite* null_sprite = NULL;
  return null_sprite;
}
  
std::vector<Sprite*> BattleDisplay::getMidlays()
{
  return midlays;
}

/* Get the overlay(s) */
Sprite* BattleDisplay::getOverlay(uint8_t index)
{
  if(overlays.size() > index)
    return overlays[index];

  Sprite* null_sprite = NULL;
  return null_sprite;
}
  
std::vector<Sprite*> BattleDisplay::getOverlays()
{
  return overlays;
}

/* Renders the battle display */
bool BattleDisplay::render(SDL_Renderer* renderer)
{
  uint16_t height = 0;
  uint16_t width = 0;

  /* Check the options for screen height and width */
  if(system_options != NULL)
  {
    height = system_options->getScreenHeight();
    width = system_options->getScreenWidth();
  }

  /* Render background */
  if(background != NULL)
    background->render(renderer, 0, 0, width, height);

  /* Render overlays - over background modifiers */
  for(uint8_t i = 0; i < overlays.size(); i++)
    overlays[i]->render(renderer, 0, 0, width, height);

  // TODO: Render enemy team

  /* Render midlays (between enemies and players */
  for(uint8_t i = 0; i < midlays.size(); i++)
    midlays[i]->render(renderer, 0, 0, width, height);

  // TODO: Render player team

  // TODO: Render enemy health bars

  // TODO: Render player health bar / action bar

  // TODO: Render extra battle flair

  return true;
}

/* Sets the background sprite */
bool BattleDisplay::setBackground(Sprite* background)
{
  if(background != NULL)
  {
    unsetBackground();
    this->background = background;
    return true;
  }

  return false;
}

/* Sets the running configuration, from the options class */
bool BattleDisplay::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    return true;
  }
  
  return false;
}

/* Sets the midlay sprite - main one, removes all others */
bool BattleDisplay::setMidlay(Sprite* midlay)
{
  if(midlay != NULL)
  {
    unsetMidlays();
    midlays.push_back(midlay);
    return true;
  }

  return false;
}

/* Sets the overlay sprite - main one, removes all others */
bool BattleDisplay::setOverlay(Sprite* overlay)
{
  if(overlay != NULL)
  {
    unsetOverlays();
    overlays.push_back(overlay);
    return true;
  }

  return false;
}

/* Unsets the background sprite */
void BattleDisplay::unsetBackground()
{
  if(background != NULL)
    delete background;
  background = NULL;
}

/* Unsets the midlay sprite(s) */
void BattleDisplay::unsetMidlay(uint8_t index)
{
  if(midlays.size() > index && midlays[index] != NULL)
  {
    delete midlays[index];
    midlays[index] = NULL;
    trimMidlays();
  }
}

void BattleDisplay::unsetMidlays()
{
  for(uint8_t i = 0; i < midlays.size(); i++)
    unsetMidlay(i);
  midlays.clear();
}

/* Unsets the overlay sprite(s) */
void BattleDisplay::unsetOverlay(uint8_t index)
{
  if(overlays.size() > index && overlays[index] != NULL)
  {
    delete overlays[index];
    overlays[index] = NULL;
    trimOverlays();
  }
}

void BattleDisplay::unsetOverlays()
{
  for(uint8_t i = 0; i < overlays.size(); i++)
    unsetOverlay(i);
  overlays.clear();
}

/* Updates the battle display */
bool BattleDisplay::update(int cycle_time)
{
  /* Update the background */
  if(background != NULL)
    background->update(cycle_time);

  /* Update the midlays */
  for(uint8_t i = 0; i < midlays.size(); i++)
    midlays[i]->update(cycle_time);

  /* Update the overlays */
  for(uint8_t i = 0; i < overlays.size(); i++)
    overlays[i]->update(cycle_time);

  return true;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/
