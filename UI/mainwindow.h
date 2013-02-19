#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>
#include <QTextCodec>
#include <QInputDialog>
#include <sstream>


#include <UI/newlist.h>
#include "../Controleur/controleur.h"
#include "mytreewidget.h"
#include "optionsdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void askSaveFile();

    void AfterCreatedWindow();
    QList<QAction *> loadingrecentFile();
    void createMenuRecentFile();

    ~MainWindow();

private slots:
    void on_actionNouveau_triggered();

    void on_actionOuvrir_triggered();

    void on_toolButtonList_clicked();

    void on_toolButtonListOrdered_clicked();

    void on_toolButtonTask_clicked();

    void on_toolButtonTrash_clicked();

    void on_actionQuitter_triggered();

    void on_actionEnregistrer_sous_triggered();

    void on_toolButtonUp_clicked();

    void on_toolButtonDown_clicked();

    void elementChanged(int key);

    void elementDeleted(int key);

    void checkboxStateChanged(int key);

    void toolButtonParam_toList(bool);

    void toolButtonParam_toSortedList(bool b);

    void toolButtonParam_toTask(bool b);

    void on_actionEnregistrer_triggered();

    void on_listTree_itemSelectionChanged();

    void on_actionAbout_triggered();

    void on_tabWidget_currentChanged(int index);

    void createList(QString templatePath, bool liste, QString name, QDateTime date);

    void on_lineEdit_editingFinished();

    void on_dateEdit_editingFinished();

    void on_radioButton_Y_toggled(bool checked);

    void on_radioButton_N_toggled(bool checked);

    void on_actionEnregistrer_template_triggered();

    void on_actionOption_triggered();

    void openFile();

    void clearMenuRecentFile();

private:
    Ui::MainWindow *ui;
    NewList * newList_;
    Controleur * cont_;
    QSignalMapper* modifiedElementSignalMapper_;
    QSignalMapper* deletedElementSignalMapper_;
    QSignalMapper* checkboxStateChangeSignalMapper_;

    QMenu * menuParam_;
    QAction * liste_;
    QAction * listeO_;
    QAction * tache_;
    QMenu * ouvrirRecent_;
    QAction * clear_;
    QSettings * settings_;

    bool boutonAnnulerActif_;

    OptionsDialog* opDial_;

    bool isModified;

};

#endif // MAINWINDOW_H
