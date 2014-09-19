#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T18:19:24
#
#-------------------------------------------------

QMAKE_CXXFLAGS = -std=c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_ResourceFileManager
TEMPLATE = app


SOURCES += src/main.cpp\
    src/forms/mainwindow.cpp \
    src/utilities/resourcefilemanager.cpp

HEADERS  += src/forms/mainwindow.h \
    src/utilities/resourcefilemanager.h

FORMS    += src/forms/mainwindow.ui
