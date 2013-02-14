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

class Task : public Component
{

public:
	Task();
    Task(const std::string& name, time_t date, bool state = false);
	~Task();

    //À faire dans le controleur
//    void transformToList(List * list);

    virtual void setParent_(List * p);

    virtual std::ostream& affichage(std::ostream& os);
	
private:
	
};

#endif
