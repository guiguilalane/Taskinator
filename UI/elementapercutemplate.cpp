#include "elementapercutemplate.h"

ElementApercuTemplate::ElementApercuTemplate(QWidget *parent) :
    QWidget(parent)
{
    type_ = new QLabel();
    name_ = new QLabel();
    name_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout_ = new QHBoxLayout();
    layout_->addWidget(type_);
    layout_->addWidget(name_);

    this->setLayout(layout_);
}

void ElementApercuTemplate::setValueType_(QString t){
    type_->setText(t);
}

void ElementApercuTemplate::setValueName_(QString t)
{
    name_->setText(t);
}

QString ElementApercuTemplate::getValueName_()
{
    return name_->text();
}
