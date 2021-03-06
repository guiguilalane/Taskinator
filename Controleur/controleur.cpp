#include "controleur.h"

Controleur::Controleur(QMainWindow *mainW, QSignalMapper *signalM, QSignalMapper *signalD, QSignalMapper *signalC):
    mainWindow_(mainW), modifiedElementSignalMapper_(signalM),
    deletedElementSignalMapper_(signalD),
    checkboxStateChangeSignalMapper_(signalC)
{
    elements_ = new QHash<int, QTreeWidgetItem*>();
    xmlOp_ = new XMLOperation();
    filePath_ = "";
    fileModified_ = false;
    root_ = NULL;
}

void Controleur::createList(const std::string& name, time_t date)
{
    if (root_ != NULL){
        delete root_;
    }
    root_ = new List(name, date);
    fileModified_ = true;
}

void Controleur::createSortedList(const std::string& name, time_t date)
{
    if (root_ != NULL){
        delete root_;
    }
    root_ = new SortedList(name, date);
    fileModified_ = true;
}

std::vector<int> Controleur::calculateArborescence(QModelIndex m)
{
    std::vector<int> tab;
    m=m.parent();
    while (m != QModelIndex()){
        tab.push_back(m.row());
        m = m.parent();
    }
    return tab;
}

void Controleur::parcoursArbre(QTreeWidget *p, QTreeWidget *tw)
{
    for (int i = 0; i < p->topLevelItemCount(); i++)
    {
        parcoursArbreRecursif(p->topLevelItem(i), tw->topLevelItem(i));
    }
}

void Controleur::parcoursArbreRecursif(QTreeWidgetItem *itemP, QTreeWidgetItem *item)
{
    if (item != 0){
        item->setExpanded(itemP->isExpanded());
        for (int i = 0; i < itemP->childCount(); i++)
        {
            parcoursArbreRecursif(itemP->child(i), item->child(i));
        }
    }
}

void Controleur::refreshVue(QTreeWidget * t)
{
    MyTreeWidget * old = new MyTreeWidget( (MyTreeWidget *) t);
    t->blockSignals(true);
    t->clear();
    elements_->clear();
    asup_.clear();
    parcoursList(t, 0, root_);
    parcoursArbre(old, t);
    delete old;
    t->blockSignals(false);
    QObject::connect(modifiedElementSignalMapper_, SIGNAL(mapped(int)), mainWindow_, SLOT(elementChanged(int)));
    QObject::connect(deletedElementSignalMapper_, SIGNAL(mapped(int)), mainWindow_, SLOT(elementDeleted(int)));
    QObject::connect(checkboxStateChangeSignalMapper_, SIGNAL(mapped(int)), mainWindow_, SLOT(checkboxStateChanged(int)));
}

void Controleur::parcoursList(QTreeWidget * t, QTreeWidgetItem * p, List* parent)
{
    int taille = parent->getTabComponent_().size();
    QTreeWidgetItem* elementItem = 0;
    Element * element;
    Component* component;
    for (int i = 1; i <= taille; ++i){
        if (parent == root_){
            elementItem = new QTreeWidgetItem(t);
        }
        else {
            elementItem = new QTreeWidgetItem(p);
        }
        component = parent->getTabComponent_()[i];
        element = new Element();
        if(!component->getName_().empty())
        {
            element->setValueName_(QString::fromUtf8(component->getName_().c_str()));
            element->setValueDate_(QDateTime::fromTime_t(component->getDate_()).date());
        }
    element->setValueCheck_(component->getState_());

        bool b = true;
        b = component->checkedPreviousTask();
        element->setCheckable(b);

        int hash = qHash(element);
        elements_->insert(hash, elementItem);
        asup_.push_back(element);
        QObject::connect(element, SIGNAL(elementChanged()), modifiedElementSignalMapper_, SLOT(map()));
        QObject::connect(element, SIGNAL(elementDeleted()), deletedElementSignalMapper_, SLOT(map()));
        QObject::connect(element, SIGNAL(elementCheckChanged()), checkboxStateChangeSignalMapper_, SLOT(map()));
        modifiedElementSignalMapper_->setMapping(element, hash);
        deletedElementSignalMapper_->setMapping(element, hash);
        checkboxStateChangeSignalMapper_->setMapping(element, hash);
        // Si on trouve une liste ordonnée
        if (dynamic_cast<SortedList *>(parent->getTabComponent_()[i])){
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursList(t, elementItem, (SortedList*) parent->getTabComponent_()[i]);
        }
        // Si on trouve une liste
        else if(dynamic_cast<List *>(parent->getTabComponent_()[i])) {
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursList(t, elementItem, (List*)parent->getTabComponent_()[i]);
        }
        // Si on trouve une tâche
        else {
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
        }
    }
    if (parent == root_){
        elementItem = new QTreeWidgetItem(t);
    }
    else {
        elementItem = new QTreeWidgetItem(p);
    }
    QLabel * videParent = new QLabel(QString::fromUtf8("Sélectionner la ligne et créer une liste ou tâche"));
    t->setItemWidget(elementItem,0,videParent);
}

void Controleur::parcoursListModele(List * parent, List * nouvelle)
{
    int taille = parent->getTabComponent_().size();
    for (int i = 1; i <= taille; ++i){
        nouvelle->addComponent(parent->getTabComponent_()[i]);
    }
}

void Controleur::addList(QTreeWidget * t)
{
    // Ajout au modele
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    lts->addComponent(new List());

    fileModified_ = true;

    // Ajout à l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandItem(w);
}

void Controleur::addSortedList(QTreeWidget * t)
{
    // Ajout au modele
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    lts->addComponent(new SortedList());

    fileModified_ = true;

    // Ajout à l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandItem(w);
}

void Controleur::addTask(QTreeWidget * t)
{
    // Ajout au modele
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    lts->addComponent(new Task());

    fileModified_ = true;

    // Ajout à l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
}

void Controleur::refreshTitle(QLineEdit *lineEdit, QDateEdit *dateEdit, QRadioButton *RBY, QRadioButton *RBN)
{
    lineEdit->setText(QString::fromStdString(root_->getName_()));
    dateEdit->setDate(QDateTime::fromTime_t(root_->getDate_()).date());
    bool b = dynamic_cast<SortedList*>(root_);
    RBY->setChecked(b);
    RBN->setChecked(!b);
}

void Controleur::removeElement(QTreeWidget * t)
{
    // Suppression au modèle
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    lts->removeComponent(m.row()+1);

    fileModified_ = true;

    // Suppression de l'IHM
    deleteCurrentItem(t, arbre, m);
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
}

bool Controleur::isListOrSortedListFromItem(QTreeWidget *t, QTreeWidgetItem *item)
{
    bool res = false;
    QModelIndex m = ((MyTreeWidget*) t)->getIndexFromItem(item);
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    Component * comp = lts->getTabComponent_()[m.row()+1];
    if (dynamic_cast<SortedList *>(comp)){
        res = true;
    }
    else if (dynamic_cast<List *>(comp)){
        res = true;
    }
    return res;
}

void Controleur::removeElementFromItem(QTreeWidget *t, QTreeWidgetItem *item)
{
    QModelIndex m = ((MyTreeWidget*) t)->getIndexFromItem(item);
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    lts->removeComponent(m.row()+1);

    fileModified_ = true;

    // Suppression de l'IHM
    QModelIndex current = ((MyTreeWidget*)t)->getIndexFromItem(t->itemAbove(t->currentItem()));
    deleteCurrentItem(t, arbre, m);
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, current);
    t->setCurrentItem(w, 0);
}

void Controleur::upElement(QTreeWidget * t)
{
    // Modification du modèle
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    lts->getTabComponent_()[m.row()+1]->upComponent();

    fileModified_ = true;

    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(t->itemAbove(w), 0);
}

void Controleur::downElement(QTreeWidget * t)
{
    // Modification du modèle
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    lts->getTabComponent_()[m.row()+1]->downComponent();

    fileModified_ = true;

    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(t->itemBelow(w), 0);
}

void Controleur::toList(QTreeWidget * t)
{
    // Modification du modèle
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    Component * comp = lts->getTabComponent_()[m.row()+1];
    if (dynamic_cast<Task *>(comp)){
        lts->getTabComponent_()[m.row()+1] = new List(comp->getName_(),comp->getDate_(), comp->getState_());
        delete comp;
    }
    else if (dynamic_cast<SortedList *>(comp)){
        lts->getTabComponent_()[m.row()+1] = new List(comp->getName_(),comp->getDate_(), comp->getState_());
        parcoursListModele((SortedList*) comp, (List*) lts->getTabComponent_()[m.row()+1]);
    }

    fileModified_ = true;

    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
}

void Controleur::toSortedList(QTreeWidget * t)
{
    // Modification du modèle
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    Component * comp = lts->getTabComponent_()[m.row()+1];
    if (dynamic_cast<Task *>(comp)){
        lts->getTabComponent_()[m.row()+1] = new SortedList(comp->getName_(),comp->getDate_(), comp->getState_());
        delete comp;
    }
    else if (dynamic_cast<List *>(comp)){
        lts->getTabComponent_()[m.row()+1] = new SortedList(comp->getName_(),comp->getDate_(), comp->getState_());
        parcoursListModele((List*) comp, (SortedList*) lts->getTabComponent_()[m.row()+1]);
    }

    fileModified_ = true;

    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
}

void Controleur::toTask(QTreeWidget * t)
{
    // Modification du modèle
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    Component * comp = lts->getTabComponent_()[m.row()+1];
    lts->getTabComponent_()[m.row()+1] = new Task(comp->getName_(),comp->getDate_(), comp->getState_());
    delete comp;

    fileModified_ = true;

    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
}

void Controleur::saveTemplate(QString nameFile)
{
    QString path = templateDirectory_ + nameFile + ".ulk";
    xmlOp_->createTemplate(path.toStdString(), root_);
}

List* Controleur::loadTemplate(const QString path, List* root)
{
    QString templatePath = templateDirectory_ + "/" + path;
    if(root != NULL)
    {
        fileModified_ = true && (root==root_);
        delete root;
    }
    else {
        fileModified_ = false;
    }
    return xmlOp_->newFileFromTemplate(templatePath.toStdString());
}

void Controleur::saveFileOn(QString path)
{
    filePath_ = path;
    xmlOp_->saveFile(path.toStdString(), root_);
    fileModified_ = false;
}

void Controleur::saveFile()
{
    xmlOp_->saveFile(filePath_.toStdString(), root_);
    fileModified_ = false;
}

void Controleur::openFile(QString path)
{
    filePath_ = path;
    if(root_ != NULL)
    {
        delete root_;
        fileModified_ = true;
    }
    else {
        fileModified_ = false;
    }
    root_ = xmlOp_->readFile(path.toStdString());
}

List *Controleur::getRoot_()
{
    return root_;
}

void Controleur::setRoot_(List *root)
{
    root_ = root;
}

bool Controleur::getFileModified_()
{
    return fileModified_;
}

void Controleur::setFilePath(QString file)
{
    filePath_ = file;
}

void Controleur::updateModel(QModelIndex *mIndex, const QString &name, const QDateTime &date, const bool state)
{
    std::vector<int> arbre = calculateArborescence(*mIndex);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    Component* componentToChange = lts->getTabComponent_()[mIndex->row()+1];
    componentToChange->setName_(name.toStdString());
    componentToChange->setDate_(date.toTime_t());
    componentToChange->setState_(state);
    fileModified_ = true;
}

QString Controleur::getFilePath() const
{
    return filePath_;
}

QString Controleur::getTemplateDirectory() const
{
    return templateDirectory_;
}

void Controleur::setTemplateDirectory(const QString path)
{
    templateDirectory_ = path;
}

QTreeWidgetItem* Controleur::getCurrentItem(QTreeWidget *t, std::vector<int> &arbre, QModelIndex m)
{
    std::vector<int>::reverse_iterator rit;
    QTreeWidgetItem * w;
    if(!arbre.empty())
    {
        rit = arbre.rbegin();
        w = t->topLevelItem(*rit);
        ++rit;
        for (rit; rit != arbre.rend(); ++rit){
            w = w->child((*rit));
        }
        w = w->child(m.row());
    }
    else
    {
        w = t->topLevelItem(m.row());
        for (rit= arbre.rbegin(); rit != arbre.rend(); ++rit){
            w = w->child((*rit));
        }
    }
    return w;
}

void Controleur::deleteCurrentItem(QTreeWidget *t, std::vector<int> &arbre, QModelIndex m)
{
    std::vector<int>::reverse_iterator rit;
    QTreeWidgetItem * w;
    if(!arbre.empty())
    {
        rit = arbre.rbegin();
        w = t->topLevelItem(*rit);
        ++rit;
        for (rit; rit != arbre.rend(); ++rit){
            w = w->child((*rit));
        }
        w->takeChild(m.row());
    }
    else
    {
        w = t->takeTopLevelItem(m.row());
    }
}

QTreeWidgetItem *Controleur::getElement(const int key)
{
    return (*elements_)[key];
}

void Controleur::is(QTreeWidget * t, std::string& type, int& nb)
{
    QModelIndex m = t->currentIndex();
    QWidget * wid = t->itemWidget(t->currentItem(),0);
    if (dynamic_cast<Element *>(wid)){
        std::vector<int> arbre = calculateArborescence(m);
        std::vector<int>::reverse_iterator rit;
        List * lts = root_;
        for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
            lts = (List*) lts->getTabComponent_()[(*rit)+1];
        }
        nb = lts->getTabComponent_().size();
        Component * comp = lts->getTabComponent_()[m.row()+1];
        if (dynamic_cast<SortedList *>(comp)){
            type = "sortedList";
        }
        else if (dynamic_cast<List *>(comp)){
            type = "list";
        }
        else if (dynamic_cast<Task *>(comp)){
            type = "task";
        }
    }
    else {
        type = "vide";
    }
}

bool Controleur::isListOrSortedList(QTreeWidget *t)
{
    bool res = false;
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    Component * comp = lts->getTabComponent_()[m.row()+1];
    if (dynamic_cast<SortedList *>(comp)){
        res = true;
    }
    else if (dynamic_cast<List *>(comp)){
        res = true;
    }
    return res;
}

void Controleur::parcoursListApercu(QTreeWidget *t, QTreeWidgetItem *p, List *parent)
{
    int taille = parent->getTabComponent_().size();
    QTreeWidgetItem* elementItem = 0;
    ElementApercu * element;
    Component* component;
    for (int i = 1; i <= taille; ++i){
        if (parent == root_){
            elementItem = new QTreeWidgetItem(t);
        }
        else {
            elementItem = new QTreeWidgetItem(p);
        }
        component = parent->getTabComponent_()[i];
        element = new ElementApercu();
        if(!component->getName_().empty())
        {
            element->setValueName_(QString::fromUtf8(component->getName_().c_str()));
            element->setValueDate_(QDateTime::fromTime_t(component->getDate_()).date());
        }
        element->setValueCheck_(component->getState_());
        // Si on trouve une liste ordonnée
        if (dynamic_cast<SortedList *>(parent->getTabComponent_()[i])){
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursListApercu(t, elementItem, (SortedList*) parent->getTabComponent_()[i]);
        }
        // Si on trouve une liste
        else if(dynamic_cast<List *>(parent->getTabComponent_()[i])) {
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursListApercu(t, elementItem, (List*)parent->getTabComponent_()[i]);
        }
        // Si on trouve une tâche
        else {
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
        }
    }
}

void Controleur::parcoursListApercuTemplate(QTreeWidget *t, QTreeWidgetItem *p, List *parent, List* root)
{
    int taille = parent->getTabComponent_().size();
    QTreeWidgetItem* elementItem = 0;
    ElementApercuTemplate * element;
    Component* component;
    for (int i = 1; i <= taille; ++i){
        if (parent == root){
            elementItem = new QTreeWidgetItem(t);
        }
        else {
            elementItem = new QTreeWidgetItem(p);
        }
        component = parent->getTabComponent_()[i];
        element = new ElementApercuTemplate();
        if(!component->getName_().empty())
        {
            element->setValueName_(QString::fromUtf8(component->getName_().c_str()));
        }
        // Si on trouve une liste ordonnée
        if (dynamic_cast<SortedList *>(parent->getTabComponent_()[i])){
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursListApercuTemplate(t, elementItem, (SortedList*) parent->getTabComponent_()[i], root);
        }
        // Si on trouve une liste
        else if(dynamic_cast<List *>(parent->getTabComponent_()[i])) {
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursListApercuTemplate(t, elementItem, (List*)parent->getTabComponent_()[i], root);
        }
        // Si on trouve une tâche
        else {
            if (dynamic_cast<SortedList *>(parent)){
                element->setValueType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->setValueType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
        }
    }
}

void Controleur::createVueApercu(QTreeWidget *t)
{
    t->clear();
    parcoursListApercu(t, 0, root_);
    t->expandAll();
}

void Controleur::createVueApercuTemplate(QTreeWidget *t, List* root)
{
    t->clear();
    parcoursListApercuTemplate(t, 0, root, root);
    t->expandAll();
}

bool Controleur::rootIsSortedList()
{
    bool res = false;
    if (dynamic_cast<SortedList*> (root_)){
        res = true;
    }
    return res;
}

void Controleur::sortedListToList(QTreeWidget *t)
{
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }
    // Modification dans le modèle
    List * comp = root_;
    root_ = new List(comp->getName_(),comp->getDate_(), comp->getState_());
    parcoursListModele((SortedList*) comp, (List*) root_);

    fileModified_ = true;

    // Modification IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
}

void Controleur::listToSortedList(QTreeWidget *t)
{
    QModelIndex m = t->currentIndex();
    std::vector<int> arbre = calculateArborescence(m);
    std::vector<int>::reverse_iterator rit;
    List * lts = root_;
    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
        lts = (List*) lts->getTabComponent_()[(*rit)+1];
    }

    fileModified_ = true;

    // Modification dans le modèle
    List * comp = root_;
    root_ = new SortedList(comp->getName_(),comp->getDate_(), comp->getState_());
    parcoursListModele(comp, (SortedList*) root_);
    // Modification IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
}
