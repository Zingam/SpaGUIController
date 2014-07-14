#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork/QTcpSocket>

#include "configloader.h"
#include "cgraphicsscene.h"
#include "temperatureindicator.h"

namespace Ui {
    class MainWindow;
}

class CGraphicsScene;
class TemperatureIndicator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow* ui;

    ProgramSettings _programSettings;
    QSettings* _programSettingsPersistant;
    QList<IndicatorProperties> _listIndicatorProperties;

    CGraphicsScene* _scene;
    QList<TemperatureIndicator*> _temperatureIndicators;
    TemperatureIndicator* _currentTemperatureIndicator;

    QTcpSocket* _socket = nullptr; 
    bool _isConnected = false;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

// GUI
private:
    void showDialogChangeTemperature();

public:
    void selectTemperatureIndicator(QPointF point);
    
private slots:
    void on_action_About_triggered();
    void onListWidgetItemClicked(QListWidgetItem* item);
    void onListWidgetItemDoubleClicked(QListWidgetItem* item);
    void on_pushButton_ScenesEditor_clicked();
    void on_pushButton_ScenesSet_clicked();
    void onTemperatureIndicatorDoubleClicked(QGraphicsSceneMouseEvent* event);

// Socket connection
private:
    void connectSocket();
    void sendTemperatureTarget(TemperatureIndicator* temperatureIndicator);
    void setTemperatureIndicator(quint8 sensorId, qreal temperature);

    TemperatureIndicator *findTemperatureIndicatorById(quint8 sensorId);
    void setTemperatureSensorDisconnected(quint8 sensorId);

private slots:
    void onDataRecieved();
    void onDisconnected();
    void onConnected();
    void onErrorSocket(QAbstractSocket::SocketError socketError);
};

#endif // MAINWINDOW_H
