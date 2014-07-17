#ifndef SCENEDATAMODEL_H
#define SCENEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QMainWindow>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

#include "../custom/structures.h"
#include "../utilities/xmlloader.h"

#define SCENEDATAMODEL_ERROR_MESSAGE "Failed to load "

class SceneDataModel : public QObject, public XmlLoader
{
    Q_OBJECT

public:
    explicit SceneDataModel(QString filePath, QString fileName, QMainWindow* mainWindow);

private:
    QString _filePath;
    QString _fileName;
    QMainWindow* _mainWindow;
    QDomDocument _documentSceneData;

    QList<Scene> _scenes;
    QList<Sensor> _sensors;

public:
    void load();
    void save();

private:
    void load_ParseScene(QDomElement& element);
};

#endif // SCENEDATAMODEL_H
