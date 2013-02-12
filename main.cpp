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
    Task * copie = new Task("Corriger copie", time(NULL));
    Task * note = new Task("Mettre bonne note", time(NULL));
    coursIHM->addComponent(copie);
    coursIHM->addComponent(note);

    Task * sujet = new Task("Preparer Sujet",time(NULL));
    Task * tirage = new Task("Tirer les copies", time(NULL));
    Task * apporterCopie = new Task("Apporter les copies", time(NULL));
    CC->addComponent(sujet);
    CC->addComponent(tirage);
    CC->addComponent(apporterCopie);
    coursIHM->affichage(std::cout);

    std::cout << "\n \n" << std::endl;

    apporterCopie->upComponent();
    apporterCopie->upComponent();
    apporterCopie->upComponent();
    apporterCopie->downComponent();
    coursIHM->affichage(std::cout);

    std::cout << "\n \n" << std::endl;
    CC->downComponent();
    coursIHM->affichage(std::cout);

    std::cout << "\n \n" << std::endl;
    sujet->check(true);
    coursIHM->affichage(std::cout);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
