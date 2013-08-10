/******************************************************************************
* Class Name: MapDialog
* Date Created: August 7, 2013
* Inheritance: none
* Description: The dialog display at the bottom of the map. Offers either
*              conversation options and display or notifications.
******************************************************************************/
#include "Game/Map/MapDialog.h"

/* Constant Implementation - see header file for descriptions */
const short MapDialog::kSHIFT_TIME = 750;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapDialog::MapDialog()
{
  /* Clear some of the initial parameters */
  animation_offset = 0;
  dialog_status = OFF;
  npc_name = "";
  person_name = "";
}

/* Destructor function */
MapDialog::~MapDialog()
{
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

/* Sets up the popout box */
//void MapStatusBar::initPopout(QImage* img, QString* dialog)
//{
//    (void)img;//warning
//    (void)dialog;//warning
//}

bool MapDialog::paintGl(QGLWidget* painter)
{
  if(dialog_status != OFF)
  {
    int height = dialog_display.getImage().height();
    int width = dialog_display.getImage().width();
    short x1 = (1216 - width) >> 1;
    short y1 = (704 - height);

    /* Paints the conversation dialog */
    dialog_display.paintGl(x1, y1 + animation_offset, width, height, 1.0);

    if(dialog_status == ON)
    {
    /* Draw text display background */
      glColor4f(0.0, 0.0, 0.0, 0.65);
      glBegin(GL_QUADS); /* 42 high, angle at 30, central width based on text */
        glVertex2f(232.0f, 482.0f);
        glVertex2f(442.0f, 482.0f);
        glVertex2f(412.0f, 440.0f);
        glVertex2f(262.0f, 440.0f);
      glEnd();

      /* Draw outline */
      glColor4f(1.0, 1.0, 1.0, 1.0);
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
      short char_height = person_display.getImage().height();
      short char_width = person_display.getImage().width();

      person_display.paintGl(x1 + width - (char_width >> 1), 
                             704 - char_height, 
                             char_width, char_height, 1.0);

      /* Paint the text for the name */
      QFont font;
      font.setPointSize(14);
      font.setBold(true);
      QFontMetrics font_details(font);
      painter->renderText(
        (412 - 262 - font_details.width(person_name) >> 1) + 262, 
        (482 - 440 - font_details.height() >> 1) + 440 + (482 - 440 >> 1) - 2, 
        person_name, font);
    }
  }
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
    animation_offset += cycle_time * height / kSHIFT_TIME;
    if(animation_offset > height)
    {
      dialog_status = OFF;
      animation_offset = 0;
    }
  }
  else if(dialog_status == SHOWING)
  {
    animation_offset -= cycle_time * height / kSHIFT_TIME;
    if(animation_offset <= 0)
    {
      dialog_status = ON;
      animation_offset = 0;
    }
  }
}
