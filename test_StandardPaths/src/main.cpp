#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>

#include "src/forms/mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("SpaCentralSoft");

    MainWindow w;
    w.show();

    return a.exec();
}
