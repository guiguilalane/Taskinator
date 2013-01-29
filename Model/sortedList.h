//
//  sortedList.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include "list.h"

#include <iostream>

class SortedList : public List
{

public:
	
	SortedList();
    SortedList(const std::string& name, time_t date, List* parent);
	~SortedList();

    virtual bool checkedPreviousTask();
	
    virtual void addComponent(Component * c);
	
	// Ajouter une fonction permettant l'échange de deux component
	
private:
	
};

#endif
