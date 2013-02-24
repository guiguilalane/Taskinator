//
//  list.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef LIST_H
#define LIST_H

#include "component.h"

#include <iostream>
#include <map>

/**
 * @brief one of the node class of composite pattern
 */
class List : public Component
{

public:
	
	List();
/**
 * @brief constructor
 *
 * @param name the list name
 * @param date the end date which the list expire
 * @param state the list state, terminated or not terminated
 */
    List(const std::string& name, time_t date, bool state = false);

    /**
     * @brief destructor
     *
     */
    ~List();

    //! \name Getters
    //@{
    /**
     * @brief the list of children component accessor
     *
     * @return the list of children in map format
     */
    std::map<int, Component*>& getTabComponent_();
    /**
     * @brief the id of the next component to add accessor
     *
     * @return the id of the next component to add
     */
    int getId_();
    //@}

    //! \name Setters
    //@{
    /**
     * @brief the component parent modifier
     *
     *      The parent can be NULL whether the component is the root component
     *
     * @param p the new component parent
     *      If a component have a parent, this parent must be a List or a SortedList
     */
    virtual void setParent_(List * p);
    //@}

    /**
     * @brief add a new component in the list
     *
     * @param c the new component to add
     */
    virtual void addComponent(Component * c);

    /**
     * @brief remove the component identify by the position in the map begin at 1
     *
     *  the map is compressed :
     *
     *  map :
     *      1, c1
     *      2, c2
     *      3, c3
     *      4, c4
     *
     *  after removing the second element :
     *  map :
     *      1, c1
     *      2, c3
     *      3, c4
     *
     * @param cle the component position in the list
     */
    virtual void removeComponent(const int cle);
	
    virtual std::ostream& affichage(std::ostream& os);

    /**
     * @brief this method look at the previous component and return true whether
     *the all previous component are checked.
     *
     *This method is recursive
     *
     * @param cle not use here \see:SortedList:isPreviousTaskChecked()
     * @return whether the all previous component are checked.
     */
    virtual bool isPreviousTaskChecked(const int cle);

    /**
     * @brief This method look at the previous component state and return true whether
     *the component can be checked
     *
     * @return whether the component can be checked
     * @see Component:checkedPreviousTask()
     */
    virtual bool checkedPreviousTask();

    /**
     * @brief this method is used each time a child is checked and check the list if all
     * children are checked
     */
    virtual void checkIfPossible();

protected:

    int id_; /**< the next component position to add */
    std::map<int, Component *> tabComponent_; /**< the list of component */
	
};

#endif
