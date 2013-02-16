#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include "../Model/task.h"
#include "../Model/sortedList.h"

class ComponentFactory
{
public:

    static ComponentFactory* getInstance();

    static void kill();

    Task* createTask(const std::string& name, const time_t date);

    List* createList(const std::string& name, const time_t date);

    SortedList* createSortedList(const std::string& name, const time_t date);

private:

    static ComponentFactory* componentFactory_;

    ComponentFactory();

    ~ComponentFactory();
};

#endif // COMPONENTFACTORY_H
