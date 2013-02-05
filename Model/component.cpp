//
//  component.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "list.h"

Component::Component(List *parent) : parent_(parent)
{}

Component::Component(const std::string& name, time_t date): name_(name), date_(date)
{
    parent_ = 0;
}

Component::~Component()
{}

std::string Component::getName_()
{
    return name_;
}

time_t Component::getDate_()
{
    return date_;
}

bool Component::getState_()
{
    return state_;
}

List *Component::getParent_()
{
    return parent_;
}

void Component::setParent_(List * p)
{
    parent_ = p;
}

// Retourne un booléan indiquant si la liste est cochable ou non
bool Component::checkedPreviousTask()
{
    bool res;
    if (parent_ == 0)
    {
       res = true;
    }
    else
    {
        res = parent_->checkedPreviousTask();
    }
    return res;
    // TODO : a réimplémenter dans sortedList -->
//    True si toutes les case précédentes sont cochées
}

int Component::getIdFromMap()
{
    bool res = false;
    int toreturn = 0;
    int i = 1;
    int size = parent_->getTabComponent_().size();
    while(i <= size && !res)
    {
        if(this == parent_->getTabComponent_()[i])
        {
            res = true;
            toreturn = i;
        }
        ++i;
    }
    return toreturn;
}

void Component::upComponent()
{
    int cleC = getIdFromMap();
    // We cannot select the root list
    if (cleC > 1)
    {
        Component * tmp = parent_->getTabComponent_()[cleC];
        parent_->getTabComponent_()[cleC] = parent_->getTabComponent_()[cleC-1];
        parent_->getTabComponent_()[cleC - 1] = tmp;
    }
}

void Component::downComponent()
{
    int cleC = getIdFromMap();
    // We cannot select the root list
    if (cleC < parent_->getTabComponent_().size())
    {
        Component * tmp = parent_->getTabComponent_()[cleC];
        parent_->getTabComponent_()[cleC] = parent_->getTabComponent_()[cleC+1] ;
        parent_->getTabComponent_()[cleC + 1] = tmp;
    }
}
