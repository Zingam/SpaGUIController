#include "mainwindow.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QtMath>
#include <QtNetwork/QHostAddress>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QMessageBox>

// Forms
#include "ui_mainwindow.h"
#include "forms/dialogabout.h"
#include "forms/dialogtemperaturetarget.h"
#include "sceneeditor/forms/dialogsceneeditor.h"

// Custom constants and structures
#include "custom/constants.h"
#include "custom/structures.h"
#include "custom/types.h"

// Utilities
#include "utilities/utilities.h"

// Temperature indicator
#include "graphics/cgraphicsrectitem.h"
#include "graphics/temperatureindicator.h"


MainWindow::MainWindow(const ConfigLoader& configLoader, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _programSettings(configLoader.getProgramSettings())
{
    bool isOk;

    qDebug() << "Current directory is:" << QDir::currentPath();

    // Program settings: Load program setting from persistant storage
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(_programSettings.application.name);

    _programSettingsPersistant = new QSettings(this);

    // UI: Setup UI
    ui->setupUi(this);

    setWindowTitle(QString("%1 " + tr("Initializing..."))
                   .arg(_programSettings.application.name));

    // UI: Setup indicators display area
    _scene = new CGraphicsScene(this);
    ui->graphicsView->setScene(_scene);

    QString imageFilePath = ASSETS_PATH + _programSettings.backgroundImageFileName;
    QImage imageBackgroundImage(ASSETS_PATH + _programSettings.backgroundImageFileName);
    if (imageBackgroundImage.isNull()) {
        qDebug() << "Background image failed to load: " + imageFilePath;
    }

    ui->graphicsView->setBackgroundBrush(QBrush(QColor(50, 50, 50)));

    QRect sceneRect(0, 0, imageBackgroundImage.width(), imageBackgroundImage.height());
    QGraphicsRectItem* sceneRectItemBackground = _scene->addRect(sceneRect);
    sceneRectItemBackground->setBrush(QBrush(imageBackgroundImage));

    // UI: Load indicators
    _listIndicatorProperties = configLoader.getIndicatorProperties();

    for (IndicatorProperties currentIndicatorProperties: _listIndicatorProperties) {
        TemperatureIndicator* temperatureIndicator =
                new TemperatureIndicator(currentIndicatorProperties,
                                         _programSettings,
                                         ui->listWidget_Sensors,
                                         _scene);
        temperatureIndicator->setPosition(currentIndicatorProperties.position);
        temperatureIndicator->update();

        QString key = QString::number(temperatureIndicator->getSensorId());
        if (_programSettingsPersistant->contains(key))  {
            qreal temperature = _programSettingsPersistant->value(key).toDouble();
            temperatureIndicator->setTemperatureTarget(temperature);
        }
        else {
            temperatureIndicator->setTemperatureTarget(currentIndicatorProperties.temperatureTarget);
            _programSettingsPersistant->setValue(key, temperatureIndicator->getTemperatureTarget());
        }

        _temperatureIndicators.push_back(temperatureIndicator);
    }

    for (auto indicator: _temperatureIndicators) {
        isOk = connect(indicator,
                       SIGNAL(doubleClicked(QGraphicsSceneMouseEvent*)),
                       this,
                       SLOT(onTemperatureIndicatorDoubleClicked(QGraphicsSceneMouseEvent*)));
        Q_ASSERT(isOk);
        Q_UNUSED(isOk);
    }

    _currentTemperatureIndicator = _temperatureIndicators[0];
    _currentTemperatureIndicator->setIndicatorSelected(true);

    updateDialByCurrentTemperatureIndicator();

    // UI: Connect QListWidget signal to TemperatureIndicators
    isOk = connect(ui->listWidget_Sensors,
                   SIGNAL(itemClicked(QListWidgetItem*)),
                   this,
                   SLOT(onListWidgetItemClicked(QListWidgetItem*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    isOk = connect(ui->listWidget_Sensors,
                   SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                   this,
                   SLOT(onListWidgetItemDoubleClicked(QListWidgetItem*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    // Check if data file exists in

    // NOTE: SceneDataModel object has to be crated after all _temperatureIndicators
    // have been loaded from config.xml as SceneDataModel uses getSensors() to
    // retrieve a list of all sensors.
    // All available sensors on the remote controller need to be defined in config.xml
    _sceneDataModel = new SceneDataModel(_programSettings.datafile.path,
                                         _programSettings.datafile.name,
                                         this);

    this->listWidget_Scenes_Update();

    // TcpSocket: Setup
    connectSocket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// PUBLIC: Miscellaneous methods
SceneDataModel* MainWindow::getSceneDataModel() const
{
    return _sceneDataModel;
} // getSceneDataModel()

void MainWindow::selectTemperatureIndicator(QPointF point)
{
    QList<QGraphicsItem*> items = _scene->items(point.x(),
                                                point.y(),
                                                1,
                                                1,
                                                Qt::IntersectsItemBoundingRect,
                                                Qt::AscendingOrder);

    QListIterator<QGraphicsItem*> iteratorItems(items);
    QGraphicsItem* item;
    CGraphicsRectItem* rectItem;

    while(iteratorItems.hasNext())
    {
        item = qgraphicsitem_cast<QGraphicsItem*>(iteratorItems.next());

        if (CGraphicsRectItem::Type == item->type()) {
            rectItem = qgraphicsitem_cast<CGraphicsRectItem*>(item);
#ifdef DEBUG_MODE_FINETUNING
            qDebug() << "Clicked CGraphicsRectItem at: "
                        + QString::number(rectItem->x()) + ", "
                        + QString::number(rectItem->y());
#endif // DEBUG_MODE_FINETUNING
            break;
        }
    }

    for (TemperatureIndicator* indicator: _temperatureIndicators) {
        if (indicator->getGraphicsRectItem() == rectItem) {
#ifdef DEBUG_MODE_FINETUNING
            qDebug() << "Clicked on indicator:" << indicator->getText();
#endif // DEBUG_MODE_FINETUNING
            _currentTemperatureIndicator->setIndicatorSelected(false);
            _currentTemperatureIndicator = indicator;
            _currentTemperatureIndicator->setIndicatorSelected(true);
        }
    }

    updateDialByCurrentTemperatureIndicator();
} // selectTemperatureIndicator()

// PRIVATE: GUI methods
void MainWindow::listWidget_Scenes_Update()
{
    ui->listWidget_Scenes->clear();
    for (Scene currentScene: this->getSceneDataModel()->_scenes) {
        QListWidgetItem* currentSceneName = new QListWidgetItem(currentScene.name);
        ui->listWidget_Scenes->addItem(currentSceneName);
    }
} // listWidget_Scenes_Update()

void MainWindow::setScene(QString sceneName)
{
    Scene* scene = _sceneDataModel->getSceneByName(sceneName);
    if (nullptr == scene) {
        qDebug() << "Scene not found in setScene(" << sceneName << ")";

        return;
    }

    for (int sensorIndex = 0;
         sensorIndex < scene->sensors.count();
         sensorIndex ++) {
        qreal temperatureTarget = scene->sensors.at(sensorIndex).temperatureTarget;
        quint8 sensorId = scene->sensors.at(sensorIndex).sensorId;
        TemperatureIndicator* indicator = findTemperatureIndicatorById(sensorId);
        indicator->setTemperatureTarget(temperatureTarget);
        sendTemperatureTarget(indicator);
    }
} // setScene()

void MainWindow::showDialogChangeTemperature()
{
    QString dialogTitle = _currentTemperatureIndicator->getText();
    quint8 sensorId = _currentTemperatureIndicator->getSensorId();
    bool temperatureIndicatorFunctional = _currentTemperatureIndicator->isSensorFunctional();
    qreal temperatureTarget = _currentTemperatureIndicator->getTemperatureTarget();
    qreal temperatureCurrent = _currentTemperatureIndicator->getTemperatureCurrent();

    DialogTemperatureTarget dialogChangeTemperature(this,
                                                    dialogTitle,
                                                    sensorId,
                                                    temperatureIndicatorFunctional,
                                                    temperatureTarget,
                                                    temperatureCurrent);

    if(dialogChangeTemperature.exec()) {
        qreal temperature = dialogChangeTemperature.getValue();
        _currentTemperatureIndicator->setTemperatureTarget(temperature);

        QString key = QString::number(_currentTemperatureIndicator->getSensorId());
        _programSettingsPersistant->setValue(key, temperature);

        sendTemperatureTarget(_currentTemperatureIndicator);

        updateDialByCurrentTemperatureIndicator();
    }
} // showDialogChangeTemperature()

void MainWindow::updateDialByCurrentTemperatureIndicator()
{
    if (_currentTemperatureIndicator) {
        qreal temperature = _currentTemperatureIndicator->getTemperatureTarget();
        ui->lcdNumber->display(QString::number(temperature, 'f',1));
        ui->dial->setValue(_currentTemperatureIndicator->getTemperatureTarget() * 10);
    }
} // updateDialByCurrentIndicator()

TemperatureIndicator* MainWindow::findTemperatureIndicatorById(quint8 sensorId)
{
    for (TemperatureIndicator* currentTemperatureIndicator: _temperatureIndicators) {
        if (currentTemperatureIndicator->getSensorId() == sensorId) {
            return currentTemperatureIndicator;
        }
    }
    return nullptr;
} // findTemperatureIndicatorById()

void MainWindow::setTemperatureIndicator(quint8 sensorId, qreal temperature)
{
    TemperatureIndicator* currentTemperatureIndicator = findTemperatureIndicatorById(sensorId);
    if (currentTemperatureIndicator) {
        currentTemperatureIndicator->setTemperatureCurrent(temperature);
        currentTemperatureIndicator->setSensorError(SensorError::OK);
    }
} // setTemperatureIndicator()

void MainWindow::setTemperatureSensorDisconnected(quint8 sensorId)
{
    TemperatureIndicator* currentTemperatureIndicator = findTemperatureIndicatorById(sensorId);
    if (currentTemperatureIndicator) {
        currentTemperatureIndicator->setSensorError(SensorError::Disconnected);   
    }
} // setTemperatureSensorDisconnected()


// PRIVATE: SLOTS
// Automatically connected
void MainWindow::on_action_About_triggered()
{
    DialogAbout dialogAbout(_programSettings);

    dialogAbout.exec();
} // on_action_About_triggered()

void MainWindow::on_action_Export_Scenes_triggered()
{
    qDebug() << "Exporting scenes";

    QFileDialog fileDialog;
    fileDialog.setWindowTitle(tr("Export Scenes"));
    fileDialog.show();
} // on_action_Export_Scenes_triggered()

void MainWindow::on_action_Import_Scenes_triggered()
{
    qDebug() << "Importing scenes";

    qDebug() << "Import scenes";
    QFileDialog fileDialog;
    fileDialog.setWindowTitle(tr("Import Scenes"));
    fileDialog.show();
} // on_action_Import_Scenes_triggered()

void MainWindow::on_dial_sliderMoved(int position)
{
    qreal value = static_cast<qreal>(position * 0.1);
    ui->lcdNumber->display( QString::number(value, 'f', 1) );
} // on_dial_sliderMoved()

void MainWindow::on_dial_valueChanged(int value)
{
    if (nullptr == _currentTemperatureIndicator) {
        return;   
    }

    qreal valueReal = static_cast<qreal>(value * 0.1);

    _currentTemperatureIndicator->setTemperatureTarget(valueReal);
    sendTemperatureTarget(_currentTemperatureIndicator);
} // on_dial_valueChanged

void MainWindow::on_pushButton_ScenesEditor_clicked()
{
    DialogSceneEditor dialogSceneEditor(this);

    dialogSceneEditor.exec();

    this->listWidget_Scenes_Update();
} // on_pushButton_ScenesEditor_clicked()

void MainWindow::on_pushButton_ScenesSet_clicked()
{
    QListWidgetItem* listWidgetItem = ui->listWidget_Scenes->currentItem();  
    if (nullptr == listWidgetItem) {
        return;
    }

    QString sceneName = listWidgetItem->text();
    QString message = tr("Do you want to activate:") + "\n" + sceneName;

    int answer = QMessageBox::question(this,
                                       sceneName,
                                       message,
                                       QMessageBox::Ok,
                                       QMessageBox::Cancel);
    if (QMessageBox::Ok == answer)
    {
        setScene(sceneName);
    }
} // on_pushButton_ScenesSet_clicked()

// Manually connected
void MainWindow::onListWidgetItemClicked(QListWidgetItem *item)
{
    QString text = item->text();
    qDebug() << "Clicked QListWidgetItem: " << text;

    // Match the clicked QListWidgetItem to the corresponding TemperatureIndicator
    for (auto indicator: _temperatureIndicators) {
        if (indicator->getText() == text) {
            _currentTemperatureIndicator->setIndicatorSelected(false);
            _currentTemperatureIndicator = indicator;
            _currentTemperatureIndicator->setIndicatorSelected(true);
        }
    }

    updateDialByCurrentTemperatureIndicator();
} // onListWidgetItemClicked()

void MainWindow::onListWidgetItemDoubleClicked(QListWidgetItem* item)
{
    Q_UNUSED(item);

    if(nullptr == _currentTemperatureIndicator) {
        return;
    }

    showDialogChangeTemperature();
} // onListWidgetItemDoubleClicked()

void MainWindow::onTemperatureIndicatorDoubleClicked(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (nullptr == _currentTemperatureIndicator) {
        return;
    }

    showDialogChangeTemperature();
} // onTemperatureIndicatorDoubleClicked

// PRIVATE: Socket connection methods
void MainWindow::connectSocket()
{
    Q_ASSERT(nullptr == _socket);

    _socket = new QTcpSocket();

    _socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    // TcpSocket: Connect signals and slots
    bool isOk;

    isOk = connect(_socket, SIGNAL(readyRead()), this, SLOT(onDataRecieved()));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    isOk = connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    isOk = connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    isOk = connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(onErrorSocket(QAbstractSocket::SocketError)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    _socket->connectToHost(_programSettings.server.ipV4Address,
                           _programSettings.server.port);
    setWindowTitle(QString("%1 " + tr("Connecting...") + " %2:%3")
                   .arg(_programSettings.application.name)
                   .arg(_programSettings.server.ipV4Address)
                   .arg(_programSettings.server.port));
}

void MainWindow::sendTemperatureTarget(TemperatureIndicator* temperatureIndicator)
{
    if (!_socket) {
        return;
    }

    if (_socket->isValid()) {
        qreal temperatureTarget = temperatureIndicator->getTemperatureTarget();
        qint8 integralPart = qFloor(temperatureTarget);
        quint8 fractionalPart = (temperatureTarget - integralPart) * FRACTIONAL_BASE;

        QByteArray commandOutgoing;
        commandOutgoing.append(QChar(COMMAND_SET).toUpper().toLatin1());
        commandOutgoing.append(static_cast<char>(temperatureIndicator->getSensorId()));
        commandOutgoing.append(static_cast<char>(integralPart));
        commandOutgoing.append(static_cast<char>(fractionalPart));

        _socket->write(commandOutgoing);
    }
}

// PRIVATE: SLOTS
void MainWindow::onConnected()
{
    qDebug() << "Connected!";
    qDebug() << "Local address:" << QString("%1:%2").arg(_socket->localAddress().toString())
                                            .arg(_socket->localPort());
    qDebug() << "Peer address:" << QString("%1:%2").arg(_socket->peerAddress().toString())
                                           .arg(_socket->localPort());

    _isConnected = true;

    setWindowTitle(_programSettings.application.name);

    // Send all target temperatures on connect
    for (TemperatureIndicator* currentTemperatureIndicator: _temperatureIndicators) {
        sendTemperatureTarget(currentTemperatureIndicator);
    }
} // onConnected()

void MainWindow::onDataRecieved()
{
    QByteArray dataIncomming = _socket->read(4);

    while( dataIncomming.size() > 0 ) {
        char byte00_Command = QChar(dataIncomming[0]).toUpper().toLatin1();
        quint8 byte01_SensorId = static_cast<quint8>(dataIncomming[1]);
        qint8 byte02 = static_cast<qint8>(dataIncomming[2]);
        quint8 byte03 = static_cast<quint8>(dataIncomming[3]);

        qDebug()<< QString("[%1]").arg(QTime::currentTime().toString())
                << byte00_Command
                << byte01_SensorId
                << byte02
                << byte03;

        switch(byte00_Command) {
        case COMMAND_SET:
        {
            qreal integralPart = static_cast<qreal>(byte02);
            qreal fractionalPart = static_cast<qreal>(byte03) / FRACTIONAL_BASE;
            qreal temperature;
            temperature = integralPart + fractionalPart;

            setTemperatureIndicator(byte01_SensorId, temperature);

            break;
        }
        case COMMAND_ERROR:
        {
            // on error read additional 8 bytes of sensor's mac id
            setTemperatureSensorDisconnected(byte01_SensorId);
            QByteArray mac = _socket->read(8);
            QString mac_hex = QString::fromLatin1(mac.toHex());

            qDebug() << "0x" << mac_hex;

            break;
        }
        default:
            qDebug()<< "Error: Unknown command byte!";

            break;
        }

        dataIncomming = _socket->read(4);
    }
} // onDataRecieved()

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected!";

    delete_safe(_socket);

   _isConnected = false;

   for (TemperatureIndicator* currentTemperatureIndicator: _temperatureIndicators) {
       currentTemperatureIndicator->setSensorError(SensorError::Undefined);
   }

   connectSocket();
} // onDisconnected

void MainWindow::onErrorSocket(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);

    _isConnected = false;

    qDebug() << "Error connecting:" << _socket->errorString();

    QMessageBox::critical(this,
                          tr("Connection error"),
                          tr("Connection error: ") + _socket->errorString()
                          + "\n\n"
                          + tr("Please restart the application!"),
                          QMessageBox::Close);

    this->close();
} // onErrorSocket()


