#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../scenedatafile.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Export_triggered();
    void on_action_Import_triggered();

private:
    Ui::MainWindow *ui;
    SceneDataFile* _sceneDataFile;
};

#endif // MAINWINDOW_H
