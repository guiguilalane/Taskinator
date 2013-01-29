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

class List;

class Component
{
	
public:
	
    Component(List * parent = 0);
    Component(const std::string& name, time_t date, List * parent);
    virtual ~Component()=0;

    // Ajouter getters et setters
    std::string getName_();
    time_t getDate_();
    bool getState_();
    List * getParent_();

    // Permet de savoir si une case est cochable ou non
    virtual bool checkedPreviousTask();
    void upComponent();
    void downComponent();

    int getIdFromMap();

protected:
	
	std::string name_;
    time_t date_;
    bool state_;
    List * parent_;
};

#endif

// Questions:
// - Le parent doit-il etre un Component ou une List
// - Faut-il implémenter le virtual ?
// - Est-ce qu'il faut mettre checkedPreviousTask en virtual --> Selon Guillaume OUI
