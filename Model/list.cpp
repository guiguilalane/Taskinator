//
//  list.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "list.h"


List::List()
{
    id_=1;
}

List::List(const std::string& name, time_t date, List* parent): Component(name,date,parent)
{
    id_=1;
}

List::~List()
{
    tabComponent_.erase(tabComponent_.begin(), tabComponent_.end());
}

std::map<int, Component *> List::getTabComponent_()
{
    return tabComponent_;
}

int List::getId_()
{
    return id_;
}

void List::addComponent(Component * c)
{
    tabComponent_.insert(std::pair<int,Component*>(id_,c));
    id_++;
}

bool List::checkedPreviousTask()
{
    return ((Component* )this)->checkedPreviousTask();
}
