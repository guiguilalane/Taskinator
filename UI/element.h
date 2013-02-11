#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

class Element : public QWidget
{
//Q_OBJECT

public:
    Element(QWidget *parent = 0);
    void changeType_(QString t);

private:
    QLabel * type_;
    QLineEdit * name_;
    QDateEdit * date_;
    QToolButton * supp_;
    QCheckBox * check_;
    QHBoxLayout * layout_;
};

#endif // ELEMENT_H
