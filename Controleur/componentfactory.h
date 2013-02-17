#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "../Model/task.h"
#include "../Model/sortedList.h"

class ComponentFactory
{
public:

    static ComponentFactory* getInstance();

    static void kill();

    Task* createTask(const std::string& name, const time_t date, const bool state = false);

    List* createList(const std::string& name, const time_t date, const bool state = false);

    SortedList* createSortedList(const std::string& name, const time_t date, const bool state = false);

private:

    static ComponentFactory* componentFactory_;

    ComponentFactory();

    ~ComponentFactory();
};

#endif // COMPONENTFACTORY_H
