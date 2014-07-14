#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QString>

DialogAbout::DialogAbout(const ProgramSettings& programSettings,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    // Remove ? from TitleBar
    Qt::WindowFlags windowFlags = this->windowFlags();
    windowFlags &= ~Qt::WindowContextHelpButtonHint;

    this->setWindowFlags(windowFlags);

    // Setup UI
    ui->setupUi(this);

    // Set labels
    ui->label_ApplicationName->setText(programSettings.application.name);
    ui->label_ApplicationVersion->setText(QString(tr("Version: %1; Date: %2"))
                                           .arg(programSettings.application.version)
                                           .arg(programSettings.application.date));
}

DialogAbout::~DialogAbout()
{
    delete ui;
}
