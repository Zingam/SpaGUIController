#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>
#include <QPointF>
#include <QFont>


struct ProgramSettings
{
    struct DataFile
    {
        QString name;
        QString path;
    } datafile;

    struct Server
    {
        QString ipV4Address;
        int port;
    } server;

    struct Application
    {
        QString date;
        QString name;
        QString version;
    } application;


    QString language;
    QString assetsPath;
    QString backgroundImageFileName;
    qreal highlightOpacity;

    struct IndicatorProperties
    {
        qreal opacity;
        int borderwidth;
        QString bordercolor;
        QString fontFamily;
        int     fontSize;
        QString fontWeight;
        QString fontColor;
        QFont   font;
        qreal rectPadding;
        int alphaSelected;
        int alphaDisselected;
    } indicator;
};

struct IndicatorProperties
{
    quint8 sensorId;
    QString text;
    QString highlightImageFileName;
    QPointF position;
    qreal temperatureTarget;
};

struct Sensor {
    QString sceneName;
    quint8 sensorId;
    QString text;
    qreal temperatureTarget;
};

struct Scene {
    QString name;
    QList<Sensor> sensors;
};

#endif // STRUCTURES_H
