#include "resourcefilemanager.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>

ResourceFileManager::ResourceFileManager(QString resourceFolderPath, QObject* parent) :
    QObject(parent),
    _resourceFolderPath(resourceFolderPath)
{
}

QByteArray ResourceFileManager::loadFrom(const QString& resourceFileName, const QString& fileName)
{
    QString resourcePath = _resourceFolderPath + "/" + resourceFileName;
    QFileInfo path(resourcePath);
    qDebug() << "Resources: " + path.filePath();

    if (path.isFile()) {
        qDebug() << "Loading file \"" + fileName + "\" form resource file \"" + resourceFileName + "\"";
        return loadFromResourceFile(path.filePath(), fileName);
    }
    else if (path.isDir()) {
        qDebug() << "Loading file \"" + fileName + "\" from resource folder \"" + resourceFileName +"\"";
        return loadFromFolder(path.filePath(), fileName);
    }
    else {
        return nullptr;
    }
}

QByteArray ResourceFileManager::loadFromFolder(const QString& folderPath, const QString& fileName)
{
    QString fullFilePath = folderPath + "/" + fileName;
    QFile file(fullFilePath);
    qDebug() << "Loading resource: " + file.fileName();

    if (file.open(QIODevice::ReadOnly)) {

        QByteArray buffer = file.readAll();
        file.close();

        return buffer;
    }
    else {
        return nullptr;
    }
}

QByteArray ResourceFileManager::loadFromResourceFile(const QString& resourceFilePath, const QString& fileName)
{
    Q_UNUSED(resourceFilePath)
    Q_UNUSED(fileName)

    qDebug() << "ResourceFile::loadFromResourceFile - Not implemented";
    return nullptr;
}
