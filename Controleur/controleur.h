#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "../Model/component.h"
#include "../Model/sortedList.h"
#include "../Model/list.h"
#include "../Model/task.h"
#include "../UI/element.h"
#include "../UI/mytreewidget.h"
#include "xmloperation.h"
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
    void parcoursListModele(List * parent, List *nouvelle);
    void removeElement(QTreeWidget *t);
    void upElement(QTreeWidget * t);
    void downElement(QTreeWidget * t);
    void valueChange(QTreeWidget t);
    void updateModel(QModelIndex* mIndex, const QString& name, const QDateTime &date, const bool state);
    QString getFilePath() const;

    QTreeWidgetItem* getElement(const int key);
    void toList(QTreeWidget *t);
    void toSortedList(QTreeWidget *t);
    void toTask(QTreeWidget *t);

    void saveFileOn(QString path);
    void saveFile();

private:
    List * root_;
    QSignalMapper* signalMapper_;
    QMainWindow * mainWindow_;
    QVector<Element*> asup_;
    QHash<int, QTreeWidgetItem*> *elements_;

    XMLOperation* xmlOp_;
    QString filePath_;

    QTreeWidgetItem* getCurrentItem(QTreeWidget* t, std::vector<int>::reverse_iterator &rit, std::vector<int> &arbre, QModelIndex m);

};

#endif // CONTROLEUR_H
