#include "mainwindow.h"

#include <QtCore/QLibraryInfo>
#include <QtCore/QObject>
#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "custom/constants.h"
#include "utilities/singleinstanceapplication.h"

const QString GUID("140805-roccoor-ab8f8ec8-f505-41b0-9b67-916cfb54c88a-b5755918-24c4-4b05-bf96-f41d7065e241");

int main(int argc, char *argv[])
{
//    bool isOk;

    SingleInstanceApplication app(argc, argv, GUID, SingleInstanceApplication::SingletonType::LocalSocket);

//    QLocale locale(QLocale::Bulgarian, QLocale::Bulgaria);
//    QString localeName = locale.name();
//    QLocale localeEn(QLocale::English, QLocale::UnitedStates);
//    QString localeNameEn = localeEn.name();

//    QTranslator translatorQt;
//    QString translationFileNameQt("qt_" + localeNameEn);
//    QString locationQt = QLibraryInfo::location(QLibraryInfo::TranslationsPath);

//    isOk = translatorQt.load(translationFileNameQt,
//                             locationQt);
//    if (!isOk) {
//        qDebug() << "Failed to load translation:"
//                    << translationFileNameQt
//                        << " in location: "
//                            << locationQt;
//    }
//    else {
//        app.installTranslator(&translatorQt);
//        qDebug() << "Loaded translation:"
//                    << translationFileNameQt
//                        << " in location: "
//                            << locationQt;
//    }

//    QTranslator translatorApp;
//    QString translationFileNameApp(QString(APPLICATION_NAME)
//                                   + localeName);
//    QString locationApp("assets/languages");

//    isOk = translatorApp.load(translationFileNameApp);
//    if (!isOk) {
//        qDebug() << "Failed to load translation:" << translationFileNameApp
//                    << "in location: "
//                       << locationApp;
//    }
//    else {
//        app.installTranslator(&translatorApp);
//        qDebug() << "Loaded translation:" << translationFileNameApp
//                    << "in location: "
//                        << locationApp;
//    }


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
