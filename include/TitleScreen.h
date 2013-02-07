/******************************************************************************
* Class Name: TitleScreen
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The TitleScreen class
******************************************************************************/
#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QString>
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QVector>

#include "Options.h"
//#include "SavedGame.h"

/* The available menu options, and another one for potential easter eggs */
//enum State{OFF,MAIN,CONTINUE,INOPTIONS,INEXIT,SECRET};
enum State{TESTMAP,TESTBATTLE,MAINEXIT};

class TitleScreen : public QWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  TitleScreen(int width, int height, QWidget* parent = 0);

  /* Destructor function */
  ~TitleScreen();

protected:
  void paintEvent(QPaintEvent*);
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);

private:
  /* For the first menu level's position */
  int cursor_index;

  /* The options at the first menu level */
  QVector<QLabel*> option_labels;

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

  /* ------------------ Constants ------------------ */
  const static int kNUM_MENU_ITEMS = 3;
  const static QString kMENU_ITEMS[];

//public: // TODO: Why is this commented out? [December 2, 2012]
 
  void decrementState();

  void incrementState();

  /* Changes the menu to state s and the given index */
  void iterate(State s, int index);

  void setup();
};


#endif // TITLESCREEN_H
