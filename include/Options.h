 /*****************************************************************************
* Class Name: Options
* Date Created: Sunday, November 4th, 2012
* Inheritance: None?
* Description: Options menu 
*  
* TODO: CONSTRUCTORS TO BE FINISHED
******************************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QtGui/QWidget>

class Options : public QWidget
{

public:

  Options(QWidget *parent =0);
  ~Options();

private:

  /* Resolution currently being used */
  int x_resolution, y_resolution; 

  /* Horizontal resolution options */
  QVector<int> x_options; 

  /* Vertical resolution options */
  QVector<int> y_options; 

  /* Primary options menu */
  QVector<QString*> main_options;

  /* Secondary options menu */
  /* Notes: other options to be added as need (audio, animations, font size) */
  QVector<QString*> secondary_Options;

  /* Flag for battle animations */
  bool ANIMATIONS; 

public:

  /* Gets the value of the given option */
  int getOption(QString s); 

  /* Sets the given option ot the give value (may require reimplementation) */
  void setOption(QString s, int value); 

};

#endif
