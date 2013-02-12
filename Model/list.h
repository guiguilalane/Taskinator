//
//  list.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef LIST_H
#define LIST_H

#include "component.h"

#include <iostream>
#include <map>

class List : public Component
{

public:
	
	List();
    List(const std::string& name, time_t date);
	~List();

    std::map<int, Component*>& getTabComponent_();
    int getId_();
    virtual void setParent_(List * p);

    virtual void addComponent(Component * c);

    virtual void removeComponent(const int cle);
	
    virtual std::ostream& affichage(std::ostream& os);

    virtual bool isPreviousTaskChecked(const int cle);

protected:

    int id_;
    std::map<int, Component *> tabComponent_;
	
};

#endif
