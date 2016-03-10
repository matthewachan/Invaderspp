#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T17:52:49
#
#-------------------------------------------------

QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled2
TEMPLATE = app

CONFIG += resources_big

SOURCES += main.cpp\
        mainwindow.cpp \
    level1.cpp \
    level2.cpp \
    level3.cpp \
    victory.cpp \
    controls.cpp

HEADERS  += mainwindow.h \
    level1.h \
    level2.h \
    level3.h \
    victory.h \
    controls.h \
    documentation.h

FORMS    += mainwindow.ui \
    level1.ui \
    level2.ui \
    level3.ui \
    victory.ui \
    controls.ui

RESOURCES += \
    resources.qrc
