#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "../Model/component.h"
#include "../Model/sortedlist.h"
#include "../Model/list.h"
#include "../Model/task.h"
#include "../UI/element.h"
#include <QModelIndex>
#include <QTreeWidget>
#include <vector>

class Controleur
{
public:
    Controleur();
    void createList();
    void createSortedList();
    std::vector<int> calculateArborescence(QModelIndex m);
    void addList(QTreeWidget *t);
    void addSortedList(QTreeWidget * t);
    void addTask(QTreeWidget *t);
    void refreshVue(QTreeWidget * t);
    void parcoursList(QTreeWidget * t, QTreeWidgetItem * p, List* parent);
    void parcoursListModele(List * parent, List *nouvelle);
    void removeElement(QTreeWidget *t);
    void upElement(QTreeWidget * t);
    void downElement(QTreeWidget * t);
    void valueChange(QTreeWidget t);
    void toList(QTreeWidget *t);
    void toSortedList(QTreeWidget *t);
    void toTask(QTreeWidget *t);

private:
    List * root_;
};

#endif // CONTROLEUR_H
