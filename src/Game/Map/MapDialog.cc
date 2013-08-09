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

bool MapDialog::paintGl()
{
  if(dialog_status != OFF)
  {
    int height = dialog_display.getImage().height();
    int width = dialog_display.getImage().width();

    dialog_display.paintGl((1216 - width) >> 1, 
                           (704 - height) + animation_offset, 
                           width, height, 1.0);
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
