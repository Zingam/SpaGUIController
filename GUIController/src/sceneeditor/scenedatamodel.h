#ifndef SCENEDATAMODEL_H
#define SCENEDATAMODEL_H

#include "../custom/structures.h"

#include <QMainWindow>
#include <QtXml/QDomDocument>

class SceneDataModel
{
public:
    SceneDataModel(QString filePath, QString fileName, QMainWindow* mainWindow);

private:
    QString _fileName;
    QMainWindow* _mainWindow;
    QDomDocument _dataDocument;

    QList<QString> _scenes;
    QList<Sensor> _sensors;

private:
    void load();
    void save();

    void logToConsole(const QDomElement& element) const;
    void logToConsole(const QDomAttr& attribute) const;
    void showFatalError(const QString errorMessage) const;
};

#endif // SCENEDATAMODEL_H
