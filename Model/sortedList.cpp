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

SortedList::SortedList(const std::string& name, time_t date, List* parent) : List(name,date, parent)
{}

SortedList::~SortedList()
{}

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

//void SortedList::addComponent(Component * c)
//{
//    std::map<int, Component *> it = c->tabComponent_.begin();
//	for (it; it != tabComponent_.end(); ++it)
//	{
//		if (c < *it)
//		{
//			tabComponent_->pushback(c);
//		}
//	}
//}
void SortedList::addComponent(Component *c)
{
    ((List*) this)->addComponent(c);
}
