#ifndef SCENEDATAMODEL_H
#define SCENEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QMainWindow>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

#include "forms/dialogsceneeditor.h"

#include "../custom/structures.h"

#include "../mainwindow.h"
#include "../utilities/xmlloader.h"

#define SCENEDATAMODEL_ERROR_MESSAGE "Failed to load "

class MainWindow;


class SceneDataModel : public QObject, public XmlLoader
{
    Q_OBJECT

    friend class DialogSceneEditor;
    friend class MainWindow;

public:
    explicit SceneDataModel(QString filePath, QString fileName, MainWindow* mainWindow);
    ~SceneDataModel();

private:
    QString _filePath;
    QString _fileName;
    MainWindow* _mainWindow;
    QDomDocument _documentSceneData;

    QList<Scene> _scenes;
    QList<Sensor> _sensors;

public:
    Scene* getSceneByName(QString sceneName);

private:
    QString getSensorNameById(quint8 id);
    void getSensors();
    void xmlSceneDataFileLoad();
    void xmlSceneDataFileSave();
    void xmlSceneDataParse(QDomElement& element);
};

#endif // SCENEDATAMODEL_H
