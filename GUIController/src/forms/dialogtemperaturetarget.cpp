#include "dialogtemperaturetarget.h"
#include "ui_dialogtemperaturetarget.h"


DialogTemperatureTarget::DialogTemperatureTarget(QWidget* parent,
                                                 QString dialogTitle,
                                                 quint8 sensorId,
                                                 bool temperatureIndicatorFunctional,
                                                 qreal temperatureTarget,
                                                 qreal temperatureCurrent) :
    QDialog(parent),
    ui(new Ui::DialogTemperatureTarget)
{
    // Set dialog title
    this->setWindowTitle(dialogTitle);

    // Remove ? from TitleBar
    Qt::WindowFlags windowFlags = this->windowFlags();
    windowFlags &= ~Qt::WindowContextHelpButtonHint;

    this->setWindowFlags(windowFlags);

    // Setup UI
    ui->setupUi(this);

    _sensorId = sensorId;

    QString temperature("");
    if (temperatureIndicatorFunctional) {
        temperature += QString("%1° /").arg(temperatureCurrent, 1, 'f', 1);
    }
    else {
        temperature += QString("?° /");
    }

    ui->label_TemperatureCurrent->setText(temperature);
    ui->spinBox_TemperatureTarget->setValue(temperatureTarget);
}

DialogTemperatureTarget::~DialogTemperatureTarget()
{
    delete ui;
}

qreal DialogTemperatureTarget::getValue()
{
    return ui->spinBox_TemperatureTarget->value();
}


