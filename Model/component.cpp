//
//  component.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "component.h"

Component::Component()
{}

Component::Component(const int id, const std::string& name, struct tm date): id(id_), name_(name), date_(date)
{}

Component::~Component()
{}

bool Component::checkedPreviousTask()
{
	bool res;
	// TODO : vérifier que toutes les taches au dessus sont checkées --> Que dans le cas d'une liste ordonnée ? --> Pas implémentée ici ?
}

bool operator<(Component* c1, Component* c2)
{
	bool res = false;
	if(c1->id_<c2->id_)
	{
		res = true;
	}
	return res;
}