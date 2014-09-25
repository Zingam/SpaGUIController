#include "xmlloader.h"

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSpacerItem>


XmlLoader::XmlLoader(const QString& errorMessage, QMainWindow* mainWindow) :
    _errorMessage(errorMessage),
    _mainWindow(mainWindow)
{
}

/*
 * Helper methods
 */

///
/// \brief ConfigLoader::firstChildElement
/// \param tagName
/// \param document
/// \return
///
///  Helper function to parse the first Child element
///
QDomElement XmlLoader::firstChildElement(const QString& tagName, QDomDocument& document) const
{
    QDomElement childElement = document.firstChildElement(tagName);
    if (childElement.isNull()) {
        this->showFatalError(QObject::tr("Failed to load") + " <"+ tagName + ">");
    }
    this->logToConsole(childElement);

    return childElement;
}

///
/// \brief ConfigLoader::firstChildElement
/// \param tagName
/// \param element
/// \return
///
///  Helper function to parse the first Child element
///
QDomElement XmlLoader::firstChildElement(const QString& tagName, QDomElement& element) const
{
    QDomElement childElement = element.firstChildElement(tagName);
    if (childElement.isNull()) {
        this->showFatalError(QObject::tr("Failed to load") + " <"+ tagName + ">");
    }
    this->logToConsole(childElement);

    return childElement;
}

///
/// \brief ConfigLoader::nextSiblingElement
/// \param tagName
/// \param element
/// \return
///
/// Helper function to parse the next Sibling element
///
QDomElement XmlLoader::nextSiblingElement(const QString& tagName, QDomElement& element) const
{
    QDomElement childElement = element.nextSiblingElement(tagName);
    if (childElement.isNull()) {
        this->showFatalError(QObject::tr("Failed to load") + " <"+ tagName + ">");
    }
    this->logToConsole(childElement);

    return childElement;
}

QString XmlLoader::getAttributeValue(const QString& attributeName, QDomElement& element) const
{
    QDomAttr attribute = element.attributeNode(attributeName);
    if(attribute.isNull()) {
        this->showFatalError(QObject::tr("No valid attribute")
                             + " <" + attributeName + "> "
                             + QObject::tr("found for")
                             + " <" + element.tagName() + ">");
    }
    this->logToConsole(attribute);

    return attribute.value();
}


/*
 * Logging & error reporting methods
 */

///
/// \brief ConfigLoader::logToConsole
/// \param element
///
/// Logs a message to the console
///
void XmlLoader::logToConsole(const QDomElement& element) const
{
    Q_UNUSED(element)

#ifdef DEBUG_OUTPUT
    qDebug() << "Loaded element:" << element.tagName();
#endif
}

///
/// \brief ConfigLoader::logToConsole
/// \param element
///
/// Logs a message to the console
///
void XmlLoader::logToConsole(const QDomAttr& attribute) const
{
    Q_UNUSED(attribute)

#ifdef DEBUG_OUTPUT
    qDebug() << "Loaded attribute: " + attribute.name() + "=\"" + attribute.value() +"\"";
#endif
}

///
/// \brief ConfigLoader::showFatalError
/// \param errorMessage
///
/// Displays an error message and quits the application
///
void XmlLoader::showFatalError(const QString errorMessage) const
{
    qDebug() << errorMessage;

    QMessageBox messageBox(_mainWindow);
    messageBox.setWindowTitle(QObject::tr("ERROR"));
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setText(_errorMessage);
    messageBox.setInformativeText(errorMessage);

    // MessageBox size hack
    QSpacerItem* horizontalSpacer = new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = qobject_cast<QGridLayout*>(messageBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    messageBox.exec(); // Show modal
}
