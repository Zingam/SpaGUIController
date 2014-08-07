#include "scenedatafile.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>


SceneDataFile::SceneDataFile(QString sceneDataFilePath,
                             QString sceneDataFileName,
                             QObject* parent) :
    QObject(parent),
    _sceneDataFileName(sceneDataFileName)
{
    QString sceneDataFilePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    qDebug() << path;
    QFile sceneDataFile(sceneDataFilePath + "/" + sceneDataFileName);
    bool fileExists = sceneDataFile.exists();
    if (!fileExists) {
        sceneDataFile
    }
}

void SceneDataFile::exportTo(QString filePath)
{
    QFile sceneDataFile(_sceneDataFileName);
    sceneDataFile.copy(filePath);
}

void SceneDataFile::importFrom(QString filePath)
{
    QFile sceneDataFile(filePath);
    sceneDataFile.copy(_sceneDataFileName);
}
