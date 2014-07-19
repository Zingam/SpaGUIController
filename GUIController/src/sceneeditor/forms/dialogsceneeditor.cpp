#include "dialogsceneeditor.h"
#include "ui_dialogsceneeditor.h"

#include <QDebug>

#include "../../custom/structures.h"
#include "../scenedatamodel.h"

DialogSceneEditor::DialogSceneEditor(MainWindow* mainWindow) :
    QDialog(mainWindow),
    ui(new Ui::DialogSceneEditor),
    _mainWindow(mainWindow)
{
    // Remove ? from TitleBar
    Qt::WindowFlags windowFlags = this->windowFlags();
    windowFlags &= ~Qt::WindowContextHelpButtonHint;

    this->setWindowFlags(windowFlags);

    // Setup UI
    ui->setupUi(this);

    // Load all sensors into the listWidget_SensorsAll.
    // This list is static (does not change).
    this->listWidget_SensorsAll_Update();

    this->listWidget_Scenes_Update();
    const QString& defaultSceneName = _mainWindow->getSceneDataModel()->_scenes.at(0).name;
    this->tableWidget_SensorsSelected_Update(defaultSceneName);
}

DialogSceneEditor::~DialogSceneEditor()
{
    delete ui;
}

void DialogSceneEditor::listWidget_Scenes_Update()
{
    for (Scene currentScene: _mainWindow->getSceneDataModel()->_scenes) {
        ui->listWidget_Scenes->addItem(currentScene.name);
    }
}

void DialogSceneEditor::listWidget_SensorsAll_Update()
{
    for (Sensor currentSensor: _mainWindow->getSceneDataModel()->_sensors) {
        ui->listWidget_SensorsAll->addItem(currentSensor.text);
    }
}

void DialogSceneEditor::tableWidget_SensorsSelected_Update(const QString& sceneName)
{
    for (Scene currentScene: _mainWindow->getSceneDataModel()->_scenes) {
        if (sceneName == currentScene.name) {
            ui->tableWidget_SensorsSelected->setRowCount(currentScene.sensors.count());
            ui->tableWidget_SensorsSelected->setColumnCount(SENSORSSELECTED_COLUMNS);
            int row = 0;
            for (Sensor currentSensor: currentScene.sensors) {
                QTableWidgetItem* sensorText = new QTableWidgetItem(currentSensor.text);
                QString temperature = QString::number(currentSensor.temperatureTarget);
                QTableWidgetItem* sensorValue = new QTableWidgetItem(temperature);
                ui->tableWidget_SensorsSelected->setItem(row, 0, sensorText);
                ui->tableWidget_SensorsSelected->setItem(row, 1, sensorValue);
                ++row;
            }
        }
    }
}

void DialogSceneEditor::on_pushButton_ButtonBox_Close_clicked()
{
    this->close();
}

void DialogSceneEditor::on_pushButton_ButtonBox_Save_clicked()
{
    qDebug() << "Scenes saved";
}

void DialogSceneEditor::on_pushButton_Scenes_ButtonBox_Add_clicked()
{
    qDebug() << "Scene added";
}

void DialogSceneEditor::on_pushButton_Scenes_ButtonBox_Delete_clicked()
{
    qDebug() << "Scene deleted";
}

void DialogSceneEditor::on_pushButton_Sensors_ButtonBox_Add_clicked()
{
    qDebug() << "Sensor added";
}

void DialogSceneEditor::on_pushButton_Sensors_ButtonBox_Remove_clicked()
{
    qDebug() << "Sensor removed";
}
