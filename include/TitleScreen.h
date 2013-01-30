/******************************************************************************
* Class Name: TitleScreen
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The TitleScreen class
******************************************************************************/
#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QImage>
#include <QString>
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QVector>

#include "Options.h"
//#include "SavedGame.h"

/* The available menu options, and another one for potential easter eggs */
enum State{OFF,MAIN,CONTINUE,INOPTIONS,INEXIT,SECRET};

class TitleScreen : public QWidget
{
public:
  /* Constructor function */
  TitleScreen(QWidget *parent = 0);

  /* Destructor function */
  ~TitleScreen();

protected:
  void paintEvent(QPaintEvent*);

private:
  /* For the first menu level's position */
  int cursor_index;

  /* The options at the first menu level */
  QVector<QString> title_options;

  /* The selection cursor */
  QImage cursor;

  /* The total saved games for menu building purposes */
//  QVector<SavedGame*> games;

  /* The options for menu building purposes */
  Options* application_options;

  /* The currently selected menu state */
  State current_state;

  /* The previously selected menu position */
  int previous_selection;

  /* The currently selected menu position */
  int current_selection;

  /* The options names for menu building */
  QVector<QString> option_names;

//public: // TODO: Why is this commented out? [December 2, 2012]

  /* Changes the menu to state s and the given index */
  void iterate(State s, int index);
};


#endif // TITLESCREEN_H
