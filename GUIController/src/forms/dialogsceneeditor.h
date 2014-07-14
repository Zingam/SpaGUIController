#ifndef DIALOGSCENEEEDITOR_H
#define DIALOGSCENEEEDITOR_H

#include <QDialog>

namespace Ui {
class DialogSceneEditor;
}

class DialogSceneEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSceneEditor(QWidget *parent = 0);
    ~DialogSceneEditor();

private slots:
    void on_pushButton_ButtonBox_Close_clicked();
    void on_pushButton_ButtonBox_Save_clicked();
    void on_pushButton_Scenes_ButtonBox_Add_clicked();
    void on_pushButton_Scenes_ButtonBox_Delete_clicked();
    void on_pushButton_Sensors_ButtonBox_Add_clicked();
    void on_pushButton_Sensors_ButtonBox_Remove_clicked();

private:
    Ui::DialogSceneEditor *ui;
};

#endif // DIALOGSCENEEEDITOR_H
