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
const uint8_t MapDialog::kLINE_SPACING = 8;
const uint8_t MapDialog::kMARGIN_SIDES = 50;
const uint8_t MapDialog::kMARGIN_TOP = 50;
const uint8_t MapDialog::kOPTION_OFFSET = 50;
const float MapDialog::kSHIFT_TIME = 3.704;
const uint8_t MapDialog::kTEXT_LINES = 4;
const uint8_t MapDialog::kTEXT_OPTIONS = 3;
const float MapDialog::kTEXT_DISPLAY_SPEED = 33.33;

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
  renderOptions(NULL);
  setupRenderText();
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
  conversation_update = false;
  conversation_waiting = false;
  dialog_mode = DISABLED;
  dialog_offset = 0.0;
  dialog_option = 0;
  dialog_option_top = 0;
  dialog_status = OFF;
  event_handler = NULL;
  font_normal = NULL;
  font_title = NULL;
  system_options = NULL;
  target = NULL;
  text_index = 0.0;
  text_top = 0;
  text_update = false;
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
                                              system_options->getFont(), 18);
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

void MapDialog::renderOptions(SDL_Renderer* renderer, 
                              std::vector<std::string> options)
{
  /* Clear the options stack */
  for(auto i = text_options.begin(); i != text_options.end(); i++)
    delete *i;
  text_options.clear();

  /* Render and create new options */
  for(auto i = options.begin(); i != options.end(); i++)
  {
    Text* t = new Text(font_normal);
    t->setFont(font_normal);
    t->setText(renderer, *i, {255,255,255,255});
    text_options.push_back(t);
  }
}

void MapDialog::setConversation(Conversation* new_convo)
{
  /* Clear the old conversation */
  conversation_info.text = "";
  conversation_info.thing_id = -1;
  conversation_info.category = DialogCategory::TEXT;
  if(event_handler != NULL)
    conversation_info.action_event = event_handler->createBlankEvent();
  conversation_info.next.clear();

  /* Insert new conversation */
  if(new_convo != NULL)
  {
    conversation_info.text = new_convo->text;
    conversation_info.thing_id = new_convo->thing_id;
    conversation_info.category = new_convo->category;
    conversation_info.action_event = new_convo->action_event;
    conversation_info.next = new_convo->next;
  }
}

/* -------------------------------------------------------------------------- */
void MapDialog::setupConversation(SDL_Renderer* renderer)
{
  Frame* dialog_frame = NULL;
  int render_height = img_convo.getHeight();
  int render_width = img_convo.getWidth();
  int txt_length = img_convo.getWidth() - (kMARGIN_SIDES << 1);
//std::cout << "A" << std::endl;
  /* Set the active thing data plus grab image frame */
  thing_active = getThingReference(conversation_info.thing_id);
  if(thing_active != NULL)
  {
    dialog_frame = thing_active->getDialogImage();

    /* Modify render height/width for new frame */
    if(dialog_frame->getHeight() > render_height)
      render_height = dialog_frame->getHeight();
    render_width += dialog_frame->getWidth() / 2;

    /* Modify text length limiters */
    int width_modifier = (dialog_frame->getWidth() >> 1);
    if(width_modifier > kMARGIN_SIDES)
      txt_length -= (width_modifier - kMARGIN_SIDES);
  }
//std::cout << "B" << std::endl;
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
//std::cout << "C" << std::endl;
  /* Render texture creation and setup */
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, 
                                           render_width, render_height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
//std::cout << "D" << std::endl;
  /* Render images onto texture */
  int convo_y = render_height - img_convo.getHeight();
  img_convo.render(renderer, 0, convo_y);
  if(dialog_frame != NULL)
    dialog_frame->render(renderer, render_width - dialog_frame->getWidth(), 
                                   render_height - dialog_frame->getHeight());
  //std::cout << "E" << std::endl;
  if(!name.empty())
  {
    /* Draw corners to name box */
    int textbox_height = img_name_l.getHeight();
    img_name_l.render(renderer, 45, convo_y - textbox_height);
    img_name_r.render(renderer, 
                      45 + img_name_l.getWidth() + name_text.getWidth(), 
                      convo_y - textbox_height);
//std::cout << "F" << std::endl;
    /* Draw top white bar encapsulating text */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect src_rect;
    src_rect.x = 45 + img_name_l.getWidth();
    src_rect.y = convo_y - textbox_height;
    src_rect.w = name_text.getWidth();
    src_rect.h = 3;
    SDL_RenderFillRect(renderer, &src_rect);
//std::cout << "G" << std::endl;
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
//std::cout << "H" << std::endl;
  /* Create the base frame display texture */
  frame_convo.setTexture(texture);
//std::cout << "I" << std::endl;
  /* Determine the length of the viewing area and split text lines */
  std::string txt_line = conversation_info.text;
  if(font_normal != NULL)
  {
    if(conversation_info.next.size() > 1)
    {
      text_strings = Text::splitLine(font_normal, txt_line, txt_length, true);
      //std::cout << "1A" << std::endl;
      /* Fill the options */
      int options_length = txt_length - kOPTION_OFFSET;
      std::vector<std::string> options_text;
      //std::cout << "1B: " << options_length << std::endl;
      //for(uint16_t i = 0; i < conversation_info.next.size(); i++)
      //  std::cout << "TEST: " << conversation_info.next[i].text << std::endl;

      for(auto i = conversation_info.next.begin(); 
               i != conversation_info.next.end(); i++)
      {
        //std::cout << "INFO: " << (font_normal == NULL) << " " << (*i).text << std::endl;
        std::vector<std::string> str = Text::splitLine(font_normal, (*i).text, options_length, true);
        //std::cout << "LENGTH: " << str.size() << std::endl;
        options_text.push_back(str.at(0));
           //Text::splitLine(font_normal, (*i).text, options_length, true).at(0));
      }
      //std::cout << "1" << std::endl;
      renderOptions(renderer, options_text);
      //std::cout << "2" << std::endl;
    }
    else
    {
      text_strings = Text::splitLine(font_normal, txt_line, txt_length);
      renderOptions(NULL);
    }

    setupRenderText(text_strings, true);
  }
//std::cout << "J" << std::endl;
  /* Modify the offset if it's above the new limits */
  if(dialog_offset > 0.0)
    dialog_offset = frame_convo.getHeight();
  dialog_option = 0;
  dialog_option_top = 0;
//std::cout << "K" << std::endl;
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

/* -------------------------------------------------------------------------- */
void MapDialog::setupRenderText(std::vector<std::string> lines, bool delete_old)
{
  /* Delete if line count of 0 is passed in */
  if(delete_old)
  {
    for(auto i = text_lines.begin(); i != text_lines.end(); i++)
      delete *i;
    text_lines.clear();

    /* Create a new stack of the applicable text handlers */
    for(uint16_t i = 0; i < lines.size(); i++)
      text_lines.push_back(new Text(font_normal));
    text_index = 0.0;
    text_index_max = 0;
    text_top = 0;
  }

  /* Set the initial max character length */
  for(uint16_t i = text_top; i < (text_top + kTEXT_LINES); i++)
  {
    if(i < lines.size())
      text_index_max += lines[i].size();
  }
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
    setConversation(&dialog_info);
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
bool MapDialog::isConversationActive()
{
  return (dialog_mode == CONVERSATION);
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
  if(event.keysym.sym == SDLK_SPACE && event.repeat == 0)
  {
    if(isConversationActive() && dialog_status == ON)
    {
      if(text_index < text_index_max)
      {
        text_index = text_index_max;
        text_update = true;
      }
      else if(conversation_info.next.size() > 0)
      {
        conversation_info = conversation_info.next[dialog_option];
        conversation_update = true;
      }
      else
      {
        dialog_status = HIDING;
      }
    }
  }
  else if(event.keysym.sym == SDLK_UP && text_index >= text_index_max)
  {
    if(dialog_option > 0)
      dialog_option--;
    if(dialog_option < dialog_option_top)
      dialog_option_top = dialog_option;
  }
  else if(event.keysym.sym == SDLK_DOWN && text_index >= text_index_max)
  {
    if(++dialog_option >= conversation_info.next.size())
      dialog_option = conversation_info.next.size() - 1;
    if(dialog_option >= (dialog_option_top + kTEXT_OPTIONS))
      dialog_option_top = dialog_option - kTEXT_OPTIONS + 1;
  }
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
bool MapDialog::loadImageNameLeftRight(std::string path, SDL_Renderer* renderer)
{
  bool success = true;

  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  img_name_r.flipHorizontal();
  success &= img_name_r.setTexture(base_path + path, renderer);
  return (success & img_name_l.setTexture(base_path + path, renderer));
}

/* -------------------------------------------------------------------------- */
bool MapDialog::loadImagePickupTopBottom(std::string path, 
                                         SDL_Renderer* renderer)
{
  bool success = true;

  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();

  img_pick_b.flipVertical();
  success &= img_pick_b.setTexture(base_path + path, renderer);
  return (success & img_pick_t.setTexture(base_path + path, renderer));
}

/* Renders the Map Thing */
/* -------------------------------------------------------------------------- */
bool MapDialog::render(SDL_Renderer* renderer)
{
  uint16_t x_index = 0;
  uint16_t y_index = 0;

  /* If the conversation has changed, update the rendering sprites/data */
  if(conversation_ready || conversation_update)
  {
    setupConversation(renderer);

    if(conversation_ready)
    {
      dialog_status = SHOWING;
      dialog_offset = 0.0;
    }
    conversation_ready = false;
    conversation_update = false;
    dialog_mode = CONVERSATION;
  }

  if(dialog_mode == CONVERSATION)
  {
    /* Render the main frame */
    x_index = (system_options->getScreenWidth() - img_convo.getWidth()) / 2;
    frame_convo.render(renderer, x_index, 
                       system_options->getScreenHeight() - dialog_offset);

    /* Compute the new parts of the rendering text */
    if(text_update)
    {
      uint16_t index = text_top;
      uint16_t length = text_index;
      while(index < (text_top + kTEXT_LINES) && index < text_lines.size() 
                                             && length > 0)
      {
        /* If length is >= string size, render whole string. Otherwise, 
         * only render the portion visible */
        if(length >= text_strings[index].size())
        {
          text_lines[index]->setText(renderer, 
                   text_strings[index], {255, 255, 255, 255});
          length -= text_strings[index].size();
        }
        else
        {
          text_lines[index]->setText(renderer, 
                   text_strings[index].substr(0, length), {255, 255, 255, 255});
          length = 0;
        }
        index++;
      }
      text_update = false;
    }

    /* Compute y base index and proceed to render text */
    y_index = system_options->getScreenHeight() - img_convo.getHeight() 
                                                + kMARGIN_TOP;
    if(dialog_status == ON)
    {
      /* Render the applicable text lines */
      for(uint16_t i = text_top; i < (text_top + kTEXT_LINES); i++)
      {
        if(i < text_lines.size())
        {
          text_lines[i]->render(renderer, x_index + kMARGIN_SIDES, y_index);
          y_index += text_lines[i]->getHeight() + kLINE_SPACING;
        }
      }

      /* Render the options, if applicable */
      if(conversation_info.next.size() > 1 && text_index >= text_index_max)
      {
        uint16_t index = dialog_option_top;
        y_index += (kLINE_SPACING << 1);

        /* Loop through and render them all, properly offset */
        while(index < text_options.size() && 
              index < (dialog_option_top + kTEXT_OPTIONS))
        {
          text_options[index]->render(renderer, 
                             x_index + kMARGIN_SIDES + kOPTION_OFFSET, y_index);
          y_index += text_options[index]->getHeight() + (kLINE_SPACING << 1);

          index++;
        }
      }
    }
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
  /* Ignore all updating if paused */
  if(true)//!paused)
  {
    /* If hiding, shift the display onto the screen */
    if(dialog_status == HIDING)
    {
      dialog_offset -= cycle_time / kSHIFT_TIME;
      if(dialog_offset <= 0.0)
      {
        dialog_status = OFF;
        dialog_offset = 0.0;
        
        if(dialog_mode == CONVERSATION)
        {
          renderOptions(NULL);
          setupRenderText();
          setConversation();
        }
        // /* If notification, remove the first ptr */
        // if(dialog_mode == NOTIFICATION)
        // {
        //   dialog_mode = DISABLED;

        //   if(conversation_waiting)
        //   {
        //     if((notification_queue[0].time_visible >> 1) > dialog_time)
        //       notification_queue[0].time_visible = 
        //                              (notification_queue[0].time_visible >> 1);
        //     setupConversation();
        //     conversation_waiting = false;
        //   }
        //   else
        //   {
        //     notification_queue.removeFirst();
        //   }
        // }
        // else
        // {
          dialog_mode = DISABLED;
        // }

        /* Set up the next notification, if there is one in the queue */
        // initiateNextNotification();
      }
    }
    /* If showing, shift the display onto the screen */
    else if(dialog_status == SHOWING)
    {
      dialog_offset += cycle_time / kSHIFT_TIME; // ~4.5 @ 16.666 ms
      if(dialog_offset >= frame_convo.getHeight())
      {
        dialog_status = ON;
        dialog_offset = frame_convo.getHeight();
      }
    }
    /* If ON, do action appropriate to the dialog mode */
    else if(dialog_status == ON)
    {
      /* This controls how long the notification box is displayed */
      // if(dialog_mode == NOTIFICATION)
      // {
      //   dialog_time -= cycle_time;
      //   if(dialog_time < 0)
      //     dialog_status = HIDING;
      // }
      /* This controls the bouncing cursor at the bottom of each conversational
       * box to signal going to the next set */
      if(isConversationActive())
      {
        /* Increment the dialog letters display */
        // if(dialog_shift_enable)
        // {
        //   dialog_shift_offset += cycle_time / kSHIFT_OFFSET;
        //   if(dialog_shift_offset >= dialog_shift_max)
        //     dialogShiftEnable(false);
        // }
        if(text_index < text_index_max)
        {
          text_index += cycle_time / kTEXT_DISPLAY_SPEED;
          if(text_index > text_index_max)
            text_index = text_index_max;
          text_update = true;
        }

        // /* Up motion */
        // if(animation_cursor_up)
        // {
        //   animation_cursor += cycle_time * kCURSOR_NEXT_SIZE 
        //                                  / kCURSOR_NEXT_TIME;
        //   if(animation_cursor >= kCURSOR_NEXT_SIZE)
        //   {
        //     animation_cursor = kCURSOR_NEXT_SIZE;
        //     animation_cursor_up = false;
        //   }
        // }
        // /* Down motion */
        // else
        // {
        //   animation_cursor -= cycle_time * kCURSOR_NEXT_SIZE 
        //                                  / kCURSOR_NEXT_TIME;
        //   if(animation_cursor <= 0)
        //   {
        //     animation_cursor = 0;
        //     animation_cursor_up = true;
        //   }
        // }

        /* The animating shifter, for text that is too long to fit in the 
         * viewing window */
        // animation_shifter += cycle_time / kBUBBLES_ANIMATE;
        // if(animation_shifter >= (kBUBBLES_COUNT + 1))
        //   animation_shifter = 0;
      }
    }
  }
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
