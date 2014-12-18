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
const uint16_t BattleDisplay::kBAR_OFFSET = 88;
const uint16_t BattleDisplay::kFOES_OFFSET = 94;
const uint16_t BattleDisplay::kFRIENDS_OFFSET = 328;
const uint8_t BattleDisplay::kMAX_LAYERS = 10;
const uint16_t BattleDisplay::kPERSON_OVERLAP = 56;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
// TODO: Comment
BattleDisplay::BattleDisplay(Options* running_config)
{
  /* Blank out variables */
  background = NULL;
  bar_offset = 0;
  battle = NULL;
  battle_bar = NULL;
  offset = 0; // TODO: Delete
  offset_2 = 0; // TODO: Delete
  system_options = NULL;

  /* Set up variables */
  setConfiguration(running_config);
}

/*
 * Description: Destructor function
 */
BattleDisplay::~BattleDisplay()
{
  clear();
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

// TODO: Comment
std::vector<Person*> BattleDisplay::getFoes()
{
  /* Check to see if the friends are set */
  if(battle->getFoes() != NULL)
    return battle->getFoes()->getMembers();

  /* Otherwise, return empty stack */
  std::vector<Person*> empty_stack;
  return empty_stack;
}

// TODO: Comment
std::vector<Frame*> BattleDisplay::getFoesFrames()
{
  std::vector<Person*> list = getFoes();

  /* Get the third person frames */
  std::vector<Frame*> frame_list;
  for(uint8_t i = 0; i < list.size(); i++)
  {
    if(list[i] != NULL)
      frame_list.push_back(list[i]->getThirdPerson());
    else
      frame_list.push_back(NULL);
  }

  return frame_list;
}

/* Get foes in battle */
// TODO: Comment
Party* BattleDisplay::getFoesParty()
{
  return battle->getFoes();
}

// TODO: Comment
std::vector<Person*> BattleDisplay::getFriends()
{
  /* Check to see if the friends are set */
  if(battle->getFriends() != NULL)
    return battle->getFriends()->getMembers();

  /* Otherwise, return empty stack */
  std::vector<Person*> empty_stack;
  return empty_stack;
}
  
// TODO: Comment
std::vector<Frame*> BattleDisplay::getFriendsFrames()
{
  std::vector<Person*> list = getFriends();

  /* Get the first person frames */
  std::vector<Frame*> frame_list;
  for(uint8_t i = 0; i < list.size(); i++)
  {
    if(list[i] != NULL)
      frame_list.push_back(list[i]->getFirstPerson());
    else
      frame_list.push_back(NULL);
  }

  return frame_list;
}

/* Get friends in battle */
// TODO: Comment
Party* BattleDisplay::getFriendsParty()
{
  return battle->getFriends();
}
  
/* Renders the battle bar */
// TODO: Comment
bool BattleDisplay::renderBar(SDL_Renderer* renderer, uint16_t screen_width, 
                                                      uint16_t screen_height)
{
  bool success = true;

  if(battle_bar != NULL)
  {
    success &= battle_bar->render(renderer, 0, 
                                  screen_height - kBAR_OFFSET - bar_offset,
                                  screen_width);

    return success;
  }

  return false;
}

/* Renders the foes */
// TODO: Comment
bool BattleDisplay::renderFoes(SDL_Renderer* renderer, uint16_t screen_width)
{
  /* Get frames */
  std::vector<Frame*> frame_list = getFoesFrames();
  bool success = true;

  /* Render the foes */
  uint8_t index = 0;
  for(uint8_t i = 0; i < frame_list.size(); i++)
  {
    if(frame_list[i] != NULL)
    {
      /* Modify index for 1 and 2 flip around */
      if(i == 0)
        index = 1;
      else if(i == 1)
        index = 0;
      else
        index = i;

      /* Render the frame */
      int width = frame_list[i]->getWidth();
      success &= frame_list[i]->render(renderer, 
                       screen_width - (width - kPERSON_OVERLAP) * index - width, 
                       kFOES_OFFSET);
    }
  }
  
  return success;
}

/* Renders the friends */
// TODO: Comment
bool BattleDisplay::renderFriends(SDL_Renderer* renderer, 
                                  uint16_t screen_height)
{
  /* Get frames */
  std::vector<Frame*> frame_list = getFriendsFrames();
  bool success = true;

  /* Render the foes */
  uint8_t index = 0;
  for(uint8_t i = 0; i < frame_list.size(); i++)
  {
    if(frame_list[i] != NULL)
    {
      /* Modify index for 1 and 2 flip around */
      if(i == 0)
        index = 1;
      else if(i == 1)
        index = 0;
      else
        index = i;

      /* Render the frame */
      int width = frame_list[i]->getWidth();
      success &= frame_list[i]->render(renderer, 
                                       (width - kPERSON_OVERLAP) * index, 
                                       screen_height - kFRIENDS_OFFSET);
    }
  }
  
  return success;
}

/* Trims the midlay vector of NULL sprite pointers */
// TODO: Comment
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
// TODO: Comment
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
// TODO: Comment
bool BattleDisplay::addMidlay(Sprite* midlay)
{
  if(midlay != NULL && midlay->isFramesSet() && midlays.size() < kMAX_LAYERS)
  {
    midlays.push_back(midlay);
    return true;
  }
  return false;
}

/* Adds an overlay sprite - appends to end of stack */
// TODO: Comment
bool BattleDisplay::addOverlay(Sprite* overlay)
{
  if(overlay != NULL && overlay->isFramesSet() && overlays.size() < kMAX_LAYERS)
  {
    overlays.push_back(overlay);
    return true;
  }
  return false;
}

/* Clears the class data */
// TODO: Comment
void BattleDisplay::clear()
{
  /* Clears variables */
  system_options = NULL;

  /* Unsets the flat rendering sprites */
  unsetBackground();
  unsetBattle();
  unsetBattleBar();
  unsetMidlays();
  unsetOverlays();
}

/* Get the background */
// TODO: Comment
Sprite* BattleDisplay::getBackground()
{
  return background;
}
  
/* Gets the battle corresponding to the display */
// TODO: Comment
Battle* BattleDisplay::getBattle()
{
  return battle;
}

/* Get the midlay(s) */
// TODO: Comment
Sprite* BattleDisplay::getMidlay(uint8_t index)
{
  if(midlays.size() > index)
    return midlays[index];

  Sprite* null_sprite = NULL;
  return null_sprite;
}
  
// TODO: Comment
std::vector<Sprite*> BattleDisplay::getMidlays()
{
  return midlays;
}

/* Get the overlay(s) */
// TODO: Comment
Sprite* BattleDisplay::getOverlay(uint8_t index)
{
  if(overlays.size() > index)
    return overlays[index];

  Sprite* null_sprite = NULL;
  return null_sprite;
}
  
// TODO: Comment
std::vector<Sprite*> BattleDisplay::getOverlays()
{
  return overlays;
}

/* Renders the battle display */
// TODO: Comment
bool BattleDisplay::render(SDL_Renderer* renderer)
{
  uint16_t height = 0;
  bool success = true;
  uint16_t width = 0;

  /* Check the options for screen height and width */
  if(system_options != NULL)
  {
    height = system_options->getScreenHeight();
    width = system_options->getScreenWidth();
  }

  if(battle != NULL)
  {
    /* Render background */
    if(background != NULL)
      success &= background->render(renderer, 0, 0, width, height);

    /* Render overlays - over background modifiers */
    for(uint8_t i = 0; i < overlays.size(); i++)
      success &= overlays[i]->render(renderer, 0, 0, width, height);

    /* Render enemies */
    success &= renderFoes(renderer, width);

    /* Render midlays (between enemies and players */
    for(uint8_t i = 0; i < midlays.size(); i++)
    {
      // TODO: Restore
      //success &= midlays[i]->render(renderer, 0, 0, width, height);

      if(i == 0)
      {
        midlays[i]->render(renderer, offset, 0, width, height);
        midlays[i]->render(renderer, offset - width, 0, width, height);
      }
      else if(i == 1)
      {
        midlays[i]->render(renderer, 0, 0, width, height);

        midlays.front()->render(renderer, offset_2, 0, width, height);
        midlays.front()->render(renderer, offset_2 - width, 0, width, height);
      }
    }

    /* Render player team */
    success &= renderFriends(renderer, height);

    // TODO: Render enemy health bars

    /* Render battle bar (on bottom) */
    renderBar(renderer, width, height);

    // TODO: Render extra battle flair

    return success;
  }

  return false;
}

/* Sets the background sprite */
// TODO: Comment
bool BattleDisplay::setBackground(Sprite* background)
{
  if(background != NULL && background->isFramesSet())
  {
    unsetBackground();
    this->background = background;
    return true;
  }

  return false;
}

/* Sets the battle connected to the display */
// TODO: Comment
bool BattleDisplay::setBattle(Battle* battle)
{
  if(battle != NULL)
  {
    this->battle = battle;
    return true;
  }
  return false;
}
  
/* Sets the rendering battle bar */
// TODO: Comment
bool BattleDisplay::setBattleBar(Frame* bar)
{
  if(bar != NULL && bar->isTextureSet())
  {
    battle_bar = bar;
    return true;
  }
  return false;
}

/* Sets the running configuration, from the options class */
// TODO: Comment
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
// TODO: Comment
bool BattleDisplay::setMidlay(Sprite* midlay)
{
  if(midlay != NULL && midlay->isFramesSet())
  {
    unsetMidlays();
    midlays.push_back(midlay);
    return true;
  }

  return false;
}

/* Sets the overlay sprite - main one, removes all others */
// TODO: Comment
bool BattleDisplay::setOverlay(Sprite* overlay)
{
  if(overlay != NULL && overlay->isFramesSet())
  {
    unsetOverlays();
    overlays.push_back(overlay);
    return true;
  }

  return false;
}

/* Unsets the background sprite */
// TODO: Comment
void BattleDisplay::unsetBackground()
{
  if(background != NULL)
    delete background;
  background = NULL;
}

/* Unsets the battle connected to the display */
// TODO: Comment
void BattleDisplay::unsetBattle()
{
  battle = NULL;
}
  
/* Unsets the rendering battle bar */
void BattleDisplay::unsetBattleBar()
{
  if(battle_bar != NULL)
    delete battle_bar;
  battle_bar = NULL;

  /* Clear rendering variables */
  bar_offset = 0;
}

/* Unsets the midlay sprite(s) */
// TODO: Comment
void BattleDisplay::unsetMidlay(uint8_t index)
{
  if(midlays.size() > index && midlays[index] != NULL)
  {
    delete midlays[index];
    midlays[index] = NULL;
    trimMidlays();
  }
}

// TODO: Comment
void BattleDisplay::unsetMidlays()
{
  for(uint8_t i = 0; i < midlays.size(); i++)
    unsetMidlay(i);
  midlays.clear();
}

/* Unsets the overlay sprite(s) */
// TODO: Comment
void BattleDisplay::unsetOverlay(uint8_t index)
{
  if(overlays.size() > index && overlays[index] != NULL)
  {
    delete overlays[index];
    overlays[index] = NULL;
    trimOverlays();
  }
}

// TODO: Comment
void BattleDisplay::unsetOverlays()
{
  for(uint8_t i = 0; i < overlays.size(); i++)
    unsetOverlay(i);
  overlays.clear();
}

/* Updates the battle display */
// TODO: Comment
bool BattleDisplay::update(int cycle_time)
{
  /* Only proceed if there is a set battle */
  if(battle != NULL)
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
  }

  // TODO: Delete
  if(midlays.size() > 0)
  {
    offset += cycle_time / 8;
    if(offset >= 1216)
      offset -= 1216;
    offset_2 += cycle_time / 4;
    if(offset_2 >= 1216)
      offset_2 -= 1216;
    //midlays.front()->setOpacity(midlays.front()->getOpacity() - 1);
  }

  return true;
}

 /*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/
