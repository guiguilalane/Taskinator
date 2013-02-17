#ifndef XMLOPERATION_H
#define XMLOPERATION_H

#include "Lib/pugixml_sources/pugixml.hpp"
#include "Model/sortedList.h"
#include "Model/task.h"
#include "componentfactory.h"
#include "QDateTime"
#include <string>

class XMLOperation
{
public:
    XMLOperation();

    void newFileFromTemplate(const std::string& file, const std::string& rootName);
    void createTemplate(const std::string& file);
    void saveFile(const std::string& file, Component* root);
    List *readFile(const std::string& file);

private:
//    pugi::xml_document doc_;

    void parcoursList(List *c, pugi::xml_node &root);

    void parcoursFile(List* c, pugi::xml_node &element);
};

#endif // XMLOPERATION_H
