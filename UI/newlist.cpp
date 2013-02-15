#include "newlist.h"
#include "ui_newlist.h"

NewList::NewList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewList)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

NewList::~NewList()
{
    delete ui;
}

void NewList::on_buttonBox_rejected()
{
    this->close();
}

void NewList::on_buttonBox_accepted()
{
    emit createList(ui->radioButtonNo->isChecked(), ui->lineEditName->text(), ui->dateEdit->dateTime());
}
