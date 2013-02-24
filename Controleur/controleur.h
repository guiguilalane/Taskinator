#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include "../Model/component.h"
#include "../Model/sortedList.h"
#include "../Model/list.h"
#include "../Model/task.h"
#include "../UI/element.h"
#include "../UI/mytreewidget.h"
#include "../UI/elementapercu.h"
#include "../UI/elementapercutemplate.h"
#include "xmloperation.h"
#include "componentfactory.h"
#include <QMainWindow>
#include <QModelIndex>
#include <QTreeWidget>
#include <QRadioButton>
#include <QSignalMapper>
#include <QHash>
#include <vector>


/**
 *  \class Controleur
 * @brief the controler class
 *
 *  This class link the the view and the model
 *
 */
class Controleur
{
public:
/**
 * @brief constructor
 *
 * @param mainW the application view
 * @param signalM the signalMapper use when an element in view is modified
 * @param signalD the signalMapper use when an element in view is deleted
 * @param signalC the signalMapper use when an element in view is checked
 */
    Controleur(QMainWindow *mainW, QSignalMapper *signalM, QSignalMapper *signalD, QSignalMapper *signalC);

    /**
     * @brief affect a new List to the root
     *
     * @param name the new List name
     * @param date the new List expiration date
     */
    void createList(const std::string &name, time_t date);

    /**
     * @brief Affect a new Sortedlist to the root
     *
     * @param name the new SortedList name
     * @param date the new SortedList expiration date
     */
    void createSortedList(const std::string &name, time_t date);

    /**
     * @brief calculate the position in the view of an Element represent by the QModelIndex
     *
     *      view exemple                Associated QModelIndex (we are using only the row of the QModelIndex)
     *  - element 1                 |   row = 0
     *      - element 1.1           |       row = 0
     *      - element 1.2           |       row = 1
     *          - element 1.2.1     |           row = 0
     *          - element 1.2.2     |           row = 1   <- element we want to calculate the tree structure
     *      - element 1.3           |       row = 2
     *
     *  This methode begin at element 1.2 and up the tree to the root element and then return the sequence of number to access
     *a component in the model (here return [1, 0])
     *
     * @param m the QModelIndex that represent the Element in the view we want to calculate the position
     * @return the revert sequence of number that make the connection between the view and the model
     */
    std::vector<int> calculateArborescence(QModelIndex m);

    /**
     * @brief add a new List in the model and the refresh the view
     *
     *  This method add a new List in the model, using calculateArborescence()
     *to know where add the new List. And then refresh the view and select the new Element in the view
     *
     * @param t the view which add the new Element
     *
     * @see calculateArborescence()
     */
    void addList(QTreeWidget *t);

    /**
     * @brief add a new SortedList in the model and the refresh the view
     *
     *  This method add a new SortedList in the model, using calculateArborescence()
     *to know where add the new SortedList. And then refresh the view and select the new Element in the view
     *
     * @param t the view which add the new Element
     *
     *@see calculateArborescence()
     */
    void addSortedList(QTreeWidget * t);

    /**
     * @brief add a new Task in the model and the refresh the view
     *
     *  This method add a new Task in the model, using calculateArborescence()
     *to know where add the new Task. And then refresh the view and select the new Element in the view
     *
     * @param t the view which add the new Element
     *
     *@see calculateArborescence()
     */
    void addTask(QTreeWidget *t);

    /**
     * @brief refresh the root Element from the model
     *
     * @param lineEdit the QLineEdit to update
     * @param dateEdit the QDateEdit to update
     * @param RBY the QRadioButton "yes" to update (if the list is sorted or not)
     * @param RBN the QRadioButton "no" to update (if the list is sorted or not)
     */
    void refreshTitle(QLineEdit* lineEdit, QDateEdit* dateEdit, QRadioButton* RBY, QRadioButton* RBN);

    /**
     * @brief refresh the view from the model
     *
     * @param t the view to update
     */
    void refreshVue(QTreeWidget * t);

    /**
     * @brief run recursively through the model and create Element that match the model Component's
     *
     * @param t the view which add the Element (if parent is the root Component, add the new Element in t)
     * @param p the current item to add the Element (if not, add the new Element in p)
     * @param parent the list to run through
     */
    void parcoursList(QTreeWidget * t, QTreeWidgetItem * p, List* parent);

    /**
     * @brief make a copy of list in an other (just on level)
     *
     * @param parent the list to copy
     * @param nouvelle the list which receive the copy
     */
    void parcoursListModele(List * parent, List *nouvelle);

    /**
     * @brief remove Component that match the selected Element in the view and the refresh the view
     *
     * @param t the view to refresh
     */
    void removeElement(QTreeWidget *t);

    /**
     * @brief remove a Component that match the Element remove signal and then refresh the view
     *
     * @param t the view to refresh
     * @param item the item to remove (only the matching Component are remove from the model, refreshing the view do the rest)
     */
    void removeElementFromItem(QTreeWidget *t, QTreeWidgetItem *item);

    /**
     * @brief move up a Component (that match the selected Element) in it tree level model and then refresh the view
     *
     * @param t the view tu refresh
     */
    void upElement(QTreeWidget * t);

    /**
     * @brief move down a Component (that match the selected Element) in it tree level model and then refresh the view
     *
     * @param t the view tu refresh
     */
    void downElement(QTreeWidget * t);

    /**
     * @brief update the Component that match the QModelIndex
     *
     * @param mIndex the QModelIndex that match the Component
     * @param name the new Component name
     * @param date the new Component expiration date
     * @param state the new Component state
     *
     * @see Controleur:calculateArborescence()
     */
    void updateModel(QModelIndex* mIndex, const QString& name, const QDateTime &date, const bool state);

    //! \name Getters
    //@{
    /**
     * @brief the current svae filepath accessor
     *
     * @return the current save filepath
     */
    QString getFilePath() const;

    /**
     * @brief the current template directory accessor
     *
     * @return the current template directory
     */
    QString getTemplateDirectory() const;

    /**
     * @brief get an Element identify by its key
     *
     * @param key the key that identify the Element to return
     * @return the Element identify by its key
     */
    QTreeWidgetItem* getElement(const int key);

    /**
     * @brief the root Component accessor
     *
     * @return the root Component
     */
    List * getRoot_();

    /**
     * @brief the flag that explain a model modification was performed accessor
     *
     * @return the flag that explain a model modification was performed
     */
    bool getFileModified_();
    //@}

    //! \name Setters
    //@{
    /**
     * @brief set a template directory
     *
     * @param path the new template directory to set
     */
    void setTemplateDirectory(const QString path);

    /**
     * @brief the root Component modifier
     *
     * @param root the new root Component
     */
    void setRoot_(List* root);

    /**
     * @brief
     *
     * @param file
     */
    void setFilePath(QString file);
    //@}

    /**
     * @brief return in type the type of the selected Element
     *
     *  type can be (sortedList, list, task or vide)
     *
     * @param t the view to inspect
     * @param type the Component type to return
     * @param nb the children number of the Component that match the selected Element
     */
    void is(QTreeWidget *t, std::string &type, int &nb);

    /**
     * @brief return whether the Component that match the selected Element is a List or a SortedList
     *
     * @param t the view to inspect
     * @return whether the Component that match the selected Element is a List or a SortedList
     */
    bool isListOrSortedList(QTreeWidget * t);

    /**
     * @brief run recursively through the model and create Element that match the model Component's for the preview
     *
     * @param t the view which add the Element (if parent is the root Component, add the new Element in t)
     * @param p the current item to add the Element (if not, add the new Element in p)
     * @param parent the list to run through
     */
    void parcoursListApercu(QTreeWidget * t, QTreeWidgetItem * p, List* parent);

    /**
     * @brief create a preview of the model
     *
     * @param t the view where create the preview
     */
    void createVueApercu(QTreeWidget * t);

    /**
     * @brief return whether the root Component is a SortedList
     *
     * @return whether the root Component is a SortedList
     */
    bool rootIsSortedList();

    /**
     * @brief turn a SortedList to a List (in model), then refresh the view
     *
     * @param t the view to refresh
     */
    void sortedListToList(QTreeWidget * t);

    /**
     * @brief turn a List to a SortedList (in model), then refresh the view
     *
     * @param t the view to refresh
     */
    void listToSortedList(QTreeWidget * t);

    /**
     * @brief @see parcoursArbre();
     *
     * @param itemP
     * @param item
     */
    void parcoursArbreRecursif(QTreeWidgetItem *itemP, QTreeWidgetItem *item);

    /**
     * @brief run through the old view and change the new to have the same look
     *
     * @param p old view to run through
     * @param tw the new view which will have the same look of the old view
     */
    void parcoursArbre(QTreeWidget *p, QTreeWidget *tw);

    /**
     * @brief delete an Element from the view
     *
     * @param t the view where remove the Element
     * @param arbre the path to the Element
     * @param m the index of the Element in the view to remove
     *
     * @see calculateArborescence()
     */
    void deleteCurrentItem(QTreeWidget *t, std::vector<int> &arbre, QModelIndex m);

    /**
     * @brief turn the Component that match the selected Element in the view, into a List, and the refresh the view
     *
     * @param t the view to refresh
     */
    void toList(QTreeWidget *t);

    /**
     * @brief turn the Component that match the selected Element in the view, into a SortedList, and the refresh the view
     *
     * @param t the view to refresh
     */
    void toSortedList(QTreeWidget *t);

    /**
     * @brief turn the Component that match the selected Element in the view, into a Task, and the refresh the view
     *
     * @param t the view to refresh
     */
    void toTask(QTreeWidget *t);

    /**
     * @brief save the tasklist in a specific file
     *
     *  correspond to "save as"
     *
     * @param path the path where saving the file
     */
    void saveFileOn(QString path);

    /**
     * @brief save the tasklist in a file
     *
     *  correpond to "save"
     *
     */
    void saveFile();

    /**
     * @brief open a file and create a tasklist
     *
     * @param path the filepath to load the tasklist
     */
    void openFile(QString path);

    /**
     * @brief save a pattern of the current tasklist in a specific file
     *
     * @param nameFile the path where saving the template
     */
    void saveTemplate(QString nameFile);

    /**
     * @brief load a template from a file
     *
     * @param path the template filepath to load
     * @param root the root Element to add the result of the loading
     * @return a list that represent the result of the loading
     */
    List* loadTemplate(const QString path, List *root);

    /**
     * @brief run recursively through a model and create Element that match the model Component's for the preview
     *
     * @param t the view which add the Element (if parent is the root Component, add the new Element in t)
     * @param p the current item to add the Element (if not, add the new Element in p)
     * @param parent the list to run through
     * @param root the first Component of the model
     */
    void parcoursListApercuTemplate(QTreeWidget *t, QTreeWidgetItem *p, List *parent, List *root);

    /**
     * @brief create a preview of a template.
     *
     * @param t the view to display the preview
     * @param root the model that correspond to the template loading
     */
    void createVueApercuTemplate(QTreeWidget *t, List *root);


    /**
     * @brief return whether the Component that match the Element is a List or a SortedList
     *
     * @param t the view to inspect
     * @param item the item to check whether is a List or a SortedList
     * @return whether the Component that match the Element is a List or a SortedList
     */
    bool isListOrSortedListFromItem(QTreeWidget *t, QTreeWidgetItem *item);

private:
    List * root_; /**< the root Component of the tasklist */
    QSignalMapper* modifiedElementSignalMapper_; /**< the signalMapper use when an element in view is modified */
    QSignalMapper* deletedElementSignalMapper_; /**< the signalMapper use when an element in view is deleted */
    QSignalMapper* checkboxStateChangeSignalMapper_; /**< the signalMapper use when an element in view is checked */
    QMainWindow * mainWindow_; /**< the GUI object */
    QVector<Element*> asup_;
    QHash<int, QTreeWidgetItem*> *elements_; /**< the Map Element identify by key*/

    XMLOperation* xmlOp_; /**< the object use to load or create xml files */
    QString filePath_; /**< the last saved file path */
    QString templateDirectory_; /**< the current template directory */
    bool fileModified_; /**< a flag that explain whether the model was modified */

    QTreeWidgetItem* getCurrentItem(QTreeWidget* t, std::vector<int> &arbre, QModelIndex m);

};

#endif // CONTROLEUR_H
