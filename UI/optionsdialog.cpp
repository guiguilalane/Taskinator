#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QDebug>

OptionsDialog::OptionsDialog(QSettings *set, Controleur *cont, QWidget *parent) :
    settings_(set),
    cont_(cont),
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    if(settings_->value("templateDirectory") != QVariant())
    {
        ui->templatePathDisplayer->setText(settings_->value("templateDirectory").toString());
    }
    else
    {
        QDir current = QDir::current();
        current.mkdir(current.absolutePath() + "/template");
        current.cd("template");
        settings_->setValue("templateDirectory", current.absolutePath());
        ui->templatePathDisplayer->setText(settings_->value("templateDirectory").toString());
        ui->TemplateContainer->setEnabled(true);
    }
    refreshTemplatesList();
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_templatePathButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QString::fromUtf8(tr("Ouvrir le répertoire des templates").toStdString().c_str()), getCurrentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->templatePathDisplayer->setText(path);
    settings_->setValue("templateDirectory", path);
    ui->TemplateContainer->setEnabled(!path.isEmpty());
    refreshTemplatesList();
}

void OptionsDialog::refreshTemplatesList()
{

    clearTemplatesList();

    QDir directory(ui->templatePathDisplayer->text());
    QStringList filters;
    filters << "*.ulk";
    directory.setNameFilters(filters);

    QFileInfoList fileInfos = directory.entryInfoList();
    QFileInfoList::iterator it;
    QListWidgetItem* item;
    for(it = fileInfos.begin(); it != fileInfos.end(); ++it)
    {
        item = new QListWidgetItem(it->baseName());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->listOfTemplates->addItem(item);
    }
    ui->deleteTemplateButton->setEnabled(false);
    ui->templatePreview->clear();
}

void OptionsDialog::clearTemplatesList()
{
    int size = ui->listOfTemplates->count();
    if(size > 0)
    {
        for(int i = 0; i < size; ++i)
        {
           delete ui->listOfTemplates->item(i);
        }
        ui->listOfTemplates->clear();
    }
}

QString OptionsDialog::getCurrentPath()
{
    QString currentPath = "./";
    if(settings_->value("templateDirectory") != QVariant())
    {
        currentPath = settings_->value("templateDirectory").toString();
    }
    return currentPath;
}

void OptionsDialog::on_listOfTemplates_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listOfTemplates->editItem(item);
    //TODO: changer le nom du fichier template
}

void OptionsDialog::on_addTemplateButton_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Ajouter des templates", getCurrentPath(), "Template (*.ulk)");
    QStringList::iterator it = files.begin();
    QDir aFile;
    QString currentPath = getCurrentPath();
    QString directory;
    while(it != files.end())
    {
        aFile = QDir(*it);
        directory = aFile.dirName();
        QFile::copy(*it, currentPath + "/" + directory);
        ++it;
    }
    refreshTemplatesList();
}

void OptionsDialog::on_deleteTemplateButton_clicked()
{
    QListWidgetItem* currentItem = ui->listOfTemplates->currentItem();
    if(NULL != currentItem)
    {
        int r = QMessageBox::warning(this, "Suppression", QString::fromUtf8(tr(" <center> Attention </center> <br/>" "Cette action supprimera le template du répertoire <br/><br/>" "Êtes-vous sur de vouloir supprimer ce template?").toStdString().c_str()), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No);
        if(r == QMessageBox::Yes)
        {
            QString baseName = currentItem->text();
            baseName += ".ulk";
            baseName = getCurrentPath() + "/" + baseName;
            qDebug() << baseName;
            QDir(baseName).remove(baseName);
            refreshTemplatesList();
        }
    }
}

void OptionsDialog::on_listOfTemplates_itemClicked(QListWidgetItem *item)
{
    ui->deleteTemplateButton->setEnabled(true);
    List* list = NULL;
    QString baseName = item->text() + ".ulk";
    list = cont_->loadTemplate(baseName, list);
    cont_->createVueApercuTemplate(ui->templatePreview, list);
}
