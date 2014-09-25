#ifndef DIALOGADDSCENE_H
#define DIALOGADDSCENE_H

#include <QtWidgets/QDialog>

namespace Ui {
class DialogAddScene;
}


class DialogAddScene : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddScene(QWidget *parent = 0);
    ~DialogAddScene();

    QString getSceneName();

private:
    Ui::DialogAddScene *ui;

    QString _sceneName;
};

#endif // DIALOGADDSCENE_H
