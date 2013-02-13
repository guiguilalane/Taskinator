#include "controleur.h"

#include <QDebug>

Controleur::Controleur()
{}

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
    parcoursList(t, 0, root_);
    // TODO A revoir pour garder l'état dans lequel les listes étaient déroulée
    t->expandAll();
    t->setAnimated(true);
//    std::cout << t->isAnimated() << std::endl;
}

// TODO: ajouter que lorsuqe le l'on créer les élément on doit passer en paramètre les valeurs déjà renseignées !!!
void Controleur::parcoursList(QTreeWidget * t, QTreeWidgetItem * p, List* parent)
{
    int taille = parent->getTabComponent_().size();
    QTreeWidgetItem* elementItem = 0;
    for (int i = 1; i <= taille; ++i){
        if (parent == root_){
            elementItem = new QTreeWidgetItem(t);
        }
        else {
            elementItem = new QTreeWidgetItem(p);
        }
        // Si on trouve une liste ordonnée
        if (dynamic_cast<SortedList *>(parent->getTabComponent_()[i])){
            Element * element = new Element();
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
            Element * element = new Element();
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
            Element * element = new Element();
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
    QLabel * videParent = new QLabel("Sélectionner la ligne et créer une liste ou tâche");
    t->setItemWidget(elementItem,0,videParent);
}

void Controleur::parcoursListModele(List * parent, List * nouvelle)
{
    int taille = parent->getTabComponent_().size();
    for (int i = 1; i <= taille; ++i){
        if (dynamic_cast<SortedList *>(parent->getTabComponent_()[i])){
            parcoursListModele((SortedList*) parent->getTabComponent_()[i], nouvelle);
        }
        else if(dynamic_cast<List *>(parent->getTabComponent_()[i])) {
            parcoursListModele((List*) parent->getTabComponent_()[i], nouvelle);
        }
        else {
            nouvelle->addComponent(parent->getTabComponent_()[i]);
        }
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
//    QTreeWidgetItem * w = t->topLevelItem(m.row());
//    for (rit= arbre.rbegin(); rit != arbre.rend(); ++rit){
//        w = w->child((*rit));
//    }
//    w->setSelected(true);
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
//    QTreeWidgetItem * w = t->topLevelItem(m.row());
//    for (rit= arbre.rbegin(); rit != arbre.rend(); ++rit){
//        w = w->child((*rit));
//    }
//    w->setSelected(true);
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
//    QTreeWidgetItem * w = t->topLevelItem(m.row());
//    for (rit= arbre.rbegin(); rit != arbre.rend(); ++rit){
//        w = w->child((*rit));
//    }
//    w->setSelected(true);
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
        lts->getTabComponent_()[m.row()+1] = new List(comp->getName_(),comp->getDate_());
        delete comp;
    }
    else if (dynamic_cast<SortedList *>(comp)){
        lts->getTabComponent_()[m.row()+1] = new List();
        parcoursListModele((SortedList*) comp, (List*) lts->getTabComponent_()[m.row()+1]);
    }
    // Modification de l'IHM
    refreshVue(t);
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
        lts->getTabComponent_()[m.row()+1] = new SortedList(comp->getName_(),comp->getDate_());
        delete comp;
    }
    else if (dynamic_cast<List *>(comp)){
        lts->getTabComponent_()[m.row()+1] = new SortedList();
        parcoursListModele((List*) comp, (SortedList*) lts->getTabComponent_()[m.row()+1]);
    }
    // Modification de l'IHM
    refreshVue(t);
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
    lts->getTabComponent_()[m.row()+1] = new Task(comp->getName_(),comp->getDate_());
    delete comp;
    // Modification de l'IHM
    refreshVue(t);
}

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
//    lts->getTabComponent_()[m.row()+1].setName_(t.currentItem()->);
}


