//
//  component.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "component.h"

Component::Component(Component * parent) : parent_(parent)
{}

Component::Component(const std::string& name, time_t date, Component * parent): name_(name), date_(date), parent_(parent)
{}

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

Component * Component::getParent_()
{
    return parent_;
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

void Component::upComponent()
{
    int cleC = static_cast<List*>(parent_)->getTabComponent_().find(this);
    // We cannot select the root list
    if (cleC > 1)
    {
        Component * tmp = parent_.getTabComponent_()[cleC];
        parent_.getTabComponent_()[cleC] = parent_.getTabComponent_()[cleC-1] ;
        parent_.getTabComponent_()[cleC - 1] = tmp;
    }
}

void Component::downComponent()
{
    int cleC = parent_.getTabComponent_().find(this);
    // We cannot select the root list
    if (cleC < parent_.getTabComponent_().size())
    {
        Component * tmp = parent_.getTabComponent_()[cleC];
        parent_.getTabComponent_()[cleC] = parent_.getTabComponent_()[cleC+1] ;
        parent_.getTabComponent_()[cleC + 1] = tmp;
    }
}
