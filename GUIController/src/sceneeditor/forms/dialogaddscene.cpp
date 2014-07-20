#include "dialogaddscene.h"
#include "ui_dialogaddscene.h"

DialogAddScene::DialogAddScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddScene)
{
    // Remove ? from TitleBar
    Qt::WindowFlags windowFlags = this->windowFlags();
    windowFlags &= ~Qt::WindowContextHelpButtonHint;

    this->setWindowFlags(windowFlags);

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
