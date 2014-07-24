#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QSettings>
#include <QtWidgets/QMainWindow>
#include <QtNetwork/QTcpSocket>

#include "graphics/cgraphicsscene.h"
#include "graphics/temperatureindicator.h"
#include "sceneeditor/scenedatamodel.h"
#include "utilities/configloader.h"

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

// Miscellaneous methods
public:
    SceneDataModel* getSceneDataModel() const;

    void selectTemperatureIndicator(QPointF point);

// GUI methods
private:
    void listWidget_Scenes_Update();
    void setScene(QString sceneName);
    void showDialogChangeTemperature();
    void updateDialByCurrentTemperatureIndicator();

    // Temperature indicator
    TemperatureIndicator* findTemperatureIndicatorById(quint8 sensorId);
    void setTemperatureIndicator(quint8 sensorId, qreal temperature);
    void setTemperatureSensorDisconnected(quint8 sensorId);

private slots:
    // Automatically connected
    void on_action_About_triggered();

    void on_dial_sliderMoved(int position);
    void on_dial_valueChanged(int value);

    void on_pushButton_ScenesEditor_clicked();
    void on_pushButton_ScenesSet_clicked();

    // Manually connected
    void onListWidgetItemClicked(QListWidgetItem* item);
    void onListWidgetItemDoubleClicked(QListWidgetItem* item);

    void onTemperatureIndicatorDoubleClicked(QGraphicsSceneMouseEvent* event);

// Socket connection methods
private:
    void connectSocket();
    void sendTemperatureTarget(TemperatureIndicator* temperatureIndicator);

private slots:
    void onConnected();
    void onDataRecieved();
    void onDisconnected();
    void onErrorSocket(QAbstractSocket::SocketError socketError);
};

#endif // MAINWINDOW_H
