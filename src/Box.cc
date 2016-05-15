/*******************************************************************************
* Class Name: Box [Implementation]
* Date Created: March 20, 2016
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
#include "Box.h"

const SDL_Color Box::kDEFAULT_COLOR_BG{0, 0, 0, 0};
const SDL_Color Box::kDEFAULT_COLOR_BORDER{255, 255, 255, 255};
const SDL_Color Box::kDEFAULT_COLOR_SCROLL{255, 255, 255, 122};
const SDL_Color Box::kDEFAULT_COLOR_SCROLL_BG{45, 45, 45, 150};
const SDL_Color Box::kDEFAULT_COLOR_SCROLL_SELECTED{255, 255, 255, 255};
const SDL_Color Box::kDEFAULT_COLOR_BLANK{0, 0, 0, 0};
const float Box::kDEFAULT_CYCLE_RATE{0.05};
const uint32_t Box::kDEFAULT_ELEMENT_GAP{0};
const uint32_t Box::kDEFAULT_ELEMENT_INSET_X{4};
const uint32_t Box::kDEFAULT_ELEMENT_INSET_Y{4};
const uint32_t Box::kDEFAULT_SCROLL_INSET_X{4};
const uint32_t Box::kDEFAULT_SCROLL_INSET_Y{4};
const uint32_t Box::kDEFAULT_SCROLL_WIDTH{8};

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
Box::Box()
{
  loadDefaults();
}

/*
 * Description:
 *
 * Inputs:
 */
Box::Box(Coordinate point, int32_t width, int32_t height) : Box()
{
  this->point = point;
  this->width = width;
  this->height = height;
}

/*
 * Description:
 *
 * Inputs:
 */
Box::Box(Coordinate point, int32_t width, int32_t height,
         std::vector<Frame*> elements)
    : Box(point, width, height)
{
  // TODO
  setElements(elements);
}

Box::~Box()
{
  clearElements();
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
void Box::loadDefaults()
{
  bar_amount = 0;
  bar_degrees = 45;
  box_type = BoxType::NORMAL_BOX;

  color_bar = kDEFAULT_COLOR_BORDER;
  color_bg = kDEFAULT_COLOR_BG;
  color_bg_selected = kDEFAULT_COLOR_BG;
  color_border = kDEFAULT_COLOR_BORDER;
  color_border_selected = kDEFAULT_COLOR_BORDER;
  color_element_border = kDEFAULT_COLOR_BLANK;
  color_element_selected = kDEFAULT_COLOR_BLANK;
  color_element_border_selected = kDEFAULT_COLOR_BLANK;
  color_scroll = kDEFAULT_COLOR_SCROLL;
  color_scroll_bg = kDEFAULT_COLOR_SCROLL_BG;
  color_scroll_selected = kDEFAULT_COLOR_SCROLL_SELECTED;
  color_scroll_bg_selected = kDEFAULT_COLOR_SCROLL_BG;

  corner_inset = 0;

  cycle_box_rate = kDEFAULT_CYCLE_RATE;
  cycle_element_rate = kDEFAULT_CYCLE_RATE;
  element_index = -1;
  view_index = -1;
  element_gap = kDEFAULT_ELEMENT_GAP;
  element_inset_x = kDEFAULT_ELEMENT_INSET_X;
  element_inset_y = kDEFAULT_ELEMENT_INSET_Y;
  flags = static_cast<BoxState>(0);
  point = Coordinate();
  height = 0;
  width = 0;
  scroll_inset_x = kDEFAULT_SCROLL_INSET_X;
  scroll_inset_y = kDEFAULT_SCROLL_INSET_Y;
  scroll_width = kDEFAULT_SCROLL_WIDTH;
  width_border = 1;
  width_border_selected = 1;
  width_element_border = 1;
  width_element_border_selected = 1;

  clearElements();
}

/* Render the box as a bar */
bool Box::renderBar(SDL_Renderer* renderer)
{
  auto bar_width = (int32_t)std::round(width * bar_amount);
  bool success = false;

  if(renderer && bar_degrees <= 90)
  {
    success = true;

    auto slope_rads = height / std::tan(bar_degrees * 3.14159265 / 180.0);
    auto delta_x = (int32_t)std::round(slope_rads);

    /* Find the four points of the scroll bar */
    Coordinate tl = {point.x + delta_x, point.y};
    Coordinate bl = {point.x, point.y + height};
    Coordinate tr = {point.x + width, point.y};
    Coordinate br = {point.x + width - delta_x, point.y + height};

    Coordinate bar_t = {tl.x + bar_width, tl.y};
    Coordinate bar_b = {bl.x + bar_width, bl.y + height};

    /* Find the two points where the bar ends */
    auto top_bar = Helpers::bresenhamPoints(tl, tr);
    auto bot_bar = Helpers::bresenhamPoints(bl, br);
    auto left_bar = Helpers::bresenhamPoints(bl, tl);
    auto right_bar = Helpers::bresenhamPoints(br, tr);
    auto fill_bar = Helpers::bresenhamPoints(bar_b, bar_t);

    /* Render the background */
    Frame::setRenderDrawColor(renderer, color_bg);
    Frame::renderFillLineToLine(left_bar, right_bar, renderer);

    /* Render the foreground */
    Frame::setRenderDrawColor(renderer, color_bar);
    Frame::renderFillLineToLine(left_bar, right_bar, renderer);

    /* Render the border */
    Frame::setRenderDrawColor(renderer, color_border);
    Frame::drawLine(top_bar, renderer);
    Frame::drawLine(left_bar, renderer);
    Frame::drawLine(bot_bar, renderer);
    Frame::drawLine(right_bar, renderer);
  }

  return success;
}

// Render elements
bool Box::renderElements(SDL_Renderer* renderer, uint32_t start_index,
                         uint32_t num_viewable)
{
  bool success = false;

  if(renderer)
  {
    success = true;

    uint32_t curr_x = point.x + element_inset_x;
    uint32_t curr_y = point.y + element_inset_y;

    /* From start_index, loop through and render num_viewable elements */
    for(uint32_t i = start_index; i < start_index + num_viewable; i++)
    {
      if(elements.at(i))
      {
        SDL_Color color = kDEFAULT_COLOR_BLANK;
        SDL_Color color_fill = kDEFAULT_COLOR_BLANK;
        uint32_t border_width = 0;

        /* If the element is currently selected, render its selected border */
        if(element_index > -1 && (uint32_t)element_index == i)
        {
          color = color_element_border_selected;
          color_fill = color_element_selected;
          border_width = width_element_border;
        }
        else
        {
          color = color_element_border;
          border_width = width_element_border_selected;
        }

        SDL_Rect rect;
        rect.x = curr_x;
        rect.y = curr_y;
        rect.h = elements.at(i)->getHeight();
        rect.w = elements.at(i)->getWidth();

        /* Render the Fill Rect */
        Frame::setRenderDrawColor(renderer, color_fill);
        success &= SDL_RenderFillRect(renderer, &rect);

        /* Set the render draw color, render the border box */
        Frame::setRenderDrawColor(renderer, color);
        success &= Frame::renderRect(rect, border_width, renderer);

        /* Render the element and update the running Y-Coordinate */
        success &= elements.at(i)->render(renderer, curr_x, curr_y);
        curr_y += elements.at(i)->getHeight() + element_gap;
      }
    }
  }

  return success;
}

bool Box::renderScrollBar(SDL_Renderer* renderer, uint32_t num_viewable)
{
  bool success = false;

  if(renderer && elements.size() > 0)
  {
    success = true;

    /* Allow proper alignment of triangle / circle points */
    if(scroll_width % 2 == 0)
      scroll_width++;

    /* Calculated Values */
    uint32_t tri_height = (uint32_t)std::round(scroll_width * 0.65);
    uint32_t tri_inset = (uint32_t)std::round(scroll_width * 0.1);
    uint32_t circle_size = (uint32_t)std::round(scroll_width * 0.25);

    /* Working coordinate */
    int32_t scroll_x = point.x + width - scroll_inset_x - scroll_width;
    int32_t scroll_y = point.y + 2 * scroll_inset_y + tri_height;

    /* Centre Coordinates */
    uint32_t center_x = scroll_x + scroll_width / 2;

    if(getFlag(BoxState::SELECTED))
      Frame::setRenderDrawColor(renderer, color_scroll);
    else
      Frame::setRenderDrawColor(renderer, color_scroll_selected);

    /* Render the top scroll bar portion (arrow / triangle where appropriate) */
    if(view_index == 0)
    {
      success &= Frame::renderCircleFilled(
          center_x, point.y + scroll_inset_y + 1, circle_size, renderer);
    }
    else if(scroll_x > 0)
    {
      /* Top Triangle Coordinates */
      UCoordinate t1 = {scroll_x + tri_inset, scroll_y - scroll_inset_y};
      UCoordinate t3 = {scroll_x + scroll_width * 4 / 5 - tri_inset, t1.y};
      UCoordinate t2 = {t1.x + (t3.x - t1.x) / 2, point.y + scroll_inset_y};

      /* Render the top triangle */
      success &= Frame::renderTriangle(t1.x, t1.y, t2.x, t2.y, t3.x, t3.y,
                                       renderer, false);
    }

    /* Render the bar behind the scroll bar */
    uint32_t bar_height = height - (4 * scroll_inset_y) - (2 * tri_height);

    if(getFlag(BoxState::SCROLL_BAR))
    {
      SDL_Rect scroll_bar_bg;
      scroll_bar_bg.x = scroll_x;
      scroll_bar_bg.y = scroll_y;
      scroll_bar_bg.h = bar_height;
      scroll_bar_bg.w = scroll_width;

      if(getFlag(BoxState::SELECTED))
        Frame::setRenderDrawColor(renderer, color_scroll_bg);
      else
        Frame::setRenderDrawColor(renderer, color_scroll_bg_selected);

      SDL_RenderFillRect(renderer, &scroll_bar_bg);

      /* Determine the appropriate height and position for the scroll bar */
      uint32_t scroll_height = (uint32_t)std::round(
          (float)bar_height * (float)num_viewable / (float)elements.size());

      uint32_t scroll_bar_y =
          scroll_y + (uint32_t)std::round((float)(view_index) /
                                          elements.size() * bar_height);
      SDL_Rect scroll_bar;
      scroll_bar.x = scroll_x;
      scroll_bar.y = scroll_bar_y;
      scroll_bar.h = scroll_height;
      scroll_bar.w = scroll_width;

      if(getFlag(BoxState::SELECTED))
        Frame::setRenderDrawColor(renderer, color_scroll);
      else
        Frame::setRenderDrawColor(renderer, color_scroll_selected);

      SDL_RenderFillRect(renderer, &scroll_bar);
    }

    /* Bottom Scroll Bar Coordinate */
    uint32_t b_scroll_y = scroll_y + bar_height;

    if(view_index + num_viewable < elements.size() && scroll_x > 0)
    {
      /* Bottom Triangle Coordinates */
      UCoordinate t4 = {scroll_x + tri_inset, b_scroll_y + scroll_inset_y};
      UCoordinate t6 = {scroll_x + scroll_width * 4 / 5 - tri_inset, t4.y};
      UCoordinate t5 = {t4.x + (t6.x - t4.x) / 2,
                        b_scroll_y + scroll_inset_y + tri_height};

      success &= Frame::renderTriangle(t4.x, t4.y, t5.x, t5.y, t6.x, t6.y,
                                       renderer, false);
    }
    else
    {
      success &= Frame::renderCircleFilled(
          center_x, scroll_y + bar_height + scroll_inset_y + tri_height / 2 - 1,
          circle_size, renderer);
    }
  }

  return success;
}

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/

void Box::clearElements()
{
  for(auto& element : elements)
    if(element)
      delete element;

  elements.clear();
}
bool Box::nextIndex()
{
  /* If it's a selectable box, update the view index (unless at the bottom) */
  if(getFlag(BoxState::SELECTABLE) && elements.size() > 0 && view_index > -1 &&
     element_index > -1)
  {
    if((uint32_t)element_index == view_index + getNumViewable() - 1 &&
       (uint32_t)element_index + 1 < elements.size())
    {
      view_index++;
      element_index++;

      return true;
    }
    else if(element_index > -1 && (uint32_t)element_index + 1 < elements.size())
    {
      element_index++;

      return true;
    }
  }
  else if(view_index > -1 && view_index + getNumViewable() < elements.size())
  {
    view_index++;

    return true;
  }

  return false;
}

bool Box::prevIndex()
{
  if(getFlag(BoxState::SELECTABLE) && elements.size() > 0)
  {
    if(element_index > view_index)
    {
      element_index--;

      return true;
    }
    else if(view_index > 0)
    {
      element_index--;
      view_index--;

      return true;
    }
  }
  else if(view_index > 0 && elements.size() > 0)
  {
    view_index--;

    return true;
  }

  return false;
}

bool Box::render(SDL_Renderer* renderer)
{
  bool success = false;

  if(renderer)
  {
    SDL_Color bg_color;
    SDL_Color border_color;
    int32_t border_width;

    if(getFlag(BoxState::SELECTED))
    {
      bg_color = color_bg_selected;
      border_color = color_border_selected;
      border_width = width_border_selected;
    }
    else
    {
      bg_color = color_bg;
      border_color = color_border;
      border_width = width_border;
    }

    success = true;
    /* Render the background rectangle */
    SDL_Rect rect;
    rect.x = point.x;
    rect.y = point.y;
    rect.h = height;
    rect.w = width;

    if(box_type == BoxType::NORMAL_BOX)
    {
      Frame::setRenderDrawColor(renderer, bg_color);
      SDL_RenderFillRect(renderer, &rect);

      /* Render the border based on whether this scroll box is currently being
       * selected (hovered on). */
      Frame::setRenderDrawColor(renderer, border_color);
      success &= Frame::renderRect(rect, border_width, renderer);
    }
    else if(box_type == BoxType::CORNER_CUT_BOX)
    {
      /* Render the frame outline and backdrop */
      Coordinate tl = {point.x, point.y};
      Coordinate tr = {point.x + width - corner_inset, point.y};
      Coordinate bl = {point.x, point.y + height};
      Coordinate br = {point.x + width - corner_inset, point.y + height};

      Coordinate blc = {br.x + 1, br.y - 1};
      Coordinate tlc = {tr.x + 1, tl.y};
      Coordinate trc = {tr.x + corner_inset, tr.y};
      Coordinate brc = {br.x + corner_inset, br.y - corner_inset};

      Coordinate atl = {blc.x - 1, blc.y};
      Coordinate abl = {blc.x, blc.y + 1};
      Coordinate atr = {brc.x - 1, brc.y};
      Coordinate abr = {brc.x, brc.y + 1};

      auto top_bar = Helpers::bresenhamPoints(tl, tr);
      auto bot_bar = Helpers::bresenhamPoints(bl, br);
      auto top_corner = Helpers::bresenhamPoints(tlc, trc);
      auto bot_corner = Helpers::bresenhamPoints(blc, brc);
      auto right_line = Helpers::bresenhamPoints(trc, brc);
      auto corner_aa_top = Helpers::bresenhamPoints(atl, atr);
      auto corner_aa_bot = Helpers::bresenhamPoints(abl, abr);

      Frame::setRenderDrawColor(renderer, bg_color);
      Frame::renderFillLineToLine(top_bar, bot_bar, renderer, true);
      Frame::renderFillLineToLine(top_corner, bot_corner, renderer, true);

      Frame::setRenderDrawColor(renderer, border_color);
      Frame::drawLine(top_bar, renderer);
      Frame::drawLine(bot_bar, renderer);
      Frame::drawLine(top_corner, renderer);
      Frame::drawLine(bot_corner, renderer);
      Frame::drawLine(right_line, renderer);

      /* Anti-Aliased Top Line */
      // Frame::setRenderDrawColor(renderer, {255, 255, 255, 45});
      // Frame::drawLine(corner_aa_top, renderer);

      /* Anti-Aliased Bot Line */
      // Frame::setRenderDrawColor(renderer, {255, 255, 255, 80});
      // Frame::drawLine(corner_aa_bot, renderer);
    }
    else if(box_type == BoxType::BAR)
    {
      success &= renderBar(renderer);
    }

    /* Grab the number of viewable elements within the box */
    auto num_viewable = getNumViewable();

    /* Render the scroll bar, if the No. viewable is less than No. of elmnts */
    if(view_index != -1 && element_index != -1 &&
       num_viewable < elements.size())
    {
      success &= renderScrollBar(renderer, num_viewable);
    }

    /* Render the required elements */
    if(elements.size())
      success &= renderElements(renderer, view_index, num_viewable);
  }

  return success;
}

bool Box::getFlag(const BoxState& test_flag)
{
  return static_cast<bool>((flags & test_flag) == test_flag);
}

uint32_t Box::getNumViewable()
{
  uint32_t num_viewable{0};

  if(height > 0 && element_index > -1 && view_index > -1)
  {
    uint32_t curr_y{element_inset_y};
    uint32_t end_y{(uint32_t)height - element_inset_y};
    uint32_t temp_index{(uint32_t)view_index};
    bool done{false};

    while(temp_index < elements.size() && !done)
    {
      curr_y += (uint32_t)elements.at(temp_index)->getHeight();
      temp_index++;

      if(temp_index < elements.size())
        if(curr_y + (uint32_t)elements.at(temp_index)->getHeight() >= end_y)
          done = true;
    }

    num_viewable = temp_index - view_index;
  }

  return num_viewable;
}

int32_t Box::getNumElements()
{
  return elements.size();
}

void Box::setElements(std::vector<Frame*> elements)
{
  element_index = -1;
  view_index = -1;

  if(elements.size() > 0)
  {
    this->elements = elements;
    element_index = 0;
    view_index = 0;
  }
}

void Box::setFlag(BoxState set_flags, const bool& set_value)
{
  (set_value) ? (flags |= set_flags) : (flags &= ~set_flags);
}
