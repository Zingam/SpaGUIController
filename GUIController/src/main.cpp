#include "mainwindow.h"

#include <QtCore/QLibraryInfo>
#include <QtCore/QObject>
#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

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

    QLocale locale(QLocale::Bulgarian, QLocale::Bulgaria);
    QString localeName = locale.name();

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + localeName,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load("myapp_" + localeName);
    app.installTranslator(&appTranslator);

    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
