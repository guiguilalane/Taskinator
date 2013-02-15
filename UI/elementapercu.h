#ifndef ELEMENTAPERCU_H
#define ELEMENTAPERCU_H

#include <iostream>

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

class ElementApercu : public QWidget
{
    Q_OBJECT

public:
    ElementApercu(QWidget *parent = 0);
    void setValueType_(QString t);
    void setValueName_(QString t);
    void setValueDate_(QDate d);
    void setValueCheck_(bool b);
    QString getValueName_();
    QDate getValueDate_();
    bool getValueCheck_();

private:
    QLabel * type_;
    QLabel * name_;
    QLabel * date_;
    QCheckBox * check_;
    QHBoxLayout * layout_;
};

#endif // ELEMENTAPERCU_H
