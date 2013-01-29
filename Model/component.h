//
//  component.h
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <ctime>

class Component
{
	
public:
	
	Component();
	Component(const int id, const std::string& name, struct tm date);
	vitual ~Component()=0;
	bool checkedPreviousTask();

protected:
	
	int id_;
	std::string name_;
	struct tm date_;
	
};

bool operator<(Component* c1, Component* c2);

#endif
