//
//  list.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <vector>

class List : public Component
{
	
public:
	
	List();
	List(const int id, const std::string& name, struct tm date);
	~List();
	
	void addComponent(Component * c);
	
		// Ajouter une fonction permettant l'échange de deux component ou juste l'affichage
	
private:
	std::vector<Component *> tabComponent_;
	
	
};

#endif
