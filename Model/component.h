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
//class SortedList;

class Component
{
	
public:
	
    Component(List * parent = 0);
    Component(const std::string& name, time_t date);
    virtual ~Component()=0;

    // Ajouter getters et setters
    std::string getName_();
    time_t getDate_();
    bool getState_();
    List * getParent_();
    virtual void setParent_(List * p);

    // Permet de savoir si une case est cochable ou non
    virtual bool checkedPreviousTask();

    void upComponent();
    void downComponent();

    void check(bool newState);

    int getIdFromMap();

    virtual std::ostream& affichage(std::ostream& os) = 0;

protected:
	
	std::string name_;
    time_t date_;
    bool state_;
    List * parent_;
};

#endif
