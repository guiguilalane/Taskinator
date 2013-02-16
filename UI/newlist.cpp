#include "newlist.h"
#include "ui_newlist.h"

NewList::NewList(bool boutonAnnulerActif, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewList)
{
    ui->setupUi(this);
    if (!boutonAnnulerActif)
    {
        QPushButton * cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
        cancelButton->setEnabled(false);
    }
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
