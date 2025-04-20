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

INCLUDEPATH += \
    include \
    include/db \
    include/utils \
    include/style


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/MainPages.cpp \
    src/db/assemblydb.cpp \
    src/utils/callmessagehs.cpp \
    src/utils/HelperFuncHS.cpp \
    src/style/style_main.cpp

HEADERS  += \
    include/mainwindow.h \
    include/MainPages.h \
    include/db/assemblydb.h \
    include/utils/HelperFuncHS.h \
    include/utils/callmessagehs.h \
    include/style/style_main.h \
    include/style/style_main.h

FORMS    += \
    ui/mainwindow.ui

DISTFILES += \
    database/AssemblyComputer.sqlite
