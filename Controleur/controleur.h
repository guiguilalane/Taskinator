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
    void removeElement(QTreeWidget *t);
    void upElement(QTreeWidget * t);
    void downElement(QTreeWidget * t);

private:
    List * root_;
};

#endif // CONTROLEUR_H
