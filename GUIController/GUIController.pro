#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T11:03:05
#
#-------------------------------------------------

#DEFINES += DEBUG_MODE_FINETUNING
DEFINES += USE_GRAPHICSRECTITEM_ZONE
DEFINES += USE_HIGHLIGHTING


QT += core gui
QT += xml
QT += network

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUIController
TEMPLATE = app

DESTDIR = ../../SpaCentralSoftBin

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/graphics/cgraphicsrectitem.cpp \
    src/graphics/cgraphicsscene.cpp \
    src/graphics/temperatureindicator.cpp \
    src/forms/dialogabout.cpp \
    src/forms/dialogtemperaturetarget.cpp \
    src/sceneeditor/forms/dialogsceneeditor.cpp \
    src/sceneeditor/scenedatamodel.cpp \
    src/utilities/configloader.cpp \
    src/utilities/xmlloader.cpp \
    src/sceneeditor/spinboxdelegate.cpp \
    src/sceneeditor/forms/dialogaddscene.cpp \
    src/utilities/singleinstanceapplication.cpp

HEADERS  += src/mainwindow.h \
    src/custom/constants.h \
    src/custom/structures.h \
    src/custom/types.h \
    src/graphics/cgraphicsrectitem.h \
    src/graphics/cgraphicsscene.h \
    src/graphics/temperatureindicator.h \
    src/forms/dialogabout.h \
    src/forms/dialogtemperaturetarget.h \
    src/sceneeditor/scenedatamodel.h \
    src/sceneeditor/forms/dialogsceneeditor.h \
    src/utlities/configloader.h \
    src/utilities/xmlloader.h \
    src/sceneeditor/forms/dialogaddscene.h \
    src/custom/utilities.h \
    src/sceneeditor/doublespinboxdelegate.h \
    src/utilities/singleinstanceapplication.h

FORMS    += src/forms/mainwindow.ui \
    src/forms/dialogabout.ui \
    src/forms/dialogtemperaturetarget.ui \
    src/sceneeditor/forms/dialogsceneeditor.ui \
    src/sceneeditor/forms/dialogaddscene.ui

TRANSLATIONS = languages/GUIController_en.ts \
    languages/GUIController_bg.ts

###################################################################################################
# Copy assets to DESTDIR
###################################################################################################

## If using Shadow build, get the output folder
#CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
#CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
#
## If using normal build (non-Shadow) that would have worked as well.
#CONFIG(release, debug|release): DESTDIR = release
#CONFIG(debug, debug|release): DESTDIR = debug
#
# Finally add "make install" as a post-build step

CONFIG += copy_dir_files # Enables the install rule to also copy directories, not just files
CONFIG += no_check_exist

assets.path = $$DESTDIR/assets
assets.files +=$$files(assets/*.*)
INSTALLS += assets

assetsImagesDark.path = $$DESTDIR/assets/dark
assetsImagesDark.files +=$$files(assets/dark/*.*)
INSTALLS += assetsImagesDark

assetsImagesLight.path = $$DESTDIR/assets/light
assetsImagesLight.files +=$$files(assets/light/*.*)
INSTALLS += assetsImagesLight

#copydata.commands = $(COPY_DIR) $$PWD/assets $$DESTDIR
#first.depends = $(first) copydata
#export(first.depends)
#export(copydata.commands)
#QMAKE_EXTRA_TARGETS += first copydata
