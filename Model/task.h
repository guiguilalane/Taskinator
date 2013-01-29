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
    Task(const std::string& name, time_t date, List* parent);
	~Task();
	
private:
	
};

#endif
