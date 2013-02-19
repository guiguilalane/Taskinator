#include "elementapercu.h"


ElementApercu::ElementApercu(QWidget *parent) : QWidget(parent)
{
    type_ = new QLabel();
    name_ = new QLabel();
    name_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    date_ = new QLabel();
    check_ = new QCheckBox();
    check_->setEnabled(false);

    layout_ = new QHBoxLayout();
    layout_->addWidget(type_);
    layout_->addWidget(name_);
    layout_->addWidget(date_);
    layout_->addWidget(check_);

    this->setLayout(layout_);
}

void ElementApercu::setValueType_(QString t){
    type_->setText(t);
}

void ElementApercu::setValueName_(QString t)
{
    name_->setText(t);
}

void ElementApercu::setValueDate_(QDate d)
{
    date_->setText(d.toString("dd/MM/yyyy"));
}

void ElementApercu::setValueCheck_(bool b)
{
    check_->setChecked(b);
}

QString ElementApercu::getValueName_()
{
    return name_->text();
}

QDate ElementApercu::getValueDate_()
{
    return QDate::fromString(date_->text());
}

bool ElementApercu::getValueCheck_()
{
    return check_->isChecked();
}
