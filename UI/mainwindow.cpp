#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"

#include <QStandardItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cont_ = new Controleur();
    // TODO A supprimer
    cont_->createList();
    ui->setupUi(this);
    QLabel * vide = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    QTreeWidgetItem* videItem = new QTreeWidgetItem(ui->listTree);
    ui->listTree->setItemWidget(videItem,0,vide);

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
