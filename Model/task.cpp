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

Task::Task(const std::string& name, time_t date): Component(name,date)
{}

Task::~Task()
{}

void Task::setParent_(List * p)
{
    parent_ = p;
}

std::ostream& Task::affichage(std::ostream& os)
{
    return os << this->getName_() << std::endl;
}
