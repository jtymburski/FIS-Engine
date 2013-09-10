/******************************************************************************
* Class Name: MapDialog
* Date Created: August 7, 2013
* Inheritance: none
* Description: The dialog display at the bottom of the map. Offers either
*              conversation options and display or notifications.
******************************************************************************/
#include "Game/Map/MapDialog.h"

/* Constant Implementation - see header file for descriptions */
const short MapDialog::kCURSOR_NEXT_SIZE = 8;
const short MapDialog::kCURSOR_NEXT_TIME = 300;
const short MapDialog::kFONT_SIZE = 12;
const short MapDialog::kFONT_SPACING = 10;
const short MapDialog::kMARGIN_SIDES = 50;
const short MapDialog::kMARGIN_TOP = 25;
const short MapDialog::kMAX_LINES = 4;
const short MapDialog::kMAX_OPTIONS = 3;
const short MapDialog::kMIN_DISPLAY_TIME = 1000;
const short MapDialog::kMSEC_PER_WORD = 333;
const short MapDialog::kNAME_BOX_ANGLE_X = 42;
const short MapDialog::kNAME_BOX_HEIGHT = 42;
const short MapDialog::kNAME_BOX_MIN_WIDTH = 50;
const short MapDialog::kNAME_BOX_X_OFFSET = 45;
const short MapDialog::kOPTION_MARGIN = 5;
const short MapDialog::kOPTION_OFFSET = 50;
const float MapDialog::kPAUSE_OPACITY_DIFF = 0.03;
const float MapDialog::kPAUSE_OPACITY_MAX = 1.0;
const short MapDialog::kPICKUP_ANGLE_X = 15;
const short MapDialog::kPICKUP_ANGLE_Y = 15;
const short MapDialog::kPICKUP_DEFAULT_TIME = 5000;
const short MapDialog::kPICKUP_HEIGHT = 104;
const short MapDialog::kPICKUP_MARGIN = 20;
const short MapDialog::kPICKUP_TEXT_SPACE = 10;
const short MapDialog::kPICKUP_Y = 50;
const short MapDialog::kSCROLL_CIRCLE_RADIUS = 3;
const short MapDialog::kSCROLL_OFFSET = 6;
const short MapDialog::kSCROLL_TRIANGLE_HEIGHT = 4;
const short MapDialog::kSHIFT_TIME = 75;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapDialog::MapDialog(QFont font)
{ 
  /* Clear some of the initial parameters */
  animation_cursor = 0;
  animation_cursor_up = true;
  animation_offset = 0;
  dialog_mode = DISABLED;
  dialog_option = 0;
  dialog_option_top = 0;
  dialog_status = OFF;
  dialog_text_index = 0;
  dialog_time = 0;
  paused = false;
  paused_opacity = kPAUSE_OPACITY_MAX;
  thing = 0;
 
  /* Conversation dialog initialization */
  conversation_waiting = false;

  /* Pickup Notification parameters cleared */
  pickup_offset = 0;
  pickup_status = OFF;
  pickup_time = 0;
  pickup_width = 0;
  
  setFont(font);
}

/* Destructor function */
MapDialog::~MapDialog()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Calculates a complete list of thing IDs that are used in the given 
 * conversation */
QList<int> MapDialog::calculateThingList(Conversation conversation)
{
  QList<int> list;

  for(int i = 0; i < conversation.next.size(); i++)
    list.append(calculateThingList(conversation.next[i]));
  list.append(conversation.thing_id);

  return list;
}

bool MapDialog::getThingPtr(int id)
{
  for(int i = 0; i < thing_data.size(); i++)
  {
    if(thing_data[i]->getID() == id)
    {
      thing = thing_data[i];
      return true;
    }
  }
  
  thing = 0;
  return false;
}

/* Halts the dialog, if it's being shown or showing */
void MapDialog::initiateAnimation(QFont display_font)
{
  animation_cursor = 0;
  animation_cursor_up = true;
  animation_offset = 0;
  dialog_status = SHOWING;
  QFontMetrics font_info(display_font);
  
  if(dialog_mode == CONVERSATION)
  {
    animation_height = dialog_display.getImage().height() + kNAME_BOX_HEIGHT;
  }
  else if(dialog_mode == NOTIFICATION)
  {
    animation_height = (dialog_text.size() - 1)*kFONT_SPACING + 
                       dialog_text.size() * font_info.height() +
                       kFONT_SPACING + kMARGIN_TOP;
  }
}

bool MapDialog::initiateNextNotification()
{
  if(notification_queue.size() > 0 && pickup_status == OFF 
                                   && dialog_mode != NOTIFICATION)
  {
    display_font.setPointSize(12);
    bool done = false;
    short index = 0;

    while(!done && index < notification_queue.size())
    {
      Notification display_info = notification_queue[index];
      
      /* Categorize the notification, as pickup notification */
      if(display_info.thing_image != 0)
      {
        QFontMetrics font_details(display_font);
        QString multiplier = "x " + QString::number(display_info.thing_count);
        
        /* Start the sequence */
        pickup_status = SHOWING;
        
        /* Set up animation */
        pickup_offset = 0;
        pickup_width = (kPICKUP_MARGIN << 1) 
                     + display_info.thing_image->getImage().width() 
                     + kPICKUP_TEXT_SPACE 
                     + font_details.boundingRect(multiplier).width();
        
        /* Set up time shown */
        pickup_time = display_info.time_visible;

        done = true;
      }
      /* Otherwise, it's a text notification */
      else if(dialog_mode == DISABLED)
      {
        dialog_mode = NOTIFICATION;
    
        /* Calculate the available space */
        int width = dialog_display.getImage().width() - (kMARGIN_SIDES << 1);
        if(width < 0)
          width = 0;
    
        /* Split the line and set the time to display*/
        dialog_text = lineSplitter(display_info.text, width, display_font);
        dialog_time = display_info.time_visible;
        if(dialog_time < kMIN_DISPLAY_TIME)
          dialog_time = kMIN_DISPLAY_TIME;

        /* Initiate animation sequence */
        initiateAnimation(display_font);
    
        done = true;
      }
      
      /* Shift the index to the front if done and index is not 0 */
      if(done && index > 0)
        notification_queue.move(index, 0);

      index++;
    }
    
    if(done)
      return true;
  }
  
  return false;
}

/* Removes duplicates from the given qlist and returns it. Used in
 * conjunction with "calculateThingList" */
QList<int> MapDialog::removeDuplicates(QList<int> duplicate_list)
{
  return duplicate_list.toSet().toList();
}

void MapDialog::setupConversation()
{
  /* Only operated on first running - gets thing data */
  if(dialog_mode == DISABLED)
  {
    /* Acquire all the needed information about things in the conversation */
    emit setThingData(removeDuplicates(calculateThingList(conversation_info)));
  }

  /* Get and set up all the pertinent thing data */
  int img_width = 0;
  Frame* thing_frame = 0;
  if(getThingPtr(conversation_info.thing_id))
    thing_frame = thing->getDialogImage();
  if(thing_frame != 0)
    img_width = thing_frame->getImage().width();
  
  /* Now work on the text length vs. viewable area */
  display_font.setPointSize(kFONT_SIZE);
  QFontMetrics normal_font(display_font);
  int txt_length = dialog_display.getImage().width() - kMARGIN_SIDES
                 - (img_width >> 1);
  if((img_width >> 1) < kMARGIN_SIDES)
    txt_length -= kMARGIN_SIDES;
  QString txt_line = conversation_info.text;

  /* Split up the text as per the visible viewing area */
  if(conversation_info.next.size() > 1)
  {
    txt_line = normal_font.elidedText(txt_line, Qt::ElideRight, txt_length);

    /* Fill the options */
    int options_length = txt_length - kOPTION_OFFSET;
    options_text.clear();
    for(int i = 0; i < conversation_info.next.size(); i++)
      options_text.append(normal_font.elidedText(
               conversation_info.next[i].text, Qt::ElideRight, options_length));
  }
  dialog_text = lineSplitter(txt_line, txt_length, display_font);
  
  /* Clear the option pointers */
  dialog_option = 0;
  dialog_option_top = 0;

  /* Only occurs on first time running - initiates animation sequence */
  if(dialog_mode == DISABLED)
  {
    dialog_mode = CONVERSATION;

    /* Initiate animation sequence */
    initiateAnimation(display_font);
  }
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Sets if the class control is paused */
void MapDialog::setPaused(bool paused)
{
  this->paused = paused;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

bool MapDialog::initConversation(Conversation dialog_info)
{
  if(isDialogImageSet() && dialog_mode != CONVERSATION)
  {
    conversation_info = dialog_info;

    if(dialog_mode == DISABLED)
    {
      setupConversation();
    }
    else if(dialog_mode == NOTIFICATION)
    {
      conversation_waiting = true;
      dialog_status = HIDING;
    }

    return true;
  }
  
  return false;
}

bool MapDialog::initNotification(QString notification, int time_visible, 
                                 bool single_line)
{
  if(isDialogImageSet() && !notification.isEmpty())
  {
    display_font.setPointSize(12);
    QFontMetrics font_info(display_font);
    
    /* Calculate the available space */
    int width = dialog_display.getImage().width() - (kMARGIN_SIDES << 1);
    if(width < 0)
      width = 0;
    
    /* Chop to single line if required */
    if(single_line)
      notification = font_info.elidedText(notification, Qt::ElideRight, width);
          
    /* Calculate the display time if invalid */
    if(time_visible <= 0)
      time_visible = ((notification.count(" ") + 1) * kMSEC_PER_WORD) 
                      + kMSEC_PER_WORD;

    /* Set up the queue entry */
    Notification queue_entry;
    queue_entry.text = notification;
    queue_entry.thing_image = 0;
    queue_entry.thing_count = 0;
    queue_entry.time_visible = time_visible;
    
    /* Append to running queue */
    notification_queue.append(queue_entry);

    /* Setup the next notification, if possible */
    initiateNextNotification();
    
    return true;
  }
  
  return false;
}

bool MapDialog::initPickup(Frame* thing_image, int thing_count, 
                           int time_visible)
{
  if(thing_image != 0 && !thing_image->getImage().isNull() && thing_count > 0)
  {
    /* Set up the queue entry */
    Notification queue_entry;
    queue_entry.text = "";
    queue_entry.thing_image = thing_image;
    queue_entry.thing_count = thing_count;
    
    if(time_visible <= 0)
      queue_entry.time_visible = kPICKUP_DEFAULT_TIME;
    else
      queue_entry.time_visible = time_visible;
    
    /* Insert into the running queue - ahead of messages */
    if(notification_queue.size() == 0 || 
       notification_queue.last().thing_image != 0)
      notification_queue.append(queue_entry);
    else
    {
      int index = 0;
      if(dialog_mode == NOTIFICATION || pickup_status != OFF)
        index = 1;
      while(index < notification_queue.size() && 
            notification_queue[index].thing_image != 0)
        index++;
      notification_queue.insert(index, queue_entry);
    }

    /* Setup the next notification, if possible */
    initiateNextNotification();
    
    return true;
  }
  
  return false;
}

bool MapDialog::isDialogImageSet()
{
  return dialog_display.isImageSet();
}
  
bool MapDialog::isDialogInUse()
{
  return (dialog_mode != DISABLED);
}

bool MapDialog::isInConversation()
{
  return (dialog_mode == CONVERSATION);
}

bool MapDialog::isPickupInUse()
{
  return (pickup_status != OFF);
}

bool MapDialog::isPaused()
{
  return paused;
}

void MapDialog::keyPress(QKeyEvent* event)
{
  /* Only proceed if the dialog isn't paused */
  if(!paused && paused_opacity == kPAUSE_OPACITY_MAX)
  {
    /* Selection case for the conversational dialog option */
    if(event->key() == Qt::Key_Space && !event->isAutoRepeat())
    {
      if(isInConversation() && dialog_status == ON)
      {
        /* Check for dialog status and shift to next conversation */
        if((dialog_text_index + kMAX_LINES) < dialog_text.size())
        {
          dialog_text_index += kMAX_LINES;
        }
        else if(conversation_info.next.size() == 0)
        {
          dialog_status = HIDING;
          emit finishThingTarget();
        }
        else
        {
          bool multiple = (conversation_info.next.size() > 1);
          conversation_info = conversation_info.next[dialog_option];
              
          /* Skip the option if it was an option case */
          if(multiple)
            conversation_info = conversation_info.next[0];

          dialog_option = 0;
          dialog_text_index = 0;
          
          /* Finalize the conversation change */
          setupConversation();
        }
      }
    }
    /* Go back an option selector */
    else if(event->key() == Qt::Key_Up)
    {
      dialog_option--;
      if(dialog_option < 0)
        dialog_option = 0;

      if(dialog_option < dialog_option_top)
        dialog_option_top = dialog_option;
    }
    /* Go to next option selector */
    else if(event->key() == Qt::Key_Down)
    {
      dialog_option++;
      if(dialog_option >= conversation_info.next.size())
        dialog_option = conversation_info.next.size() - 1;

      if(dialog_option >= (dialog_option_top + kMAX_OPTIONS))
        dialog_option_top = dialog_option - kMAX_OPTIONS + 1;
    }
  }
}

void MapDialog::keyRelease(QKeyEvent* event)
{
  /* Unused currently */
  (void)event;
}

bool MapDialog::paintGl(QGLWidget* painter)
{
  if(dialog_status != OFF)
  {
    QFontMetrics font_info(display_font);
    int height = dialog_display.getImage().height();
    int width = dialog_display.getImage().width();
    short x1 = (1216 - width) >> 1;
    short y1 = 704 - animation_offset;
    
    /* Slightly offset y1 for conversational name box addition */
    if(isInConversation())
      y1 += kNAME_BOX_HEIGHT;

    /* Paints the conversation dialog */
    dialog_display.paintGl(x1, y1, width, height, paused_opacity);

    if(dialog_mode == NOTIFICATION)
    {
      short x2 = x1;
      short y2 = y1 + kMARGIN_TOP + font_info.ascent();
      glColor4f(1.0, 1.0, 1.0, paused_opacity);
      
      for(int i = 0; i < dialog_text.size(); i++)
      {
        x2 = x1 + ((width - font_info.width(dialog_text[i])) >> 1);
        painter->renderText(x2, y2, dialog_text[i], display_font);
        y2 += font_info.descent() + kFONT_SPACING + font_info.ascent();
      }
    }
    else if(isInConversation())
    {
      /* Bold font info */
      display_font.setPointSize(14);
      display_font.setBold(true);
      QFontMetrics bold_font(display_font);
      
      /* Thing data, for the conversation */
      Frame* thing_frame = 0;
      QString thing_name = "";
      if(thing != 0)
      {
        thing_frame = thing->getDialogImage();
        thing_name = thing->getName();
      }

      /* Calculate the width and height of the name box */
      int name_height = kNAME_BOX_HEIGHT;
      int name_x = x1 + kNAME_BOX_X_OFFSET;
      int name_width = bold_font.width(thing_name);
      if(name_width < kNAME_BOX_MIN_WIDTH)
        name_width = kNAME_BOX_MIN_WIDTH;

      if(thing_name.size() > 0)
      {
        /* Draw text display background */
        glColor4f(0.0, 0.0, 0.0, 0.65 * paused_opacity);
        glBegin(GL_QUADS);
          glVertex2f(name_x, y1);
          glVertex2f(name_x + (kNAME_BOX_ANGLE_X << 1) + name_width, y1);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X + name_width, y1 - name_height);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X, y1 - name_height);
        glEnd();

        /* Draw outline */
        glColor4f(1.0, 1.0, 1.0, paused_opacity);
        glLineWidth(3);
        glBegin(GL_LINES);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X + name_width, y1 - name_height);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X, y1 - name_height);
        glEnd();
        glBegin(GL_LINES);
          glVertex2f(name_x, y1);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X, y1 - name_height);
        glEnd();
        glBegin(GL_LINES);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X + name_width, y1 - name_height);
          glVertex2f(name_x + (kNAME_BOX_ANGLE_X << 1) + name_width, y1);
        glEnd();
      }

      /* Only display the information once the dialog box is visible */
      if(dialog_status == ON)
      {
        /* Draw the name text */
        glColor4f(1.0, 1.0, 1.0, paused_opacity);
        painter->renderText(x1 + kNAME_BOX_X_OFFSET + kNAME_BOX_ANGLE_X, 
                           y1 - (name_height >> 1) 
                              - (bold_font.height() >> 1) + bold_font.ascent(), 
                           thing_name, display_font);
        display_font.setBold(false);

        /* Paint the object frame, if it exists */
        int img_width = 0;
        int img_height = 0;
        if(thing_frame != 0)
        {
          img_width = thing_frame->getImage().width();
          img_height = thing_frame->getImage().height();
          thing_frame->paintGl(x1 + width - (img_width >> 1), 704 - img_height, 
                               img_width, img_height, paused_opacity);
        }

        /* Draw the conversational text */
        int txt_y = y1 + (kMARGIN_TOP << 1) + font_info.ascent();
        display_font.setPointSize(kFONT_SIZE);
        int max_lines = dialog_text_index + kMAX_LINES;
        if(max_lines > dialog_text.size())
          max_lines = dialog_text.size();

        glColor4f(1.0, 1.0, 1.0, paused_opacity);
        for(int i = dialog_text_index; i < max_lines; i++)
        {
          painter->renderText(x1 + kMARGIN_SIDES, txt_y, 
                              dialog_text[i], display_font);
          txt_y += (kFONT_SPACING << 1) + font_info.height();
        }

        /* The last conversational text line, based on long sets */
        glColor4f(1.0, 1.0, 1.0, 0.25 * paused_opacity);
        if(dialog_text.size() > max_lines)
        {
          painter->renderText(x1 + kMARGIN_SIDES, txt_y, 
                              dialog_text[max_lines], display_font);
          txt_y += (kFONT_SPACING << 1) + font_info.height();
        }

        /* Add the option text, if it's applicable */
        if(conversation_info.next.size() > 1)
        {
          short arrow_y = txt_y - kSCROLL_OFFSET;
          int option_x = x1 + kMARGIN_SIDES + kOPTION_OFFSET;
          
          for(int i = dialog_option_top; 
              i < dialog_option_top + kMAX_OPTIONS; i++)
          {
            /* Paint the bounding box portion */
            if(dialog_option == i)
            {
              int margin = kOPTION_MARGIN;
              QRect option = font_info.boundingRect(options_text[i]);
              glColor4f(0.0, 0.0, 0.0, paused_opacity);
              glBegin(GL_QUADS);
                glVertex3f(option_x + option.x() - margin, 
                           txt_y + option.y() + margin + option.height(), 0);
                glVertex3f(option_x + option.x() - margin, 
                           txt_y + option.y()- margin, 0);
                glVertex3f(option_x + option.x() + margin + option.width(), 
                           txt_y + option.y() - margin, 0);
                glVertex3f(option_x + option.x() + margin + option.width(), 
                           txt_y + option.y() + margin + option.height(), 0);
              glEnd();
            }

            /* Paint the text portion */
            glColor4f(1.0, 1.0, 1.0, paused_opacity);
            painter->renderText(option_x, txt_y, options_text[i], display_font);
            txt_y += (kFONT_SPACING << 1) + font_info.height();
          }
          
          /* Paint the scrolling arrows */
          if(conversation_info.next.size() > kMAX_OPTIONS)
          {
            glColor4f(1.0, 1.0, 1.0, 0.5 * paused_opacity);
            option_x -= (kOPTION_OFFSET >> 1);
            
            /* The top indicator */
            if(dialog_option_top == 0)
            {
              short edge_width = (kSCROLL_CIRCLE_RADIUS >> 1);
              short inner_radius = kSCROLL_CIRCLE_RADIUS - 1;
              
              glBegin(GL_QUADS);
                glVertex2f(option_x - inner_radius, arrow_y - inner_radius);
                glVertex2f(option_x - inner_radius, arrow_y + inner_radius);
                glVertex2f(option_x + inner_radius, arrow_y + inner_radius);
                glVertex2f(option_x + inner_radius, arrow_y - inner_radius);
                
                glVertex2f(option_x - edge_width, 
                           arrow_y - kSCROLL_CIRCLE_RADIUS);
                glVertex2f(option_x - edge_width, 
                           arrow_y + kSCROLL_CIRCLE_RADIUS);
                glVertex2f(option_x + edge_width, 
                           arrow_y + kSCROLL_CIRCLE_RADIUS);
                glVertex2f(option_x + edge_width, 
                           arrow_y - kSCROLL_CIRCLE_RADIUS);

                glVertex2f(option_x - kSCROLL_CIRCLE_RADIUS, 
                           arrow_y - edge_width);
                glVertex2f(option_x - kSCROLL_CIRCLE_RADIUS, 
                           arrow_y + edge_width);
                glVertex2f(option_x + kSCROLL_CIRCLE_RADIUS, 
                           arrow_y + edge_width);
                glVertex2f(option_x + kSCROLL_CIRCLE_RADIUS, 
                           arrow_y - edge_width);
              glEnd();
            }
            else
            {
              glBegin(GL_TRIANGLES);
                glVertex2f(option_x, arrow_y - kSCROLL_TRIANGLE_HEIGHT);
                glVertex2f(option_x - kSCROLL_TRIANGLE_HEIGHT, 
                           arrow_y + kSCROLL_TRIANGLE_HEIGHT);
                glVertex2f(option_x + kSCROLL_TRIANGLE_HEIGHT, 
                           arrow_y + kSCROLL_TRIANGLE_HEIGHT);
              glEnd();
            }
              
            /* The bottom indicator */
            arrow_y += ((kFONT_SPACING << 1) + font_info.height()) << 1;
            if((dialog_option_top + kMAX_OPTIONS) == 
                                                conversation_info.next.size())
            {
              short edge_width = (kSCROLL_CIRCLE_RADIUS >> 1);
              short inner_radius = kSCROLL_CIRCLE_RADIUS - 1;
              
              glBegin(GL_QUADS);
                glVertex2f(option_x - inner_radius, arrow_y - inner_radius);
                glVertex2f(option_x - inner_radius, arrow_y + inner_radius);
                glVertex2f(option_x + inner_radius, arrow_y + inner_radius);
                glVertex2f(option_x + inner_radius, arrow_y - inner_radius);
                
                glVertex2f(option_x - edge_width, 
                           arrow_y - kSCROLL_CIRCLE_RADIUS);
                glVertex2f(option_x - edge_width, 
                           arrow_y + kSCROLL_CIRCLE_RADIUS);
                glVertex2f(option_x + edge_width, 
                           arrow_y + kSCROLL_CIRCLE_RADIUS);
                glVertex2f(option_x + edge_width, 
                           arrow_y - kSCROLL_CIRCLE_RADIUS);

                glVertex2f(option_x - kSCROLL_CIRCLE_RADIUS, 
                           arrow_y - edge_width);
                glVertex2f(option_x - kSCROLL_CIRCLE_RADIUS, 
                           arrow_y + edge_width);
                glVertex2f(option_x + kSCROLL_CIRCLE_RADIUS, 
                           arrow_y + edge_width);
                glVertex2f(option_x + kSCROLL_CIRCLE_RADIUS, 
                           arrow_y - edge_width);
              glEnd();
            }
            else
            {
              glBegin(GL_TRIANGLES);
                glVertex2f(option_x - kSCROLL_TRIANGLE_HEIGHT, 
                           arrow_y - kSCROLL_TRIANGLE_HEIGHT);
                glVertex2f(option_x, arrow_y + kSCROLL_TRIANGLE_HEIGHT);
                glVertex2f(option_x + kSCROLL_TRIANGLE_HEIGHT, 
                           arrow_y - kSCROLL_TRIANGLE_HEIGHT);
              glEnd();
            }
          }
        }

        /* Draw triangle cursor */
        if(conversation_info.next.size() <= 1 && 
           (dialog_text_index + kMAX_LINES) >= dialog_text.size())
        {
          int cursor_x = (1216 >> 1);
          glLineWidth(2);
          glColor4f(1.0, 1.0, 1.0, paused_opacity);
          glBegin(GL_LINE_LOOP);
            glVertex2f(cursor_x - kCURSOR_NEXT_SIZE, 
                       y1 + height - kCURSOR_NEXT_SIZE - (int)animation_cursor);
            glVertex2f(cursor_x + kCURSOR_NEXT_SIZE, 
                       y1 + height - kCURSOR_NEXT_SIZE - (int)animation_cursor);
            glVertex2f(cursor_x, y1 + height - (int)animation_cursor);
          glEnd();
        }
      }
      display_font.setBold(false);
    }

    /* Multiple side by side painting - test */
/*    usable_font.setBold(false);
    font_details = QFontMetrics(usable_font);
    painter->renderText(800, 300, "Test Running.", usable_font);
    painter->renderText(800, 300 + font_details.height(), 
                        "Test ", usable_font);
    glColor4f(0.9, 0.2, 0.0, 1.0);
    usable_font.setUnderline(true);
    painter->renderText(800 + font_details.width("Test "), 
                        300 + font_details.height(), "Running.", usable_font);*/
  }

  /* Draw the pickup notification portion */
  if(pickup_status != OFF)
  {
    /* Get tile data  */
    Frame* tile = notification_queue[0].thing_image;
    short tile_height = 0;
    short tile_width = 0;
    if(tile != 0)
    {
      tile_height = tile->getImage().height();
      tile_width = tile->getImage().width();
    }
    
    /* Get pickup general data for drawing */
    int pickup_height = tile_height + (kPICKUP_MARGIN << 1);
    if(pickup_height < kPICKUP_HEIGHT)
      pickup_height = kPICKUP_HEIGHT;
    int pickup_x = 1216 - pickup_offset;
    int pickup_y = kPICKUP_Y;

    /* The background draw */
    glColor4f(0.0, 0.0, 0.0, 0.65 * paused_opacity);  
    glBegin(GL_POLYGON);
      glVertex2f(pickup_x + kPICKUP_ANGLE_X, pickup_y);
      glVertex2f(pickup_x, pickup_y + kPICKUP_ANGLE_Y);
      glVertex2f(pickup_x, pickup_y + pickup_height - kPICKUP_ANGLE_Y);
      glVertex2f(pickup_x + kPICKUP_ANGLE_X, pickup_y + pickup_height);
      glVertex2f(pickup_x + pickup_width, pickup_y + pickup_height);
      glVertex2f(pickup_x + pickup_width, pickup_y);
    glEnd();
    
    /* The outline draw */
    glColor4f(1.0, 1.0, 1.0, paused_opacity);
    glLineWidth(4);
    glBegin(GL_LINES);
      glVertex2f(pickup_x + kPICKUP_ANGLE_X, pickup_y);
      glVertex2f(pickup_x, pickup_y + kPICKUP_ANGLE_Y);
      glVertex2f(pickup_x, pickup_y + pickup_height - kPICKUP_ANGLE_Y);
      glVertex2f(pickup_x + kPICKUP_ANGLE_X, pickup_y + pickup_height);
    glEnd();
    glLineWidth(3);
    glBegin(GL_LINES);
      glVertex2f(pickup_x + kPICKUP_ANGLE_X, pickup_y);
      glVertex2f(pickup_x + pickup_width, pickup_y);
      glVertex2f(pickup_x + 1, pickup_y + kPICKUP_ANGLE_Y - 1);
      glVertex2f(pickup_x + 1, pickup_y + pickup_height - kPICKUP_ANGLE_Y + 1);
      glVertex2f(pickup_x + kPICKUP_ANGLE_X, pickup_y + pickup_height);
      glVertex2f(pickup_x + pickup_width, pickup_y + pickup_height);
    glEnd();
    
    /* Paint the tile portion */
    tile->paintGl(pickup_x + kPICKUP_MARGIN, pickup_y + kPICKUP_MARGIN,
                  tile_width, tile_height, paused_opacity);
    
    /* Paint the text portion */
    display_font.setPointSize(12);
    display_font.setBold(true);
    QFontMetrics font_info(display_font);
    QString multiplier = "x " 
                       + QString::number(notification_queue[0].thing_count);
    int text_x = pickup_x + kPICKUP_MARGIN + tile_width + kPICKUP_TEXT_SPACE;
    int text_y = pickup_y + (pickup_height >> 1) + (font_info.ascent() >> 2);
    painter->renderText(text_x, text_y, multiplier, display_font);
    display_font.setBold(false);    
  }
  
  return true;
}

bool MapDialog::setDialogImage(QString path)
{
  if(dialog_display.setImage(path))
  {
    dialog_display.initializeGl();
    return true;
  }
  return false;
}

/* Sets the rendering font */
void MapDialog::setFont(QFont font)
{
  display_font = font;
  display_font.setBold(false);
  display_font.setItalic(false);
  display_font.setOverline(false);
  display_font.setUnderline(false);
}
  
void MapDialog::setThingData(QList<MapThing*> data)
{
  thing_data = data;
}

void MapDialog::update(float cycle_time)
{
  short height = dialog_display.getImage().height();

  /* Modify the opacity of the dialog information based on the paused status */
  if(paused && paused_opacity != 0)
  {
    paused_opacity -= kPAUSE_OPACITY_DIFF;
    if(paused_opacity <= 0.0)
    {
      paused_opacity = 0.0;
      
      /* Complete the animation sequence if it's occurring during a pause */
      if(dialog_status == HIDING)
        animation_offset = 0;
      else if(dialog_status == SHOWING)
        animation_offset = animation_height;
        
      /* Complete the pickup animation sequence */
      if(pickup_status == HIDING)
        pickup_offset = 0;
      else if(pickup_status == SHOWING)
        pickup_offset = pickup_width;
    }
  }
  else if(!paused && paused_opacity != kPAUSE_OPACITY_MAX)
  {
    paused_opacity += kPAUSE_OPACITY_DIFF;
    if(paused_opacity > kPAUSE_OPACITY_MAX)
      paused_opacity = kPAUSE_OPACITY_MAX;
  }
  
  /* Ignore all updating if paused */
  if(!paused)
  {
    /* If hiding, shift the display onto the screen */
    if(dialog_status == HIDING)
    {
      animation_offset -= kSHIFT_TIME * 1.0 / cycle_time;
      if(animation_offset <= 0.0)
      {
        dialog_status = OFF;
        animation_offset = 0.0;
        
        /* If notification, remove the first ptr */
        if(dialog_mode == NOTIFICATION)
        {
          dialog_mode = DISABLED;

          if(conversation_waiting)
          {
            if((notification_queue[0].time_visible >> 1) > dialog_time)
              notification_queue[0].time_visible = 
                                     (notification_queue[0].time_visible >> 1);
            setupConversation();
            conversation_waiting = false;
          }
          else
          {
            notification_queue.removeFirst();
          }
        }
        else
        {
          dialog_mode = DISABLED;
        }

        /* Set up the next notification, if there is one in the queue */
        initiateNextNotification();
      }
    }
    /* If showing, shift the display onto the screen */
    else if(dialog_status == SHOWING)
    {
      animation_offset += kSHIFT_TIME * 1.0 / cycle_time;
      if(animation_offset >= animation_height)
      {
        dialog_status = ON;
        animation_offset = animation_height;
      }
    }
    /* If ON, do action appropriate to the dialog mode */
    else if(dialog_status == ON)
    {
      /* This controls how long the notification box is displayed */
      if(dialog_mode == NOTIFICATION)
      {
        dialog_time -= cycle_time;
        if(dialog_time < 0)
          dialog_status = HIDING;
      }
      /* This controls the bouncing cursor at the bottom of each conversational
       * box to signal going to the next set */
      else if(isInConversation())
      {
        /* Up motion */
        if(animation_cursor_up)
        {
          animation_cursor += cycle_time * kCURSOR_NEXT_SIZE 
                                         / kCURSOR_NEXT_TIME;
          if(animation_cursor >= kCURSOR_NEXT_SIZE)
          {
            animation_cursor = kCURSOR_NEXT_SIZE;
            animation_cursor_up = false;
          }
        }
        /* Down motion */
        else
        {
          animation_cursor -= cycle_time * kCURSOR_NEXT_SIZE 
                                         / kCURSOR_NEXT_TIME;
          if(animation_cursor <= 0)
          {
            animation_cursor = 0;
            animation_cursor_up = true;
          }
        }
      }
    }
    
    /* The portion that updates the pickup notification - if hiding */
    if(pickup_status == HIDING)
    {
      pickup_offset -= kSHIFT_TIME * 1.0 / cycle_time;
      if(pickup_offset <= 0.0)
      {
        pickup_status = OFF;
        pickup_offset = 0.0;
        
        /* Remove from queue since completed */
        notification_queue.removeFirst();
        
        /* Set up the next notification, if there is one in the queue */
        initiateNextNotification();
      }
    }
    /* If showing, shift it out until fully visible */
    else if(pickup_status == SHOWING)
    {
      pickup_offset += kSHIFT_TIME * 1.0 / cycle_time;
      if(pickup_offset >= pickup_width)
      {
        pickup_status = ON;
        pickup_offset = pickup_width;
      }
    }
    /* If visible, display for the duration as set of the pickup time */
    else if(pickup_status == ON)
    {
      pickup_time -= cycle_time;
      if(pickup_time < 0)
        pickup_status = HIDING;
    }
  }
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/* Fits a line into a list of lines that are less than the size using the
 * given parameters */
QList<QString> MapDialog::lineSplitter(QString line, 
                                       int line_length, QFont font)
{
  /* Initial parameter setup */
  QFontMetrics font_details(font);
  int index = 0;
  QList<QString> line_stack;
  QString line_str = "";
  QString temp_str = "";
  QStringList word_list = line.split(" ");
  
  /* Loop through all the words in the line */
  while(index < word_list.size())
  {
    do
    {
      /* Try and append the next word */
      temp_str = line_str;
      if(!temp_str.isEmpty())
        temp_str += " ";
      temp_str += word_list[index];
      
      /* If the line is too long, append the previous line */
      if(font_details.boundingRect(temp_str).width() > line_length)
      {
        /* This is for infinite loop checking. If the first word is too long,
         * append it anyway */
        if(line_str.isEmpty())
        {
          index++;
          line_str = temp_str;
        }
          
        line_stack.append(line_str);
        line_str = "";
      }
      /* Otherwise, add the next word and increase the line length */
      else
      {
        index++;
        line_str = temp_str;
      }
    } while(!line_str.isEmpty() && index < word_list.size());
    
    /* If it ended before filling the line, append the remainder */
    if(!line_str.isEmpty())
      line_stack.append(line_str);
  }
  
  return line_stack;
}
