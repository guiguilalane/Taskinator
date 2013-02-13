#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "../Model/component.h"
#include "../Model/sortedList.h"
#include "../Model/list.h"
#include "../Model/task.h"
#include "../UI/element.h"
#include <QMainWindow>
#include <QModelIndex>
#include <QTreeWidget>
#include <QSignalMapper>
#include <QHash>
#include <vector>

class Controleur
{
public:
    Controleur(QMainWindow *mainW, QSignalMapper *signalM);
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
    void valueChange(QTreeWidget t);
    void updateModel(QModelIndex* mIndex, const QString& name, const QDateTime &date, const bool state);

    QTreeWidgetItem* getElement(const int key);

private:
    List * root_;
    QSignalMapper* signalMapper_;
    QMainWindow * mainWindow_;
    QVector<Element*> asup_;
    QHash<int, QTreeWidgetItem*> *elements_;

};

#endif // CONTROLEUR_H
