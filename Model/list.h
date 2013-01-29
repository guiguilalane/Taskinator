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
#include <map>

class List : public Component
{

public:
	
	List();
    List(const std::string& name, struct tm date);
	~List();

    std::map<int, Component *>& getTabComponent_();
    int getId_();

    virtual bool checkedPreviousTask();
    void addComponent(Component * c);
	
		// Ajouter une fonction permettant l'échange de deux component ou juste l'affichage
	
private:
    int id_;
    std::map<int, Component *> tabComponent_;
	
};

#endif
