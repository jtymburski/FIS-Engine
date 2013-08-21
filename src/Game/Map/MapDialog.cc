/******************************************************************************
* Class Name: MapDialog
* Date Created: August 7, 2013
* Inheritance: none
* Description: The dialog display at the bottom of the map. Offers either
*              conversation options and display or notifications.
******************************************************************************/
#include "Game/Map/MapDialog.h"

/* Constant Implementation - see header file for descriptions */
const short MapDialog::kFONT_SIZE = 12;
const short MapDialog::kFONT_SPACING = 10;
const short MapDialog::kMARGIN_SIDES = 50;
const short MapDialog::kMARGIN_TOP = 25;
const short MapDialog::kMSEC_PER_WORD = 333;
const short MapDialog::kPIXELS_PER_100MS = 30;
const short MapDialog::kSHIFT_TIME = 750;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapDialog::MapDialog(QFont font)
{ 
  /* Clear some of the initial parameters */
  animation_offset = 0;
  dialog_mode = DISABLED;
  dialog_status = OFF;
  display_time = 0;
  npc_name = "";
  person_name = "";
  
  setFont(font);

  /* Testing */
  Conversation convo;
  convo.thing_id = 1;
  Conversation test1, test2, test3, test4, test5;
  test1.thing_id = 2;
  test2.thing_id = 3;
  test3.thing_id = 4;
  test4.thing_id = 3;
  test5.thing_id = 24;
  test3.next.append(test4);
  test1.next.append(test3);
  test1.next.append(test2);
  convo.next.append(test1);
  convo.next.append(test5);
  convo.next.append(test1);
  convo.next.append(test3);
  qDebug() << calculateThingList(convo);
  qDebug() << removeDuplicates(calculateThingList(convo));
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

/* Halts the dialog, if it's being shown or showing */
void MapDialog::initiateAnimation(QFont display_font)
{
  animation_offset = 0;
  dialog_status = SHOWING;
  QFontMetrics font_info(display_font);
  
  if(dialog_mode == CONVERSATION)
  {
  
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

bool MapDialog::initConversation(Conversation* dialog_info)
{
  if(dialog_mode == DISABLED && isDialogImageSet() && dialog_info != 0)
  {
    dialog_mode = CONVERSATION;
//    display_font.setPointSize(12);
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
    
    /* Calculate the display time if invalid */
//    if(time_visible < 0)
//      display_time = ((notification.count(" ") + 1) * kMSEC_PER_WORD) 
//                   + kMSEC_PER_WORD;
//    else
//      display_time = time_visible;
    
    /* Split the line */
//    display_text = lineSplitter(notification, available_width, display_font);
    
    /* Initiate animation sequence */
//    initiateAnimation(display_font);
    
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
  if(dialog_mode == DISABLED && isDialogImageSet())
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
  
bool MapDialog::paintGl(QGLWidget* painter)
{
  if(dialog_status != OFF)
  {
    int height = dialog_display.getImage().height();
    int width = dialog_display.getImage().width();
    short x1 = (1216 - width) >> 1;
    short y1 = 704;

    /* Paints the conversation dialog */
    dialog_display.paintGl(x1, y1 - animation_offset, width, height, 1.0);

    if(dialog_mode == NOTIFICATION)
    {
      QFontMetrics font_info(display_font);
      short x2 = x1;
      short y2 = y1 + kMARGIN_TOP + font_info.ascent();
      
      for(int i = 0; i < display_text.size(); i++)
      {
        x2 = x1 + ((width - font_info.width(display_text[i])) >> 1);
        painter->renderText(x2, y2 - animation_offset, 
                            display_text[i], display_font);
        y2 += font_info.descent() + kFONT_SPACING + font_info.ascent();
      }
    }
    
/*    if(dialog_status == ON)
    {
      /* Draw text display background */
/*      glColor4f(0.0, 0.0, 0.0, 0.65);
      glBegin(GL_QUADS); /* 42 high, angle at 30, central width based on text */
/*        glVertex2f(232.0f, 482.0f);
        glVertex2f(442.0f, 482.0f);
        glVertex2f(412.0f, 440.0f);
        glVertex2f(262.0f, 440.0f);
      glEnd();

      /* Draw outline */
/*      glColor4f(1.0, 1.0, 1.0, 1.0);
      glLineWidth(3);
      glBegin(GL_LINES);
        glVertex2f(412.0f, 440.0f);
        glVertex2f(262.0f, 440.0f);
      glEnd();
      glBegin(GL_LINES);
        glVertex2f(232.0f, 482.0f);
        glVertex2f(262.0f, 440.0f);
      glEnd();
      glBegin(GL_LINES);
        glVertex2f(412.0f, 440.0f);
        glVertex2f(442.0f, 482.0f);
      glEnd();

      /* Paints the person near the conversation */
/*      short char_height = person_display.getImage().height();
      short char_width = person_display.getImage().width();

      person_display.paintGl(x1 + width - (char_width >> 1), 
                             704 - char_height, 
                             char_width, char_height, 1.0);

      /* Paint the text for the name */
/*      QFont usable_font = display_font;
      usable_font.setPointSize(14);
      usable_font.setBold(true);
      QFontMetrics font_details(usable_font);
      char_height = font_details.height();
      char_width = font_details.boundingRect(person_name).width();
      painter->renderText(
        (412 - 262 - char_width >> 1) + 262, 
        (482 - 440 - char_height >> 1) + 440 + font_details.ascent(), 
        person_name, usable_font);
        
      /* Multiple side by side painting - test */
/*      usable_font.setBold(false);
      font_details = QFontMetrics(usable_font);
      painter->renderText(800, 300, "Test Running.", usable_font);
      painter->renderText(800, 300 + font_details.height(), 
                          "Test ", usable_font);
      glColor4f(0.9, 0.2, 0.0, 1.0);
      usable_font.setUnderline(true);
      painter->renderText(800 + font_details.width("Test "), 
                          300 + font_details.height(), "Running.", usable_font);
    }*/
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

void MapDialog::update(float cycle_time)
{
  short height = dialog_display.getImage().height();

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
  else if(dialog_status == SHOWING)
  {
    animation_offset += cycle_time * height / kSHIFT_TIME;
    if(animation_offset >= animation_height)
    {
      dialog_status = ON;
      animation_offset = animation_height;
    }
  }
  else if(dialog_status == ON)
  {
    if(dialog_mode == NOTIFICATION)
    {
      display_time -= cycle_time;
      if(display_time < 0)
        dialog_status = HIDING;
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
