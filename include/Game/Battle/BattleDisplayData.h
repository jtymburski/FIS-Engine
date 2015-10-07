/*******************************************************************************
* Class Name: RenderElement
* Date Created: June 22, 2015
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
#ifndef BATTLEDISPLAYDATA_H
#define BATTLEDISPLAYDATA_H

#include <SDL2/SDL.h>
#include <map>

#include "Sprite.h"
#include "EnumDb.h"
#include "Options.h"

using std::begin;
using std::end;

class BattleDisplayData
{
public:
  /* Constructs the BattleDisplay */
  BattleDisplayData();

  /* Constructs the BattleDisplayData */
  ~BattleDisplayData();

private:
  /* Has the data been built? */
  bool data_built;

  /* The configuration pointer */
  Options* config;

  /* The battle bar frame */
  Frame* frame_battle_bar;

  /* The accuracy percent frame */
  Frame* frame_percent;

  /* The QD Frame */
  Frame* frame_qd;

  /* The cooldown frame */
  Frame* frame_time;

  /* Frames for each type of Ailment */
  std::map<Infliction, Frame> frames_ailments;

  /* Frames for each Element */
  std::map<Element, Frame> frames_elements;

  /* Frames for each ActionScope */
  std::map<ActionScope, Frame> frames_scopes;

  /* Pleps for each ailment */
  std::map<Infliction, Sprite*> pleps_ailments;

  /* Pleps for events */
  std::map<EventType, Sprite*> pleps_events;

  /* The SDL Renderer */
  SDL_Renderer* renderer;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Constructs the battle bar */
  void buildBattleBar();

  /* Builds the ailment frames */
  void buildFramesAilments();

  /* Builds elemental frames */
  void buildFramesElements();

  /* Builds action scopes */
  void buildFramesScopes();

  /* Build extra frames */
  void buildFramesExtra();

  /* Builds ailment pleps */
  void buildPlepsAilments();

  /* Build pleps for event types (such as defend) */
  void buildPlepsEvents();

  /* Assigns an ailment frame */
  void setFrameAilment(Infliction type, std::string path);

  /* Assigns an element frame */
  void setFrameElement(Element type, std::string path);

  /* Assigns a scope frame */
  void setFrameScope(ActionScope type, std::string path);

  /* Assigns an ailment plep */
  void setPlepAilment(Infliction type, Sprite* plep);

  /* Assigns a plep event */
  void setPlepEvent(EventType event, Sprite* plep);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Returns whether the data has been built */
  bool isDataBuilt();

  /* Builds the data of the BDD */
  bool buildData();

  /* Obtains the battle bar frame */
  Frame* getBattleBar();

  /* Returns the frame for percent */
  Frame* getFramePercent();

  /* Returns the frame for QD */
  Frame* getFrameQD();

  /* Returns the frame for time */
  Frame* getFrameTime();

  /* Returns an ailment frame of a given Infliction */
  Frame getFrameAilment(Infliction ailment_frame);

  /* Returns an element frame of a given element */
  Frame getFrameElement(Element element_frame);

  /* Returns a scope frame of a given action scope */
  Frame getFrameScope(ActionScope scope_frame);

  /* Returns a pointer to a Plep ailment sprite */
  Sprite* getPlepAilment(Infliction ailment);

  /* Returns a pointer to a Plep event sprite */
  Sprite* getPlepEvent(EventType event_type);

  /* Assign a configuration */
  void setConfig(Options* config);

  /* Assign a renderer */
  void setRenderer(SDL_Renderer* renderer);
};

#endif // BATTLEDISPLAYDATA_H
