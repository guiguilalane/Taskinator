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

    // Menu pour le bouton changement de type
    menuParam_ = new QMenu();
    liste_ = new QAction("Liste non ordonnée",0);
    liste_->setCheckable(true);
    listeO_ = new QAction("Liste ordonnée",0);
    listeO_->setCheckable(true);
    tache_ = new QAction("Tâche",0);
    tache_->setCheckable(true);
    menuParam_->addAction(liste_);
    menuParam_->addAction(listeO_);
    menuParam_->addAction(tache_);
    ui->toolButtonParam->setMenu(menuParam_);
    connect(liste_, SIGNAL(triggered(bool)),this,SLOT(toolButtonParam_toList(bool)));
    connect(listeO_, SIGNAL(triggered(bool)),this,SLOT(toolButtonParam_toSortedList(bool)));
    connect(tache_, SIGNAL(triggered(bool)),this,SLOT(toolButtonParam_toTask(bool)));

    // Désactivation des boutons
    ui->toolButtonList->setEnabled(false);
    ui->toolButtonListOrdered->setEnabled(false);
    ui->toolButtonTask->setEnabled(false);
    ui->toolButtonParam->setEnabled(false);
    ui->toolButtonUp->setEnabled(false);
    ui->toolButtonDown->setEnabled(false);
    ui->toolButtonTrash->setEnabled(false);
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
    cont_->saveFile(fichier);
}

void MainWindow::on_actionQuitter_triggered()
{
    close();
}

void MainWindow::on_toolButtonList_clicked()
{
    ui->listTree->blockSignals(true);
    cont_->addList(ui->listTree);
    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonListOrdered_clicked()
{
    ui->listTree->blockSignals(true);
    cont_->addSortedList(ui->listTree);
    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonTask_clicked()
{
    ui->listTree->blockSignals(true);
    cont_->addTask(ui->listTree);
    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonTrash_clicked()
{
    ui->listTree->blockSignals(true);
    cont_->removeElement(ui->listTree);
    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonUp_clicked()
{
    ui->listTree->blockSignals(true);
    cont_->upElement(ui->listTree);
    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonDown_clicked()
{
    ui->listTree->blockSignals(true);
    cont_->downElement(ui->listTree);
    ui->listTree->blockSignals(false);
}

void MainWindow::elementChanged(int key)
{
    QTreeWidgetItem* changedItem = cont_->getElement(key);
    Element* changedElement = (Element*) ui->listTree->itemWidget(changedItem, 0);
    QModelIndex changedMIndex = ui->listTree->getIndexFromItem(changedItem);
    cont_->updateModel(&changedMIndex, changedElement->getValueName_(), QDateTime(changedElement->getValueDate_()), changedElement->getValueCheck_());
}

void MainWindow::toolButtonParam_toList(bool b)
{
    if (b)
    {
        cont_->toList(ui->listTree);
        listeO_->setChecked(false);
        tache_->setChecked(false);
    }
    else{
        liste_->setChecked(true);
    }
}

void MainWindow::toolButtonParam_toSortedList(bool b)
{
    if (b)
    {
        cont_->toSortedList(ui->listTree);
        liste_->setChecked(false);
        tache_->setChecked(false);
    }
    else{
        listeO_->setChecked(true);
    }
}

void MainWindow::toolButtonParam_toTask(bool b)
{
    if (b)
    {
        cont_->toTask(ui->listTree);
        liste_->setChecked(false);
        listeO_->setChecked(false);
    }
    else{
        tache_->setChecked(true);
    }
}

void MainWindow::on_listTree_itemSelectionChanged()
{
    // Désactivation de tous les boutons
    liste_->setChecked(false);
    listeO_->setChecked(false);
    tache_->setChecked(false);
    ui->toolButtonList->setEnabled(false);
    ui->toolButtonListOrdered->setEnabled(false);
    ui->toolButtonTask->setEnabled(false);
    ui->toolButtonParam->setEnabled(false);
    ui->toolButtonUp->setEnabled(false);
    ui->toolButtonDown->setEnabled(false);
    ui->toolButtonTrash->setEnabled(false);
    // Calcul du type d'élément
    int nb = 0;
    std::string type = "";
    cont_->is(ui->listTree, type, nb);
    // En fonction du type d'élément sélectionné
    if (type == "vide"){
        ui->toolButtonList->setEnabled(true);
        ui->toolButtonListOrdered->setEnabled(true);
        ui->toolButtonTask->setEnabled(true);
    }
    // Si c'est un élément
    else {
        ui->toolButtonTrash->setEnabled(true);
        ui->toolButtonParam->setEnabled(true);
        if (ui->listTree->currentIndex().row() != 0){
            ui->toolButtonUp->setEnabled(true);
        }
        if (ui->listTree->currentIndex().row() != nb-1){
            ui->toolButtonDown->setEnabled(true);
        }
        if (type == "list")
        {
            liste_->setChecked(true);
        }
        else if (type == "sortedList")
        {
            listeO_->setChecked(true);
        }
        else if (type == "task")
        {
            tache_->setChecked(true);
        }
    }
}
