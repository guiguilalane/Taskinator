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

SortedList::SortedList(const int id, const std::string& name, struct tm date) : List(id,name,date)
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
        int cle = parent_->getTabComponent_().find(this).first;
        bool check = false;
        if (cle > 1)
        {
            check = parent_->getTabComponent_()[cle-1].getState_();
        }
        res = parent_->checkedPreviousTask() && check;
    }
    return res;
}

void SortedList::addComponent(Component * c)
{
	std::vector<Component *> it = c->tabComponent_.begin();
	for (it; it != tabComponent_.end(); ++it)
	{
		if (c < *it)
		{
			tabComponent_->pushback(c);
		}
	}
}
