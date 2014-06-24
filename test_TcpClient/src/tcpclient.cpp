#include "tcpclient.h"

#include <QMessageBox>
#include <QString>
#include <QtMath>

TcpClient::TcpClient(QString ipV4Address,
                     quint16 port,
                     QList<quint8> sensorIds,
                     quint32 queryInterval,
                     QMainWindow* mainWindow) :
    QObject(mainWindow)
{
    _ipV4Address.setAddress(ipV4Address);
    _port = port;
    _sensorIds = sensorIds;
    _queryInterval = queryInterval;
    _mainWindow = mainWindow;

    bool isOk;
    isOk = connect(&_timer, SIGNAL(timeout()),
                   this, SLOT(requestData()));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    isOk = connect(&_socket, SIGNAL(readyRead()),
                   this, SLOT(readData()));
    Q_ASSERT(isOk);
    Q_UNUSED(isOk);

    isOk = connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(displaySocketError(QAbstractSocket::SocketError)));
}

void TcpClient::start()
{
    qDebug() << "Timer";
    _timer.start(_queryInterval);
}

void TcpClient::showErrorMessage(const QString& errorMessage)
{
    QMessageBox messageBox(_mainWindow);
    messageBox.setWindowTitle("Connection error");
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setText(errorMessage);
    messageBox.setStandardButtons(QMessageBox::Abort);

    messageBox.exec(); // Show modal

    //exit(-1);
}

void TcpClient::requestData()
{
    qDebug() << "Connecting to host...";

    if ( _sensorIds.count() - 1 < _currentSensorIdIndex) {
        _currentSensorIdIndex = 0;
    }

    QByteArray outgoingCommandBlock;
    outgoingCommandBlock.append('g');
    outgoingCommandBlock.append(static_cast<char>(_sensorIds.at(_currentSensorIdIndex)));
    outgoingCommandBlock.append(static_cast<char>(0));
    outgoingCommandBlock.append(static_cast<char>(0));

    qDebug() << outgoingCommandBlock.at(0)
                << static_cast<quint8>(outgoingCommandBlock.at(1))
                    << static_cast<qint8>(outgoingCommandBlock.at(2))
                       << static_cast<quint8>(outgoingCommandBlock.at(3));

    _socket.abort();
    _socket.connectToHost(_ipV4Address, _port, QIODevice::WriteOnly);
    _socket.write(outgoingCommandBlock);

    _currentSensorIdIndex++;
}

void TcpClient::setData(quint8 sensorId, qreal temperatureDesired)
{
    Q_UNUSED(sensorId);
    Q_UNUSED(temperatureDesired);

    int integerPart = qFloor(temperatureDesired);
    int fractionalPart = (temperatureDesired - integerPart) * 100;

    QByteArray outgoingCommandBlock;

    outgoingCommandBlock.append('s');
    outgoingCommandBlock.append(static_cast<char>(sensorId));
    outgoingCommandBlock.append(static_cast<char>(integerPart));
    outgoingCommandBlock.append(static_cast<char>(fractionalPart));

    qDebug() << outgoingCommandBlock.at(0)
                << static_cast<quint8>(outgoingCommandBlock.at(1))
                    << static_cast<qint8>(outgoingCommandBlock.at(2))
                       << static_cast<quint8>(outgoingCommandBlock.at(3));

    _socket.abort();
    _socket.connectToHost(_ipV4Address, _port, QIODevice::WriteOnly);
    _socket.write(outgoingCommandBlock);
}

void TcpClient::readData()
{
    QByteArray incommingDataBlock;
    incommingDataBlock = _socket.read(4);

    SensorData sensorData;
    sensorData.byte01 = static_cast<char>(incommingDataBlock.at(0));
    sensorData.byte02 = static_cast<quint8>(incommingDataBlock.at(1));
    sensorData.byte03 = static_cast<qint8>(incommingDataBlock.at(2));
    sensorData.byte03 = static_cast<quint8>(incommingDataBlock.at(3));

    quint8 sensorId = sensorData.byte02;
    qreal temperatureDesired = sensorData.byte03  + (sensorData.byte04 / 100);

    emit dataRecieved(sensorId, temperatureDesired);
}

void TcpClient::displaySocketError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        showErrorMessage(QString("The host was not found")
                         + "please check the host name"
                         + "and port settings.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        showErrorMessage(QString("The connection was refused by the peer.")
                         + "Make sure the server is running,"
                         + "and check that the host name and port"
                         + "settings are correct.");
        break;
    default:
        QString errorMessage("The following error occured:");
        errorMessage.append(_socket.errorString());
        showErrorMessage(QString());
    }
}

