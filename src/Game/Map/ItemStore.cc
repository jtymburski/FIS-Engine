/*******************************************************************************
 * Class Name: ItemStore [Implementation]
 * Date Created: January 11, 2014
 * Inheritance: None
 * Description: This view is the item storage / access that appears on the map.
 *              It allows for direct access to items which can be useful for
 *              shops, inventories, chests, etc. Contains the appropriate API
 *              for setting up a view and then it runs off of update, render, 
 *              and keyPressEvents.
 *
 * Notes
 * -----
 *
 * [1]:
 *
 * See .h file for TODOs
 ******************************************************************************/
#include "Game/Map/ItemStore.h"

/* Constant Implementation - see header file for descriptions */
const uint8_t ItemStore::kALPHA_MAX = 255;
const uint16_t ItemStore::kBACKEND_RIGHT_X = 119;
const uint16_t ItemStore::kBACKEND_RIGHT_Y = 330;
const uint8_t ItemStore::kBACKEND_TITLE_HEIGHT = 42;
const uint8_t ItemStore::kGRID_ALPHA = 64;
const uint8_t ItemStore::kGRID_HEIGHT = 4;
const uint8_t ItemStore::kGRID_SIZE = 8;
const uint8_t ItemStore::kGRID_WIDTH = 5;
const uint16_t ItemStore::kSHOW_TIME = 750;
const uint8_t ItemStore::kTILE_SIZE = 64;

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *============================================================================*/

/*
 * Description:
 *
 * Inputs:
 */
ItemStore::ItemStore(Options* running_config)
{
  clearData();
  setConfiguration(running_config);
}

/*
 * Description:
 */
ItemStore::~ItemStore()
{
  deleteFonts();
  clearData();
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
void ItemStore::clearData()
{
  event_handler = NULL;
  font_title = NULL;
  store_status = WindowStatus::OFF;
  system_options = NULL;
}

/*
 * Description: Attempts to create the main font and the title font that is used
 *              throughout dialog representation. This is acquired from the
 *              configuration of the application. If successful, set the new
 *              fonts.
 *
 * Inputs: none
 * Output: bool - status if creation was successful
 */
bool ItemStore::createFonts()
{
  bool success = false;

  if(system_options != NULL)
  {
    /* Try and create the new fonts */
    //TTF_Font* regular_font = Text::createFont(system_options->getBasePath() + 
    //                                          system_options->getFont(), 20);
    TTF_Font* title_font = Text::createFont(system_options->getBasePath() + 
                                            system_options->getFont(), 
                                            20, TTF_STYLE_BOLD);

    /* If successful, insert the new fonts. Otherwise, delete if any were
     * created */
    if(title_font != NULL)//regular_font != NULL && title_font != NULL)
    {
      deleteFonts();
      //font_normal = regular_font;
      font_title = title_font;
      success = true;
    }
    else
    {
      //TTF_CloseFont(regular_font);
      //regular_font = NULL;

      TTF_CloseFont(title_font);
      title_font = NULL;
    }
  }

  return success;
}

/*
 * Description: Delete the rendering fonts stored in the class. Do not try and
 *              render again since it will try to use the NULL fonts and fail.
 *
 * Inputs: none
 * Ouptut: none
 */
void ItemStore::deleteFonts()
{
  //TTF_CloseFont(font_normal);
  //font_normal = NULL;

  TTF_CloseFont(font_title);
  font_title = NULL;
}

/* Sets the alpha of all rendering textures on the dialog */
void ItemStore::setAlpha(uint8_t alpha)
{
  store_alpha = alpha;
  
  /* Update the alpha of all the frames */
  frame_main.setAlpha(alpha);
  img_backend_right.setAlpha(alpha);
}

/* Sets up the views, to be rendered on the screen */
void ItemStore::setupMainView(SDL_Renderer* renderer, std::string title)
{
  int render_height = img_backend_left.getHeight();
  if(!title.empty())
    render_height += kBACKEND_TITLE_HEIGHT + 1; /* Extra 1 for border */
  int render_width = img_backend_left.getWidth();
  
  /* Create rendering texture */
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, 
                                           render_width, render_height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  /* Render main frame */
  img_backend_left.render(renderer, 0, 
                          render_height - img_backend_left.getHeight());

  /* Render name */
  if(!title.empty())
  {
    Text title_text(font_title);
    title_text.setText(renderer, title, {255, 255, 255, 255});

    /* Rectangle beneath name */
    SDL_Rect rect;
    rect.x = (render_width - title_text.getWidth()) / 2;
    rect.y = 1;
    rect.w = title_text.getWidth();
    rect.h = kBACKEND_TITLE_HEIGHT;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255*0.65);
    SDL_RenderFillRect(renderer, &rect);

    /* Triangles around name */
    Frame::renderRHTriangle(rect.x, rect.y, kBACKEND_TITLE_HEIGHT, 
                            renderer, true);
    Frame::renderRHTriangle(rect.x + rect.w, rect.y, kBACKEND_TITLE_HEIGHT, 
                            renderer);

    /* Name text */
    title_text.render(renderer, rect.x, 
                      (kBACKEND_TITLE_HEIGHT - title_text.getHeight()) / 2);

    /* Top and Left hand side triangle border */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    rect.y = 0;
    rect.h = 3;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawLine(renderer, rect.x, rect.y, 
                       rect.x - kBACKEND_TITLE_HEIGHT, 
                       rect.y + kBACKEND_TITLE_HEIGHT);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + 1, 
                       rect.x - kBACKEND_TITLE_HEIGHT, 
                       rect.y + 1 + kBACKEND_TITLE_HEIGHT);
    SDL_RenderDrawLine(renderer, rect.x, rect.y - 1, 
                       rect.x - kBACKEND_TITLE_HEIGHT - 1, 
                       rect.y + kBACKEND_TITLE_HEIGHT);
    SDL_RenderDrawLine(renderer, rect.x, rect.y - 2, 
                       rect.x - kBACKEND_TITLE_HEIGHT - 2, 
                       rect.y + kBACKEND_TITLE_HEIGHT);

    /* Right hand side triangle border */
    rect.x += title_text.getWidth();
    SDL_RenderDrawLine(renderer, rect.x, rect.y, 
                       rect.x + kBACKEND_TITLE_HEIGHT, 
                       rect.y + kBACKEND_TITLE_HEIGHT);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + 1, 
                       rect.x + kBACKEND_TITLE_HEIGHT, 
                       rect.y + kBACKEND_TITLE_HEIGHT + 1);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + 2, 
                       rect.x + kBACKEND_TITLE_HEIGHT, 
                       rect.y + kBACKEND_TITLE_HEIGHT + 2);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + 3, 
                       rect.x + kBACKEND_TITLE_HEIGHT, 
                       rect.y + kBACKEND_TITLE_HEIGHT + 3);
  }

  /* Draw the grid space */
  int grid_x = (img_backend_left.getWidth() - kGRID_WIDTH * kTILE_SIZE) / 2;
  int grid_y = (img_backend_left.getHeight() - kGRID_HEIGHT * kTILE_SIZE) / 2;
  if(!title.empty())
    grid_y += kBACKEND_TITLE_HEIGHT;
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, kGRID_ALPHA);
  for(uint8_t i = 0; i < kGRID_WIDTH; i++)
  {
    for(uint8_t j = 0; j < kGRID_HEIGHT; j++)
    { 
      SDL_Rect rects[8];

      /* Top left corner */
      rects[0] = {grid_x + i*kTILE_SIZE, 
                  grid_y + j*kTILE_SIZE, kGRID_SIZE + 1, 1};
      rects[1] = {rects[0].x, rects[0].y + 1, 1, kGRID_SIZE};

      /* Top right corner */
      rects[2] = {rects[0].x + kTILE_SIZE - kGRID_SIZE - 1, 
                  rects[0].y, kGRID_SIZE + 1, 1};
      rects[3] = {rects[0].x + kTILE_SIZE - 1, rects[0].y + 1, 1, kGRID_SIZE};

      /* Bottom left corner */
      rects[4] = {rects[0].x, rects[0].y + kTILE_SIZE - 1, kGRID_SIZE + 1, 1};
      rects[5] = {rects[0].x, 
                  rects[0].y + kTILE_SIZE - kGRID_SIZE - 1, 1, kGRID_SIZE};

      /* Bottom right corner */
      rects[6] = {rects[4].x + kTILE_SIZE - kGRID_SIZE - 1, 
                  rects[4].y, kGRID_SIZE + 1, 1};
      rects[7] = {rects[4].x + kTILE_SIZE - 1, rects[5].y, 1, kGRID_SIZE};

      /* Modifications for vertical edges */
      if(i == 0)
      {
        rects[0].x--;
        rects[0].w++;
        rects[1].x--;
        rects[1].w++;
        rects[4].x--;
        rects[4].w++;
        rects[5].x--;
        rects[5].w++;
      }
      else if(i == (kGRID_WIDTH - 1))
      {
        rects[2].w++;
        rects[3].w++;
        rects[6].w++;
        rects[7].w++;
      }

      /* Modifications for horizontal edges */
      if(j == 0)
      {
        rects[0].y--;
        rects[0].h++;
        rects[2].y--;
        rects[2].h++;
      }
      else if(j == (kGRID_HEIGHT - 1))
      {
        rects[4].h++;
        rects[6].h++;
      }

      /* Paint call for all rects */
      SDL_RenderFillRects(renderer, rects, 8);
    }
  }

  /* Finally, fill in the main frame */
  SDL_SetRenderTarget(renderer, NULL);
  frame_main.setTexture(texture);
}

void ItemStore::setupSecondaryView(SDL_Renderer* renderer)
{

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

/* Closes the active display */
void ItemStore::close()
{
  if(store_status == WindowStatus::ON || store_status == WindowStatus::SHOWING)
    store_status = WindowStatus::HIDING;
}

bool ItemStore::initDisplay(StoreMode mode, std::vector<Item*> items, 
                            std::vector<uint32_t> counts, 
                            std::vector<int32_t> cost_modifiers, 
                            std::string name, bool show_empty)
{
  /* Only proceed if the store is not being used */
  if(store_status == WindowStatus::OFF)
  {
    /* Clear the current store stack */
    store_stack.clear();
    
    /* Run through all items */
    for(uint32_t i = 0; i < items.size(); i++)
    {
      /* Only proceed if the counts is large enough and if it should be shown
       * based on empty qualifications (depending on show_empty) variable */
      if(items[i] != NULL && counts.size() > i && 
         ((!show_empty && counts[i] > 0) || show_empty))
      {
        /* Create the initial display item */
        ItemDisplay new_item;
        new_item.item = items[i];
        new_item.count = counts[i];
        new_item.cost = items[i]->getValue();
        new_item.category = ItemFlags::NO_CATEGORY;
        
        /* Modify the cost, if applicable */
        int32_t cost_signed = new_item.cost;
        if(cost_modifiers.size() > i && (cost_modifiers[i] * -1) <= cost_signed)
          new_item.cost += cost_modifiers[i];
        else if(cost_modifiers.size() == 1 && 
                (cost_modifiers[0] * -1) <= cost_signed)
          new_item.cost += cost_modifiers[0];
          
        /* Modify the item category, if applicable */
        if(items[i]->getFlag(ItemFlags::EQUIPMENT))
          new_item.category = ItemFlags::EQUIPMENT;
        else if(items[i]->getFlag(ItemFlags::BUBBY))
          new_item.category = ItemFlags::BUBBY;
        else if(items[i]->getFlag(ItemFlags::KEY_ITEM))
          new_item.category = ItemFlags::KEY_ITEM;
        else if(items[i]->getFlag(ItemFlags::MONEY))
          new_item.category = ItemFlags::MONEY;
        else if(!items[i]->getFlag(ItemFlags::NO_CATEGORY))
          new_item.category = ItemFlags::CONSUMED;
        
        /* Push it onto the stack */
        if(new_item.category != ItemFlags::KEY_ITEM)
          store_stack.push_back(new_item);
      }
    }
    
    /* Final setup of configuration variables */
    frame_setup = true;
    store_mode = mode;
    store_title = name;
    return true;
  }
  
  return false;
}

/* Returns if the item store is currently active */
bool ItemStore::isActive()
{
  return (store_status != WindowStatus::OFF && 
          store_status != WindowStatus::HIDING);
}

/* Key Down/Up events handled */
void ItemStore::keyDownEvent(SDL_KeyboardEvent event)
{

}

void ItemStore::keyUpEvent(SDL_KeyboardEvent event)
{

}

bool ItemStore::loadImageBackend(std::string left, std::string right, 
                                                   SDL_Renderer* renderer)
{
  bool success = true;

  /* Get the base path */
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  /* Load the frame data */
  success &= img_backend_left.setTexture(base_path + left, renderer);
  success &= img_backend_right.setTexture(base_path + right, renderer);

  return success;
}

/* Renders the Item Store */
bool ItemStore::render(SDL_Renderer* renderer)
{
  /* Check if the main frame needs to be setup */
  if(frame_setup)
  {
    setupMainView(renderer, store_title);
    frame_setup = false;
    store_status = WindowStatus::SHOWING;
    setAlpha(0);
  }

  /* Render the item store */
  if(store_status != WindowStatus::OFF)
  {
    int x = 0;
    int y = 0;
    
    /* Try to set the render location */
    if(system_options != NULL)
    {
      x = (system_options->getScreenWidth() - frame_main.getWidth() 
            - img_backend_right.getWidth() + kBACKEND_RIGHT_X) / 2;
      y = (system_options->getScreenHeight() - frame_main.getHeight() 
            - img_backend_right.getHeight() + kBACKEND_RIGHT_Y) / 2;
    }

    /* Render the frames */
    frame_main.render(renderer, x, y);
    img_backend_right.render(renderer, 
                             x + frame_main.getWidth() - kBACKEND_RIGHT_X, 
                             y + frame_main.getHeight() - kBACKEND_RIGHT_Y);
  }
}

/* Sets the running configuration, from the options class */
bool ItemStore::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    createFonts();

    return true;
  }

  return false;
}

/* Sets the event handler */
void ItemStore::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;
}

/* Updates the item store, called on the tick */
void ItemStore::update(int cycle_time)
{
  /* When the item store is showing, up the alpha until fully visible */
  if(store_status == WindowStatus::SHOWING)
  {
    /* Determine the differential alpha for showing */
    float alpha_diff = cycle_time * 1.0 * kALPHA_MAX / kSHOW_TIME;
    if(alpha_diff < 1)
      alpha_diff = 1.0;
    
    /* Compute the change in alpha */
    if(store_alpha + alpha_diff >= kALPHA_MAX)
      setAlpha(kALPHA_MAX);
    else
      setAlpha(store_alpha + alpha_diff);
      
    /* If the alpha has reached max, set the status as fully on */
    if(store_alpha == kALPHA_MAX)
      store_status = WindowStatus::ON;
  }
  /* When the item store is hiding, down the alpha until fully invisible */
  else if(store_status == WindowStatus::HIDING)
  {
    /* Determine the differential alpha for hiding */
    float alpha_diff = cycle_time * 1.0 * kALPHA_MAX / kSHOW_TIME;
    if(alpha_diff < 1)
      alpha_diff = 1.0;
      
    /* Compute the change in alpha */
    if(alpha_diff >= store_alpha)
      setAlpha(0);
    else
      setAlpha(store_alpha - alpha_diff);
    
    /* If the alpha has reached 0, clear the store status */
    if(store_alpha == 0)
      store_status = WindowStatus::OFF;
  }
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/