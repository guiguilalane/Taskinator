#ifndef NEWLIST_H
#define NEWLIST_H

#include <iostream>
#include <QDialog>
#include <QDateTime>
#include <QPushButton>

namespace Ui {
class NewList;
}

class NewList : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewList(bool boutonAnnulerActif, QStringList templates, QWidget *parent = 0);
    ~NewList();

signals:

    void createList(QString templatePath, bool liste, QString name, QDateTime date);
    
private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::NewList *ui;
};

#endif // NEWLIST_H
