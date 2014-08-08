#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QDebug>
#include <QtWidgets/QFileDialog>

#define ASSETS_PATH "assets/"
#define ASSETS_DATAFILENAME "SceneData.xml"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try {

        _sceneDataFile = SceneDataFile::getInstance(ASSETS_PATH, ASSETS_DATAFILENAME, parent);
    }
    catch (ExceptionInitialization& exception) {
        throw exception;
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    qDebug() << "Destroying: MainWindow";
}

void MainWindow::on_action_Export_triggered()
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle(tr("Export"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setNameFilter(tr("XML Data File") + " (*.xml)");

    bool wasFileSelected = fileDialog.exec();
    QString filePath;
    if (wasFileSelected) {
        filePath = fileDialog.selectedFiles().at(0);
    }

    qDebug() << filePath;
}

void MainWindow::on_action_Import_triggered()
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle(tr("Export"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter(tr("XML Data File") + " (*.xml)");

    bool wasFileSelected = fileDialog.exec();
    QString filePath;
    if (wasFileSelected) {
        filePath = fileDialog.selectedFiles().at(0);
    }

    qDebug() << filePath;
}
