#include "GridShifter.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

GridShifter::GridShifter(QWidget *parent)
{
    setParent(parent);
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    QHBoxLayout* hlayout = new QHBoxLayout(this);
    base = new QLineEdit(":",this);
    lower = new QLineEdit(":",this);
    upper = new QLineEdit(":",this);
    QPushButton* ok = new QPushButton("Ok",this);

    hlayout->addWidget(new QLabel("Base:"));
    hlayout->addWidget(base);
    hlayout->addWidget(new QLabel("Lower:"));
    hlayout->addWidget(lower);
    hlayout->addWidget(new QLabel("Upper:"));
    hlayout->addWidget(upper);
    hlayout->addWidget(ok);
    setLayout(hlayout);

    connect(ok,SIGNAL(clicked()),parent,SLOT(update()));
    connect(ok,SIGNAL(clicked()),this,SLOT(close()));
    hide();
}

QLineEdit* GridShifter::getBase()
{
    return base;
}
QLineEdit* GridShifter::getLower()
{
    return lower;
}
QLineEdit* GridShifter::getUpper()
{
    return upper;
}

void GridShifter::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter)
        close();
}
