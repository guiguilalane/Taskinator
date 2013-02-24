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

/**
 * @brief Abstract class of composite pattern
 *
 *  A Component will be represented by a Task, a List or a SortedList
 *
 */
class Component
{
	
public:
	
/**
 * @brief constructor
 *
 * @param parent the parent of this component, it must be a List object
 */
    Component(List * parent = 0);

/**
 * @brief constructor
 *
 * @param name the component name
 * @param date the end date which the component expire
 * @param state the componant state, terminated or not terminated
 */
    Component(const std::string& name, time_t date, bool state = false);

    /**
     * @brief destructor
     *
     */
    virtual ~Component()=0;

    //! \name Getters
    //@{

    /**
     * @brief the component name accessor
     *
     * @return the component name
     */
    std::string getName_();

    /**
     * @brief the component expiration date accessor
     *
     * @return the component expiration date
     */
    time_t getDate_();

    /**
     * @brief the component state accessor
     *
     * @return whether the component is terminated or not
     */
    bool getState_();

    /**
     * @brief the component parent accessor
     *
     *      The parent can be NULL whether the component is the root component
     *
     * @return the component parent
     *      If a component have a parent, this parent must be a List or a SortedList
     */
    List * getParent_();
    //@}

    //! \name Setters
    //@{
    /**
     * @brief the component name modifier
     * @param the new name of the component
     */
    void setName_(std::string name);

    /**
     * @brief the component expiration date modifier
     *
     * @param date the new expiration date
     */
    void setDate_(time_t date);

    /**
     * @brief the component state modifier
     *
     * @param state the new component state
     */
    void setState_(bool state);

    /**
     * @brief the component parent modifier
     *
     *      The parent can be NULL whether the component is the root component
     *
     * @param p the new component parent
     *      If a component have a parent, this parent must be a List or a SortedList
     */
    virtual void setParent_(List * p);
    //@}

    // Permet de savoir si une case est cochable ou non
    /**
     * @brief This method look at the previous component state and return true whether
     *the component can be checked
     *
     * @return whether the component can be checked
     */
    virtual bool checkedPreviousTask();

    /**
     * @brief this method move the component up in the parent component list
     *
     */
    void upComponent();

    /**
     * @brief this method move the component down in the parent component list
     *
     */
    void downComponent();

    /**
     * @brief check whether the component can be check and set the new value if possible
     *
     * @param newState the new component state
     */
    void check(bool newState);

    /**
     * @brief this method return the id of the component in its parent components
     *
     * @return the id of the component
     */
    int getIdFromMap();
    virtual std::ostream& affichage(std::ostream& os) = 0;

protected:
	
    std::string name_; /**< component name*/
    time_t date_; /**< component expiration date*/
    bool state_; /**< component state */
    List * parent_; /**< the component parent */
};

#endif
