#include "controleur.h"

#include <QDebug>

Controleur::Controleur(QMainWindow *mainW, QSignalMapper *signalM): mainWindow_(mainW), signalMapper_(signalM)
{
    elements_ = new QHash<int, QTreeWidgetItem*>();
}

void Controleur::createList()
{
    root_ = new List();
}

void Controleur::createSortedList()
{
    root_ = new SortedList();
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
    t->clear();
    elements_->clear();
    asup_.clear();
    parcoursList(t, 0, root_);
    QObject::connect(signalMapper_, SIGNAL(mapped(int)), mainWindow_, SLOT(elementChanged(int)));
    // TODO A revoir pour garder l'état dans lequel les listes étaient déroulée
    t->expandAll();
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
        int hash = qHash(element);
        elements_->insert(hash, elementItem);
        asup_.push_back(element);
        QObject::connect(element, SIGNAL(nameChanged()), signalMapper_, SLOT(map()));
        signalMapper_->setMapping(element, hash);
        // Si on trouve une liste ordonnée
        if (dynamic_cast<SortedList *>(parent->getTabComponent_()[i])){
            if (dynamic_cast<SortedList *>(parent)){
                element->changeType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->changeType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursList(t, elementItem, (SortedList*) parent->getTabComponent_()[i]);
        }
        // Si on trouve une liste
        else if(dynamic_cast<List *>(parent->getTabComponent_()[i])) {
            if (dynamic_cast<SortedList *>(parent)){
                element->changeType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->changeType_(QString("-"));
            }
            t->setItemWidget(elementItem,0,element);
            parcoursList(t, elementItem, (List*)parent->getTabComponent_()[i]);
        }
        // Si on trouve une tâche
        else {
            if (dynamic_cast<SortedList *>(parent)){
                element->changeType_(QString::number(i)+QString("-"));
            }
            else if (dynamic_cast<List *>(parent)){
                element->changeType_(QString("-"));
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
    QLabel * videParent = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    t->setItemWidget(elementItem,0,videParent);
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
}

// TODO Change type --> Bouton param

void Controleur::valueChange(QTreeWidget t)
{
    // Change les valeurs dans le modèle
//    QModelIndex m = t->currentIndex();
//    std::vector<int> arbre = calculateArborescence(m);
//    std::vector<int>::reverse_iterator rit;
//    List * lts = root_;
//    for (rit = arbre.rbegin(); rit != arbre.rend(); ++rit){
//        lts = (List*) lts->getTabComponent_()[(*rit)+1];
//    }
//        lts->getTabComponent_()[m.row()+1].setName_(t.currentItem()->);
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
    root_->affichage(std::cout);
}

QTreeWidgetItem *Controleur::getElement(const int key)
{
    return (*elements_)[key];
}
