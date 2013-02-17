#include "componentfactory.h"

ComponentFactory* ComponentFactory::componentFactory_ = NULL;

ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
}


ComponentFactory *ComponentFactory::getInstance()
{
    if(NULL == componentFactory_)
    {
        componentFactory_ = new ComponentFactory();
    }
    return componentFactory_;
}

void ComponentFactory::kill()
{
    if(NULL != componentFactory_)
    {
        delete componentFactory_;
        componentFactory_ = NULL;
    }
}

Task *ComponentFactory::createTask(const std::string &name, const time_t date, const bool state)
{
    return new Task(name, date, state);
}

List *ComponentFactory::createList(const std::string &name, const time_t date, const bool state)
{
    return new List(name, date, state);
}

SortedList *ComponentFactory::createSortedList(const std::string &name, const time_t date, const bool state)
{
    return new SortedList(name, date, state);
}

