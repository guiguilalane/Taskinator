//
//  sortedList.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include "list.h"

#include <iostream>

/**
 * @brief one of the node class of composite pattern
 *
 */
class SortedList : public List
{

public:
	
	SortedList();
/**
 * @brief constructor
 *
 * @param name the list name
 * @param date the end date witch the list expire
 * @param state the list state, terminated or not terminated
 */
    SortedList(const std::string& name, time_t date, bool state = false);

    /**
     * @brief destructor
     *
     */
    ~SortedList();

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
     * @brief This method look at the previous component state and return true whether
     *the component can be checked
     *
     *  SortedList:checkedPreviousTask() is quite different from the other Component:checkedPreviousTask()
     *  because only the first component of a SortedList can be checked. And when the previous component is checked
     *  the component can be checked.
     *
     * @return whether the component can be checked
     */
    virtual bool checkedPreviousTask();
	
    /**
     * @brief add a new component in the sortedList
     *
     * @param the new component to add
     */
    virtual void addComponent(Component * c);
	
    virtual std::ostream& affichage(std::ostream& os);

    /**
     * @brief this method look at the previous component and return true whether
     *the all previous component are checked.
     *
     *This method is recursive
     *
     * @param use to check whether all previous (< cle) component in sortedList are checked
     * @return whether the all previous component are checked.
     */
    virtual bool isPreviousTaskChecked(const int cle);

private:



	
};


#endif
