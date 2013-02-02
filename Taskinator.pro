#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T12:53:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = taskinator
TEMPLATE = app

SOURCES += main.cpp\
        UI/mainwindow.cpp\
        Model/component.cpp\
        Model/list.cpp\
        Model/sortedList.cpp\
        Model/task.cpp \
    UI/newlist.cpp

HEADERS += UI/mainwindow.h\
           Model/component.h\
           Model/list.h\
           Model/sortedList.h\
           Model/task.h \
    UI/newlist.h

FORMS += UI/mainwindow.ui \
    UI/newlist.ui

RESOURCES += \
    Resources.qrc

ICON = Pictures/taskinator.icns
