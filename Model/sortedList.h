//
//  sortedList.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include <iostream>

class SortedList : public List
{

public:
	
	SortedList();
	SortedList(const int id, const std::string& name, struct tm date);
	~SortedList();
	
	void addComponent(Component * c);
	
	// Ajouter une fonction permettant l'échange de deux component
	
private:
	
};

#endif
