#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include "structures.h"

//extern "C" {
//    #include "utilities.h"
//}

class TcpServer : public QTcpServer
{
    Q_OBJECT
private:
    QHostAddress _hostAddress;
    quint16 _port;
    QMainWindow* _mainWindow;

    SensorData _sensorData;

public:
    explicit TcpServer(QString ipV4Address, quint16 port, QMainWindow* mainWindow = 0);

    void start();

private:
    void showErrorMessage(const QString& errorMessage);

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void serverStarted(QString ipV4Address, quint16 port);
    void serverError(QString errorMessage);
    void dataSent(SensorData sensorData);

public slots:
    void onSensorDataChanged(SensorData& sensorData);

private slots:
    void onSocketError(QTcpSocket::SocketError socketError);
};

#endif // TCPSERVER_H
