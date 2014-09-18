#include "resourcefilemanager.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>

ResourceFileManager::ResourceFileManager(QObject *parent) :
    QObject(parent)
{
}

QByteArray* ResourceFileManager::loadFrom(const QString& pathName, const QString& fileName)
{
    QFileInfo path(pathName);

    if (path.isFile()) {
        qDebug() << "Loading file \"" + fileName + "\" form resource file \"" + pathName + "\"";
        return loadFromFolder(pathName, fileName);
    }
    else if (path.isDir()) {
        qDebug() << "Loading file \"" + fileName + "\" from resource folder \"" + pathName +"\"";
        return loadFromResourceFile(pathName, fileName);
    }
    else {
        return nullptr;
    }
}

QByteArray* ResourceFileManager::loadFromFolder(const QString &folderPath, const QString &fileName)
{
    QString fullFilePath = folderPath + "/" + fileName;
    QFile file(fullFilePath);

    if (file.open(QIODevice::ReadOnly)) {
        return new QByteArray(file.readAll());
//        QByteArray buffer = std::move(file.readAll());
//        file.close();

//        return &buffer;
    }
    else {
        return nullptr;
    }
}

QByteArray* ResourceFileManager::loadFromResourceFile(const QString &resourceFilePath, const QString &fileName)
{
    Q_UNUSED(resourceFilePath);
    Q_UNUSED(fileName);

    qDebug() << "ResourceFile::loadFromResourceFile - Note implemented";
    return nullptr;
}
