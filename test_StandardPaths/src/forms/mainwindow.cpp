#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QDebug>
#include <QtWidgets/QFileDialog>

#include "../scenedatafile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SceneDataFile sceneDataFile;
}

MainWindow::~MainWindow()
{
    delete ui;
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
