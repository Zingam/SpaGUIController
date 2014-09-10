#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QDebug>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#define ASSETS_PATH "assets/"
#define ASSETS_DATAFILENAME "SceneData.xml"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try {
        _sceneDataFile = SceneDataFile::getInstance(ASSETS_PATH,
                                                    ASSETS_DATAFILENAME,
                                                    this);
    }
    catch (SceneDataFileException& exception) {
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
    // Set the dialog for saving files, sets the dialog button to "Save"
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    // Set the dialog to return the file name wheather the file exists or not
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setNameFilter(tr("XML Data File") + " (*.xml)");

    bool wasFileSelected = fileDialog.exec();
    QString filePath;
    if (wasFileSelected) {
        filePath = fileDialog.selectedFiles().at(0);

        qDebug() << "Exporting file: " + filePath;
        try {
            _sceneDataFile->exportTo(filePath);
        }
        catch (SceneDataFileException& exception)
        {
            QMessageBox::critical(this,
                                  tr("ERROR: File Export Failed"),
                                  exception.getMessage());
        }
    }
}

void MainWindow::on_action_Import_triggered()
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle(tr("Export"));
    // Set the dialog to return the file name of a single existing file
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter(tr("XML Data File") + " (*.xml)");

    bool wasFileSelected = fileDialog.exec();
    QString filePath;
    if (wasFileSelected) {
        filePath = fileDialog.selectedFiles().at(0);

        qDebug() << "Importing file: " + filePath;
        try {
            _sceneDataFile->importFrom(filePath);
        }
        catch (SceneDataFileException& exception)
        {
            QMessageBox::critical(this,
                                  tr("ERROR: File Import Failed"),
                                  exception.getMessage());
        }
    }
}
