#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "../Model/component.h"
#include "../Model/sortedList.h"
#include "../Model/list.h"
#include "../Model/task.h"
#include "../UI/element.h"
#include "../UI/mytreewidget.h"
#include "../UI/elementapercu.h"
#include "xmloperation.h"
#include "componentfactory.h"
#include <QMainWindow>
#include <QModelIndex>
#include <QTreeWidget>
#include <QRadioButton>
#include <QSignalMapper>
#include <QHash>
#include <vector>


//TODO: remplacer les création de composant par un appel à la factory
class Controleur
{
public:
    Controleur(QMainWindow *mainW, QSignalMapper *signalM);
    void createList(const std::string &name, time_t date);
    void createSortedList(const std::string &name, time_t date);
    std::vector<int> calculateArborescence(QModelIndex m);
    void addList(QTreeWidget *t);
    void addSortedList(QTreeWidget * t);
    void addTask(QTreeWidget *t);
    void refreshTitle(QLineEdit* lineEdit, QDateEdit* dateEdit, QRadioButton* RBY, QRadioButton* RBN);
    void refreshVue(QTreeWidget * t);
    void parcoursList(QTreeWidget * t, QTreeWidgetItem * p, List* parent);
    void parcoursListModele(List * parent, List *nouvelle);
    void removeElement(QTreeWidget *t);
    void upElement(QTreeWidget * t);
    void downElement(QTreeWidget * t);
    void updateModel(QModelIndex* mIndex, const QString& name, const QDateTime &date, const bool state);
    QString getFilePath() const;
    QString getTemplateDirectory() const;
    void setTemplateDirectory(const QString path);
    void is(QTreeWidget *t, std::string &type, int &nb);
    bool isListOrSortedList(QTreeWidget * t);
    void parcoursListApercu(QTreeWidget * t, QTreeWidgetItem * p, List* parent);
    void createVueApercu(QTreeWidget * t);
    bool rootIsSortedList();
    void sortedListToList(QTreeWidget * t);
    void listToSortedList(QTreeWidget * t);


    QTreeWidgetItem* getElement(const int key);
    void toList(QTreeWidget *t);
    void toSortedList(QTreeWidget *t);
    void toTask(QTreeWidget *t);

    void saveFileOn(QString path);
    void saveFile();

    void openFile(QString path);
    List * getRoot_();
    bool getFileModified_();

    void saveTemplate(QString nameFile);
    void loadTemplate(const QString path);
private:
    List * root_;
    QSignalMapper* signalMapper_;
    QMainWindow * mainWindow_;
    QVector<Element*> asup_;
    QHash<int, QTreeWidgetItem*> *elements_;

    XMLOperation* xmlOp_;
    QString filePath_;
    QString templateDirectory_;
    bool fileModified_;

    QTreeWidgetItem* getCurrentItem(QTreeWidget* t, std::vector<int> &arbre, QModelIndex m);

};

#endif // CONTROLEUR_H
