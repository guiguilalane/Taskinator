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
