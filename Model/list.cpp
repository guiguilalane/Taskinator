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

List::List(const std::string& name, time_t date): Component(name,date)
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

void List::setParent_(List * p)
{
    parent_ = p;
}

void List::addComponent(Component * c)
{
    c->setParent_(this);
    tabComponent_.insert(std::pair<int,Component*>(id_,c));
    id_++;
}

bool List::checkedPreviousTask()
{
    return ((Component* )this)->checkedPreviousTask();
}

std::ostream& List::affichage(std::ostream& os)
{
    os << this->getName_() << std::endl;
    for (int i = 1; i <= this->getTabComponent_().size() ; i++){
        os << "- ";
        tabComponent_[i]->affichage(os);
    }
    return os;
}
