#include "scenedatafile.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QMessageBox>


SceneDataFile::SceneDataFile(QString sceneDataFilePath,
                             QString sceneDataFileName,
                             QObject* parent) :
    QObject(parent),
    _sceneDataFileName(sceneDataFileName)
{
    bool doesExist = false;

    QString sceneDataFileStandardPath =
            QStandardPaths::writableLocation(QStandardPaths::DataLocation);

    QDir standardDataLocationDir;
    doesExist = standardDataLocationDir.exists(sceneDataFileStandardPath);
    if (!doesExist) {
        bool isOk = standardDataLocationDir.mkdir(sceneDataFileStandardPath);
        if (!isOk) {
            QString errorMessage =
                    QObject::tr("Unable to create standard folder:") + "\n"
                    + sceneDataFileStandardPath;

            throw ExceptionInitialization(errorMessage);
        }
    }

    QString sceneDataFileFullPath(sceneDataFileStandardPath + "/" + _sceneDataFileName);
    QString sceneDataFileSpareFullPath(sceneDataFilePath + "/" + _sceneDataFileName);

    QString errorMessage =
            " " + sceneDataFileName + "\n"
            + QObject::tr("Unable to restore file from default source!") + "\n"
            + QObject::tr("Please reinstall this application!");

    QFile sceneDataFile;
    doesExist = sceneDataFile.exists(sceneDataFileFullPath);
    if (!doesExist) {
        bool fileExists = sceneDataFile.exists(sceneDataFileSpareFullPath);
        if (fileExists) {
            bool isOk = sceneDataFile.copy(sceneDataFileSpareFullPath,
                                           sceneDataFileFullPath);
            if (!isOk) {
                errorMessage.prepend(tr("Unable to copy file:"));

                throw ExceptionInitialization(errorMessage);
            }
        }
        else {
            errorMessage.prepend(QObject::tr("File not found:"));

            throw ExceptionInitialization(errorMessage);
        }
    }
}

SceneDataFile::~SceneDataFile()
{
     qDebug() << "SceneDataFile";
}

SceneDataFile* SceneDataFile::getInstance(QString sceneDataFilePath,
                                          QString sceneDataFileName,
                                          QObject* parent)
{
    static SceneDataFile* _instance =
            new SceneDataFile(sceneDataFilePath,
                              sceneDataFileName,
                              parent);
    return _instance;
}

void SceneDataFile::exportTo(QString filePath)
{
    QFile sceneDataFile(_sceneDataFileName);
    sceneDataFile.copy(filePath);
}

void SceneDataFile::importFrom(QString filePath)
{
    QString sceneDataFileStandardPath =
            QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QString sceneDataFileFullPath(sceneDataFileStandardPath + "/" + _sceneDataFileName);
    QFile sceneDataFile;
    bool doesExist = sceneDataFile.exists(sceneDataFileFullPath);
    if (doesExist) {
        sceneDataFile.remove(sceneDataFileFullPath);
    }

    sceneDataFile.copy(filePath, sceneDataFileFullPath);
}
