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
const short MapDialog::kMSEC_PER_WORD = 333;
const short MapDialog::kNAME_BOX_ANGLE_X = 42;
const short MapDialog::kNAME_BOX_HEIGHT = 42;
const short MapDialog::kNAME_BOX_MIN_WIDTH = 50;
const short MapDialog::kNAME_BOX_X_OFFSET = 45;
const short MapDialog::kOPTION_MARGIN = 5;
const short MapDialog::kOPTION_OFFSET = 50;
const float MapDialog::kPAUSE_OPACITY_DIFF = 0.03;
const float MapDialog::kPAUSE_OPACITY_MAX = 1.0;
const short MapDialog::kSHIFT_TIME = 750;

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
  dialog_status = OFF;
  display_index = 0;
  display_option = 0;
  display_time = 0;
  paused = false;
  paused_opacity = kPAUSE_OPACITY_MAX;
  thing = 0;
  
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
    animation_height = (display_text.size() - 1)*kFONT_SPACING + 
                       display_text.size() * font_info.height() +
                       kFONT_SPACING + kMARGIN_TOP;
  }
}

/* Removes duplicates from the given qlist and returns it. Used in
 * conjunction with "calculateThingList" */
QList<int> MapDialog::removeDuplicates(QList<int> duplicate_list)
{
  return duplicate_list.toSet().toList();
}

void MapDialog::setupConversation()
{
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
  int txt_length = dialog_display.getImage().width() - (kMARGIN_SIDES << 1) 
                               - (img_width >> 1);
  QString txt_line = conversation_info.text;

  /* Split up the text as per the visible viewing area */
  if(conversation_info.next.size() > 1)
    txt_line = normal_font.elidedText(txt_line, Qt::ElideRight, txt_length);
  display_text = lineSplitter(txt_line, txt_length, display_font);
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
  if(isDialogImageSet())
  {
    if(dialog_mode == DISABLED)
    {
      dialog_mode = CONVERSATION;
      display_font.setPointSize(12);
      conversation_info = dialog_info;

      /* Acquire all the needed information about things in the conversation */
      emit setThingData(removeDuplicates(calculateThingList(dialog_info)));
      setupConversation();
    
      /* Initiate animation sequence */
      initiateAnimation(display_font);
    
      return true;
    }
    else if(dialog_mode == NOTIFICATION)
    {
      // TODO: Make it a queue that ends the notification and then initiates
      // conversation. However, this still keeps the image display notification
      return false;
    }
  }
  
  return false;
}

bool MapDialog::initNotification(QString notification, Frame* thing_image, 
                                 int thing_count, int time_visible, 
                                 bool single_line)
{
  if(!isBottomInUse() && isDialogImageSet())
  {
    dialog_mode = NOTIFICATION;
    display_font.setPointSize(12);
    QFontMetrics font_info(display_font);
    
    /* Calculate the available space */
    int available_width = 
                      dialog_display.getImage().width() - (kMARGIN_SIDES << 1);
    if(available_width < 0)
      available_width = 0;
    
    /* Chop to single line if required */
    if(single_line)
      notification = 
            font_info.elidedText(notification, Qt::ElideRight, available_width);
    
    /* Calculate the display time if invalid */
    if(time_visible < 0)
      display_time = ((notification.count(" ") + 1) * kMSEC_PER_WORD) 
                   + kMSEC_PER_WORD;
    else
      display_time = time_visible;
    
    /* Split the line */
    display_text = lineSplitter(notification, available_width, display_font);
    
    /* Initiate animation sequence */
    initiateAnimation(display_font);
    
    return true;
  }
  
  return false;
}

bool MapDialog::isDialogImageSet()
{
  return dialog_display.isImageSet();
}
  
bool MapDialog::isInConversation()
{
  return (dialog_mode == CONVERSATION);
}

bool MapDialog::isBottomInUse()
{
  return (dialog_mode != DISABLED);
}

bool MapDialog::isSideInUse()
{
  return false; // TODO
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
        if((display_index + kMAX_LINES) < display_text.size())
        {
          display_index += kMAX_LINES;
        }
        else if(conversation_info.next.size() == 0)
        {
          dialog_status = HIDING;
          emit finishThingTarget();
        }
        else
        {
          bool multiple = (conversation_info.next.size() > 1);
          conversation_info = conversation_info.next[display_option];
              
          /* Skip the option if it was an option case */
          if(multiple)
            conversation_info = conversation_info.next[0];

          display_index = 0;
          display_option = 0;

          /* Finalize the conversation change */
          setupConversation();
        }
      }
    }
    /* Go back an option selector */
    else if(event->key() == Qt::Key_Up)
    {
      display_option--;
      if(display_option < 0)
      {
        if(conversation_info.next.size() > 0)
          display_option = conversation_info.next.size() - 1;
        else
          display_option = 0;
      }
    }
    /* Go to next option selector */
    else if(event->key() == Qt::Key_Down)
    {
      display_option++;
      if(display_option >= conversation_info.next.size())
        display_option = 0;
    }
  }
}

void MapDialog::keyRelease(QKeyEvent* event)
{
  /* Unused currently */
  (void)event;
}

// TODO: Transition elements of conversation
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
      
      for(int i = 0; i < display_text.size(); i++)
      {
        x2 = x1 + ((width - font_info.width(display_text[i])) >> 1);
        painter->renderText(x2, y2, display_text[i], display_font);
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
        int max_lines = display_index + kMAX_LINES;
        if(max_lines > display_text.size())
          max_lines = display_text.size();

        glColor4f(1.0, 1.0, 1.0, paused_opacity);
        for(int i = display_index; i < max_lines; i++)
        {
          painter->renderText(x1 + kMARGIN_SIDES, txt_y, 
                              display_text[i], display_font);
          txt_y += (kFONT_SPACING << 1) + font_info.height();
        }

        /* The last conversational text line, based on long sets */
        glColor4f(1.0, 1.0, 1.0, 0.25 * paused_opacity);
        if(display_text.size() > max_lines)
        {
          painter->renderText(x1 + kMARGIN_SIDES, txt_y, 
                              display_text[max_lines], display_font);
          txt_y += (kFONT_SPACING << 1) + font_info.height();
        }

        /* Add the option text, if it's applicable */
        if(conversation_info.next.size() > 1)
        {
          for(int i = 0; i < conversation_info.next.size(); i++)
          {
            int option_x = x1 + kMARGIN_SIDES + kOPTION_OFFSET;

            /* Paint the bounding box portion */
            if(display_option == i)
            {
              int margin = kOPTION_MARGIN;
              QRect option = 
                  font_info.boundingRect(conversation_info.next[i].text);
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
            painter->renderText(option_x, txt_y, 
                                conversation_info.next[i].text, display_font);
            txt_y += (kFONT_SPACING << 1) + font_info.height();
          }
        }

        /* Draw triangle cursor */
        if(conversation_info.next.size() <= 1 && 
           (display_index + kMAX_LINES) >= display_text.size())
        {
          int cursor_x = (1216 >> 1);
          glLineWidth(2);
          glColor4f(1.0, 1.0, 1.0, paused_opacity);
          glBegin(GL_LINE_LOOP);
            glVertex2f(cursor_x - kCURSOR_NEXT_SIZE, 
                       y1 + height - kCURSOR_NEXT_SIZE - animation_cursor);
            glVertex2f(cursor_x + kCURSOR_NEXT_SIZE, 
                       y1 + height - kCURSOR_NEXT_SIZE - animation_cursor);
            glVertex2f(cursor_x, y1 + height - animation_cursor);
          glEnd();
        }
      }
      display_font.setBold(false);
    }
   
    /* Testing the notification display */
    //Frame notification_1("sprites/Map/Overlay/notification_1.png");
    //notification_1.initializeGl(); // 1039, 1100
    //notification_1.paintGl(1100.0, 25.0, 177, 148, paused_opacity);
    Frame notification_2("sprites/Map/Overlay/notification_2.png");
    notification_2.initializeGl(); // 1060, 1117
    notification_2.paintGl(1060.0, 50.0, 156, 110, paused_opacity);
    //Frame notification_3("sprites/Map/Overlay/notification_3.png");
    //notification_3.initializeGl(); // 1061, 1118
    //notification_3.paintGl(1061.0, 50.0, 155, 108, paused_opacity);

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

  /* Update from events that occurred during running */
  //if(display_text_update)
  //{
  //  display_text = display_text_new;
  //  display_text_new.clear();
  //  display_text_update = false;
  //}
  
  return true; // TODO??
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
      animation_offset -= cycle_time * height / kSHIFT_TIME;
      if(animation_offset <= 0)
      {
        dialog_mode = DISABLED;
        dialog_status = OFF;
        animation_offset = 0;
      }
    }
    /* If showing, shift the display onto the screen */
    else if(dialog_status == SHOWING)
    {
      animation_offset += cycle_time * height / kSHIFT_TIME;
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
        display_time -= cycle_time;
        if(display_time < 0)
          dialog_status = HIDING;
      }
      /* This controls the bouncing cursor at the bottom of each conversational
       * box to signal going to the next set */
      else if(isInConversation())
      {
        if(animation_cursor_up)
        {
          animation_cursor += cycle_time * kCURSOR_NEXT_SIZE / kCURSOR_NEXT_TIME;
          if(animation_cursor >= kCURSOR_NEXT_SIZE)
          {
            animation_cursor = kCURSOR_NEXT_SIZE;
            animation_cursor_up = false;
          }
        }
        else
        {
          animation_cursor -= cycle_time * kCURSOR_NEXT_SIZE / kCURSOR_NEXT_TIME;
          if(animation_cursor <= 0)
          {
            animation_cursor = 0;
            animation_cursor_up = true;
          }
        }
      }
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
