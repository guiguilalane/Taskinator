#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"

#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

void MainWindow::on_toolButtonList_clicked()
{
    // On remplace l'élément sélectionné par la nouvelle liste non ordonnée
    QTreeWidgetItem* elementItem = ui->listTree->currentItem();
    Element * element = new Element();
    // TODO A modifier à chercher dans liste ?
    element->changeType_(QString("-"));
    ui->listTree->setItemWidget(elementItem,0,element);

    // On créer un élément vide pour la liste
    QLabel * vide = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    QTreeWidgetItem* videItem = new QTreeWidgetItem(elementItem);
    ui->listTree->setItemWidget(videItem,0,vide);
    elementItem->addChild(videItem);

    // On créer un élément vide pour le parent
    QTreeWidgetItem * elementPItem = ui->listTree->currentItem()->parent();
    QTreeWidgetItem* videParentItem = 0;
    if (elementPItem == 0){
        videParentItem = new QTreeWidgetItem(ui->listTree);
    }
    else {
        videParentItem = new QTreeWidgetItem(elementPItem);
    }
    QLabel * videParent = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    ui->listTree->setItemWidget(videParentItem,0,videParent);
}

void MainWindow::on_toolButtonListOrdered_clicked()
{
    // On remplace l'élément sélectionné par la nouvelle liste non ordonnée
    QTreeWidgetItem* elementItem = ui->listTree->currentItem();
    Element * element = new Element();
    // TODO A modifier pour récupérer le num de la liste
    element->changeType_(QString("-"));
    ui->listTree->setItemWidget(elementItem,0,element);

    // On créer un élément vide pour la liste
    QLabel * vide = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    QTreeWidgetItem* videItem = new QTreeWidgetItem(elementItem);
    ui->listTree->setItemWidget(videItem,0,vide);
    elementItem->addChild(videItem);

    // On créer un élément vide pour le parent
    QTreeWidgetItem * elementPItem = ui->listTree->currentItem()->parent();
    QTreeWidgetItem* videParentItem = 0;
    if (elementPItem == 0){
        videParentItem = new QTreeWidgetItem(ui->listTree);
    }
    else {
        videParentItem = new QTreeWidgetItem(elementPItem);
    }
    QLabel * videParent = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    ui->listTree->setItemWidget(videParentItem,0,videParent);
}

void MainWindow::on_toolButtonTask_clicked()
{
    // On remplace l'élément sélectionné par la nouvelle liste non ordonnée
    QTreeWidgetItem* elementItem = ui->listTree->currentItem();
    Element * element = new Element();
    // TODO A modifier pour récupérer le num de la liste pour les enfants
    element->changeType_(QString("-"));
    ui->listTree->setItemWidget(elementItem,0,element);

    // On créer un élément vide pour le parent
    QTreeWidgetItem * elementPItem = ui->listTree->currentItem()->parent();
    QTreeWidgetItem* videParentItem = 0;
    if (elementPItem == 0){
        videParentItem = new QTreeWidgetItem(ui->listTree);
    }
    else {
        videParentItem = new QTreeWidgetItem(elementPItem);
    }
    QLabel * videParent = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    ui->listTree->setItemWidget(videParentItem,0,videParent);
}

void MainWindow::on_toolButtonTrash_clicked()
{
    QTreeWidgetItem* elementItem = ui->listTree->currentItem();
    ui->listTree->removeItemWidget(elementItem,0);
}

void MainWindow::on_actionQuitter_triggered()
{
    close();
}

void MainWindow::on_actionEnregistrer_sous_triggered()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer sous ...", QString(), "Taskinator (*.tor)");
}

void MainWindow::on_toolButtonUp_clicked()
{

}

void MainWindow::on_toolButtonDown_clicked()
{

}
