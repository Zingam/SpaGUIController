#-------------------------------------------------
#
# Project created by QtCreator 2014-08-07T15:16:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_StandardPaths
TEMPLATE = app

DESTDIR = ../../SpaCentralSoftBin

SOURCES += src\main.cpp\
        src\forms\mainwindow.cpp \
    src/scenedatafile.cpp

HEADERS  += src\forms\mainwindow.h \
    src/scenedatafile.h

FORMS    += src\forms\mainwindow.ui

#CONFIG += copy_dir_files
#CONFIG += no_check_exist

#assets.path = $$DESTDIR/assets
#assets.files += $$files(assets/*.*)
#INSTALLS += assets
