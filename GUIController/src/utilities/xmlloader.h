#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <QtCore/QString>
#include <QtWidgets/QMainWindow>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class XmlLoader
{
public:
    XmlLoader(const QString& errorMessage, QMainWindow* mainWindow);

private:
    QString _errorMessage;
    QMainWindow* _mainWindow;

protected:
    QDomElement firstChildElement(const QString& tagName, QDomDocument& document) const;
    QDomElement firstChildElement(const QString& tagName, QDomElement& element) const;
    QDomElement nextSiblingElement(const QString& tagName, QDomElement& element) const;
    QString getAttributeValue(const QString& attributeName, QDomElement& element) const;

    void logToConsole(const QDomElement &element) const;
    void logToConsole(const QDomAttr& attribute) const;
    void showFatalError(const QString errorMessage) const;
};

#endif // XMLLOADER_H
