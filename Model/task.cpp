//
//  task.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "task.h"
#include "list.h"
Task::Task()
{}

Task::Task(const std::string& name, time_t date, bool state): Component(name, date, state)
{}

Task::~Task()
{}

void Task::setParent_(List * p)
{
    parent_ = p;
}

std::ostream& Task::affichage(std::ostream& os)
{
    return os << this->getName_() << ", "<< (state_ ? "checked" : "not checked") << std::endl;
}
