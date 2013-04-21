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
#include "Sound.h"
//#include "SavedGame.h"

class TitleScreen : public QWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  TitleScreen(int width, int height, QWidget* parent = 0);

  /* Destructor function */
  ~TitleScreen();

  /* Public enumerators */
  //enum MenuState{OFF,MAIN,CONTINUE,INOPTIONS,INEXIT,SECRET};
  enum MenuState{TESTMAP,TESTBATTLE,MAINEXIT};

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
  MenuState current_state;

  /* The previously selected menu position */
  int previous_selection;

  /* The currently selected menu position */
  int current_selection;

  /* The options names for menu building */
  QVector<QString> option_names;

  Sound* background_sound;
  Sound* menu_click_sound;

  /* ------------------ Constants ------------------ */
  const static int kNUM_MENU_ITEMS;   /* Number of menu items in screen */
  const static QString kMENU_ITEMS[]; /* The stored menu items */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  void paintEvent(QPaintEvent*);
  void keyPressEvent(QKeyEvent*);

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  void close();
  void openBattle();
  void openMap();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void closing();
  void openingBattle(int index);
  void openingMap(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  void decrementState();

  void incrementState();

  /* Changes the menu to state s and the given index */
  void iterate(MenuState s, int index);

  void playBackground();

  bool setSelectedMenu(int menu_count);

  bool setState(int index);

  void setup();

  bool unsetSelectedMenu(int menu_count);
};

#endif // TITLESCREEN_H
