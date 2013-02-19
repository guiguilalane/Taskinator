#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "element.h"

#include <QStandardItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    boutonAnnulerActif_ = true;
    newList_ = NULL;
    ui->setupUi(this);
    settings_ = new QSettings("kiwiCorporation", "Taskinator");
    //TODO: faire une fenêtre d'option qui permettra de changer le répertoire des templates
//    settings_->setValue("templateDirectory", "/home/guillaume/Bureau/template/");

    modifiedElementSignalMapper_ = new QSignalMapper(this);
    deletedElementSignalMapper_ = new QSignalMapper(this);
    checkboxStateChangeSignalMapper_ = new QSignalMapper(this);

    cont_ = new Controleur(this, modifiedElementSignalMapper_, deletedElementSignalMapper_, checkboxStateChangeSignalMapper_);
    cont_->setTemplateDirectory(settings_->value("templateDirectory").toString());

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
}

void MainWindow::askSaveFile()
{
    int r = QMessageBox::warning(this, "Enregistrement", tr(" <center> Attention </center> <br/>" "Cette action remplacera la liste en cours <br/><br/>" "Voulez-vous enregistrer votre liste ?"), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No);
    if (r == QMessageBox::Yes){
        ui->actionEnregistrer->trigger();
    }
}

void MainWindow::AfterCreatedWindow()
{
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
        ui->radioButton_N->blockSignals(true);
        ui->radioButton_Y->blockSignals(true);
        cont_->refreshTitle(ui->lineEdit, ui->dateEdit, ui->radioButton_Y, ui->radioButton_N);
        ui->radioButton_Y->blockSignals(false);
        ui->radioButton_N->blockSignals(false);
    }
    opDial_ = new OptionsDialog(settings_, cont_, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNouveau_triggered()
{
    if (cont_->getFileModified_()){
        askSaveFile();
    }
    QDir templateDirectory(cont_->getTemplateDirectory());
    QStringList filters;
    filters << "*.ulk";
    templateDirectory.setNameFilters(filters);
    if(NULL != newList_)
    {
        delete newList_;
    }
    newList_ = new NewList(boutonAnnulerActif_, templateDirectory.entryList(), this);
    newList_->show();
    QObject::connect(newList_,SIGNAL(createList(QString, bool, QString, QDateTime)),this,SLOT(createList(QString, bool, QString, QDateTime)));
    boutonAnnulerActif_ = true;
}

void MainWindow::on_actionOuvrir_triggered()
{
    if (cont_->getFileModified_()){
        askSaveFile();
    }
    QString file = QFileDialog::getOpenFileName(this, "Enregistrer sous ...", QString(), "Taskinator (*.tor)");
    if (file != ""){
        cont_->openFile(file);
        cont_->refreshVue(ui->listTree);
        cont_->refreshTitle(ui->lineEdit, ui->dateEdit, ui->radioButton_Y, ui->radioButton_N);
    }
}

void MainWindow::on_actionEnregistrer_sous_triggered()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer sous ...", QString(), "Taskinator (*.tor)");
    cont_->saveFileOn(fichier);
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
    askSaveFile();
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
    QTreeWidgetItem* changedItem = cont_->getElement(key);
    Element* changedElement = (Element*) ui->listTree->itemWidget(changedItem, 0);
    QModelIndex changedMIndex = ui->listTree->getIndexFromItem(changedItem);
    cont_->updateModel(&changedMIndex, changedElement->getValueName_(), QDateTime(changedElement->getValueDate_()), changedElement->getValueCheck_() > 0);
}

void MainWindow::elementDeleted(int key)
{
    QTreeWidgetItem* deletedItem = cont_->getElement(key);
    if(NULL != deletedItem)
    {
        ui->listTree->blockSignals(true);
        if (cont_->isListOrSortedListFromItem(ui->listTree, deletedItem))
        {
            int r = QMessageBox::warning(this, "Suppression", QString::fromUtf8(tr(" <center> Attention </center> <br/>" "Vous allez supprimer une liste de tâche. Cette opération supprimera toutes les sous-listes ou tâches. <br/><br/>" "Êtes-vous sûr de vouloir continuer ?").toStdString().c_str()), QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
            if (r == QMessageBox::Yes){
                cont_->removeElementFromItem(ui->listTree, deletedItem);
            }
        }
        else
        {
            cont_->removeElementFromItem(ui->listTree, deletedItem);
        }
        ui->listTree->blockSignals(false);
        ui->listTree->onItemSelectionChanged();
    }
}

void MainWindow::checkboxStateChanged(int key)
{
    ui->listTree->blockSignals(true);
    ui->listTree->setAnimated(false);
    QTreeWidgetItem* checkedStatedChangedItem = cont_->getElement(key);
    Element* e = (Element*) ui->listTree->itemWidget(checkedStatedChangedItem, 0);
    bool state = e->getValueCheck_();
    QModelIndex checkedStateMIndex = ui->listTree->getIndexFromItem(checkedStatedChangedItem);
    cont_->updateModel(&checkedStateMIndex, e->getValueName_(), QDateTime(e->getValueDate_()), e->getValueCheck_() > 0);
    QTreeWidgetItem* parent = checkedStatedChangedItem->parent();
    if(parent == 0)
    {
        if(ui->radioButton_Y->isChecked())
        {//sortedList
            std::cout << "modifier le state des task seulement" << std::endl;
            //dans le cas d'une liste ordonnée, récupérer le prochain élément non checké qui ne soit pas une liste
            //doit correspondre à un task
            QTreeWidgetItem* nextItemCheckable = getNextItemCheckable(checkedStatedChangedItem);
            Element* e = (Element*) ui->listTree->itemWidget(nextItemCheckable, 0);
            e->setCheckable(state);
        }
    }
    else
    {
        std::cout << "récupérer la liste complète des enfants" << std::endl;
    }
    /*idée d'algorithme pour changer la checkabilité des éléments :
     *  - peut-être créer un signalMapper pour cet algorithme
     *  - récupérer le QTreeWidgetItem "parent" de celui qu'on a modifié
     *  - si c'est le root on ne fait rien
     *  - sinon on récupère la liste complete des enfants de "parent"
     *  - en fonction du type de liste qu'est "parent" on modifie l'état de la checkbox à changer
     *  - si la checkbox cochée correspond à la dernière sous-(tâche/liste) cochée, cochée la checkbox de "parent"
     *  - si la checkbox de "parent" à été modifier, rappeler cet algorithme sur le QTreeWidgetItem "parent"
     */
    ui->listTree->setAnimated(false);
    ui->listTree->blockSignals(false);
}

QTreeWidgetItem *MainWindow::getNextItemCheckable(QTreeWidgetItem *checkedStatedChangedItem)
{
    bool b = false;
    QTreeWidgetItem* current = checkedStatedChangedItem;
    QTreeWidgetItem* toReturn = current;
    QTreeWidgetItem* lastElement = getLastElement(checkedStatedChangedItem);
    while(current != lastElement)
    {
        if(itemHasChild(current))
        {
            toReturn = current;
            b = true;
        }
        current = ui->listTree->itemBelow(current);
    }
    return current;
}

QTreeWidgetItem *MainWindow::getChild(QTreeWidgetItem *parent, QTreeWidgetItem *lastElement)
{
    bool b = false;
    QTreeWidgetItem* current = parent;
    while(current != lastElement && !b)
    {
        if(itemHasChild(current))
        {
            current = getChild(current, lastElement);
            b = true;
        }
        if(!b){
            current = ui->listTree->itemBelow(current);
        }
    }
    return current;
}

bool MainWindow::itemHasChild(QTreeWidgetItem *item)
{
    bool b = false;
    if(item->childCount() > 0)
    {
        if(dynamic_cast<Element*>(ui->listTree->itemWidget(item->child(0), 0)))
        {
            b = true;
        }
    }
    return b;
}

QTreeWidgetItem *MainWindow::getLastElement(QTreeWidgetItem *twi)
{
    QTreeWidgetItem* item = twi;
    QTreeWidgetItem* nextItem = ui->listTree->itemBelow(item);
    while(nextItem != 0)
    {
        if(dynamic_cast<Element*>(ui->listTree->itemWidget(nextItem, 0)))
        {
            item = nextItem;
        }
        nextItem = ui->listTree->itemBelow(nextItem);
    }
    return item;
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
    QMessageBox::about(this,"About Taskinator",QString::fromUtf8(tr("<center> <b> Taskinator </b> </center> <br/><br/> " "Créer par Guillaume COUTABLE et Noémie RULLIER. <br/><br/> <img src=\"://Pictures/Copyright.png\"> Copyright KiwiCorporation ").toStdString().c_str()));
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

void MainWindow::createList(QString templatePath, bool liste, QString name, QDateTime date)
{
    if(templatePath=="")
    {
        if (liste){
            cont_->createList(name.toStdString(), date.toTime_t());
            ui->radioButton_N->setChecked(true);
        }
        else {
            cont_->createSortedList(name.toStdString(), date.toTime_t());
            ui->radioButton_Y->setChecked(true);
        }
    }
    else
    {
        cont_->setRoot_(cont_->loadTemplate(templatePath, cont_->getRoot_()));
    }
    ui->lineEdit->setText(name);
    on_lineEdit_editingFinished();
    ui->dateEdit->setDate(date.date());
    on_dateEdit_editingFinished();

    cont_->refreshVue(ui->listTree);
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

void MainWindow::on_actionOption_triggered()
{
    opDial_->show();
}
