#include "scenedatafile.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>


SceneDataFile::SceneDataFile(QObject *parent) :
    QObject(parent)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    qDebug() << path;
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
