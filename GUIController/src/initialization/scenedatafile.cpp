#include "scenedatafile.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QStandardPaths>


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

            throw SceneDataFileException(errorMessage);
        }
    }

    _sceneDataFileFullPath = sceneDataFileStandardPath + "/" + _sceneDataFileName;
    QString sceneDataFileSpareFullPath(sceneDataFilePath + "/" + _sceneDataFileName);

    QString errorMessage =
            " " + sceneDataFileName + "\n"
            + QObject::tr("Unable to restore file from default source!") + "\n"
            + QObject::tr("Please reinstall this application!");

    QFile sceneDataFile;
    doesExist = sceneDataFile.exists(_sceneDataFileFullPath);
    if (!doesExist) {
        bool fileExists = sceneDataFile.exists(sceneDataFileSpareFullPath);
        if (fileExists) {
            bool isOk = sceneDataFile.copy(sceneDataFileSpareFullPath,
                                           _sceneDataFileFullPath);
            if (!isOk) {
                errorMessage.prepend(tr("Unable to copy file:"));

                throw SceneDataFileException(errorMessage);
            }
        }
        else {
            errorMessage.prepend(QObject::tr("File not found:"));

            throw SceneDataFileException(errorMessage);
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
    QFile sceneDataFile(_sceneDataFileFullPath);

    bool isOk = sceneDataFile.copy(filePath);
    if (!isOk) {
        QString errorMessage =
                QObject::tr("Unable to export file:") + "\n"
                + filePath;

        throw SceneDataFileException(errorMessage);
    }
}

void SceneDataFile::importFrom(QString filePath)
{
    bool isOk;

    QFile sceneDataFile(_sceneDataFileFullPath);

    bool doesExist = sceneDataFile.exists();
    if (doesExist) {
        isOk = sceneDataFile.remove(_sceneDataFileFullPath);
        if (!isOk) {
            QString errorMessage =
                    QObject::tr("Removing old file failed.") + "\n"
                    + QObject::tr("Unable to import file:") + "\n"
                    + filePath;

            throw SceneDataFileException(errorMessage);

            return;
        }
    }

    isOk = sceneDataFile.copy(filePath, _sceneDataFileFullPath);
    if (!isOk) {
        QString errorMessage =
                QObject::tr("Unable to copy file.") + "\n"
                + QObject::tr("Unable to import file:") + "\n"
                + filePath;

        throw SceneDataFileException(errorMessage);
    }
}
