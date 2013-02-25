#ifndef GRIDSHIFTER_H
#define GRIDSHIFTER_H

#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

class GridShifter : public QWidget
{
  Q_OBJECT
public:
   GridShifter(QWidget *parent = 0);
   QLineEdit* getBase();
   QLineEdit* getLower();
   QLineEdit* getUpper();
private:
   QLineEdit* base;
   QLineEdit* lower;
   QLineEdit* upper;
protected:
   void keyPressEvent(QKeyEvent *event);
};

#endif // GRIDSHIFTER_H
