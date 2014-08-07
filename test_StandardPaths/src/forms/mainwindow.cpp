#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets/QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    fileDialog.exec();
}

void MainWindow::on_action_Import_triggered()
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle(tr("Export"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter(tr("XML Data File") + " (*.xml)");

    fileDialog.exec();
}
