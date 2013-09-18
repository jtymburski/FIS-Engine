/******************************************************************************
* Class Name: Options
* Date Created: Sunday, December 2, 2012
* Inheritance: QWidget
* Description: Options menu
*
* TODO: CONSTRUCTORS TO BE FINISHED
*       Add option to load settings from file and dump to file as well.
*
* Options to add:
*  - Set the text display speed in MapDialog (for the character letters)
******************************************************************************/
#include "Options.h"

/* Constant Implementation - see header file for descriptions */
const short Options::kNUM_RESOLUTIONS = 1;
const short Options::kRESOLUTIONS_X[1] = {1216};
const short Options::kRESOLUTIONS_Y[1] = {704};

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
Options::Options(QWidget* parent) : QWidget(parent)
{
  setAllToDefault();

  /* Temporary Vsync settings */
#ifdef unix
  vsync_enabled = false;
#else
  vsync_enabled = true;
#endif
}

Options::Options(const Options &source)
{
  copySelf(source);
}

/* Destructor function */
Options::~Options()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

void Options::copySelf(const Options &source)
{
  resolution_x = source.resolution_x;
  resolution_y = source.resolution_y;
  vsync_enabled = source.vsync_enabled;
}

void Options::setAllToDefault()
{
  setScreenHeight(0);
  setScreenWidth(0);
  setVsync(false);
}

void Options::setScreenHeight(int index)
{
  if(index >= 0 && index < kNUM_RESOLUTIONS)
    resolution_y = index;
}

void Options::setScreenWidth(int index)
{
  if(index >= 0 && index < kNUM_RESOLUTIONS)
    resolution_x = index;
}

void Options::setVsync(bool enabled)
{
  vsync_enabled = enabled;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Gets the value of the given option */
// TODO
//int Options::getOption(QString option)
//{
//    (void)option; //warning
//    return 0;
//}

short Options::getScreenHeight()
{
  return kRESOLUTIONS_Y[resolution_y];
}

short Options::getScreenWidth()
{
  return kRESOLUTIONS_X[resolution_x];
}

bool Options::isVsyncEnabled()
{
  return vsync_enabled;
}

/* Sets the given option ot the give value (may require reimplementation) */
// TODO
//void Options::setOption(QString option, int value)
//{
//    (void)option; //warning
//    (void)value; //warning
//}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

Options& Options::operator= (const Options &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
