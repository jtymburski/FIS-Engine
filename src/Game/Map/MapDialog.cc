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
  /* Set class parameters */
  clearData();
  setConfiguration(running_config);
}

/* Destructor function */
MapDialog::~MapDialog()
{
  deleteFonts();
  clearData();
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

/* -------------------------------------------------------------------------- */
void MapDialog::clearData()
{
  conversation_ready = false;
  conversation_waiting = false;
  dialog_mode = DISABLED;
  dialog_status = OFF;
  event_handler = NULL;
  font_normal = NULL;
  font_title = NULL;
  system_options = NULL;
  target = NULL;
  thing_active = NULL;
}

/* -------------------------------------------------------------------------- */
bool MapDialog::createFonts()
{
  bool success = false;

  if(system_options != NULL)
  {
    /* Try and create the new fonts */
    TTF_Font* regular_font = Text::createFont(system_options->getBasePath() + 
                                              system_options->getFont(), 12);
    TTF_Font* title_font = Text::createFont(system_options->getBasePath() + 
                                            system_options->getFont(), 
                                            20, TTF_STYLE_BOLD);

    /* If successful, insert the new fonts. Otherwise, delete if any were
     * created */
    if(regular_font != NULL && title_font != NULL)
    {
      deleteFonts();
      font_normal = regular_font;
      font_title = title_font;
      success = true;
    }
    else
    {
      TTF_CloseFont(regular_font);
      regular_font = NULL;

      TTF_CloseFont(title_font);
      title_font = NULL;
    }
  }

  return success;
}

/* -------------------------------------------------------------------------- */
void MapDialog::deleteFonts()
{
  TTF_CloseFont(font_normal);
  font_normal = NULL;

  TTF_CloseFont(font_title);
  font_title = NULL;
}

/* -------------------------------------------------------------------------- */
MapThing* MapDialog::getThingReference(int id)
{
  MapThing* thing_reference = NULL;

  for(auto i = thing_data.begin(); i != thing_data.end(); i++)
  {
    if((*i)->getID() == id)
      thing_reference = *i;
  }

  return thing_reference;
}

/* -------------------------------------------------------------------------- */
void MapDialog::setupConversation(SDL_Renderer* renderer)
{
  Frame* dialog_frame = NULL;
  int render_height = img_convo.getHeight();
  int render_width = img_convo.getWidth();

  /* Set the active thing data plus grab image frame */
  thing_active = getThingReference(conversation_info.thing_id);
  if(thing_active != NULL)
  {
    dialog_frame = thing_active->getDialogImage();
    if(dialog_frame->getHeight() > render_height)
      render_height = dialog_frame->getHeight();
    render_width += dialog_frame->getWidth() / 2;
  }

  /* Create the name information */
  std::string name = "";
  Text name_text;
  if(thing_active != NULL)
   name = thing_active->getName();
  if(!name.empty())
  {
    SDL_Color text_color = {255, 255, 255, 255};
    name_text.setFont(font_title);
    name_text.setText(renderer, name, text_color);
    if((img_convo.getHeight() + img_name_l.getHeight()) > render_height)
      render_height = img_convo.getHeight() + img_name_l.getHeight();
  }

  /* Render texture creation and setup */
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, 
                                           render_width, render_height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  /* Render images onto texture */
  int convo_y = render_height - img_convo.getHeight();
  img_convo.render(renderer, 0, convo_y);
  if(dialog_frame != NULL)
    dialog_frame->render(renderer, render_width - dialog_frame->getWidth(), 
                                   render_height - dialog_frame->getHeight());
  if(!name.empty())
  {
    /* Draw corners to name box */
    int textbox_height = img_name_l.getHeight();
    img_name_l.render(renderer, 45, convo_y - textbox_height);
    img_name_r.render(renderer, 
                      45 + img_name_l.getWidth() + name_text.getWidth(), 
                      convo_y - textbox_height);

    /* Draw top white bar encapsulating text */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect src_rect;
    src_rect.x = 45 + img_name_l.getWidth();
    src_rect.y = convo_y - textbox_height;
    src_rect.w = name_text.getWidth();
    src_rect.h = 3;
    SDL_RenderFillRect(renderer, &src_rect);

    /* Draw opaque fill under text */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255*0.65);
    src_rect.y += src_rect.h;
    src_rect.h = textbox_height - src_rect.h;
    SDL_RenderFillRect(renderer, &src_rect);

    /* Draw text */
    name_text.render(renderer, 
                     src_rect.x + (src_rect.w - name_text.getWidth()) / 2, 
                     src_rect.y + (src_rect.h - name_text.getHeight()) / 2);
  } 
  SDL_SetRenderTarget(renderer, NULL);

  frame_convo.setTexture(texture);

  // /* Get and set up all the pertinent thing data */
  // dialog_text_index = 0;
  // int img_width = 0;
  // Frame* thing_frame = 0;
  // if(getThingPtr(conversation_info.thing_id))
  //   thing_frame = thing->getDialogImage();
  // if(thing_frame != 0)
  //   img_width = thing_frame->getImage().width();
  
  // /* Now work on the text length vs. viewable area */
  // display_font.setPointSize(kFONT_SIZE);
  // QFontMetrics normal_font(display_font);
  // int txt_length = dialog_display.getImage().width() - kMARGIN_SIDES
  //                - (img_width >> 1);
  // if((img_width >> 1) < kMARGIN_SIDES)
  //   txt_length -= kMARGIN_SIDES;
  // QString txt_line = conversation_info.text;

  // /* Split up the text as per the visible viewing area */
  // if(conversation_info.next.size() > 1)
  // {
  //   txt_line = normal_font.elidedText(txt_line, Qt::ElideRight, txt_length);

  //   /* Fill the options */
  //   int options_length = txt_length - kOPTION_OFFSET;
  //   options_text.clear();
  //   for(int i = 0; i < conversation_info.next.size(); i++)
  //     options_text.append(normal_font.elidedText(
  //              conversation_info.next[i].text, Qt::ElideRight, options_length));
  // }
  // dialog_text = lineSplitter(txt_line, txt_length, display_font);
  
  // /* Clear the option and index counters */
  // dialog_option = 0;
  // dialog_option_top = 0;
  // dialog_shift_enable = false;
  // dialog_shift_offset = 0;

  // /* Reset the conversation text display */
  // resetConversationTextDisplay();
  
  // /* Only occurs on first time running - initiates animation sequence */
  // if(dialog_mode == DISABLED)
  // {
  //   dialog_mode = CONVERSATION;

  //   /* Initiate animation sequence */
  //   initiateAnimation(display_font);
  // }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

std::vector<int> MapDialog::getConversationIDs()
{
  std::vector<int> thing_ids;

  if(isConversationWaiting())
  {
    thing_ids = calculateThingList(conversation_info);
    thing_ids.push_back(target->getID());
    thing_ids.erase(Helpers::uniqueSplit(thing_ids.begin(), thing_ids.end()), 
                    thing_ids.end());
  }

  return thing_ids;
}

/* -------------------------------------------------------------------------- */
bool MapDialog::initConversation(Conversation* dialog_info, MapPerson* target)
{
  if(dialog_info != NULL)
    return initConversation(*dialog_info, target);
  return false;
}

/* -------------------------------------------------------------------------- */
bool MapDialog::initConversation(Conversation dialog_info, MapPerson* target)
{
  if(dialog_mode != CONVERSATION && target != NULL && isImagesSet())
  {
    conversation_info = dialog_info;
    this->target = target;
    thing_active = NULL;
    thing_data.clear();
    conversation_ready = false;
    conversation_waiting = true;

    return true;
  }
  
  return false;
}

/* -------------------------------------------------------------------------- */
bool MapDialog::isConversationWaiting()
{
  return conversation_waiting;
}

/* -------------------------------------------------------------------------- */
bool MapDialog::isImagesSet(bool conversation, bool pickup)
{
  bool loaded = true;

  /* Check conversation image data */
  if(conversation)
  {
    loaded &= img_convo.isImageSet();
    loaded &= img_name_l.isImageSet();
    loaded &= img_name_r.isImageSet();
  }

  /* Check pickup image data */
  if(pickup)
  {
    loaded &= img_pick_b.isImageSet();
    loaded &= img_pick_t.isImageSet();
  }

  return loaded;
}

/* Key Down events handled */
/* -------------------------------------------------------------------------- */
void MapDialog::keyDownEvent(SDL_KeyboardEvent event)
{
  // TODO
}

/* Key Flush events handled */
/* -------------------------------------------------------------------------- */
void MapDialog::keyFlush()
{
  // TODO
}

/* Key Up events handled */
/* -------------------------------------------------------------------------- */
void MapDialog::keyUpEvent(SDL_KeyboardEvent event)
{
  // TODO
}

/* -------------------------------------------------------------------------- */
bool MapDialog::loadImageConversation(std::string path, SDL_Renderer* renderer)
{
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  return img_convo.setTexture(base_path + path, renderer);
}

/* -------------------------------------------------------------------------- */
bool MapDialog::loadImageNameLeft(std::string path, SDL_Renderer* renderer)
{
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  return img_name_l.setTexture(base_path + path, renderer);
}

/* -------------------------------------------------------------------------- */
bool MapDialog::loadImageNameRight(std::string path, SDL_Renderer* renderer)
{
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  img_name_r.flipHorizontal();
  return img_name_r.setTexture(base_path + path, renderer);
}

/* -------------------------------------------------------------------------- */
bool MapDialog::loadImagePickupBottom(std::string path, SDL_Renderer* renderer)
{
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  img_pick_b.flipVertical();
  return img_pick_b.setTexture(base_path + path, renderer);
}

/* -------------------------------------------------------------------------- */
bool MapDialog::loadImagePickupTop(std::string path, SDL_Renderer* renderer)
{
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  return img_pick_t.setTexture(base_path + path, renderer);
}

/* Renders the Map Thing */
/* -------------------------------------------------------------------------- */
bool MapDialog::render(SDL_Renderer* renderer)
{
  if(conversation_ready)
  {
    setupConversation(renderer);
    conversation_ready = false;
    dialog_mode = CONVERSATION;
  }

  if(dialog_mode == CONVERSATION)
  {
    frame_convo.render(renderer, 
                  (system_options->getScreenWidth() - img_convo.getWidth()) / 2, 
                  system_options->getScreenHeight() - img_convo.getHeight());
  }

  // // TODO
  // int convo_x = (1216 - img_convo.getWidth()) / 2;
  // int convo_y = 704 - img_convo.getHeight();
  
  // img_convo.render(renderer, convo_x, convo_y);
  // img_name_l.render(renderer, convo_x + 45, convo_y - img_name_l.getHeight());
  // img_name_r.render(renderer, convo_x + 45 + 150 + img_name_r.getWidth(), 
  //                   convo_y - img_name_r.getHeight());

  // /* Draw text box */
  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  // SDL_Rect src_rect;
  // src_rect.x = convo_x + 45 + img_name_l.getWidth();
  // src_rect.y = convo_y - img_name_l.getHeight();
  // src_rect.w = 150;
  // src_rect.h = 3;
  // SDL_RenderFillRect(renderer, &src_rect);
  // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255*0.65);
  // src_rect.y += src_rect.h;
  // src_rect.h = 42 - src_rect.h;
  // SDL_RenderFillRect(renderer, &src_rect);
  
  // img_pick_b.render(renderer, 500, 100);
  // img_pick_t.render(renderer, 500, 200);

  return true;
}

/* Sets the running configuration, from the options class */
/* -------------------------------------------------------------------------- */
bool MapDialog::setConfiguration(Options* running_config)
{
  if(running_config != NULL)
  {
    system_options = running_config;
    createFonts();

    return true;
  }
  
  return false;
}

/* -------------------------------------------------------------------------- */
bool MapDialog::setConversationThings(std::vector<MapThing*> things)
{
  if(conversation_waiting)
  {
    thing_data = things;
    conversation_waiting = false;
    conversation_ready = true;
    return true;
  }

  return false;
}

/*
 * Description: Sets the event handler to create and manage all existing events
 *              that get fired throughout interaction with the class. This is 
 *              necessary to ensure that any events work.
 *
 * Inputs: EventHandler* event_handler - the new handler pointer (NULL to unset)
 * Output: none
 */
void MapDialog::setEventHandler(EventHandler* event_handler)
{
  this->event_handler = event_handler;
}

/* Updates the thing, called on the tick */
/* -------------------------------------------------------------------------- */
void MapDialog::update(int cycle_time)
{
  // TODO
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
