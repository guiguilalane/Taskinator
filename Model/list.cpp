//
//  list.cpp
//  
//
//  Created by Noémie RULLIER on 27/01/13.
//
//

#include "list.h"


List::List()
{
    id_=1;
}

List::List(const std::string& name, time_t date): Component(name,date)
{
    id_=1;
}

List::~List()
{
    int i = 1;
    int taille = tabComponent_.size();
    for (i; i <= taille; ++i)
    {
        delete tabComponent_[i];
    }
    tabComponent_.clear();
}

std::map<int, Component *>& List::getTabComponent_()
{
    return tabComponent_;
}

int List::getId_()
{
    return id_;
}

void List::setParent_(List * p)
{
    parent_ = p;
}

void List::addComponent(Component * c)
{
    c->setParent_(this);
    tabComponent_.insert(std::pair<int,Component*>(id_,c));
    id_++;

    //Vérification que la date de la liste correspond à la date le plus "vieille" des sous-tâches
}

bool List::isPreviousTaskChecked(const int cle)
{
    return this->checkedPreviousTask();
}

void List::removeComponent(const int cle)
{
    Component *tmp = tabComponent_[cle];
    delete tmp;

    int i = cle+1;
    int taille = tabComponent_.size();
    for(i; i <= taille; ++i)
    {
        tabComponent_[i-1] = tabComponent_[i];
    }
    i--;
    id_--;
    tabComponent_.erase(i);
}

std::ostream& List::affichage(std::ostream& os)
{
    os << this->getName_() << ", " << (state_ ? "checked" : "not checked") << std::endl;
    for (int i = 1; i <= this->getTabComponent_().size() ; i++){
        os << "- ";
        tabComponent_[i]->affichage(os);
    }
    return os;
}
