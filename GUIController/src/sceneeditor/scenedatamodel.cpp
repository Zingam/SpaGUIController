#include "scenedatamodel.h"

#include <QtCore/QFile>
#include <QtCore/QXmlStreamWriter>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSpacerItem>

#include <QtCore/QDebug>

SceneDataModel::SceneDataModel(QString filePath, QString fileName, QMainWindow* mainWindow):
    XmlLoader(QString(SCENEDATAMODEL_ERROR_MESSAGE )+ " " + fileName, mainWindow),
    _filePath(filePath),
    _fileName(fileName),
    _mainWindow(mainWindow)
{
}

void SceneDataModel::load()
{
    QFile fileSceneData(_filePath + "/" + _fileName);

    bool isOk;

    // Open SceneData file from device
    isOk = fileSceneData.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!isOk) {
        this->showFatalError("Failed to open " + _fileName + " in path: \n" + _filePath);
    }

    // Set config.xml as content of the dom document
    QString errorMessage;
    int errorLine;
    int errorColumn;

    isOk = _documentSceneData.setContent(&fileSceneData, false, &errorMessage, &errorLine, &errorColumn);
    if (!isOk) {
        this->showFatalError("Failed to read " + _fileName + " from device with:\n    "
                        + errorMessage
                        + "\nat line "
                        + QString::number(errorLine)
                        + ", column "
                        + QString::number(errorColumn));
    }

    qDebug() << "Begin parsing: " << _fileName;

    QDomElement elementScenes = this->firstChildElement("scenes", _documentSceneData);

    QDomNodeList listSceneElement = elementScenes.elementsByTagName("scene");

    for (int i = 0; listSceneElement.count() > i; i++) {
        QDomElement currentSceneElement = listSceneElement.at(i).toElement();
        if (currentSceneElement.isNull()) {
            this->showFatalError("Failed loading <scene>");
        }
        this->logToConsole(currentSceneElement);

        load_ParseScene(currentSceneElement);
    }

    qDebug() << "Succeeded parsing: " << _fileName;
}

void SceneDataModel::save()
{
    qDebug() << "Begin saving:" << _fileName;

    QFile fileSceneData(_filePath + "/" + _fileName);

    bool isOk;

    // Open SceneData file from device
    isOk = fileSceneData.open(QIODevice::WriteOnly);
    if (!isOk) {
        this->showFatalError("Failed to open " + _fileName + " in path: \n" + _filePath);

        return;
    }

    // Create XML
    QXmlStreamWriter fileSceneDataWriter;
    fileSceneDataWriter.setDevice(&fileSceneData);

    // Automatically format the XML document
    fileSceneDataWriter.setAutoFormatting(true);

    // Write XML document start with XML version number
    fileSceneDataWriter.writeStartDocument();

    // Start root tag <scenes>
    fileSceneDataWriter.writeStartElement("scenes");

    // Iterate through all <scene> in the list of scenes
    for (int currentSceneIndex = 0; _scenes.count() > currentSceneIndex; currentSceneIndex++) {
        // Start new <scene> tag
        fileSceneDataWriter.writeStartElement("scene");
        const Scene& currentScene = _scenes.at(currentSceneIndex);
        // Add "name" attribute to the current <scene> tag
        fileSceneDataWriter.writeAttribute("name", currentScene.name);

        // Iterate through all sensors in the current <scene>
        for (int currentSensorIndex = 0;
             _scenes.at(currentSceneIndex).sensors.count() > currentSensorIndex;
             currentSensorIndex++) {
            const Sensor& currentSensor = currentScene.sensors.at(currentSensorIndex);
            // Start new <sensor> tag
            fileSceneDataWriter.writeStartElement("sensor");
            // Add attributes to the current <sensor> tag
            fileSceneDataWriter.writeAttribute("id", QString::number(currentSensor.sensorId));
            fileSceneDataWriter.writeAttribute("target", QString::number(currentSensor.temperatureTarget));
            // Close <sensor/> tag
            fileSceneDataWriter.writeEndElement();
        }

        // Close <scene/> tag
        fileSceneDataWriter.writeEndElement();
    }

    // Close <scenes/> tag
    fileSceneDataWriter.writeEndElement();
    fileSceneDataWriter.writeEndDocument();

    qDebug() << "Succeeded saving: " << _fileName;
}

void SceneDataModel::load_ParseScene(QDomElement &element)
{
    Scene scene;
    scene.name = getAttributeValue("name", element);

    // Load all <sensor> from current <scene>
    QDomNodeList listChildElement = element.childNodes();

    // Iterate through all <sensor> in current <scene>
    for (int i = 0; listChildElement.count() > i; i++) {
        QDomElement currentChildElement = listChildElement.at(i).toElement();

        if (currentChildElement.isNull()) {
            this->showFatalError("Failed to load <sensor>");
        }
        this->logToConsole(currentChildElement);

        Sensor sensor;
        sensor.sceneName = scene.name;
        sensor.sensorId = getAttributeValue("id", currentChildElement).toInt();
        sensor.temperatureTarget = getAttributeValue("target", currentChildElement).toDouble();

        scene.sensors.push_back(sensor);
    }

    _scenes.push_back(scene);
}





