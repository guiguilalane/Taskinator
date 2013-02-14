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
    Q_OBJECT

public:
    Element(QWidget *parent = 0);
    void setValueType_(QString t);
    void setValueName_(QString t);
    void setValueDate_(QDate d);
    void setValueCheck_(int b);
    QString getValueName_();
    QDate getValueDate_();
    int getValueCheck_();

signals:
    void elementChanged();

private slots:
    void onNameChange();
    void onDateChange();
    void onCheckChange(int state);

private:
    QLabel * type_;
    QLineEdit * name_;
    QDateEdit * date_;
    QToolButton * supp_;
    QCheckBox * check_;
    QHBoxLayout * layout_;
};

#endif // ELEMENT_H
