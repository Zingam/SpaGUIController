#include <QString>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QtMath>
#include <QTime>

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "forms/dialogabout.h"
#include "forms/dialogtemperaturetarget.h"
#include "sceneeditor/forms/dialogsceneeditor.h"

#include "custom/constants.h"
#include "custom/types.h"
#include "custom/structures.h"
#include "graphics/cgraphicsrectitem.h"
#include "graphics/temperatureindicator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    bool isOk;

    // Program settings: Load program settings from "assets/config.xml"
    ConfigLoader configLoader(CONFIG_FILE, this);
    _programSettings = configLoader.getProgramSettings();

    // Program settings: Load program setting from persistant storage
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(_programSettings.application.name);

    _programSettingsPersistant = new QSettings(this);

    // UI: Setup UI
    ui->setupUi(this);

    setWindowTitle(QString("%1 Initializing...")
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
                                         ui->listWidget,
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

    for(auto indicator: _temperatureIndicators) {
        isOk = connect(indicator,
                       SIGNAL(doubleClicked(QGraphicsSceneMouseEvent*)),
                       this,
                       SLOT(onTemperatureIndicatorDoubleClicked(QGraphicsSceneMouseEvent*)));
        Q_ASSERT(isOk);
        Q_UNUSED(isOk);
    }

    _currentTemperatureIndicator = _temperatureIndicators[0];
    _currentTemperatureIndicator->setIndicatorSelected(true);

    // UI: Connect QListWidget signal to TemperatureIndicators
    isOk = connect(ui->listWidget,
                   SIGNAL(itemClicked(QListWidgetItem*)),
                   this,
                   SLOT(onListWidgetItemClicked(QListWidgetItem*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    isOk = connect(ui->listWidget,
                   SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                   this,
                   SLOT(onListWidgetItemDoubleClicked(QListWidgetItem*)));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

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

    // TcpSocket: Connect signals and slots
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    }
}

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

            qDebug() << "Clicked CGraphicsRectItem at: "
                        + QString::number(rectItem->x()) + ", "
                        + QString::number(rectItem->x());
            break;
        }
    }

    for (TemperatureIndicator* indicator: _temperatureIndicators) {
        if (indicator->getGraphicsRectItem() == rectItem) {
            qDebug() << "Clicked on the indicator:" << indicator->getText();
            _currentTemperatureIndicator->setIndicatorSelected(false);
            _currentTemperatureIndicator = indicator;
            _currentTemperatureIndicator->setIndicatorSelected(true);
        }
    }
}

SceneDataModel *MainWindow::getSceneDataModel() const
{
    return _sceneDataModel;
}


void MainWindow::on_action_About_triggered()
{
    DialogAbout dialogAbout(_programSettings);
    dialogAbout.exec();
}

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
}

void MainWindow::onListWidgetItemDoubleClicked(QListWidgetItem* item)
{
    Q_UNUSED(item);

    if(nullptr == _currentTemperatureIndicator) {
        return;
    }

    showDialogChangeTemperature();
}

void MainWindow::on_pushButton_ScenesEditor_clicked()
{
    DialogSceneEditor dialogSceneEditor(this);

    dialogSceneEditor.exec();

    this->listWidget_Scenes_Update();
}

void MainWindow::on_pushButton_ScenesSet_clicked()
{
    QListWidgetItem* listWidgetItem = ui->listWidget_Scenes->currentItem();
    QString sceneName = listWidgetItem->text();
    if (QMessageBox::Ok == QMessageBox::question(this,
                                                 QString("Set: %1").arg(sceneName),
                                                 "Are you sure?",
                                                 QMessageBox::Ok,
                                                 QMessageBox::Cancel))
    {

    }
}

void MainWindow::onTemperatureIndicatorDoubleClicked(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if(nullptr == _currentTemperatureIndicator) {
        return;
    }

    showDialogChangeTemperature();
}

// Connection
void MainWindow::connectSocket()
{
    Q_ASSERT(nullptr == _socket);

    _socket = new QTcpSocket();
    _socket->connectToHost(_programSettings.server.ipV4Address,
                           _programSettings.server.port);
    setWindowTitle(QString("%1 Connecting...").arg(_programSettings.application.name));
}

void MainWindow::listWidget_Scenes_Update()
{
    for (Scene currentScene: this->getSceneDataModel()->_scenes) {
        QListWidgetItem* currentSceneName = new QListWidgetItem(currentScene.name);
        ui->listWidget_Scenes->addItem(currentSceneName);
    }
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

TemperatureIndicator* MainWindow::findTemperatureIndicatorById(quint8 sensorId)
{
    for (TemperatureIndicator* currentTemperatureIndicator: _temperatureIndicators)
    {
        if (currentTemperatureIndicator->getSensorId() == sensorId)
        {
            return currentTemperatureIndicator;
        }
    }
    return nullptr;
}

void MainWindow::setTemperatureIndicator(quint8 sensorId, qreal temperature)
{
    TemperatureIndicator* currentTemperatureIndicator = findTemperatureIndicatorById(sensorId);
    if( currentTemperatureIndicator )
    {
        currentTemperatureIndicator->setTemperatureCurrent(temperature);
        currentTemperatureIndicator->setSensorError(SensorError::OK);
    }
}

void MainWindow::setTemperatureSensorDisconnected(quint8 sensorId)
{
    TemperatureIndicator* currentTemperatureIndicator = findTemperatureIndicatorById(sensorId);
    if( currentTemperatureIndicator )
        currentTemperatureIndicator->setSensorError(SensorError::Disconnected);
}

// Private slots
void MainWindow::onDataRecieved()
{
    QByteArray dataIncomming = _socket->read(4);

    while( dataIncomming.size() > 0 )
    {
        char byte00_Command = QChar(dataIncomming[0]).toUpper().toLatin1();
        quint8 byte01_SensorId = static_cast<quint8>(dataIncomming[1]);
        qint8 byte02 = static_cast<qint8>(dataIncomming[2]);
        quint8 byte03 = static_cast<quint8>(dataIncomming[3]);

        qDebug()<< QString("[%1]").arg(QTime::currentTime().toString())
                << byte00_Command
                << byte01_SensorId
                << byte02
                << byte03;

        switch(byte00_Command)
        {
        case COMMAND_SET:
        {
            qreal integralPart = static_cast<qreal>(byte02);
            qreal fractionalPart = static_cast<qreal>(byte03) / FRACTIONAL_BASE;
            qreal temperature;
            temperature = integralPart + fractionalPart;

            setTemperatureIndicator(byte01_SensorId, temperature);
        }
            break;
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
}

void MainWindow::onConnected()
{
    qDebug()<< "Connected!";
    qDebug() << "Local address" << QString("%1:%2").arg(_socket->localAddress().toString())
                                           .arg(_socket->localPort());
    qDebug() << "Peer address:" << QString("%1:%2").arg(_socket->peerAddress().toString())
                                           .arg(_socket->localPort());

    _isConnected = true;
    setWindowTitle(_programSettings.application.name);

    // Send all target temperatures on connect
    for (TemperatureIndicator* currentTemperatureIndicator: _temperatureIndicators) {
        sendTemperatureTarget(currentTemperatureIndicator);
    }
}

void MainWindow::onErrorSocket(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);

    _isConnected = false;

    qDebug()<< "Error connecting: " << _socket->errorString();

    QMessageBox::critical(this,
                          "Connection error",
                          "Connection error: " + _socket->errorString()
                          + "\n\nPlease restart the application",
                          QMessageBox::Close);

    this->close();
}

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected!";

    delete _socket;
    _socket = nullptr;

   _isConnected = false;
   for (TemperatureIndicator* currentTemperatureIndicator: _temperatureIndicators) {
       currentTemperatureIndicator->setSensorError(SensorError::Undefined);
   }

   connectSocket();
}
