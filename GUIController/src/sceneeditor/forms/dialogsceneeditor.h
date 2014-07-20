#ifndef DIALOGSCENEEEDITOR_H
#define DIALOGSCENEEEDITOR_H

#include <QtWidgets/QDialog>

#include "../scenedatamodel.h"
#include "../../mainwindow.h"

#define SENSORSSELECTED_COLUMNS 2

namespace Ui {
class DialogSceneEditor;
}

class MainWindow;

class DialogSceneEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSceneEditor(MainWindow* mainWindow);
    ~DialogSceneEditor();

private:
    void listWidget_Scenes_Update();
    void listWidget_SensorsAll_Update();
    void tableWidget_SensorsSelected_Update();
    void tableWidget_SensorsSelected_SetRow(int row, Sensor& currentSensor);
    void saveScene();

private slots:
    void on_pushButton_ButtonBox_Close_clicked();
    void on_pushButton_ButtonBox_Save_clicked();
    void on_pushButton_Scenes_ButtonBox_Add_clicked();
    void on_pushButton_Scenes_ButtonBox_Delete_clicked();
    void on_pushButton_Sensors_ButtonBox_Add_clicked();
    void on_pushButton_Sensors_ButtonBox_Remove_clicked();

    void on_listWidget_Scenes_itemClicked(QListWidgetItem *item);

    void on_tableWidget_SensorsSelected_cellChanged(int row, int column);

private:
    Ui::DialogSceneEditor *ui;

    MainWindow* _mainWindow;

    Scene* _currentScene = nullptr;
    bool _tableSensorsModified = false;
    bool _hasUnsavedChanges = false;
};

#endif // DIALOGSCENEEEDITOR_H
