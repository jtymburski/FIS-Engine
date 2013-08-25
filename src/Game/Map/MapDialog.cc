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
const short MapDialog::kMSEC_PER_WORD = 333;
const short MapDialog::kNAME_BOX_ANGLE_X = 42;
const short MapDialog::kNAME_BOX_HEIGHT = 42;
const short MapDialog::kNAME_BOX_MIN_WIDTH = 50;
const short MapDialog::kNAME_BOX_X_OFFSET = 45;
const short MapDialog::kOPTION_MARGIN = 5;
const short MapDialog::kOPTION_OFFSET = 50;
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
  display_option = 0;
  display_time = 0;
  npc_name = "";
  person_name = "";
  
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
  
Frame* MapDialog::getThingDisplay(int id)
{
  Frame* null_frame = 0;

  for(int i = 0; i < thing_data.size(); i++)
  {
    if(thing_data[i]->getID() == id)
      return thing_data[i]->getDialogImage();
  }

  return null_frame;
}

QString MapDialog::getThingName(int id)
{
  for(int i = 0; i < thing_data.size(); i++)
  {
    if(thing_data[i]->getID() == id)
      return thing_data[i]->getName();
  }
  
  return "";
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

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

bool MapDialog::haltDialog()
{
  if(dialog_display.isImageSet())
  {
    /* Reset the animation sequence if ON */
    if(dialog_status == ON)
      animation_offset = 0;

    /* Change the status, if not already in OFF */
    if(dialog_status != OFF)
    {
      dialog_status = HIDING;
      return true;
    }
  }

  return false;
}

bool MapDialog::initConversation(Conversation dialog_info)
{
  if(!isInUse() && isDialogImageSet())
  {
    dialog_mode = CONVERSATION;
    display_font.setPointSize(12);
    conversation_info = dialog_info;

    /* Acquire all the needed information about things in the conversation */
    emit setThingData(removeDuplicates(calculateThingList(dialog_info)));

//    QFontMetrics font_info(display_font);
    
    /* Calculate the available space */
//    int available_width = 
//                      dialog_display.getImage().width() - (kMARGIN_SIDES << 1);
//    if(available_width < 0)
//      available_width = 0;
    
    /* Chop to single line if required */
//    if(single_line)
//      notification = 
//            font_info.elidedText(notification, Qt::ElideRight, available_width);
    
    /* Split the line */
//    display_text = lineSplitter(notification, available_width, display_font);
    
    /* Initiate animation sequence */
    initiateAnimation(display_font);
    
    return true;
  }
  
  return false;
}

/* Sets up a dialog with the initial parameters */
bool MapDialog::initDialog()//MapPerson* left, MapPerson* right)
{
  if(dialog_display.isImageSet())
  {
    /*Reset the animation sequence if OFF */
    if(dialog_status == OFF)
      animation_offset = dialog_display.getImage().height();

    /* Change the status, if not already in ON */
    if(dialog_status != ON)
    {
      dialog_status = SHOWING;
      return true;
    }
  }

  return false;
}

bool MapDialog::initNotification(QString notification, int time_visible, 
                                                       bool single_line)
{
  if(!isInUse() && isDialogImageSet())
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

/* Sets up the popout box */
//void MapStatusBar::initPopout(QImage* img, QString* dialog)
//{
//    (void)img;//warning
//    (void)dialog;//warning
//}

bool MapDialog::isDialogImageSet()
{
  return dialog_display.isImageSet();
}
  
bool MapDialog::isInConversation()
{
  return (dialog_mode == CONVERSATION);
}

bool MapDialog::isInUse()
{
  return (dialog_mode != DISABLED);
}

void MapDialog::keyPress(QKeyEvent* event)
{
  /* Selection case for the conversational dialog option */
  if(event->key() == Qt::Key_Space && !event->isAutoRepeat())
  {
    if(isInConversation() && dialog_status == ON)
    {
      bool multiple = (conversation_info.next.size() > 1);

      /* Check for dialog status and shift to next conversation */
      if(conversation_info.next.size() == 0)
        dialog_status = HIDING;
      else
        conversation_info = conversation_info.next[display_option];
      
      /* Skip the option if it was an option case */
      if(multiple)
        conversation_info = conversation_info.next[0];
      display_option = 0;
    }
  }
  /* Go back an option selector */
  else if(event->key() == Qt::Key_Up)
  {
    display_option--;
    if(display_option < 0)
      display_option = conversation_info.next.size() - 1;
  }
  /* Go to next option selector */
  else if(event->key() == Qt::Key_Down)
  {
    display_option++;
    if(display_option >= conversation_info.next.size())
      display_option = 0;
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
    dialog_display.paintGl(x1, y1, width, height, 1.0);

    if(dialog_mode == NOTIFICATION)
    {
      short x2 = x1;
      short y2 = y1 + kMARGIN_TOP + font_info.ascent();
      
      for(int i = 0; i < display_text.size(); i++)
      {
        x2 = x1 + ((width - font_info.width(display_text[i])) >> 1);
        painter->renderText(x2, y2, display_text[i], display_font);
        y2 += font_info.descent() + kFONT_SPACING + font_info.ascent();
      }
    }
    else if(isInConversation())
    {
      display_font.setPointSize(14);
      display_font.setBold(true);
      
      QFontMetrics bold_font(display_font);
      Frame* thing_frame = getThingDisplay(conversation_info.thing_id);
      QString thing_name = getThingName(conversation_info.thing_id);

      /* Calculate the width and height of the name box */
      int name_height = kNAME_BOX_HEIGHT;
      int name_x = x1 + kNAME_BOX_X_OFFSET;
      int name_width = bold_font.width(thing_name);
      if(name_width < kNAME_BOX_MIN_WIDTH)
        name_width = kNAME_BOX_MIN_WIDTH;

      if(thing_name.size() > 0)
      {
        /* Draw text display background */
        glColor4f(0.0, 0.0, 0.0, 0.65);
        glBegin(GL_QUADS);
          glVertex2f(name_x, y1);
          glVertex2f(name_x + (kNAME_BOX_ANGLE_X << 1) + name_width, y1);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X + name_width, y1 - name_height);
          glVertex2f(name_x + kNAME_BOX_ANGLE_X, y1 - name_height);
        glEnd();

        /* Draw outline */
        glColor4f(1.0, 1.0, 1.0, 1.0);
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
        glColor4f(1.0, 1.0, 1.0, 1.0);
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
                               img_width, img_height, 1.0);
        }

        /* Draw the conversational text */
        int txt_length = width - (kMARGIN_SIDES << 1) 
                               - (img_width >> 1);
        QString txt_line = conversation_info.text;
        int txt_y = y1 + (kMARGIN_TOP << 1) + font_info.ascent();
        display_font.setPointSize(kFONT_SIZE);
        QFontMetrics normal_font(display_font);

        if(conversation_info.next.size() > 1)
          txt_line = normal_font.elidedText(txt_line, 
                                            Qt::ElideRight, txt_length);
        QList<QString> convo_list = lineSplitter(txt_line, 
                                                 txt_length, display_font);
        for(int i = 0; i < convo_list.size(); i++)
        {
          painter->renderText(x1 + kMARGIN_SIDES, txt_y, 
                              convo_list[i], display_font);
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
                  normal_font.boundingRect(conversation_info.next[i].text);
              glColor4f(0.0, 0.0, 0.0, 1.0);
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
            glColor4f(1.0, 1.0, 1.0, 1.0);
            painter->renderText(option_x, txt_y, 
                                conversation_info.next[i].text, display_font);
            txt_y += (kFONT_SPACING << 1) + font_info.height();
          }
        }
      }
      display_font.setBold(false);

      /* Draw triangle cursor */
      if(conversation_info.next.size() <= 1)
      {
        int cursor_x = (1216 >> 1);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
          glVertex2f(cursor_x - kCURSOR_NEXT_SIZE, 
                     y1 + height - kCURSOR_NEXT_SIZE - animation_cursor);
          glVertex2f(cursor_x + kCURSOR_NEXT_SIZE, 
                     y1 + height - kCURSOR_NEXT_SIZE - animation_cursor);
          glVertex2f(cursor_x, y1 + height - animation_cursor);
        glEnd();
      }
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

  return true; // TODO??
}

/* Proceeds in the conversation, enter key triggers this */
//void MapStatusBar::proceed()
//{
//}

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
  
bool MapDialog::setNpcDisplay(QString path)
{
  if(npc_display.setImage(path))
  {
    npc_display.initializeGl();
    return true;
  }
  return false;
}
  
void MapDialog::setNpcName(QString name)
{
  npc_name = name;
}

bool MapDialog::setPersonDisplay(QString path)
{
  if(person_display.setImage(path))
  {
    person_display.initializeGl();
    return true;
  }
  return false;
}

void MapDialog::setPersonName(QString name)
{
  person_name = name;
}

void MapDialog::setThingData(QList<MapThing*> data)
{
  thing_data = data;
}

void MapDialog::update(float cycle_time)
{
  short height = dialog_display.getImage().height();

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
