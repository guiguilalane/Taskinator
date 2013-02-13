#include "element.h"

Element::Element(QWidget *parent) : QWidget(parent)
{
    type_ = new QLabel();
    name_ = new QLineEdit(QString("Nom de l'élément"));
    date_ = new QDateEdit(QDate::currentDate());
    supp_ = new QToolButton();
    supp_->setIcon(QIcon("://Pictures/trash_empty.png"));
    check_ = new QCheckBox();

    layout_ = new QHBoxLayout();
    layout_->addWidget(type_);
    layout_->addWidget(name_);
    layout_->addWidget(date_);
    layout_->addWidget(supp_);
    layout_->addWidget(check_);

    this->setLayout(layout_);
}

void Element::setValueType_(QString t){
    type_->setText(t);
}

void Element::setValueName_(QString t)
{
    name_->setText(t);
}

void Element::setValueDate_(QDate d)
{
    date_->setDate(d);
}

void Element::setValueCheck_(bool b)
{
    check_->setChecked(b);
}

QString Element::getValueName_()
{
    return name_->text();
}

QDate Element::getValueDate_()
{
    // TODO : est-ce que ca devrait pas retourner un time_t ?
    return date_->date();
}

bool Element::getValueCheck_()
{
    return check_->isChecked();
}
