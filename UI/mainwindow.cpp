#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"

#include <QStandardItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //in developpement
    signalMapper_ = new QSignalMapper(this);
    //end in developpement

    cont_ = new Controleur(this, signalMapper_);
    // TODO A supprimer
    cont_->createList();
    ui->setupUi(this);
    QLabel * vide = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    QTreeWidgetItem* videItem = new QTreeWidgetItem(ui->listTree);
    ui->listTree->setItemWidget(videItem,0,vide);

    QMenu * menuParam = new QMenu();
    QAction * liste = new QAction("Liste non ordonnée",0);
    liste->setCheckable(true);
    QAction * listeO = new QAction("Liste ordonnée",0);
    listeO->setCheckable(true);
    QAction * tache = new QAction("Tâche",0);
    tache->setCheckable(true);
    menuParam->addAction(liste);
    menuParam->addAction(listeO);
    menuParam->addAction(tache);
    ui->toolButtonParam->setMenu(menuParam);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNouveau_triggered()
{
    newList_ = new NewList();
    newList_->show();
}

void MainWindow::on_actionOuvrir_triggered()
{
    QFileDialog * filediag = new QFileDialog();
    filediag->getOpenFileNames();
}

void MainWindow::on_actionEnregistrer_sous_triggered()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer sous ...", QString(), "Taskinator (*.tor)");
}

void MainWindow::on_actionQuitter_triggered()
{
    close();
}

void MainWindow::on_toolButtonList_clicked()
{
    cont_->addList(ui->listTree);
}

void MainWindow::on_toolButtonListOrdered_clicked()
{
    cont_->addSortedList(ui->listTree);
}

void MainWindow::on_toolButtonTask_clicked()
{
    cont_->addTask(ui->listTree);
}

void MainWindow::on_toolButtonTrash_clicked()
{
    cont_->removeElement(ui->listTree);
}

void MainWindow::on_toolButtonUp_clicked()
{
    cont_->upElement(ui->listTree);
}

void MainWindow::on_toolButtonDown_clicked()
{
    cont_->downElement(ui->listTree);
}

//void MainWindow::elementChanged(QWidget* elem)
//{
//    std::cout << ((Element*)elem)->getName().toStdString() << " " << ((Element*)elem)->getDate().toStdString() << std::endl;
//}

void MainWindow::elementChanged(int key)
{
    QTreeWidgetItem* changedItem = cont_->getElement(key);
    Element* changedElement = (Element*) ui->listTree->itemWidget(changedItem, 0);
    QModelIndex changedMIndex = ui->listTree->getIndexFromItem(changedItem);
    cont_->updateModel(&changedMIndex, changedElement->getName(), QDateTime(QDate::fromString(changedElement->getDate())), changedElement->getState());
    //    std::cout << key << " " << cont_->getElement(key)->getName().toStdString() << " " << cont_->getElement(key)->getDate().toStdString() << std::endl;
}
