#include "element.h"

Element::Element(QWidget *parent) : QWidget(parent)
{
    type_ = new QLabel();
    name_ = new QLineEdit(QString(QString::fromUtf8("Nom de l'élément")));
    date_ = new QDateEdit(QDate::currentDate());
    date_->setDisplayFormat("dd/MM/yyyy");
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

    connect(name_, SIGNAL(textChanged(QString)), this, SLOT(onNameChange(QString)));
    connect(date_, SIGNAL(dateChanged(QDate)), this, SLOT(onDateChange(QDate)));
    connect(check_, SIGNAL(stateChanged(int)), this, SLOT(onCheckChange(int)));

    connect(supp_, SIGNAL(clicked()), this, SLOT(onDeleteElement()));
}

void Element::setValueType_(QString t){
    type_->setText(t);
}

void Element::onNameChange()
{
    emit elementChanged();
}

void Element::onNameChange(QString t)
{
    emit elementChanged();
}

void Element::onDateChange()
{
    emit elementChanged();
}

void Element::onDateChange(QDate d)
{
    emit elementChanged();
}

void Element::onCheckChange(int state)
{
    emit elementCheckChanged();
}

void Element::onDeleteElement()
{
    emit elementDeleted();
}

void Element::setValueName_(QString t)
{
    name_->setText(t);
}

void Element::setValueDate_(QDate d)
{
    date_->setDate(d);
}

void Element::setValueCheck_(int b)
{
    check_->setChecked(b);
}

QString Element::getValueName_()
{
    return name_->text();
}

QDate Element::getValueDate_()
{
    return date_->date();
}

int Element::getValueCheck_()
{
    return check_->isChecked();
}

void Element::setCheckable(const bool b)
{
    check_->setEnabled(b);
}
