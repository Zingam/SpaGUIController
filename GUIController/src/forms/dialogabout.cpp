#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QtCore/QDebug>
#include <QtCore/QString>


DialogAbout::DialogAbout(const ProgramSettings& programSettings,
                         QPixmap& pixmapLogo,
                         QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    // Remove ? from TitleBar
    Qt::WindowFlags windowFlags = this->windowFlags();
    windowFlags &= ~Qt::WindowContextHelpButtonHint;

    this->setWindowFlags(windowFlags);

    // Setup UI
    ui->setupUi(this);

    // Setup logo
    if (!pixmapLogo.isNull()) {
        this->resizePixmapLogo(pixmapLogo);
        ui->label_Logo->setPixmap(pixmapLogo);
    }

    // Set labels
    ui->label_ApplicationName->setText(programSettings.application.name);
    ui->label_ApplicationVersion->setText(QString(tr("Version:") + " %1; "
                                                  + tr("Date:") + " %2")
                                          .arg(programSettings.application.version)
                                          .arg(programSettings.application.date));
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::resizePixmapLogo(QPixmap& pixmap)
{
    qDebug() << "Resizing logo:";
    quint32 labelWidth = ui->label_Logo->width();
    quint32 labelHeight = ui->label_Logo->height();
    qDebug() << "labelWidth:" << labelWidth << ", labelHeight:" << labelHeight;

    quint32 pixmapWidth = pixmap.width();
    quint32 pixmapHeight = pixmap.height();
    qDebug() << "pixmapWidth:" << pixmapWidth << ", pixmapHeight:" << pixmapHeight;

    quint32 newWidth = 0;
    quint32 newHeight = 0;

    if (labelWidth < pixmapWidth) {
        qreal ratio = static_cast<qreal>(labelWidth) / pixmapWidth;
        qDebug() << "Width ratio:" << ratio;

        newWidth = labelWidth;
        newHeight = pixmapHeight * ratio;
    }
    else {
        newWidth = pixmapWidth;
        newHeight = pixmapHeight;
    }

    if (labelHeight < newHeight) {
        qreal ratio = static_cast<qreal>(labelHeight) / newHeight;
        qDebug() << "Height ratio:" << ratio;

        newWidth = newWidth * ratio;
        newHeight = labelHeight;
    }

    qDebug() << "newWidth:" << newWidth << ", newHeight:" << newHeight;

    pixmap =  pixmap.scaled(newWidth, newHeight);
}
