//
//  task.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef TASK_H
#define TASK_H

#include "component.h"

class SortedList;

#include <iostream>

/**
 * @brief the leaf class of composite pattern
 *
 */
class Task : public Component
{

public:
	Task();
/**
 * @brief constructor
 *
 * @param name the task name
 * @param date the task expiration date
 * @param state the task state
 */
    Task(const std::string& name, time_t date, bool state = false);
    /**
     * @brief destructor
     *
     */
    ~Task();

    //À faire dans le controleur
//    void transformToList(List * list);

    //! \name Setters
    //@{
    /**
     * @brief the component parent modifier
     *
     * @param p the new component parent
     */
    virtual void setParent_(List * p);
    //@}

    virtual std::ostream& affichage(std::ostream& os);
	
private:
	
};

#endif
