#-------------------------------------------------
#
# Project created by QtCreator 2017-09-22T15:06:04
#
#-------------------------------------------------

QT       += core gui sql webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qRutrGui
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    tablemodel.cpp \
    itemviewform.cpp \
    ../qRutrXMLConv/database.cpp \
    categorydelegate.cpp \
    databaseworker.cpp
HEADERS  += mainwindow.h \
    tablemodel.h \
    itemviewform.h \
    ../qRutrXMLConv/database.h \
    ../qRutrXMLConv/rutritem.h \
    categorydelegate.h \
    databaseworker.h

FORMS    += mainwindow.ui \
    itemviewform.ui

RESOURCES += \
    res.qrc
