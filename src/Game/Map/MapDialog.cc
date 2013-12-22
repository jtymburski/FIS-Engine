/*******************************************************************************
 * Class Name: MapDialog
 * Date Created: August 7, 2013
 * Inheritance: none
 * Description: The dialog display at the bottom of the map. Offers either
 *              conversation options and display or notifications.
 *
 * TODO:
 *  - Number entry, text entry. Shop mode? Also, built into conversation
 *  - On a very dark background, the black highlight is kind of unnoticeable
 *
 * Want List:
 *  - Text HTML like response to allow for color changes
 *    - <b> - bold
 *    - <u> - underline
 *    - <i> - italic
 *    - <color r g b> - color change
 *    - <id 12> - id to text name reference change
 ******************************************************************************/
#include "Game/Map/MapDialog.h"

/* Constant Implementation - see header file for descriptions */
//const float MapDialog::kBUBBLES_ANIMATE = 277.67;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapDialog::MapDialog(Options* running_config)
{
  uint32_t index = 4000000000;
  std::cout << index << std::endl;
  std::cout << static_cast<int>(index) << std::endl;
  
  /* Reset new variables */
  system_options = NULL;
  
  /* Set class parameters */
  setConfiguration(running_config);
}

/* Destructor function */
MapDialog::~MapDialog()
{
  system_options = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Computes all IDs that are needed for displaying the conversation */
// TODO: This will still need to add the target thing, wherever relevant
std::vector<int> MapDialog::calculateThingList(Conversation convo)
{
  std::vector<int> list;
 
  /* Recursively get all other IDs from embedded conversations */
  for(auto i = convo.next.begin(); i != convo.next.end(); i++)
  {
    std::vector<int> computed_list = calculateThingList(*i);
    list.insert(list.end(), computed_list.begin(), computed_list.end());
  }

  /* Append final ID for the current conversation */
  list.push_back(convo.thing_id);

  return list;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Key Down events handled */
void MapDialog::keyDownEvent(SDL_KeyboardEvent event)
{
  // TODO
}

/* Key Flush events handled */
void MapDialog::keyFlush()
{
  // TODO
}

/* Key Up events handled */
void MapDialog::keyUpEvent(SDL_KeyboardEvent event)
{
  // TODO
}

bool MapDialog::loadImageConversation(std::string path, SDL_Renderer* renderer)
{
  return img_convo.setTexture(path, renderer);
}

bool MapDialog::loadImageNameLeft(std::string path, SDL_Renderer* renderer)
{
  return img_name_l.setTexture(path, renderer);
}

bool MapDialog::loadImageNameRight(std::string path, SDL_Renderer* renderer)
{
  img_name_r.flipHorizontal();
  return img_name_r.setTexture(path, renderer);
}

bool MapDialog::loadImagePickupBottom(std::string path, SDL_Renderer* renderer)
{
  img_pick_b.flipVertical();
  return img_pick_b.setTexture(path, renderer);
}

bool MapDialog::loadImagePickupTop(std::string path, SDL_Renderer* renderer)
{
  return img_pick_t.setTexture(path, renderer);
}

/* Renders the Map Thing */
bool MapDialog::render(SDL_Renderer* renderer)
{
  // TODO
  int convo_x = (1216 - img_convo.getWidth()) / 2;
  int convo_y = 704 - img_convo.getHeight();
  
  img_convo.render(renderer, convo_x, convo_y);
  img_name_l.render(renderer, convo_x + 45, convo_y - img_name_l.getHeight());
  img_name_r.render(renderer, convo_x + 45 + 150 + img_name_r.getWidth(), 
                    convo_y - img_name_r.getHeight());

  /* Draw text box */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect src_rect;
  src_rect.x = convo_x + 45 + img_name_l.getWidth();
  src_rect.y = convo_y - img_name_l.getHeight();
  src_rect.w = 150;
  src_rect.h = 3;
  SDL_RenderFillRect(renderer, &src_rect);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255*0.65);
  src_rect.y += src_rect.h;
  src_rect.h = 42 - src_rect.h;
  SDL_RenderFillRect(renderer, &src_rect);
  
  img_pick_b.render(renderer, 500, 100);
  img_pick_t.render(renderer, 500, 200);
}

/* Sets the running configuration, from the options class */
bool MapDialog::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    return true;
  }
  
  return false;
}

/* Updates the thing, called on the tick */
void MapDialog::update(int cycle_time)
{
  // TODO
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
