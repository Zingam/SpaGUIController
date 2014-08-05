#include "mainwindow.h"

#include <QtWidgets/QApplication>
#include "utilities/singleinstanceapplication.h"

const QString GUID("140805-roccoor-ab8f8ec8-f505-41b0-9b67-916cfb54c88a-b5755918-24c4-4b05-bf96-f41d7065e241");

int main(int argc, char *argv[])
{
    SingleInstanceApplication app(argc, argv, GUID, SingleInstanceApplication::SingletonType::LocalSocket);
    if (app.isInstanciated()) {
        QString titleMessage(QObject::tr("ERROR: Application is already instanciated!"));
        QString message(QObject::tr("Application is already instanciated!!!"));
        message += QObject::tr("\nCan't run a second instance.");
        QMessageBox::critical(nullptr, titleMessage, message);

        return -1;
    }

    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
