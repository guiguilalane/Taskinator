#ifndef XMLOPERATION_H
#define XMLOPERATION_H

#include "Lib/pugixml_sources/pugixml.hpp"
#include "Model/sortedList.h"
#include "Model/task.h"
#include "componentfactory.h"
#include "QDateTime"
#include <string>

/**
 * @brief Class use to load a TaskList from .tor or .ulk file.
 * It use also to save a List in .tor or .ulk file.
 *
 * The .tor files are the xml format of the TaskList
 *      <tasklist type="list | sortedList" name="taskList name" date="dd/MM/yyyy" checked="true|false">
 *          <component type="list | sortedList | task" name="component name" date="dd/MM/yyyy" checked="true|false"/>
 *          ...
 *      </tasklist>
 *
 * The .ulk files are the xml format of a Template
 *      <template type="list | sortedList" name="" date="" checked="false">
 *          <component type="list | sortedList | task" name="component name" date="" checked="false"/>
 *          ...
 *      </template>
 *
 *When the user whant to use load a template he had to fill the blank.
 *
 *The xml parsing use the pugixml Library @see http://pugixml.org/
 *
 */
class XMLOperation
{
public:
    /**
     * @brief constructor
     *
     */
    XMLOperation();

    /**
     * @brief create a new list from template file
     *
     * @param file the template path
     * @return the new list which created from the template file
     */
    List *newFileFromTemplate(const std::string& file);

    /**
     * @brief create a new template from an existing list
     *
     * @param file the path where the template will be saved
     * @param root the root component of the list used to create the template file
     */
    void createTemplate(const std::string& file, Component *root);

    /**
     * @brief save the list
     *
     * @param file the path where the list will be saved
     * @param root the root component of the list which will be saved
     */
    void saveFile(const std::string& file, Component* root);

    /**
     * @brief create a new list from a saved list
     *
     * @param file the file where read the list
     * @return the new list which created from the saved list
     */
    List *readFile(const std::string& file);

private:
//    pugi::xml_document doc_;

    /**
     * @brief run recursively through a List and then return a xml tree (second parameter)
     *
     *  This method look at the list (get the name, the date and the state) create a new xml_node for this list,
     *  doing the same thing with each own children and then make the new xml_node a child of root (which is a xml_node).
     *
     * @param c the list to browse
     * @param root the xml_node to add child
     * @param isTemplate flag to explain that the xml tree should be a template or not.
     */
    void parcoursList(List *c, pugi::xml_node &root, const bool isTemplate);

    /**
     * @brief run recursively through a xml_node and the return a new List (first parameter)
     *
     *  This method look at the xml_node (get the type, the name, the date and the state) create a new Component depend on the type,
     *  and if the type is a list or a sortedList, do the same thing (the recursion here) with all the xml_node children.
     *
     * @param c the result list of browsing
     * @param element the xml_node to browse
     */
    void parcoursFile(List* c, pugi::xml_node &element);
};

#endif // XMLOPERATION_H
