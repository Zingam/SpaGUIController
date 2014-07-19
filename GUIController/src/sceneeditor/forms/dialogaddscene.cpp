#include "dialogaddscene.h"
#include "ui_dialogaddscene.h"

DialogAddScene::DialogAddScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddScene)
{
    ui->setupUi(this);
}

DialogAddScene::~DialogAddScene()
{
    delete ui;
}

QString DialogAddScene::getSceneName()
{
    return ui->lineEdit->text();
}
