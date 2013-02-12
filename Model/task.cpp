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

Task::Task(const std::string& name, time_t date): Component(name,date)
{}

Task::~Task()
{}

void Task::setParent_(List * p)
{
    parent_ = p;
}

//void Task::transformToList(List *list)
//{
//    list = new List(name_, date_);
//    list->setParent_(parent_);
//    int cle = getIdFromMap();
//    parent_->getTabComponent_()[cle] = list;
//    delete this;
//}

std::ostream& Task::affichage(std::ostream& os)
{
    return os << this->getName_() << ", "<< (state_ ? "checked" : "not checked") << std::endl;
}
