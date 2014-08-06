#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <QMainWindow>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "../custom/structures.h"

#define ERROR_MESSAGE_01 "Failed to load:"
#define ERROR_MESSAGE_02 " \"config.xml\""


class ConfigLoader
{
private:
    QDomDocument _configDocument;

    ProgramSettings _programSettings;
    QList<IndicatorProperties> _listIndicatorProperties;

public:
    ConfigLoader(QString fileName);

private:
    QDomElement firstChildElement(const QString& tagName, QDomDocument& document) const;
    QDomElement firstChildElement(const QString& tagName, QDomElement& element) const;
    QDomElement nextSiblingElement(const QString& tagName, QDomElement& element) const;
    QString getAttributeValue(const QString& attributeName, QDomElement& element) const;

    void parseProgramSettings(QDomElement& element);
    void parseIndicatorProperties(QDomElement& element);

public:
    ProgramSettings getProgramSettings() const;
    QList<IndicatorProperties> getIndicatorProperties() const;

private:
    void logToConsole(const QDomElement &element) const;
    void logToConsole(const QDomAttr& attribute) const;
    void showFatalError(const QString errorMessage) const;
};

#endif // CONFIGLOADER_H
