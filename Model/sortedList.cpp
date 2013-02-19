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

SortedList::SortedList(const std::string& name, time_t date, bool state) : List(name, date, state)
{}

SortedList::~SortedList()
{
    int i = 1;
    int taille = tabComponent_.size();
    for (; i <= taille; ++i)
    {
        delete tabComponent_[i];
    }
    tabComponent_.clear();
}

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
        int cle = getIdFromMap();
        bool check = false;
        if (cle > 1)
        {
            check = parent_->getTabComponent_()[cle-1]->getState_();
        }
        res = parent_->checkedPreviousTask() && check;
    }
    return res;
}

bool SortedList::isPreviousTaskChecked(const int cle)
{
    int i = 1;
    bool isChecked = true;
    while(i < cle && isChecked){
        isChecked = isChecked && tabComponent_[i]->getState_();
        ++i;
    }
    bool b;
    if(parent_ == 0)
    {
        b = true;
    }
    else
    {
        b = parent_->isPreviousTaskChecked(getIdFromMap());
    }
    return isChecked && b;
}

void SortedList::addComponent(Component *c)
{
    c->setParent_(this);
    tabComponent_.insert(std::pair<int,Component*>(id_,c));
    ++id_;
    //TODO: mettre la date de la liste à la dernière date de la sous-tâche
}

std::ostream& SortedList::affichage(std::ostream& os)
{
    os << this->getName_() << ", " << (state_ ? "checked" : "not checked") << std::endl;
    for (int i = 1; i <= this->getTabComponent_().size() ; i++){
        os << i << "- ";
        tabComponent_[i]->affichage(os);

    }

    return os;
}
