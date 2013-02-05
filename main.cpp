#include <QApplication>
#include "UI/mainwindow.h"
#include "Model/component.h"
#include "Model/list.h"
#include "Model/sortedList.h"
#include "Model/task.h"

int main(int argc, char *argv[])
{

    List * coursIHM = new List("Cours IHM",time(NULL));
    SortedList * CC = new SortedList("Preparer CC", time(NULL));
    coursIHM->addComponent(CC);
    /*std::cout << "2ème affichage" << std::endl;
    coursIHM->affichage(std::cout);*/

    Task * sujet = new Task("Preparer Sujet",time(NULL));
    Task * tirage = new Task("Tirer les copies", time(NULL));
    CC->addComponent(sujet);
    CC->addComponent(tirage);
    coursIHM->affichage(std::cout);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
