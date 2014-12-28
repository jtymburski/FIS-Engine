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

  /* Element frames */
  std::vector<Frame> elements;
  
  /* The rendering enemy info bar */
  Frame* foes_backdrop;
  std::vector<PersonState*> foes_state;

  /* Rendering fonts */
  TTF_Font* font_header;
  TTF_Font* font_subheader;

  /* Rendering frames - in bar */
  Frame frame_percent;
  Frame frame_qd;
  Frame frame_time;

  /* The rendering friend info bar */
  std::vector<PersonState*> friends_state;

  /* Rendering indexes */
  uint16_t index_actions;
  uint16_t index_layer;
  int32_t index_person;
  uint16_t index_types;

  /* Mid scene overlays */
  std::vector<Sprite*> midlays; // TODO: Make overlay class when created

  /* Background overlays */
  std::vector<Sprite*> overlays; // TODO: Make overlay class when created

  /* Rendering turn state */
  TurnState rendering_state;
  
  /* Action scope frames */
  std::vector<Frame> scopes;
  
  /* Show info regardless of state */
  bool show_info;

  /* Skill information for rendering */
  std::vector<Frame*> skill_info;
  std::vector<Frame*> skill_names;

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
  const static float kBIGBAR_M1; /* The percentage of the middle section */
  const static float kBIGBAR_M2; /* The percentage of the second middle */
  const static uint16_t kBIGBAR_OFFSET; /* Offset of bar off bottom */
  const static float kBIGBAR_R; /* The percentage of the right section */
  const static uint16_t kBIGBAR_R_OFFSET; /* Offset off end for right section */
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
  const static uint8_t kSCROLL_R; /* Radius on scroll renders */
  const static uint8_t kSKILL_BORDER; /* Border around edge and elements */
  const static uint8_t kSKILL_BORDER_WIDTH; /* Width of border around element */
  const static uint8_t kSKILL_DESC_GAP; /* Gap between name and description */
  const static uint8_t kSKILL_DESC_LINES; /* Max number of description lines */
  const static uint8_t kSKILL_DESC_SEP; /* Gap between lines in description */
  const static uint8_t kSKILL_FRAME_S; /* Small frame size on skill info */
  const static uint8_t kSKILL_FRAME_L; /* Large frame size on skill info */
  const static uint8_t kSKILL_QD_GAP; /* Gap between top edge and QD icon */
  const static uint8_t kSKILL_SEP; /* Separator between image and text */
  const static uint8_t kSKILL_SUCCESS; /* Gap between success and cooldown */
  const static uint8_t kSKILL_TIME_GAP; /* Gap between cooldown and bottom */
  const static uint8_t kTYPE_MARGIN; /* Margin around text options in type */
  const static uint8_t kTYPE_MAX; /* Max number of action types to render */
  const static uint8_t kTYPE_SELECT; /* Margin to spread select around type */

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

  /* Generates info for the skills of the selecting person in battle */
  SDL_Texture* createSkill(SDL_Renderer* renderer, Skill* skill, 
                           uint16_t width, uint16_t height);
  bool createSkills(SDL_Renderer* renderer, BattleMenu* menu, 
                    uint16_t width_left, uint16_t width_right);
  
  /* Deletes the rendering fonts, if they've been created */
  void deleteFonts();
  
  /* Deletes the rendering skills, for the menu */
  void deleteSkills();
  
  /* Get foes party in battle */
  Party* getFoesParty();
  PersonState* getFoesState(int32_t index);

  /* Get friends in battle */
  Party* getFriendsParty();
  PersonState* getFriendsState(int32_t index);

  /* Returns modified index */
  uint32_t getIndex(int32_t index);

  /* Render the action skills */
  bool renderActionSkills(SDL_Renderer* renderer, BattleMenu* menu, uint16_t x,
                          uint16_t y, uint16_t width, uint16_t height);

  /* Render the action categories */
  bool renderActionTypes(SDL_Renderer* renderer, BattleMenu* menu, uint16_t x, 
                         uint16_t y, uint16_t width, uint16_t height);

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

  /* Returns the element frame */
  Frame* getElement(Element element);

  /* Get the midlay(s) */
  Sprite* getMidlay(uint8_t index);
  std::vector<Sprite*> getMidlays();

  /* Get the overlay(s) */
  Sprite* getOverlay(uint8_t index);
  std::vector<Sprite*> getOverlays();

  /* Returns the rendering turn state - may be slightly varied with battle */
  TurnState getRenderingState();

  /* Returns the action scope frame */
  Frame* getScope(ActionScope scope);

  /* Returns if the battle display is paused and control should be halted */
  bool isPaused();

  /* Renders the battle display */
  bool render(SDL_Renderer* renderer);

  /* Sets the ailment frame */
  bool setAilment(Infliction ailment, std::string path, SDL_Renderer* renderer);

  /* Sets the background sprite */
  bool setBackground(Sprite* background);

  /* Sets the battle connected to the display */
  bool setBattle(Battle* battle, SDL_Renderer* renderer);

  /* Sets the rendering battle bar */
  bool setBattleBar(Frame* bar);

  /* Sets the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Sets the element frame */
  bool setElement(Element element, std::string path, SDL_Renderer* renderer);

  /* Sets the rendering helper frames for display */
  bool setFramePercent(std::string path, SDL_Renderer* renderer);
  bool setFrameQD(std::string path, SDL_Renderer* renderer);
  bool setFrameTime(std::string path, SDL_Renderer* renderer);

  /* Sets the midlay sprite - main one, removes all others */
  bool setMidlay(Sprite* midlay);

  /* Sets the overlay sprite - main one, removes all others */
  bool setOverlay(Sprite* overlay);

  /* Sets the action scope frame */
  bool setScope(ActionScope scope, std::string path, SDL_Renderer* renderer);
  
  /* Sets if info about player should be shown regardless of state */
  void setShowInfo(bool show);
  
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
