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

Task::Task(const std::string& name, time_t date, List *parent): Component(name,date, parent)
{}

Task::~Task()
{}
