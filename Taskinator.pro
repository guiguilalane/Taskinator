#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T12:53:21
#
#-------------------------------------------------

#Permet de définir une variable global du projet qui contient
#le chemin vers ce fichier(utilisée dans le fichier MainWindow.ui


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
		Lib/pugixml_sources/pugixml.cpp \
		Controleur/xmloperation.cpp \
		Controleur/controleur.cpp \
		UI/mytreewidget.cpp

HEADERS += UI/mainwindow.h \
		UI/newlist.h \
		Model/component.h \
		Model/list.h \
		Model/sortedList.h \
		Model/task.h \
		Model/history.h \
		UI/element.h \
		Lib/pugixml_sources/pugiconfig.hpp \
		Lib/pugixml_sources/pugixml.hpp \
		Controleur/xmloperation.h \
		Controleur/controleur.h \
    UI/mytreewidget.h

FORMS += UI/mainwindow.ui \
    UI/newlist.ui \
    UI/dateIncoherente.ui

RESOURCES += \
    Resources.qrc

ICON = Pictures/taskinator.icns

UI_HEADERS_DIR = $$PWD/UIHeaderFile
UI_DIR = $$PWD/UIHeaderFile
message($$UI_HEADERS_DIR)
message($$UI_DIR)
