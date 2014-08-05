#include "mainwindow.h"

#include <QtCore/QLibraryInfo>
#include <QtCore/QObject>
#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "custom/constants.h"
#include "utilities/singleinstanceapplication.h"

int main(int argc, char *argv[])
{
    bool isOk;
    QString language = "bg";

    SingleInstanceApplication app(argc,
                                  argv,
                                  QString(GUID),
                                  SingleInstanceApplication::SingletonType::LocalSocket);

    QLocale locale;
    if ("bg" == language) {
        locale = QLocale(QLocale::Bulgarian, QLocale::Bulgaria);
    }
    else {
        locale = QLocale(QLocale::English, QLocale::UnitedStates);
    }

    // Load translation of the application for the selected language
    QString translationFileNameApp(QString(APPLICATION_NAME)
                                   + locale.name());
    QString translationPathApp("assets/languages");

    QTranslator translatorApp;
    isOk = translatorApp.load(translationFileNameApp, translationPathApp);
    if (isOk) {
        app.installTranslator(&translatorApp);
        qDebug() << "Loaded translation:" << translationFileNameApp
                    << "in location: "
                        << translationPathApp;
    }
    else {
        qDebug() << "Failed to load translation:" << translationFileNameApp
                    << "in location: "
                        << translationPathApp;
    }

    // Check if the application was already instanciated once and prevent a second instance
    if (app.isInstanciated()) {
        QString titleMessage(QObject::tr("ERROR: Application is already instanciated!"));
        QString message(QObject::tr("Application is already instanciated!!!"));
        message += "\n" + QObject::tr("Can't run a second instance.");
        QMessageBox::critical(nullptr, titleMessage, message);

        return -1;
    }

    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
