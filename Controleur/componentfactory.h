#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "../Model/task.h"
#include "../Model/sortedList.h"

/**
 * @brief Class to create all Component needed if the composite Pattern,
 *  Component, List, SortedList, Task
 *
 */
class ComponentFactory
{
public:

    /**
     * @brief get an Instance of ComponentFactory
     *
     * @return This ComponentFactory object if an instance allready exist, a new instance otherwise
     */
    static ComponentFactory* getInstance();

    /**
     * @brief destroy this instance of ComponentFactory object
     *
     */
    static void kill();

    /**
     * @brief create and allocate Task object
     *
     * @param name the Task name
     * @param date the Task expiration date
     * @param state the Task state
     * @return A new instance of Task object
     */
    Task* createTask(const std::string& name, const time_t date, const bool state = false);

    /**
     * @brief create and allocate List object
     *
     * @param name the List name
     * @param date the List expiratino date
     * @param state the List state
     * @return A new instance of List object
     */
    List* createList(const std::string& name, const time_t date, const bool state = false);

    /**
     * @brief create and allocate SortedList object
     *
     * @param name the SortedList name
     * @param date the SortedList expiration date
     * @param state the SortedList state
     * @return A new instance of SortedList object
     */
    SortedList* createSortedList(const std::string& name, const time_t date, const bool state = false);

private:

    static ComponentFactory* componentFactory_; /**< TODO */

/**
 * @brief constructor
 *
 *  ComponentFactory constructor
 *
 */
    ComponentFactory();

    /**
     * @brief destructor
     *
     *  ComponentFactory destructor
     *
     */
    ~ComponentFactory();
};

#endif // COMPONENTFACTORY_H
