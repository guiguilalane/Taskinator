#include "newlist.h"
#include "ui_newlist.h"

NewList::NewList(bool boutonAnnulerActif, QStringList templates, QWidget *parent) :
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
    templates = QStringList() << tr("aucun") << templates;
    ui->comboBox->addItems(templates);
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
    QString path = "";
    if(ui->comboBox->currentIndex() > 0)
    {
        path = ui->comboBox->currentText();
    }
    emit createList(path, ui->radioButtonNo->isChecked(), ui->lineEditName->text(), ui->dateEdit->dateTime());
}

void NewList::on_comboBox_currentIndexChanged(int index)
{
    bool b = (index==0);
    ui->ordered->setEnabled(b);
    ui->radioButtonNo->setEnabled(b);
    ui->radioButtonYes->setEnabled(b);
}
