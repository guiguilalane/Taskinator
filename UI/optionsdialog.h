#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include <QListWidgetItem>
#include <Controleur/controleur.h>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OptionsDialog(QSettings* set, Controleur* cont, QWidget *parent = 0);
    ~OptionsDialog();
    
private slots:
    void on_templatePathButton_clicked();

    void on_listOfTemplates_itemDoubleClicked(QListWidgetItem *item);

    void on_addTemplateButton_clicked();

    void on_deleteTemplateButton_clicked();

    void on_listOfTemplates_itemClicked(QListWidgetItem *item);

private:
    Ui::OptionsDialog *ui;

    QSettings* settings_;

    Controleur* cont_;

    void refreshTemplatesList();

    void clearTemplatesList();

    QString getCurrentPath();
};

#endif // OPTIONSDIALOG_H
