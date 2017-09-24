#-------------------------------------------------
#
# Project created by QtCreator 2017-09-23T13:55:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qRutrXMLConv
TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp \
    database.cpp \
    worker.cpp

HEADERS  += widget.h \
    database.h \
    rutritem.h \
    worker.h

FORMS    += widget.ui
