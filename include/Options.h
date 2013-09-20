/******************************************************************************
* Class Name: Options
* Date Created: Sunday, November 4th, 2012
* Inheritance: QWidget
* Description: Options menu 
*  
* TODO: See cc file
******************************************************************************/
#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

class Options : public QWidget
{
public:
  /* Constructor(s) */
  Options(QWidget* parent = 0);

  /* Copy constructor */
  Options(const Options &source);

  /* Destructor */
  ~Options();

private:
  /* Resolution of the screen currently being used */
  short resolution_x;
  short resolution_y;

  /* Vertical refresh syncing enable */
  bool vsync_enabled;

  /* Horizontal resolution options */
  //QVector<int> x_options; 

  /* Vertical resolution options */
  //QVector<int> y_options; 

  /* Primary options menu */
  //QList<QString> main_options;

  /* Secondary options menu */
  /* Notes: other options to be added as need (audio, animations, font size) */
  //QList<QString> secondary_Options;

  /* Flag for battle animations */
  //bool ANIMATIONS; 

  /*------------------- Constants -----------------------*/
  const static short kNUM_RESOLUTIONS;  /* Number of resolutions available */
  const static short kRESOLUTIONS_X[1]; /* X Resolution sizes available */
  const static short kRESOLUTIONS_Y[1]; /* Y Resolution sizes available */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy all parameters from the other to the current if called */
  void copySelf(const Options &source);

  /* Sets all parameters to default */
  void setAllToDefault();

  /* Set the screen dimensions to be painted, based on the array index */
  void setScreenHeight(int index);
  void setScreenWidth(int index);

  /* Enable or disable vertical refresh syncing */
  void setVsync(bool enabled);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the value of the given option */
  //int getOption(QString s); 

  /* Returns the screen dimensions to be painted */
  short getScreenHeight();
  short getScreenWidth();

  /* Returns if vertical refresh syncing is enabled */
  bool isVsyncEnabled();

  /* Sets the given option ot the give value (may require reimplementation) */
  //void setOption(QString s, int value); 

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  Options& operator= (const Options &source);
};

#endif // OPTIONS_H
