//
//  task.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "task.h"

Task::Task()
{}

Task::Task(const int id, const std::string& name, struct tm date): Component(id,name,date)
{}

Task::~Task()
{}