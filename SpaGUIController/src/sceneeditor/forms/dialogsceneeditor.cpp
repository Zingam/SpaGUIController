#include "dialogsceneeditor.h"
#include "ui_dialogsceneeditor.h"

#include <QtCore/QDebug>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QMessageBox>

#include "../../custom/structures.h"
#include "../../utilities/utilities.h"

#include "dialogaddscene.h"
#include "../doublespinboxdelegate.h"
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

    ui->pushButton_ButtonBox_Save->setEnabled(false);
    ui->pushButton_Sensors_ButtonBox_Add->setEnabled(false);
    ui->pushButton_Sensors_ButtonBox_Remove->setEnabled(false);
    ui->listWidget_SensorsAll->setEnabled(false);

    // Load all sensors into the listWidget_SensorsAll.
    // This list is static (does not change).
    this->listWidget_SensorsAll_Update();

    this->listWidget_Scenes_Update();
}

DialogSceneEditor::~DialogSceneEditor()
{
    delete ui;
}

void DialogSceneEditor::listWidget_Scenes_Update()
{
    for (Scene& currentScene: _mainWindow->getSceneDataModel()->_scenes) {
        QListWidgetItem* currentSceneName = new QListWidgetItem(currentScene.name);
        ui->listWidget_Scenes->addItem(currentSceneName);
    }
}

void DialogSceneEditor::listWidget_SensorsAll_Update()
{
    ui->listWidget_SensorsAll->clear();
    for (Sensor& currentSensor: _mainWindow->getSceneDataModel()->_sensors) {
        bool isSensorInTable = false;
        for( int row = 0; row < ui->tableWidget_SensorsSelected->rowCount(); row ++ ) {
            if (ui->tableWidget_SensorsSelected->item(row, 0)->text() == currentSensor.text) {
                isSensorInTable = true;

                break;
            }
        }

        if (!isSensorInTable) {
            ui->listWidget_SensorsAll->addItem(currentSensor.text);
        }
    }
}

void DialogSceneEditor::tableWidget_SensorsSelected_Update()
{
    ui->tableWidget_SensorsSelected->setRowCount(_currentScene->sensors.count());
    ui->tableWidget_SensorsSelected->setColumnCount(SENSORSSELECTED_COLUMNS);

    int currentRowIndex = 0;

    for (Sensor& currentSensor: _currentScene->sensors) {
        tableWidget_SensorsSelected_SetRow(currentRowIndex, currentSensor);
        ++currentRowIndex;
    }
}

void DialogSceneEditor::tableWidget_SensorsSelected_SetRow(int row, Sensor &currentSensor)
{
    QTableWidgetItem* sensorText = new QTableWidgetItem(currentSensor.text);
    Qt::ItemFlags flags = sensorText->flags();
    flags = flags & (~Qt::ItemIsEditable);
    sensorText->setFlags(flags);
    ui->tableWidget_SensorsSelected->setItem(row, 0, sensorText);

    QString temperature = QString::number(currentSensor.temperatureTarget);
    QTableWidgetItem* sensorValue = new QTableWidgetItem(temperature);
    sensorValue->setData(Qt::UserRole, QVariant(currentSensor.sensorId));
    ui->tableWidget_SensorsSelected->setItem(row, 1, sensorValue);
    // This adds QDoubleSpinBox widget to the second column as an Input Widget
    DoubleSpinBoxDelegate* doubleSpinBoxDelegate = new DoubleSpinBoxDelegate();
    ui->tableWidget_SensorsSelected->setItemDelegateForColumn(1, doubleSpinBoxDelegate);

    for (int currentItemIndex = 0;
         ui->listWidget_SensorsAll->count() > currentItemIndex;
         currentItemIndex ++)
    {
        QListWidgetItem* listWidgettem = ui->listWidget_SensorsAll->item(currentItemIndex);

        if (listWidgettem->text() == currentSensor.text) {
            delete_safe(listWidgettem);

            break;
        }
    }
}

void DialogSceneEditor::on_pushButton_ButtonBox_Close_clicked()
{
    this->sceneTrySave();

    _mainWindow->getSceneDataModel()->xmlSceneDataFileSave();

    this->close();
}

void DialogSceneEditor::sceneTrySave()
{
    // Check if user wants to save current scene
    if (nullptr != _currentScene
            && _tableSensorsModified)
    {
        int answer = QMessageBox::question(this,
                                           "Save before change?",
                                           "Current scene has been modified."
                                           "Do you want to save changes?",
                                           QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::No);
        if (QMessageBox::Yes != answer) {
            return;
        }
    }
    else {
        return;
    }

    // Continue saving current scene
    _currentScene->sensors.clear();

    for (int currentRowIndex = 0;
         ui->tableWidget_SensorsSelected->rowCount() > currentRowIndex;
         currentRowIndex++)
    {
        // Get sensor name from first column
        QTableWidgetItem *currentItem = ui->tableWidget_SensorsSelected->item(currentRowIndex, 0);

        Sensor sensor;
        sensor.text = currentItem->text();

        // Get target temperature from second column
        currentItem = ui->tableWidget_SensorsSelected->item(currentRowIndex, 1);

        sensor.temperatureTarget = currentItem->text().toDouble();
        sensor.sensorId = (quint8) currentItem->data(Qt::UserRole).toInt();

        _currentScene->sensors.push_back(sensor);
    }
}

void DialogSceneEditor::on_listWidget_Scenes_itemSelectionChanged()
{
    QListWidgetItem* currentItem = ui->listWidget_Scenes->currentItem();

    QString currentSceneName = currentItem->text();

    // Try saving current scene if modified
    this->sceneTrySave();

    // Find current scene pointer by selected string in scenes list
    _currentScene = _mainWindow->getSceneDataModel()->getSceneByName(currentSceneName);

    // Enable/disable Save button, depending on scene selection
    ui->pushButton_ButtonBox_Save->setEnabled(_currentScene != nullptr );
    ui->pushButton_Sensors_ButtonBox_Add->setEnabled(_currentScene != nullptr);
    ui->pushButton_Sensors_ButtonBox_Remove->setEnabled(_currentScene != nullptr);
    ui->listWidget_SensorsAll->setEnabled(_currentScene != nullptr);


    // Update sensors table if new scene is selected
    if (_currentScene) {
        this->tableWidget_SensorsSelected_Update();

        setWindowTitle(QString("%1").arg(currentItem->text()));

        _tableSensorsModified = false;
    }

    this->listWidget_SensorsAll_Update();
}

void DialogSceneEditor::on_pushButton_ButtonBox_Save_clicked()
{
    // Try saving the current scene if modified
    this->sceneTrySave();
}

void DialogSceneEditor::on_pushButton_Scenes_ButtonBox_Add_clicked()
{
    DialogAddScene dialogAddScene;

    if (dialogAddScene.exec())
    {
        QString sceneName = dialogAddScene.getSceneName();
        for (Scene& currentScene: _mainWindow->getSceneDataModel()->_scenes)
        {
            if (sceneName == currentScene.name)
            {
                QMessageBox::critical(this,
                                      "Error",
                                      "Scene with same name already exists!");

                this->on_pushButton_Scenes_ButtonBox_Add_clicked();

                return;
            }
        }

        Scene scene;
        scene.name = dialogAddScene.getSceneName();
        _mainWindow->getSceneDataModel()->_scenes.push_back(scene);
        ui->listWidget_Scenes->addItem(scene.name);
    }
}

void DialogSceneEditor::on_pushButton_Scenes_ButtonBox_Delete_clicked()
{
    if (_currentScene == nullptr) {
        QMessageBox::critical(this,
                              "Error delete scene",
                              "Scene not selected.");

        return;
    }

    qDebug() << _currentScene->name;

    int answer = QMessageBox::critical(this,
                                       "Are you sure",
                                       QString("Delete scene \"%1\" ?")
                                       .arg(_currentScene->name),
                                       QMessageBox::Ok,
                                       QMessageBox::Cancel);
    if (QMessageBox::Ok != answer) {
        return;
    }

    Scene* _scenePtr = _currentScene;

    // Remove from list widget
    for (int currentRowIndex = 0;
         ui->listWidget_Scenes->count() > currentRowIndex;
         currentRowIndex++)
    {
        QListWidgetItem* item = ui->listWidget_Scenes->item(currentRowIndex);
        if (item->text() == _currentScene->name)
        {
            _currentScene = nullptr;
            delete_safe(item);
            break;
        }
    }

    // Remove from scenes list
    for( int sceneIndex =0;
         _mainWindow->getSceneDataModel()->_scenes.count() > sceneIndex;
         sceneIndex ++ )
    {
        const Scene* scene = &_mainWindow->getSceneDataModel()->_scenes.at(sceneIndex);
        if (scene == _scenePtr)
        {
            _mainWindow->getSceneDataModel()->_scenes.removeAt(sceneIndex);
            break;
        }
    }
}

void DialogSceneEditor::on_pushButton_Sensors_ButtonBox_Add_clicked()
{
    QListWidgetItem *currentItem = ui->listWidget_SensorsAll->currentItem();
    if (currentItem == nullptr)
        return;

    _tableSensorsModified = true;

    QString selectedSensorText = currentItem->text();

    for (Sensor& sensor:  _mainWindow->getSceneDataModel()->_sensors) {
        if (sensor.text == selectedSensorText)
        {
            int rowsCount = ui->tableWidget_SensorsSelected->rowCount();
            ui->tableWidget_SensorsSelected->insertRow(rowsCount);
            tableWidget_SensorsSelected_SetRow(rowsCount, sensor);
        }
    }

    qDebug() << "Sensor added";
}

void DialogSceneEditor::on_pushButton_Sensors_ButtonBox_Remove_clicked()
{
    _tableSensorsModified = true;

    QList<QTableWidgetItem*> itemsList = ui->tableWidget_SensorsSelected->selectedItems();

    if (itemsList.count() < 1)
        return;

    QTableWidgetItem* item = *itemsList.begin();

    int rowNumber = item->row();

    ui->tableWidget_SensorsSelected->removeRow(rowNumber);

    listWidget_SensorsAll_Update();

    qDebug() << "Sensor removed:" << rowNumber;
}

void DialogSceneEditor::on_tableWidget_SensorsSelected_cellChanged(int row, int column)
{
#ifdef DEBUG
    qDebug() << QString("Scene Editor: Table cell changed: row: %1, column: %2")
                .arg(row)
                .arg(column);
#else
    Q_UNUSED(row);
    Q_UNUSED(column);
#endif // DEBUG

    _tableSensorsModified = true;
}
