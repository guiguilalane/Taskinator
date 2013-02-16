#include "xmloperation.h"

using namespace pugi;

XMLOperation::XMLOperation()
{
}

void XMLOperation::saveFile(const std::string &file, Component *root)
{
    xml_document doc;
    xml_node component = doc.append_child("taskList");
    parcoursList((List*)root, component, 0);
    //enregistrer xml_document
    doc.save_file(file.c_str());
}

void XMLOperation::readFile(const std::string &file)
{
    xml_document doc;
    xml_parse_result result = doc.load_file(file.c_str());
    std::cout << result.description() << std::endl;
}

//TODO: enlever le paramètre 'indent' qui ne sert que pour l'affichage
void XMLOperation::parcoursList(List *c, xml_node& root, int indent)
{
    int taille = c->getTabComponent_().size();
    Component* current;
    for(int i = 1; i <= taille; ++i)
    {
        xml_node component = root.append_child("component");
        current = c->getTabComponent_()[i];
        if(dynamic_cast<SortedList*>(current))
        {//sortedList
            component.append_attribute("type") = "sortedList";

            component.append_attribute("name") = current->getName_().c_str();

            component.append_attribute("date") = QDateTime::fromTime_t(current->getDate_()).date().toString("dd/MM/yyyy").toStdString().c_str();

            component.append_attribute("checked") = current->getState_() ;

            printIndent(indent);
            std::cout << "<component type=\"sortedList\" name=\"" << current->getName_() << "\" date=\"" << QDateTime::fromTime_t(current->getDate_()).date().toString("dd/MM/yyyy").toStdString() << "\" checked=\"" << (current->getState_() ? "true" : "false") << "\">" << std::endl;
            parcoursList((SortedList*) current, component, indent+1);
        }
        else if(dynamic_cast<List*>(current))
        {//list
            component.append_attribute("type") = "list";

            component.append_attribute("name") = current->getName_().c_str();

            component.append_attribute("date") = QDateTime::fromTime_t(current->getDate_()).date().toString("dd/MM/yyyy").toStdString().c_str();

            component.append_attribute("checked") = current->getState_() ;

            printIndent(indent);
            std::cout << "<component type=\"list\" name=\"" << current->getName_() << "\" date=\"" << QDateTime::fromTime_t(current->getDate_()).date().toString("dd/MM/yyyy").toStdString() << "\" checked=\"" << (current->getState_() ? "true" : "false") << "\">" << std::endl;
            parcoursList((List*) current, component, indent+1);
        }
        else
        {//task
            component.append_attribute("type") = "task";

            component.append_attribute("name") = current->getName_().c_str();

            component.append_attribute("date") = QDateTime::fromTime_t(current->getDate_()).date().toString("dd/MM/yyyy").toStdString().c_str();

            component.append_attribute("checked") = current->getState_() ;

            printIndent(indent);
            std::cout << "<component type=\"task\" name=\"" << current->getName_() << "\" date=\"" << QDateTime::fromTime_t(current->getDate_()).date().toString("dd/MM/yyyy").toStdString() << "\" checked=\"" << (current->getState_() ? "true" : "false") << "\">" << std::endl;
        }
    }
}

void XMLOperation::printIndent(const int indent)
{
    for(int i = 0; i < indent; ++i)
    {
        std::cout << "\t";
    }
}
