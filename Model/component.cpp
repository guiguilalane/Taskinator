//
//  component.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "list.h"

Component::Component(List *parent) : parent_(parent), state_(false)
{}

Component::Component(const std::string& name, time_t date): name_(name), date_(date), state_(false)
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
        res = parent_->isPreviousTaskChecked(getIdFromMap());
    }
    return res;
}

int Component::getIdFromMap()
{
    bool res = false;
    int toreturn = 0;
    int i = 1;
    int size = parent_->getTabComponent_().size();
    while(i <= size && !res)
    {
        Component* component = parent_->getTabComponent_()[i];
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

void Component::check(bool newState)
{
    if(newState && checkedPreviousTask())
    {
//        std::cout << "checké" << std::endl;
        state_ = true;
        //lorsque toutes les tâches soeurs sont checkées, checker la liste parent
    } else {
//        std::cout << "impossible de checker" << std::endl;
        state_ = false;
    }
}
