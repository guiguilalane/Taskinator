//
//  task.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef TASK_H
#define TASK_H

#include <iostream>

class Task : public Component
{

public:
	Task();
	Task(const int id, const std::string& name, struct tm date);
	~Task();
	
private:
	
};

#endif
