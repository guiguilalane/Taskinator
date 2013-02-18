#ifndef ELEMENTAPERCUTEMPLATE_H
#define ELEMENTAPERCUTEMPLATE_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>

class ElementApercuTemplate : public QWidget
{
    Q_OBJECT
public:
    explicit ElementApercuTemplate(QWidget *parent = 0);
    void setValueType_(QString t);
    void setValueName_(QString t);
    QString getValueName_();

private:
    QLabel * type_;
    QLabel * name_;
    QHBoxLayout * layout_;
};

#endif // ELEMENTAPERCUTEMPLATE_H
