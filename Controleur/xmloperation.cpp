#include "xmloperation.h"

using namespace pugi;

XMLOperation::XMLOperation()
{
}

void XMLOperation::saveFile(const std::string &file, Component *root)
{
    xml_document doc;
    xml_node component = doc.append_child("taskList");
    if(dynamic_cast<SortedList*>(root))
    {
        component.append_attribute("type") = "sortedList";
    }
    else if(dynamic_cast<List*>(root))
    {
        component.append_attribute("type") = "list";
    }
    else
    {
        //lever une exception (ne devrait jamais arriver)
    }
    component.append_attribute("name") = root->getName_().c_str();
    component.append_attribute("date") = QDateTime::fromTime_t(root->getDate_()).date().toString("dd/MM/yyyy").toStdString().c_str();
    component.append_attribute("checked") = false;

    parcoursList((List*)root, component, false);
    //enregistrer xml_document
    doc.save_file(file.c_str());
}

void XMLOperation::createTemplate(const std::string &file, Component *root)
{
    xml_document doc;
    xml_node component = doc.append_child("template");
    if(dynamic_cast<SortedList*>(root))
    {
        component.append_attribute("type") = "sortedList";
    }
    else if(dynamic_cast<List*>(root))
    {
        component.append_attribute("type") = "list";
    }
    else
    {
        //lever une exception (ne devrait jamais arriver)
    }
    component.append_attribute("name") = "";
    component.append_attribute("date") = "";
    component.append_attribute("checked") = false;

    parcoursList((List*)root, component, true);
    //enregistrer xml_document
    doc.save_file(file.c_str());
}

List* XMLOperation::readFile(const std::string &file)
{
    xml_document doc;
    doc.load_file(file.c_str());
    xml_node rootElement = doc.child("taskList");
    std::string type(rootElement.attribute("type").as_string());
    List* root = NULL;
    if(type.compare("sortedList")==0)
    {
        root = ComponentFactory::getInstance()->createSortedList(std::string(rootElement.attribute("name").as_string()), QDateTime::fromString(QString(rootElement.attribute("date").as_string()), "dd/MM/yyyy").toTime_t());
    }
    else if(type.compare("list")==0)
    {
        root = ComponentFactory::getInstance()->createList(std::string(rootElement.attribute("name").as_string()), QDateTime::fromString(QString(rootElement.attribute("date").as_string()), "dd/MM/yyyy").toTime_t());
    }
    else
    {
        //lever une exception(n'est possible que si l'utilisateur change la valeur de l'attribut 'type' de la balise 'tasklist'
        //NOTE: le mieux serait de cryper les données pour que l'utilisateur ne puisse pas les modifier à la main
    }
    parcoursFile(root, rootElement);
    return root;
}

void XMLOperation::parcoursList(List *c, xml_node& root, const bool isTemplate)
{
    int taille = c->getTabComponent_().size();
    Component* current;
    for(int i = 1; i <= taille; ++i)
    {
        xml_node component = root.append_child("component");
        current = c->getTabComponent_()[i];

        xml_attribute firstAttr = component.append_attribute("name") = current->getName_().c_str();
        if(!isTemplate)
        {
            component.append_attribute("date") = QDateTime::fromTime_t(current->getDate_()).date().toString("dd/MM/yyyy").toStdString().c_str();

            component.append_attribute("checked") = current->getState_() ;
        }
        else
        {
            component.append_attribute("date") = "";

            component.append_attribute("checked") = false;
        }

        if(dynamic_cast<SortedList*>(current))
        {//sortedList
            component.insert_attribute_before("type", firstAttr) = "sortedList";

            parcoursList((SortedList*) current, component, isTemplate);
        }
        else if(dynamic_cast<List*>(current))
        {//list
            component.insert_attribute_before("type", firstAttr) = "list";

            parcoursList((List*) current, component, isTemplate);
        }
        else
        {//task
            component.insert_attribute_before("type", firstAttr) = "task";
        }
    }
}

void XMLOperation::parcoursFile(List *c, xml_node &element)
{
    std::string type;
    Component* child;
    for(xml_node component = element.child("component"); component; component = component.next_sibling("component"))
    {
        type = component.attribute("type").as_string();
        if(type.compare("sortedList")==0)
        {
            child = ComponentFactory::getInstance()->createSortedList(std::string(component.attribute("name").as_string()), QDateTime::fromString(QString(component.attribute("date").as_string()), "dd/MM/yyyy").toTime_t(), component.attribute("checked").as_bool());
            parcoursFile((List*) child, component);
        }
        else if(type.compare("list")==0)
        {
            child = ComponentFactory::getInstance()->createList(std::string(component.attribute("name").as_string()), QDateTime::fromString(QString(component.attribute("date").as_string()), "dd/MM/yyyy").toTime_t(), component.attribute("checked").as_bool());
            parcoursFile((List*) child, component);
        }
        else
        {
            child = ComponentFactory::getInstance()->createTask(std::string(component.attribute("name").as_string()), QDateTime::fromString(QString(component.attribute("date").as_string()), "dd/MM/yyyy").toTime_t(), component.attribute("checked").as_bool());
        }
        c->addComponent(child);
    }
}
