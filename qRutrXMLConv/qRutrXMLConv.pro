#-------------------------------------------------
#
# Project created by QtCreator 2017-09-23T13:55:34
#
#-------------------------------------------------

QT       += core gui sql widgets

TARGET = qRutrXMLConv
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        widget.cpp \
    database.cpp \
    worker.cpp

HEADERS  += widget.h \
    database.h \
    rutritem.h \
    worker.h

FORMS    += widget.ui
