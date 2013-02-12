#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T12:53:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = taskinator
TEMPLATE = app

SOURCES += main.cpp \
		UI/mainwindow.cpp \
		UI/newlist.cpp \
		Model/component.cpp \
		Model/list.cpp \
		Model/sortedList.cpp \
        Model/task.cpp \
		Model/history.cpp \
    UI/element.cpp \
    Controleur/controleur.cpp

HEADERS += UI/mainwindow.h \
		UI/newlist.h \
		Model/component.h \
		Model/list.h \
		Model/sortedList.h \
		Model/task.h \
		Model/history.h \
    UI/element.h \
    Controleur/controleur.h

FORMS += UI/mainwindow.ui \
    UI/newlist.ui \
    UI/dateIncoherente.ui

RESOURCES += \
    Resources.qrc

ICON = Pictures/taskinator.icns
