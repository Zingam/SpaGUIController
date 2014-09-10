#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "src/forms/mainwindow.h"

#include "src/scenedatafile.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("SpaCentralSoft");

    try {
        MainWindow w;
        w.show();

        return a.exec();
    }
    catch (SceneDataFileException& exception) {
        qDebug() << exception.getMessage();
        QMessageBox::critical(nullptr,
                              QObject::tr("ERROR"),
                              exception.getMessage());

        return (-1);
    }
}
