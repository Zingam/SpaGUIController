#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork/QTcpSocket>

#include "utilities/configloader.h"
#include "graphics/cgraphicsscene.h"
#include "graphics/temperatureindicator.h"
#include "sceneeditor/scenedatamodel.h"

namespace Ui {
    class MainWindow;
}

class CGraphicsScene;
class SceneDataModel;
class TemperatureIndicator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class SceneDataModel;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

    ProgramSettings _programSettings;
    QSettings* _programSettingsPersistant;
    QList<IndicatorProperties> _listIndicatorProperties;

    CGraphicsScene* _scene;
    QList<TemperatureIndicator*> _temperatureIndicators;
    TemperatureIndicator* _currentTemperatureIndicator;

    SceneDataModel* _sceneDataModel;

    QTcpSocket* _socket = nullptr;
    bool _isConnected = false;

// GUI
private:
    void showDialogChangeTemperature();

public:
    void selectTemperatureIndicator(QPointF point);

    SceneDataModel *getSceneDataModel() const;

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
