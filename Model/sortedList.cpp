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