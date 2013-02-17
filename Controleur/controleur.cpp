#include "controleur.h"

#include <QDebug>

Controleur::Controleur(QMainWindow *mainW, QSignalMapper *signalM): mainWindow_(mainW), signalMapper_(signalM)
{
    elements_ = new QHash<int, QTreeWidgetItem*>();
    xmlOp_ = new XMLOperation();
    filePath_ = "";
    root_ = NULL;
}

void Controleur::createList(const std::string& name, time_t date)
{
    root_ = new List(name, date);
}

void Controleur::createSortedList(const std::string& name, time_t date)
{
    root_ = new SortedList(name, date);
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

void Controleur::refreshVue(QTreeWidget * t)
{
    t->setAnimated(false);
    t->clear();
    elements_->clear();
    asup_.clear();
    parcoursList(t, 0, root_);
    //FIXME: actuellement, execution du slot elementChanged nb_QTreeWidgetItem fois sur le même objet, même si un seul signal emit
    QObject::connect(signalMapper_, SIGNAL(mapped(int)), mainWindow_, SLOT(elementChanged(int)));
    // TODO A revoir pour garder l'état dans lequel les listes étaient déroulée et éviter de mettre des expandAll() dans toutes les fonctionnalitées qui utilise la méthode refreshView()
}

// TODO: ajouter que lorsque le l'on créer les élément on doit passer en paramètre les valeurs déjà renseignées !!!
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
            element->setValueName_(QString::fromStdString(component->getName_()));
            element->setValueDate_(QDateTime::fromTime_t(component->getDate_()).date());

            //NOTE: sur une liste dépendra de la valeur des composents fils
            element->setValueCheck_(component->getState_());
        }
        int hash = qHash(element);
        elements_->insert(hash, elementItem);
        asup_.push_back(element);
        QObject::connect(element, SIGNAL(elementChanged()), signalMapper_, SLOT(map()));
        signalMapper_->setMapping(element, hash);
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
    // Ajout à l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Ajout à l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Ajout à l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Suppression de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(t->itemAbove(w), 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(t->itemBelow(w), 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Modification de l'IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
}

void Controleur::saveFileOn(QString path)
{
    filePath_ = path;
    xmlOp_->saveFile(path.toStdString(), root_);
}

void Controleur::saveFile()
{
    xmlOp_->saveFile(filePath_.toStdString(), root_);
}

void Controleur::openFile(QString path)
{
    filePath_ = path;
    //TODO: proposer d'enregistrer
    if(root_ != NULL)
    {
        delete root_;
    }
    root_ = xmlOp_->readFile(path.toStdString());
}

List *Controleur::getRoot_()
{
    return root_;
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
}

QString Controleur::getFilePath() const
{
    return filePath_;
}

QTreeWidgetItem* Controleur::getCurrentItem(QTreeWidget *t, std::vector<int> &arbre, QModelIndex m)
{
    std::vector<int>::reverse_iterator rit;
    QTreeWidgetItem * w;
    if(!arbre.empty())
    {
        rit = arbre.rbegin();
        //FIXME: voir pour le cas de premiere création de composant
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
            element->setValueName_(QString::fromStdString(component->getName_()));
            element->setValueDate_(QDateTime::fromTime_t(component->getDate_()).date());
            element->setValueCheck_(component->getState_());
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

void Controleur::createVueApercu(QTreeWidget *t)
{
    t->clear();
    parcoursListApercu(t, 0, root_);
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
    // Modification IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
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
    // Modification dans le modèle
    List * comp = root_;
    root_ = new SortedList(comp->getName_(),comp->getDate_(), comp->getState_());
    parcoursListModele(comp, (SortedList*) root_);
    // Modification IHM
    refreshVue(t);
    QTreeWidgetItem* w = getCurrentItem(t, arbre, m);
    t->setCurrentItem(w, 0);
    t->expandAll();
    t->setAnimated(true);
}
