#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"

#include <QStandardItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //encodage en UTF8
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    boutonAnnulerActif_ = true;
    ui->setupUi(this);
    settings_ = new QSettings("kiwiCorporation", "Taskinator");

    //in developpement
    signalMapper_ = new QSignalMapper(this);
    //end in developpement

    cont_ = new Controleur(this, signalMapper_);

    // Ajout de la première ligne vide dans la vue
    QLabel * vide = new QLabel(QString::fromUtf8("Sélectionner la ligne et créer une liste ou tâche"));
    QTreeWidgetItem* videItem = new QTreeWidgetItem(ui->listTree);
    ui->listTree->setItemWidget(videItem,0,vide);

    // Menu pour le bouton changement de type
    menuParam_ = new QMenu();
    liste_ = new QAction(QString::fromUtf8("Liste non ordonnée"),0);
    liste_->setCheckable(true);
    listeO_ = new QAction(QString::fromUtf8("Liste ordonnée"),0);
    listeO_->setCheckable(true);
    tache_ = new QAction(QString::fromUtf8("Tâche"),0);
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

    // On actionne la fenêtre nouveau si aucun fichier n'est enregistrer ou s'il n'existe plus
    QFile * f = new QFile(settings_->value("lastFile").toString());
    if(!f->exists()){
        boutonAnnulerActif_ = false;
        //TODO: enlever la croix qui permet de fermer la fenetre de dialogue (au moins pour la première ouverture) sous Ubuntu
        ui->actionNouveau->trigger();
    }
    // Sinon ajouter le chargement automatique du fichiers
    else
    {
        QString file = settings_->value("lastFile").toString();
        cont_->openFile(file);
        cont_->refreshVue(ui->listTree);
        cont_->refreshTitle(ui->lineEdit, ui->dateEdit, ui->radioButton_Y, ui->radioButton_N);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNouveau_triggered()
{
    newList_ = new NewList(boutonAnnulerActif_);
    newList_->show();
    QObject::connect(newList_,SIGNAL(createList(bool, QString, QDateTime)),this,SLOT(createList(bool, QString, QDateTime)));
    boutonAnnulerActif_ = true;
}

void MainWindow::on_actionOuvrir_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Enregistrer sous ...", QString(), "Taskinator (*.tor)");
    cont_->openFile(file);
    cont_->refreshVue(ui->listTree);
    cont_->refreshTitle(ui->lineEdit, ui->dateEdit, ui->radioButton_Y, ui->radioButton_N);
}

void MainWindow::on_actionEnregistrer_sous_triggered()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer sous ...", QString(), "Taskinator (*.tor)");
    cont_->saveFileOn(fichier);
    std::cout << fichier.toStdString() << std::endl;
    settings_->setValue("lastFile",fichier);
}

void MainWindow::on_actionEnregistrer_triggered()
{
    if(cont_->getFilePath().isEmpty())
    {
        on_actionEnregistrer_sous_triggered();
    }
    else
    {
        cont_->saveFile();
    }
}

void MainWindow::on_actionEnregistrer_template_triggered()
{
    bool ok;
    QString fichier = QInputDialog::getText(this, QString::fromUtf8(tr("Nom du fichier template").toStdString().c_str()), QString::fromUtf8(tr("Veuillez entrer le nom du template").toStdString().c_str()), QLineEdit::Normal, QString(), &ok);
    if(ok && !fichier.isEmpty())
    {
        cont_->saveTemplate(fichier);
    }
}


void MainWindow::on_actionQuitter_triggered()
{
    close();
}

void MainWindow::on_toolButtonList_clicked()
{
//    ui->listTree->blockSignals(true);
    cont_->addList(ui->listTree);
//    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonListOrdered_clicked()
{
//    ui->listTree->blockSignals(true);
    cont_->addSortedList(ui->listTree);
//    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonTask_clicked()
{
//    ui->listTree->blockSignals(true);
    cont_->addTask(ui->listTree);
//    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonTrash_clicked()
{
    ui->listTree->blockSignals(true);
    if (cont_->isListOrSortedList(ui->listTree))
    {
        int r = QMessageBox::warning(this, "Suppression", QString::fromUtf8(tr(" <center> Attention </center> <br/>" "Vous allez supprimer une liste de tâche. Cette opération supprimera toutes les sous-listes ou tâches. <br/><br/>" "Êtes-vous sûr de vouloir continuer ?").toStdString().c_str()), QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
        if (r == QMessageBox::Yes){
            cont_->removeElement(ui->listTree);
        }
    }
    else
    {
        cont_->removeElement(ui->listTree);
    }
    ui->listTree->blockSignals(false);
    ui->listTree->onItemSelectionChanged();
}

void MainWindow::on_toolButtonUp_clicked()
{
//    ui->listTree->blockSignals(true);
    cont_->upElement(ui->listTree);
//    ui->listTree->blockSignals(false);
}

void MainWindow::on_toolButtonDown_clicked()
{
//    ui->listTree->blockSignals(true);
    cont_->downElement(ui->listTree);
//    ui->listTree->blockSignals(false);
}

void MainWindow::elementChanged(int key)
{
    //FIXME: forcer l'actualisation des champs d'un item(s'ils sont en édition) lors de l'utilisation d'un bouton de la barre d'outils
    QTreeWidgetItem* changedItem = cont_->getElement(key);
    Element* changedElement = (Element*) ui->listTree->itemWidget(changedItem, 0);
    QModelIndex changedMIndex = ui->listTree->getIndexFromItem(changedItem);
    cont_->updateModel(&changedMIndex, changedElement->getValueName_(), QDateTime(changedElement->getValueDate_()), changedElement->getValueCheck_() > 0);
}

void MainWindow::toolButtonParam_toList(bool b)
{
    ui->listTree->blockSignals(true);
    if (b)
    {
        cont_->toList(ui->listTree);
        listeO_->setChecked(false);
        tache_->setChecked(false);
    }
    ui->listTree->blockSignals(false);
}

void MainWindow::toolButtonParam_toSortedList(bool b)
{
    ui->listTree->blockSignals(true);
    if (b)
    {
        cont_->toSortedList(ui->listTree);
        liste_->setChecked(false);
        tache_->setChecked(false);
    }
    ui->listTree->blockSignals(false);
}

void MainWindow::toolButtonParam_toTask(bool b)
{
    ui->listTree->blockSignals(true);
    if (b)
    {
        int r = QMessageBox::warning(this, "Changement de type", QString::fromUtf8(tr("<center> Attention </center> <br/>" "Vous allez changer une liste en tâche. Cette opération supprimera toutes les sous-listes ou tâches. <br/><br/>" "Êtes-vous sûr de vouloir continuer ?").toStdString().c_str()), QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
        if (r == QMessageBox::Yes){
            cont_->toTask(ui->listTree);
            liste_->setChecked(false);
            listeO_->setChecked(false);
        }
        else {
            tache_->setChecked(false);
        }
    }
    ui->listTree->blockSignals(false);
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

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About Taskinator",QString::fromUtf8(tr("<center> <b> Taskinator </b> </center> <br/><br/> " "Créer par Guillaume COUTABLE et Noémie RULLIER").toStdString().c_str()));
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 1){
        // Désactivation des boutons
        ui->toolButtonList->setEnabled(false);
        ui->toolButtonListOrdered->setEnabled(false);
        ui->toolButtonTask->setEnabled(false);
        ui->toolButtonParam->setEnabled(false);
        ui->toolButtonUp->setEnabled(false);
        ui->toolButtonDown->setEnabled(false);
        ui->toolButtonTrash->setEnabled(false);
        cont_->createVueApercu(ui->listTreeAp);
        ui->nameLAp->setText(QString(cont_->getRoot_()->getName_().c_str()));
        ui->dateAp->setText(QDateTime::fromTime_t(cont_->getRoot_()->getDate_()).toString("dd/MM/yyyy"));
        if (cont_->rootIsSortedList()){
            ui->listSorted->setText("Oui");
        }
        else {
            ui->listSorted->setText("Non");
        }
    }
    else if (index == 0){
        ui->listTree->onItemSelectionChanged();
    }
}

void MainWindow::createList(bool liste, QString name, QDateTime date)
{
    if (liste){
        cont_->createList(name.toStdString(), date.toTime_t());
        ui->radioButton_N->setChecked(true);
    }
    else {
        cont_->createSortedList(name.toStdString(), date.toTime_t());
        ui->radioButton_Y->setChecked(true);
    }
    ui->lineEdit->setText(name);
    ui->dateEdit->setDate(date.date());
}

void MainWindow::on_lineEdit_editingFinished()
{
    cont_->getRoot_()->setName_(ui->lineEdit->text().toStdString());
}

void MainWindow::on_dateEdit_editingFinished()
{
    cont_->getRoot_()->setDate_(ui->dateEdit->dateTime().toTime_t());
}

void MainWindow::on_radioButton_Y_toggled(bool checked)
{
    if (checked){
        cont_->listToSortedList(ui->listTree);
    }
}

void MainWindow::on_radioButton_N_toggled(bool checked)
{
    if (checked){
        cont_->sortedListToList(ui->listTree);
    }
}
