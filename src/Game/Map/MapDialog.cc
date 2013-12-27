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
const uint8_t MapDialog::kBORDER_WIDTH = 3;
const float MapDialog::kBUBBLES_ANIMATE = 277.67;
const uint8_t MapDialog::kBUBBLES_COUNT = 3;
const uint8_t MapDialog::kBUBBLES_OFFSET = 16;
const uint8_t MapDialog::kBUBBLES_SPACING = 7;
const uint16_t MapDialog::kCURSOR_ANIMATE = 300;
const uint8_t MapDialog::kCURSOR_HEIGHT = 8;
const uint8_t MapDialog::kHIGHLIGHT_MARGIN = 5;
const uint8_t MapDialog::kLINE_SPACING = 12;
const uint8_t MapDialog::kMARGIN_SIDES = 50;
const uint8_t MapDialog::kMARGIN_TOP = 50;
const uint8_t MapDialog::kNAME_BOX_OFFSET = 45;
const float MapDialog::kOPACITY_BACKEND = 0.65;
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
void MapDialog::clearConversation(Conversation* convo)
{
  if(convo != NULL)
  {
    /* Recursively get all other IDs from embedded conversations */
    for(auto i = convo->next.begin(); i != convo->next.end(); i++)
      clearConversation(&(*i));
    convo->text = "";
    convo->thing_id = -1;
    convo->next.clear();
  }
}

/* -------------------------------------------------------------------------- */
void MapDialog::clearData()
{
  animation_cursor = 0.0;
  animation_cursor_up = true;
  animation_shifter = 0.0;
  clearConversation(&conversation_info);
  conversation_ready = false;
  conversation_update = false;
  conversation_waiting = false;
  dialog_mode = DISABLED;
  dialog_status = OFF;
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
  text_index_max = 0;
  text_lines.clear();
  text_options.clear();
  text_strings.clear();
  text_top = 0;
  text_update = false;
  thing_active = NULL;
  thing_data.clear();
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
void MapDialog::executeEvent()
{
  if(event_handler != NULL && 
     conversation_info.action_event.classification != EventClassifier::NOEVENT)
  {
    event_handler->executeEvent(conversation_info.action_event, target);
  }
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

/* -------------------------------------------------------------------------- */
void MapDialog::setConversation(Conversation* new_convo)
{
  /* Clear the old conversation */
  clearConversation(&conversation_info);
  conversation_info.text = "";
  conversation_info.thing_id = -1;
  conversation_info.category = DialogCategory::TEXT;
  if(event_handler != NULL)
    conversation_info.action_event = event_handler->createBlankEvent();

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
  
  /* Handle name rendering, if it exists */
  if(!name.empty())
  {
    /* Draw corners to name box */
    int textbox_height = img_name_l.getHeight();
    img_name_l.render(renderer, kNAME_BOX_OFFSET, convo_y - textbox_height);
    img_name_r.render(renderer, 
                kNAME_BOX_OFFSET + img_name_l.getWidth() + name_text.getWidth(), 
                convo_y - textbox_height);

    /* Draw top white bar encapsulating text */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect src_rect;
    src_rect.x = kNAME_BOX_OFFSET + img_name_l.getWidth();
    src_rect.y = convo_y - textbox_height;
    src_rect.w = name_text.getWidth();
    src_rect.h = kBORDER_WIDTH;
    SDL_RenderFillRect(renderer, &src_rect);

    /* Draw opaque fill under text */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 * kOPACITY_BACKEND);
    src_rect.y += src_rect.h;
    src_rect.h = textbox_height - src_rect.h;
    SDL_RenderFillRect(renderer, &src_rect);

    /* Draw text */
    name_text.render(renderer, 
                     src_rect.x + (src_rect.w - name_text.getWidth()) / 2, 
                     src_rect.y + (src_rect.h - name_text.getHeight()) / 2);
  } 
  SDL_SetRenderTarget(renderer, NULL);

  /* Create the base frame display texture */
  frame_convo.setTexture(texture);

  /* Determine the length of the viewing area and split text lines */
  std::string txt_line = conversation_info.text;
  if(font_normal != NULL)
  {
    if(conversation_info.next.size() > 1)
    {
      text_strings = Text::splitLine(font_normal, txt_line, txt_length, true);

      /* Fill the options */
      int options_length = txt_length - kOPTION_OFFSET;
      std::vector<std::string> options_text;

      for(auto i = conversation_info.next.begin(); 
               i != conversation_info.next.end(); i++)
      {
        options_text.push_back(
           Text::splitLine(font_normal, (*i).text, options_length, true).at(0));
      }
      renderOptions(renderer, options_text);
    }
    else
    {
      text_strings = Text::splitLine(font_normal, txt_line, txt_length);
      renderOptions(NULL);
    }

    setupRenderText(text_strings, true);
  }

  /* Modify the offset if it's above the new limits */
  if(dialog_offset > 0.0)
    dialog_offset = frame_convo.getHeight();
  dialog_option = 0;
  dialog_option_top = 0;

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
      /* If the letters still need to be displayed, finish */
      if(text_index < text_index_max)
      {
        text_index = text_index_max;
        text_update = true;
      }
      /* Otherwise, if end of conversation has been reached, start to hide it */
      else if(conversation_info.next.size() == 0)
      {
        dialog_status = HIDING;
      }
      /* Otherwise, there is a next conversation. Proceed */
      else
      {
        bool multiple = (conversation_info.next.size() > 1);
        
        /* Do the initial conversation shift */
        executeEvent();
        Conversation new_convo = conversation_info.next[dialog_option];
        setConversation(&new_convo);
        
        /* If multiple options, shift to the next one */
        if(multiple)
        {
          executeEvent();
          if(conversation_info.next.size() == 0)
            dialog_status = HIDING;
          else
          {
            Conversation new_convo2 = conversation_info.next[0];
            setConversation(&new_convo2);  
          }
        }
        
        conversation_update = true;
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
bool MapDialog::loadImageDialogShifts(std::string path_next, 
                                      std::string path_more, 
                                      SDL_Renderer* renderer)
{
  bool success = true;
  
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();
    
  /* Set all the frame information */
  success &= img_convo_m.setTexture(base_path + path_more, renderer);
  success &= img_convo_n.setTexture(base_path + path_next, renderer);
  
  return success;
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
bool MapDialog::loadImageOptions(std::string path_circle, 
                                 std::string path_triangle, 
                                 SDL_Renderer* renderer)
{
  bool success = true;
  
  std::string base_path = "";
  if(system_options != NULL)
    base_path = system_options->getBasePath();
    
  /* Set all the frame information */
  success &= img_opt_c.setTexture(base_path + path_circle, renderer);
  success &= img_opt_u.setTexture(base_path + path_triangle, renderer);
  img_opt_d.flipVertical();
  success &= img_opt_d.setTexture(base_path + path_triangle, renderer);
  
  return success;
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
    x_index += kMARGIN_SIDES;
    y_index = system_options->getScreenHeight() - img_convo.getHeight() 
                                                + kMARGIN_TOP;
    if(dialog_status == ON)
    {
      /* Render the applicable text lines */
      for(uint16_t i = text_top; i < (text_top + kTEXT_LINES); i++)
      {
        if(i < text_lines.size())
        {
          text_lines[i]->render(renderer, x_index, y_index);
          y_index += text_lines[i]->getHeight() + kLINE_SPACING;
        }
      }

      /* Render the options, if applicable */
      if(conversation_info.next.size() > 1 && text_index >= text_index_max)
      {
        uint8_t index = dialog_option_top;
        y_index += kLINE_SPACING;

        /* Loop through and render them all, properly offset */
        while(index < text_options.size() && 
              index < (dialog_option_top + kTEXT_OPTIONS))
        {
          /* If this option is the active one, render a highlight box under */
          if(index == dialog_option)
          {
            uint8_t m = kHIGHLIGHT_MARGIN;
            SDL_Rect highlight_rect = {0, 0, 0, 0};
            highlight_rect.x = x_index + kOPTION_OFFSET - m;
            highlight_rect.y = y_index - m;
            highlight_rect.w = text_options[index]->getWidth() + (m << 1);
            highlight_rect.h = text_options[index]->getHeight() + (m << 1);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 45);
            SDL_RenderFillRect(renderer, &highlight_rect);
          }
          
          /* Determine if there is more than the display options */
          if(conversation_info.next.size() > kTEXT_OPTIONS)
          {
            Frame* frame = NULL;
            
            /* If it's the top option, render beside */
            if(index == dialog_option_top)
            {
              if(dialog_option_top == 0)
                frame = &img_opt_c;
              else
                frame = &img_opt_u;
            }
            
            /* If it's the bottom option, render beside */
            if(index == (dialog_option_top + kTEXT_OPTIONS - 1))
            {
              if(index == (conversation_info.next.size() - 1))
                frame = &img_opt_c;
              else
                frame = &img_opt_d;
            }
            
            /* Render the frame if non-null and set */
            if(frame != NULL)
              frame->render(renderer, x_index + (kOPTION_OFFSET 
                                              - frame->getWidth() - 1) / 2, 
                            y_index + (text_options[index]->getHeight() 
                                    - frame->getHeight()) / 2);
          }
          
          /* Finally, render the option text and increment index */
          text_options[index]->render(renderer, 
                             x_index + kOPTION_OFFSET, y_index);
          y_index += text_options[index]->getHeight() + (kLINE_SPACING << 1);
          index++;
        }
      }
      /* Otherwise, check the parameters for displaying the dialog shifters */
      else if(text_index >= text_index_max)
      {
        x_index += (img_convo.getWidth() / 2 - kMARGIN_SIDES);
        
        /* If the display is not done, show the shifting more pointers */
        uint16_t top_index = text_top + kTEXT_LINES;
        if(top_index < text_strings.size())
        {
          uint16_t offset = kBUBBLES_OFFSET;
          
          for(uint8_t i = kBUBBLES_COUNT; i > 0; i--)
          {
            if(animation_shifter >= i)
              img_convo_m.render(renderer, x_index - img_convo_m.getWidth() / 2, 
                                 system_options->getScreenHeight() - offset);
            offset += kBUBBLES_SPACING;
          }
        }
        /* Else, the words have ended. Show the next shifter arrow */
        else
        {
          img_convo_n.render(renderer, x_index - img_convo_n.getWidth() / 2, 
                             system_options->getScreenHeight() 
                                  - img_convo_n.getHeight() 
                                  - static_cast<int>(animation_cursor));
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
        if(text_index < text_index_max) // else
        {
          text_index += cycle_time / kTEXT_DISPLAY_SPEED;
          if(text_index > text_index_max)
            text_index = text_index_max;
          text_update = true;
        }

        /* Up motion */
        if(animation_cursor_up)
        {
          animation_cursor += 1.0 * cycle_time * kCURSOR_HEIGHT 
                                               / kCURSOR_ANIMATE;
          if(animation_cursor >= kCURSOR_HEIGHT)
          {
            animation_cursor = kCURSOR_HEIGHT;
            animation_cursor_up = false;
          }
        }
        /* Down motion */
        else
        {
          animation_cursor -= 1.0 * cycle_time * kCURSOR_HEIGHT 
                                               / kCURSOR_ANIMATE;
          if(animation_cursor <= 0)
          {
            animation_cursor = 0;
            animation_cursor_up = true;
          }
        }

        /* The animating shifter, for text that is too long to fit in the 
         * viewing window */
        animation_shifter += cycle_time / kBUBBLES_ANIMATE;
        if(animation_shifter >= (kBUBBLES_COUNT + 1))
          animation_shifter = 0;
      }
    }
  }
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
