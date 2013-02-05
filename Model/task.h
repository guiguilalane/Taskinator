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

#include <iostream>

class Task : public Component
{

public:
	Task();
    Task(const std::string& name, time_t date);
	~Task();

    virtual void setParent_(List * p);

    virtual std::ostream& affichage(std::ostream& os);
	
private:
	
};

#endif
