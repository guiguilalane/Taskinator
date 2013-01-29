//
//  list.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "list.h"


List::List()
{}

List::List(const int id, const std::string& name, struct tm date): Component(id,name,date)
{}

List::~List()
{}

void List::addComponent(Component * c)
{
	tabComponent_->pushback(c);
}