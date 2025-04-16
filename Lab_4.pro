#-------------------------------------------------
#
# Project created by QtCreator 2025-04-09T21:35:57
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab_4
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    assemblydb.cpp \
    callmessagehs.cpp \
    style_main.cpp \
    MainPages.cpp

HEADERS  += mainwindow.h \
    assemblydb.h \
    callmessagehs.h \
    style_main.h \
    MainPages.h

FORMS    += mainwindow.ui

DISTFILES += \
    AssemblyComputer.sqlite
