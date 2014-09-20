#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T11:03:05
#
#-------------------------------------------------

#DEFINES += DEBUG_MODE_FINETUNING
DEFINES += USE_GRAPHICSRECTITEM_ZONE
DEFINES += USE_HIGHLIGHTING
DEFINES += USE_NETWORKING

QMAKE_CXXFLAGS += -std=c++11

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += xml
QT += network

TARGET = GUIController
TEMPLATE = app

DESTDIR = ../../SpaCentralSoftBin

SOURCES += src/main.cpp\
    src/forms/dialogabout.cpp \
    src/forms/dialogtemperaturetarget.cpp \
    src/forms/mainwindow.cpp \
    src/graphics/cgraphicsrectitem.cpp \
    src/graphics/cgraphicsscene.cpp \
    src/graphics/temperatureindicator.cpp \
    src/initialization/configloader.cpp \
    src/initialization/scenedatafile.cpp \
    src/sceneeditor/doublespinboxdelegate.cpp \
    src/sceneeditor/scenedatamodel.cpp \
    src/sceneeditor/forms/dialogaddscene.cpp \
    src/sceneeditor/forms/dialogsceneeditor.cpp \
    src/utilities/resourcefilemanager.cpp \
    src/utilities/singleinstanceapplication.cpp \
    src/utilities/xmlloader.cpp

HEADERS  += \
    src/custom/constants.h \
    src/custom/structures.h \
    src/custom/types.h \
    src/forms/dialogabout.h \
    src/forms/dialogtemperaturetarget.h \
    src/forms/mainwindow.h \
    src/graphics/cgraphicsrectitem.h \
    src/graphics/cgraphicsscene.h \
    src/graphics/temperatureindicator.h \
    src/initialization/configloader.h \
    src/initialization/scenedatafile.h \
    src/sceneeditor/doublespinboxdelegate.h \
    src/sceneeditor/scenedatamodel.h \
    src/sceneeditor/forms/dialogaddscene.h \
    src/sceneeditor/forms/dialogsceneeditor.h \
    src/utilities/resourcefilemanager.h \
    src/utilities/singleinstanceapplication.h \
    src/utilities/utilities.h \
    src/utilities/xmlloader.h

FORMS    += src/forms/mainwindow.ui \
    src/forms/dialogabout.ui \
    src/forms/dialogtemperaturetarget.ui \
    src/sceneeditor/forms/dialogsceneeditor.ui \
    src/sceneeditor/forms/dialogaddscene.ui

TRANSLATIONS = languages/$${TARGET}_bg_BG.ts

RESOURCES += \
    res/resources.qrc

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
assets.files += $$files(assets/*.*)
INSTALLS += assets

assetsImagesDark.path = $$DESTDIR/assets/dark
assetsImagesDark.files += $$files(assets/dark/*.*)
INSTALLS += assetsImagesDark

assetsImagesLight.path = $$DESTDIR/assets/light
assetsImagesLight.files += $$files(assets/light/*.*)
INSTALLS += assetsImagesLight

assetsLanguages.path = $$DESTDIR/assets/languages
assetsLanguages.files += $$files(languages/*.qm)
INSTALLS += assetsLanguages

#copydata.commands = $(COPY_DIR) $$PWD/assets $$DESTDIR
#first.depends = $(first) copydata
#export(first.depends)
#export(copydata.commands)
#QMAKE_EXTRA_TARGETS += first copydata
