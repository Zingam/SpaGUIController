#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "src/forms/mainwindow.h"

#include "src/scenedatafile.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("SpaCentralSoft");

    MainWindow* w;

    try {
        w = new MainWindow();
        w->show();
    }
    catch (ExceptionInitialization& exception) {
        QMessageBox::critical(nullptr,
                              QObject::tr("ERROR"),
                              exception.getMessage());
        return (-1);
    }

    int returnCode = a.exec();

    delete w;

    return returnCode;
}
