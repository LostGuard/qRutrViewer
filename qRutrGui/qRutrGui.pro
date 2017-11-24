#-------------------------------------------------
#
# Project created by QtCreator 2017-09-22T15:06:04
#
#-------------------------------------------------

QT       += core gui sql webkitwidgets widgets

TARGET = qRutrGui
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    tablemodel.cpp \
    itemviewform.cpp \
    ../qRutrXMLConv/database.cpp \
    databaseworker.cpp \
    categorydelegate.cpp \
    settingswindow.cpp \
    settings.cpp
HEADERS  += mainwindow.h \
    tablemodel.h \
    itemviewform.h \
    ../qRutrXMLConv/database.h \
    ../qRutrXMLConv/rutritem.h \
    databaseworker.h \
    categorydelegate.h \
    settingswindow.h \
    settings.h

FORMS    += mainwindow.ui \
    itemviewform.ui \
    settingswindow.ui

RESOURCES += \
    res.qrc
