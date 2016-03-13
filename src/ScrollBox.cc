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
#include "ScrollBox.h"

const SDL_Color ScrollBox::kDEFAULT_COLOR_BG{0, 0, 0, 150};
const SDL_Color ScrollBox::kDEFAULT_COLOR_BORDER{255, 255, 255, 255};
const SDL_Color ScrollBox::kDEFAULT_COLOR_SCROLL{150, 150, 150, 255};
const uint32_t ScrollBox::kDEFAULT_ELEMENT_GAP{0};
const uint32_t ScrollBox::kDEFAULT_ELEMENT_INSET_X{4};
const uint32_t ScrollBox::kDEFAULT_ELEMENT_INSET_Y{4};
const uint32_t ScrollBox::kDEFAULT_SCROLL_INSET_X{4};
const uint32_t ScrollBox::kDEFAULT_SCROLL_INSET_Y{4};
const uint32_t ScrollBox::kDEFAULT_SCROLL_WIDTH{10};

/*=============================================================================
 * CONSTANTS
 *============================================================================*/

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
ScrollBox::ScrollBox()
{
  loadDefaults();
}

/*
 * Description:
 *
 * Inputs:
 */
ScrollBox::ScrollBox(std::vector<Frame> elements)
{
  loadDefaults();

  // TODO
  this->elements = elements;
}

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/

/*
 * Description: Reset ll the ScrollBox elements to a default state.
 *
 * Inputs: none
 * Output: none
 */
void ScrollBox::loadDefaults()
{
  color_bg = kDEFAULT_COLOR_BG;
  color_border = kDEFAULT_COLOR_BORDER;
  color_border_selected = kDEFAULT_COLOR_BORDER;
  color_scroll = kDEFAULT_COLOR_SCROLL;
  element_index = -1;
  element_gap = kDEFAULT_ELEMENT_GAP;
  element_inset_x = kDEFAULT_ELEMENT_INSET_X;
  element_inset_y = kDEFAULT_ELEMENT_INSET_Y;
  flags = static_cast<ScrollBoxState>(0);
  scroll_box = Box();
  scroll_inset_x = kDEFAULT_SCROLL_INSET_X;
  scroll_inset_y = kDEFAULT_SCROLL_INSET_Y;
  scroll_width = kDEFAULT_SCROLL_WIDTH;
  width_border = 1;
  width_border_selected = 1;
  elements.clear();
}

// Render elements
bool ScrollBox::renderElements(SDL_Renderer* renderer, uint32_t start_index,
                               uint32_t num_viewable)
{
  bool success = false;

  if(renderer)
  {
    success = true;

    uint32_t curr_x = scroll_box.point.x + element_inset_x;
    uint32_t curr_y = scroll_box.point.y + element_inset_y;

    for(uint32_t i = start_index; i < start_index + num_viewable; i++)
    {
      success &= elements.at(i).render(renderer, curr_x, curr_y);
      curr_y += elements.at(i).getHeight() + element_gap;
    }
  }

  return success;
}

/*   []   Non-selectable
  e  [] |    If ( e > 0) --> render arrow on top
     [] |    If ( 1 + 2 < 4) -- > Render arrow on bot
     []
*/
bool ScrollBox::renderScrollBar(SDL_Renderer* renderer, uint32_t num_viewable)
{
  bool success = false;

  if(renderer && elements.size() > 0)
  {
    success = true;

    uint32_t curr_x =
        scroll_box.point.x + scroll_box.width - scroll_inset_x - scroll_width;
    uint32_t curr_y = scroll_box.point.y + scroll_inset_y;

    /* Render the top arrow */
    if(element_index > 0)
    {
    }

    /* Render the actual bar */
    uint32_t bar_height =
        scroll_box.height - (2 * scroll_inset_y) - (2 * scroll_width);

    uint32_t scroll_height = (uint32_t)std::round(
        (float)bar_height * (float)num_viewable / (float)elements.size());
    (void)scroll_height;//TODO

    SDL_Rect scroll_bar;
    scroll_bar.x = curr_x;
    scroll_bar.y = curr_y + scroll_width;
    scroll_bar.h = bar_height;
    scroll_bar.w = scroll_width;

    SDL_SetRenderDrawColor(renderer, color_scroll.r, color_scroll.g,
                           color_scroll.b, color_scroll.a);
    SDL_RenderFillRect(renderer, &scroll_bar);

    /* Render the bottom arrow */
    if(element_index + num_viewable < elements.size())
    {
    }
  }

  return success;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

bool ScrollBox::nextIndex()
{
  if(element_index > -1 && element_index + 1 < (int32_t)elements.size())
  {
    element_index++;

    return true;
  }

  return false;
}

bool ScrollBox::prevIndex()
{
  if(element_index > 0 && elements.size() > 0)
  {
    element_index--;

    return true;
  }

  return false;
}

bool ScrollBox::render(SDL_Renderer* renderer)
{
  bool success = false;

  if(renderer)
  {
    success = true;
    /* Render the background rectangle */
    SDL_Rect rect;
    rect.x = scroll_box.point.x;
    rect.y = scroll_box.point.y;
    rect.h = scroll_box.height;
    rect.w = scroll_box.width;

    SDL_SetRenderDrawColor(renderer, color_bg.r, color_bg.g, color_bg.b,
                           color_bg.a);
    SDL_RenderFillRect(renderer, &rect);

    /* Render the border based on whether this scroll box is currently being
     * selected (hovered on).
     */
    if(getFlag(ScrollBoxState::SELECTED))
    {
      SDL_SetRenderDrawColor(renderer, color_border_selected.r,
                             color_border_selected.g, color_border_selected.b,
                             color_border_selected.a);
      success &= Frame::renderRect(rect, width_border_selected, renderer);
    }
    else
    {
      SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g,
                             color_border.b, color_border.a);
      success &= Frame::renderRect(rect, width_border, renderer);
    }

    /* Grab the number of viewable elements within the box */
    auto num_viewable = getNumViewable();

    /* Render the scroll bar, if the No. viewable is less than No. of elmnts */
    if(num_viewable < elements.size())
      success &= renderScrollBar(renderer, num_viewable);

    /* Render the required elements */
    success &= renderElements(renderer, element_index, num_viewable);
  }

  return success;
}

bool ScrollBox::getFlag(const ScrollBoxState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

uint32_t ScrollBox::getNumViewable()
{
  uint32_t num_viewable{0};

  if(scroll_box.height > 0 && element_index > 0)
  {
    uint32_t curr_y{element_inset_y};
    uint32_t end_y{(uint32_t)scroll_box.height - element_inset_y};
    uint32_t temp_index{(uint32_t)element_index};
    bool done{false};

    while(temp_index < elements.size() && !done)
    {
      curr_y += (uint32_t)elements.at(temp_index).getHeight();
      temp_index++;

      if(temp_index < elements.size())
        if(curr_y + (uint32_t)elements.at(temp_index).getHeight() >= end_y)
          done = true;
    }

    num_viewable = temp_index - element_index;
  }

  return num_viewable;
}

void ScrollBox::setElements(std::vector<Frame> elements)
{
  element_index = -1;

  if(elements.size() > 0)
  {
    this->elements = elements;
    element_index = 0;
  }
}

void ScrollBox::setFlag(ScrollBoxState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}

/*=============================================================================
* PUBLIC STATIC FUNCTIONS
*============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/
