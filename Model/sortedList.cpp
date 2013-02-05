//
//  sortedList.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "sortedList.h"

SortedList::SortedList()
{}

SortedList::SortedList(const std::string& name, time_t date) : List(name, date)
{}

SortedList::~SortedList()
{}

void SortedList::setParent_(List * p)
{
    parent_ = p;
}

// Retourne un booléan indiquant si la liste est cochable ou non
// et si toutes les cases précédentes sont cochées.
bool SortedList::checkedPreviousTask()
{
    bool res;
    if (parent_ == 0)
    {
       res = true;
    }
    else
    {
        int cle = parent_->getTabComponent_().find(getIdFromMap())->first;
        bool check = false;
        if (cle > 1)
        {
            check = parent_->getTabComponent_()[cle-1]->getState_();
        }
        res = parent_->checkedPreviousTask() && check;
    }
    return res;
}

void SortedList::addComponent(Component *c)
{
    c->setParent_(this);
    tabComponent_.insert(std::pair<int,Component*>(id_,c));
    ++id_;
}

std::ostream& SortedList::affichage(std::ostream& os)
{
    os << this->getName_() << std::endl;
    for (int i = 1; i <= this->getTabComponent_().size() ; i++){
        os << i << "- ";
        tabComponent_[i]->affichage(os);
    }
    return os;
}
