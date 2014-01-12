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
const uint16_t ItemStore::kBACKEND_RIGHT_X = 119;
const uint16_t ItemStore::kBACKEND_RIGHT_Y = 330;
const uint8_t ItemStore::kBACKEND_TITLE_HEIGHT = 42;
const uint8_t ItemStore::kGRID_HEIGHT = 4;
const uint8_t ItemStore::kGRID_WIDTH = 5;
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
    Frame::renderRHTriangle(rect.x, rect.y, kBACKEND_TITLE_HEIGHT, renderer, true);
    Frame::renderRHTriangle(rect.x + rect.w, rect.y, kBACKEND_TITLE_HEIGHT, renderer);

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
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for(uint8_t i = 0; i <= kGRID_WIDTH; i++)
  {
    for(uint8_t j = 0; j <= kGRID_HEIGHT; j++)
    {
      SDL_RenderDrawPoint(renderer, grid_x + i*kTILE_SIZE, grid_y + j*kTILE_SIZE);
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
  if(!frame_main.isImageSet())
    setupMainView(renderer, "Joe's Store");

  int x = 100;
  int y = 100;

  frame_main.render(renderer, x, y);
  img_backend_right.render(renderer, 
                           x + frame_main.getWidth() - kBACKEND_RIGHT_X, 
                           y + frame_main.getHeight() - kBACKEND_RIGHT_Y);
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

}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/