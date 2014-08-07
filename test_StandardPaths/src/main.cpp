#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>

#include "src/forms/mainwindow.h"

#define ASSETS_PATH "assets/"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("SpaCentralSoft");


    SceneDataFile sceneDataFile;

    MainWindow w;
    w.show();

    return a.exec();
}
