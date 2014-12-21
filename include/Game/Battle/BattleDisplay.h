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
#include "Text.h"

/*
 * Structure which handles the information required in battle for each given
 * person. Used for controlling state, what's happening, etc.
 */
struct PersonState
{
  Sprite* fp;
  Frame* info;
  Person* self;
  Sprite* tp;
};

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

  /* Ailment frames */
  std::vector<Frame> ailments;

  /* Animation control */
  uint16_t animation_delay;

  /* Background sprite */
  Sprite* background;
  
  /* Bar variables - for rendering */
  uint16_t bar_offset;
  
  /* Battle to be displayed */
  Battle* battle;

  /* The rendering battle bar */
  Frame* battle_bar;

  /* The rendering enemy info bar */
  Frame* foes_backdrop;
  std::vector<PersonState*> foes_state;

  /* Rendering fonts */
  TTF_Font* font_header;
  TTF_Font* font_subheader;

  /* The rendering friend info bar */
  std::vector<PersonState*> friends_state;

  /* Mid scene overlays */
  std::vector<Sprite*> midlays; // TODO: Make overlay class when created

  /* Background overlays */
  std::vector<Sprite*> overlays; // TODO: Make overlay class when created

  /* Rendering turn state */
  TurnState rendering_state;

  /* The system options. Used for rendering, settings, etc */
  Options* system_options;

  /* ------------ Constants --------------- */
  const static uint8_t kAILMENT_BORDER; /* Ailment border width */
  const static uint8_t kAILMENT_GAP; /* Ailment gap between edges of space */
  const static uint8_t kAILMENT_OPACITY; /* Ailment background opacity */
  const static uint8_t kALLY_HEALTH_H; /* Ally health bar height */
  const static uint8_t kALLY_HEALTH_TRIANGLE; /* Ally health triangle width */
  const static uint8_t kALLY_HEALTH_W; /* Ally health bar width */
  const static uint16_t kALLY_HEIGHT; /* Ally display section height */
  const static uint8_t kALLY_QD_H; /* Ally qd bar height */
  const static uint8_t kALLY_QD_OFFSET; /* Ally qd bar offset off health */
  const static uint8_t kALLY_QD_TRIANGLE; /* Ally qd triangle width */
  const static uint8_t kALLY_QD_W; /* Ally qd bar width */
  const static uint16_t kANIMATION_PROCESS; /* Time to process actions */
  const static uint8_t kCOLOR_BASE; /* Base of color for shifting bars */
  const static uint16_t kBIGBAR_CHOOSE; /* Additional offset for choice */
  const static float kBIGBAR_L; /* The percentage of the left section */
  const static float kBIGBAR_M; /* The percentage of the middle section */
  const static uint16_t kBIGBAR_OFFSET; /* Offset of bar off bottom */
  const static float kBIGBAR_R; /* The percentage of the right section */
  const static uint8_t kINFO_BORDER; /* Border width on enemy info bar */
  const static uint8_t kINFO_GREY; /* Grey value for border bar */
  const static uint16_t kINFO_H; /* Height of enemy info bar */
  const static uint8_t kINFO_OPACITY; /* Opacity of black background in info */
  const static uint8_t kINFO_TRIANGLE; /* Height of triangle in info corner */
  const static uint16_t kINFO_W; /* Width of enemy info bar */
  const static uint8_t kFOE_BAR_H; /* Height of health bar for foes */
  const static uint8_t kFOE_BAR_OFFSET; /* Offset of foe health off center */
  const static uint8_t kFOE_BAR_TRIANGLE; /* Width of foe health triangle */
  const static uint16_t kFOE_BAR_W; /* Width of rect in foe health bar */
  const static uint16_t kFOES_BAR_GAP; /* Offset of bar above foes */
  const static uint16_t kFOES_OFFSET; /* Offset of foes from top */
  const static uint16_t kFRIENDS_OFFSET; /* Offset of friends from bottom */
  const static uint8_t kMAX_CHARS; /* Max number of foes in battle */
  const static uint8_t kMAX_LAYERS; /* Max number of layers that can be set */
  const static uint8_t kMENU_SEPARATOR_B; /* Separator gap off bottom */
  const static uint8_t kMENU_SEPARATOR_T; /* Separator gap off top */ 
  const static uint16_t kPERSON_SPREAD; /* Rendering overlay of persons */
  const static uint16_t kPERSON_WIDTH; /* Width of persons on battle */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Generates info and frames for foes in battle */
  void createFoeBackdrop(SDL_Renderer* renderer);
  Frame* createFoeInfo(Person* foe, SDL_Renderer* renderer);

  /* Creates the rendering fonts, based on the system options font path */
  bool createFonts();

  /* Generates info for friends in battle */
  Frame* createFriendInfo(Person* ally, SDL_Renderer* renderer);

  /* Deletes the rendering fonts, if they've been created */
  void deleteFonts();
  
  /* Get foes party in battle */
  Party* getFoesParty();
  PersonState* getFoesState(int32_t index);

  /* Get friends in battle */
  Party* getFriendsParty();
  PersonState* getFriendsState(int32_t index);

  /* Returns modified index */
  uint32_t getIndex(int32_t index);

  /* Renders the ailments for a given person at a given location */
  bool renderAilment(SDL_Renderer* renderer, Person* person, 
                     uint16_t x, uint16_t y, bool foe = false, 
                     bool full_border = false);

  /* Renders the battle bar */
  bool renderBar(SDL_Renderer* renderer, uint16_t screen_width, 
                                         uint16_t screen_height);

  /* Renders the foes */
  bool renderFoes(SDL_Renderer* renderer, uint16_t screen_width);
  bool renderFoesInfo(SDL_Renderer* renderer, uint16_t screen_width);

  /* Renders the friends */
  bool renderFriendInfo(SDL_Renderer* renderer, PersonState* state,
                        uint16_t screen_height, uint16_t x, uint16_t y,
                        bool below = false);
  bool renderFriends(SDL_Renderer* renderer, uint16_t screen_height);
  bool renderFriendsInfo(SDL_Renderer* renderer, uint16_t screen_height);

  /* Render menu at bottom of screen - for skill selecting */
  bool renderMenu(SDL_Renderer* renderer, PersonState* state, 
                  uint16_t screen_width, uint16_t screen_height);

  /* Set person state */
  bool setPersonState(Person* person, uint8_t index, SDL_Renderer* renderer, 
                      bool foe = false);

  /* Start battle - fires up the variables */
  bool startBattle(SDL_Renderer* renderer);

  /* Stop battle - cleans up */
  void stopBattle();

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

  /* Returns the ailment frame */
  Frame* getAilment(Infliction ailment);

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

  /* Returns the rendering turn state - may be slightly varied with battle */
  TurnState getRenderingState();

  /* Renders the battle display */
  bool render(SDL_Renderer* renderer);

  /* Sets the ailment sprite */
  bool setAilment(Infliction ailment, std::string path, SDL_Renderer* renderer);

  /* Sets the background sprite */
  bool setBackground(Sprite* background);

  /* Sets the battle connected to the display */
  bool setBattle(Battle* battle, SDL_Renderer* renderer);

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
