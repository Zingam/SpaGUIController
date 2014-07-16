#include "dialogsceneeditor.h"
#include "ui_dialogsceneeditor.h"

#include <QDebug>

DialogSceneEditor::DialogSceneEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSceneEditor)
{
    // Remove ? from TitleBar
    Qt::WindowFlags windowFlags = this->windowFlags();
    windowFlags &= ~Qt::WindowContextHelpButtonHint;

    this->setWindowFlags(windowFlags);

    // Setup UI
    ui->setupUi(this);

//    ui->tableWidget_SensorsSelected->insertRow();
//    ui->tableWidget_SensorsSelected->insertColumn();
//    QTableWidgetItem i;
//    i.
//    ui->tableWidget_SensorsSelected->setItem(0, 0, QTableWidgetItem k);
}

DialogSceneEditor::~DialogSceneEditor()
{
    delete ui;
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
