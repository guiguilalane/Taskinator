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
#include <map>
class List;

class Component
{
	
public:
	
    Component(Component * parent = 0);
    Component(const std::string& name, time_t date, Component * parent);
    virtual ~Component()=0;

    // Ajouter getters et setters
    std::string getName_();
    time_t getDate_();
    bool getState_();
    Component * getParent_();

    // Permet de savoir si une case est cochable ou non
    virtual bool checkedPreviousTask();
    void upComponent();
    void downComponent();

protected:
	
	std::string name_;
    time_t date_;
    bool state_;
    Component * parent_;
};

#endif

// Questions:
// - Le parent doit-il etre un Component ou une List
// - Faut-il implémenter le virtual ?
// - Est-ce qu'il faut mettre checkedPreviousTask en virtual --> Selon Guillaume OUI
