/*******************************************************************************
* Class Name: Box
* Date Created: March 13, 2016
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
#ifndef BOX_H
#define BOX_H

#include "Helpers.h"
#include "EnumFlags.h"
#include "Frame.h"

#include <vector>

using std::begin;
using std::end;

ENUM_FLAGS(ScrollBoxState)
enum class ScrollBoxState
{
  SCROLL_BOX = 1 << 0,
  SCROLL_BAR = 1 << 1,
  SELECTABLE = 1 << 2,
  BORDER_HOVER_CYCLE = 1 << 3,
  ELEMENT_HOVER_CYCLE = 1 << 4,
  HOVER_CYCLE = 1 << 5,
  SELECTED = 1 << 7
};

class Box
{
public:
  /* Constructs a default ScrollBox object */
  Box();

  /* Simple box constructor */
  Box(Coordinate point, int32_t width, int32_t height);

  /* Vector of frames for the elements in a Box */
  Box(Coordinate point, int32_t width, int32_t height,
      std::vector<Frame*> elements);

private:
  /* Current working element index */
  int32_t element_index;
  int32_t view_index;

  /* Vector of Frame elements */
  std::vector<Frame*> elements;

public:
  /* Colors */
  SDL_Color color_bg;
  SDL_Color color_bg_selected;
  SDL_Color color_border;
  SDL_Color color_element_border;
  SDL_Color color_border_selected;
  SDL_Color color_element_border_selected;
  SDL_Color color_scroll;
  SDL_Color color_scroll_bg;
  SDL_Color color_scroll_selected;
  SDL_Color color_scroll_bg_selected;

  /* Cycle rates for the Box / Element */
  float cycle_box_rate;
  float cycle_element_rate;

  /* X, Y insets for the Element Frames */
  uint32_t element_gap;
  uint32_t element_inset_x;
  uint32_t element_inset_y;

  /* Flags describing this ScrollBox object */
  ScrollBoxState flags;

  /* Location and dimension of the Box */
  Coordinate point;
  int32_t width;
  int32_t height;

  /* X, Y insets for the Scroll Bar */
  uint32_t scroll_inset_x;
  uint32_t scroll_inset_y;
  uint32_t scroll_width;

  /* Rectangle border widths */
  uint32_t width_border;
  uint32_t width_border_selected;

  /* Element border widths */
  uint32_t width_element_border;
  uint32_t width_element_border_selected;

  /* ------------ Constants --------------- */
  static const SDL_Color kDEFAULT_COLOR_BG;
  static const SDL_Color kDEFAULT_COLOR_BORDER;
  static const SDL_Color kDEFAULT_COLOR_SCROLL;
  static const SDL_Color kDEFAULT_COLOR_SCROLL_BG;
  static const SDL_Color kDEFAULT_COLOR_SCROLL_SELECTED;
  static const SDL_Color kDEFAULT_COLOR_BLANK;
  static const float kDEFAULT_CYCLE_RATE;
  static const uint32_t kDEFAULT_ELEMENT_GAP;
  static const uint32_t kDEFAULT_ELEMENT_INSET_X;
  static const uint32_t kDEFAULT_ELEMENT_INSET_Y;
  static const uint32_t kDEFAULT_SCROLL_INSET_X;
  static const uint32_t kDEFAULT_SCROLL_INSET_Y;
  static const uint32_t kDEFAULT_SCROLL_WIDTH;

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Load the class variables with default values */
  void loadDefaults();

  /* Render the elements starting at a given index */
  bool renderElements(SDL_Renderer* renderer, uint32_t start_index,
                      uint32_t number);

  /* Render the ScrollBar */
  bool renderScrollBar(SDL_Renderer* renderer, uint32_t num_viewable);

  /*=============================================================================
   * PUBLIC FUNCTIONS
   *============================================================================*/
public:
  /* Updates the scroll box to the next index */
  bool nextIndex();

  /* Updates the scroll box to the previous index */
  bool prevIndex();

  /* Render the scroll box in its current state */
  bool render(SDL_Renderer* renderer);

  /* Returns the value of a given ActorState flag */
  bool getFlag(const ScrollBoxState& test_flag);

  /* Calculate and return the number of viewable elements */
  uint32_t getNumViewable();

  /* Assign a vector of elements to the scroll box */
  void setElements(std::vector<Frame*> elements);

  /* Assigns a given ActorState flag to a given value */
  void setFlag(ScrollBoxState set_flags, const bool& set_value = true);

  /*=============================================================================
   * PUBLIC STATIC FUNCTIONS
   *============================================================================*/
public:
  /*============================================================================
   * OPERATOR FUNCTIONS
   *===========================================================================*/
public:
};

#endif // BOX_H
